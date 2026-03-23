"""卡片排版引擎 - 使用 Pillow 生成新闻卡片图片"""
import os
import textwrap
from PIL import Image, ImageDraw, ImageFont
from config import (
    VIDEO_WIDTH, VIDEO_HEIGHT, TITLE_TEXT, WATERMARK_TEXT,
    CARD_MIN_DURATION, CARD_MAX_DURATION,
    TEMP_DIR,
)

# 字体路径
_FONT_PATH = os.path.expanduser("~/.local/share/fonts/NotoSansCJKsc-Regular.otf")

# 颜色
_BG_COLOR = (26, 26, 46)  # 深蓝灰
_TITLE_COLOR = (255, 255, 255)
_DATE_COLOR = (170, 170, 170)
_INDEX_COLOR = (0, 170, 255)
_NEWS_TITLE_COLOR = (255, 180, 50)
_TEXT_COLOR = (220, 220, 220)
_LINE_COLOR = (80, 80, 80)
_PROGRESS_BG = (68, 68, 68)
_PROGRESS_FG = (255, 255, 255)
_WATERMARK_COLOR = (255, 255, 255, 100)


def _font(size: int) -> ImageFont.FreeTypeFont:
    return ImageFont.truetype(_FONT_PATH, size)


def render_card(
    bg_image_path: str | None,
    date_display: str,
    news_idx: int,
    total_news: int,
    title: str,
    text_lines: list[str],
    page_idx: int,
    total_pages: int,
    news_screenshot: str | None = None,
) -> Image.Image:
    """渲染单张新闻卡片"""
    # 背景
    if bg_image_path and os.path.exists(bg_image_path):
        img = Image.open(bg_image_path).resize((VIDEO_WIDTH, VIDEO_HEIGHT))
    else:
        img = Image.new("RGB", (VIDEO_WIDTH, VIDEO_HEIGHT), _BG_COLOR)

    # 半透明遮罩
    overlay = Image.new("RGBA", (VIDEO_WIDTH, VIDEO_HEIGHT), (0, 0, 0, 80))
    img = Image.alpha_composite(img.convert("RGBA"), overlay).convert("RGB")
    draw = ImageDraw.Draw(img)

    cx = VIDEO_WIDTH // 2
    left = 45
    right_margin = 45

    # 顶部标题
    _draw_centered(draw, TITLE_TEXT, _font(44), _TITLE_COLOR, 45)
    _draw_centered(draw, date_display, _font(30), _DATE_COLOR, 95)

    # 新闻编号
    draw.text((left, 155), f"{news_idx + 1}/{total_news}", fill=_INDEX_COLOR, font=_font(28))

    # 新闻标题
    draw.text((left, 190), title, fill=_NEWS_TITLE_COLOR, font=_font(42))

    # 分隔线
    draw.line([(left, 245), (VIDEO_WIDTH - right_margin, 245)], fill=_LINE_COLOR, width=2)

    # 正文
    y = 265
    for line in text_lines:
        draw.text((left, y), line, fill=_TEXT_COLOR, font=_font(32))
        y += 44

    # 新闻截图：放在正文下方的空白区域
    img_top = y + 15
    img_bottom = VIDEO_HEIGHT - 120  # 进度条上方
    img_space = img_bottom - img_top
    if news_screenshot and os.path.exists(news_screenshot) and img_space > 100:
        try:
            shot = Image.open(news_screenshot)
            # 等比缩放到可用宽度，高度不超过空白区域
            avail_w = VIDEO_WIDTH - left - right_margin
            ratio = min(avail_w / shot.width, img_space / shot.height)
            new_w = int(shot.width * ratio)
            new_h = int(shot.height * ratio)
            shot = shot.resize((new_w, new_h), Image.LANCZOS)
            # 居中放置，加圆角效果（简单裁剪）
            paste_x = left + (avail_w - new_w) // 2
            paste_y = img_top + (img_space - new_h) // 2
            img.paste(shot, (paste_x, paste_y))
        except Exception:
            pass

    # 进度条
    bar_y = VIDEO_HEIGHT - 100
    bar_w = VIDEO_WIDTH - 80
    progress_frac = (page_idx + 1) / total_pages
    draw = ImageDraw.Draw(img)  # 重新获取 draw（paste 后需要）
    draw.rectangle([(40, bar_y), (40 + bar_w, bar_y + 6)], fill=_PROGRESS_BG)
    draw.rectangle([(40, bar_y), (40 + int(bar_w * progress_frac), bar_y + 6)], fill=_PROGRESS_FG)

    # 水印（每页随机位置）
    import random
    random.seed(page_idx + 7)
    f_wm = _font(22)
    wm_w = draw.textlength(WATERMARK_TEXT, font=f_wm)
    wm_x = random.randint(20, int(VIDEO_WIDTH - wm_w - 20))
    wm_y = random.randint(10, int(VIDEO_HEIGHT * 0.12))
    draw.text((wm_x, wm_y), WATERMARK_TEXT,
              fill=(255, 255, 255, 100), font=f_wm)

    return img


