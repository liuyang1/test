"""全局配置"""
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
OUTPUT_DIR = os.path.join(BASE_DIR, "output")
TEMP_DIR = os.path.join(BASE_DIR, "temp")

# 视频输出规格
VIDEO_WIDTH = 720
VIDEO_HEIGHT = 1280
VIDEO_FPS = 30

# TTS 配置
TTS_VOICE = "zh-CN-YunxiNeural"
TTS_RATE = "+35%"  # ~1.5x 语速，高效感

# 摘要目标时长（秒）
TARGET_DURATION = 170

# 水印
WATERMARK_TEXT = "YYY"
WATERMARK_FONTSIZE = 28
WATERMARK_OPACITY = 0.6  # 0-1

# 字幕样式预设（大字体，两行显示，适配手机阅读）
SUBTITLE_STYLES = {
    "default": {
        "fontname": "Noto Sans CJK SC",
        "fontsize": 58,
        "primary_color": "&H00FFFFFF",
        "outline_color": "&H00000000",
        "back_color": "&H80000000",
        "outline": 3,
        "shadow": 0,
        "margin_v": 80,
        "alignment": 2,
        "border_style": 1,
    },
    "news": {
        "fontname": "Noto Sans CJK SC",
        "fontsize": 56,
        "primary_color": "&H00FFFFFF",
        "outline_color": "&H00222222",
        "back_color": "&HC0000000",
        "outline": 1,
        "shadow": 0,
        "margin_v": 70,
        "alignment": 2,
        "border_style": 3,
    },
    "minimal": {
        "fontname": "Noto Sans CJK SC",
        "fontsize": 54,
        "primary_color": "&H00FFFFFF",
        "outline_color": "&H00000000",
        "back_color": "&H00000000",
        "outline": 4,
        "shadow": 0,
        "margin_v": 90,
        "alignment": 2,
        "border_style": 1,
    },
}
SUBTITLE_STYLE = "news"

# 视频布局模式
# "fit_top"    - 原视频等比缩放放在上方，下方模糊背景
# "triple"     - 3x1 重复布局填满竖屏
# "blur_full"  - 模糊放大的原视频做全屏背景，原视频居中叠加
VIDEO_LAYOUTS = ["fit_top", "triple", "blur_full"]  # 一次性生成所有布局
VIDEO_LAYOUT = "fit_top"  # 默认布局

# 音效
DING_SOUND = os.path.join(BASE_DIR, "assets", "ding.mp3")
BGM_SOUND = os.path.join(BASE_DIR, "assets", "bgm.mp3")

# 标题
TITLE_TEXT = "新闻联播2分钟"
TITLE_FONTSIZE = 64
DATE_FONTSIZE = 40

# 进度条
PROGRESS_BAR_HEIGHT = 6
PROGRESS_BAR_Y = None  # 自动计算
PROGRESS_BAR_COLOR = "FFFFFF"  # 白色
PROGRESS_BAR_BG_COLOR = "444444"  # 灰色

# 图片卡片模式
CARD_READ_SPEED = 0.3  # 每字秒数
CARD_MIN_DURATION = 3.0
CARD_MAX_DURATION = 6.0
FFMPEG = os.environ.get("FFMPEG_PATH", "ffmpeg")
FFPROBE = os.environ.get("FFPROBE_PATH", "ffprobe")

os.makedirs(OUTPUT_DIR, exist_ok=True)
os.makedirs(TEMP_DIR, exist_ok=True)
