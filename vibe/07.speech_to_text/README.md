# Speech to Text — Voice Input

Record audio locally → transcribe on remote dev server via SSH → result to clipboard.

## Architecture

```
┌─────────────────────┐    scp wav     ┌──────────────────────┐
│  Local (16GB)       │ ──────────────→│  Dev (92GB/48core)   │
│                     │                │                      │
│  voice_input_gui.py │    ssh poll    │  worker.py           │
│  (GTK3 window)      │ ←─────────────│  (faster-whisper,    │
│  parecord → wav     │    result.json │   model stays loaded)│
│  xclip ← result    │                │                      │
└─────────────────────┘                └──────────────────────┘
```

- **Local**: records audio, uploads wav, receives transcription, copies to clipboard
- **Dev**: persistent whisper worker keeps model in memory, transcribes on demand
- **SSH ControlMaster**: connection reuse avoids repeated wssh proxy handshake

## Files

| File | Where | Purpose |
|------|-------|---------|
| `config.py` | local + dev | Centralized configuration |
| `voice_input_gui.py` | local | GTK3 floating window, record + trigger transcription |
| `worker.py` | dev | Persistent whisper process, watches for trigger file |
| `transcribe.py` | dev | Standalone one-shot transcription (used by `main.py`) |
| `main.py` | local | CLI alternative: record → transcribe → SRT output |
| `~/.local/bin/voice-input` | local | Shell launcher for awesome WM keybinding |

## Configuration (`config.py`)

```python
REMOTE_HOST = "dev"           # SSH host alias (must be in ~/.ssh/config)
REMOTE_PYTHON = "~/.local/share/rtx/installs/python/3.11.7/bin/python3"
REMOTE_WORK_DIR = "/tmp/whisper_work"

WHISPER_MODEL = "small"       # tiny|base|small|medium|large-v2
WHISPER_BEAM_SIZE = 3         # Lower = faster, higher = more accurate
WHISPER_CPU_THREADS = 12      # Match remote CPU cores

RECORD_SAMPLE_RATE = 16000    # 16kHz mono, whisper's native rate
RECORD_FORMAT = "s16le"
RECORD_CHANNELS = 1
```

Override model via environment: `WHISPER_MODEL=base python3 worker.py`

## Deployment

### 1. Remote (dev server)

```bash
# Install faster-whisper (needs Python 3.8+)
~/.local/share/rtx/installs/python/3.11.7/bin/pip3 install --only-binary=av faster-whisper

# Deploy scripts
ssh dev "mkdir -p /tmp/whisper_work"
scp config.py worker.py transcribe.py dev:/tmp/whisper_work/

# Start persistent worker (model loads once, stays in memory)
ssh dev "cd /tmp/whisper_work && nohup ~/.local/share/rtx/installs/python/3.11.7/bin/python3 worker.py > worker.log 2>&1 &"

# Verify
ssh dev "cat /tmp/whisper_work/worker.log"
# Should show: "Loading model 'small'..." then "Ready. Waiting for jobs..."
```

### 2. Local (workstation)

Dependencies (Ubuntu/Debian):
```bash
sudo apt install pulseaudio-utils xclip xdotool
# pulseaudio-utils provides parecord
# GTK3 + Python GI bindings (usually pre-installed on Ubuntu desktop)
```

Install launcher:
```bash
# Already at ~/.local/bin/voice-input
chmod +x ~/.local/bin/voice-input
```

### 3. SSH ControlMaster (critical for speed)

Add to `~/.ssh/config` under your dev host:
```
host dev
    ...existing config...
    ControlMaster auto
    ControlPath /tmp/ssh-dev-%r@%h:%p
    ControlPersist 4h
```

Establish master connection: `ssh -fN dev`

Without this, each SSH call goes through wssh proxy (~5s). With it: ~0.3s.

### 4. Awesome WM integration

In `~/.config/awesome/rc.lua`:

Keybinding (in `globalkeys`):
```lua
awful.key({ modkey }, "v", function() awful.spawn("voice-input") end,
          {description = "voice input (toggle)", group = "launcher"})
```

Window rule (in `awful.rules.rules`):
```lua
{ rule = { class = "Voice-Input" },
  properties = { floating = true, ontop = true, placement = awful.placement.centered }
},
```

Reload: `Mod4+Ctrl+r` or `echo 'awesome.restart()' | awesome-client`

## Usage

### GUI mode (Mod4+v)

1. `Mod4+v` → floating window appears, recording starts
2. `Mod4+v` again or click **⏹ Stop** → uploads + transcribes
3. Result copied to clipboard, window auto-closes after 1.5s
4. `Esc` → cancel recording, window closes

### CLI mode

```bash
# Record and transcribe (Ctrl+C to stop recording)
python3 main.py -l zh -o output.srt

# Transcribe existing file
python3 main.py -i recording.wav -o output.srt

# Use different model
python3 main.py -m base -l en
```

## Troubleshooting

- **Worker not running**: GUI auto-starts it on first use; or manually start per deployment step 1
- **Slow transcription**: check `ssh dev "cat /tmp/whisper_work/worker.log"` for actual time; if SSH is slow, ensure ControlMaster is active (`ls /tmp/ssh-dev-*`)
- **No audio**: check `parecord --list` for available sources; ensure PulseAudio is running
- **Model download**: first run downloads model to `~/.cache/huggingface/` on dev; subsequent runs use cache
