"""验证 akshare 获取股票数据"""
try:
    import akshare as ak
except ImportError:
    print("[akshare] 未安装，执行: pip install akshare")
    raise SystemExit(1)

try:
    # 获取贵州茅台日K线
    df = ak.stock_zh_a_hist(symbol="600519", period="daily", adjust="qfq")
    print(f"[akshare] 获取到 {len(df)} 条记录")
    print(df.tail(3).to_string(index=False))
except Exception as e:
    print(f"[akshare] 请求失败: {e}")
