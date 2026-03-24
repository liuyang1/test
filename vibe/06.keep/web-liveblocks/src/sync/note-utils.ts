import { Note, ChecklistItem } from '../types/note'
import { v4 as uuidv4 } from 'uuid'

export function createNoteData(partial?: Partial<Note>): Note {
  const now = Date.now()
  return {
    id: uuidv4(),
    title: '',
    content: '',
    type: 'text',
    checklist: [],
    color: '#ffffff',
    background: '',
    pinned: false,
    archived: false,
    deleted: false,
    deletedAt: null,
    labels: [],
    sortOrder: now,
    createdAt: now,
    updatedAt: now,
    ...partial,
  }
}

export function createChecklistItemData(text = ''): ChecklistItem {
  return { id: uuidv4(), text, checked: false, sortOrder: Date.now() }
}

/** Parse #tags from text, return cleaned text and extracted tags */
export function parseHashTags(text: string): { clean: string; tags: string[] } {
  const tags: string[] = []
  const clean = text.replace(/#(\S+)/g, (_, tag) => { tags.push(tag); return '' }).trim()
  return { clean, tags }
}

/** Check if HTML content is effectively empty (e.g. '<p></p>', '<p><br></p>') */
export function isEmptyHtml(html: string): boolean {
  return !html || !html.replace(/<[^>]*>/g, '').replace(/\s|&nbsp;/g, '')
}

/** Strip HTML tags and return plain text */
export function stripHtml(html: string): string {
  return html.replace(/<[^>]+>/g, '\n').replace(/\n+/g, '\n').trim()
}
