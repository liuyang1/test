# Zero-Server PWA Deployment Plan

## Overview

Replace the self-hosted Node.js sync server with a fully serverless architecture using y-webrtc (P2P) + PWA (installable web app). One codebase covers all platforms (desktop, iOS, Android) with automatic hot updates.

## Design Principles

1. **User experience first** — pixel-accurate Google Keep clone is the primary goal. If UX and serverless conflict, UX wins at any cost.
2. **Zero-server where possible** — eliminate self-hosted servers using P2P sync, but never at the expense of UX.

## Platform Strategy

```
┌─────────────────────────────────────────────────────┐
│  Web (Desktop / Android)                            │
│  PWA + y-webrtc + IndexedDB                         │
│  Zero server, auto-update, installable              │
├─────────────────────────────────────────────────────┤
│  iOS                                                │
│  Native Swift shell + WKWebView hybrid              │
│                                                     │
│  Native shell (~200 lines Swift) handles:           │
│    • Widget — one-tap new note from lock screen     │
│    • Share Extension — share from any app           │
│    • 3D Touch / Haptic Touch quick actions          │
│    • Siri Shortcuts — voice input                   │
│    • App lifecycle & background handling            │
│                                                     │
│  WKWebView loads the same PWA URL and handles:      │
│    • Full editor UI (same code as web)              │
│    • Yjs + y-webrtc sync (same code as web)         │
│    • IndexedDB persistence                          │
│    • JS Bridge to read Widget/Share data            │
│                                                     │
│  Result:                                            │
│    ✅ Native-quality quick input (Widget, Share)    │
│    ✅ Hot-update for UI/logic (web code auto-loads) │
│    ✅ Same sync protocol as all other devices       │
│    ⚠️ Requires TestFlight or App Store for install  │
└─────────────────────────────────────────────────────┘
```

### Why not pure PWA on iOS?

PWA cannot access: Widgets, Share Extension, Siri, 3D Touch quick actions. These are critical for the "quick capture" UX that Google Keep's native iOS app provides. The 3-5 second path to input (unlock → find icon → open → wait → tap → type) vs 1 second (notification center → widget → type) is an unacceptable UX gap.

### Why not pure native Swift?

The editor, note grid, sync logic, and all UI features are already implemented in the web codebase (React + Yjs). Rewriting in Swift would double the maintenance burden. The WKWebView hybrid approach reuses 95% of the web code while adding native system integration on top.

## Architecture

```
GitHub repo
    │ git push
    ▼
Vercel / GitHub Pages (free static hosting, auto-deploy)
    │ user visits URL
    ▼
PWA App (runs in browser)
    ├── Service Worker    → offline cache + auto update
    ├── IndexedDB         → local data persistence (already implemented)
    ├── y-webrtc          → P2P sync via public signaling server
    └── manifest.json     → installable to home screen
```

## How It Works

### Data Sync (y-webrtc)

```
Device A ◀── WebRTC DataChannel (encrypted, P2P) ──▶ Device B
    │                                                     │
    └──── Public Signaling Server (yjs.dev) ──────────────┘
          Only exchanges "who is online" info
          Never touches note data
```

- Signaling server: `wss://signaling.yjs.dev` (free, maintained by Yjs team)
- Data never leaves the two devices — encrypted P2P direct connection
- Each device persists to IndexedDB locally
- Yjs CRDT handles conflict resolution automatically

### Limitation & Workaround

**Limitation**: Two devices must be online simultaneously to sync.

**Workaround**: Keep one device always online (e.g., a Mac Mini, Raspberry Pi, or a pinned browser tab). This acts as a "personal sync hub" — other devices sync with it whenever they come online, then can go offline freely.

Optionally automate with a headless browser (Puppeteer/Playwright) via systemd/cron to keep the page open permanently.

### PWA on iOS

When users open the URL in Safari and tap "Add to Home Screen":
- Full-screen app (no browser chrome)
- Home screen icon
- Offline support (Service Worker cache)
- Auto-update (new code deployed → next app open fetches update)
- IndexedDB persistence
- WebRTC P2P sync

Not available: push notifications (limited on iOS), App Store listing.

## Implementation Steps

### 1. Add PWA support

```bash
npm install vite-plugin-pwa -D
```

Update `vite.config.ts`:
```ts
import { VitePWA } from 'vite-plugin-pwa'

export default defineConfig({
  plugins: [
    react(),
    VitePWA({
      registerType: 'autoUpdate',
      manifest: {
        name: 'Keep Notes',
        short_name: 'Keep',
        start_url: '/',
        display: 'standalone',
        background_color: '#ffffff',
        theme_color: '#fbbc04',
        icons: [
          { src: '/icon-192.png', sizes: '192x192', type: 'image/png' },
          { src: '/icon-512.png', sizes: '512x512', type: 'image/png' },
        ],
      },
    }),
  ],
})
```

### 2. Switch to y-webrtc

```bash
npm install y-webrtc
npm uninstall y-websocket
```

