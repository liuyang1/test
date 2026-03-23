import { Note } from '../types/note'
import { PinIcon, PaletteIcon, ArchiveIcon, DeleteIcon, CloseIcon } from './Icons'
import { ColorPicker } from './ColorPicker'
import { useState } from 'react'

interface Props {
  selected: Set<string>
  notes: Note[]
  onUpdate: (notes: Note[]) => void
  onClear: () => void
}

export function SelectionBar({ selected, notes, onUpdate, onClear }: Props) {
  const [showColors, setShowColors] = useState(false)
  const sel = notes.filter(n => selected.has(n.id))
  const allPinned = sel.every(n => n.pinned)

  return (
    <div className="fixed top-0 left-0 right-0 z-50 bg-white border-b border-[#e0e0e0] shadow-sm flex items-center gap-2 px-4 h-16" data-testid="selection-bar">
      <button onClick={onClear} title="Clear selection" className="w-10 h-10 rounded-full flex items-center justify-center hover:bg-black/[0.08]">
        <CloseIcon size={22} />
      </button>
      <span className="text-[16px] font-medium text-[#202124] mr-4" data-testid="selection-count">{selected.size} selected</span>
      <div className="flex items-center gap-1 relative">
        <Btn title={allPinned ? 'Unpin' : 'Pin'} onClick={() => { onUpdate(sel.map(n => ({ ...n, pinned: !allPinned, updatedAt: Date.now() }))); onClear() }}>
          <PinIcon size={20} filled={allPinned} />
        </Btn>
        <Btn title="Archive" onClick={() => { onUpdate(sel.map(n => ({ ...n, archived: true, updatedAt: Date.now() }))); onClear() }}>
          <ArchiveIcon size={20} />
        </Btn>
        <div className="relative">
          <Btn title="Change color" onClick={() => setShowColors(!showColors)}>
            <PaletteIcon size={20} />
          </Btn>
          {showColors && (
            <div className="absolute top-10 left-0 z-20 bg-white rounded-lg shadow-xl border border-[#e0e0e0] p-1">
              <ColorPicker current={sel[0]?.color || '#ffffff'} onChange={c => { onUpdate(sel.map(n => ({ ...n, color: c, updatedAt: Date.now() }))); setShowColors(false); onClear() }} />
            </div>
          )}
        </div>
        <Btn title="Delete" onClick={() => { onUpdate(sel.map(n => ({ ...n, deleted: true, deletedAt: Date.now(), updatedAt: Date.now() }))); onClear() }}>
          <DeleteIcon size={20} />
        </Btn>
      </div>
    </div>
  )
}

function Btn({ children, title, onClick }: { children: React.ReactNode; title: string; onClick: () => void }) {
  return (
    <button title={title} onClick={onClick}
      className="w-10 h-10 rounded-full flex items-center justify-center text-[#5f6368] hover:bg-black/[0.08] transition-colors">
      {children}
    </button>
  )
}
