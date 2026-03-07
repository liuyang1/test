"""Vercel Serverless Function - 聊天 API"""
import json
import sys
import os
from http.server import BaseHTTPRequestHandler

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "src"))
from user_store import UserStore
from stock_advisor import StockAdvisor

user_store = UserStore()
advisor = StockAdvisor()


class handler(BaseHTTPRequestHandler):
    def do_POST(self):
        length = int(self.headers.get("Content-Length", 0))
        body = json.loads(self.rfile.read(length))

        user_id = body.get("user_id", "anonymous")
        message = body.get("message", "").strip()

        if not message:
            reply = "请输入你的问题"
        elif message.startswith("设置"):
            reply = self._set_pref(user_id, message[2:].strip())
        elif message in ("偏好", "我的偏好"):
            reply = self._show_pref(user_id)
        else:
            user_store.create_user(user_id)
            reply = advisor.get_advice(user_id, message, user_store)

        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.send_header("Access-Control-Allow-Origin", "*")
        self.end_headers()
        self.wfile.write(json.dumps({"reply": reply}, ensure_ascii=False).encode())

    def do_OPTIONS(self):
        self.send_response(200)
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.end_headers()

    def _set_pref(self, user_id, text):
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
        user_store.create_user(user_id)
        user_store.update_preference(user_id, prefs)
        return f"偏好已更新：{prefs}"

    def _show_pref(self, user_id):
        user = user_store.get_user(user_id)
        if not user or not user.get("preference"):
            return "未设置偏好，发送「设置 风险:稳健 周期:中线 板块:新能源」"
        p = user["preference"]
        return f"风险:{p.get('risk','未设置')} 周期:{p.get('period','未设置')} 板块:{p.get('sector','未设置')}"
