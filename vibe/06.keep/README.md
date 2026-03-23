# Keep Clone

A Google Keep-like personal note-taking app with P2P sync.

## Quick Start

### 1. Start sync server (optional, for multi-device sync)
```bash
cd server && npm start
```

### 2. Start web app
```bash
cd web && npm run dev
```

Open http://localhost:5173 in your browser.

The app works fully offline. The sync server is only needed for multi-device sync.

## Project Structure

```
├── doc/              # Requirements & architecture docs
├── web/              # React web client
├── server/           # Sync relay server (WebSocket)
└── ios/              # iOS native client (future)
```
