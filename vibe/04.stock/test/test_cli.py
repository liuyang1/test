"""本地命令行测试，直接模拟用户对话，不需要微信"""
import sys, os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "src"))
from user_store import UserStore
from stock_advisor import StockAdvisor

user_store = UserStore()
advisor = StockAdvisor()
TEST_OPENID = "test_user_001"


def handle(text):
    text = text.strip()
    if text.startswith("设置"):
        prefs = {}
        mapping = {"风险": "risk", "周期": "period", "板块": "sector"}
        for part in text[2:].strip().split():
            if ":" in part or "：" in part:
                k, v = part.replace("：", ":").split(":", 1)
                key = mapping.get(k.strip())
                if key:
                    prefs[key] = v.strip()
        if not prefs:
            return "格式示例：设置 风险:稳健 周期:中线 板块:新能源"
        user_store.update_preference(TEST_OPENID, prefs)
        return f"偏好已更新：{prefs}"
    if text in ("偏好", "我的偏好"):
        user = user_store.get_user(TEST_OPENID)
        if not user or not user.get("preference"):
            return "未设置偏好"
        p = user["preference"]
        return f"风险:{p.get('risk','未设置')} 周期:{p.get('period','未设置')} 板块:{p.get('sector','未设置')}"
    return advisor.get_advice(TEST_OPENID, text, user_store)


if __name__ == "__main__":
    user_store.create_user(TEST_OPENID)
    print("AI 炒股助手 - 本地测试模式")
    print("命令示例：")
    print("  设置 风险:稳健 周期:中线 板块:新能源")
    print("  推荐新能源股票")
    print("  偏好")
    print("  输入 q 退出\n")
    while True:
        try:
            text = input("你> ").strip()
        except (EOFError, KeyboardInterrupt):
            break
        if text.lower() == "q":
            break
        if not text:
            continue
        print(f"\n助手> {handle(text)}\n")
