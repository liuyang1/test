import * as Y from 'yjs'
import { createClient } from '@liveblocks/client'
import { getYjsProviderForRoom, LiveblocksYjsProvider } from '@liveblocks/yjs'
import { IndexeddbPersistence } from 'y-indexeddb'
import { Note, ChecklistItem } from '../types/note'
import { v4 as uuidv4 } from 'uuid'

const ROOM = import.meta.env.VITE_DB_NAME || 'keep-notes'
const LIVEBLOCKS_KEY = import.meta.env.VITE_LIVEBLOCKS_KEY || ''
const USE_LIVEBLOCKS = !!LIVEBLOCKS_KEY && !window.location.search.includes('nosync')

// ─── Y.Doc setup ───
// When using Liveblocks, we let it own the Y.Doc (created after room connect).
// When offline/test mode, we use a local Y.Doc + IndexedDB.
let ydoc: Y.Doc = new Y.Doc()
let yNotes: Y.Map<Y.Map<any>> = ydoc.getMap<Y.Map<any>>('notes')
let yLabels: Y.Array<string> = ydoc.getArray<string>('labels')

let indexeddbProvider: IndexeddbPersistence | null = null
let lbRoom: any = null
let lbLeave: (() => void) | null = null
let lbProvider: LiveblocksYjsProvider | null = null
let syncStatusCallback: ((status: string) => void) | null = null
let docReady = false
let docReadyCallbacks: (() => void)[] = []

// For non-Liveblocks mode, use IndexedDB
if (!USE_LIVEBLOCKS) {
  indexeddbProvider = new IndexeddbPersistence(ROOM, ydoc)
  docReady = true
}

export function onSyncStatus(cb: (status: string) => void) { syncStatusCallback = cb }

export function connectSync() {
  if (!USE_LIVEBLOCKS) return
  if (lbRoom) return

  console.log(`[sync] connecting to Liveblocks, room=${ROOM}`)
  syncStatusCallback?.('connecting')

  try {
    const client = createClient({ publicApiKey: LIVEBLOCKS_KEY })
    const result = client.enterRoom(ROOM)
    lbRoom = result.room
    lbLeave = result.leave

    lbProvider = getYjsProviderForRoom(lbRoom, {
      offlineSupport_experimental: true,
    }) as LiveblocksYjsProvider

    // Use Liveblocks' Y.Doc as the single source of truth
    const lbDoc = lbProvider.getYDoc()
    ydoc = lbDoc
    yNotes = lbDoc.getMap<Y.Map<any>>('notes')
    yLabels = lbDoc.getArray<string>('labels')

    lbProvider.on('sync', (synced: boolean) => {
      console.log(`[sync] ${synced ? 'connected' : 'disconnected'}`)
      syncStatusCallback?.(synced ? 'connected' : 'disconnected')
      if (synced && !docReady) {
        docReady = true
        docReadyCallbacks.forEach(cb => cb())
        docReadyCallbacks = []
      }
    })
  } catch (e) {
    console.warn('[sync] Failed to connect:', e)
    syncStatusCallback?.('error')
  }
}

export function disconnectSync() {
  lbLeave?.()
  lbRoom = null
  lbLeave = null
  lbProvider = null
}

// ─── Data access (always uses current ydoc/yNotes/yLabels) ───

function yMapToNote(id: string, yNote: Y.Map<any>): Note {
  return {
    id,
    title: yNote.get('title') || '',
    content: yNote.get('content') || '',
    type: yNote.get('type') || 'text',
    checklist: yNote.get('checklist')?.toJSON() || [],
    color: yNote.get('color') || '#ffffff',
    background: yNote.get('background') || '',
    pinned: yNote.get('pinned') || false,
    archived: yNote.get('archived') || false,
    deleted: yNote.get('deleted') || false,
    deletedAt: yNote.get('deletedAt') || null,
    labels: yNote.get('labels')?.toJSON() || [],
    sortOrder: yNote.get('sortOrder') || 0,
    createdAt: yNote.get('createdAt') || Date.now(),
    updatedAt: yNote.get('updatedAt') || Date.now(),
  }
}

