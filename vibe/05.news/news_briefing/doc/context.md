# Project Context — CCTV 新闻联播视频简报生成器

> 用于恢复开发/使用上下文。将此文件内容提供给 AI 助手即可继续。

## 项目概述

自动抓取 CCTV 新闻联播每日文稿和视频，生成 ≤2 分钟的竖版短视频简报（720x1280），适合手机端快速消费。

## 项目路径

```
/local/home/lyz/git/hub/test/vibe/05.news/news_briefing/
```

## 文件结构

```
news_briefing/
├── main.py              # 主入口，串联所有模块
├── config.py            # 全局配置（布局/字幕/水印/语速/标题等）
├── scraper.py           # 央视网数据抓取（文稿+视频URL+标题）
├── downloader.py        # m3u8 视频下载
├── summarizer.py        # LLM 文稿摘要（kiro-cli，输出带索引+标题）
├── tts.py               # edge-tts 分段语音生成 + 字幕时间轴
├── video_editor.py      # 视频剪辑/合成/字幕/标题/进度条/水印/首图/文字稿
├── card_renderer.py     # Pillow 排版引擎（卡片模式）
├── requirements.txt     # Python 依赖
├── run_daily.sh         # crontab 每日运行脚本
├── assets/              # 运行时生成的音效（bgm.mp3, ding.mp3）
├── .gitignore
├── doc/
│   ├── requirements.md  # 完整需求文档
│   ├── crontab.md       # 定时任务配置说明
│   └── context.md       # 本文件
├── temp/                # 临时文件（视频缓存/TTS/字幕等）
└── output/              # 最终输出（视频/首图/文字稿）
```

## 输出物（每日）

| 文件 | 说明 |
|------|------|
| `news_briefing_YYYYMMDD_fit_top.mp4` | TTS配音 + 上方视频 + 模糊背景 |
| `news_briefing_YYYYMMDD_triple.mp4` | TTS配音 + 3x1 重复布局 |
| `news_briefing_YYYYMMDD_blur_full.mp4` | TTS配音 + 居中视频 + 模糊背景 |
| `news_briefing_YYYYMMDD_cards.mp4` | Pillow卡片 + BGM + 新闻截图 |
| `cover_YYYYMMDD.jpg` | 封面首图 |
| `article_YYYYMMDD.txt` | 格式化文字稿 |

## 核心技术方案

- **数据源**：央视网 cctv.com，抓取文稿+m3u8视频，跳过整期完整版，标题从 `<a title="">` 提取
- **摘要**：kiro-cli 调用 LLM，输出 `[索引|标题] 摘要` 格式，索引对应原始文章编号
- **TTS**：edge-tts，语速 +35%（约1.5x），分段生成获取精确时长
- **音视频同步**：每段摘要的视频片段严格按文章索引匹配，视频1.5x加速
- **字幕**：ASS 格式，`\pos` 绝对像素定位避免跳动，长文本从标点处断为两行
- **标题/进度条**：所有布局统一显示"新闻联播2分钟"+日期，时间线性进度条
- **水印**：`\move` 缓慢随机漂移，默认文本 "YYY"（config.py 可改）
- **卡片模式**：Pillow 排版引擎，左对齐信息流风格，含新闻视频截图（取60%位置避开主持人）
- **视频处理**：纯 CPU ffmpeg（静态编译），无 GPU 依赖

## 环境依赖

```
Python 3.11.7
ffmpeg 7.0.2-static (~/.local/bin/ffmpeg)
kiro-cli (~/.local/bin/kiro-cli)
中文字体: NotoSansCJKsc-Regular.otf (~/.local/share/fonts/)

pip: edge-tts, m3u8, requests, beautifulsoup4, Pillow
```

## 使用方式

```bash
cd /local/home/lyz/git/hub/test/vibe/05.news/news_briefing

# 生成指定日期（所有布局+卡片）
python3 main.py --date 20260321

# 只生成特定布局
python3 main.py --date 20260321 --layout blur_full

# 跳过卡片模式
python3 main.py --date 20260321 --no-cards

# 运行模块测试
python3 main.py --test

# crontab 已配置：每天 CST 21:00 自动运行
```

## 已知问题 / TODO

- [ ] BGM 是合成的正弦波琶音，效果一般。后续替换为真正的免费音乐
- [ ] 卡片模式排版可进一步优化（段落间距、标点避头尾等）
- [ ] 字幕分行在极端情况下（如"38.3%。"）可能不完美
- [ ] 未实现自动发布到短视频平台
- [ ] 未实现错误通知机制（当前只有日志）

## 关键设计决策记录

1. **字幕用 ASS `\pos` 绝对定位**：因为 libass 碰撞检测会导致多行字幕位置跳动和顺序反转
2. **TTS 分段生成**：每段摘要独立生成 TTS 获取精确时长，而非整体生成后按字数估算
3. **跳过完整版视频**：scraper 过滤掉30分钟整期视频，确保 article[i] 和 video[i] 一一对应
4. **视频截图取60%位置**：避开片头主持人画面，获取新闻现场画面
5. **卡片用 Pillow 而非 ASS**：ASS 字幕系统做排版是 hack，Pillow 能精确控制像素级布局
