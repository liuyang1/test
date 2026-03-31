import { useRef, useEffect, useState, forwardRef, useImperativeHandle, useCallback } from 'react'
import { ChecklistItem } from '../types/note'
import { createChecklistItemData as createChecklistItem } from '../sync/yjs-sync'
import { useSettings } from '../hooks/useSettings'
import { useEditor, EditorContent, Editor } from '@tiptap/react'
import StarterKit from '@tiptap/starter-kit'
import Placeholder from '@tiptap/extension-placeholder'
import Underline from '@tiptap/extension-underline'
import DOMPurify from 'dompurify'

export interface ChecklistHandle {
  focusFirst: () => void
  activeEditor: Editor | null
}

interface Props {
  items: ChecklistItem[]
  onChange: (items: ChecklistItem[]) => void
  preview?: boolean
  onEscape?: () => void
  moveCheckedToBottom?: boolean
  onBackspaceAtStart?: () => void
  onActiveEditorChange?: (editor: Editor | null) => void
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

export const Checklist = forwardRef<ChecklistHandle, Props>(({ items, onChange, preview, onEscape, moveCheckedToBottom: propMoveChecked, onBackspaceAtStart, onActiveEditorChange }, ref) => {
  const settings = useSettings()
  const moveCheckedToBottom = propMoveChecked ?? settings.moveCheckedToBottom
  const [focusId, setFocusId] = useState<string | null>(null)
  const [showChecked, setShowChecked] = useState(true)
  const [activeEditor, setActiveEditor] = useState<Editor | null>(null)
  const firstInputRef = useRef<HTMLInputElement>(null)
  const [dragId, setDragId] = useState<string | null>(null)
  const [dragOverId, setDragOverId] = useState<string | null>(null)

  useImperativeHandle(ref, () => ({
    focusFirst: () => {
      const uc = items.filter(i => !i.checked)
      if (uc.length) setFocusId(uc[0].id)
    },
    activeEditor,
  }), [items, activeEditor])

  const handleActiveEditorChange = useCallback((editor: Editor | null) => {
    setActiveEditor(editor)
    onActiveEditorChange?.(editor)
  }, [onActiveEditorChange])

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
            <div className={`text-[11pt] leading-[1.38] font-['Google_Sans_Text',Roboto,sans-serif] pt-[3px] [&_strong]:font-medium ${i.checked ? 'line-through text-[#80868b]' : 'text-[#3c4043]'}`}
              style={{ overflowWrap: 'break-word', minWidth: 0 }}
              dangerouslySetInnerHTML={{ __html: DOMPurify.sanitize(i.text) || '\u00A0' }} />
          </div>
        ))}
        {checked.length > 0 && <div className="text-[12px] text-[#80868b] pl-[30px] mt-1">+{checked.length} checked</div>}
      </div>
    )
  }

  // ─── Edit mode — drag handle + checkbox + rich content ───
  return (
    <div>
      {unchecked.map((item, idx) => (
        <EditRow key={item.id} item={item} checked={!moveCheckedToBottom ? item.checked : false} autoFocus={focusId === item.id}
          onCheck={() => update(item.id, { checked: !item.checked })}
          onChange={t => update(item.id, { text: t })}
          onEnter={(atStart) => atStart ? addItemBefore(item.id) : addItemAfter(item.id)}
          onBackspace={() => {
            if (idx === 0 && unchecked.length <= 1) { onBackspaceAtStart?.(); return }
            remove(item.id)
          }}
          onEscape={() => onEscape?.()}
          onArrowDown={() => { if (idx < unchecked.length - 1) setFocusId(unchecked[idx + 1].id) }}
          onArrowUp={() => { if (idx > 0) setFocusId(unchecked[idx - 1].id); else onBackspaceAtStart?.() }}
          onRemove={() => remove(item.id)}
          onFocused={() => { if (focusId === item.id) setFocusId(null) }}
          onEditorFocus={handleActiveEditorChange}
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
              <div className="text-[14px] line-through text-[#80868b] [&_strong]:font-medium"
                dangerouslySetInnerHTML={{ __html: DOMPurify.sanitize(item.text) }} />
              <button onClick={() => remove(item.id)} tabIndex={-1} className="opacity-0 group-hover:opacity-100 text-[#80868b] hover:text-[#5f6368] text-xs px-1 transition-opacity">✕</button>
            </div>
          ))}
        </>
      )}
    </div>
  )
})

