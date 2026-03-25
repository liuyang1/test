import { useRef, useEffect, useState, forwardRef, useImperativeHandle, useCallback } from 'react'
import { ChecklistItem } from '../types/note'
import { createChecklistItemData as createChecklistItem } from '../sync/yjs-sync'
import { useSettings } from '../hooks/useSettings'

export interface ChecklistHandle { focusFirst: () => void }

interface Props {
  items: ChecklistItem[]
  onChange: (items: ChecklistItem[]) => void
  preview?: boolean
  onEscape?: () => void
  moveCheckedToBottom?: boolean
  onBackspaceAtStart?: () => void
}

/* Google Keep checkbox SVGs — 18×18 square style */
const UncheckedSvg = () => (
  <svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 24 24" fill="#000" style={{ opacity: 0.54 }}>
    <path d="M19 5v14H5V5h14m0-2H5c-1.1 0-2 .9-2 2v14c0 1.1.9 2 2 2h14c1.1 0 2-.9 2-2V5c0-1.1-.9-2-2-2z"/>
  </svg>
)
const CheckedSvg = () => (
  <svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 24 24" fill="#000" style={{ opacity: 0.54 }}>
    <path d="M19 3H5c-1.1 0-2 .9-2 2v14c0 1.1.9 2 2 2h14c1.1 0 2-.9 2-2V5c0-1.1-.9-2-2-2zm0 16H5V5h14v14z"/>
    <path d="M18 9l-1.4-1.4-6.6 6.6-2.6-2.6L6 13l4 4z"/>
  </svg>
)

