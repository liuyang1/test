"""验证 tushare 获取股票数据"""
import os

try:
    import tushare as ts
except ImportError:
    print("[tushare] 未安装，执行: pip install tushare")
    raise SystemExit(1)

TOKEN = os.environ.get("TUSHARE_TOKEN", "")
if not TOKEN:
    print("[tushare] 请设置环境变量 TUSHARE_TOKEN（从 https://tushare.pro 获取）")
    raise SystemExit(1)

try:
    pro = ts.pro_api(TOKEN)
    df = pro.daily(ts_code="600519.SH", start_date="20250301", end_date="20250307")
    print(f"[tushare] 获取到 {len(df)} 条记录")
    print(df.to_string(index=False))
except Exception as e:
    print(f"[tushare] 请求失败: {e}")
