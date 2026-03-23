"""视频编辑模块 - 标题/进度条/音效/字幕/水印/卡片模式"""
import os
import re
import subprocess
import json
from config import (
    FFMPEG, FFPROBE, VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_FPS,
    SUBTITLE_STYLES, SUBTITLE_STYLE, VIDEO_LAYOUT, VIDEO_LAYOUTS,
    WATERMARK_TEXT, WATERMARK_FONTSIZE, WATERMARK_OPACITY,
    TITLE_TEXT, TITLE_FONTSIZE, DATE_FONTSIZE,
    PROGRESS_BAR_HEIGHT, PROGRESS_BAR_COLOR, PROGRESS_BAR_BG_COLOR,
    CARD_READ_SPEED, CARD_MIN_DURATION, CARD_MAX_DURATION,
    DING_SOUND, BGM_SOUND,
    TEMP_DIR, OUTPUT_DIR,
)

VIDEO_SPEED = 1.5  # 视频播放倍速


# ─── 工具函数 ───

def get_duration(file_path: str) -> float:
    cmd = [FFPROBE, "-v", "quiet", "-print_format", "json", "-show_format", file_path]
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(f"ffprobe 失败: {result.stderr[:200]}")
    return float(json.loads(result.stdout)["format"]["duration"])


def get_video_dimensions(file_path: str) -> tuple[int, int]:
    cmd = [FFPROBE, "-v", "quiet", "-print_format", "json", "-show_streams", file_path]
    result = subprocess.run(cmd, capture_output=True, text=True)
    for s in json.loads(result.stdout)["streams"]:
        if s["codec_type"] == "video":
            return int(s["width"]), int(s["height"])
    raise RuntimeError("未找到视频流")


# ─── 模糊背景 ───

def generate_blur_background(video_path: str, output_path: str, duration: float) -> str:
    """从视频提取一帧，模糊放大作为竖屏背景"""
    img_path = output_path.replace(".mp4", ".jpg")
    cmd = [
        FFMPEG, "-y", "-ss", "5", "-i", video_path, "-frames:v", "1",
        "-vf", (f"scale={VIDEO_WIDTH}:{VIDEO_HEIGHT}:force_original_aspect_ratio=increase,"
                f"crop={VIDEO_WIDTH}:{VIDEO_HEIGHT},boxblur=30:10,eq=brightness=-0.08"),
        "-q:v", "3", img_path,
    ]
    subprocess.run(cmd, capture_output=True, text=True, timeout=30)
    cmd = [
        FFMPEG, "-y", "-loop", "1", "-i", img_path, "-t", str(duration),
        "-vf", f"scale={VIDEO_WIDTH}:{VIDEO_HEIGHT}",
        "-r", str(VIDEO_FPS), "-c:v", "libx264", "-preset", "fast", "-pix_fmt", "yuv420p",
        output_path,
    ]
    subprocess.run(cmd, capture_output=True, text=True, timeout=60)
    return output_path


# ─── 布局滤镜 ───

def _build_overlay_filter(src_w: int, src_h: int, layout: str) -> tuple[str, int]:
    """返回 (滤镜字符串, y偏移)"""
    scaled_h = int(VIDEO_WIDTH * src_h / src_w)
    if layout == "fit_top":
        return f"scale={VIDEO_WIDTH}:{scaled_h}", int(VIDEO_HEIGHT * 0.13)  # 下移给标题留空间
    elif layout == "blur_full":
        return f"scale={VIDEO_WIDTH}:{scaled_h}", (VIDEO_HEIGHT - scaled_h) // 2
    return f"scale={VIDEO_WIDTH}:{scaled_h}", (VIDEO_HEIGHT - scaled_h) // 2


def _build_triple_filter(src_w: int, src_h: int) -> str:
    tile_h = VIDEO_HEIGHT // 3
    return (f"scale={VIDEO_WIDTH}:{tile_h},"
            f"split=3[a][b][c];[a][b][c]vstack=inputs=3,"
            f"pad={VIDEO_WIDTH}:{VIDEO_HEIGHT}:0:0:black")


# ─── 按摘要段落提取对应视频片段 ───

