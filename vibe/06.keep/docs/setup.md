# Setup

## Prerequisites

- Node.js >= 18 (recommended: 20 LTS)

## Running

### Start the sync server

```bash
cd server
npm install
npm start
```

### Start the web frontend

```bash
cd web
npm install
npm run dev
```

### Or use Docker Compose (server only)

```bash
docker compose up --build
```

## Features

### Hash tag labels

Type `#` followed by text in the title field to get an autocomplete dropdown of existing labels. Select one to instantly tag the note. If the label doesn't exist yet, choose "Create" from the dropdown to create it on the fly. The `#text` is removed from the title and the label appears as a chip below the note content.

- Works in both the new-note input and the note editor
- Arrow keys to navigate suggestions, Enter/Tab to select, Esc to dismiss
- Existing `#tag` text in title/content is also parsed on save as a fallback

### Multi-select

Click the checkbox that appears on the top-left corner of a note card (on hover) to select it. Once a note is selected, a selection bar appears at the top with bulk actions:

- Pin / Unpin
- Archive
- Change color
- Delete

Click additional note cards to add them to the selection. Press `Esc` or click the `✕` button in the selection bar to clear.

## Known Issues Fixed

### Empty HTML content leak (`<p></p>` bug)

The TipTap rich text editor represents empty content as `<p></p>`. Previously this leaked into saved notes in two ways:
- Text notes with no body content were saved with `content: '<p></p>'` and rendered as empty cards
- Switching from text mode to checklist mode converted `<p></p>` into a checklist item with literal `<p></p>` text

Fixed by adding `isEmptyHtml()` and `stripHtml()` utilities that properly detect and strip empty HTML. Applied in:
- NoteInput submit (hasContent check)
- NoteInput text→checklist toggle
- NoteEditor text→checklist toggle
- NoteCard content rendering

## Testing

Tests run on a separate Vite instance (port 5138) with an isolated IndexedDB (`keep-notes-test`), so your development data is never affected.

```bash
cd web
npx playwright test --config=playwright.config.cjs
```
