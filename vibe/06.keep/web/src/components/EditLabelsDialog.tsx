import { useState } from 'react'
import { DeleteIcon, CheckIcon, AddIcon, LabelIcon, CloseIcon } from './Icons'

interface Props {
  labels: string[]
  onAdd: (label: string) => void
  onRemove: (label: string) => void
  onRename: (old: string, next: string) => void
  onClose: () => void
}

export function EditLabelsDialog({ labels, onAdd, onRemove, onRename, onClose }: Props) {
  const [newLabel, setNewLabel] = useState('')
  const [editingLabel, setEditingLabel] = useState<string | null>(null)
  const [editValue, setEditValue] = useState('')

  const handleCreate = () => {
    const v = newLabel.trim()
    if (v && !labels.includes(v)) { onAdd(v); setNewLabel('') }
  }

  const startEdit = (l: string) => { setEditingLabel(l); setEditValue(l) }
  const commitEdit = () => {
    if (editingLabel && editValue.trim() && editValue.trim() !== editingLabel) {
      onRename(editingLabel, editValue.trim())
    }
    setEditingLabel(null)
  }

  return (
    <div className="editor-overlay fixed inset-0 z-50 flex items-center justify-center bg-black/30" onClick={onClose}>
      <div className="bg-white rounded-lg shadow-2xl w-[300px] max-h-[80vh] flex flex-col" onClick={e => e.stopPropagation()}>
        <div className="px-4 pt-4 pb-2 text-[15px] font-medium text-[#202124]">Edit labels</div>

        {/* Create new label */}
        <div className="flex items-center gap-2 px-3 py-1.5 group">
          <button onClick={() => newLabel.trim() ? handleCreate() : undefined}
            className="w-8 h-8 rounded-full flex items-center justify-center text-[#5f6368] hover:bg-[#f1f3f4] transition-colors">
            {newLabel.trim() ? <CheckIcon size={18} /> : <AddIcon size={18} />}
          </button>
          <input value={newLabel} onChange={e => setNewLabel(e.target.value)}
            onKeyDown={e => { if (e.key === 'Enter') handleCreate() }}
            placeholder="Create new label" className="flex-1 outline-none text-[14px] text-[#202124] placeholder:text-[#80868b] bg-transparent border-b border-transparent focus:border-[#e0e0e0] py-1" />
          {newLabel && (
            <button onClick={() => setNewLabel('')} className="w-8 h-8 rounded-full flex items-center justify-center text-[#5f6368] hover:bg-[#f1f3f4]">
              <CloseIcon size={18} />
            </button>
          )}
        </div>

        {/* Existing labels */}
        <div className="flex-1 overflow-y-auto px-1 py-1">
          {labels.map(l => (
            <div key={l} className="flex items-center gap-2 px-2 py-1 group hover:bg-[#f1f3f4] rounded-md">
              {editingLabel === l ? (
                <>
                  <button onClick={() => onRemove(l)} className="w-8 h-8 rounded-full flex items-center justify-center text-[#5f6368] hover:bg-black/[0.08]">
                    <DeleteIcon size={18} />
                  </button>
                  <input value={editValue} onChange={e => setEditValue(e.target.value)} autoFocus
                    onKeyDown={e => { if (e.key === 'Enter') commitEdit(); if (e.key === 'Escape') setEditingLabel(null) }}
                    onBlur={commitEdit}
                    className="flex-1 outline-none text-[14px] text-[#202124] bg-transparent border-b border-[#e0e0e0] py-1" />
                  <button onClick={commitEdit} className="w-8 h-8 rounded-full flex items-center justify-center text-[#5f6368] hover:bg-black/[0.08]">
                    <CheckIcon size={18} />
                  </button>
                </>
              ) : (
                <>
                  <button onClick={() => onRemove(l)} className="w-8 h-8 rounded-full flex items-center justify-center text-[#5f6368] opacity-0 group-hover:opacity-100 hover:bg-black/[0.08] transition-opacity" data-testid="delete-label">
                    <DeleteIcon size={18} />
                  </button>
                  <span className="flex-1 text-[14px] text-[#202124] py-1 cursor-text" onClick={() => startEdit(l)}>{l}</span>
                  <button onClick={() => startEdit(l)} className="w-8 h-8 rounded-full flex items-center justify-center text-[#5f6368] opacity-0 group-hover:opacity-100 hover:bg-black/[0.08] transition-opacity">
                    <span className="material-symbols-outlined" style={{ fontSize: 18 }}>edit</span>
                  </button>
                </>
              )}
            </div>
          ))}
        </div>

        <div className="border-t border-[#e0e0e0] px-4 py-2.5 flex justify-end">
          <button onClick={onClose} className="px-4 py-1.5 text-[14px] font-medium text-[#202124] hover:bg-[#f1f3f4] rounded-md transition-colors">Done</button>
        </div>
      </div>
    </div>
  )
}
