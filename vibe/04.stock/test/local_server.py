"""本地 HTTP 服务器，模拟 SCF API 网关，用于微信接入调试"""
import sys, os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "src"))
from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs
import json
from index import main_handler


class Handler(BaseHTTPRequestHandler):
    def do_GET(self):
        parsed = urlparse(self.path)
        qs = {k: v[0] for k, v in parse_qs(parsed.query).items()}
        event = {"httpMethod": "GET", "queryString": qs}
        result = main_handler(event, {})
        self.send_response(result.get("statusCode", 200))
        self.end_headers()
        self.wfile.write(result.get("body", "").encode())

    def do_POST(self):
        length = int(self.headers.get("Content-Length", 0))
        body = self.rfile.read(length).decode()
        event = {"httpMethod": "POST", "body": body}
        result = main_handler(event, {})
        self.send_response(result.get("statusCode", 200))
        for k, v in result.get("headers", {}).items():
            self.send_header(k, v)
        self.end_headers()
        self.wfile.write(result.get("body", "").encode())


if __name__ == "__main__":
    port = 8080
    print(f"本地服务启动: http://localhost:{port}")
    print("可配合 ngrok 进行微信公众号联调: ngrok http 8080")
    HTTPServer(("", port), Handler).serve_forever()
