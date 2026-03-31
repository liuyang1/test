import { useState, useEffect, useCallback } from 'react'
import { Note, NoteView, TRASH_RETENTION_DAYS } from '../types/note'
import {
  getAllNotes, upsertNote, deleteNotePermanently, onNotesChange,
  connectSync, waitForSync, createNoteData,
  getAllLabels, addLabel, removeLabel, renameLabel, onLabelsChange,
  onSyncStatus,
} from '../sync/yjs-sync'

export function useNotes() {
  const [notes, setNotes] = useState<Note[]>([])
  const [labels, setLabels] = useState<string[]>([])
  const [loading, setLoading] = useState(true)
  const [syncStatus, setSyncStatus] = useState('disconnected')

  useEffect(() => {
    let unsubNotes: (() => void) | undefined
    let unsubLabels: (() => void) | undefined

    waitForSync().then(() => {
      setNotes(getAllNotes())
      setLabels(getAllLabels())
      setLoading(false)
      connectSync()

      const cutoff = Date.now() - TRASH_RETENTION_DAYS * 24 * 60 * 60 * 1000
      for (const n of getAllNotes()) {
        if (n.deleted && n.deletedAt && n.deletedAt < cutoff) deleteNotePermanently(n.id)
      }

      unsubNotes = onNotesChange(setNotes)
      unsubLabels = onLabelsChange(setLabels)
      onSyncStatus(setSyncStatus)
    })

    return () => { unsubNotes?.(); unsubLabels?.() }
  }, [])

  const save = useCallback((note: Note) => {
    const updated = { ...note, updatedAt: Date.now() }
    upsertNote(updated)
    return updated
  }, [])

  const add = useCallback((partial?: Partial<Note>) => {
    const note = createNoteData(partial)
    upsertNote(note)
    return note
  }, [])

  const remove = useCallback((id: string) => {
    const note = notes.find(n => n.id === id)
    if (!note) return
    if (note.deleted) {
      deleteNotePermanently(id)
    } else {
      upsertNote({ ...note, deleted: true, deletedAt: Date.now(), updatedAt: Date.now() })
    }
  }, [notes])

  const restore = useCallback((id: string) => {
    const note = notes.find(n => n.id === id)
    if (!note) return
    upsertNote({ ...note, deleted: false, deletedAt: null, updatedAt: Date.now() })
  }, [notes])

  const emptyTrash = useCallback(() => {
    for (const n of notes) {
      if (n.deleted) deleteNotePermanently(n.id)
    }
  }, [notes])

  const getFiltered = useCallback((view: NoteView, search: string, labelFilter?: string) => {
    let filtered = notes
    if (view === 'notes') filtered = filtered.filter(n => !n.archived && !n.deleted)
    else if (view === 'archive') filtered = filtered.filter(n => n.archived && !n.deleted)
    else if (view === 'trash') filtered = filtered.filter(n => n.deleted)
    else if (view === 'label') filtered = filtered.filter(n => !n.deleted && labelFilter && n.labels.includes(labelFilter))

    if (search.trim()) {
      const q = search.toLowerCase()
      filtered = filtered.filter(n =>
        n.title.toLowerCase().includes(q) ||
        n.content.toLowerCase().includes(q) ||
        n.checklist.some(item => item.text.replace(/<[^>]*>/g, '').toLowerCase().includes(q)) ||
        n.labels.some(l => l.toLowerCase().includes(q))
      )
    }

    if (view !== 'trash') {
      const pinned = filtered.filter(n => n.pinned).sort((a, b) => b.sortOrder - a.sortOrder)
      const unpinned = filtered.filter(n => !n.pinned).sort((a, b) => b.sortOrder - a.sortOrder)
      return { pinned, unpinned }
    }
    return { pinned: [], unpinned: filtered.sort((a, b) => (b.deletedAt ?? 0) - (a.deletedAt ?? 0)) }
  }, [notes])

  return {
    notes, labels, loading, syncStatus, save, add, remove, restore, emptyTrash, getFiltered,
    addLabel, removeLabel, renameLabel,
  }
}
