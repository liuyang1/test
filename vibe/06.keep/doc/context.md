# Keep Clone — Project Context & Continuation Guide

This document contains everything needed to continue development on any machine.

## Project Overview

A Google Keep clone with P2P sync (VerySync-inspired). Single user, no login.
Sync architecture: data lives only on devices, server is a dumb WebSocket relay.

## Current Status (as of 2026-03-23)

### ✅ Web Client — Feature Complete
- React 18 + TypeScript + Vite 5 + Tailwind CSS 3
- Yjs CRDT sync engine + y-indexeddb + y-websocket
- Tiptap rich text editor (bold, italic, underline, strikethrough, H1, H2, bullet/ordered lists)
- 84 tests passing (41 Vitest unit + 43 Playwright e2e)

### ✅ Sync Server — Complete
- ~80 lines Node.js, Yjs sync protocol over WebSocket
- Docker deployment ready (Dockerfile + docker-compose.yml)
- Listens on 0.0.0.0:4444

### ⏳ iOS Client — Not Started (needs macOS + Xcode)

## How to Run

```bash
# Web app (port 5137)
cd web && npm install && npm run dev

# Sync server (port 4444) — needed for cross-browser/device sync
cd server && npm install && npm start

# Unit tests
cd web && npm test

# E2E tests (needs Chromium installed: npx playwright install chromium)
cd web && npm run test:e2e
```

## Tech Stack

| Layer | Technology | Why |
|-------|-----------|-----|
| UI Framework | React 18 + TypeScript | Standard, good ecosystem |
| Build | Vite 5 | Fast, works with Node 18 |
| Styling | Tailwind CSS 3 | Utility-first, rapid iteration |
| Icons | Material Symbols Outlined (font) | Pixel-identical to Google Keep |
| Rich Text | Tiptap (ProseMirror) | Best React CRDT-compatible editor |
| CRDT Sync | Yjs + y-websocket + y-indexeddb | Mature, handles conflicts automatically |
| Local Storage | IndexedDB (via y-indexeddb) | Offline-first, persists across sessions |
| Drag & Drop | @dnd-kit | Best React DnD library |
| Unit Tests | Vitest 1.x | Fast, Vite-native |
| E2E Tests | Playwright 1.40 | Cross-browser, headless |
| Server | Node.js + ws + yjs | Minimal relay, no data storage |

### Node.js Constraint
Current dev machine has GLIBC 2.26, so **only Node 18.18.x works** (Node 20+ needs GLIBC 2.28). All dependencies are pinned to Node 18-compatible versions:
- Vite 5 (not 6+)
- Vitest 1 (not 2+)
- Playwright 1.40 (not 1.41+)
- Tailwind 3 (not 4)

On macOS this constraint doesn't apply — you can use Node 20/22.

## Architecture

```
┌─────────────────┐     ┌─────────────────┐
│   Web (React)   │     │  iOS (SwiftUI)  │
│                 │     │                 │
│  ┌───────────┐  │     │  ┌───────────┐  │
│  │  Yjs Doc  │  │     │  │  Yjs Doc  │  │
│  └─────┬─────┘  │     │  └─────┬─────┘  │
│        │        │     │        │        │
│  ┌─────┴─────┐  │     │  ┌─────┴─────┐  │
│  │ IndexedDB │  │     │  │ SwiftData │  │
│  └─────┬─────┘  │     │  └─────┬─────┘  │
└────────┼────────┘     └────────┼────────┘
         │    WebSocket          │
         └──────┬────────────────┘
                │
       ┌────────┴────────┐
       │  Sync Server    │
       │  ws://host:4444 │
       └─────────────────┘
```

### Data Model (Note)
```typescript
{
  id: string           // UUID v4
  title: string
  content: string      // HTML (Tiptap output)
  type: 'text' | 'checklist'
  checklist: ChecklistItem[]  // { id, text, checked, sortOrder }
  color: string        // hex, e.g. '#faafa8'
  background: string   // pattern name: 'groceries', 'food', etc.
  pinned: boolean
  archived: boolean
  deleted: boolean
  deletedAt: number | null
  labels: string[]
  sortOrder: number    // float, for drag reorder
  createdAt: number
  updatedAt: number
}
```

### Sync Protocol
- Single Yjs Doc with `Y.Map<Y.Map>` for notes, `Y.Array<string>` for labels
- WebSocket URL auto-detected: `ws://${window.location.hostname}:4444`
- `?nosync` query param disables WebSocket (used in tests)
- `window.__clearKeepData()` exposed for test cleanup
- Conflict resolution: Yjs CRDT (LWW for scalar fields, merge for arrays)

## File Structure