export const Checklist = forwardRef<ChecklistHandle, Props>(({ items, onChange, preview, onEscape, moveCheckedToBottom: propMoveChecked, onBackspaceAtStart }, ref) => {
  const settings = useSettings()
  const moveCheckedToBottom = propMoveChecked ?? settings.moveCheckedToBottom
  const [focusId, setFocusId] = useState<string | null>(null)
  const [showChecked, setShowChecked] = useState(true)
  const firstInputRef = useRef<HTMLInputElement>(null)
  const [dragId, setDragId] = useState<string | null>(null)
  const [dragOverId, setDragOverId] = useState<string | null>(null)

  useImperativeHandle(ref, () => ({
    focusFirst: () => {
      const uc = items.filter(i => !i.checked)
      if (uc.length) setFocusId(uc[0].id)
    }
  }), [items])

  const update = (id: string, patch: Partial<ChecklistItem>) => onChange(items.map(i => i.id === id ? { ...i, ...patch } : i))
  const remove = (id: string) => {
    const uc = getSorted().unchecked
    const idx = uc.findIndex(i => i.id === id)
    if (idx > 0) setFocusId(uc[idx - 1].id)
    onChange(items.filter(i => i.id !== id))
  }
  const addItemBefore = (beforeId: string) => {
    const sorted = [...items].sort((a, b) => a.sortOrder - b.sortOrder)
    const idx = sorted.findIndex(i => i.id === beforeId)
    const curOrder = sorted[idx]?.sortOrder ?? Date.now()
    const prevOrder = idx > 0 ? sorted[idx - 1].sortOrder : curOrder - 2
    const n = createChecklistItem()
    n.sortOrder = (prevOrder + curOrder) / 2
    onChange([...items, n]); setFocusId(n.id)
  }
  const addItemAfter = (afterId: string) => {
    const sorted = [...items].sort((a, b) => a.sortOrder - b.sortOrder)
    const idx = sorted.findIndex(i => i.id === afterId)
    const curOrder = sorted[idx]?.sortOrder ?? Date.now()
    const nextOrder = sorted[idx + 1]?.sortOrder ?? curOrder + 2
    const n = createChecklistItem()
    n.sortOrder = (curOrder + nextOrder) / 2
    onChange([...items, n]); setFocusId(n.id)
  }
  const addItem = () => {
    const maxOrder = items.length ? Math.max(...items.map(i => i.sortOrder)) : 0
    const n = createChecklistItem()
    n.sortOrder = settings.newItemPosition === 'bottom' ? maxOrder + 1 : maxOrder + 1
    onChange([...items, n]); setFocusId(n.id)
  }

  const getSorted = () => {
    if (moveCheckedToBottom) {
      return {
        unchecked: items.filter(i => !i.checked).sort((a, b) => a.sortOrder - b.sortOrder),
        checked: items.filter(i => i.checked).sort((a, b) => a.sortOrder - b.sortOrder),
      }
    }
    return { unchecked: items.sort((a, b) => a.sortOrder - b.sortOrder), checked: [] as ChecklistItem[] }
  }

  const handleDrop = useCallback((targetId: string) => {
    if (!dragId || dragId === targetId) return
    const sorted = [...items].sort((a, b) => a.sortOrder - b.sortOrder)
    const fromIdx = sorted.findIndex(i => i.id === dragId)
    const toIdx = sorted.findIndex(i => i.id === targetId)
    if (fromIdx < 0 || toIdx < 0) return
    const moved = sorted.splice(fromIdx, 1)[0]
    sorted.splice(toIdx, 0, moved)
    onChange(sorted.map((it, i) => ({ ...it, sortOrder: i })))
    setDragId(null)
    setDragOverId(null)
  }, [dragId, items, onChange])

  const { unchecked, checked } = getSorted()

  // ─── Preview mode (on cards) — grid: 30px checkbox col + 1fr content, top-aligned ───
  if (preview) {
    return (
      <div>
        {unchecked.map(i => (
          <div key={i.id} className="grid min-h-[29px]" style={{ gridTemplateColumns: '30px 1fr' }}>
            <div className="w-[22px] pt-[3px]">
              <button onClick={e => { e.stopPropagation(); update(i.id, { checked: !i.checked }) }}
                className="checklist-check cursor-pointer outline-none" tabIndex={-1}>
                {(!moveCheckedToBottom ? i.checked : false) ? <CheckedSvg /> : <UncheckedSvg />}
              </button>
            </div>
            <div className={`text-[11pt] leading-[1.38] font-['Google_Sans_Text',Roboto,sans-serif] pt-[3px] ${i.checked ? 'line-through text-[#80868b]' : 'text-[#3c4043]'}`}>
              {i.text || '\u00A0'}
            </div>
          </div>
        ))}
        {checked.length > 0 && <div className="text-[12px] text-[#80868b] pl-[30px] mt-1">+{checked.length} checked</div>}
      </div>
    )
  }

  // ─── Edit mode — drag handle + checkbox + content ───
  const handleKeyDown = (e: React.KeyboardEvent, itemId: string, idx: number) => {
    if (e.key === 'Enter') {
      e.preventDefault()
      const input = e.target as HTMLInputElement
      if (input.selectionStart === 0 && input.value.length > 0) {
        addItemBefore(itemId)
      } else {
        addItemAfter(itemId)
      }
    }
    else if (e.key === 'Backspace' && !items.find(i => i.id === itemId)?.text) {
      e.preventDefault()
      if (idx === 0 && unchecked.length <= 1) { onBackspaceAtStart?.(); return }
      remove(itemId)
    }
    else if (e.key === 'Escape') { e.preventDefault(); onEscape?.() }
    else if (e.key === 'ArrowDown' && idx < unchecked.length - 1) { e.preventDefault(); setFocusId(unchecked[idx + 1].id) }
    else if (e.key === 'ArrowUp' && idx > 0) { e.preventDefault(); setFocusId(unchecked[idx - 1].id) }
    else if (e.key === 'Tab') e.preventDefault()
  }

  return (
    <div>
      {unchecked.map((item, idx) => (
        <EditRow key={item.id} item={item} checked={!moveCheckedToBottom ? item.checked : false} autoFocus={focusId === item.id}
          inputRef={idx === 0 ? firstInputRef : undefined}
          onCheck={() => update(item.id, { checked: !item.checked })}
          onChange={t => update(item.id, { text: t })}
          onKeyDown={e => handleKeyDown(e, item.id, idx)}
          onRemove={() => remove(item.id)}
          onFocused={() => { if (focusId === item.id) setFocusId(null) }}
          isDragOver={dragOverId === item.id}
          onDragStart={() => setDragId(item.id)}
          onDragOver={() => setDragOverId(item.id)}
          onDrop={() => handleDrop(item.id)}
          onDragEnd={() => { setDragId(null); setDragOverId(null) }} />
      ))}
      <button onClick={addItem} tabIndex={-1} className="text-[13px] text-[#80868b] hover:text-[#5f6368] pl-[54px] py-1 transition-colors">
        + List item
      </button>
      {moveCheckedToBottom && checked.length > 0 && (
        <>
          <button onClick={() => setShowChecked(!showChecked)} className="flex items-center gap-2 text-[12px] text-[#80868b] hover:text-[#5f6368] border-t border-black/[0.06] mt-2 pt-2 w-full transition-colors">
            <span className="text-[10px]">{showChecked ? '▼' : '▶'}</span>
            {checked.length} completed
          </button>
          {showChecked && checked.map(item => (
            <div key={item.id} className="grid min-h-[29px] group items-center" style={{ gridTemplateColumns: '24px 30px 1fr auto' }}>
              <div />
              <div className="w-[22px] flex items-center">
                <button onClick={() => update(item.id, { checked: false })} className="cursor-pointer outline-none" tabIndex={-1}>
                  <CheckedSvg />
                </button>
              </div>
              <div className="text-[14px] line-through text-[#80868b]">{item.text}</div>
              <button onClick={() => remove(item.id)} tabIndex={-1} className="opacity-0 group-hover:opacity-100 text-[#80868b] hover:text-[#5f6368] text-xs px-1 transition-opacity">✕</button>
            </div>
          ))}
        </>
      )}
    </div>
  )
})

