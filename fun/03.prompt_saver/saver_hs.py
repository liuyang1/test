#!/usr/bin/env python3
import subprocess
import datetime
from pathlib import Path
import os


def get_clipboard():
    return subprocess.check_output("pbpaste", env={"LANG": "en_US.UTF-8"}).decode(
        "utf-8"
    )


def notify(title, text):
    os.system(
        """
              osascript -e 'display notification "{}" with title "{}"'
              """.format(
            text, title
        )
    )


def main():
    # 设置文件路径
    target_file = Path.home() / "saver.wiki"

    try:
        # 获取剪贴板内容
        content = get_clipboard()
        timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        # 准备预览文本
        preview_head = content[:20]
        preview_tail = content[-10:] if len(content) > 30 else ""
        preview = f"{preview_head}......{preview_tail}" if preview_tail else preview_head

        # 写入文件
        with open(target_file, "a", encoding="utf-8") as f:
            f.write(f"{timestamp}\n{content}\n\n")

        # 发送通知
        # notify("记录成功", preview)
        print("SR:" + preview)

    except Exception as e:
        # notify("错误", str(e))
        print("Fail to Record")


if __name__ == "__main__":
    main()
