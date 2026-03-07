import json
import hashlib
import os
import time
import xml.etree.ElementTree as ET
from user_store import UserStore
from stock_advisor import StockAdvisor


user_store = UserStore()
advisor = StockAdvisor()


def main_handler(event, context):
    """腾讯云 SCF 入口函数"""
    # GET 请求：微信服务器验证
    if event.get("httpMethod") == "GET":
        return verify_wechat(event)

    # POST 请求：处理微信消息
    body = event.get("body", "")
    try:
        msg = parse_xml(body)
        reply = handle_message(msg)
        return make_xml_response(msg, reply)
    except Exception as e:
        return {"statusCode": 500, "body": str(e)}


def verify_wechat(event):
    """微信接入验证"""
    params = event.get("queryString", {})
    token = os.environ.get("WECHAT_TOKEN", "your_wechat_token")
    signature = params.get("signature", "")
    timestamp = params.get("timestamp", "")
    nonce = params.get("nonce", "")
    echostr = params.get("echostr", "")

    items = sorted([token, timestamp, nonce])
    check = hashlib.sha1("".join(items).encode()).hexdigest()
    if check == signature:
        return {"statusCode": 200, "body": echostr}
    return {"statusCode": 403, "body": "forbidden"}


def parse_xml(xml_str):
    """解析微信 XML 消息"""
    root = ET.fromstring(xml_str)
    return {child.tag: child.text for child in root}


def handle_message(msg):
    """处理用户消息，返回回复文本"""
    openid = msg.get("FromUserName", "")
    content = msg.get("Content", "").strip()
    msg_type = msg.get("MsgType", "")

    if msg_type == "event":
        if msg.get("Event") == "subscribe":
            user_store.create_user(openid)
            return "欢迎使用 AI 炒股助手！\n\n请先设置你的偏好：\n发送「设置 风险:稳健 周期:中线 板块:新能源」\n\n或直接问我，比如：\n「推荐几只低估值的银行股」"

    if msg_type != "text":
        return "目前只支持文字消息哦"

    # 设置偏好
    if content.startswith("设置"):
        return set_preference(openid, content[2:].strip())

    # 查看偏好
    if content in ("我的偏好", "偏好", "设置"):
        return show_preference(openid)

    # 股票建议
    return advisor.get_advice(openid, content, user_store)


def set_preference(openid, text):
    """解析并保存用户偏好，格式：风险:稳健 周期:中线 板块:新能源"""
    prefs = {}
    mapping = {"风险": "risk", "周期": "period", "板块": "sector"}
    for part in text.split():
        if ":" in part or "：" in part:
            k, v = part.replace("：", ":").split(":", 1)
            key = mapping.get(k.strip())
            if key:
                prefs[key] = v.strip()
    if not prefs:
        return "格式示例：设置 风险:稳健 周期:中线 板块:新能源"
    user_store.update_preference(openid, prefs)
    return f"偏好已更新：{prefs}"


def show_preference(openid):
    user = user_store.get_user(openid)
    if not user or not user.get("preference"):
        return "你还没有设置偏好，发送「设置 风险:稳健 周期:中线 板块:新能源」来设置"
    p = user["preference"]
    return f"你的偏好：\n风险偏好：{p.get('risk', '未设置')}\n投资周期：{p.get('period', '未设置')}\n关注板块：{p.get('sector', '未设置')}"


def make_xml_response(msg, reply_text):
    """构造微信 XML 回复"""
    xml = f"""<xml>
<ToUserName><![CDATA[{msg.get('FromUserName', '')}]]></ToUserName>
<FromUserName><![CDATA[{msg.get('ToUserName', '')}]]></FromUserName>
<CreateTime>{int(time.time())}</CreateTime>
<MsgType><![CDATA[text]]></MsgType>
<Content><![CDATA[{reply_text}]]></Content>
</xml>"""
    return {
        "statusCode": 200,
        "headers": {"Content-Type": "application/xml"},
        "body": xml,
    }
