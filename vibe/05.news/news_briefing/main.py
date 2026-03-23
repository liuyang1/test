"""新闻联播每日视频简报生成器 - 主入口"""
import sys
import os
import time
import json
import argparse

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from config import OUTPUT_DIR, TEMP_DIR, VIDEO_LAYOUTS
from scraper import scrape_news
from downloader import download_all_videos
from summarizer import summarize
from tts import generate_tts_by_segments
from video_editor import (
    extract_matched_clips, compose_final_video, get_duration,
    prepend_ding, generate_card_video,
    generate_cover_image, generate_text_article,
)


def _progress(step: int, total: int, name: str, start_time: float):
    elapsed = time.time() - start_time
    print(f"\n{'='*50}")
    print(f"  [{step}/{total}] {name}  (已用时 {elapsed:.0f}s)")
    print(f"{'='*50}")


def run(date_str: str | None = None, layouts: list[str] | None = None,
        subtitle_style: str | None = None, enable_cards: bool = True):
    if layouts is None:
        layouts = VIDEO_LAYOUTS

    start_time = time.time()
    total_steps = 6

    print("╔══════════════════════════════════════════════════╗")
    print("║    CCTV 新闻联播视频简报生成器 (1.5x)          ║")
    print("╚══════════════════════════════════════════════════╝")

    # Step 1
    _progress(1, total_steps, "抓取新闻联播数据", start_time)
    news_data = scrape_news(date_str)
    date = news_data["date"]
    print(f"  → 日期: {date}, 文章: {len(news_data['articles'])} 条, "
          f"视频: {len(news_data['video_urls'])} 个, 文稿: {len(news_data['full_text'])} 字")

    os.makedirs(os.path.join(TEMP_DIR, date), exist_ok=True)
    with open(os.path.join(TEMP_DIR, date, "full_text.txt"), "w", encoding="utf-8") as f:
        f.write(news_data["full_text"])

    # Step 2
    _progress(2, total_steps, "下载视频", start_time)
    video_paths = download_all_videos(news_data["video_urls"], date)
    print(f"  → 下载完成: {len(video_paths)} 个视频")

    # Step 3
    _progress(3, total_steps, "生成文稿摘要 (LLM, 带索引+标题)", start_time)
    segments = summarize(news_data["full_text"], news_data["articles"])
    summary_text = "\n\n".join(s["text"] for s in segments)
    with open(os.path.join(TEMP_DIR, date, "summary.txt"), "w", encoding="utf-8") as f:
        f.write(summary_text)
    with open(os.path.join(TEMP_DIR, date, "segments.json"), "w", encoding="utf-8") as f:
        json.dump(segments, f, ensure_ascii=False, indent=2)
    print(f"  → 摘要: {len(summary_text)} 字, {len(segments)} 段")
    for s in segments:
        print(f"    [{s['index']:>2}|{s.get('title',''):<8s}] {s['text'][:30]}...")

    # Step 4
    _progress(4, total_steps, "生成 TTS 配音 + 字幕 + 音效", start_time)
    audio_path, subtitle_path, seg_durations = generate_tts_by_segments(segments, date)
    # 添加叮咚音效
    ding_audio = os.path.join(TEMP_DIR, date, "tts_with_ding.mp4")
    audio_path = prepend_ding(audio_path, ding_audio)
    audio_dur = get_duration(audio_path)
    # 叮咚音效增加的时长需要加到第一段
    ding_extra = audio_dur - sum(seg_durations)
    if ding_extra > 0:
        seg_durations[0] += ding_extra
    print(f"  → 音频时长: {audio_dur:.1f}s ({audio_dur/60:.1f}min) (含叮咚)")

    # 生成首图和文字稿
    cover = generate_cover_image(date, video_paths)
    article = generate_text_article(segments, date)

    # Step 5: 风格A - TTS 配音版
    _progress(5, total_steps, f"风格A: 合成视频 ({len(layouts)} 种布局)", start_time)
    outputs = []
    for layout in layouts:
        print(f"\n  --- 布局: {layout} ---")
        bg_video = extract_matched_clips(
            segments, seg_durations, video_paths,
            news_data["articles"], date, layout,
        )
        output = compose_final_video(
            bg_video, audio_path, subtitle_path, date, subtitle_style, layout,
            segments=segments, seg_durations=seg_durations,
        )
        outputs.append((layout, output))

    # Step 6: 风格B - 图片卡片版
    if enable_cards:
        _progress(6, total_steps, "风格B: 图片卡片版", start_time)
        try:
            card_output = generate_card_video(
                segments, video_paths, news_data["articles"], date,
            )
            outputs.append(("cards", card_output))
        except Exception as e:
            print(f"  ⚠️ 卡片模式失败: {e}")

    total_time = time.time() - start_time
    print(f"\n╔══════════════════════════════════════════════════╗")
    print(f"║  ✅ 全部完成！                                  ║")
    print(f"╠══════════════════════════════════════════════════╣")
    for layout, output in outputs:
        dur = get_duration(output)
        size = os.path.getsize(output) / (1024 * 1024)
        print(f"║  [{layout:>9s}] {dur:.0f}s / {size:.1f}MB{'':>22s} ║")
    print(f"╠══════════════════════════════════════════════════╣")
    print(f"║  总耗时: {total_time:.0f}s{'':>37s} ║")
    print(f"╚══════════════════════════════════════════════════╝")
    for _, output in outputs:
        print(f"  {output}")
    print(f"  {cover}")
    print(f"  {article}")
    return [o for _, o in outputs]


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="CCTV 新闻联播视频简报生成器")
    parser.add_argument("--date", type=str, default=None, help="日期 YYYYMMDD")
    parser.add_argument("--layout", nargs="+", choices=["fit_top", "triple", "blur_full"],
                        default=None, help="视频布局（可多选，默认全部）")
    parser.add_argument("--subtitle-style", choices=["default", "news", "minimal"],
                        default=None, help="字幕风格")
    parser.add_argument("--no-cards", action="store_true", help="跳过卡片模式")
    parser.add_argument("--test", action="store_true", help="运行模块测试")
    args = parser.parse_args()

    if args.test:
        print("╔══════════════════════════════════════════════════╗")
        print("║  模块测试                                       ║")
        print("╚══════════════════════════════════════════════════╝")

        print("\n--- 1. TTS 分段生成测试 ---")
        from tts import generate_tts_by_segments as tts_test
        segs = [
            {"index": -1, "title": "开场", "text": "各位观众，以下是今日新闻联播要闻速览。"},
            {"index": 0, "title": "经济向好", "text": "全国经济形势持续向好，各项指标稳中有进。"},
        ]
        _, s, durs = tts_test(segs, "test")
        print(f"精确段落时长: {durs}")

        print("\n--- 2. 字幕分行+烧录测试 ---")
        from video_editor import test_split_two_lines, test_subtitle_burn
        test_split_two_lines()
        test_subtitle_burn()

        print("\n--- 3. 布局+1.5x倍速测试 ---")
        from video_editor import test_layout
        test_layout()

        print("\n✅ 全部测试完成")
    else:
        run(args.date, args.layout, args.subtitle_style, not args.no_cards)
