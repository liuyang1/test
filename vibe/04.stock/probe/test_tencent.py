"""验证腾讯财经行情接口"""
import urllib.request

CODE = "sh600519"  # 贵州茅台
URL = f"http://qt.gtimg.cn/q={CODE}"

try:
    resp = urllib.request.urlopen(URL, timeout=10).read().decode("gbk")
    parts = resp.strip().split("~")
    if len(parts) > 3:
        print(f"[腾讯] {parts[1]} 当前价: {parts[3]}")
    else:
        print("[腾讯] 返回数据格式异常:", resp[:200])
except Exception as e:
    print(f"[腾讯] 请求失败: {e}")
