"""一键运行所有数据源验证"""
import subprocess, sys, pathlib

scripts = sorted(pathlib.Path(__file__).parent.glob("test_*.py"))
for s in scripts:
    print(f"\n{'='*40}\n运行 {s.name}\n{'='*40}")
    subprocess.run([sys.executable, str(s)])
