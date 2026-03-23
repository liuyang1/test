import { describe, it, expect } from 'vitest'
import { NOTE_COLORS, TRASH_RETENTION_DAYS, DEFAULT_SETTINGS } from '../types/note'

describe('NOTE_COLORS', () => {
  it('has 12 colors', () => expect(NOTE_COLORS.length).toBe(12))
  it('all valid hex', () => NOTE_COLORS.forEach(c => {
    expect(c.name).toBeTruthy()
    expect(c.value).toMatch(/^#[0-9a-f]{6}$/i)
  }))
  it('first is white', () => expect(NOTE_COLORS[0].value).toBe('#ffffff'))
  it('unique values', () => {
    const vals = NOTE_COLORS.map(c => c.value)
    expect(new Set(vals).size).toBe(vals.length)
  })
  it('unique names', () => {
    const names = NOTE_COLORS.map(c => c.name)
    expect(new Set(names).size).toBe(names.length)
  })
})

describe('TRASH_RETENTION_DAYS', () => {
  it('is 30', () => expect(TRASH_RETENTION_DAYS).toBe(30))
})

describe('DEFAULT_SETTINGS', () => {
  it('newItemPosition is bottom', () => expect(DEFAULT_SETTINGS.newItemPosition).toBe('bottom'))
  it('moveCheckedToBottom is true', () => expect(DEFAULT_SETTINGS.moveCheckedToBottom).toBe(true))
  it('layout is grid', () => expect(DEFAULT_SETTINGS.layout).toBe('grid'))
})