```
├── doc/
│   ├── requirements.md      # Full requirements with completion status
│   ├── architecture.md       # Technical architecture design
│   └── context.md            # THIS FILE
├── web/
│   ├── src/
│   │   ├── App.tsx           # Main app, keyboard shortcuts, layout
│   │   ├── main.tsx          # Entry point, exposes __clearKeepData
│   │   ├── index.css         # Global styles, animations, tiptap styles
│   │   ├── types/note.ts     # Note interface, colors, settings
│   │   ├── sync/
│   │   │   ├── yjs-sync.ts   # Yjs doc, WebSocket provider, CRUD ops
│   │   │   ├── note-utils.ts # Pure functions (createNote, parseHashTags)
│   │   │   └── db.ts         # Legacy IndexedDB (unused, kept for reference)
│   │   ├── hooks/
│   │   │   ├── useNotes.ts   # Main state hook, wraps yjs-sync
│   │   │   └── useSettings.ts # Settings context
│   │   ├── components/
│   │   │   ├── Toolbar.tsx    # Top bar + NoteInput + Sidebar + MobileSidebar
│   │   │   ├── NoteCard.tsx   # Card with hover actions, pin, color picker
│   │   │   ├── NoteEditor.tsx # Modal editor with rich text + format bar
│   │   │   ├── NoteGrid.tsx   # Masonry layout + DnD + DragOverlay
│   │   │   ├── Checklist.tsx  # Checklist with keyboard nav, preview mode
│   │   │   ├── RichEditor.tsx # Tiptap wrapper + FormatBar component
│   │   │   ├── ColorPicker.tsx# Colors + background patterns + getNoteBackground()
│   │   │   ├── LabelPicker.tsx# Label search/create/toggle
│   │   │   ├── EditLabelsDialog.tsx # Create/rename/delete labels
│   │   │   ├── LinkPreview.tsx# URL extraction, Linkified text, favicon preview
│   │   │   └── Icons.tsx      # Material Symbols font wrapper
│   │   └── __tests__/         # Vitest unit tests
│   ├── e2e/app.spec.mjs      # Playwright e2e tests (43 tests)
│   ├── playwright.config.cjs  # Playwright config (4 workers, 1 retry)
│   ├── vite.config.ts         # Vite + Tailwind + test config
│   └── index.html             # Material Symbols font, viewport meta
├── server/
│   ├── index.js               # WebSocket relay (~80 lines)
│   ├── Dockerfile
│   └── package.json
├── docker-compose.yml
├── .gitignore
└── README.md
```

## Key Design Decisions

1. **Masonry layout via JS absolute positioning** (not CSS columns/grid)
   - CSS columns: no animation support
   - CSS grid: equal row heights, wastes space
   - JS masonry: absolute position each card in shortest column, CSS transition on left/top for smooth reflow

2. **DragOverlay for drag-and-drop** (not transform on original)
   - Original card hidden during drag (opacity: 0)
   - Floating overlay copy shown with shadow + rotation
   - `dropAnimation: null` to prevent flash-back-then-animate bug
   - `CSS.Translate` (not `CSS.Transform`) to prevent stretch/scale

3. **Tiptap for rich text** (not textarea)
   - Content stored as HTML string
   - FormatBar is a separate row from tool buttons (Google Keep style)
   - Supports Ctrl+B/I/U keyboard shortcuts natively

4. **Settings via React Context + localStorage**
   - `moveCheckedToBottom`, `newItemPosition`, `layout` (grid/list)
   - Persisted to localStorage, read on mount

5. **Labels stored in Yjs Y.Array** (separate from notes)
   - Notes reference labels by string name
   - Rename propagates to all notes in a Yjs transaction

## Known Issues & Remaining Work

### Bugs to Fix
- Masonry layout doesn't perfectly match Google Keep's (cards sometimes have slight gaps)
- Drag reflow animation could be smoother (masonry recalc is synchronous)
- Some e2e tests are flaky when sync server is running (use `?nosync` to isolate)

### Features Not Yet Implemented
- **iOS native client** (Phase 3 — needs macOS + Xcode)
  - SwiftUI + SwiftData + y-swift
  - Connect to same sync server
- **Server deployment** to Hong Kong VPS (for China mainland access)
- **Reminders** (not in scope)
- **Image attachments** (not in scope)
- **Collaboration** (not in scope)

### UI Polish Remaining
- Card hover action bar animation (fade in/out timing)
- Sidebar hover expand/collapse animation smoothness
- Mobile touch gestures for drag reorder
- Dark mode (Google Keep supports it)

## iOS Development Plan

### Tech Stack
- SwiftUI + SwiftData (iOS 17+)
- y-swift (Yjs Swift port) for CRDT sync
- URLSessionWebSocketTask for WebSocket
- LazyVGrid for masonry layout

### Key Files to Create
```
ios/KeepClone/
├── KeepCloneApp.swift
├── Models/
│   ├── Note.swift              # SwiftData @Model
│   └── SyncManager.swift       # y-swift + WebSocket
├── Views/
│   ├── NoteGridView.swift      # Main grid
│   ├── NoteCardView.swift      # Card component
│   ├── NoteEditorView.swift    # Editor sheet
│   ├── ChecklistView.swift
│   ├── ColorPickerView.swift
│   └── SearchView.swift
└── Persistence/
    └── LocalStore.swift
```

### Sync Compatibility
iOS client must connect to the same Yjs room (`keep-notes`) on the same WebSocket server. The Yjs binary protocol is cross-platform — y-swift and y-js produce identical CRDT updates.

## Commands Reference

```bash
# Development
cd web && npm run dev          # Start web app on :5137
cd server && npm start         # Start sync server on :4444

# Testing
cd web && npm test             # Vitest unit tests (41)
cd web && npm run test:e2e     # Playwright e2e tests (43)

# Building
cd web && npx tsc --noEmit     # Type check
cd web && npx vite build       # Production build

# Docker (sync server)
docker-compose up -d           # Start sync server in Docker

# Playwright setup (first time)
cd web && npx playwright install chromium
```

## Environment Notes

- **Linux dev machine**: Node 18.18.2 (GLIBC 2.26 constraint), no Xcode
- **macOS**: Use Node 20+ (no GLIBC issue), install Xcode for iOS dev
- **Sync server**: Must be accessible from all devices (use LAN IP or VPS)
- **China mainland**: Deploy sync server to Hong Kong VPS for reliable access
