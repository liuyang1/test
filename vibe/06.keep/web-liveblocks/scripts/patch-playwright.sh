#!/bin/bash
# Patches Playwright's reinstall scripts to remove insecure curl -k flag.
# CVE: CWE-295 — curl -k disables SSL certificate validation, enabling MitM attacks.
# Run automatically via npm postinstall.

PLAYWRIGHT_BIN="node_modules/playwright-core/bin"

if [ -d "$PLAYWRIGHT_BIN" ]; then
  for f in "$PLAYWRIGHT_BIN"/reinstall_*_mac.sh; do
    [ -f "$f" ] && sed -i.bak 's/curl \(.*\)-k /curl \1/g' "$f" && rm -f "$f.bak"
  done
  echo "[security] Patched Playwright scripts: removed curl -k (insecure SSL bypass)"
fi
