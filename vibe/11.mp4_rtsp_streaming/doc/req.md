# RTSP Server 项目需求

## 目标

从 MP4 文件读取 H.264/AAC 流，通过自实现的 RTSP/RTP 协议栈实时推流，可在 VLC/ffplay 上正常播放。

## 约束

- 语言：C (POSIX)
- 平台：Linux
- 依赖：仅系统库 (socket, stdio, pthread, etc.)，不使用 ffmpeg/live555/gstreamer 等第三方库
- 验证：VLC / ffplay 作为客户端可正常播放音视频

## 输入格式

- 常规 MP4 文件（moov box 在文件头部，非 fragmented MP4）
- 视频：H.264，需处理 B-frame 重排序（解析 ctts box，PTS = DTS + CTS offset）
- 音频：AAC

## 传输协议

- RTSP (RFC 2326) 控制通道，TCP 监听
- RTP (RFC 3550) 媒体传输，over UDP
- 视频 RTP 打包：RFC 6184 (H.264)，支持 FU-A 分片
- 音频 RTP 打包：RFC 3640 (AAC)

## RTSP 功能范围

- OPTIONS
- DESCRIBE（返回 SDP）
- SETUP（建立 RTP session，分配 UDP 端口）
- PLAY（按原始帧率实时发送）
- TEARDOWN（断开释放资源）

## 客户端支持

- 当前实现：单客户端
- 架构要求：session 状态封装为独立结构体，保持多客户端扩展能力

## 播放行为

- 按 MP4 原始帧率定时发送 RTP 包（模拟实时播放）
- RTP 时间戳使用 PTS，发送顺序按 DTS

## 需自实现的模块

| 模块 | 说明 |
|------|------|
| MP4 demuxer | 解析 box 结构，提取 H.264 NALU 和 AAC frame，处理 stts/ctts/stsc/stco/stsz 等 box |
| H.264 RTP packetizer | RFC 6184，单 NALU 包 + FU-A 分片 |
| AAC RTP packetizer | RFC 3640，AU header section |
| RTSP server | RFC 2326，文本协议解析与响应 |
| SDP generator | 根据媒体信息生成 SDP 描述 |
| RTP sender | UDP socket 发包，序列号/时间戳管理 |
| 定时调度 | clock_nanosleep 按帧间隔发送 |