Change in `yjs-sync.ts`:
```ts
// Before:
import { WebsocketProvider } from 'y-websocket'
wsProvider = new WebsocketProvider(WS_URL, ROOM, ydoc)

// After:
import { WebrtcProvider } from 'y-webrtc'
const webrtcProvider = new WebrtcProvider(ROOM, ydoc, {
  signaling: ['wss://signaling.yjs.dev'],
})
```

### 3. Deploy to Vercel

```bash
npm run build   # outputs to dist/
# Push to GitHub → Vercel auto-deploys
```

Or GitHub Pages:
```bash
npm run build
# Copy dist/ to gh-pages branch
```

### 4. Clean up

- Delete `server/` directory (no longer needed)
- Delete `docker-compose.yml`
- Remove `VITE_SYNC_URL` / `VITE_SYNC_TOKEN` env vars
- Update `docs/setup.md`

## Recommended Setup

### Target Environment

```
Ubuntu host (always online, high availability)
    │  Chromium headless, page always open
    │  IndexedDB holds complete data — acts as "personal sync hub"
    │
    ├── iPhone  → open PWA → P2P sync → edit → sync back → close ✅
    ├── Server  → may reboot for security updates → reconnects to Ubuntu ✅
    ├── Any new device → open URL → syncs full data from Ubuntu ✅
```

### Ubuntu Sync Node (systemd)

Create `/etc/systemd/system/keep-sync.service`:

```ini
[Unit]
Description=Keep Notes Sync Node
After=network.target

[Service]
Type=simple
User=your-username
ExecStart=/usr/bin/node -e "\
  const { chromium } = require('playwright');\
  (async () => {\
    const browser = await chromium.launch({ headless: true });\
    const page = await browser.newPage();\
    await page.goto('https://your-app.vercel.app/#room=your-secret-room');\
    console.log('Sync node running');\
    await new Promise(() => {});\
  })();"
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
```

```bash
# Install playwright on Ubuntu
npm install -g playwright
npx playwright install chromium --with-deps

# Enable and start
sudo systemctl enable keep-sync
sudo systemctl start keep-sync

# Check status
sudo systemctl status keep-sync
```

This ensures the sync node:
- Starts on boot
- Restarts automatically if it crashes
- Keeps IndexedDB data persistent across restarts

## Room Security

The room name determines who can sync. Options:
- **Random UUID room**: Generate on first use, share via URL/QR code (e.g., `https://app.com/#room=abc-123`)
- **Password-derived room**: Hash a user-chosen password to create the room name
- **No auth needed**: The signaling server only helps devices find each other; data is P2P encrypted

## Comparison

| | Current (y-websocket) | Proposed (y-webrtc PWA) |
|---|---|---|
| Server | Self-hosted Node.js | None (public signaling only) |
| Cost | VPS/container hosting | Free (Vercel + yjs.dev) |
| Offline sync | ✅ Server holds state | ⚠️ Need one device always on |
| Privacy | Data passes through server | P2P encrypted, no middleman |
| iOS support | Separate native app | PWA (same codebase) |
| Hot update | Redeploy server + refresh | Push code → auto-update everywhere |
| Maintenance | Server uptime, updates | Zero |

## Scalability & Data Limits

### Capacity estimate

```
Active notes (not archived):  ~hundreds to thousands  →  ~few MB
Archived notes:               ~100K over time         →  ~100MB
Total in IndexedDB:           well within 1-2GB browser limit
```

### Sync behavior

- **First sync on new device**: Full Y.Doc transfer (~100MB) takes ~30-60 seconds over WebRTC P2P
- **Subsequent syncs**: Incremental only — Yjs transmits just the delta (typically a few KB), near-instant
- **Local performance**: All data in IndexedDB, search is local (no network), rendering uses virtual scroll for large lists

### Limitation: no on-demand / partial sync

Yjs syncs the entire Y.Doc as a unit. There is no "sync only recent notes" or "lazy-load archived notes" capability. All 100K notes sync to every device.

This is acceptable because:
1. Archived notes are small (text only, ~1KB each)
2. First sync is a one-time cost per device
3. Daily usage only transfers incremental changes

If on-demand loading becomes necessary in the future, it would require replacing Yjs P2P with a traditional backend API — a fundamentally different architecture.

### Images (future)

Images should NOT go through Yjs. Planned approach:
- Store images in object storage (Cloudflare R2 free 10GB, or S3)
- Notes store only the URL reference
- Images load on-demand from the URL

## Disaster Recovery

### iPhone as backup node

Open the PWA on iPhone → syncs full data from Ubuntu → close. iPhone IndexedDB now holds a complete backup. If Ubuntu dies, open iPhone → sync to a new node.

### Automated backup (Ubuntu)

```bash
# cron: daily export at 3am
0 3 * * * /home/user/keep-backup.sh
```

Backup script uses Playwright to open the page, call `window.__exportData()`, save JSON to local disk + push to remote storage (S3 / Google Drive / git repo).

### Recovery priority

```
1. Ubuntu IndexedDB (primary, always online)
2. iPhone IndexedDB (manual backup, open app to sync)
3. Server IndexedDB (may reboot, reconnects to Ubuntu)
4. Exported JSON files (cold backup, daily)
```

## Status

**Not yet implemented.** Current architecture uses y-websocket with self-hosted server. This plan is approved for future execution.