def extract_matched_clips(
    segments: list[dict],
    segment_durations: list[float],
    video_paths: list[str],
    articles: list[dict],
    date_str: str,
    layout: str | None = None,
) -> str:
    """
    按摘要段落的文章索引，提取对应的原始视频片段（1.5x加速），拼接为背景视频。
    segments: [{"index": int, "text": str}, ...]
    segment_durations: 每段 TTS 音频的时长（秒）
    """
    if layout is None:
        layout = VIDEO_LAYOUT

    clip_dir = os.path.join(TEMP_DIR, date_str, "clips")
    os.makedirs(clip_dir, exist_ok=True)

    # 建立文章索引 → 视频路径的映射
    idx_to_video = {}
    for i, art in enumerate(articles):
        if i < len(video_paths):
            idx_to_video[i] = video_paths[i]

    # 生成模糊背景
    blur_bg = None
    if layout in ("fit_top", "blur_full") and video_paths:
        blur_bg_path = os.path.join(TEMP_DIR, date_str, f"blur_bg_{layout}.mp4")
        total_dur = sum(segment_durations)
        print(f"[video_editor] 生成模糊背景...")
        generate_blur_background(video_paths[0], blur_bg_path, total_dur + 5)
        blur_bg = blur_bg_path

    clip_files = []
    for seg_i, (seg, seg_dur) in enumerate(zip(segments, segment_durations)):
        clip_path = os.path.join(clip_dir, f"matched_{layout}_{seg_i:03d}.mp4")
        art_idx = seg["index"]

        # 找到对应的视频
        src_video = idx_to_video.get(art_idx) if art_idx >= 0 else None
        # INTRO/OUTRO 或找不到对应视频时，用第一个视频
        if src_video is None and video_paths:
            src_video = video_paths[0]

        if src_video is None:
            # 无视频，生成纯色背景
            _generate_color_clip(clip_path, seg_dur)
            clip_files.append(clip_path)
            continue

        try:
            src_dur = get_duration(src_video)
            src_w, src_h = get_video_dimensions(src_video)
        except Exception:
            _generate_color_clip(clip_path, seg_dur)
            clip_files.append(clip_path)
            continue

        # 原视频需要的实际时长 = TTS时长 * 倍速（因为视频要加速播放）
        needed_src_dur = seg_dur * VIDEO_SPEED
        start = max(0, (src_dur - needed_src_dur) / 2)

        ok = False
        if layout == "triple":
            vf = _build_triple_filter(src_w, src_h)
            # setpts 加速视频
            vf += f",setpts=PTS/{VIDEO_SPEED}"
            cmd = [
                FFMPEG, "-y", "-ss", str(start), "-i", src_video,
                "-t", str(needed_src_dur), "-vf", vf, "-an",
                "-r", str(VIDEO_FPS), "-c:v", "libx264", "-preset", "fast",
                clip_path,
            ]
            r = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
            ok = r.returncode == 0
        elif blur_bg:
            scale_f, y_pos = _build_overlay_filter(src_w, src_h, layout)
            # 计算此段在整体时间线中的起始位置（用于从 blur_bg 截取对应段）
            bg_offset = sum(segment_durations[:seg_i])
            cmd = [
                FFMPEG, "-y",
                "-ss", str(bg_offset), "-i", blur_bg,
                "-ss", str(start), "-i", src_video,
                "-t", str(seg_dur),
                "-filter_complex",
                f"[1:v]{scale_f},setpts=PTS/{VIDEO_SPEED}[fg];"
                f"[0:v][fg]overlay=0:{y_pos}:shortest=1",
                "-an", "-r", str(VIDEO_FPS),
                "-c:v", "libx264", "-preset", "fast",
                clip_path,
            ]
            r = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
            ok = r.returncode == 0

        if not ok:
            _generate_color_clip(clip_path, seg_dur)

        clip_files.append(clip_path)
        print(f"[video_editor] 片段 {seg_i+1}/{len(segments)}: "
              f"art[{art_idx}] → {seg_dur:.1f}s ({layout})")

    # 拼接
    concat_path = os.path.join(TEMP_DIR, date_str, f"bg_concat_{layout}.mp4")
    list_file = os.path.join(TEMP_DIR, date_str, f"concat_list_{layout}.txt")
    with open(list_file, "w") as f:
        for cf in clip_files:
            f.write(f"file '{cf}'\n")
    cmd = [FFMPEG, "-y", "-f", "concat", "-safe", "0", "-i", list_file, "-c", "copy", concat_path]
    subprocess.run(cmd, capture_output=True, text=True, timeout=120)
    print(f"[video_editor] 背景拼接完成: {concat_path}")
    return concat_path


def _generate_color_clip(path: str, duration: float):
    cmd = [
        FFMPEG, "-y", "-f", "lavfi", "-i",
        f"color=c=0x1a1a2e:s={VIDEO_WIDTH}x{VIDEO_HEIGHT}:d={duration}:r={VIDEO_FPS}",
        "-c:v", "libx264", "-preset", "fast", path,
    ]
    subprocess.run(cmd, capture_output=True, text=True, timeout=30)


# ─── 字幕 SRT → ASS ───

