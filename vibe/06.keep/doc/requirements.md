# Keep Clone - 个人便签应用需求文档

## 项目概述

一个类似 Google Keep 的个人便签应用，支持 Web 端和 iOS 原生端，单用户使用（无需登录/用户系统）。
同步架构灵感来自 VerySync（微力同步），采用去中心化的 P2P 同步模式，服务器仅做设备桥接。

## 核心需求与完成状态

### 1. 便签系统

- [x] 瀑布流/平铺展开的便签卡片布局（CSS columns masonry）
- [x] 支持创建、编辑、删除便签
- [x] 便签支持标题 + 正文
- [x] 标题输入回车/Tab 自动跳转到正文/Checklist 编辑区
- [x] 便签支持 Checklist（待办清单）模式
- [x] 文本 ↔ Checklist 模式互转
- [x] 便签支持置顶/固定（Pin），Pin 的便签始终显示在顶部区域
- [x] 支持手动拖拽排序（Pin 区域和普通区域各自独立排序）
- [x] 默认按修改时间排序
- [x] 搜索功能：按标题、正文、Checklist 内容、标签全文搜索
- [x] 标签功能：便签可打多个标签，按标签筛选，侧边栏标签导航
- [x] #tag 语法自动识别为标签
- [x] 卡片显示最后修改时间
- [x] 链接自动识别、可点击、域名预览（favicon + 域名）
- [x] 创建便签时即可 Pin、选颜色、加标签、归档
- [x] 响应式设计：移动浏览器适配
- [x] 卡片悬浮时显示快捷操作（归档、颜色、标签、删除）
- [x] 卡片上直接勾选 Checklist（不需打开编辑器）
- [x] Grid / List 视图切换
- [x] 设置面板（新项位置、勾选项移到底部）
- [x] 简单富文本格式（粗体、斜体、下划线、删除线、H1 标题、H2 副标题、有序/无序列表）
- [x] 悬浮卡片时直接选择背景颜色（颜色弹出面板）

### 1.1 UI 布局（Google Keep 风格）

- [x] 左侧常驻侧边栏（桌面端），包含导航和标签
- [x] 侧边栏支持展开/收起两种模式（图标模式 / 图标+文字模式，悬浮自动展开）
- [x] 搜索栏在顶部居中，带搜索图标
- [x] 搜索栏聚焦时有下方阴影效果
- [x] Grid/List 视图切换按钮在右上角
- [x] 设置按钮在右上角
- [x] 编辑标签对话框（创建/重命名/删除标签）

### 2. 多彩主题

- [x] 12 种背景颜色（Google Keep 色板）
- [x] 颜色在卡片列表中直接可见
- [x] 创建和编辑时均可选颜色
- [x] 背景图案/插图（6 种主题：购物、美食、音乐、旅行、庆祝、自然）

### 3. Archive 归档功能

- [x] 便签可归档，归档后从主视图隐藏
- [x] 独立的归档视图
- [x] 从归档恢复到主视图

### 4. 回收站

- [x] 软删除进入回收站
- [x] 30 天自动永久删除
- [x] 从回收站恢复
- [x] 手动清空回收站

### 5. 快捷键（Google Keep 风格）

- [x] `C` 新建文本便签
- [x] `L` 新建 Checklist
- [x] `/` 聚焦搜索
- [x] `Esc` 保存并关闭（编辑器/创建框/搜索）
- [x] `Enter` 标题 → 正文/Checklist
- [x] `Enter` Checklist item → 新建下一项
- [x] `Backspace` 空 Checklist item → 删除并聚焦上一项
- [x] `↑↓` Checklist item 间导航
- [x] Tab 不会跳到工具栏按钮

### 6. 多端同步

- [x] Web 端 Yjs CRDT 同步引擎
- [x] IndexedDB 本地持久化（离线可用）
- [x] WebSocket 同步服务器（本地可运行）
- [x] 多标签页/多浏览器同步验证测试（Playwright 验证通过）
- [ ] iOS 端（需 Mac + Xcode）

### 7. 同步架构（VerySync 模式）

- [x] 去中心化，数据不存服务器
- [x] Yjs CRDT 自动冲突解决
- [x] 服务器仅做 WebSocket 中继
- [x] 离线→上线同步（Yjs + IndexedDB 内置支持）
- [x] 服务器 Docker 部署配置（Dockerfile + docker-compose.yml）

### 8. 稳定性

- [x] Local-first 架构
- [x] IndexedDB 持久化
- [x] WebSocket 自动重连（y-websocket 内置）

### 9. 测试

- [x] 单元测试框架（Vitest）
- [x] 便签创建/默认值/ID 唯一性测试
- [x] #tag 解析测试
- [x] URL 提取/域名解析测试
- [x] 常量验证测试
- [ ] 同步/冲突解决集成测试

## 当前未完成项（按优先级）

### P0 - 本次应完成
1. ~~响应式设计（移动浏览器适配）~~ ✅ 已完成
2. ~~多标签页同步验证~~ ✅ Yjs 保证，需手动验证
3. ~~Docker 部署配置~~ ✅ 已完成

### P1 - 需要 Mac 环境
3. iOS 原生端开发

### P2 - 部署相关
4. Docker 部署配置
5. 服务器部署到香港 VPS

### 低优先级
6. 背景图案/插图
7. 同步集成测试

## 数据规模

- 历史便签总量：1000+ 个
- 同时活跃便签：约 100 个
- 需支持大量便签下的流畅搜索和滚动

## 目标平台

| 平台 | 优先级 | 方案 | 状态 |
|------|--------|------|------|
| Web（桌面浏览器） | P0 | React + TypeScript | ✅ 已完成 |
| Web（移动浏览器） | P0 | 响应式设计 | ✅ 已完成 |
| iOS | P1 | Swift 原生 | ⏳ 需 Mac |
| Android | 暂不考虑 | — | — |

## 非需求（当前阶段不考虑）

- 用户注册/登录/多用户
- Android 支持
- 图片/附件
- 提醒功能
- 协作/分享

## 技术方案

### 客户端

- **Web**：React + TypeScript + Vite + Tailwind CSS v3
- **iOS**：Swift（SwiftUI）原生
- **本地存储**：IndexedDB via y-indexeddb（Web）/ SwiftData（iOS）
- **CRDT 同步**：Yjs + y-websocket
- **布局**：CSS columns masonry（Web）
- **拖拽**：@dnd-kit
- **搜索**：本地内存全文搜索

### 服务器

- 极简 WebSocket 中继（~80 行 Node.js）
- 基于 Yjs sync protocol
- **部署方案**（按优先级）：
  1. **香港/国内轻量 VPS**（最可靠，确保中国大陆访问）
  2. Cloudflare Workers（备选）
- Docker 部署（VPS 方案时）

### ⚠️ 中国大陆可访问性（硬约束）

- Vercel：❌ 域名已被墙
- Cloudflare Workers：⚠️ 部分地区不稳定
- 香港 VPS + 自有域名：✅ 最稳定方案
- Local-first 架构，服务器不可达时各端仍可独立使用

## 开发环境

- **Linux 机器**：Node.js 18.18.2，Web 端 + 同步服务器开发
- **Mac 机器**：Xcode + Swift，iOS 端开发（需切换时提醒用户）

## 测试策略

- 每步开发结果必须可测试、可验证
- 发现的问题必须有 test case 覆盖
- `npm test` 运行所有单元测试
- `npm run dev` 浏览器交互验证
- 同步服务器本地运行 `cd server && npm start`
