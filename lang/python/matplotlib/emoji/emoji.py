import sys
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties

# 设置较大的字体大小
plt.figure(figsize=(10, 6))
font = FontProperties(fname='/System/Library/Fonts/Apple Color Emoji.ttc', size=20)

fig, ax = plt.subplots()
ax.text(0.5, 0.5, "Hello 👋 World 🌍", 
                fontproperties=font,
                ha='center', va='center',
                fontsize=20)  # 明确指定字体大小
plt.show()

sys.exit()
import matplotlib.pyplot as plt
from fontTools.ttLib import TTCollection
from matplotlib.font_manager import FontProperties

# 方案1: 直接指定 ttc 文件
font = FontProperties(fname='/System/Library/Fonts/Apple Color Emoji.ttc')

# 方案2: 如果方案1不工作，可以尝试提取特定的字体
ttc = TTCollection('/System/Library/Fonts/Apple Color Emoji.ttc')
# 通常第一个字体是我们需要的
font_index = 0  # 可以尝试 0, 1, 2 等不同索引

fig, ax = plt.subplots()
ax.text(0.5, 0.5, "Hello 👋 World 🌍", 
                fontproperties=font,
                ha='center', va='center')
plt.show()
sys.exit()
import matplotlib.font_manager as fm

# 列出所有可用字体
fonts = [f.name for f in fm.fontManager.ttflist]
print("Available fonts:", fonts)

# 查找包含 'emoji' 的字体
emoji_fonts = [f for f in fonts if 'emoji' in f.lower()]
print("Emoji fonts:", emoji_fonts)
sys.exit()
import matplotlib.pyplot as plt

# 设置字体支持emoji
plt.rcParams['font.family'] = 'Apple Color Emoji'  # macOS
# 或 'Segoe UI Emoji' for Windows
# 或 'Noto Color Emoji' for Linux

plt.text(0.5, 0.5, "Hello 👋 World 🌍")
plt.show()