def _split_two_lines(text: str) -> tuple[str, str]:
    """将文本拆为上下两行。返回 (line1, line2)。短文本只用下行。"""
    text = text.strip()
    cpl = 12
    if len(text) <= cpl + 2:
        return ("", text)
    mid = len(text) // 2
    best = -1
    for offset in range(0, min(mid, 8)):
        for pos in [mid + offset, mid - offset]:
            if 0 < pos < len(text) and text[pos] in "，。、；！？：":
                # 确保拆分后两边都不会太短（至少3个字）
                if pos >= 3 and len(text) - pos - 1 >= 3:
                    best = pos
                    break
        if best >= 0:
            break
    if best >= 0:
        return (text[:best + 1], text[best + 1:])
    return (text[:cpl], text[cpl:])


def srt_to_ass(srt_path: str, ass_path: str, style_name: str | None = None,
               y_offset: int = 0) -> str:
    """SRT -> ASS. Uses \\pos for absolute pixel positioning to avoid collision detection."""
    if style_name is None:
        style_name = SUBTITLE_STYLE
    style = SUBTITLE_STYLES[style_name]

    with open(srt_path, "r", encoding="utf-8") as f:
        srt_content = f.read()

    wm_alpha = hex(int((1 - WATERMARK_OPACITY) * 255))[2:].upper().zfill(2)
    wm_color = f"&H{wm_alpha}FFFFFF"

    # Fixed Y coordinates (from top of screen)
    # 字幕位置：固定在屏幕下方，进度条上方
    line2_y = 1020
    line1_y = line2_y - style['fontsize'] - 10
    center_x = VIDEO_WIDTH // 2

    common = (f"{style['fontname']},{style['fontsize']},"
              f"{style['primary_color']},&H000000FF,"
              f"{style['outline_color']},{style['back_color']},"
              f"-1,0,0,0,100,100,0,0,"
              f"{style['border_style']},{style['outline']},{style['shadow']}")

    ass_header = f"""[Script Info]
Title: News Briefing
ScriptType: v4.00+
PlayResX: {VIDEO_WIDTH}
PlayResY: {VIDEO_HEIGHT}
Collisions: Normal

[V4+ Styles]
Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding
Style: Sub,{common},2,20,20,0,1
Style: Watermark,{style['fontname']},{WATERMARK_FONTSIZE},{wm_color},&H000000FF,&H66000000,&H00000000,0,0,0,0,100,100,0,0,1,1,0,9,0,20,15,1

[Events]
Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text
"""

    blocks = re.split(r'\n\n+', srt_content.strip())
    events = []
    max_end = "0:00:00.00"
    for block in blocks:
        lines = block.strip().split('\n')
        if len(lines) < 2:
            continue
        time_line = None
        text_parts = []
        for line in lines:
            if '-->' in line:
                time_line = line
            elif not line.strip().isdigit() and time_line is not None:
                text_parts.append(line.strip())
        if not time_line:
            continue
        raw_text = ''.join(text_parts).strip()
        if not raw_text:
            continue
        match = re.match(r'(\d+):(\d+):(\d+),(\d+)\s*-->\s*(\d+):(\d+):(\d+),(\d+)', time_line)
        if not match:
            continue
        h1, m1, s1, ms1, h2, m2, s2, ms2 = match.groups()
        start = f"{int(h1)}:{m1}:{s1}.{ms1[:2]}"
        end = f"{int(h2)}:{m2}:{s2}.{ms2[:2]}"
        max_end = end

        line1, line2 = _split_two_lines(raw_text)

        # \pos = absolute pixel position, \an8 = top-center anchor
        if line1:
            events.append(
                f"Dialogue: 0,{start},{end},Sub,,0,0,0,,"
                + "{" + f"\\pos({center_x},{line1_y})\\an8" + "}" + line1
            )
        events.append(
            f"Dialogue: 0,{start},{end},Sub,,0,0,0,,"
            + "{" + f"\\pos({center_x},{line2_y})\\an8" + "}" + line2
        )

    # 解析 max_end 为秒数用于移动水印
    _me = max_end.split(":")
    total_s = int(_me[0])*3600 + int(_me[1])*60 + float(_me[2])
    events.extend(_generate_moving_watermark_events(total_s))

    with open(ass_path, "w", encoding="utf-8") as f:
        f.write(ass_header)
        f.write('\n'.join(events))
        f.write('\n')

    sub_count = len([e for e in events if 'Watermark' not in e])
    print(f"[video_editor] ASS 字幕: {sub_count} 条, 风格={style_name}")
    return ass_path


# ─── 最终合成 ───

