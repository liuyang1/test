import { useState, useEffect, useRef } from 'react'
import { Note } from '../types/note'
import { ColorPicker, getNoteBackground } from './ColorPicker'
import { Checklist, ChecklistHandle } from './Checklist'
import { LabelPicker } from './LabelPicker'
import { RichEditor, RichEditorHandle, FormatBar } from './RichEditor'
import { HashTagSuggest } from './HashTagSuggest'
import { PinIcon, PaletteIcon, LabelIcon, CheckBoxIcon, TextIcon, ArchiveIcon, UnarchiveIcon, DeleteIcon, MoreIcon } from './Icons'
import { createChecklistItemData as createChecklistItem } from '../sync/yjs-sync'
import { stripHtml, isEmptyHtml } from '../sync/note-utils'

interface Props {
  note: Note; labels: string[]
  onSave: (note: Note) => void; onClose: () => void; onDelete: () => void; onAddLabel: (label: string) => void
}

function fmtDate(ts: number) {
  return new Date(ts).toLocaleString([], { year: 'numeric', month: 'short', day: 'numeric', hour: '2-digit', minute: '2-digit' })
}

export function NoteEditor({ note, labels, onSave, onClose, onDelete, onAddLabel }: Props) {
  const [draft, setDraft] = useState<Note>(note)
  const [showColors, setShowColors] = useState(false)
  const [showLabels, setShowLabels] = useState(false)
  const [editor, setEditor] = useState<any>(null)
  const [titleCursor, setTitleCursor] = useState(0)
  const titleRef = useRef<HTMLInputElement>(null)
  const richRef = useRef<RichEditorHandle>(null)
  const checklistRef = useRef<ChecklistHandle>(null)

  useEffect(() => { setDraft(note) }, [note])
  useEffect(() => { titleRef.current?.focus() }, [])

  const patch = (p: Partial<Note>) => { const u = { ...draft, ...p }; setDraft(u); onSave(u) }
  const toggleType = () => {
    if (draft.type === 'text') {
      // Convert each line/paragraph of rich text content into a checklist item
      const text = stripHtml(draft.content)
      const lines = text.split('\n').filter(l => l.trim())
      const items = lines.map(l => createChecklistItem(l))
      patch({ type: 'checklist', checklist: items.length ? items : [createChecklistItem()], content: '' })
    } else {
      // Convert checklist items back to rich text paragraphs, preserving inline formatting
      const clHtml = draft.checklist.filter(i => !isEmptyHtml(i.text)).map(i => i.text.replace(/<\/?p>/g, '')).filter(Boolean)
      const extra = clHtml.length ? `<p>${clHtml.join('</p><p>')}</p>` : ''
      const newContent = draft.content && !isEmptyHtml(draft.content) ? draft.content + extra : extra
      patch({ type: 'text', content: newContent, checklist: [] })
    }
  }
  const toggleLabel = (l: string) => patch({ labels: draft.labels.includes(l) ? draft.labels.filter(x => x !== l) : [...draft.labels, l] })
  const hashTagSelect = (label: string, start: number, end: number) => {
    const newTitle = draft.title.slice(0, start) + draft.title.slice(end)
    const newLabels = draft.labels.includes(label) ? draft.labels : [...draft.labels, label]
    patch({ title: newTitle, labels: newLabels })
    setTitleCursor(start)
    setTimeout(() => { titleRef.current?.setSelectionRange(start, start) }, 0)
  }
  const hashTagCreate = (label: string, start: number, end: number) => {
    onAddLabel(label)
    hashTagSelect(label, start, end)
  }
  const focusTitle = () => { titleRef.current?.focus(); titleRef.current?.setSelectionRange(titleRef.current.value.length, titleRef.current.value.length) }
  const focusContent = () => { draft.type === 'checklist' ? checklistRef.current?.focusFirst() : richRef.current?.focus() }

  return (
    <div className="editor-overlay fixed inset-0 z-50 flex items-end sm:items-center justify-center bg-black/30" onClick={onClose}>
      <div className="editor-panel w-full sm:max-w-[600px] sm:mx-4 rounded-t-2xl sm:rounded-xl shadow-2xl max-h-[90vh] sm:max-h-[80vh] flex flex-col"
        style={getNoteBackground(draft.background, draft.color)} onClick={e => e.stopPropagation()}>

        {/* Content */}
        <div className="p-4 sm:p-5 flex-1 overflow-y-auto">
          <div className="flex items-start gap-2">
            <input ref={titleRef} value={draft.title} onChange={e => { patch({ title: e.target.value }); setTitleCursor(e.target.selectionStart ?? 0) }}
              onKeyUp={e => setTitleCursor((e.target as HTMLInputElement).selectionStart ?? 0)}
              onClick={e => setTitleCursor((e.target as HTMLInputElement).selectionStart ?? 0)}
              onKeyDown={e => { if (e.key === 'Enter' || e.key === 'Tab' || e.key === 'ArrowDown') { e.preventDefault(); focusContent() } }}
              placeholder="Title" className="flex-1 bg-transparent outline-none font-medium text-[18px] text-[#202124] mb-3 placeholder:text-[#80868b]" />
            <HashTagSuggest text={draft.title} cursorPos={titleCursor} allLabels={labels} anchorRef={titleRef}
              onSelect={hashTagSelect} onCreate={hashTagCreate} />
            <button onClick={() => patch({ pinned: !draft.pinned })} tabIndex={-1}
              className={`pin-btn w-9 h-9 rounded-full flex items-center justify-center hover:bg-black/[0.08] flex-shrink-0 transition-all ${draft.pinned ? 'pinned' : 'unpinned'}`}>
              <PinIcon size={22} filled={draft.pinned} />
            </button>
          </div>
          {draft.type === 'checklist' ? (
            <Checklist ref={checklistRef} items={draft.checklist} onChange={cl => patch({ checklist: cl })} onBackspaceAtStart={focusTitle} onActiveEditorChange={setEditor} />
          ) : (
            <RichEditor ref={richRef} content={draft.content} onChange={c => patch({ content: c })} onEditorReady={setEditor} onBackspaceAtStart={focusTitle} />
          )}
          {draft.labels.length > 0 && (
            <div className="flex flex-wrap gap-1.5 mt-3">
              {draft.labels.map(l => (
                <span key={l} onClick={() => toggleLabel(l)} className="text-[12px] bg-black/[0.06] rounded-full px-2.5 py-1 cursor-pointer hover:bg-black/[0.12] text-[#3c4043] transition-colors flex items-center gap-1">
                  {l} <span className="text-[9px] opacity-60">✕</span>
                </span>
              ))}
            </div>
          )}
        </div>

        {/* Pickers */}
        {showColors && <div className="border-t border-black/[0.06] px-3"><ColorPicker current={draft.color} onChange={c => patch({ color: c })} currentBg={draft.background} onBgChange={bg => patch({ background: bg })} /></div>}
        {showLabels && <div className="border-t border-black/[0.06] px-3"><LabelPicker allLabels={labels} selected={draft.labels} onToggle={toggleLabel} onCreateLabel={l => { onAddLabel(l); toggleLabel(l) }} /></div>}

        {/* Format bar */}
        <FormatBar editor={editor} />

        {/* Tool buttons — bottom row */}
        <div className="flex items-center px-1.5 sm:px-2 py-1 border-t border-black/[0.06]">
          <TBtn t="Color" onClick={() => { setShowColors(!showColors); setShowLabels(false) }}><PaletteIcon size={18} /></TBtn>
          <TBtn t="Labels" onClick={() => { setShowLabels(!showLabels); setShowColors(false) }}><LabelIcon size={18} /></TBtn>
          {draft.type === 'text'
            ? <TBtn t="Checklist" onClick={toggleType}><CheckBoxIcon size={18} /></TBtn>
            : <TBtn t="Text" onClick={toggleType}><TextIcon size={18} /></TBtn>}
          <TBtn t={draft.archived ? 'Unarchive' : 'Archive'} onClick={() => { patch({ archived: !draft.archived }); onClose() }}>{draft.archived ? <UnarchiveIcon size={18} /> : <ArchiveIcon size={18} />}</TBtn>
          <TBtn t="Delete" onClick={onDelete}><DeleteIcon size={18} /></TBtn>
          <TBtn t="More" onClick={() => {}}><MoreIcon size={18} /></TBtn>
          <div className="flex-1" />
          <span className="text-[11px] text-[#80868b] hidden sm:inline mr-2">Edited {fmtDate(draft.updatedAt)}</span>
          <button onClick={onClose} tabIndex={-1} className="px-4 py-1.5 text-[14px] font-medium text-[#202124] hover:bg-black/[0.06] rounded-md transition-colors">Close</button>
        </div>
      </div>
    </div>
  )
}

function TBtn({ children, t, onClick }: { children: React.ReactNode; t: string; onClick: () => void }) {
  return <button title={t} onClick={onClick} tabIndex={-1} className="w-9 h-9 rounded-full flex items-center justify-center text-[#5f6368] hover:bg-black/[0.08] active:bg-black/[0.12] transition-colors">{children}</button>
}
