#!/usr/bin/env python3
"""Benchmark whisper models: tiny, base, small, medium on CPU.
Measures load time, transcription time, and output quality."""
import sys, json, time, os

AUDIO_DIR = "/tmp/whisper_work"
MODELS = ["tiny", "base", "small", "medium"]
FILES = {
    "en":    os.path.join(AUDIO_DIR, "bench_en.wav"),
    "zh":    os.path.join(AUDIO_DIR, "bench_zh.wav"),
    "mixed": os.path.join(AUDIO_DIR, "bench_mixed.wav"),
}

def bench():
    from faster_whisper import WhisperModel
    results = []

    for model_name in MODELS:
        print(f"\n{'='*60}")
        print(f"Model: {model_name}")
        print(f"{'='*60}")

        t0 = time.time()
        model = WhisperModel(model_name, device="cpu", compute_type="int8", cpu_threads=12)
        load_time = time.time() - t0
        print(f"  Load time: {load_time:.1f}s")

        for label, path in FILES.items():
            if not os.path.exists(path):
                print(f"  [{label}] SKIP - file not found")
                continue

            t0 = time.time()
            segs, info = model.transcribe(path, language=None, beam_size=3, vad_filter=True)
            text = "".join(s.text for s in segs).strip()
            trans_time = time.time() - t0

            print(f"  [{label}] {trans_time:.1f}s | lang={info.language} | {text}")
            results.append({
                "model": model_name, "audio": label,
                "load_s": round(load_time, 1), "transcribe_s": round(trans_time, 1),
                "language": info.language, "text": text,
            })

        del model

    # Summary table
    print(f"\n{'='*60}")
    print(f"{'Model':<8} {'Audio':<6} {'Load':>6} {'Trans':>6} {'Lang':>5}  Text")
    print(f"{'-'*60}")
    for r in results:
        print(f"{r['model']:<8} {r['audio']:<6} {r['load_s']:>5.1f}s {r['transcribe_s']:>5.1f}s {r['language']:>5}  {r['text'][:50]}")

    json.dump(results, open(os.path.join(AUDIO_DIR, "benchmark.json"), "w"),
              ensure_ascii=False, indent=2)
    print(f"\nResults saved to {AUDIO_DIR}/benchmark.json")

if __name__ == "__main__":
    bench()
