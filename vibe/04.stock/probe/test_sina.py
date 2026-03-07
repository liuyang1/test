"""验证新浪财经行情接口"""
import urllib.request

CODE = "sh600519"  # 贵州茅台
URL = f"http://hq.sinajs.cn/list={CODE}"

try:
    req = urllib.request.Request(URL, headers={"Referer": "https://finance.sina.com.cn"})
    resp = urllib.request.urlopen(req, timeout=10).read().decode("gbk")
    parts = resp.split(",")
    if len(parts) > 3:
        # 格式: 名称,开盘,昨收,当前价,...
        name = resp.split('"')[1].split(",")[0]
        price = parts[3]
        print(f"[新浪] {name} 当前价: {price}")
    else:
        print("[新浪] 返回数据格式异常:", resp[:200])
except Exception as e:
    print(f"[新浪] 请求失败: {e}")
