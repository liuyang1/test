# RTSP Server 模块设计

## 目录结构

```
rtsp-server/
├── Makefile
├── doc/
│   ├── req.md              # 需求文档
│   └── design.md           # 本文档
├── src/
│   ├── main.c              # 入口，解析参数，启动 server
│   ├── mp4_demux.h/.c      # MP4 解析，提取 track/sample 信息
│   ├── rtp_packet.h/.c     # RTP 打包（H.264 FU-A + AAC AU）
│   ├── rtsp_server.h/.c    # RTSP 协议处理 + 播放主循环
│   ├── sdp.h/.c            # SDP 生成
│   ├── rtp_send.h/.c       # RTP/UDP 发送
│   └── utils.h             # 字节序读写、日志宏（header-only）
├── test/
│   ├── dump_mp4.c          # 打印 MP4 track/sample 信息
│   ├── extract_h264.c      # 导出 H.264 Annex B 裸流
│   ├── extract_aac.c       # 导出 AAC（加 ADTS header）
│   └── verify.sh           # 自动化验证脚本（17 tests）
└── tool/
    └── input.mp4           # 测试文件（H.264+AAC, 5s, 320x176）
```

## 模块职责

| 模块 | 职责 | 不负责 |
|------|------|--------|
| mp4_demux | 解析 MP4 box，构建 sample 索引，按索引读取 sample 数据 | 网络、RTP |
| rtp_packet | 将一帧数据打成 RTP 包列表（H.264 FU-A / AAC AU） | socket 发送 |
| rtp_send | UDP socket 创建、绑定、发包 | 打包逻辑、定时 |
| sdp | 根据 track 信息生成 SDP 文本 | 协议交互 |
| rtsp_server | RTSP 协议解析与响应，串联所有模块，定时发送控制 | 底层解析/打包 |
| utils | 大端字节序读写 (rd16/rd32/rd64/wr16/wr32)、LOG/ERR 宏 | 业务逻辑 |

## 核心数据结构

### MP4 层

```c
typedef struct {
    uint64_t offset;      // 文件中的字节偏移
    uint32_t size;        // sample 大小
    uint64_t dts;         // 解码时间（timescale 单位）
    uint64_t pts;         // 显示时间（dts + cts_offset）
    int      is_sync;     // 是否关键帧（来自 stss）
} mp4_sample_t;

typedef struct {
    uint32_t      track_id;
    uint32_t      timescale;
    uint32_t      codec;           // fourcc: 'avc1' 或 'mp4a'
    uint32_t      sample_count;
    mp4_sample_t *samples;

    // H.264: SPS/PPS 从 avcC box 提取
    uint8_t *sps;  uint16_t sps_len;
    uint8_t *pps;  uint16_t pps_len;
    uint8_t  nalu_len_size;   // avcC 中的 length field 字节数（通常 4）

    // AAC: AudioSpecificConfig 从 esds box 提取
    uint8_t *asc;  uint16_t asc_len;
    uint32_t sample_rate;
    uint16_t channels;
    uint8_t  profile;       // AAC profile (用于 ADTS header)
    uint8_t  freq_index;    // 采样率索引
    uint8_t  chan_config;   // 声道配置
} mp4_track_t;

typedef struct {
    FILE        *fp;
    uint32_t     track_count;
    mp4_track_t  tracks[8];
    int          video_idx;    // -1 表示无
    int          audio_idx;
} mp4_demux_t;
```

### RTP 打包层

```c
#define RTP_MAX_PKT_SIZE 1400  // MTU-safe

typedef struct {
    uint8_t  data[RTP_MAX_PKT_SIZE + 12]; // 12-byte RTP header + payload
    uint16_t len;
} rtp_pkt_t;

typedef struct {
    rtp_pkt_t *pkts;
    int        count;
    int        cap;
} rtp_pkt_list_t;
```

### RTSP/RTP 会话层

```c
typedef struct {
    int      sockfd;
    uint16_t server_port;
    uint16_t client_port;
    uint32_t ssrc;
    uint16_t seq;
    struct sockaddr_in client_addr;
} rtp_channel_t;

typedef struct {
    char          session_id[16];
    int           tcp_fd;
    rtp_channel_t video_rtp;
    rtp_channel_t audio_rtp;
    int           video_setup;
    int           audio_setup;
    int           playing;
} rtsp_session_t;

typedef struct {
    int            listen_fd;
    uint16_t       port;          // 默认 8554
    mp4_demux_t   *demux;
    rtsp_session_t session;       // 单客户端；多客户端时改为动态管理
    volatile int   running;
} rtsp_server_t;
```

## 模块接口

### mp4_demux

