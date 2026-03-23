"""文稿摘要模块 - 通过 LLM 将长文稿压缩为短配音稿，保留文章索引对应关系"""
import subprocess
import re
import os
import json
from config import TARGET_DURATION, TEMP_DIR

CHARS_PER_SECOND = 5.5  # +35% 语速下约 5.5 字/秒


def estimate_max_chars(duration_seconds: int = TARGET_DURATION) -> int:
    return int(duration_seconds * CHARS_PER_SECOND)


def _strip_ansi(text: str) -> str:
    return re.sub(r'\x1b\[[0-9;]*[a-zA-Z]|\x1b\[\?[0-9]*[a-zA-Z]', '', text)


def _clean_kiro_output(text: str) -> str:
    text = _strip_ansi(text)
    lines = text.strip().split('\n')
    cleaned = []
    for line in lines:
        line = line.strip()
        if line.startswith('> '):
            line = line[2:]
        if line.startswith('▸ Credits:') or line.startswith('Credits:'):
            continue
        cleaned.append(line)
    return '\n'.join(cleaned).strip()


def summarize_via_kiro(full_text: str, articles: list[dict],
                       max_chars: int | None = None) -> list[dict]:
    """
    通过 kiro-cli 生成摘要，返回带文章索引的摘要段落列表。
    返回: [{"index": 0, "text": "摘要文本"}, ...]
    """
    if max_chars is None:
        max_chars = estimate_max_chars()

    # 构建带编号的文章列表
    article_list = ""
    for i, art in enumerate(articles):
        title = art.get("title", "")
        content = art.get("content", "")[:300]
        article_list += f"[{i}] {title}: {content}\n"

    prompt = (
        f"你是一个专业的新闻编辑。请将以下新闻联播文章列表压缩为一份简短的新闻简报。\n\n"
        f"要求：\n"
        f"1. 总字数严格控制在{max_chars}字以内\n"
        f"2. 保留最重要的5-8条新闻\n"
        f"3. 每条新闻用1-3句话概括，语言简洁正式，适合新闻播报\n"
        f"4. 关键：每条摘要前必须标注对应的原始文章编号和简短标题（≤8字），格式为 [编号|标题] 摘要内容\n"
        f"5. 第一行固定为：[INTRO|开场] 各位观众，以下是今日新闻联播要闻速览。\n"
        f"6. 最后一行固定为：[OUTRO|结束] 以上就是今日新闻联播的主要内容。\n"
        f"7. 每条之间用空行分隔\n\n"
        f"示例格式：\n"
        f"[INTRO|开场] 各位观众，以下是今日新闻联播要闻速览。\n\n"
        f"[2|春季农业会议] 全国春季农业生产工作会议在河南召开...\n\n"
        f"[OUTRO|结束] 以上就是今日新闻联播的主要内容。\n\n"
        f"文章列表：\n{article_list}"
    )

    prompt_file = os.path.join(TEMP_DIR, "summary_prompt.txt")
    os.makedirs(os.path.dirname(prompt_file), exist_ok=True)
    with open(prompt_file, "w", encoding="utf-8") as f:
        f.write(prompt)

    print(f"[summarizer] 调用 kiro-cli 生成摘要（目标 {max_chars} 字）...")
    result = subprocess.run(
        ["bash", "-c", f'kiro-cli chat --no-interactive "$(cat {prompt_file})"'],
        capture_output=True, text=True, timeout=120,
    )
    if result.returncode != 0:
        raise RuntimeError(f"kiro-cli 调用失败: {result.stderr[:500]}")

    raw = _clean_kiro_output(result.stdout)
    if not raw:
        raise RuntimeError("kiro-cli 返回空结果")

    segments = _parse_indexed_summary(raw)
    total_chars = sum(len(s["text"]) for s in segments)
    print(f"[summarizer] 摘要: {total_chars} 字, {len(segments)} 段")
    return segments


def _parse_indexed_summary(text: str) -> list[dict]:
    """解析带 [编号|标题] 标记的摘要文本"""
    segments = []
    for block in re.split(r'\n\n+', text.strip()):
        block = block.strip()
        if not block:
            continue
        # 匹配 [数字|标题] 或 [INTRO|标题] 或 [OUTRO|标题]
        m = re.match(r'\[(\d+|INTRO|OUTRO)\|([^\]]*)\]\s*(.*)', block, re.DOTALL)
        if not m:
            # fallback: 旧格式 [数字]
            m2 = re.match(r'\[(\d+|INTRO|OUTRO)\]\s*(.*)', block, re.DOTALL)
            if m2:
                tag, content = m2.group(1), m2.group(2).strip()
                title = content[:8] if content else ""
            else:
                segments.append({"index": -1, "title": "", "text": block})
                continue
        else:
            tag, title, content = m.group(1), m.group(2).strip(), m.group(3).strip()

        if tag == "INTRO":
            segments.append({"index": -1, "title": title or "开场", "text": content})
        elif tag == "OUTRO":
            segments.append({"index": -2, "title": title or "结束", "text": content})
        else:
            segments.append({"index": int(tag), "title": title, "text": content})
    return segments


def summarize(full_text: str, articles: list[dict], method: str = "kiro") -> list[dict]:
    """
    摘要入口。返回 [{"index": int, "text": str}, ...]
    index: -1=INTRO, -2=OUTRO, >=0=对应文章索引
    """
    if method == "kiro":
        return summarize_via_kiro(full_text, articles)
    raise ValueError(f"不支持的摘要方法: {method}")
