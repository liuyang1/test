import { useState } from 'react'

interface Props {
  allLabels: string[]
  selected: string[]
  onToggle: (label: string) => void
  onCreateLabel: (label: string) => void
}

export function LabelPicker({ allLabels, selected, onToggle, onCreateLabel }: Props) {
  const [search, setSearch] = useState('')

  const filtered = allLabels.filter(l => l.toLowerCase().includes(search.toLowerCase()))
  const canCreate = search.trim() && !allLabels.some(l => l.toLowerCase() === search.trim().toLowerCase())

  return (
    <div className="py-2 max-h-48 overflow-y-auto">
      <div className="text-[12px] font-medium text-[#202124] mb-2">Label note</div>
      <input
        type="text"
        value={search}
        onChange={e => setSearch(e.target.value)}
        placeholder="Enter label name"
        className="w-full bg-transparent outline-none text-[13px] mb-2 border-b border-[#e0e0e0] pb-2 placeholder:text-[#80868b]"
        autoFocus
      />
      {filtered.map(label => (
        <label key={label} className="flex items-center gap-2.5 py-1.5 cursor-pointer hover:bg-black/[0.04] rounded-md px-1 transition-colors">
          <input type="checkbox" checked={selected.includes(label)} onChange={() => onToggle(label)} className="w-4 h-4" />
          <span className="text-[13px] text-[#3c4043]">{label}</span>
        </label>
      ))}
      {canCreate && (
        <button
          onClick={() => { onCreateLabel(search.trim()); setSearch('') }}
          className="flex items-center gap-2 text-[13px] text-[#1a73e8] hover:bg-blue-50 rounded-md px-1 py-1.5 mt-1 w-full transition-colors"
        >
          <span className="text-lg leading-none">+</span> Create "{search.trim()}"
        </button>
      )}
      {!filtered.length && !canCreate && (
        <div className="text-[12px] text-[#80868b] py-2">No labels found</div>
      )}
    </div>
  )
}
