# Keep Clone - 技术架构设计

## 整体架构

```
┌─────────────────┐     ┌─────────────────┐
│   Web (React)   │     │  iOS (SwiftUI)  │
│                 │     │                 │
│  ┌───────────┐  │     │  ┌───────────┐  │
│  │  Yjs Doc  │  │     │  │  Yjs Doc  │  │
│  └─────┬─────┘  │     │  └─────┬─────┘  │
│        │        │     │        │        │
│  ┌─────┴─────┐  │     │  ┌─────┴─────┐  │
│  │ IndexedDB │  │     │  │ SwiftData │  │
│  └─────┬─────┘  │     │  └─────┬─────┘  │
└────────┼────────┘     └────────┼────────┘
         │    WebSocket          │
         └──────┬────────────────┘
                │
       ┌────────┴────────┐
       │  Sync Server    │
       │  (信令+中继)     │
       │  ~200行代码      │
       │  香港 VPS        │
       └─────────────────┘
```

## 数据模型

### Note（便签）

```typescript
interface Note {
  id: string              // UUID v4
  title: Y.Text           // CRDT 文本，支持合并
  content: Y.Text         // CRDT 文本，支持合并
  type: 'text' | 'checklist'
  checklist: Y.Array<ChecklistItem>  // CRDT 数组
  color: string           // LWW 字段
  background: string      // LWW 字段
  pinned: boolean         // LWW
  archived: boolean       // LWW
  deleted: boolean        // LWW（软删除）
  deletedAt: number       // 删除时间戳，用于 30 天自动清理
  sortOrder: number       // 浮点数，拖拽排序
  createdAt: number
  updatedAt: number
  deviceId: string        // 最后修改设备
}

interface ChecklistItem {
  id: string
  text: Y.Text            // CRDT 文本
  checked: boolean        // LWW
  sortOrder: number       // LWW
}
```

### LWW 元数据（非 CRDT 字段的冲突解决）

```typescript
// 每个 LWW 字段附带元数据
interface LWWField<T> {
  value: T
  timestamp: number       // 修改时间
  deviceId: string        // 修改设备
}
```

## 同步层设计

### 选型：Yjs + y-websocket 协议

Yjs 已经解决了我们需要的一切：
- 文本 CRDT 合并（Y.Text）
- 数组 CRDT（Y.Array，用于 Checklist）
- Map CRDT（Y.Map，用于 LWW 字段）
- 内置 WebSocket 同步协议（y-websocket）
- 内置离线支持（状态向量 + 增量更新）
- Web 端直接用，iOS 有 y-swift

**关键决策：每个便签是一个独立的 Yjs Document**

这样的好处：
- 同步粒度细，只同步变更的便签
- 不会因为一个便签的编辑触发全量同步
- 便签之间完全隔离

### 便签列表本身的同步

便签列表（包含所有便签的元数据：id, color, pinned, archived, deleted, sortOrder）
作为一个单独的 Yjs Document（`notes-meta`），使用 Y.Map 存储。

```
Yjs Documents:
├── notes-meta          # Y.Map<NoteId, NoteMeta>  便签元数据列表
├── note-{id-1}         # Y.Doc  便签1的内容（title + content/checklist）
├── note-{id-2}         # Y.Doc  便签2的内容
└── ...
```

### 同步流程

```
设备A 编辑便签 → Yjs 生成 update (二进制增量)
  → 写入本地 IndexedDB (y-indexeddb)
  → 通过 WebSocket 发送 update 到服务器
  → 服务器转发给在线设备 / 暂存给离线设备
  → 设备B 收到 update → Yjs 自动合并 → UI 更新
```

### 离线 → 上线同步

```
设备A 离线编辑 → updates 累积在本地
设备A 上线 → 发送 state vector 给服务器
服务器 → 返回暂存的 updates（其他设备发来的）
设备A → 应用 updates，同时发送自己的累积 updates
→ 双向同步完成，Yjs 自动合并所有冲突
```

## 服务器设计

### 基于 y-websocket 修改

y-websocket 官方服务端已经实现了我们需要的大部分功能，只需少量修改：

```
服务器职责：
1. WebSocket 连接管理
2. 接收 Yjs update → 转发给同 room 的其他连接
3. 离线暂存：将 update 持久化到磁盘，设备上线后发送
4. 无需理解数据内容
```

### 持久化（仅暂存用途）

