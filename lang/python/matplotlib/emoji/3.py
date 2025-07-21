import asyncio
from playwright.async_api import async_playwright
from PIL import Image
import io
import matplotlib.pyplot as plt
import numpy as np

async def render_text_to_image(text, width=400, height=200):
    async with async_playwright() as p:
        browser = await p.chromium.launch()
        page = await browser.new_page(viewport={'width': width, 'height': height})
        
        # HTML模板，使用你的xkcd字体路径
        html = f"""
        <html>
        <head>
            <style>
                @font-face {{
                    font-family: 'xkcd';
                    src: url('file:///Users/lyz/Library/Fonts/xkcd-script.ttf') format('truetype'),
                         url('file:///Users/lyz/Library/Fonts/xkcd.otf') format('opentype');
                }}
            </style>
        </head>
        <body style="
            margin: 0;
            width: {width}px;
            height: {height}px;
            display: flex;
            align-items: center;
            justify-content: center;
            background: white;
        ">
            <div style="
                font-family: 'xkcd', system-ui, -apple-system;
                font-size: 24px;
                color: black;
                letter-spacing: 1px;  /* 增加字间距，更像xkcd风格 */
            ">{text}</div>
        </body>
        </html>
        """
        
        await page.set_content(html)
        await page.wait_for_load_state('networkidle')
        
        screenshot = await page.screenshot(
            type='png',
            full_page=True,
            scale='device'
        )
        
        await browser.close()
        return Image.open(io.BytesIO(screenshot))

def plot_with_text(text, figsize=(6, 3)):
    img = asyncio.run(render_text_to_image(text))
    
    fig, ax = plt.subplots(figsize=figsize)
    ax.imshow(np.array(img))
    ax.axis('off')
    return fig

# 使用示例
fig = plot_with_text("Hello 👋 World 🌍")
plt.show()