```c
int  mp4_open(mp4_demux_t *d, const char *path);
int  mp4_read_sample(mp4_demux_t *d, int track_idx,
                     uint32_t sample_idx, uint8_t *buf, uint32_t buf_size);
void mp4_close(mp4_demux_t *d);
void mp4_dump(const mp4_demux_t *d);   // debug 输出
```

### rtp_packet

```c
int  rtp_packetize_h264(rtp_pkt_list_t *out, const uint8_t *nalu,
                        uint32_t nalu_len, uint32_t ts,
                        uint16_t *seq, uint32_t ssrc, int marker);
int  rtp_packetize_aac(rtp_pkt_list_t *out, const uint8_t *frame,
                       uint32_t frame_len, uint32_t ts,
                       uint16_t *seq, uint32_t ssrc);
void rtp_pkt_list_init(rtp_pkt_list_t *l);
void rtp_pkt_list_free(rtp_pkt_list_t *l);
```

### rtp_send

```c
int  rtp_channel_open(rtp_channel_t *ch, uint16_t server_port);
void rtp_channel_set_dest(rtp_channel_t *ch, uint32_t client_ip, uint16_t client_port);
int  rtp_channel_send(rtp_channel_t *ch, const rtp_pkt_list_t *list);
void rtp_channel_close(rtp_channel_t *ch);
```

### sdp

```c
int sdp_generate(char *buf, int buf_size, const mp4_demux_t *d,
                 const char *server_ip);
```

### rtsp_server

```c
int  rtsp_server_init(rtsp_server_t *srv, uint16_t port, mp4_demux_t *demux);
int  rtsp_server_run(rtsp_server_t *srv);
void rtsp_server_stop(rtsp_server_t *srv);
```

## 数据流

```
mp4_open() → 构建 sample 索引
                │
                ▼
        遍历 sample（按 DTS 时间交替音视频）
                │
                ▼
        mp4_read_sample() → 原始帧数据
                │
                ▼
        rtp_packetize_h264/aac() → RTP 包列表
                │
                ▼
        rtp_channel_send() → UDP 发送
                │
                ▼
        clock_nanosleep() → 等待下一帧时间
```

## 关键实现细节

### MP4 解析
- 递归遍历 box 树，遇到 trak 时创建新 track
- stsd 中 visual sample entry 固定字段 78 字节后是子 box（avcC）
- stsd 中 audio sample entry 固定字段 28 字节后是子 box（esds）
- esds 采用层级解析：ES_Descriptor(3) → DecoderConfigDescriptor(4) → DecoderSpecificInfo(5)
- NALU 长度为 0 时需跳出循环防止死循环

### RTP 时间戳
- H.264: PTS × 90000 / timescale（RTP clock rate 固定 90kHz）
- AAC: PTS × sample_rate / timescale（RTP clock rate = 采样率）

### SDP 关键字段
- H.264: `a=rtpmap:96 H264/90000` + `sprop-parameter-sets` (base64 SPS,PPS)
- AAC: `a=rtpmap:97 MPEG4-GENERIC/{sample_rate}/{channels}` + `config` (hex ASC)

### 播放节奏控制
- 记录 stream_loop 开始的 CLOCK_MONOTONIC 时间
- 每帧按 DTS/timescale 计算绝对发送时间
- clock_nanosleep(TIMER_ABSTIME) 精确等待

## 开发过程中修复的问题

| 问题 | 原因 | 修复 |
|------|------|------|
| SPS/PPS 未解析 | visual sample entry 偏移写成 70，应为 78 | 修正偏移 |
| AAC ASC 未解析 | esds 线性扫描 tag，应层级解析 | 重写为 tag3→tag4→tag5 |
| H264 提取死循环 | NALU length=0 时指针不前进 | 添加 nalu_len==0 break |
| ffmpeg 报 clock rate 错误 | SDP 中 H264 用了 track timescale | 改为固定 90000 |
| ffmpeg 拉流后挂起 | 流结束后 server 未关闭连接 | stream_loop 后 break |

## 使用方式

```bash
# 构建
make

# 启动 server
./build/rtsp_server tool/input.mp4 8554

# 拉流（另一终端）
ffmpeg -rtsp_transport udp -i rtsp://127.0.0.1:8554/stream -c copy output.mp4

# 自动化验证
bash test/verify.sh
```

## 测试结果

```
=== Phase 1: Build ===          PASS
=== Phase 2: MP4 Demux ===      4/4 PASS
=== Phase 3: Raw Stream ===     5/5 PASS
=== Phase 4: RTSP E2E ===      8/8 PASS
Total: 17/17 ALL TESTS PASSED
```
