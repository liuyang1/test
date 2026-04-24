# RTSP Server 模块设计

## 目录结构

```
rtsp-server/
├── Makefile
├── doc/
│   ├── req.md
│   └── design.md
├── src/
│   ├── main.c              # 入口，启动 RTSP server
│   ├── mp4_demux.h/.c      # MP4 解析，提取 track/sample 信息
│   ├── rtp_packet.h/.c     # RTP 打包（H.264 FU-A + AAC AU）
│   ├── rtsp_server.h/.c    # RTSP 协议处理（TCP 监听、请求解析、响应）
│   ├── sdp.h/.c            # SDP 生成
│   ├── rtp_send.h/.c       # RTP/UDP 发送 + 定时调度
│   └── utils.h/.c          # 字节序、日志、通用工具
└── test/
    ├── dump_mp4.c           # 打印 MP4 box 树形结构
    ├── extract_h264.c       # 导出 H.264 裸流验证
    ├── extract_aac.c        # 导出 AAC 裸流验证
    └── verify.sh            # 自动化验证脚本（调用 ffprobe/ffplay 对比）
```

## 模块职责

| 模块 | 职责 | 不负责 |
|------|------|--------|
| mp4_demux | 解析 MP4 box，构建 sample 索引，按索引读取 sample 数据 | 网络、RTP |
| rtp_packet | 将一帧数据打成 RTP 包列表（H.264 FU-A / AAC AU） | socket 发送 |
| rtp_send | UDP socket 发包 | 打包逻辑 |
| sdp | 根据 track 信息生成 SDP 文本 | 协议交互 |
| rtsp_server | RTSP 协议解析与响应，串联所有模块，控制播放流程 | 底层解析/打包 |
| utils | 字节序转换、日志、通用工具 | 业务逻辑 |

## 核心数据结构

### MP4 层

```c
// 单个 sample（一帧音频或视频）
typedef struct {
    uint64_t offset;      // 文件中的字节偏移
    uint32_t size;        // sample 大小
    uint64_t dts;         // 解码时间（timescale 单位）
    uint64_t pts;         // 显示时间（dts + cts_offset）
    int      is_sync;     // 是否关键帧（来自 stss）
} mp4_sample_t;

// 一条 track
typedef struct {
    uint32_t track_id;
    uint32_t timescale;
    uint32_t codec;           // 'avc1' 或 'mp4a'
    uint32_t sample_count;
    mp4_sample_t *samples;    // sample 数组

    // H.264 特有
    uint8_t *sps;  uint16_t sps_len;
    uint8_t *pps;  uint16_t pps_len;
    uint8_t  nalu_len_size;   // avcC 中的 length field 字节数（通常 4）

    // AAC 特有
    uint8_t *asc;  uint16_t asc_len;  // AudioSpecificConfig
    uint32_t sample_rate;
    uint16_t channels;
} mp4_track_t;

// 整个 MP4 文件
typedef struct {
    FILE       *fp;
    uint32_t    track_count;
    mp4_track_t tracks[8];
    int         video_idx;    // -1 表示无
    int         audio_idx;
} mp4_demux_t;
```

### RTP 打包层

```c
typedef struct {
    uint8_t  *data;
    uint16_t  len;
} rtp_pkt_t;

// 一帧可能产生多个 RTP 包
typedef struct {
    rtp_pkt_t *pkts;
    int        count;
} rtp_pkt_list_t;
```

### RTSP/RTP 会话层

```c
// 单个 RTP 通道（一条 track 对应一个）
typedef struct {
    int      sockfd;
    uint16_t server_port;
    uint16_t client_port;
    uint32_t ssrc;
    uint16_t seq;
    struct sockaddr_in client_addr;
} rtp_channel_t;

// 客户端 session（当前单客户端，后续可扩展为数组/链表）
typedef struct {
    char          session_id[16];
    int           tcp_fd;         // RTSP 控制连接
    rtp_channel_t video_ch;
    rtp_channel_t audio_ch;
    int           playing;        // 0=stopped, 1=playing
} rtsp_session_t;

// RTSP 服务器
typedef struct {
    int            listen_fd;
    uint16_t       port;          // 默认 8554
    mp4_demux_t   *demux;
    rtsp_session_t session;       // 单客户端；多客户端时改为动态管理
} rtsp_server_t;
```

## 模块接口

### mp4_demux

```c
int  mp4_open(mp4_demux_t *demux, const char *path);
int  mp4_read_sample(mp4_demux_t *demux, mp4_track_t *track,
                     uint32_t sample_idx, uint8_t *buf, uint32_t buf_size);
void mp4_close(mp4_demux_t *demux);
```

- `mp4_open`: 解析整个 MP4 结构，构建 sample 索引
- `mp4_read_sample`: 按索引读取一个 sample 的原始数据
- 调用方控制遍历顺序（按 DTS 排序发送）

### rtp_packet

```c
int  rtp_packetize_h264(rtp_pkt_list_t *out, const uint8_t *nalu,
                        uint32_t nalu_len, uint32_t ts,
                        uint16_t *seq, uint32_t ssrc);
int  rtp_packetize_aac(rtp_pkt_list_t *out, const uint8_t *frame,
                       uint32_t frame_len, uint32_t ts,
                       uint16_t *seq, uint32_t ssrc);
void rtp_pkt_list_free(rtp_pkt_list_t *list);
```

- 纯数据转换，不涉及 socket
- `seq` 指针传入传出，调用方持有状态

### rtp_send

```c
int rtp_send_pkts(rtp_channel_t *ch, const rtp_pkt_list_t *list);
```

### sdp

```c
int sdp_generate(char *buf, int buf_size, const mp4_demux_t *demux,
                 const char *server_ip);
```

### rtsp_server

```c
int  rtsp_server_init(rtsp_server_t *srv, uint16_t port, mp4_demux_t *demux);
int  rtsp_server_run(rtsp_server_t *srv);   // 主循环
void rtsp_server_stop(rtsp_server_t *srv);
```

## 数据流

```
mp4_open() → 构建 sample 索引
                │
                ▼
        遍历 sample（按 DTS 顺序）
                │
                ▼
        mp4_read_sample() → 原始帧数据
                │
                ▼
        rtp_packetize_h264/aac() → RTP 包列表
                │
                ▼
        rtp_send_pkts() → UDP 发送
                │
                ▼
        clock_nanosleep() → 等待下一帧时间
```

## 分阶段验证

| 阶段 | 验证方式 |
|------|---------|
| 1. MP4 box 解析 | `dump_mp4` 输出 vs `mp4box -info` 对比 |
| 2. sample 索引 | 输出 offset/size/DTS/PTS vs `ffprobe -show_packets` 对比 |
| 3. H.264 提取 | 导出 .h264 文件，`ffplay test.h264` 能播放 |
| 4. AAC 提取 | 导出 .aac 文件（加 ADTS header），`ffplay test.aac` 能播放 |
| 5. RTSP 握手 | VLC 连接，Wireshark 验证 OPTIONS/DESCRIBE/SETUP/PLAY 交互 |
| 6. 视频推流 | VLC 有画面 |
| 7. 音频推流 | VLC 有声音 |
| 8. 音视频同步 | 正常播放，唇音同步 |
