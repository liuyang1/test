import { describe, it, expect } from 'vitest'
import { createNoteData, createChecklistItemData, parseHashTags } from '../sync/note-utils'
import { Note } from '../types/note'

describe('createNoteData', () => {
  it('creates with defaults', () => {
    const n = createNoteData()
    expect(n.id).toBeTruthy()
    expect(n.title).toBe('')
    expect(n.content).toBe('')
    expect(n.type).toBe('text')
    expect(n.checklist).toEqual([])
    expect(n.color).toBe('#ffffff')
    expect(n.background).toBe('')
    expect(n.pinned).toBe(false)
    expect(n.archived).toBe(false)
    expect(n.deleted).toBe(false)
    expect(n.deletedAt).toBeNull()
    expect(n.labels).toEqual([])
    expect(n.sortOrder).toBeGreaterThan(0)
    expect(n.createdAt).toBeGreaterThan(0)
    expect(n.updatedAt).toBe(n.createdAt)
  })

  it('applies overrides', () => {
    const n = createNoteData({ title: 'T', pinned: true, color: '#faafa8', labels: ['a'], background: 'food' })
    expect(n.title).toBe('T')
    expect(n.pinned).toBe(true)
    expect(n.color).toBe('#faafa8')
    expect(n.labels).toEqual(['a'])
    expect(n.background).toBe('food')
  })

  it('unique ids', () => {
    expect(createNoteData().id).not.toBe(createNoteData().id)
  })

  it('checklist type', () => {
    const n = createNoteData({ type: 'checklist', checklist: [createChecklistItemData('x')] })
    expect(n.type).toBe('checklist')
    expect(n.checklist.length).toBe(1)
    expect(n.checklist[0].text).toBe('x')
  })

  it('deleted note has deletedAt', () => {
    const n = createNoteData({ deleted: true, deletedAt: 12345 })
    expect(n.deleted).toBe(true)
    expect(n.deletedAt).toBe(12345)
  })

  it('archived note', () => {
    const n = createNoteData({ archived: true })
    expect(n.archived).toBe(true)
  })
})

describe('createChecklistItemData', () => {
  it('with text', () => {
    const i = createChecklistItemData('Buy milk')
    expect(i.id).toBeTruthy()
    expect(i.text).toBe('Buy milk')
    expect(i.checked).toBe(false)
    expect(i.sortOrder).toBeGreaterThan(0)
  })

  it('empty', () => {
    expect(createChecklistItemData().text).toBe('')
  })

  it('unique ids', () => {
    expect(createChecklistItemData().id).not.toBe(createChecklistItemData().id)
  })
})

describe('parseHashTags', () => {
  it('single tag', () => {
    const { clean, tags } = parseHashTags('hello #world')
    expect(clean).toBe('hello')
    expect(tags).toEqual(['world'])
  })

  it('multiple tags', () => {
    const { clean, tags } = parseHashTags('#work meeting #urgent')
    expect(clean).toBe('meeting')
    expect(tags).toEqual(['work', 'urgent'])
  })

  it('no tags', () => {
    const { clean, tags } = parseHashTags('no tags here')
    expect(clean).toBe('no tags here')
    expect(tags).toEqual([])
  })

  it('empty string', () => {
    const { clean, tags } = parseHashTags('')
    expect(clean).toBe('')
    expect(tags).toEqual([])
  })

  it('tag at end', () => {
    const { clean, tags } = parseHashTags('buy groceries #shopping')
    expect(clean).toBe('buy groceries')
    expect(tags).toEqual(['shopping'])
  })

  it('tag only', () => {
    const { clean, tags } = parseHashTags('#onlytag')
    expect(clean).toBe('')
    expect(tags).toEqual(['onlytag'])
  })

  it('multiple spaces after removal', () => {
    const { clean, tags } = parseHashTags('#a #b #c')
    expect(tags).toEqual(['a', 'b', 'c'])
    expect(clean).toBe('')
  })

  it('tag with numbers', () => {
    const { clean, tags } = parseHashTags('test #tag123')
    expect(tags).toEqual(['tag123'])
  })

  it('hash in url not treated as tag', () => {
    // # followed by space is not a tag
    const { tags } = parseHashTags('see # this')
    expect(tags).toEqual([])
  })
})