def compose_final_video(bg_video: str, audio_path: str, subtitle_path: str,
                        date_str: str, subtitle_style: str | None = None,
                        layout: str = "", segments: list[dict] | None = None,
                        seg_durations: list[float] | None = None) -> str:
    suffix = f"_{layout}" if layout else ""
    output_path = os.path.join(OUTPUT_DIR, f"news_briefing_{date_str}{suffix}.mp4")
    audio_dur = get_duration(audio_path)

    # 字幕 ASS（带 \pos 定位）
    sub_ass = subtitle_path.replace(".srt", f"{suffix}_sub.ass")
    srt_to_ass(subtitle_path, sub_ass, subtitle_style)

    # 标题+进度条 ASS 覆盖层
    overlay_ass = subtitle_path.replace(".srt", f"{suffix}_overlay.ass")
    if segments and seg_durations:
        build_overlay_ass(overlay_ass, date_str, segments, seg_durations, audio_dur,
                         subtitle_style)
        sub_esc = sub_ass.replace("\\", "/").replace(":", "\\:")
        ovl_esc = overlay_ass.replace("\\", "/").replace(":", "\\:")
        vf = f"ass={sub_esc},ass={ovl_esc}"
    else:
        sub_esc = sub_ass.replace("\\", "/").replace(":", "\\:")
        vf = f"ass={sub_esc}"

    cmd = [
        FFMPEG, "-y",
        "-i", bg_video, "-i", audio_path,
        "-t", str(audio_dur),
        "-vf", vf,
        "-c:v", "libx264", "-preset", "medium", "-crf", "23",
        "-c:a", "aac", "-b:a", "128k",
        "-shortest", "-movflags", "+faststart",
        output_path,
    ]
    print(f"[video_editor] 合成视频 ({layout or 'default'})...")
    result = subprocess.run(cmd, capture_output=True, text=True, timeout=600)
    if result.returncode != 0:
        raise RuntimeError(f"视频合成失败: {result.stderr[-500:]}")

    print(f"[video_editor] 输出: {output_path}")
    return output_path



# ─── 标题/进度条 ASS 覆盖层 ───


# ─── 首图生成 ───

def generate_cover_image(date_str: str, video_paths: list[str], output_dir: str | None = None) -> str:
    """生成封面首图：模糊背景 + 大字标题居中"""
    if output_dir is None:
        output_dir = OUTPUT_DIR
    out_path = os.path.join(output_dir, f"cover_{date_str}.jpg")
    date_display = f"{date_str[:4]}年{int(date_str[4:6])}月{int(date_str[6:8])}日"
    font = SUBTITLE_STYLES[SUBTITLE_STYLE]['fontname']

    # 模糊背景
    bg_img = os.path.join(TEMP_DIR, date_str, "cover_bg.jpg")
    if video_paths:
        subprocess.run([
            FFMPEG, "-y", "-ss", "15", "-i", video_paths[0], "-frames:v", "1",
            "-vf", (f"scale={VIDEO_WIDTH}:{VIDEO_HEIGHT}:force_original_aspect_ratio=increase,"
                    f"crop={VIDEO_WIDTH}:{VIDEO_HEIGHT},boxblur=30:10,eq=brightness=-0.2"),
            "-q:v", "2", bg_img], capture_output=True, text=True, timeout=30)

    # ASS 覆盖大标题
    ass_path = os.path.join(TEMP_DIR, date_str, "cover.ass")
    cx = VIDEO_WIDTH // 2
    cy = VIDEO_HEIGHT // 2
    ass = f"""[Script Info]
ScriptType: v4.00+
PlayResX: {VIDEO_WIDTH}
PlayResY: {VIDEO_HEIGHT}

[V4+ Styles]
Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding
Style: S,{font},40,&H00FFFFFF,&H000000FF,&H00000000,&H00000000,-1,0,0,0,100,100,0,0,1,3,0,5,20,20,0,1

[Events]
Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text
"""
    ev = []
    ev.append(f"Dialogue: 0,0:00:00.00,0:00:01.00,S,,0,0,0,,"
              + "{" + f"\\pos({cx},{cy - 50})\\an5\\fs88\\b1" + "}" + TITLE_TEXT)
    ev.append(f"Dialogue: 0,0:00:00.00,0:00:01.00,S,,0,0,0,,"
              + "{" + f"\\pos({cx},{cy + 50})\\an5\\fs48\\c&HDDDDDD&" + "}" + date_display)
    # 水印
    ev.append(f"Dialogue: 0,0:00:00.00,0:00:01.00,S,,0,0,0,,"
              + "{\\an9\\fs24\\c&H66FFFFFF&}" + WATERMARK_TEXT)

    with open(ass_path, "w", encoding="utf-8") as f:
        f.write(ass)
        f.write('\n'.join(ev))
        f.write('\n')

    ass_esc = ass_path.replace("\\", "/").replace(":", "\\:")
    if os.path.exists(bg_img):
        cmd = [FFMPEG, "-y", "-i", bg_img, "-vf", f"ass={ass_esc}", "-q:v", "2", out_path]
    else:
        cmd = [FFMPEG, "-y", "-f", "lavfi", "-i",
               f"color=c=0x1a1a2e:s={VIDEO_WIDTH}x{VIDEO_HEIGHT}",
               "-frames:v", "1", "-vf", f"ass={ass_esc}", "-q:v", "2", out_path]
    subprocess.run(cmd, capture_output=True, text=True, timeout=30)
    print(f"[cover] 首图: {out_path}")
    return out_path


