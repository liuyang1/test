"""行情数据获取 - 基于腾讯财经接口，无需注册"""
import urllib.request
import json


# A股常用股票池（代码, 板块标签）
STOCK_POOL = {
    # 银行
    "sh601398": ["银行", "红利", "低估值"], "sh601939": ["银行", "红利", "低估值"],
    "sh600036": ["银行", "红利"], "sh601288": ["银行", "红利", "低估值"],
    "sh601328": ["银行", "红利", "低估值"], "sh600016": ["银行", "红利"],
    "sh601166": ["银行"], "sh600000": ["银行"],
    "sh601229": ["银行", "红利"], "sh601998": ["银行"],
    "sh601169": ["银行"], "sh600015": ["银行"],
    # 白酒/消费
    "sh600519": ["白酒", "消费"], "sz000858": ["白酒", "消费"],
    "sz000568": ["白酒", "消费"], "sh600809": ["白酒", "消费"],
    "sz000596": ["白酒", "消费"], "sh603369": ["白酒", "消费"],
    "sh600887": ["消费", "乳业"], "sh601888": ["消费", "旅游"],
    "sz000895": ["消费", "乳业"], "sh600600": ["消费", "啤酒"],
    # 新能源
    "sz002594": ["新能源", "科技"], "sz300750": ["新能源", "科技"],
    "sh601012": ["新能源"], "sz002459": ["新能源"],
    "sh600438": ["新能源"], "sz300274": ["新能源"],
    "sh688599": ["新能源"], "sz002129": ["新能源"],
    "sz300763": ["新能源"], "sh600905": ["新能源", "电力"],
    # 医药
    "sh600276": ["医药"], "sz000538": ["医药"],
    "sh603259": ["医药"], "sz300760": ["医药"],
    "sh600196": ["医药"], "sz002007": ["医药"],
    "sh600436": ["医药"], "sz300347": ["医药"],
    "sz300122": ["医药"], "sh688180": ["医药"],
    # 科技/半导体
    "sz002230": ["科技", "半导体"], "sh688981": ["科技", "半导体"],
    "sz000725": ["科技"], "sh603501": ["科技"],
    "sz002415": ["科技"], "sh688036": ["科技"],
    "sh688012": ["科技", "半导体"], "sz002049": ["科技"],
    "sz300782": ["科技"], "sh688111": ["科技", "半导体"],
    "sz002371": ["科技"], "sh603986": ["科技"],
    "sz300496": ["科技"], "sh688008": ["科技"],
    # 红利/高股息/资源
    "sh601088": ["红利", "煤炭"], "sh600028": ["红利", "石油"],
    "sh601857": ["红利", "石油"], "sh600900": ["红利", "电力"],
    "sh600585": ["红利", "建材"], "sh601006": ["红利", "石油"],
    "sh600188": ["红利", "煤炭"], "sh601225": ["红利", "煤炭"],
    "sh600019": ["红利", "钢铁"], "sh601899": ["红利", "矿业"],
    "sh600104": ["红利", "汽车"], "sh601668": ["红利", "建筑"],
    "sh600048": ["红利", "地产"], "sh601390": ["红利", "铁路"],
    # 金融
    "sh601318": ["保险", "金融"], "sh600030": ["券商", "金融"],
    "sh601601": ["保险", "金融"], "sh601688": ["券商", "金融"],
    "sh600837": ["券商", "金融"], "sh601211": ["保险", "金融"],
    # 家电/制造
    "sz000333": ["家电"], "sz000651": ["家电"],
    "sz002032": ["家电"], "sh600690": ["家电"],
    # 地产
    "sh600048": ["地产"], "sz000002": ["地产"],
    "sh601155": ["地产"], "sz001979": ["地产"],
}


def fetch_quotes(codes):
    """批量获取实时行情"""
    if not codes:
        return []
    code_str = ",".join(codes)
    url = f"http://qt.gtimg.cn/q={code_str}"
    try:
        resp = urllib.request.urlopen(url, timeout=10).read().decode("gbk")
    except Exception as e:
        raise RuntimeError(f"行情接口请求失败: {e}")

    results = []
    for line in resp.strip().split(";"):
        line = line.strip()
        if not line:
            continue
        parts = line.split("~")
        if len(parts) < 45:
            continue
        results.append({
            "code": parts[2],
            "name": parts[1],
            "price": float(parts[3]) if parts[3] else 0,
            "change": float(parts[32]) if parts[32] else 0,
            "volume": float(parts[36]) if parts[36] else 0,
            "amount": float(parts[37]) if parts[37] else 0,
            "pe": float(parts[39]) if parts[39] else 0,
            "pb": float(parts[46]) if parts[46] and len(parts) > 46 else 0,
        })
    return results


def fetch_kline(code, days=60):
    """获取日K线历史数据，返回 [{date, open, close, high, low, volume}, ...]"""
    prefix = "sh" if code.startswith("6") or code.startswith("9") else "sz"
    symbol = f"{prefix}{code}"
    url = f"http://web.ifzq.gtimg.cn/appstock/app/fqkline/get?param={symbol},day,,,{days},qfq"
    try:
        resp = urllib.request.urlopen(url, timeout=10).read().decode("utf-8")
        data = json.loads(resp)
        stock_data = data.get("data", {}).get(symbol, {})
        day_data = stock_data.get("qfqday") or stock_data.get("day", [])
    except Exception:
        return []

    results = []
    for item in day_data:
        if len(item) >= 6:
            results.append({
                "date": item[0],
                "open": float(item[1]),
                "close": float(item[2]),
                "high": float(item[3]),
                "low": float(item[4]),
                "volume": float(item[5]),
            })
    return results


def get_all_quotes():
    """获取股票池全部行情"""
    codes = list(STOCK_POOL.keys())
    all_results = []
    batch_size = 50
    for i in range(0, len(codes), batch_size):
        batch = codes[i:i + batch_size]
        all_results.extend(fetch_quotes(batch))
    for item in all_results:
        full_code = f"sh{item['code']}" if item['code'].startswith('6') or item['code'].startswith('9') else f"sz{item['code']}"
        item["tags"] = STOCK_POOL.get(full_code, [])
    return all_results
