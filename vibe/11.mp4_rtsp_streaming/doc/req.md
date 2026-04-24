# RTSP Server 项目需求

## 目标

从 MP4 文件读取 H.264/AAC 流，通过自实现的 RTSP/RTP 协议栈实时推流，可在兼容客户端（VLC/ffplay/ffmpeg）上正常播放。

## 约束

- 语言：C (POSIX, C99)
- 平台：Linux (Amazon Linux 2, x86_64)
- 依赖：仅系统库 (socket, stdio, time, signal, etc.)，不使用 ffmpeg/live555/gstreamer 等第三方库
- 验证：ffmpeg 作为 RTSP 客户端拉流，输出可正常解码播放

## 输入格式

- 常规 MP4 文件（moov box 在 mdat 前，即 faststart 格式）
- 视频：H.264，处理 B-frame 重排序（解析 ctts box，PTS = DTS + CTS offset）
- 音频：AAC-LC

## 传输协议

- RTSP (RFC 2326) 控制通道，TCP 监听，默认端口 8554
- RTP (RFC 3550) 媒体传输，over UDP
- 视频 RTP 打包：RFC 6184 (H.264)，单 NALU 包 + FU-A 分片，clock rate 90000Hz
- 音频 RTP 打包：RFC 3640 (AAC)，AU header section，clock rate = sample rate

## RTSP 功能范围

- OPTIONS
- DESCRIBE（返回 SDP，含 H.264 sprop-parameter-sets 和 AAC config）
- SETUP（建立 RTP session，分配 UDP 端口，video=50000, audio=50002）
- PLAY（按原始帧率实时发送）
- TEARDOWN（断开释放资源）

## 客户端支持

- 当前实现：单客户端
- 架构：session 状态封装为独立结构体，保持多客户端扩展能力

## 播放行为

- 按 MP4 原始帧率定时发送 RTP 包（clock_nanosleep 模拟实时播放）
- RTP 时间戳基于 PTS 转换到对应 clock rate（视频 90kHz，音频 sample rate）
- 发送顺序按 DTS 交替音视频
- 流结束后关闭 TCP 连接

## 已实现的模块

| 模块 | 文件 | 说明 |
|------|------|------|
| MP4 demuxer | mp4_demux.h/.c | 递归解析 box，处理 stts/ctts/stsc/stco/stsz/stss/avcC/esds |
| H.264 RTP packetizer | rtp_packet.h/.c | 单 NALU 包 + FU-A 分片 (PT=96) |
| AAC RTP packetizer | rtp_packet.h/.c | AU header section (PT=97) |
| RTP sender | rtp_send.h/.c | UDP socket 发包 |
| SDP generator | sdp.h/.c | profile-level-id, sprop-parameter-sets, AAC config |
| RTSP server | rtsp_server.h/.c | 协议解析、响应、播放主循环 |
| 工具函数 | utils.h | 大端字节序读写、日志宏 |

## 测试验证

| 测试 | 工具 | 结果 |
|------|------|------|
| MP4 box 解析 | dump_mp4 vs ffprobe | offset/size/DTS/PTS 一致 |
| H.264 裸流提取 | extract_h264 → ffprobe/ffmpeg | 可解码播放 |
| AAC 裸流提取 | extract_aac → ffprobe/ffmpeg | 48kHz stereo 可解码播放 |
| RTSP 端到端 | ffmpeg 拉流 | 125 video + 235 audio frames, 5s, 可播放 |
| 自动化验证 | verify.sh | 17/17 tests passed |
