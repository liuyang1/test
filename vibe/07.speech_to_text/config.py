"""Centralized configuration for voice input / speech-to-text."""
import os

# ── Remote server (runs whisper) ──────────────────────────────
REMOTE_HOST = "dev"                          # SSH host alias
REMOTE_PYTHON = "~/.local/share/rtx/installs/python/3.11.7/bin/python3"
REMOTE_WORK_DIR = "/tmp/whisper_work"
WORKER_PORT = 19876                          # TCP port for worker; tunneled via SSH

# ── Whisper model ─────────────────────────────────────────────
# Options: tiny, base, small, medium, large-v2
WHISPER_MODEL = os.environ.get("WHISPER_MODEL", "medium")
WHISPER_BEAM_SIZE = 1           # 1 = greedy (fastest), 3-5 = beam search (slower, marginal gain)
WHISPER_CPU_THREADS = 12        # Sweet spot on 48-core dev; more threads = diminishing returns
WHISPER_LANGUAGE = "zh"         # Pre-set language skips 2s detection. None = auto-detect.
WHISPER_CONDITION_ON_PREVIOUS = False  # False = faster, avoids hallucination loops

# ── Recording ─────────────────────────────────────────────────
RECORD_SAMPLE_RATE = 16000
RECORD_FORMAT = "s16le"
RECORD_CHANNELS = 1

# ── Local paths ───────────────────────────────────────────────
WAVFILE = "/tmp/voice-input.wav"
LOCKFILE = "/tmp/voice-input-gui.pid"