export function getAllNotes(): Note[] {
  const notes: Note[] = []
  yNotes.forEach((yNote, id) => notes.push(yMapToNote(id, yNote)))
  return notes
}

export function upsertNote(note: Note) {
  ydoc.transact(() => {
    let yNote = yNotes.get(note.id)
    if (!yNote) {
      yNote = new Y.Map()
      yNotes.set(note.id, yNote)
    }
    yNote.set('title', note.title)
    yNote.set('content', note.content)
    yNote.set('type', note.type)
    const yChecklist = new Y.Array()
    yChecklist.push(note.checklist.map(item => {
      const m = new Y.Map()
      m.set('id', item.id)
      m.set('text', item.text)
      m.set('checked', item.checked)
      m.set('sortOrder', item.sortOrder)
      return m
    }))
    yNote.set('checklist', yChecklist)
    const yLabelsArr = new Y.Array<string>()
    yLabelsArr.push(note.labels)
    yNote.set('labels', yLabelsArr)
    yNote.set('color', note.color)
    yNote.set('background', note.background)
    yNote.set('pinned', note.pinned)
    yNote.set('archived', note.archived)
    yNote.set('deleted', note.deleted)
    yNote.set('deletedAt', note.deletedAt)
    yNote.set('sortOrder', note.sortOrder)
    yNote.set('createdAt', note.createdAt)
    yNote.set('updatedAt', note.updatedAt)
  })
}

export function deleteNotePermanently(id: string) {
  yNotes.delete(id)
}

export function onNotesChange(callback: (notes: Note[]) => void) {
  const handler = () => callback(getAllNotes())
  yNotes.observeDeep(handler)
  return () => yNotes.unobserveDeep(handler)
}

export function getAllLabels(): string[] {
  return yLabels.toJSON()
}

export function addLabel(label: string) {
  const existing = yLabels.toJSON()
  if (!existing.includes(label)) yLabels.push([label])
}

export function removeLabel(label: string) {
  const arr = yLabels.toJSON()
  const idx = arr.indexOf(label)
  if (idx >= 0) yLabels.delete(idx, 1)
}

export function renameLabel(oldName: string, newName: string) {
  const arr = yLabels.toJSON()
  const idx = arr.indexOf(oldName)
  if (idx >= 0) {
    yLabels.delete(idx, 1)
    yLabels.insert(idx, [newName])
  }
  yNotes.forEach((yNote) => {
    const labels: string[] = yNote.get('labels')?.toJSON() || []
    const li = labels.indexOf(oldName)
    if (li >= 0) {
      labels[li] = newName
      const yArr = new Y.Array<string>()
      yArr.push(labels)
      yNote.set('labels', yArr)
    }
  })
}

export function onLabelsChange(callback: (labels: string[]) => void) {
  const handler = () => callback(getAllLabels())
  yLabels.observe(handler)
  return () => yLabels.unobserve(handler)
}

export function waitForSync(): Promise<void> {
  if (docReady) return Promise.resolve()
  return new Promise(resolve => {
    // Timeout fallback — don't block the app forever
    const timer = setTimeout(() => { docReady = true; resolve() }, 5000)
    const done = () => { clearTimeout(timer); docReady = true; resolve() }
    if (!USE_LIVEBLOCKS) {
      if (indexeddbProvider?.synced) return done()
      indexeddbProvider?.on('synced', done)
    } else {
      docReadyCallbacks.push(done)
    }
  })
}

export { createNoteData, createChecklistItemData, parseHashTags } from './note-utils'

export function clearAllData() {
  ydoc.transact(() => {
    yNotes.forEach((_, key) => yNotes.delete(key))
    while (yLabels.length > 0) yLabels.delete(0, 1)
  })
}
