import asyncio
from playwright.async_api import async_playwright
from PIL import Image
import io
import matplotlib.pyplot as plt
import numpy as np

async def render_text_to_image(text, width=400, height=200):
    async with async_playwright() as p:
        # 使用 chromium (也支持 firefox/webkit)
        browser = await p.chromium.launch()
        page = await browser.new_page(viewport={'width': width, 'height': height})
        
        # HTML模板，使用现代CSS特性
        html = f"""
        <html>
        <body style="margin: 0; background: transparent;">
            <div style="
                width: {width}px;
                height: {height}px;
                display: flex;
                align-items: center;
                justify-content: center;
                font-family: system-ui, -apple-system;
                font-size: 24px;
                color: black;
                background: white;
            ">{text}</div>
        </body>
        </html>
        """
        
        await page.set_content(html)
        
        # 截图，使用高质量设置
        screenshot = await page.screenshot(
            type='png',
            full_page=True,
            scale='device',
            # background_color='white'
        )
        
        await browser.close()
        return Image.open(io.BytesIO(screenshot))

def plot_with_text(text, figsize=(6, 3)):
    # 转换为同步调用
    img = asyncio.run(render_text_to_image(text))
    
    # 在matplotlib中显示
    fig, ax = plt.subplots(figsize=figsize)
    ax.imshow(np.array(img))
    ax.axis('off')
    return fig

# 使用示例
fig = plot_with_text("Hello 👋 World 🌍")
plt.show()
