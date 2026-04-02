#!/usr/bin/env python3
"""Persistent FunASR worker on dev. TCP socket interface.
Protocol: client sends [4-byte length][wav data], worker replies [4-byte length][json result]."""
import sys, json, os, time, signal, socket, struct, tempfile

CACHE = "/home/lyz/.cache/modelscope/hub/models/iic"
MODEL_DIR = f"{CACHE}/speech_seaco_paraformer_large_asr_nat-zh-cn-16k-common-vocab8404-pytorch"
VAD_DIR = f"{CACHE}/speech_fsmn_vad_zh-cn-16k-common-pytorch"
PUNC_DIR = f"{CACHE}/punc_ct-transformer_cn-en-common-vocab471067-large"
PORT = 19876
WORK_DIR = "/tmp/whisper_work"
PIDFILE = os.path.join(WORK_DIR, "worker.pid")

def recvall(sock, n):
    data = b""
    while len(data) < n:
        chunk = sock.recv(n - len(data))
        if not chunk:
            raise ConnectionError("closed")
        data += chunk
    return data

def handle_client(conn, model):
    try:
        header = recvall(conn, 4)
        wav_len = struct.unpack(">I", header)[0]
        wav_data = recvall(conn, wav_len)

        with tempfile.NamedTemporaryFile(suffix=".wav", dir=WORK_DIR, delete=True) as f:
            f.write(wav_data)
            f.flush()
            t0 = time.time()
            res = model.generate(input=f.name)
            dt = time.time() - t0

        text = res[0]["text"] if res else ""
        resp = json.dumps({"text": text}, ensure_ascii=False).encode()
        conn.sendall(struct.pack(">I", len(resp)) + resp)
        print(f"Done in {dt:.1f}s ({wav_len}B) | {text[:50]}", flush=True)
    except Exception as e:
        print(f"Error: {e}", flush=True)
        try:
            err = json.dumps({"text": "", "error": str(e)}).encode()
            conn.sendall(struct.pack(">I", len(err)) + err)
        except: pass
    finally:
        conn.close()

def main():
    os.makedirs(WORK_DIR, exist_ok=True)
    with open(PIDFILE, "w") as f:
        f.write(str(os.getpid()))

    from funasr import AutoModel
    print("Loading FunASR model...", flush=True)
    model = AutoModel(model=MODEL_DIR, vad_model=VAD_DIR, punc_model=PUNC_DIR, disable_update=True)
    print(f"Ready. Listening on port {PORT}...", flush=True)

    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(("0.0.0.0", PORT))
    srv.listen(2)

    def cleanup(sig, frame):
        srv.close()
        try: os.unlink(PIDFILE)
        except: pass
        sys.exit(0)
    signal.signal(signal.SIGTERM, cleanup)
    signal.signal(signal.SIGINT, cleanup)

    while True:
        conn, addr = srv.accept()
        handle_client(conn, model)

if __name__ == "__main__":
    main()
