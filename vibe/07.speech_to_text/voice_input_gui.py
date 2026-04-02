#!/usr/bin/env python3
"""Voice input: floating GTK3 window, record → TCP to dev whisper → clipboard → auto close.
Mod4+v to launch. Mod4+v again or click Stop to finish. Esc to cancel."""

import gi, os, subprocess, signal, threading, time, json, socket, struct
gi.require_version("Gtk", "3.0")
from gi.repository import Gtk, Gdk, GLib
from config import (WAVFILE, LOCKFILE, WORKER_PORT,
                    RECORD_SAMPLE_RATE, RECORD_FORMAT, RECORD_CHANNELS)

LOCAL_PORT = WORKER_PORT  # SSH tunnel maps this to remote

CSS = b"""
window { background: #3f3f3f; border-radius: 12px; }
label { color: #dcdccc; }
.title { font-size: 14px; font-weight: bold; color: #f0dfaf; }
.status { font-size: 12px; color: #7f9f7f; }
.time { font-size: 28px; font-weight: bold; color: #cc9393; }
.result { font-size: 12px; color: #dcdccc; padding: 4px; }
button { background: #4f4f4f; color: #dcdccc; border: 1px solid #6f6f6f;
         border-radius: 6px; padding: 6px 16px; min-height: 0; }
button:hover { background: #5f5f5f; }
.btn-stop { background: #8c5353; }
.btn-stop:hover { background: #9c6363; }
.btn-cancel { background: #4f4f4f; }
"""

def tcp_transcribe(wav_path):
    """Send wav over TCP, receive JSON result. ~50ms overhead vs 2.4s for scp+ssh."""
    with open(wav_path, "rb") as f:
        wav_data = f.read()
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(60)
    sock.connect(("127.0.0.1", LOCAL_PORT))
    sock.sendall(struct.pack(">I", len(wav_data)) + wav_data)
    resp_len = struct.unpack(">I", _recvall(sock, 4))[0]
    resp = _recvall(sock, resp_len)
    sock.close()
    return json.loads(resp)

def _recvall(sock, n):
    data = b""
    while len(data) < n:
        chunk = sock.recv(n - len(data))
        if not chunk:
            raise ConnectionError("connection closed")
        data += chunk
    return data

