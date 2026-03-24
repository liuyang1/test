import { Note } from '../types/note'
import { useState } from 'react'
import DOMPurify from 'dompurify'
import { Checklist } from './Checklist'
import { Linkified, LinkPreview, extractUrls } from './LinkPreview'
import { PinIcon, PaletteIcon, ArchiveIcon, DeleteIcon, LabelIcon, MoreIcon, CheckCircleIcon } from './Icons'
import { isEmptyHtml } from '../sync/note-utils'
import { ColorPicker, getNoteBackground } from './ColorPicker'

interface Props {
  note: Note; onClick: () => void; onUpdate: (note: Note) => void; listView?: boolean
  selected?: boolean; onSelect?: (id: string) => void; selectionActive?: boolean
}

function formatDate(ts: number): string {
  const d = new Date(ts), now = new Date()
  if (d.toDateString() === now.toDateString()) return d.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })
  if (d.getFullYear() === now.getFullYear()) return d.toLocaleDateString([], { month: 'short', day: 'numeric' })
  return d.toLocaleDateString([], { year: 'numeric', month: 'short', day: 'numeric' })
}

export function NoteCard({ note, onClick, onUpdate, listView, selected, onSelect, selectionActive }: Props) {
  const [showColorPicker, setShowColorPicker] = useState(false)
  const allText = [note.title, note.content, ...note.checklist.map(i => i.text)].join(' ')
  const urls = extractUrls(allText)
  const isDefault = note.color === '#ffffff'
  const act = (fn: () => void) => (e: React.MouseEvent) => { e.stopPropagation(); fn() }
  const handleClick = () => { if (selectionActive && onSelect) onSelect(note.id); else onClick() }

  return (
    <div onClick={handleClick}
      className={`note-card rounded-lg cursor-pointer select-none min-h-[60px] group relative ${listView ? 'flex gap-4 items-start p-3' : 'p-3'} ${selected ? 'ring-2 ring-[#1a73e8]' : ''}`}
      style={{ ...getNoteBackground(note.background, note.color), border: `1px solid ${isDefault ? '#e0e0e0' : 'rgba(0,0,0,0.08)'}` }}>

      {/* Select checkbox — 20×20 centered on top-left corner via translate(-7,-7) */}
      {onSelect && !note.deleted && (
        <button onClick={act(() => onSelect(note.id))} data-testid="select-check"
          className={`absolute left-0 top-0 w-5 h-5 rounded-full flex items-center justify-center z-[201] transition-all duration-200 ${selected ? 'opacity-100 bg-[#6f6f6f]' : 'opacity-0 group-hover:opacity-100 bg-white'}`}
          style={{ transform: 'translate(-7px, -7px)' }}>
          <svg width="24" height="24" viewBox="0 0 24 24" fill={selected ? '#fff' : '#000'}>
            <path d="M12,2 C17.52,2 22,6.48 22,12 C22,17.52 17.52,22 12,22 C6.48,22 2,17.52 2,12 C2,6.48 6.48,2 12,2 Z M10,14.2 L7.4,11.6 L6,13 L10,17 L18,9 L16.6,7.6 L10,14.2 Z" />
          </svg>
        </button>
      )}

      {/* Pin — right:4px top:5px, opacity .71 pinned / .54 hover / 0 default */}
      {!note.deleted && (
        <button onClick={act(() => onUpdate({ ...note, pinned: !note.pinned, updatedAt: Date.now() }))}
          className={`pin-btn absolute z-[201] rounded-full w-9 h-9 flex items-center justify-center hover:bg-black/[0.08] ${note.pinned ? 'pinned' : 'unpinned'}`}
          style={{ right: 4, top: 5 }}>
          <PinIcon size={24} filled={note.pinned} />
        </button>
      )}

      <div className={listView ? 'flex-1 min-w-0' : ''}>
        {note.title && (
          <div className={`font-medium leading-snug mb-1.5 text-[#202124] ${listView ? 'text-[1rem]' : 'text-[1rem] pr-10'}`}>
            <Linkified text={note.title} />
          </div>
        )}
        {note.type === 'checklist' ? (
          <Checklist items={note.checklist} onChange={items => onUpdate({ ...note, checklist: items, updatedAt: Date.now() })} preview />
        ) : (note.content && !isEmptyHtml(note.content)) ? (
          <div className={`text-[11pt] leading-[1.38] text-[#3c4043] font-['Google_Sans_Text',Roboto,sans-serif] [&_h1]:text-[18px] [&_h1]:font-medium [&_h2]:text-[15px] [&_h2]:font-medium [&_h2]:italic [&_h2]:text-[#5f6368] [&_strong]:font-medium [&_a]:text-[#1a73e8] [&_a]:underline [&_ul]:list-disc [&_ul]:pl-5 [&_ol]:list-decimal [&_ol]:pl-5`}
            dangerouslySetInnerHTML={{ __html: DOMPurify.sanitize(note.content) }} onClick={e => { if ((e.target as HTMLElement).tagName === 'A') e.stopPropagation() }} />
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

      {/* Hover action bar — matches Keep: 32px buttons, 18px icons, opacity transition */}
      {!note.deleted && (
        <div className="card-actions flex items-center px-1 py-0.5 -mx-1 opacity-0 group-hover:opacity-100 transition-opacity duration-200" data-testid="card-actions">
          <ActionBtn title="Archive" onClick={act(() => onUpdate({ ...note, archived: !note.archived, updatedAt: Date.now() }))}><ArchiveIcon size={18} /></ActionBtn>
          <ActionBtn title="Change color" onClick={act(() => setShowColorPicker(!showColorPicker))}><PaletteIcon size={18} /></ActionBtn>
          <ActionBtn title="Add label" onClick={act(onClick)}><LabelIcon size={18} /></ActionBtn>
          <ActionBtn title="Delete" onClick={act(() => onUpdate({ ...note, deleted: true, deletedAt: Date.now(), updatedAt: Date.now() }))}><DeleteIcon size={18} /></ActionBtn>
          <ActionBtn title="More" onClick={act(onClick)}><MoreIcon size={18} /></ActionBtn>
          {showColorPicker && (
            <div className="absolute bottom-10 left-0 z-20 bg-white rounded-lg shadow-xl border border-[#e0e0e0] p-1" onClick={e => e.stopPropagation()}>
              <ColorPicker current={note.color} onChange={c => { onUpdate({ ...note, color: c, updatedAt: Date.now() }); setShowColorPicker(false) }} />
            </div>
          )}
        </div>
      )}

      {/* Date — visible by default, hidden when action bar shows */}
      <div className={`text-[11px] text-[#80868b] ${note.deleted ? 'mt-1.5' : 'mt-0.5 h-[26px] leading-[26px] group-hover:opacity-0 transition-opacity duration-200'}`}>
        {formatDate(note.updatedAt)}
      </div>
    </div>
  )
}

function ActionBtn({ children, title, onClick }: { children: React.ReactNode; title: string; onClick: (e: React.MouseEvent) => void }) {
  return (
    <button title={title} onClick={onClick}
      className="w-8 h-8 mx-[3px] rounded-full flex items-center justify-center text-[#202124] opacity-[.71] hover:opacity-100 hover:bg-black/[0.08] active:bg-black/[0.12] transition-all">
      {children}
    </button>
  )
}
