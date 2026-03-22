export interface ChecklistItem {
  id: string
  text: string
  checked: boolean
  sortOrder: number
}

export interface Note {
  id: string
  title: string
  content: string
  type: 'text' | 'checklist'
  checklist: ChecklistItem[]
  color: string
  background: string
  pinned: boolean
  archived: boolean
  deleted: boolean
  deletedAt: number | null
  labels: string[]
  sortOrder: number
  createdAt: number
  updatedAt: number
}

export type NoteView = 'notes' | 'archive' | 'trash' | 'label'
export type LayoutView = 'grid' | 'list'

export interface Settings {
  newItemPosition: 'top' | 'bottom'
  moveCheckedToBottom: boolean
  layout: LayoutView
}

export const DEFAULT_SETTINGS: Settings = {
  newItemPosition: 'bottom',
  moveCheckedToBottom: true,
  layout: 'grid',
}

export const NOTE_COLORS = [
  { name: 'Default', value: '#ffffff' },
  { name: 'Coral', value: '#faafa8' },
  { name: 'Peach', value: '#f39f76' },
  { name: 'Sand', value: '#fff8b8' },
  { name: 'Mint', value: '#e2f6d3' },
  { name: 'Sage', value: '#b4ddd3' },
  { name: 'Fog', value: '#d3e4ec' },
  { name: 'Storm', value: '#aeccdc' },
  { name: 'Dusk', value: '#d3bfdb' },
  { name: 'Blossom', value: '#f6e2dd' },
  { name: 'Clay', value: '#e9e3d4' },
  { name: 'Chalk', value: '#efeff1' },
] as const

export const TRASH_RETENTION_DAYS = 30
