import { Note, ChecklistItem, TRASH_RETENTION_DAYS } from '../types/note'
import { v4 as uuidv4 } from 'uuid'

const DB_NAME = 'keep-clone'
const DB_VERSION = 1
const STORE_NAME = 'notes'

function openDB(): Promise<IDBDatabase> {
  return new Promise((resolve, reject) => {
    const req = indexedDB.open(DB_NAME, DB_VERSION)
    req.onupgradeneeded = () => {
      const db = req.result
      if (!db.objectStoreNames.contains(STORE_NAME)) {
        db.createObjectStore(STORE_NAME, { keyPath: 'id' })
      }
    }
    req.onsuccess = () => resolve(req.result)
    req.onerror = () => reject(req.error)
  })
}

async function getAllNotes(): Promise<Note[]> {
  const db = await openDB()
  return new Promise((resolve, reject) => {
    const tx = db.transaction(STORE_NAME, 'readonly')
    const store = tx.objectStore(STORE_NAME)
    const req = store.getAll()
    req.onsuccess = () => resolve(req.result)
    req.onerror = () => reject(req.error)
  })
}

async function putNote(note: Note): Promise<void> {
  const db = await openDB()
  return new Promise((resolve, reject) => {
    const tx = db.transaction(STORE_NAME, 'readwrite')
    const store = tx.objectStore(STORE_NAME)
    store.put(note)
    tx.oncomplete = () => resolve()
    tx.onerror = () => reject(tx.error)
  })
}

async function deleteNote(id: string): Promise<void> {
  const db = await openDB()
  return new Promise((resolve, reject) => {
    const tx = db.transaction(STORE_NAME, 'readwrite')
    const store = tx.objectStore(STORE_NAME)
    store.delete(id)
    tx.oncomplete = () => resolve()
    tx.onerror = () => reject(tx.error)
  })
}

export function createNote(partial?: Partial<Note>): Note {
  const now = Date.now()
  return {
    id: uuidv4(),
    title: '',
    content: '',
    type: 'text',
    checklist: [],
    color: '#ffffff',
    pinned: false,
    archived: false,
    deleted: false,
    deletedAt: null,
    sortOrder: now,
    createdAt: now,
    updatedAt: now,
    ...partial,
  }
}

export function createChecklistItem(text = ''): ChecklistItem {
  return { id: uuidv4(), text, checked: false, sortOrder: Date.now() }
}

export const db = {
  getAll: getAllNotes,
  put: putNote,
  delete: deleteNote,

  async cleanTrash(): Promise<string[]> {
    const notes = await getAllNotes()
    const cutoff = Date.now() - TRASH_RETENTION_DAYS * 24 * 60 * 60 * 1000
    const expired = notes.filter(n => n.deleted && n.deletedAt && n.deletedAt < cutoff)
    for (const n of expired) await deleteNote(n.id)
    return expired.map(n => n.id)
  },
}
