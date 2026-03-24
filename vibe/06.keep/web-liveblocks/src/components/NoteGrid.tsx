import { Note, LayoutView } from '../types/note'
import { NoteCard } from './NoteCard'
import { DndContext, closestCenter, PointerSensor, useSensor, useSensors, DragEndEvent, DragOverlay, DragStartEvent } from '@dnd-kit/core'
import { SortableContext, useSortable, rectSortingStrategy } from '@dnd-kit/sortable'
import { CSS } from '@dnd-kit/utilities'
import { useState, useRef, useEffect, useCallback } from 'react'

interface Props {
  pinned: Note[]; unpinned: Note[]
  onSelect: (note: Note) => void
  onReorder: (id: string, newSortOrder: number) => void
  onUpdate: (note: Note) => void
  layout: LayoutView
  selected?: Set<string>; onToggleSelect?: (id: string) => void
}

// ─── Masonry layout engine ───
function useMasonry(containerRef: React.RefObject<HTMLDivElement | null>, itemCount: number, gap: number, colWidth: number) {
  const [positions, setPositions] = useState<Map<string, { x: number; y: number; w: number }>>(new Map())
  const [height, setHeight] = useState(0)

  const recalc = useCallback(() => {
    const container = containerRef.current
    if (!container) return
    const containerW = container.offsetWidth
    const cols = Math.max(1, Math.floor((containerW + gap) / (colWidth + gap)))
    const actualColW = (containerW - (cols - 1) * gap) / cols
    const colHeights = new Array(cols).fill(0)
    const newPos = new Map<string, { x: number; y: number; w: number }>()

    const children = container.querySelectorAll<HTMLElement>('[data-note-id]')
    children.forEach(child => {
      const id = child.dataset.noteId!
      const minH = Math.min(...colHeights)
      const col = colHeights.indexOf(minH)
      const x = col * (actualColW + gap)
      const y = minH
      newPos.set(id, { x, y, w: actualColW })
      child.style.width = `${actualColW}px`
      child.style.position = 'absolute'
      // Use transition for smooth reflow animation
      child.style.transition = 'left 300ms cubic-bezier(0.25,1,0.5,1), top 300ms cubic-bezier(0.25,1,0.5,1), width 200ms ease'
      child.style.left = `${x}px`
      child.style.top = `${y}px`
      colHeights[col] = y + child.offsetHeight + gap
    })

    setPositions(newPos)
    setHeight(Math.max(...colHeights) - gap)
  }, [containerRef, gap, colWidth])

  useEffect(() => {
    recalc()
    const ro = new ResizeObserver(() => recalc())
    if (containerRef.current) ro.observe(containerRef.current)
    return () => ro.disconnect()
  }, [recalc, itemCount])

  return { positions, height, recalc }
}

// ─── Sortable card wrapper ───
function SortableNote({ note, onClick, onUpdate, listView, selected, onToggleSelect, selectionActive }: { note: Note; onClick: () => void; onUpdate: (n: Note) => void; listView: boolean; selected?: boolean; onToggleSelect?: (id: string) => void; selectionActive?: boolean }) {
  const { attributes, listeners, setNodeRef, transform, transition, isDragging } = useSortable({
    id: note.id,
    transition: { duration: 250, easing: 'cubic-bezier(0.25, 1, 0.5, 1)' },
  })
  return (
    <div ref={setNodeRef} data-note-id={note.id}
      style={{
        transform: transform ? `translate3d(${transform.x}px, ${transform.y}px, 0)` : undefined,
        transition: transition || 'all 250ms cubic-bezier(0.25, 1, 0.5, 1)',
        opacity: isDragging ? 0 : 1,
        zIndex: isDragging ? -1 : 'auto',
      }}
      {...attributes} {...listeners}>
      <NoteCard note={note} onClick={onClick} onUpdate={onUpdate} listView={listView} selected={selected} onSelect={onToggleSelect} selectionActive={selectionActive} />
    </div>
  )
}

