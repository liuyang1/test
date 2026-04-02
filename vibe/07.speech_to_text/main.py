#!/usr/bin/env python3
"""Local: record audio, scp to dev, ssh transcribe, get result back."""
import subprocess, sys, os, json, tempfile, time, signal, wave
from config import (REMOTE_HOST as REMOTE, REMOTE_WORK_DIR as REMOTE_DIR,
                    REMOTE_PYTHON, RECORD_SAMPLE_RATE)

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
REMOTE_SCRIPT = f"{REMOTE_DIR}/transcribe.py"

def ensure_remote_setup():
    subprocess.run(["ssh", REMOTE, f"mkdir -p {REMOTE_DIR}"], check=True)
    subprocess.run(["scp", "-q", os.path.join(SCRIPT_DIR, "transcribe.py"),
                     f"{REMOTE}:{REMOTE_SCRIPT}"], check=True)

def record(output_path, sample_rate=RECORD_SAMPLE_RATE):
    """Record from default mic using arecord. Ctrl+C to stop."""
    print("🎙️  Recording... Press Ctrl+C to stop.")
    proc = subprocess.Popen([
        "arecord", "-f", "S16_LE", "-r", str(sample_rate), "-c", "1", "-t", "wav", output_path
    ], stderr=subprocess.DEVNULL)
    try:
        proc.wait()
    except KeyboardInterrupt:
        proc.send_signal(signal.SIGINT)
        proc.wait()
    print(f"✅ Saved: {output_path}")

def transcribe_remote(local_wav, language=None, model="medium"):
    remote_wav = f"{REMOTE_DIR}/input.wav"
    subprocess.run(["scp", "-q", local_wav, f"{REMOTE}:{remote_wav}"], check=True)
    cmd = f"{REMOTE_PYTHON} {REMOTE_SCRIPT} {remote_wav} --model {model}"
    if language:
        cmd += f" --language {language}"
    result = subprocess.run(["ssh", REMOTE, cmd], capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error: {result.stderr}", file=sys.stderr)
        sys.exit(1)
    return json.loads(result.stdout)

def format_time(seconds):
    m, s = divmod(int(seconds), 60)
    h, m = divmod(m, 60)
    ms = int((seconds % 1) * 1000)
    return f"{h:02d}:{m:02d}:{s:02d},{ms:03d}"

def to_srt(segments):
    lines = []
    for i, s in enumerate(segments, 1):
        lines.append(f"{i}")
        lines.append(f"{format_time(s['start'])} --> {format_time(s['end'])}")
        lines.append(s["text"].strip())
        lines.append("")
    return "\n".join(lines)

def main():
    import argparse
    p = argparse.ArgumentParser(description="Record locally, transcribe on dev via ssh")
    p.add_argument("-i", "--input", help="Skip recording, use existing wav file")
    p.add_argument("-o", "--output", help="Output srt file path")
    p.add_argument("-l", "--language", default=None, help="Language hint (e.g. zh, en)")
    p.add_argument("-m", "--model", default="medium", help="Whisper model size")
    args = p.parse_args()

    ensure_remote_setup()

    if args.input:
        wav_path = args.input
    else:
        wav_path = tempfile.mktemp(suffix=".wav", dir="/tmp")
        record(wav_path)

    print("📡 Uploading & transcribing on dev...")
    t0 = time.time()
    result = transcribe_remote(wav_path, args.language, args.model)
    elapsed = time.time() - t0
    print(f"⏱️  Done in {elapsed:.1f}s (lang={result['language']})")

    srt = to_srt(result["segments"])
    if args.output:
        with open(args.output, "w", encoding="utf-8") as f:
            f.write(srt)
        print(f"📄 SRT saved: {args.output}")
    else:
        print("\n" + srt)

if __name__ == "__main__":
    main()
