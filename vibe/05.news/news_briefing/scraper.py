"""央视网新闻联播数据抓取"""
import re
import json
import requests
from bs4 import BeautifulSoup
from datetime import datetime, timedelta


def get_article_list_url(date_str: str) -> str:
    """获取指定日期新闻联播文章列表页 URL"""
    return f"https://tv.cctv.com/lm/xwlb/day/{date_str}.shtml"


def fetch_article_links(date_str: str) -> list[dict]:
    """获取指定日期所有新闻联播条目的链接和标题"""
    url = get_article_list_url(date_str)
    resp = requests.get(url, timeout=30)
    resp.encoding = "utf-8"
    soup = BeautifulSoup(resp.text, "html.parser")
    items = []
    for li in soup.select("li"):
        # 找第二个 <a>（第一个是图片链接，第二个是标题链接）
        links = li.find_all("a")
        for a in links:
            href = a.get("href", "")
            if not href or "shtml" not in href:
                continue
            # 优先从 title 属性获取标题
            title = a.get("title", "") or a.get("alt", "") or a.get_text(strip=True)
            # 清理标题：去掉 [视频] 前缀和【】标记
            title = re.sub(r'^\[视频\]', '', title).strip()
            title = re.sub(r'【[^】]*】', '', title).strip()
            # 跳过完整版（第一条是整期视频）
            if "《新闻联播》" in title and "19:00" in title:
                continue
            if href not in [item["url"] for item in items]:
                items.append({"title": title, "url": href})
    return items


def fetch_article_content(article_url: str) -> str:
    """获取单篇文章的正文内容"""
    resp = requests.get(article_url, timeout=30)
    resp.encoding = "utf-8"
    soup = BeautifulSoup(resp.text, "html.parser")
    content_div = soup.select_one("#content_area") or soup.select_one(".content_area")
    if content_div:
        return content_div.get_text(strip=True)
    # fallback: 取所有 p 标签
    paragraphs = soup.select("p")
    return "\n".join(p.get_text(strip=True) for p in paragraphs if p.get_text(strip=True))


def fetch_video_url(article_url: str) -> str | None:
    """从文章页面提取视频 m3u8 地址"""
    resp = requests.get(article_url, timeout=30)
    resp.encoding = "utf-8"
    # 央视网视频 ID 通常在页面 JS 中
    guid_match = re.search(r'var\s+guid\s*=\s*"([^"]+)"', resp.text)
    if not guid_match:
        # 尝试其他模式
        guid_match = re.search(r'"guid"\s*:\s*"([^"]+)"', resp.text)
    if not guid_match:
        return None
    guid = guid_match.group(1)
    # 通过央视 API 获取视频流地址
    api_url = f"https://vdn.apps.cntv.cn/api/getHttpVideoInfo.do?pid={guid}"
    try:
        api_resp = requests.get(api_url, timeout=30)
        data = api_resp.json()
        # 尝试获取 hls_url 或 video.chapters
        hls_url = data.get("hls_url")
        if hls_url:
            return hls_url
        # 备选：直接 mp4
        video = data.get("video", {})
        chapters = video.get("chapters", [])
        if chapters:
            return chapters[0].get("url")
    except Exception:
        pass
    return None


def scrape_news(date_str: str | None = None) -> dict:
    """
    抓取指定日期的新闻联播内容。
    返回 {"date": str, "articles": [...], "full_text": str, "video_urls": [...]}
    """
    if date_str is None:
        # 默认取今天，如果晚间还没更新则取昨天
        now = datetime.now()
        if now.hour < 20:
            date_str = (now - timedelta(days=1)).strftime("%Y%m%d")
        else:
            date_str = now.strftime("%Y%m%d")

    print(f"[scraper] 获取 {date_str} 新闻联播...")
    articles = fetch_article_links(date_str)
    if not articles:
        raise RuntimeError(f"未找到 {date_str} 的新闻联播内容")

    print(f"[scraper] 找到 {len(articles)} 条新闻")
    full_text_parts = []
    video_urls = []

    for i, art in enumerate(articles):
        print(f"[scraper] ({i+1}/{len(articles)}) {art['title'][:30]}...")
        try:
            content = fetch_article_content(art["url"])
            art["content"] = content
            full_text_parts.append(f"【{art['title']}】\n{content}")
        except Exception as e:
            print(f"  [warn] 获取文稿失败: {e}")
            art["content"] = ""

        try:
            vurl = fetch_video_url(art["url"])
            if vurl:
                video_urls.append(vurl)
                art["video_url"] = vurl
        except Exception as e:
            print(f"  [warn] 获取视频URL失败: {e}")

    return {
        "date": date_str,
        "articles": articles,
        "full_text": "\n\n".join(full_text_parts),
        "video_urls": video_urls,
    }


if __name__ == "__main__":
    result = scrape_news()
    print(f"\n日期: {result['date']}")
    print(f"文章数: {len(result['articles'])}")
    print(f"视频URL数: {len(result['video_urls'])}")
    print(f"文稿总长度: {len(result['full_text'])} 字")
    if result["video_urls"]:
        print(f"首个视频URL: {result['video_urls'][0]}")