// ─── Section with masonry or list ───
function Section({ notes, onSelect, onReorder, onUpdate, label, layout, selected, onToggleSelect }: {
  notes: Note[]; onSelect: (n: Note) => void; onReorder: (id: string, s: number) => void; onUpdate: (n: Note) => void; label?: string; layout: LayoutView; selected?: Set<string>; onToggleSelect?: (id: string) => void
}) {
  const sensors = useSensors(useSensor(PointerSensor, { activationConstraint: { distance: 8 } }))
  const [activeNote, setActiveNote] = useState<Note | null>(null)
  const containerRef = useRef<HTMLDivElement>(null)
  const isList = layout === 'list'
  const { height, recalc } = useMasonry(containerRef, notes.length, 16, 240)

  // Recalc after render
  useEffect(() => { if (!isList) requestAnimationFrame(recalc) }, [notes, isList, recalc])

  const handleDragStart = (e: DragStartEvent) => setActiveNote(notes.find(n => n.id === e.active.id) || null)
  const handleDragEnd = (e: DragEndEvent) => {
    setActiveNote(null)
    const { active, over } = e
    if (!over || active.id === over.id) return
    const oi = notes.findIndex(n => n.id === active.id), ni = notes.findIndex(n => n.id === over.id)
    if (oi < 0 || ni < 0) return
    let s: number
    if (ni === 0) s = notes[0].sortOrder + 1
    else if (ni === notes.length - 1) s = notes[notes.length - 1].sortOrder - 1
    else if (ni < oi) s = (notes[ni].sortOrder + notes[ni - 1].sortOrder) / 2
    else s = (notes[ni].sortOrder + notes[ni + 1].sortOrder) / 2
    onReorder(active.id as string, s)
  }

  return (
    <>
      {label && <div className="text-[11px] font-medium text-[#80868b] uppercase tracking-wider mb-2 mt-5 px-1">{label}</div>}
      <DndContext sensors={sensors} collisionDetection={closestCenter} onDragStart={handleDragStart} onDragEnd={handleDragEnd}>
        <SortableContext items={notes.map(n => n.id)} strategy={rectSortingStrategy}>
          {isList ? (
            <div className="max-w-2xl mx-auto space-y-2">
              {notes.map(n => (
                <div key={n.id} data-note-id={n.id}>
                  <NoteCard note={n} onClick={() => onSelect(n)} onUpdate={onUpdate} listView selected={selected?.has(n.id)} onSelect={onToggleSelect} selectionActive={!!selected?.size} />
                </div>
              ))}
            </div>
          ) : (
            <div ref={containerRef} className="relative" style={{ minHeight: height }} data-testid="masonry-grid">
              {notes.map(n => <SortableNote key={n.id} note={n} onClick={() => onSelect(n)} onUpdate={onUpdate} listView={false} selected={selected?.has(n.id)} onToggleSelect={onToggleSelect} selectionActive={!!selected?.size} />)}
            </div>
          )}
        </SortableContext>
        <DragOverlay dropAnimation={null}>
          {activeNote && (
            <div style={{ width: containerRef.current ? (containerRef.current.offsetWidth - (Math.max(1, Math.floor((containerRef.current.offsetWidth + 16) / 256)) - 1) * 16) / Math.max(1, Math.floor((containerRef.current.offsetWidth + 16) / 256)) : 240 }}
              className="shadow-2xl rotate-[2deg] opacity-95 pointer-events-none rounded-lg">
              <NoteCard note={activeNote} onClick={() => {}} onUpdate={() => {}} listView={isList} />
            </div>
          )}
        </DragOverlay>
      </DndContext>
    </>
  )
}

export function NoteGrid({ pinned, unpinned, onSelect, onReorder, onUpdate, layout, selected, onToggleSelect }: Props) {
  if (!pinned.length && !unpinned.length) return (
    <div className="text-center mt-32">
      <div className="text-6xl mb-4 opacity-20">💡</div>
      <div className="text-[#80868b] text-[14px]">Notes you add appear here</div>
    </div>
  )
  return (
    <div className="px-3 sm:px-4 pb-8">
      {pinned.length > 0 && <Section notes={pinned} onSelect={onSelect} onReorder={onReorder} onUpdate={onUpdate} label="Pinned" layout={layout} selected={selected} onToggleSelect={onToggleSelect} />}
      {pinned.length > 0 && unpinned.length > 0 && <div className="mt-4" />}
      {unpinned.length > 0 && <Section notes={unpinned} onSelect={onSelect} onReorder={onReorder} onUpdate={onUpdate} label={pinned.length > 0 ? 'Others' : undefined} layout={layout} selected={selected} onToggleSelect={onToggleSelect} />}
    </div>
  )
}