- 使用 LevelDB 或文件系统存储未投递的 updates
- 设备消费后可清理
- 也可选择保留作为备份（可选，非核心需求）

### 部署

- Node.js 单进程
- 香港轻量 VPS（1核1G足够）
- Docker 部署
- Nginx 反向代理 + Let's Encrypt SSL

## Web 端架构

```
src/
├── components/
│   ├── NoteCard.tsx          # 便签卡片
│   ├── NoteEditor.tsx        # 编辑器（文本 + Checklist）
│   ├── NoteGrid.tsx          # 瀑布流布局
│   ├── ColorPicker.tsx       # 颜色选择器
│   ├── SearchBar.tsx         # 搜索栏
│   └── Toolbar.tsx           # 顶部工具栏
├── hooks/
│   ├── useNotes.ts           # 便签 CRUD 操作
│   ├── useSync.ts            # Yjs 同步管理
│   └── useSearch.ts          # 搜索逻辑
├── sync/
│   ├── provider.ts           # Yjs WebSocket provider
│   ├── persistence.ts        # IndexedDB 持久化
│   └── schema.ts             # Yjs document schema
├── store/
│   └── notes.ts              # 便签状态管理（从 Yjs 派生）
└── App.tsx
```

### 关键技术选型

| 功能 | 库 |
|------|-----|
| UI 框架 | React 18 + TypeScript |
| CRDT 同步 | Yjs + y-websocket + y-indexeddb |
| 瀑布流布局 | react-masonry-css 或 CSS columns |
| 拖拽排序 | @dnd-kit/core |
| 搜索 | 本地 IndexedDB 全文遍历（数据量 1000 级别足够） |
| 样式 | Tailwind CSS |
| 构建 | Vite |

## iOS 端架构

```
KeepClone/
├── Models/
│   ├── Note.swift             # 数据模型
│   └── SyncManager.swift      # Yjs 同步管理
├── Views/
│   ├── NoteGridView.swift     # 瀑布流主视图
│   ├── NoteCardView.swift     # 便签卡片
│   ├── NoteEditorView.swift   # 编辑器
│   ├── ColorPickerView.swift  # 颜色选择
│   └── SearchView.swift       # 搜索
├── Sync/
│   ├── YjsBridge.swift        # y-swift 桥接
│   └── WebSocketProvider.swift
└── Persistence/
    └── LocalStore.swift       # SwiftData 持久化
```

### iOS 关键选型

| 功能 | 方案 |
|------|------|
| UI | SwiftUI |
| CRDT | y-swift（Yjs 的 Swift 实现） |
| 本地存储 | SwiftData |
| 布局 | LazyVGrid + 自定义 CompositionalLayout |
| WebSocket | URLSessionWebSocketTask |

## 开发阶段

### Phase 1：Web 端核心功能
- 便签 CRUD + 瀑布流布局
- 颜色选择
- Checklist
- Pin / Archive / 回收站
- 拖拽排序
- 搜索
- IndexedDB 本地持久化

### Phase 2：同步服务器 + Web 同步
- y-websocket 服务端部署
- Web 端接入 Yjs 同步
- 离线/上线同步测试
- 冲突解决验证

### Phase 3：iOS 原生端
- SwiftUI 界面
- y-swift 集成
- 与服务器同步
- 多端同步联调

### Phase 4：打磨
- 动画和交互细节
- 性能优化（大量便签场景）
- 回收站自动清理
- 错误处理和边界情况

## 开发环境

- **Linux 机器**：Node.js 18.18.2，用于 Web 端 + 同步服务器开发
- **Mac 机器**：Xcode + Swift，用于 iOS 端开发（需切换时提醒用户）
- iOS 最低支持版本：iOS 17+（使用 SwiftData）

## 测试策略

### 原则
- 每步开发结果必须可测试、可验证
- 发现的问题必须有 test case 覆盖
- 后续改动必须保证已有 test case 通过，不返工

### Web 端
- 每完成一个功能模块 → `npm run dev` 浏览器可交互验证
- 核心逻辑（CRUD、搜索、排序、回收站清理）有单元测试
- 同步/合并逻辑有单元测试覆盖各种冲突场景

### 同步服务器
- 优先部署在本地（`node server.js`）进行测试
- 多客户端连接的集成测试
- 离线→上线同步场景测试

### iOS 端
- 在模拟器中测试，不依赖真机
- UI 交互在模拟器中验证
- 同步逻辑与本地服务器联调测试
