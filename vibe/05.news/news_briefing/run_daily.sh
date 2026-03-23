#!/bin/bash
# 每日新闻联播简报生成脚本
# 用于 crontab 定时执行

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

DATE=$(TZ='Asia/Shanghai' date +%Y%m%d)
LOG="$SCRIPT_DIR/temp/cron_${DATE}.log"
mkdir -p "$SCRIPT_DIR/temp"

echo "=== $(date) === 生成 $DATE 新闻简报" >> "$LOG"
python3 "$SCRIPT_DIR/main.py" --date "$DATE" >> "$LOG" 2>&1
echo "=== $(date) === 完成" >> "$LOG"
