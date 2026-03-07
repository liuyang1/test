"""数据缓存层 - 定时预拉取行情和K线，预计算技术指标"""
import json
import os
import time
from data_source import get_all_quotes, fetch_kline, STOCK_POOL
from indicator import analyze

CACHE_DIR = os.environ.get("CACHE_DIR", "/tmp/stock_cache")
QUOTES_FILE = os.path.join(CACHE_DIR, "quotes.json")
ANALYSIS_FILE = os.path.join(CACHE_DIR, "analysis.json")
META_FILE = os.path.join(CACHE_DIR, "meta.json")


def refresh_cache():
    """拉取全量数据并预计算指标，供定时任务调用"""
    os.makedirs(CACHE_DIR, exist_ok=True)

    # 1. 拉取实时行情
    quotes = get_all_quotes()
    with open(QUOTES_FILE, "w") as f:
        json.dump(quotes, f, ensure_ascii=False)

    # 2. 逐只拉K线并计算技术指标
    analysis = {}
    for stock in quotes:
        code = stock["code"]
        try:
            klines = fetch_kline(code, days=60)
            result = analyze(klines)
        except Exception:
            result = {"signal": "观望", "reasons": ["数据获取失败"], "score": 0}
        analysis[code] = result

    with open(ANALYSIS_FILE, "w") as f:
        json.dump(analysis, f, ensure_ascii=False)

    # 3. 记录更新时间
    meta = {"updated_at": int(time.time()), "stock_count": len(quotes)}
    with open(META_FILE, "w") as f:
        json.dump(meta, f, ensure_ascii=False)

    return meta


def get_cached_quotes():
    """读取缓存的行情数据"""
    if not os.path.exists(QUOTES_FILE):
        return None
    with open(QUOTES_FILE, "r") as f:
        return json.load(f)


def get_cached_analysis(code):
    """读取缓存的技术分析结果"""
    if not os.path.exists(ANALYSIS_FILE):
        return None
    with open(ANALYSIS_FILE, "r") as f:
        data = json.load(f)
    return data.get(code)


def get_cache_meta():
    """获取缓存元信息"""
    if not os.path.exists(META_FILE):
        return None
    with open(META_FILE, "r") as f:
        return json.load(f)


def is_cache_fresh(max_age=18 * 3600):
    """缓存是否在有效期内（默认18小时，覆盖一个交易日）"""
    meta = get_cache_meta()
    if not meta:
        return False
    return (time.time() - meta["updated_at"]) < max_age
