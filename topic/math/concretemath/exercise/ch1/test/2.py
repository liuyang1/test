import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
from matplotlib.gridspec import GridSpec

# 节点数列表
node_counts = [3, 9, 27, 81, 243]

# 计算比例
ratios = np.sqrt(node_counts)
total_ratio = sum(ratios)

# 创建图形
fig = plt.figure(figsize=(20, 15))

# 创建左右分布的 GridSpec
gs = GridSpec(4, 2, width_ratios=[sum(ratios[:4]), ratios[-1]], height_ratios=ratios[:4])

# 创建和绘制图形
for i, count in enumerate(node_counts):
    # 创建随机图
    G = nx.gnm_random_graph(count, count * 2)
    
    # 确定子图位置
    if i < 4:
        ax = fig.add_subplot(gs[i, 0])
    else:
        ax = fig.add_subplot(gs[:, 1])
    
    # 绘制网络图
    pos = nx.spring_layout(G)
    nx.draw(G, pos, ax=ax, node_size=50, node_color='lightblue', 
            edge_color='gray', with_labels=False)
    
    # 设置标题
    ax.set_title(f'Nodes: {count}')
    
    # 移除坐标轴
    ax.axis('off')

# 调整子图之间的间距
plt.tight_layout()

# 显示图形
plt.show()
