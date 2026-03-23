import * as Y from 'yjs'
import { WebrtcProvider } from 'y-webrtc'
import { IndexeddbPersistence } from 'y-indexeddb'
import { Note, ChecklistItem } from '../types/note'
import { v4 as uuidv4 } from 'uuid'

const ROOM = import.meta.env.VITE_DB_NAME || 'keep-notes'
const SIGNALING = (import.meta.env.VITE_SIGNALING_URLS || 'wss://signaling.yjs.dev').split(',')

const ydoc = new Y.Doc()
const yNotes = ydoc.getMap<Y.Map<any>>('notes')
const yLabels = ydoc.getArray<string>('labels')

const indexeddbProvider = new IndexeddbPersistence(ROOM, ydoc)

let rtcProvider: WebrtcProvider | null = null
let syncStatusCallback: ((status: string) => void) | null = null

export function onSyncStatus(cb: (status: string) => void) { syncStatusCallback = cb }

export function connectSync() {
  if (rtcProvider) return
  // Skip sync in test mode
  if (window.location.search.includes('nosync')) return
  console.log(`[sync] connecting via WebRTC, room=${ROOM}, signaling=${SIGNALING.join(',')}`)
  try {
    rtcProvider = new WebrtcProvider(ROOM, ydoc, { signaling: SIGNALING })
    rtcProvider.on('synced', ({ synced }: { synced: boolean }) => {
      console.log(`[sync] ${synced ? 'connected' : 'disconnected'}`)
      syncStatusCallback?.(synced ? 'connected' : 'disconnected')
    })
    syncStatusCallback?.('connecting')
  } catch (e) {
    console.warn('[sync] Failed to connect:', e)
    syncStatusCallback?.('error')
  }
}

export function disconnectSync() {
  rtcProvider?.destroy()
  rtcProvider = null
}

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

// Labels management
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
  // Update all notes with this label
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
  return new Promise(resolve => {
    if (indexeddbProvider.synced) return resolve()
    indexeddbProvider.on('synced', () => resolve())
  })
}

// Re-export pure utility functions
export { createNoteData, createChecklistItemData, parseHashTags } from './note-utils'

// Test helper: clear all data
export function clearAllData() {
  ydoc.transact(() => {
    yNotes.forEach((_, key) => yNotes.delete(key))
    while (yLabels.length > 0) yLabels.delete(0, 1)
  })
}
