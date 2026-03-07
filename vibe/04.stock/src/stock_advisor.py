import time
from cache import get_cached_quotes, get_cached_analysis, is_cache_fresh
from data_source import get_all_quotes, fetch_kline
from indicator import analyze

KEYWORD_TAGS = {
    "红利": "红利", "高分红": "红利", "高股息": "红利", "分红": "红利",
    "低估值": "低估值", "低估": "低估值", "便宜": "低估值",
    "银行": "银行", "白酒": "白酒", "医药": "医药", "科技": "科技",
    "新能源": "新能源", "光伏": "新能源", "锂电": "新能源", "电池": "新能源",
    "半导体": "半导体", "芯片": "半导体",
    "消费": "消费", "金融": "金融", "保险": "保险", "券商": "券商",
    "煤炭": "煤炭", "石油": "石油", "电力": "电力", "家电": "家电",
}

SORT_KEYWORDS = {
    "强势": {"sort": "change", "ascending": False, "filter": lambda s: s["change"] > 2},
    "活跃": {"sort": "amount", "ascending": False},
    "放量": {"sort": "amount", "ascending": False},
    "低估值": {"sort": "pe", "ascending": True, "filter": lambda s: 0 < s["pe"] < 15},
}

RISK_FILTERS = {
    "保守": lambda s: -2 <= s["change"] <= 2,
    "激进": lambda s: s["change"] > 1,
}


def _get_analysis(stock):
    """优先读缓存，缓存没有则实时计算"""
    if is_cache_fresh():
        cached = get_cached_analysis(stock["code"])
        if cached:
            return cached
    klines = fetch_kline(stock["code"], days=60)
    return analyze(klines)


class StockAdvisor:

    def get_advice(self, openid, query, user_store):
        user = user_store.get_user(openid) or {}
        pref = user.get("preference", {})

        tags = [tag for kw, tag in KEYWORD_TAGS.items() if kw in query]
        sort_strategy = next((v for kw, v in SORT_KEYWORDS.items() if kw in query), None)

        # 优先读缓存
        stocks = None
        try:
            if is_cache_fresh():
                stocks = get_cached_quotes()
            if not stocks:
                stocks = get_all_quotes()
        except Exception as e:
            return f"获取行情数据出错：{e}"

        if not stocks:
            return "暂时无法获取行情数据，请稍后再试"

        # 按标签过滤
        filtered = [s for s in stocks if any(t in s.get("tags", []) for t in tags)] if tags else stocks

        # 策略过滤
        if sort_strategy and sort_strategy.get("filter"):
            sf = [s for s in filtered if sort_strategy["filter"](s)]
            if sf:
                filtered = sf

        # 风险偏好
        risk_fn = RISK_FILTERS.get(pref.get("risk"))
        if risk_fn:
            rf = [s for s in filtered if risk_fn(s)]
            if rf:
                filtered = rf

        # 排序
        if sort_strategy:
            filtered.sort(key=lambda s: s.get(sort_strategy["sort"], 0), reverse=not sort_strategy.get("ascending", False))
        else:
            filtered.sort(key=lambda s: s.get("amount", 0), reverse=True)

        # 意图识别
        want_buy = any(kw in query for kw in ("买", "购买", "买入", "入手", "抄底", "低吸"))
        want_sell = any(kw in query for kw in ("卖", "卖出", "减仓", "清仓", "止盈", "止损"))

        # 逐批分析
        result_list = []
        batch_size = 10
        for i in range(0, len(filtered), batch_size):
            for s in filtered[i:i + batch_size]:
                s["analysis"] = _get_analysis(s)
                score = s["analysis"]["score"]
                if want_buy and score >= 25:
                    result_list.append(s)
                elif want_sell and score <= -25:
                    result_list.append(s)
                elif not want_buy and not want_sell:
                    result_list.append(s)
                if len(result_list) >= 5:
                    break
            if len(result_list) >= 5:
                break

        # 放宽条件
        if want_buy and not result_list:
            for s in filtered:
                if "analysis" not in s:
                    s["analysis"] = _get_analysis(s)
                if s["analysis"]["score"] > 0:
                    result_list.append(s)
                if len(result_list) >= 5:
                    break

        if want_sell and not result_list:
            for s in filtered:
                if "analysis" not in s:
                    s["analysis"] = _get_analysis(s)
                if s["analysis"]["score"] < 0:
                    result_list.append(s)
                if len(result_list) >= 5:
                    break

        sort_asc = want_sell
        result_list.sort(key=lambda s: s["analysis"]["score"], reverse=not sort_asc)
        top = result_list[:5]

        if not top:
            if want_buy:
                return "当前没有找到符合买入条件的股票，建议观望"
            elif want_sell:
                return "当前没有找到需要卖出的股票，持仓信号尚可"
            return "暂未找到符合条件的股票，换个关键词试试？"

        user_store.add_history(openid, {"query": query, "time": int(time.time())})

        desc = "、".join(set(tags)) if tags else "热门"
        result = f"📊 {desc}股票推荐：\n\n"
        for s in top:
            a = s["analysis"]
            result += f"• {s['name']}({s['code']})\n"
            result += f"  价格:{s['price']} 涨幅:{s['change']}%\n"
            result += f"  {a['signal']}（评分:{a['score']}）\n"
            result += f"  理由：{'；'.join(a['reasons'])}\n\n"
        result += "⚠️ 仅为技术指标参考，不构成投资建议"
        return result
