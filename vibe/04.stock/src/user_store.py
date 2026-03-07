import json
import os

DATA_DIR = "/tmp/users"


class UserStore:
    """基于文件的简易用户存储（SCF 环境下用 /tmp，后续可替换为数据库）"""

    def __init__(self):
        os.makedirs(DATA_DIR, exist_ok=True)

    def _path(self, openid):
        return os.path.join(DATA_DIR, f"{openid}.json")

    def create_user(self, openid):
        if not os.path.exists(self._path(openid)):
            self._save(openid, {"openid": openid, "preference": {}, "history": []})

    def get_user(self, openid):
        path = self._path(openid)
        if os.path.exists(path):
            with open(path, "r") as f:
                return json.load(f)
        return None

    def update_preference(self, openid, prefs):
        user = self.get_user(openid) or {"openid": openid, "preference": {}, "history": []}
        user["preference"].update(prefs)
        self._save(openid, user)

    def add_history(self, openid, record):
        user = self.get_user(openid) or {"openid": openid, "preference": {}, "history": []}
        user["history"].append(record)
        user["history"] = user["history"][-50:]  # 保留最近50条
        self._save(openid, user)

    def _save(self, openid, data):
        with open(self._path(openid), "w") as f:
            json.dump(data, f, ensure_ascii=False)
