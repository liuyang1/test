import { useRef, useEffect, useState, forwardRef, useImperativeHandle } from 'react'
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

export const Checklist = forwardRef<ChecklistHandle, Props>(({ items, onChange, preview, onEscape, moveCheckedToBottom: propMoveChecked, onBackspaceAtStart }, ref) => {
  const settings = useSettings()
  const moveCheckedToBottom = propMoveChecked ?? settings.moveCheckedToBottom
  const [focusId, setFocusId] = useState<string | null>(null)
  const [showChecked, setShowChecked] = useState(true)
  const firstInputRef = useRef<HTMLInputElement>(null)

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
  const addItemAfter = (afterId: string) => {
    const n = createChecklistItem()
    const idx = items.findIndex(i => i.id === afterId)
    const next = [...items]; next.splice(idx + 1, 0, n); onChange(next); setFocusId(n.id)
  }
  const addItem = () => { const n = createChecklistItem(); onChange([...items, n]); setFocusId(n.id) }

  const getSorted = () => {
    if (moveCheckedToBottom) {
      return {
        unchecked: items.filter(i => !i.checked).sort((a, b) => a.sortOrder - b.sortOrder),
        checked: items.filter(i => i.checked).sort((a, b) => a.sortOrder - b.sortOrder),
      }
    }
    // When not moving checked to bottom, show all in original order
    return { unchecked: items.sort((a, b) => a.sortOrder - b.sortOrder), checked: [] as ChecklistItem[] }
  }

  const { unchecked, checked } = getSorted()

  // ─── Preview mode (on cards) ───
  if (preview) {
    const display = unchecked.slice(0, 8)
    return (
      <div className="space-y-0.5">
        {display.map(i => (
          <div key={i.id} className="flex items-center gap-2 py-0.5">
            <input type="checkbox" checked={!moveCheckedToBottom ? i.checked : false}
              onChange={e => { e.stopPropagation(); update(i.id, { checked: !i.checked }) }}
              onClick={e => e.stopPropagation()}
              className="w-[15px] h-[15px] flex-shrink-0 cursor-pointer" />
            <span className={`text-[14px] leading-snug truncate ${i.checked ? 'line-through text-[#80868b]' : 'text-[#3c4043]'}`}>{i.text || '\u00A0'}</span>
          </div>
        ))}
        {unchecked.length > 8 && <div className="text-[12px] text-[#80868b] pl-6">+{unchecked.length - 8} more</div>}
        {checked.length > 0 && <div className="text-[12px] text-[#80868b] pl-6 mt-1">+{checked.length} checked</div>}
      </div>
    )
  }

  // ─── Edit mode ───
  const handleKeyDown = (e: React.KeyboardEvent, itemId: string, idx: number) => {
    if (e.key === 'Enter') { e.preventDefault(); addItemAfter(itemId) }
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
    <div className="space-y-0.5">
      {unchecked.map((item, idx) => (
        <EditRow key={item.id} item={item} checked={!moveCheckedToBottom ? item.checked : false} autoFocus={focusId === item.id}
          inputRef={idx === 0 ? firstInputRef : undefined}
          onCheck={() => update(item.id, { checked: !item.checked })}
          onChange={t => update(item.id, { text: t })}
          onKeyDown={e => handleKeyDown(e, item.id, idx)}
          onRemove={() => remove(item.id)}
          onFocused={() => { if (focusId === item.id) setFocusId(null) }} />
      ))}
      <button onClick={addItem} tabIndex={-1} className="text-[13px] text-[#80868b] hover:text-[#5f6368] pl-7 py-1 transition-colors">
        + List item
      </button>
      {moveCheckedToBottom && checked.length > 0 && (
        <>
          <button onClick={() => setShowChecked(!showChecked)} className="flex items-center gap-2 text-[12px] text-[#80868b] hover:text-[#5f6368] border-t border-black/[0.06] mt-2 pt-2 w-full transition-colors">
            <span className="text-[10px]">{showChecked ? '▼' : '▶'}</span>
            {checked.length} completed
          </button>
          {showChecked && checked.map(item => (
            <div key={item.id} className="flex items-center gap-2 group py-0.5">
              <input type="checkbox" checked onChange={() => update(item.id, { checked: false })} className="w-[15px] h-[15px] flex-shrink-0" tabIndex={-1} />
              <span className="flex-1 text-[14px] line-through text-[#80868b]">{item.text}</span>
              <button onClick={() => remove(item.id)} tabIndex={-1} className="opacity-0 group-hover:opacity-100 text-[#80868b] hover:text-[#5f6368] text-xs transition-opacity">✕</button>
            </div>
          ))}
        </>
      )}
    </div>
  )
})

function EditRow({ item, checked, autoFocus, inputRef: extRef, onCheck, onChange, onKeyDown, onRemove, onFocused }: {
  item: ChecklistItem; checked: boolean; autoFocus: boolean; inputRef?: React.RefObject<HTMLInputElement | null>
  onCheck: () => void; onChange: (t: string) => void; onKeyDown: (e: React.KeyboardEvent) => void; onRemove: () => void; onFocused: () => void
}) {
  const localRef = useRef<HTMLInputElement>(null)
  const r = extRef || localRef
  useEffect(() => {
    if (autoFocus && r.current) { r.current.focus(); const l = r.current.value.length; r.current.setSelectionRange(l, l); onFocused() }
  }, [autoFocus])

  return (
    <div className="flex items-center gap-2 group py-0.5">
      <input type="checkbox" checked={checked} onChange={onCheck} className="w-[15px] h-[15px] flex-shrink-0" tabIndex={-1} />
      <input ref={r} type="text" value={item.text} onChange={e => onChange(e.target.value)} onKeyDown={onKeyDown}
        placeholder="List item" className={`flex-1 bg-transparent outline-none text-[14px] placeholder:text-[#bdc1c6] ${checked ? 'line-through text-[#80868b]' : 'text-[#3c4043]'}`} />
      <button onClick={onRemove} tabIndex={-1} className="opacity-0 group-hover:opacity-100 text-[#80868b] hover:text-[#5f6368] text-xs transition-opacity">✕</button>
    </div>
  )
}
