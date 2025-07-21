from PIL import Image, ImageDraw, ImageFont
import matplotlib.pyplot as plt
import numpy as np

# 创建图像
img = Image.new('RGB', (400, 200), color='white')
draw = ImageDraw.Draw(img)

# 加载两种字体
emoji_font = ImageFont.truetype('/System/Library/Fonts/Apple Color Emoji.ttc', 40)
text_font = ImageFont.truetype('/System/Library/Fonts/Arial Unicode.ttf', 40)  # 普通文本字体

# 分别绘制文本和emoji
text_parts = {
    "Hello ": text_font,
    "👋": emoji_font,
    " World ": text_font,
    "🌍": emoji_font
}

# 计算总宽度以居中
total_width = sum(draw.textlength(text, font) for text, font in text_parts.items())
x = (400 - total_width) / 2
y = 100

# 依次绘制每个部分
for text, font in text_parts.items():
    draw.text((x, y), text, font=font, fill='black', anchor="lm")
    x += draw.textlength(text, font)

# 显示在matplotlib中
plt.imshow(np.array(img))
plt.axis('off')
plt.show()