# ─── 文字稿生成 ───

def generate_text_article(segments: list[dict], date_str: str, output_dir: str | None = None) -> str:
    """生成格式化的文字稿"""
    if output_dir is None:
        output_dir = OUTPUT_DIR
    out_path = os.path.join(output_dir, f"article_{date_str}.txt")
    date_display = f"{date_str[:4]}年{int(date_str[4:6])}月{int(date_str[6:8])}日"

    lines = []
    lines.append(f"{'='*40}")
    lines.append(f"  {TITLE_TEXT}")
    lines.append(f"  {date_display}")
    lines.append(f"{'='*40}")
    lines.append("")

    news_idx = 0
    total_news = sum(1 for s in segments if s['index'] >= 0)
    for seg in segments:
        if seg['index'] == -1:
            lines.append(seg['text'])
            lines.append("")
        elif seg['index'] == -2:
            lines.append(seg['text'])
        else:
            news_idx += 1
            title = seg.get('title', '')
            lines.append(f"【{news_idx}/{total_news}】{title}")
            lines.append(seg['text'])
            lines.append("")

    lines.append("")
    lines.append(f"{'='*40}")

    with open(out_path, "w", encoding="utf-8") as f:
        f.write('\n'.join(lines))
    print(f"[article] 文字稿: {out_path}")
    return out_path

def build_overlay_ass(ass_path: str, date_str: str, segments: list[dict],
                      seg_durations: list[float], total_duration: float,
                      style_name: str | None = None, y_offset: int = 0) -> str:
    """生成标题+时间进度条+新闻标题的 ASS 覆盖层（适用所有布局）"""
    if style_name is None:
        style_name = SUBTITLE_STYLE
    style = SUBTITLE_STYLES[style_name]
    font = style['fontname']
    date_display = f"{date_str[:4]}年{int(date_str[4:6])}月{int(date_str[6:8])}日"

    # 布局位置
    title_y = 45
    date_y = 105
    bar_x1, bar_x2 = 40, VIDEO_WIDTH - 40
    bar_w = bar_x2 - bar_x1
    # 进度条和新闻标题：固定在屏幕底部（避开平台UI）
    bar_y = 1085
    news_label_y = bar_y + PROGRESS_BAR_HEIGHT + 20

    # 段落起止时间
    seg_times = []
    t = 0.0
    for d in seg_durations:
        seg_times.append((t, t + d))
        t += d

    wm_alpha = hex(int((1 - WATERMARK_OPACITY) * 255))[2:].upper().zfill(2)
    wm_color = f"&H{wm_alpha}FFFFFF"
    cx = VIDEO_WIDTH // 2
    end_t = _ass_time(total_duration)

    ass = f"""[Script Info]
ScriptType: v4.00+
PlayResX: {VIDEO_WIDTH}
PlayResY: {VIDEO_HEIGHT}
Collisions: Normal

[V4+ Styles]
Format: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding
Style: S,{font},40,&H00FFFFFF,&H000000FF,&H00000000,&H00000000,-1,0,0,0,100,100,0,0,1,2,0,8,20,20,0,1
Style: Watermark,{font},{WATERMARK_FONTSIZE},{wm_color},&H000000FF,&H66000000,&H00000000,0,0,0,0,100,100,0,0,1,1,0,9,0,20,15,1

[Events]
Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text
"""
    ev = []
    # 标题（全程）
    ev.append(f"Dialogue: 2,0:00:00.00,{end_t},S,,0,0,0,,"
              + "{" + f"\\pos({cx},{title_y})\\an8\\fs{TITLE_FONTSIZE}\\b1" + "}" + TITLE_TEXT)
    ev.append(f"Dialogue: 2,0:00:00.00,{end_t},S,,0,0,0,,"
              + "{" + f"\\pos({cx},{date_y})\\an8\\fs{DATE_FONTSIZE}\\c&HAAAAAA&" + "}" + date_display)
    # 水印
    ev.extend(_generate_moving_watermark_events(total_duration))
    # 进度条背景（全程）
    ev.append(f"Dialogue: 1,0:00:00.00,{end_t},S,,0,0,0,,"
              + "{" + f"\\pos({bar_x1},{bar_y})\\an7\\c&H{PROGRESS_BAR_BG_COLOR}&\\p1" + "}"
              + f"m 0 0 l {bar_w} 0 l {bar_w} {PROGRESS_BAR_HEIGHT} l 0 {PROGRESS_BAR_HEIGHT}")

    # 时间线性进度条：每0.5秒一帧，线性增长
    step = 0.5
    t = 0.0
    while t < total_duration:
        t_end = min(t + step, total_duration)
        frac = t_end / total_duration
        pw = int(bar_w * frac)
        s, e = _ass_time(t), _ass_time(t_end)
        ev.append(f"Dialogue: 2,{s},{e},S,,0,0,0,,"
                  + "{" + f"\\pos({bar_x1},{bar_y})\\an7\\c&H{PROGRESS_BAR_COLOR}&\\p1" + "}"
                  + f"m 0 0 l {pw} 0 l {pw} {PROGRESS_BAR_HEIGHT} l 0 {PROGRESS_BAR_HEIGHT}")
        t = t_end

    # 新闻标题（按段落时间切换）
    news_idx = 0
    total_news = sum(1 for sg in segments if sg['index'] >= 0)
    for seg, (t_start, t_end) in zip(segments, seg_times):
        if seg['index'] >= 0:
            news_idx += 1
        title = seg.get('title', '')
        if seg['index'] >= 0 and title:
            label = f"{news_idx}/{total_news}  {title}"
        elif seg['index'] == -1:
            label = "开场"
        else:
            label = ""
        if label:
            s, e = _ass_time(t_start), _ass_time(t_end)
            ev.append(f"Dialogue: 2,{s},{e},S,,0,0,0,,"
                      + "{" + f"\\pos({cx},{news_label_y})\\an8\\fs34\\c&HBBBBBB&" + "}" + label)

    with open(ass_path, "w", encoding="utf-8") as f:
        f.write(ass)
        f.write('\n'.join(ev))
        f.write('\n')
    return ass_path


