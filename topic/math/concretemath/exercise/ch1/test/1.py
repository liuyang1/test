import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
from matplotlib.gridspec import GridSpec

# 创建图形和节点数列表
graphs = []
node_counts = [3, 9, 27, 81, 243]

for count in node_counts:
    G = nx.gnm_random_graph(count, count * 2)
    graphs.append(G)

# 计算总的网格单元数
total_cells = sum(np.sqrt(count) for count in node_counts)

# 设置图形大小
fig = plt.figure(figsize=(20, 10))

# 创建 GridSpec
gs = GridSpec(1, int(total_cells), figure=fig)

# 绘制每个子图
start = 0
for i, (G, count) in enumerate(zip(graphs, node_counts)):
    # 计算子图宽度
    width = int(np.sqrt(count))
    
    # 创建子图
    ax = fig.add_subplot(gs[0, start:start+width])
    
    # 绘制网络图
    pos = nx.spring_layout(G)
    nx.draw(G, pos, ax=ax, node_size=50, node_color='lightblue', 
            edge_color='gray', with_labels=False)
    
    # 设置标题
    ax.set_title(f'Nodes: {count}')
    
    # 移除坐标轴
    ax.axis('off')
    
    # 更新起始位置
    start += width

# 调整子图之间的间距
plt.tight_layout()

# 显示图形
plt.show()
