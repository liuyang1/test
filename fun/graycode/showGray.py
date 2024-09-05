"""
Sep-05-2024 16:25:16 >  
今天在办公室看到同事使用2x2的模块在屏幕闪烁黑白，从而监测显示的效果
我想到可以利用gray code，减少闪烁次数，但是达到相似的效果。
这样的方式，可以降低对于摄像头的要求，比如60fps摄像头，理论上可以监测120fps的效果

具体来说，就是利用gray code的减少反转的特性，从而减少闪烁频率

操作指南
启动后，按键
f 默认方式闪烁
g gray code方式闪烁
b binary 闪烁
q 退出
"""
import pygame
import time

# print(pygame.version.verFilePath())
start_time = time.time()  # 记录启动前的时间戳
pygame.mixer.quit()
pygame.mixer = None
# 初始化Pygame
pygame.init()

# 设置窗口大小
window_size = (200, 200)
screen = pygame.display.set_mode(window_size, flags=pygame.NOFRAME)

# 设置窗口标题
# pygame.display.set_caption("2x2 Grid")
end_time = time.time()  # 记录启动后的时间戳
startup_time = end_time - start_time  # 计算启动时间
print(f"Pygame startup time: {startup_time:.6f} seconds")

# 定义颜色
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)

# 定义方格大小
grid_size = 200
cell_size = grid_size // 2

# 定义方格初始颜色
# grid_color = WHITE

cnt = 0


def f(row, col, cnt):
    """
    默认反转策略，逐个亮，注意这里之所以交换第二行的顺序，是为了视觉上的顺时针效果
                00 01 10 11
    round-robin: W  B  B  B
                 B  W  B  B
                 B  B  B  W
                 B  B  W  B
    假设原始视频流为60fps，则每块反转频率为30fps。但是每块点亮的时间周期是16ms
    合计闪烁120fps
    """
    return ((row != col) + 2 * row) == cnt % 4


def g(row, col, cnt):
    """
    按照格雷码闪烁
    假设原始视频流为60fps，则第一,二块闪烁为7.5fps，第三块是15fps。第四块是30fps
    每块点亮时间对称均衡的
    合计闪烁60fps
    """
    cnt = cnt % 16
    gray = [
        (0, 0, 0, 0),
        (0, 0, 0, 1),
        (0, 0, 1, 1),
        (0, 0, 1, 0),
        (0, 1, 1, 0),
        (0, 1, 1, 1),
        (0, 1, 0, 1),
        (0, 1, 0, 0),
        (1, 1, 0, 0),
        (1, 1, 0, 1),
        (1, 1, 1, 1),
        (1, 1, 1, 0),
        (1, 0, 1, 0),
        (1, 0, 1, 1),
        (1, 0, 0, 1),
        (1, 0, 0, 0),
    ]
    g = gray[cnt]
    return g[row * 2 + col]


def binary(row, col, cnt):
    """
    按照二进制编码闪烁
    这种方式最低位（第四位）按照60fps闪烁，第三位按照30fps，第二位按照15fps，最高位按照7.5fps闪烁
    每块点亮时间自己也是对称均衡的
    合计闪烁112.5fps
    """
    cnt = cnt % 16
    b = bin(cnt)[2:]  # skip first "0b"
    b = b.zfill(4)
    # print(b)
    return int(b[row * 2 + col])


first_show = True
# 游戏循环
running = True
func = f
while running:
    # 处理事件
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.KEYDOWN:
            key = event.unicode.lower()
            if key == "f":
                func = f
                print("Switch to round-robin code")
            elif key == "g":
                func = g
                print("Switch to gray code")
            elif key == "b":
                func = binary
                print("switch to binary code")
            elif key == "q":
                running = False
            else:
                print("Unknown key")

    # 清空屏幕
    screen.fill(BLACK)

    # 绘制方格
    for row in range(2):
        for col in range(2):
            # cell_color = grid_color if (row + col) % 2 == 0 else (255, 255, 255) if grid_color == (0, 0, 0) else (0, 0, 0)
            # cell_color = WHITE if f(row, col, cnt) else BLACK
            cell_color = WHITE if func(row, col, cnt) else BLACK
            # cell_color = WHITE if f(row,
            pygame.draw.rect(
                screen,
                cell_color,
                (col * cell_size, row * cell_size, cell_size, cell_size),
            )

    # 更新显示
    pygame.display.flip()
    if first_show:
        end_time = time.time()  # 记录启动后的时间戳
        startup_time = end_time - start_time  # 计算启动时间
        print(f"Pygame startup time: {startup_time:.6f} seconds")
        first_show = False

    # 切换方格颜色
    # grid_color = WHITE if grid_color == BLACK else BLACK
    cnt += 1

    # 延时
    time.sleep(0.1)

# 退出Pygame
pygame.quit()
