#!/bin/bash
# Automated verification for RTSP server
set -e
cd "$(dirname "$0")/.."

PASS=0
FAIL=0
pass() { echo "  PASS: $1"; PASS=$((PASS+1)); }
fail() { echo "  FAIL: $1"; FAIL=$((FAIL+1)); }

echo "=== Phase 1: Build ==="
timeout 15 make clean all >/dev/null 2>&1 && pass "build" || fail "build"

echo "=== Phase 2: MP4 Demux ==="
OUT=$(timeout 5 ./build/dump_mp4 tool/input.mp4 2>&1)
echo "$OUT" | grep -q "codec=avc1" && pass "video track found" || fail "video track"
echo "$OUT" | grep -q "codec=mp4a" && pass "audio track found" || fail "audio track"
echo "$OUT" | grep -q "sps_len=26" && pass "SPS parsed" || fail "SPS"
echo "$OUT" | grep -q "asc_len=5" && pass "ASC parsed" || fail "ASC"

echo "=== Phase 3: Raw Stream Extraction ==="
timeout 10 ./build/extract_h264 tool/input.mp4 /tmp/verify_test.h264 2>/dev/null
timeout 10 ./build/extract_aac tool/input.mp4 /tmp/verify_test.aac 2>/dev/null

H264_OK=$(timeout 10 ffprobe -v error -show_entries stream=codec_name /tmp/verify_test.h264 2>&1)
echo "$H264_OK" | grep -q "h264" && pass "H264 extraction" || fail "H264 extraction"

AAC_OK=$(timeout 10 ffprobe -v error -show_entries stream=codec_name,sample_rate /tmp/verify_test.aac 2>&1)
echo "$AAC_OK" | grep -q "aac" && pass "AAC extraction" || fail "AAC extraction"
echo "$AAC_OK" | grep -q "48000" && pass "AAC sample rate" || fail "AAC sample rate"

# Decode test
timeout 10 ffmpeg -y -i /tmp/verify_test.h264 -c:v copy -f null /dev/null </dev/null 2>/dev/null && pass "H264 decode" || fail "H264 decode"
timeout 10 ffmpeg -y -i /tmp/verify_test.aac -c:a copy -f null /dev/null </dev/null 2>/dev/null && pass "AAC decode" || fail "AAC decode"

echo "=== Phase 4: RTSP End-to-End ==="
# Cleanup ports
fuser -k 8554/tcp 2>/dev/null || true
fuser -k 50000/udp 2>/dev/null || true
fuser -k 50002/udp 2>/dev/null || true
sleep 1

# Start server
./build/rtsp_server tool/input.mp4 8554 > /tmp/verify_server.log 2>&1 &
SERVER_PID=$!
sleep 1

if kill -0 $SERVER_PID 2>/dev/null; then
    pass "server started"
else
    fail "server start"
    echo "ABORT: server failed to start"
    cat /tmp/verify_server.log
    exit 1
fi

# Pull stream
rm -f /tmp/verify_output.mp4
timeout 15 ffmpeg -y -rtsp_transport udp \
    -i rtsp://127.0.0.1:8554/stream \
    -c copy /tmp/verify_output.mp4 </dev/null 2>/dev/null
FFMPEG_EXIT=$?

kill $SERVER_PID 2>/dev/null; wait $SERVER_PID 2>/dev/null

# Check server log
grep -q "streaming ended" /tmp/verify_server.log && pass "stream completed" || fail "stream completed"

# Check output file
if [ -f /tmp/verify_output.mp4 ] && [ $(stat -c%s /tmp/verify_output.mp4) -gt 1000 ]; then
    pass "output file created"
else
    fail "output file created"
fi

# Verify output content
VDUR=$(timeout 5 ffprobe -v error -select_streams v -show_entries stream=duration \
    -of csv=p=0 /tmp/verify_output.mp4 2>&1)
VDUR_INT=$(echo "$VDUR" | cut -d. -f1)
if [ -n "$VDUR_INT" ] && [ "$VDUR_INT" -ge 4 ] 2>/dev/null; then
    pass "video duration ${VDUR}s"
else
    fail "video duration (got: $VDUR)"
fi

VFRAMES=$(timeout 10 ffprobe -v error -count_frames -select_streams v \
    -show_entries stream=nb_read_frames -of csv=p=0 /tmp/verify_output.mp4 2>&1)
if [ -n "$VFRAMES" ] && [ "$VFRAMES" -ge 120 ] 2>/dev/null; then
    pass "video frames: $VFRAMES"
else
    fail "video frames (got: $VFRAMES)"
fi

AFRAMES=$(timeout 10 ffprobe -v error -count_frames -select_streams a \
    -show_entries stream=nb_read_frames -of csv=p=0 /tmp/verify_output.mp4 2>&1)
if [ -n "$AFRAMES" ] && [ "$AFRAMES" -ge 230 ] 2>/dev/null; then
    pass "audio frames: $AFRAMES"
else
    fail "audio frames (got: $AFRAMES)"
fi

# Decode output to verify playability
timeout 10 ffmpeg -y -i /tmp/verify_output.mp4 -f null /dev/null </dev/null 2>/dev/null \
    && pass "output playable" || fail "output playable"

# Cleanup
rm -f /tmp/verify_test.h264 /tmp/verify_test.aac /tmp/verify_output.mp4

echo ""
echo "=== Results: $PASS passed, $FAIL failed ==="
[ $FAIL -eq 0 ] && echo "ALL TESTS PASSED" || echo "SOME TESTS FAILED"
exit $FAIL
