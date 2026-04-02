#!/usr/bin/env python3
"""Remote whisper transcription script. Run on dev via ssh."""
import sys, json, os

def transcribe(audio_path, language=None, model_size="medium"):
    from faster_whisper import WhisperModel
    model = WhisperModel(model_size, device="cpu", compute_type="int8", cpu_threads=8)
    segments, info = model.transcribe(audio_path, language=language, beam_size=5)
    result = [{"start": s.start, "end": s.end, "text": s.text} for s in segments]
    json.dump({"language": info.language, "segments": result}, sys.stdout, ensure_ascii=False)

if __name__ == "__main__":
    import argparse
    p = argparse.ArgumentParser()
    p.add_argument("audio")
    p.add_argument("--language", default=None)
    p.add_argument("--model", default="medium")
    args = p.parse_args()
    transcribe(args.audio, args.language, args.model)