def _ass_time(seconds: float) -> str:
    h = int(seconds // 3600)
    m = int((seconds % 3600) // 60)
    s = seconds % 60
    return f"{h}:{m:02d}:{s:05.2f}"


def _generate_moving_watermark_events(total_duration: float, style: str = "Watermark") -> list[str]:
    """生成缓慢随机移动的水印 ASS 事件"""
    import random
    random.seed(42)  # 可复现
    events = []
    seg_dur = 8.0  # 每段移动8秒
    margin = 30
    t = 0.0
    # 初始位置
    x, y = VIDEO_WIDTH - 120, 30
    while t < total_duration:
        t_end = min(t + seg_dur, total_duration)
        # 随机目标位置（在屏幕边缘区域漂移）
        nx = random.randint(margin, VIDEO_WIDTH - margin - 80)
        ny = random.randint(margin, int(VIDEO_HEIGHT * 0.15))
        # 偶尔移到中间或底部区域
        if random.random() < 0.3:
            ny = random.randint(int(VIDEO_HEIGHT * 0.3), int(VIDEO_HEIGHT * 0.7))
        s = _ass_time(t)
        e = _ass_time(t_end)
        events.append(
            f"Dialogue: 3,{s},{e},{style},,0,0,0,,"
            + "{" + f"\\move({x},{y},{nx},{ny})" + "}" + WATERMARK_TEXT
        )
        x, y = nx, ny
        t = t_end
    return events


# ─── 音效 ───

def prepend_ding(audio_path: str, output_path: str) -> str:
    if not os.path.exists(DING_SOUND):
        return audio_path
    cmd = [FFMPEG, "-y", "-i", DING_SOUND, "-i", audio_path,
           "-filter_complex", "[0][1]concat=n=2:v=0:a=1",
           "-c:a", "aac", "-b:a", "128k", output_path]
    r = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
    return output_path if r.returncode == 0 else audio_path


# ─── 风格B：图片卡片 ───

def generate_card_video(segments: list[dict], video_paths: list[str],
                        articles: list[dict], date_str: str) -> str:
    """图片卡片模式：Pillow 排版引擎 + BGM"""
    from card_renderer import generate_card_pages
    card_dir = os.path.join(TEMP_DIR, date_str, "cards")
    os.makedirs(card_dir, exist_ok=True)

    # 模糊背景图
    bg_img = os.path.join(card_dir, "bg.jpg")
    if video_paths:
        subprocess.run([
            FFMPEG, "-y", "-ss", "10", "-i", video_paths[0], "-frames:v", "1",
            "-vf", (f"scale={VIDEO_WIDTH}:{VIDEO_HEIGHT}:force_original_aspect_ratio=increase,"
                    f"crop={VIDEO_WIDTH}:{VIDEO_HEIGHT},boxblur=40:15,eq=brightness=-0.15"),
            "-q:v", "3", bg_img], capture_output=True, text=True, timeout=30)
    bg = bg_img if os.path.exists(bg_img) else None

    # Pillow 渲染卡片
    pages = generate_card_pages(segments, bg, date_str, video_paths)
    if not pages:
        raise RuntimeError("没有生成任何卡片")

    # 每张图片转为视频片段
    clip_files = []
    for img_path, dur in pages:
        clip_path = img_path.replace(".png", ".mp4")
        cmd = [FFMPEG, "-y", "-loop", "1", "-i", img_path, "-t", str(dur),
               "-r", str(VIDEO_FPS), "-c:v", "libx264", "-preset", "fast",
               "-pix_fmt", "yuv420p", clip_path]
        subprocess.run(cmd, capture_output=True, text=True, timeout=30)
        clip_files.append(clip_path)

    # 拼接
    concat_path = os.path.join(card_dir, "concat.mp4")
    list_file = os.path.join(card_dir, "concat.txt")
    with open(list_file, "w") as f:
        for cf in clip_files:
            f.write(f"file '{cf}'\n")
    subprocess.run([FFMPEG, "-y", "-f", "concat", "-safe", "0", "-i", list_file,
                    "-c", "copy", concat_path], capture_output=True, text=True, timeout=60)

    # BGM + 叮咚
    total_dur = sum(d for _, d in pages)
    output_path = os.path.join(OUTPUT_DIR, f"news_briefing_{date_str}_cards.mp4")

    bgm_trimmed = os.path.join(card_dir, "bgm_t.mp3")
    subprocess.run([FFMPEG, "-y", "-i", BGM_SOUND, "-t", str(total_dur + 1),
                    "-af", f"afade=t=out:st={max(0,total_dur-2)}:d=2",
                    bgm_trimmed], capture_output=True, text=True, timeout=30)

    # 叮咚混入 BGM
    mixed = os.path.join(card_dir, "mixed.mp3")
    r = subprocess.run([FFMPEG, "-y", "-i", DING_SOUND, "-i", bgm_trimmed,
                    "-filter_complex",
                    "[0]volume=1.5[d];[1]volume=1.0[b];[d][b]amix=inputs=2:duration=longest",
                    mixed], capture_output=True, text=True, timeout=30)
    audio = mixed if r.returncode == 0 else bgm_trimmed

    # 合成
    subprocess.run([FFMPEG, "-y", "-i", concat_path, "-i", audio,
                   "-map", "0:v", "-map", "1:a", "-c:v", "copy", "-c:a", "aac", "-b:a", "128k",
                   "-shortest", "-movflags", "+faststart", output_path],
                  capture_output=True, text=True, timeout=120)

    print(f"[card] 输出: {output_path} ({total_dur:.0f}s, {len(pages)} 页)")
    return output_path

# ─── 测试函数 ───

def test_split_two_lines():
    """测试分行逻辑，覆盖边界情况"""
    cases = [
        ("短句测试", "", "短句测试"),
        ("扎实推进乡村全面振兴。", "", "扎实推进乡村全面振兴。"),
        ("为世界注入确定性和正能量。", "", "为世界注入确定性和正能量。"),  # 13字，不应拆
        ("各位观众，以下是今日新闻联播要闻速览。", "各位观众，", "以下是今日新闻联播要闻速览。"),
        ("全国春季农业生产工作会议3月20日在河南周口召开。", None, None),  # 只检查不崩溃
    ]
    print("=== 分行逻辑测试 ===")
    all_ok = True
    for text, exp_l1, exp_l2 in cases:
        l1, l2 = _split_two_lines(text)
        # 核心断言：下行不能只有标点
        if l2 and len(l2) <= 2 and all(c in "，。、；！？：" for c in l2):
            print(f"  ❌ BUG 下行只有标点: [{text}] → L1=[{l1}] L2=[{l2}]")
            all_ok = False
            continue
        # 核心断言：上行不能只有标点
        if l1 and len(l1) <= 2 and all(c in "，。、；！？：" for c in l1):
            print(f"  ❌ BUG 上行只有标点: [{text}] → L1=[{l1}] L2=[{l2}]")
            all_ok = False
            continue
        if exp_l1 is not None:
            ok = l1 == exp_l1 and l2 == exp_l2
            print(f"  {'✅' if ok else '❌'} [{len(text):>2}字] L1=[{l1}] L2=[{l2}]")
            if not ok:
                all_ok = False
        else:
            print(f"  ✅ [{len(text):>2}字] L1=[{l1}] L2=[{l2}]")
    assert all_ok, "分行测试失败"
    print()


def test_subtitle_burn(date_str: str = "test"):
    out_dir = os.path.join(TEMP_DIR, date_str)
    os.makedirs(out_dir, exist_ok=True)

    # 测试各种长度的字幕，包括边界情况
    srt_path = os.path.join(out_dir, "test_sub.srt")
    with open(srt_path, "w", encoding="utf-8") as f:
        f.write("1\n00:00:00,000 --> 00:00:02,000\n短句测试\n\n")
        f.write("2\n00:00:02,000 --> 00:00:04,000\n为世界注入确定性和正能量。\n\n")
        f.write("3\n00:00:04,000 --> 00:00:06,000\n各位观众，以下是今日新闻联播要闻速览。\n\n")
        f.write("4\n00:00:06,000 --> 00:00:08,000\n全国春季农业生产工作会议3月20日在河南周口召开。\n\n")

    # 只测一种风格，重点验证内容正确性
    style_name = "news"
    ass_path = os.path.join(out_dir, f"test_sub_{style_name}.ass")
    srt_to_ass(srt_path, ass_path, style_name)

    with open(ass_path) as f:
        content = f.read()
    dialogues = [l for l in content.split('\n') if l.startswith('Dialogue:') and 'Watermark' not in l]

    all_ok = True
    for d in dialogues:
        text = d.split(',', 9)[-1]
        # 去掉 \pos 标签取纯文本
        import re as _re
        pure = _re.sub(r'\{[^}]*\}', '', text).strip()
        print(f"    → [{len(pure):>2}字] {pure}")
        if len(pure) <= 2 and all(c in "，。、；！？：" for c in pure):
            print(f"    ❌ BUG: 只有标点!")
            all_ok = False

    # 生成测试视频并截图验证
    out_video = os.path.join(out_dir, f"test_subtitle_{style_name}.mp4")
    ass_escaped = ass_path.replace("\\", "/").replace(":", "\\:")
    cmd = [
        FFMPEG, "-y", "-f", "lavfi", "-i",
        f"color=c=0x1a1a2e:s={VIDEO_WIDTH}x{VIDEO_HEIGHT}:d=8:r={VIDEO_FPS}",
        "-vf", f"ass={ass_escaped}",
        "-c:v", "libx264", "-preset", "fast", out_video,
    ]
    r = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
    print(f"  {'✅' if r.returncode==0 else '❌'} 视频: {out_video}")

    # 截帧验证无 \u3000 等异常字符
    for t in [1, 3, 5, 7]:
        frame = os.path.join(out_dir, f"test_frame_{t}s.jpg")
        subprocess.run([FFMPEG, "-y", "-ss", str(t), "-i", out_video,
                       "-frames:v", "1", "-update", "1", frame],
                      capture_output=True, timeout=10)
    print(f"  截帧: temp/{date_str}/test_frame_*.jpg (请目视检查)")

    assert all_ok, "字幕内容测试失败"


def test_layout(date_str: str = "test"):
    import glob
    videos = sorted(glob.glob(os.path.join(TEMP_DIR, "*/videos/clip_000.mp4")))
    if not videos:
        print("  ⚠️ 没有视频文件，跳过")
        return
    src = videos[0]
    src_w, src_h = get_video_dimensions(src)
    out_dir = os.path.join(TEMP_DIR, date_str)
    os.makedirs(out_dir, exist_ok=True)

    blur_bg = os.path.join(out_dir, "test_blur_bg.mp4")
    generate_blur_background(src, blur_bg, 5)

    for layout in VIDEO_LAYOUTS:
        out_video = os.path.join(out_dir, f"test_layout_{layout}.mp4")
        if layout == "triple":
            vf = _build_triple_filter(src_w, src_h) + f",setpts=PTS/{VIDEO_SPEED}"
            cmd = [FFMPEG, "-y", "-i", src, "-t", "5", "-vf", vf, "-an",
                   "-r", str(VIDEO_FPS), "-c:v", "libx264", "-preset", "fast", out_video]
        else:
            scale_f, y_pos = _build_overlay_filter(src_w, src_h, layout)
            cmd = [
                FFMPEG, "-y", "-i", blur_bg, "-i", src, "-t", "5",
                "-filter_complex",
                f"[1:v]{scale_f},setpts=PTS/{VIDEO_SPEED}[fg];"
                f"[0:v][fg]overlay=0:{y_pos}:shortest=1",
                "-an", "-r", str(VIDEO_FPS), "-c:v", "libx264", "-preset", "fast", out_video,
            ]
        r = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
        if r.returncode == 0:
            w, h = get_video_dimensions(out_video)
            print(f"  ✅ 布局 ({layout}): {out_video} [{w}x{h}] @{VIDEO_SPEED}x")
        else:
            print(f"  ❌ 布局失败 ({layout}): {r.stderr[-200:]}")


if __name__ == "__main__":
    print("=== 字幕+水印测试 ===")
    test_subtitle_burn()
    print("\n=== 布局+倍速测试 ===")
    test_layout()
