#!/usr/bin/env python3
"""Persistent whisper worker on dev. TCP socket interface for low-latency communication.
Protocol: client sends [4-byte length][wav data], worker replies [4-byte length][json result]."""
import sys, json, os, time, signal, socket, struct, tempfile

try:
    from config import REMOTE_WORK_DIR as WORK_DIR, WHISPER_MODEL as MODEL, \
        WHISPER_BEAM_SIZE as BEAM, WHISPER_CPU_THREADS as THREADS, \
        WHISPER_LANGUAGE as LANGUAGE, WHISPER_CONDITION_ON_PREVIOUS as CONDITION, \
        WORKER_PORT as PORT
except ImportError:
    WORK_DIR = "/tmp/whisper_work"
    MODEL = os.environ.get("WHISPER_MODEL", "medium")
    BEAM = 1
    THREADS = 12
    LANGUAGE = "zh"
    CONDITION = False
    PORT = 19876

PIDFILE = os.path.join(WORK_DIR, "worker.pid")

def recvall(sock, n):
    data = b""
    while len(data) < n:
        chunk = sock.recv(n - len(data))
        if not chunk:
            raise ConnectionError("connection closed")
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
            segs, info = model.transcribe(f.name, language=LANGUAGE, beam_size=BEAM,
                vad_filter=True, condition_on_previous_text=CONDITION)
            result = [{"start": s.start, "end": s.end, "text": s.text} for s in segs]
            dt = time.time() - t0

        resp = json.dumps({"language": info.language, "segments": result}, ensure_ascii=False).encode()
        conn.sendall(struct.pack(">I", len(resp)) + resp)
        print(f"Done in {dt:.1f}s lang={info.language} ({wav_len}B)", flush=True)
    except Exception as e:
        print(f"Error: {e}", flush=True)
        try:
            err = json.dumps({"segments": [], "error": str(e)}).encode()
            conn.sendall(struct.pack(">I", len(err)) + err)
        except:
            pass
    finally:
        conn.close()

def main():
    os.makedirs(WORK_DIR, exist_ok=True)
    with open(PIDFILE, "w") as f:
        f.write(str(os.getpid()))

    from faster_whisper import WhisperModel
    print(f"Loading model '{MODEL}'...", flush=True)
    model = WhisperModel(MODEL, device="cpu", compute_type="int8", cpu_threads=THREADS)
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
