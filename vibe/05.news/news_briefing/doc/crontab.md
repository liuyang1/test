# 定时生成新闻联播简报

## 设置 crontab

每天 CST 21:00 自动运行（央视网通常 20:00 左右更新当天内容）：

```bash
# 编辑 crontab
crontab -e

# 添加以下行（CST 21:00 = UTC 13:00）
0 13 * * * /local/home/lyz/git/hub/test/vibe/05.news/news_briefing/run_daily.sh
```

如果系统时区已经是 `Asia/Shanghai`，直接用本地时间：

```bash
0 21 * * * /local/home/lyz/git/hub/test/vibe/05.news/news_briefing/run_daily.sh
```

## 确认时区

```bash
timedatectl | grep "Time zone"
# 如果不是 Asia/Shanghai，用 UTC 13:00 的写法
```

## 查看日志

```bash
# 当天日志
cat /local/home/lyz/git/hub/test/vibe/05.news/news_briefing/temp/cron_$(date +%Y%m%d).log

# 最近的 cron 执行记录
grep run_daily /var/log/cron 2>/dev/null || grep CRON /var/log/syslog 2>/dev/null | tail -5
```

## 手动测试

```bash
# 先手动跑一次确认没问题
/local/home/lyz/git/hub/test/vibe/05.news/news_briefing/run_daily.sh

# 查看输出
ls -lh /local/home/lyz/git/hub/test/vibe/05.news/news_briefing/output/
```

## 注意事项

- crontab 环境的 PATH 可能不包含 python3/ffmpeg，`run_daily.sh` 中可按需添加：
  ```bash
  export PATH="$HOME/.local/bin:$HOME/.local/share/rtx/installs/python/3.11.7/bin:$PATH"
  ```
- 确保 `mwinit` / midway 认证有效（kiro-cli 需要）
- 输出文件在 `output/` 目录，日志在 `temp/cron_YYYYMMDD.log`
