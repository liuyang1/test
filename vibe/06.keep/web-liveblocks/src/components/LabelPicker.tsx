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
          <button type="button" onClick={() => onToggle(label)} className="flex-shrink-0">
            {selected.includes(label) ? (
              <svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 24 24" fill="#000" style={{ opacity: 0.54 }}>
                <path d="M19 3H5c-1.1 0-2 .9-2 2v14c0 1.1.9 2 2 2h14c1.1 0 2-.9 2-2V5c0-1.1-.9-2-2-2zm0 16H5V5h14v14z"/>
                <path d="M18 9l-1.4-1.4-6.6 6.6-2.6-2.6L6 13l4 4z"/>
              </svg>
            ) : (
              <svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 24 24" fill="#000" style={{ opacity: 0.54 }}>
                <path d="M19 5v14H5V5h14m0-2H5c-1.1 0-2 .9-2 2v14c0 1.1.9 2 2 2h14c1.1 0 2-.9 2-2V5c0-1.1-.9-2-2-2z"/>
              </svg>
            )}
          </button>
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
