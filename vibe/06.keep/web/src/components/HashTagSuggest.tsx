import { useState, useEffect, useRef, useCallback } from 'react'

interface Props {
  text: string
  cursorPos: number
  allLabels: string[]
  onSelect: (label: string, hashStart: number, hashEnd: number) => void
  onCreate: (label: string, hashStart: number, hashEnd: number) => void
  anchorRef: React.RefObject<HTMLElement | null>
}

export function HashTagSuggest({ text, cursorPos, allLabels, onSelect, onCreate, anchorRef }: Props) {
  const [active, setActive] = useState(0)
  const ref = useRef<HTMLDivElement>(null)

  // Find #fragment at cursor
  const before = text.slice(0, cursorPos)
  const match = before.match(/#(\S*)$/)
  const query = match ? match[1] : null
  const hashStart = match ? match.index! : -1
  const hashEnd = cursorPos

  const filtered = query !== null ? allLabels.filter(l => l.toLowerCase().includes(query.toLowerCase())) : []
  const canCreate = query !== null && query.length > 0 && !allLabels.some(l => l.toLowerCase() === query.toLowerCase())
  const showCreate = canCreate ? [`__create__`] : []
  const items = [...filtered, ...showCreate]
  const open = query !== null && items.length > 0

  useEffect(() => { setActive(0) }, [query])

  const pick = useCallback((idx: number) => {
    if (!match) return
    if (idx < filtered.length) onSelect(filtered[idx], hashStart, hashEnd)
    else onCreate(match[1], hashStart, hashEnd)
  }, [match, filtered, hashStart, hashEnd, onSelect, onCreate])

  useEffect(() => {
    if (!open) return
    const handler = (e: KeyboardEvent) => {
      if (e.key === 'ArrowDown') { e.preventDefault(); e.stopPropagation(); setActive(p => (p + 1) % items.length) }
      else if (e.key === 'ArrowUp') { e.preventDefault(); e.stopPropagation(); setActive(p => (p - 1 + items.length) % items.length) }
      else if (e.key === 'Enter' || e.key === 'Tab') { e.preventDefault(); e.stopPropagation(); pick(active) }
      else if (e.key === 'Escape') { e.preventDefault(); e.stopPropagation() }
    }
    document.addEventListener('keydown', handler, true)
    return () => document.removeEventListener('keydown', handler, true)
  }, [open, active, items.length, pick])

  if (!open) return null

  const rect = anchorRef.current?.getBoundingClientRect()
  if (!rect) return null

  return (
    <div ref={ref} data-testid="hashtag-suggest"
      className="fixed bg-white rounded-lg shadow-2xl border border-[#e0e0e0] py-1 z-[100] max-h-48 overflow-y-auto min-w-[200px]"
      style={{ top: rect.bottom + 4, left: rect.left }}>
      {items.map((item, i) => (
        <div key={item} onMouseDown={e => { e.preventDefault(); pick(i) }}
          className={`px-3 py-1.5 text-[13px] cursor-pointer ${i === active ? 'bg-[#e8f0fe] text-[#1a73e8]' : 'text-[#3c4043] hover:bg-[#f1f3f4]'}`}>
          {item === '__create__' ? <span className="text-[#1a73e8]">+ Create "{match![1]}"</span> : <span>#{item}</span>}
        </div>
      ))}
    </div>
  )
}