def _draw_centered(draw: ImageDraw.ImageDraw, text: str, font, color, y: int):
    w = draw.textlength(text, font=font)
    draw.text(((VIDEO_WIDTH - w) / 2, y), text, fill=color, font=font)


def wrap_text_for_card(text: str, font_size: int = 32, max_width: int = None) -> list[str]:
    """根据实际像素宽度换行"""
    if max_width is None:
        max_width = VIDEO_WIDTH - 90  # left + right margin
    font = _font(font_size)
    lines = []
    for paragraph in text.split('\n'):
        if not paragraph.strip():
            continue
        current = ""
        for char in paragraph:
            test = current + char
            if font.getlength(test) > max_width:
                if current:
                    lines.append(current)
                current = char
            else:
                current = test
        if current:
            lines.append(current)
    return lines


def generate_card_pages(segments: list[dict], bg_image_path: str | None,
                        date_str: str, video_paths: list[str] | None = None) -> list[tuple[str, float]]:
    """生成所有卡片页面图片，含新闻截图"""
    import subprocess
    from config import FFMPEG
    date_display = f"{date_str[:4]}年{int(date_str[4:6])}月{int(date_str[6:8])}日"
    news_segs = [s for s in segments if s['index'] >= 0]
    total_news = len(news_segs)

    card_dir = os.path.join(TEMP_DIR, date_str, "cards")
    os.makedirs(card_dir, exist_ok=True)

    # 为每条新闻从对应视频截取一帧作为截图
    screenshots = {}
    if video_paths:
        for i, seg in enumerate(news_segs):
            idx = seg['index']
            if idx < len(video_paths):
                shot_path = os.path.join(card_dir, f"shot_{i:03d}.jpg")
                # 取视频60%位置，避开片头主持人画面
                try:
                    dur_cmd = subprocess.run(
                        [FFMPEG.replace('ffmpeg','ffprobe'), "-v", "quiet", "-print_format", "json",
                         "-show_format", video_paths[idx]],
                        capture_output=True, text=True, timeout=10)
                    import json as _json
                    vdur = float(_json.loads(dur_cmd.stdout)["format"]["duration"])
                    seek = max(10, int(vdur * 0.6))
                except Exception:
                    seek = 25
                subprocess.run([
                    FFMPEG, "-y", "-ss", str(seek), "-i", video_paths[idx],
                    "-frames:v", "1", "-q:v", "3", shot_path,
                ], capture_output=True, text=True, timeout=15)
                if os.path.exists(shot_path) and os.path.getsize(shot_path) > 500:
                    screenshots[i] = shot_path

    # 分页
    max_lines = 8  # 减少行数给截图留空间
    pages = []
    for i, seg in enumerate(news_segs):
        title = seg.get('title', f'新闻{i+1}')
        all_lines = wrap_text_for_card(seg['text'])
        for start in range(0, len(all_lines), max_lines):
            page_lines = all_lines[start:start + max_lines]
            pages.append((i, title, page_lines, start > 0))

    total_pages = len(pages)
    results = []

    for pi, (news_i, title, text_lines, is_cont) in enumerate(pages):
        img = render_card(
            bg_image_path, date_display, news_i, total_news,
            title, text_lines, pi, total_pages,
            news_screenshot=screenshots.get(news_i),
        )
        img_path = os.path.join(card_dir, f"card_{pi:03d}.png")
        img.save(img_path, quality=95)

        # 阅读时间
        chars = sum(len(l) for l in text_lines)
        dur = max(CARD_MIN_DURATION, min(CARD_MAX_DURATION, chars * 0.12 + 1.5))
        results.append((img_path, dur))

        cont = " (续)" if is_cont else ""
        print(f"[card] p{pi+1}/{total_pages}: {title}{cont} ({dur:.1f}s)")

    return results