class VoiceWindow(Gtk.Window):
    def __init__(self):
        super().__init__(title="Voice Input")
        self.set_wmclass("voice-input", "Voice-Input")
        self.set_default_size(280, -1)
        self.set_resizable(False)
        self.set_keep_above(True)
        self.set_decorated(False)
        self.set_position(Gtk.WindowPosition.CENTER)
        self.set_type_hint(Gdk.WindowTypeHint.DIALOG)

        self.rec_proc = None
        self.recording = False
        self.start_time = 0

        sp = Gtk.CssProvider()
        sp.load_from_data(CSS)
        Gtk.StyleContext.add_provider_for_screen(
            Gdk.Screen.get_default(), sp, Gtk.STYLE_PROVIDER_PRIORITY_APPLICATION)

        box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL, spacing=8)
        box.set_margin_top(16); box.set_margin_bottom(16)
        box.set_margin_start(20); box.set_margin_end(20)

        self.title_label = Gtk.Label(label="🎙️ Voice Input")
        self.title_label.get_style_context().add_class("title")
        box.pack_start(self.title_label, False, False, 0)

        self.time_label = Gtk.Label(label="00:00")
        self.time_label.get_style_context().add_class("time")
        box.pack_start(self.time_label, False, False, 4)

        self.status_label = Gtk.Label(label="Recording...")
        self.status_label.get_style_context().add_class("status")
        box.pack_start(self.status_label, False, False, 0)

        self.result_label = Gtk.Label()
        self.result_label.get_style_context().add_class("result")
        self.result_label.set_line_wrap(True)
        self.result_label.set_max_width_chars(30)
        self.result_label.set_visible(False)
        box.pack_start(self.result_label, False, False, 0)

        btnbox = Gtk.Box(spacing=8)
        btnbox.set_halign(Gtk.Align.CENTER)
        self.stop_btn = Gtk.Button(label="⏹ Stop")
        self.stop_btn.get_style_context().add_class("btn-stop")
        self.stop_btn.connect("clicked", lambda _: self.stop_and_transcribe())
        btnbox.pack_start(self.stop_btn, False, False, 0)
        self.cancel_btn = Gtk.Button(label="✕ Cancel")
        self.cancel_btn.get_style_context().add_class("btn-cancel")
        self.cancel_btn.connect("clicked", lambda _: self.cancel())
        btnbox.pack_start(self.cancel_btn, False, False, 0)
        box.pack_start(btnbox, False, False, 4)

        hint = Gtk.Label(label="Mod4+v stop  |  Esc cancel")
        hint.get_style_context().add_class("status")
        box.pack_start(hint, False, False, 0)

        self.add(box)
        self.connect("key-press-event", self.on_key)
        self.connect("destroy", self.on_destroy)
        self.start_recording()

    def start_recording(self):
        self.recording = True
        self.start_time = time.time()
        self.rec_proc = subprocess.Popen(
            ["parecord", f"--format={RECORD_FORMAT}", f"--rate={RECORD_SAMPLE_RATE}",
             f"--channels={RECORD_CHANNELS}", WAVFILE],
            stderr=subprocess.DEVNULL)
        GLib.timeout_add(200, self.update_timer)

    def update_timer(self):
        if not self.recording:
            return False
        elapsed = time.time() - self.start_time
        m, s = divmod(int(elapsed), 60)
        self.time_label.set_text(f"{m:02d}:{s:02d}")
        return True

    def on_key(self, widget, event):
        if event.keyval == Gdk.KEY_Escape:
            self.cancel()
            return True
        return False

    def cancel(self):
        if self.rec_proc:
            self.rec_proc.terminate()
            self.rec_proc.wait()
        self.cleanup_and_quit()

    def stop_and_transcribe(self):
        if not self.recording:
            return
        self.recording = False
        self.rec_proc.terminate()
        self.rec_proc.wait()
        self.stop_btn.set_sensitive(False)
        self.status_label.set_text("Transcribing...")
        threading.Thread(target=self.transcribe, daemon=True).start()

    def transcribe(self):
        try:
            t0 = time.time()
            data = tcp_transcribe(WAVFILE)
            text = data.get("text") or "".join(s["text"] for s in data.get("segments", [])).strip()
            dt = time.time() - t0
            print(f"E2E: {dt:.2f}s", flush=True)
        except Exception as e:
            text = ""
            GLib.idle_add(self.status_label.set_text, f"Error: {e}")

        if text:
            subprocess.run(["xclip", "-selection", "clipboard"],
                           input=text.encode(), check=True)
            GLib.idle_add(self.show_result_and_close, text)
        else:
            GLib.idle_add(self.status_label.set_text, "No speech detected")
            GLib.timeout_add(1500, self.cleanup_and_quit)

    def show_result_and_close(self, text):
        display = text if len(text) <= 60 else text[:57] + "..."
        self.status_label.set_text("✅ Copied to clipboard")
        self.result_label.set_text(display)
        self.result_label.set_visible(True)
        GLib.timeout_add(1500, self.cleanup_and_quit)

    def cleanup_and_quit(self):
        try: os.unlink(WAVFILE)
        except: pass
        try: os.unlink(LOCKFILE)
        except: pass
        Gtk.main_quit()
        return False

    def on_destroy(self, *_):
        if self.rec_proc and self.rec_proc.poll() is None:
            self.rec_proc.terminate()
        try: os.unlink(LOCKFILE)
        except: pass

def main():
    if os.path.exists(LOCKFILE):
        try:
            pid = int(open(LOCKFILE).read().strip())
            os.kill(pid, signal.SIGUSR1)
            return
        except (ProcessLookupError, ValueError):
            os.unlink(LOCKFILE)

    with open(LOCKFILE, "w") as f:
        f.write(str(os.getpid()))

    win = VoiceWindow()
    def on_stop_signal(sig, frame):
        GLib.idle_add(win.stop_and_transcribe)
    signal.signal(signal.SIGUSR1, on_stop_signal)

    win.show_all()
    win.result_label.set_visible(False)
    Gtk.main()

if __name__ == "__main__":
    main()