function EditRow({ item, checked, autoFocus, onCheck, onChange, onEnter, onBackspace, onEscape, onArrowDown, onArrowUp, onRemove, onFocused, onEditorFocus, isDragOver, onDragStart, onDragOver, onDrop, onDragEnd }: {
  item: ChecklistItem; checked: boolean; autoFocus: boolean
  onCheck: () => void; onChange: (html: string) => void; onEnter: (atStart: boolean) => void; onBackspace: () => void
  onEscape: () => void; onArrowDown: () => void; onArrowUp: () => void
  onRemove: () => void; onFocused: () => void; onEditorFocus: (editor: Editor | null) => void
  isDragOver: boolean; onDragStart: () => void; onDragOver: () => void; onDrop: () => void; onDragEnd: () => void
}) {
  const editor = useEditor({
    extensions: [
      StarterKit.configure({
        heading: false, blockquote: false, codeBlock: false, code: false,
        horizontalRule: false, bulletList: false, orderedList: false, listItem: false,
        hardBreak: false,
      }),
      Underline,
      Placeholder.configure({ placeholder: 'List item' }),
    ],
    content: item.text,
    onUpdate: ({ editor: e }) => onChange(e.getHTML()),
    editorProps: {
      attributes: { class: `checklist-item-editor outline-none text-[14px] [&_strong]:font-medium ${checked ? 'line-through text-[#80868b]' : 'text-[#3c4043]'} [&_.is-editor-empty:first-child::before]:text-[#bdc1c6] [&_.is-editor-empty:first-child::before]:content-[attr(data-placeholder)] [&_.is-editor-empty:first-child::before]:float-left [&_.is-editor-empty:first-child::before]:pointer-events-none` },
      handleKeyDown: (_view, event) => {
        if (event.key === 'Enter') { event.preventDefault(); const { from } = _view.state.selection; onEnter(from <= 1 && _view.state.doc.textContent.length > 0); return true }
        if (event.key === 'Escape') { event.preventDefault(); onEscape(); return true }
        if (event.key === 'ArrowDown') { event.preventDefault(); onArrowDown(); return true }
        if (event.key === 'ArrowUp') { event.preventDefault(); onArrowUp(); return true }
        if (event.key === 'Tab') { event.preventDefault(); return true }
        if (event.key === 'Backspace') {
          const { from, empty } = _view.state.selection
          if (empty && from <= 1 && _view.state.doc.textContent === '') { event.preventDefault(); onBackspace(); return true }
        }
        return false
      },
    },
    onFocus: ({ editor: e }) => onEditorFocus(e),
  })

  useEffect(() => {
    if (autoFocus && editor) { editor.commands.focus('end'); onFocused() }
  }, [autoFocus, editor])

  useEffect(() => {
    if (editor && item.text !== editor.getHTML()) editor.commands.setContent(item.text)
  }, [item.text])

  return (
    <div className={`grid min-h-[29px] group items-center ${isDragOver ? 'border-t-2 border-[#1a73e8]' : 'border-t border-transparent'}`}
      style={{ gridTemplateColumns: '24px 30px 1fr auto' }}
      onDragOver={e => { e.preventDefault(); onDragOver() }} onDrop={e => { e.preventDefault(); onDrop() }}>
      <div className="flex items-center justify-center cursor-grab opacity-0 group-hover:opacity-[.54] transition-opacity"
        draggable onDragStart={onDragStart} onDragEnd={onDragEnd} data-testid="drag-handle">
        <svg width="18" height="18" viewBox="0 0 24 24" fill="#000" style={{ opacity: 0.54 }}>
          <circle cx="9" cy="6" r="1.5"/><circle cx="15" cy="6" r="1.5"/>
          <circle cx="9" cy="12" r="1.5"/><circle cx="15" cy="12" r="1.5"/>
          <circle cx="9" cy="18" r="1.5"/><circle cx="15" cy="18" r="1.5"/>
        </svg>
      </div>
      <div className="w-[22px] flex items-center">
        <button onClick={onCheck} className="cursor-pointer outline-none" tabIndex={-1}>
          {checked ? <CheckedSvg /> : <UncheckedSvg />}
        </button>
      </div>
      <div className="min-w-0" style={{ overflowWrap: 'break-word' }}><EditorContent editor={editor} /></div>
      <button onClick={onRemove} tabIndex={-1} className="opacity-0 group-hover:opacity-100 text-[#80868b] hover:text-[#5f6368] text-xs px-1 transition-opacity">✕</button>
    </div>
  )
}
