"""TTS 语音生成模块 - 按段落分别生成，精确时间轴"""
import asyncio
import edge_tts
import os
import subprocess
import json
from config import TTS_VOICE, TTS_RATE, TEMP_DIR, FFMPEG, FFPROBE

_SENTENCE_BREAKS = set("。！？；\n")
_CLAUSE_BREAKS = set("，、：")
# 每行最多字数（720p竖屏 58pt 字体）
_CHARS_PER_LINE = 12
# 每条字幕最多总字数（两行）
_MAX_PER_SUB = _CHARS_PER_LINE * 2


async def _tts_segment(text: str, output_path: str) -> list[dict]:
    communicate = edge_tts.Communicate(text, TTS_VOICE, rate=TTS_RATE)
    audio_chunks = []
    sub_entries = []
    async for chunk in communicate.stream():
        if chunk["type"] == "audio":
            audio_chunks.append(chunk["data"])
        elif chunk["type"] in ("WordBoundary", "SentenceBoundary"):
            sub_entries.append({
                "offset": chunk["offset"],
                "duration": chunk["duration"],
                "text": chunk["text"],
            })
    with open(output_path, "wb") as f:
        for c in audio_chunks:
            f.write(c)
    return sub_entries


def _get_audio_duration(path: str) -> float:
    cmd = [FFPROBE, "-v", "quiet", "-print_format", "json", "-show_format", path]
    r = subprocess.run(cmd, capture_output=True, text=True)
    return float(json.loads(r.stdout)["format"]["duration"])


def _split_for_subtitles(text: str) -> list[str]:
    """将长文本拆分为多条字幕，每条 ≤ _MAX_PER_SUB 字"""
    text = text.strip()
    if not text:
        return []
    if len(text) <= _MAX_PER_SUB:
        return [text]

    clauses = []
    current = ""
    for ch in text:
        current += ch
        if ch in _SENTENCE_BREAKS | _CLAUSE_BREAKS and len(current) >= 4:
            clauses.append(current)
            current = ""
    if current:
        clauses.append(current)

    result = []
    buf = ""
    for clause in clauses:
        if len(buf) + len(clause) <= _MAX_PER_SUB:
            buf += clause
        else:
            if buf:
                result.append(buf)
            buf = clause
    if buf:
        result.append(buf)
    return result if result else [text[:_MAX_PER_SUB]]


def generate_tts_by_segments(segments: list[dict], date_str: str) -> tuple[str, str, list[float]]:
    out_dir = os.path.join(TEMP_DIR, date_str)
    os.makedirs(out_dir, exist_ok=True)
    seg_dir = os.path.join(out_dir, "tts_segments")
    os.makedirs(seg_dir, exist_ok=True)

    print(f"[tts] 按段落生成语音: {TTS_VOICE}, 语速: {TTS_RATE}")

    seg_files = []
    seg_durations = []
    all_srt_entries = []
    time_offset = 0.0

    for i, seg in enumerate(segments):
        seg_path = os.path.join(seg_dir, f"seg_{i:03d}.mp3")
        print(f"[tts] 段落 {i+1}/{len(segments)}: [{seg['index']:>2}] {seg['text'][:25]}...")

        sub_entries = asyncio.run(_tts_segment(seg["text"], seg_path))
        dur = _get_audio_duration(seg_path)
        seg_files.append(seg_path)
        seg_durations.append(dur)

        if sub_entries:
            for entry in sub_entries:
                entry_start = entry["offset"] / 10_000_000
                entry_end = (entry["offset"] + entry["duration"]) / 10_000_000
                raw_text = entry["text"].strip()
                if not raw_text:
                    continue
                subs = _split_for_subtitles(raw_text)
                total_chars = sum(len(s) for s in subs)
                t = entry_start
                for sub_text in subs:
                    sub_dur = (entry_end - entry_start) * len(sub_text) / total_chars if total_chars else 0
                    all_srt_entries.append({
                        "start": time_offset + t,
                        "end": time_offset + t + sub_dur,
                        "text": sub_text,  # 纯文本，不做格式化
                    })
                    t += sub_dur
        else:
            subs = _split_for_subtitles(seg["text"])
            total_chars = sum(len(s) for s in subs)
            t = 0.0
            for sub_text in subs:
                sub_dur = dur * len(sub_text) / total_chars if total_chars else dur
                all_srt_entries.append({
                    "start": time_offset + t,
                    "end": time_offset + t + sub_dur,
                    "text": sub_text,
                })
                t += sub_dur

        time_offset += dur

    # 拼接音频
    audio_path = os.path.join(out_dir, "tts_audio.mp3")
    list_file = os.path.join(seg_dir, "concat.txt")
    with open(list_file, "w") as f:
        for sf in seg_files:
            f.write(f"file '{sf}'\n")
    subprocess.run([FFMPEG, "-y", "-f", "concat", "-safe", "0", "-i", list_file, "-c", "copy", audio_path],
                   capture_output=True, text=True, timeout=60)

    # 写 SRT（纯文本，单行）
    subtitle_path = os.path.join(out_dir, "subtitles.srt")
    with open(subtitle_path, "w", encoding="utf-8") as f:
        for i, s in enumerate(all_srt_entries, 1):
            f.write(f"{i}\n{_fmt(s['start'])} --> {_fmt(s['end'])}\n{s['text']}\n\n")

    total_dur = _get_audio_duration(audio_path)
    print(f"[tts] 音频: {audio_path} ({total_dur:.1f}s)")
    print(f"[tts] 字幕: {subtitle_path} ({len(all_srt_entries)} 条)")
    print(f"[tts] 段落时长: {['%.1f' % d for d in seg_durations]}")
    return audio_path, subtitle_path, seg_durations


def _fmt(seconds: float) -> str:
    h = int(seconds // 3600)
    m = int((seconds % 3600) // 60)
    s = int(seconds % 60)
    ms = int((seconds % 1) * 1000)
    return f"{h:02d}:{m:02d}:{s:02d},{ms:03d}"


def generate_tts(text: str, date_str: str) -> tuple[str, str]:
    segs = [{"index": -1, "text": text}]
    audio, srt, _ = generate_tts_by_segments(segs, date_str)
    return audio, srt


if __name__ == "__main__":
    print("=== TTS 分段测试 ===")
    segs = [
        {"index": -1, "text": "各位观众，以下是今日新闻联播要闻速览。"},
        {"index": 0, "text": "全国经济形势持续向好，各项指标稳中有进。"},
    ]
    _, srt, durs = generate_tts_by_segments(segs, "test")
    print(f"段落时长: {durs}")
    with open(srt) as f:
        print(f.read())
