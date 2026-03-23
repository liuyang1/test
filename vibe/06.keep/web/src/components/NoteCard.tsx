import { Note } from '../types/note'
import { useState } from 'react'
import { Checklist } from './Checklist'
import { Linkified, LinkPreview, extractUrls } from './LinkPreview'
import { PinIcon, PaletteIcon, ArchiveIcon, DeleteIcon, LabelIcon, MoreIcon } from './Icons'
import { ColorPicker, getNoteBackground } from './ColorPicker'

interface Props {
  note: Note; onClick: () => void; onUpdate: (note: Note) => void; listView?: boolean
}

function formatDate(ts: number): string {
  const d = new Date(ts), now = new Date()
  if (d.toDateString() === now.toDateString()) return d.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })
  if (d.getFullYear() === now.getFullYear()) return d.toLocaleDateString([], { month: 'short', day: 'numeric' })
  return d.toLocaleDateString([], { year: 'numeric', month: 'short', day: 'numeric' })
}

export function NoteCard({ note, onClick, onUpdate, listView }: Props) {
  const [showColorPicker, setShowColorPicker] = useState(false)
  const allText = [note.title, note.content, ...note.checklist.map(i => i.text)].join(' ')
  const urls = extractUrls(allText)
  const isDefault = note.color === '#ffffff'
  const act = (fn: () => void) => (e: React.MouseEvent) => { e.stopPropagation(); fn() }

  return (
    <div onClick={onClick}
      className={`note-card rounded-lg cursor-pointer select-none min-h-[60px] group relative ${listView ? 'flex gap-4 items-start p-3' : 'p-3'}`}
      style={{ ...getNoteBackground(note.background, note.color), border: `1px solid ${isDefault ? '#e0e0e0' : 'rgba(0,0,0,0.08)'}` }}>

      {/* Pin — top right, visible on hover or when pinned */}
      {!note.deleted && (
        <button onClick={act(() => onUpdate({ ...note, pinned: !note.pinned, updatedAt: Date.now() }))}
          className={`pin-btn absolute top-1.5 right-1.5 w-8 h-8 rounded-full flex items-center justify-center z-10 hover:bg-black/[0.08] transition-all ${note.pinned ? 'pinned opacity-100' : 'unpinned opacity-0 group-hover:opacity-100'}`}>
          <PinIcon size={18} filled={note.pinned} />
        </button>
      )}

      <div className={listView ? 'flex-1 min-w-0' : ''}>
        {note.title && (
          <div className={`font-medium leading-snug mb-1.5 text-[#202124] ${listView ? 'text-[16px]' : 'text-[16px] line-clamp-2 pr-8'}`}>
            <Linkified text={note.title} />
          </div>
        )}
        {note.type === 'checklist' ? (
          <Checklist items={note.checklist} onChange={items => onUpdate({ ...note, checklist: items, updatedAt: Date.now() })} preview />
        ) : note.content ? (
          <div className={`text-[14px] leading-[1.5] text-[#3c4043] ${listView ? '' : 'line-clamp-10'} [&_h1]:text-[18px] [&_h1]:font-medium [&_h2]:text-[15px] [&_h2]:font-medium [&_h2]:italic [&_h2]:text-[#5f6368] [&_strong]:font-medium [&_a]:text-[#1a73e8] [&_a]:underline [&_ul]:list-disc [&_ul]:pl-5 [&_ol]:list-decimal [&_ol]:pl-5`}
            dangerouslySetInnerHTML={{ __html: note.content }} onClick={e => { if ((e.target as HTMLElement).tagName === 'A') e.stopPropagation() }} />
        ) : null}
        <LinkPreview urls={urls} />
        {note.labels.length > 0 && (
          <div className="flex flex-wrap gap-1 mt-2">
            {note.labels.map(l => (
              <span key={l} className="text-[12px] leading-tight bg-black/[0.06] rounded-full px-2.5 py-1 text-[#3c4043]">{l}</span>
            ))}
          </div>
        )}
      </div>

      {/* Hover action bar */}
      {!note.deleted && (
        <div className="card-actions flex items-center px-0 py-0.5 -mx-0.5 relative" data-testid="card-actions">
          <ActionBtn title="Archive" onClick={act(() => onUpdate({ ...note, archived: !note.archived, updatedAt: Date.now() }))}><ArchiveIcon size={16} /></ActionBtn>
          <ActionBtn title="Change color" onClick={act(() => setShowColorPicker(!showColorPicker))}><PaletteIcon size={16} /></ActionBtn>
          <ActionBtn title="Add label" onClick={act(onClick)}><LabelIcon size={16} /></ActionBtn>
          <ActionBtn title="Delete" onClick={act(() => onUpdate({ ...note, deleted: true, deletedAt: Date.now(), updatedAt: Date.now() }))}><DeleteIcon size={16} /></ActionBtn>
          <ActionBtn title="More" onClick={act(onClick)}><MoreIcon size={16} /></ActionBtn>
          {showColorPicker && (
            <div className="absolute bottom-8 left-0 z-20 bg-white rounded-lg shadow-xl border border-[#e0e0e0] p-1" onClick={e => e.stopPropagation()}>
              <ColorPicker current={note.color} onChange={c => { onUpdate({ ...note, color: c, updatedAt: Date.now() }); setShowColorPicker(false) }} />
            </div>
          )}
        </div>
      )}

      {/* Date — always visible, bottom right, subtle */}
      <div className={`text-[11px] text-[#80868b] ${note.deleted ? 'mt-1.5' : 'mt-1 group-hover:invisible'}`}>
        {formatDate(note.updatedAt)}
      </div>
    </div>
  )
}

function ActionBtn({ children, title, onClick }: { children: React.ReactNode; title: string; onClick: (e: React.MouseEvent) => void }) {
  return (
    <button title={title} onClick={onClick}
      className="w-7 h-7 rounded-full flex items-center justify-center text-[#5f6368] hover:bg-black/[0.08] active:bg-black/[0.12] transition-colors">
      {children}
    </button>
  )
}
