"""视频下载模块 - m3u8 解析与下载"""
import os
import subprocess
from config import FFMPEG, TEMP_DIR


def download_video(url: str, output_path: str) -> str:
    """通过 ffmpeg 下载 m3u8 或直接视频 URL"""
    print(f"[downloader] 下载视频: {url[:80]}...")
    cmd = [
        FFMPEG, "-y",
        "-i", url,
        "-c", "copy",
        "-bsf:a", "aac_adtstoasc",
        output_path,
    ]
    result = subprocess.run(cmd, capture_output=True, text=True, timeout=600)
    if result.returncode != 0:
        # 如果 copy 失败，尝试重新编码
        cmd = [FFMPEG, "-y", "-i", url, "-c:v", "libx264", "-c:a", "aac", output_path]
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=600)
        if result.returncode != 0:
            raise RuntimeError(f"视频下载失败: {result.stderr[-500:]}")
    print(f"[downloader] 下载完成: {output_path}")
    return output_path


def download_all_videos(video_urls: list[str], date_str: str) -> list[str]:
    """下载所有视频，返回本地文件路径列表"""
    downloaded = []
    out_dir = os.path.join(TEMP_DIR, date_str, "videos")
    os.makedirs(out_dir, exist_ok=True)

    for i, url in enumerate(video_urls):
        out_path = os.path.join(out_dir, f"clip_{i:03d}.mp4")
        if os.path.exists(out_path) and os.path.getsize(out_path) > 1000:
            print(f"[downloader] 已存在，跳过: {out_path}")
            downloaded.append(out_path)
            continue
        try:
            download_video(url, out_path)
            downloaded.append(out_path)
        except Exception as e:
            print(f"[downloader] 下载失败 ({i}): {e}")
    return downloaded


if __name__ == "__main__":
    # 测试用
    import sys
    if len(sys.argv) > 1:
        out = download_video(sys.argv[1], os.path.join(TEMP_DIR, "test_download.mp4"))
        print(f"Downloaded: {out}")