function EditRow({ item, checked, autoFocus, inputRef: extRef, onCheck, onChange, onKeyDown, onRemove, onFocused, isDragOver, onDragStart, onDragOver, onDrop, onDragEnd }: {
  item: ChecklistItem; checked: boolean; autoFocus: boolean; inputRef?: React.RefObject<HTMLInputElement | null>
  onCheck: () => void; onChange: (t: string) => void; onKeyDown: (e: React.KeyboardEvent) => void; onRemove: () => void; onFocused: () => void
  isDragOver: boolean; onDragStart: () => void; onDragOver: () => void; onDrop: () => void; onDragEnd: () => void
}) {
  const localRef = useRef<HTMLInputElement>(null)
  const r = extRef || localRef
  useEffect(() => {
    if (autoFocus && r.current) { r.current.focus(); const l = r.current.value.length; r.current.setSelectionRange(l, l); onFocused() }
  }, [autoFocus])

  return (
    <div className={`grid min-h-[29px] group items-center ${isDragOver ? 'border-t-2 border-[#1a73e8]' : 'border-t border-transparent'}`}
      style={{ gridTemplateColumns: '24px 30px 1fr auto' }}
      onDragOver={e => { e.preventDefault(); onDragOver() }} onDrop={e => { e.preventDefault(); onDrop() }}>
      {/* Drag handle — visible on hover */}
      <div className="flex items-center justify-center cursor-grab opacity-0 group-hover:opacity-[.54] transition-opacity"
        draggable onDragStart={onDragStart} onDragEnd={onDragEnd} data-testid="drag-handle">
        <svg width="18" height="18" viewBox="0 0 24 24" fill="#000" style={{ opacity: 0.54 }}>
          <circle cx="9" cy="6" r="1.5"/><circle cx="15" cy="6" r="1.5"/>
          <circle cx="9" cy="12" r="1.5"/><circle cx="15" cy="12" r="1.5"/>
          <circle cx="9" cy="18" r="1.5"/><circle cx="15" cy="18" r="1.5"/>
        </svg>
      </div>
      {/* Checkbox */}
      <div className="w-[22px] flex items-center">
        <button onClick={onCheck} className="cursor-pointer outline-none" tabIndex={-1}>
          {checked ? <CheckedSvg /> : <UncheckedSvg />}
        </button>
      </div>
      {/* Input */}
      <input ref={r} type="text" value={item.text} onChange={e => onChange(e.target.value)} onKeyDown={onKeyDown}
        placeholder="List item" className={`bg-transparent outline-none text-[14px] placeholder:text-[#bdc1c6] ${checked ? 'line-through text-[#80868b]' : 'text-[#3c4043]'}`} />
      {/* Remove button */}
      <button onClick={onRemove} tabIndex={-1} className="opacity-0 group-hover:opacity-100 text-[#80868b] hover:text-[#5f6368] text-xs px-1 transition-opacity">✕</button>
    </div>
  )
}
