import { useState, useRef, useEffect, useCallback, forwardRef, useImperativeHandle } from 'react'
import { Note, NoteView, LayoutView, Settings } from '../types/note'
import { ColorPicker, getNoteBackground } from './ColorPicker'
import { Checklist, ChecklistHandle } from './Checklist'
import { LabelPicker } from './LabelPicker'
import { PinIcon, PaletteIcon, LabelIcon, CheckBoxIcon, TextIcon, ArchiveIcon, MenuIcon, LightbulbIcon, DeleteIcon, SearchIcon, GridViewIcon, ListViewIcon, SettingsIcon, EditIcon } from './Icons'
import { createChecklistItemData as createChecklistItem } from '../sync/yjs-sync'
import { parseHashTags, isEmptyHtml, stripHtml } from '../sync/note-utils'
import { RichEditor, RichEditorHandle, FormatBar } from './RichEditor'
import { HashTagSuggest } from './HashTagSuggest'

// ─── NoteInput ───

export interface NoteInputHandle { submit: () => void }

interface InputProps {
  onAdd: (partial?: Partial<Note>) => void; allLabels: string[]; onAddLabel: (label: string) => void
  expandAs: false | 'text' | 'checklist'; onExpandChange: (v: false | 'text' | 'checklist') => void
}

export const NoteInput = forwardRef<NoteInputHandle, InputProps>(({ onAdd, allLabels, onAddLabel, expandAs, onExpandChange }, ref) => {
  const [title, setTitle] = useState('')
  const [content, setContent] = useState('')
  const [color, setColor] = useState('#ffffff')
  const [pinned, setPinned] = useState(false)
  const [selectedLabels, setSelectedLabels] = useState<string[]>([])
  const [isChecklist, setIsChecklist] = useState(false)
  const [checklist, setChecklist] = useState<ReturnType<typeof createChecklistItem>[]>([])
  const [showColors, setShowColors] = useState(false)
  const [showLabels, setShowLabels] = useState(false)
  const [titleCursor, setTitleCursor] = useState(0)
  const wrapperRef = useRef<HTMLDivElement>(null)
  const contentRef = useRef<HTMLTextAreaElement>(null)
  const titleRef = useRef<HTMLInputElement>(null)
  const checklistRef = useRef<ChecklistHandle>(null)
  const richRef = useRef<RichEditorHandle>(null)
  const [richEditor, setRichEditor] = useState<any>(null)
  const expanded = expandAs !== false

  useEffect(() => {
    if (expandAs === 'checklist') { setIsChecklist(true); setChecklist([createChecklistItem()]) }
    if (expandAs) setTimeout(() => titleRef.current?.focus(), 50)
  }, [expandAs])

  useEffect(() => {
    if (!expanded) return
    const handler = (e: MouseEvent) => { if (wrapperRef.current && !wrapperRef.current.contains(e.target as Node)) submit() }
    document.addEventListener('mousedown', handler)
    return () => document.removeEventListener('mousedown', handler)
  }, [expanded, title, content, color, pinned, selectedLabels, isChecklist, checklist])

  const submit = useCallback(() => {
    const hasContent = title.trim() || !isEmptyHtml(content) || checklist.some(i => i.text.trim())
    if (hasContent) {
      const { clean: ct, tags: tt } = parseHashTags(title)
      const { clean: cc, tags: tc } = parseHashTags(isEmptyHtml(content) ? '' : content)
      const tags = [...new Set([...selectedLabels, ...tt, ...tc])]
      tags.forEach(t => { if (!allLabels.includes(t)) onAddLabel(t) })
      if (isChecklist) onAdd({ title: ct, type: 'checklist', checklist: checklist.filter(i => i.text.trim()), color, pinned, labels: tags, content: '' })
      else onAdd({ title: ct, content: isEmptyHtml(cc) ? '' : cc, color, pinned, labels: tags })
    }
    reset()
  }, [title, content, color, pinned, selectedLabels, isChecklist, checklist, allLabels])

  useImperativeHandle(ref, () => ({ submit }), [submit])

  const reset = () => {
    setTitle(''); setContent(''); setColor('#ffffff'); setPinned(false)
    setSelectedLabels([]); setIsChecklist(false); setChecklist([]); setShowColors(false); setShowLabels(false)
    onExpandChange(false)
  }
  const toggleLabel = (l: string) => setSelectedLabels(p => p.includes(l) ? p.filter(x => x !== l) : [...p, l])
  const hashTagSelect = (label: string, start: number, end: number) => {
    setTitle(t => t.slice(0, start) + t.slice(end))
    setTitleCursor(start)
    if (!selectedLabels.includes(label)) setSelectedLabels(p => [...p, label])
    setTimeout(() => { titleRef.current?.setSelectionRange(start, start) }, 0)
  }
  const hashTagCreate = (label: string, start: number, end: number) => {
    onAddLabel(label)
    hashTagSelect(label, start, end)
  }
  const focusTitle = () => { titleRef.current?.focus(); titleRef.current?.setSelectionRange(titleRef.current.value.length, titleRef.current.value.length) }
  const focusContent = () => { isChecklist ? checklistRef.current?.focusFirst() : richRef.current?.focus() }

  if (!expanded) return (
    <div className="max-w-xl mx-auto px-3 sm:px-4 mb-6">
      <div onClick={() => onExpandChange('text')} className="border border-[#e0e0e0] rounded-lg px-4 py-3 shadow-sm cursor-text text-[#80868b] hover:shadow-md transition-shadow flex justify-between items-center bg-white">
        <span className="text-[14px]">Take a note...</span>
        <button onClick={e => { e.stopPropagation(); onExpandChange('checklist') }} className="hover:bg-[#f1f3f4] rounded-full w-9 h-9 flex items-center justify-center transition-colors text-[#5f6368]" title="New list">
          <CheckBoxIcon size={18} />
        </button>
      </div>
    </div>
  )

  const tools: { t: string; icon: React.ReactNode; fn: () => void }[] = [
    { t: 'Color', icon: <PaletteIcon size={18} />, fn: () => { setShowColors(!showColors); setShowLabels(false) } },
    { t: 'Labels', icon: <LabelIcon size={18} />, fn: () => { setShowLabels(!showLabels); setShowColors(false) } },
    { t: isChecklist ? 'Text' : 'Checklist', icon: isChecklist ? <TextIcon size={18} /> : <CheckBoxIcon size={18} />, fn: () => {
      if (!isChecklist) { const text = stripHtml(content); const items = text.split('\n').filter(l => l.trim()).map(l => createChecklistItem(l)); setChecklist(items.length ? items : [createChecklistItem()]); setContent('') }
      else { setContent(checklist.map(i => i.text).filter(Boolean).join('\n')); setChecklist([]) }
      setIsChecklist(!isChecklist)
    }},
    { t: pinned ? 'Unpin' : 'Pin', icon: <PinIcon size={18} filled={pinned} />, fn: () => setPinned(!pinned) },
    { t: 'Archive', icon: <ArchiveIcon size={18} />, fn: () => { const h = title.trim() || content.trim() || checklist.some(i => i.text.trim()); if (h) onAdd({ title, content, color, pinned, labels: selectedLabels, archived: true }); reset() } },
  ]

  return (
    <div className="max-w-xl mx-auto px-3 sm:px-4 mb-6">
      <div ref={wrapperRef} className="border border-[#e0e0e0] rounded-lg shadow-md overflow-hidden" style={{ backgroundColor: color }}>
        <div className="p-4">
          <div className="flex items-center gap-2 mb-2">
            <input ref={titleRef} value={title} onChange={e => { setTitle(e.target.value); setTitleCursor(e.target.selectionStart ?? 0) }}
              onKeyUp={e => setTitleCursor((e.target as HTMLInputElement).selectionStart ?? 0)}
              onClick={e => setTitleCursor((e.target as HTMLInputElement).selectionStart ?? 0)}
              onKeyDown={e => { if (e.key === 'Enter' || e.key === 'Tab') { e.preventDefault(); focusContent() } if (e.key === 'Escape') { e.preventDefault(); submit() } }}
              placeholder="Title" className="flex-1 outline-none font-medium text-[15px] bg-transparent text-[#202124] placeholder:text-[#80868b]" />
            <HashTagSuggest text={title} cursorPos={titleCursor} allLabels={allLabels} anchorRef={titleRef}
              onSelect={hashTagSelect} onCreate={hashTagCreate} />
            <button onClick={() => setPinned(!pinned)} tabIndex={-1}
              className={`pin-btn w-9 h-9 flex items-center justify-center rounded-full hover:bg-black/[0.08] ${pinned ? 'pinned' : 'unpinned'}`}>
              <PinIcon size={18} filled={pinned} />
            </button>
          </div>
          {isChecklist ? <Checklist ref={checklistRef} items={checklist} onChange={setChecklist} onEscape={submit} onBackspaceAtStart={focusTitle} /> : (
            <RichEditor ref={richRef} content={content} onChange={setContent} placeholder="Take a note... (#tag to label)" onEditorReady={setRichEditor} onBackspaceAtStart={focusTitle} />
          )}
          {selectedLabels.length > 0 && (
            <div className="flex flex-wrap gap-1.5 mt-2">
              {selectedLabels.map(l => <span key={l} onClick={() => toggleLabel(l)} className="text-[11px] bg-black/[0.06] rounded-full px-2.5 py-[3px] cursor-pointer hover:bg-black/[0.12] text-[#3c4043] transition-colors">{l} ✕</span>)}
            </div>
          )}
        </div>
        {showColors && <div className="border-t border-black/[0.06] px-2"><ColorPicker current={color} onChange={setColor} /></div>}
        {showLabels && <div className="border-t border-black/[0.06] px-2"><LabelPicker allLabels={allLabels} selected={selectedLabels} onToggle={toggleLabel} onCreateLabel={l => { onAddLabel(l); toggleLabel(l) }} /></div>}
        {!isChecklist && <FormatBar editor={richEditor} />}
        <div className="flex items-center px-1.5 py-1 border-t border-black/[0.06]">
          <div className="flex">
            {tools.map(b => <button key={b.t} title={b.t} onClick={b.fn} tabIndex={-1} className="w-9 h-9 rounded-full flex items-center justify-center text-[#5f6368] hover:bg-black/[0.08] active:bg-black/[0.12] transition-colors">{b.icon}</button>)}
          </div>
          <div className="flex-1" />
          <button onClick={submit} tabIndex={-1} className="px-4 py-1.5 text-[13px] font-medium text-[#202124] hover:bg-black/[0.06] rounded-md transition-colors">Close</button>
        </div>
      </div>
    </div>
  )
})

// ─── Sidebar ───

interface SidebarProps {
  view: NoteView; activeLabel: string; labels: string[]
  onViewChange: (v: NoteView, label?: string) => void
  collapsed: boolean; onToggle: () => void
  onEditLabels?: () => void
}

export function Sidebar({ view, activeLabel, labels, onViewChange, collapsed, onToggle, onEditLabels }: SidebarProps) {
  return (
    <aside className={`hidden md:flex flex-col fixed left-0 top-[49px] bottom-0 bg-white z-30 transition-all duration-200 overflow-y-auto overflow-x-hidden ${collapsed ? 'w-[72px]' : 'w-[280px]'}`}
      onMouseEnter={() => { if (collapsed) onToggle() }}
      onMouseLeave={() => { if (!collapsed) onToggle() }}>
      <div className="py-2 flex-1">
        <SideItem icon={<LightbulbIcon size={20} />} label="Notes" active={view === 'notes'} collapsed={collapsed} onClick={() => onViewChange('notes')} />
        {labels.length > 0 && <>
          <div className={`border-t border-[#e0e0e0] my-1.5 ${collapsed ? 'mx-3' : 'mx-0'}`} />
          {labels.map(l => <SideItem key={l} icon={<LabelIcon size={20} />} label={l} active={view === 'label' && activeLabel === l} collapsed={collapsed} onClick={() => onViewChange('label', l)} />)}
          <SideItem icon={<EditIcon size={20} />} label="Edit labels" active={false} collapsed={collapsed} onClick={() => onEditLabels?.()} />
        </>}
        <div className={`border-t border-[#e0e0e0] my-1.5 ${collapsed ? 'mx-3' : 'mx-0'}`} />
        <SideItem icon={<ArchiveIcon size={20} />} label="Archive" active={view === 'archive'} collapsed={collapsed} onClick={() => onViewChange('archive')} />
        <SideItem icon={<DeleteIcon size={20} />} label="Trash" active={view === 'trash'} collapsed={collapsed} onClick={() => onViewChange('trash')} />
      </div>
    </aside>
  )
}

function SideItem({ icon, label, active, collapsed, onClick }: { icon: React.ReactNode; label: string; active: boolean; collapsed: boolean; onClick: () => void }) {
  return (
    <button onClick={onClick} title={collapsed ? label : undefined}
      className={`w-full text-left flex items-center transition-colors ${
        collapsed ? 'px-0 justify-center py-3' : 'pl-6 pr-4 py-2.5 rounded-r-full'
      } ${active ? 'bg-[#feefc3] font-medium text-[#202124]' : 'hover:bg-[#f1f3f4] text-[#3c4043]'}`}>
      <span className={`flex-shrink-0 ${active ? 'text-[#202124]' : 'text-[#5f6368]'}`}>{icon}</span>
      {!collapsed && <span className="ml-5 text-[13px] truncate">{label}</span>}
    </button>
  )
}

// ─── Mobile sidebar (overlay) ───

interface MobileSidebarProps extends SidebarProps { open: boolean; onClose: () => void }

export function MobileSidebar({ open, onClose, ...rest }: MobileSidebarProps) {
  if (!open) return null
  return (
    <div className="sidebar-overlay fixed inset-0 z-50 flex md:hidden" onClick={onClose}>
      <nav className="sidebar-panel w-72 bg-white h-full shadow-2xl overflow-y-auto" onClick={e => e.stopPropagation()}>
        <Sidebar {...rest} collapsed={false} onToggle={() => {}} />
      </nav>
    </div>
  )
}

// ─── Top bar ───

interface ToolbarProps {
  view: NoteView; activeLabel: string
  search: string; onSearchChange: (s: string) => void; onEmptyTrash: () => void
  searchRef: React.RefObject<HTMLInputElement | null>
  layout: LayoutView; onLayoutChange: (l: LayoutView) => void
  settings: Settings; onSettingsChange: (s: Settings) => void
  onMenuClick: () => void
  sidebarCollapsed: boolean
}

export function Toolbar({ view, activeLabel, search, onSearchChange, onEmptyTrash, searchRef, layout, onLayoutChange, settings, onSettingsChange, onMenuClick, sidebarCollapsed }: ToolbarProps) {
  const [showSettings, setShowSettings] = useState(false)
  const [searchFocused, setSearchFocused] = useState(false)

  return (
    <header className={`sticky top-0 z-40 bg-white border-b border-[#e0e0e0] px-2 sm:px-4 py-1.5 transition-shadow ${searchFocused ? 'shadow-md' : 'shadow-sm'}`}>
      <div className="flex items-center gap-2">
        <button onClick={onMenuClick} className="hover:bg-[#f1f3f4] rounded-full w-10 h-10 flex items-center justify-center transition-colors text-[#5f6368] flex-shrink-0">
          <MenuIcon size={22} />
        </button>
        <h1 className="text-[18px] font-normal text-[#5f6368] flex-shrink-0 hidden sm:block min-w-[60px]">
          {view === 'label' ? activeLabel : view === 'archive' ? 'Archive' : view === 'trash' ? 'Trash' : 'Keep'}
        </h1>
        <div className="flex-1 max-w-[720px]">
          <div className={`relative rounded-lg transition-all ${searchFocused ? 'bg-white shadow-lg ring-1 ring-[#e0e0e0]' : 'bg-[#f1f3f4]'}`}>
            <span className="absolute left-3 top-1/2 -translate-y-1/2 text-[#5f6368]"><SearchIcon size={20} /></span>
            <input ref={searchRef} type="text" value={search} onChange={e => onSearchChange(e.target.value)}
              onFocus={() => setSearchFocused(true)} onBlur={() => setSearchFocused(false)}
              placeholder="Search" className="w-full bg-transparent rounded-lg pl-11 pr-4 py-2.5 text-[14px] outline-none placeholder:text-[#80868b]" />
          </div>
        </div>
        {/* Spacer pushes right-side buttons to far right */}
        <div className="flex-1 min-w-0" />
        {view === 'trash' && <button onClick={onEmptyTrash} className="text-[13px] text-[#d93025] hover:bg-red-50 px-3 py-1.5 rounded-md transition-colors flex-shrink-0">Empty trash</button>}
        <button onClick={() => onLayoutChange(layout === 'grid' ? 'list' : 'grid')} title={layout === 'grid' ? 'List view' : 'Grid view'}
          className="w-10 h-10 rounded-full flex items-center justify-center text-[#5f6368] hover:bg-[#f1f3f4] transition-colors flex-shrink-0">
          {layout === 'grid' ? <ListViewIcon size={20} /> : <GridViewIcon size={20} />}
        </button>
        <div className="relative flex-shrink-0">
            <button onClick={() => setShowSettings(!showSettings)} title="Settings"
              className="w-10 h-10 rounded-full flex items-center justify-center text-[#5f6368] hover:bg-[#f1f3f4] transition-colors">
              <SettingsIcon size={20} />
            </button>
            {showSettings && (
              <>
                <div className="fixed inset-0 z-50" onClick={() => setShowSettings(false)} />
                <div className="absolute right-0 top-11 bg-white rounded-lg shadow-2xl border border-[#e0e0e0] w-72 py-1 z-50">
                  <div className="px-4 py-2.5 text-[13px] font-medium text-[#202124] border-b border-[#e0e0e0]">Settings</div>
                  <label className="flex items-center justify-between px-4 py-2.5 hover:bg-[#f1f3f4] cursor-pointer transition-colors">
                    <span className="text-[13px] text-[#3c4043]">New items at bottom</span>
                    <input type="checkbox" checked={settings.newItemPosition === 'bottom'} onChange={e => onSettingsChange({ ...settings, newItemPosition: e.target.checked ? 'bottom' : 'top' })} />
                  </label>
                  <label className="flex items-center justify-between px-4 py-2.5 hover:bg-[#f1f3f4] cursor-pointer transition-colors">
                    <span className="text-[13px] text-[#3c4043]">Move checked to bottom</span>
                    <input type="checkbox" checked={settings.moveCheckedToBottom} onChange={e => onSettingsChange({ ...settings, moveCheckedToBottom: e.target.checked })} />
                  </label>
                  <div className="border-t border-[#e0e0e0] mt-1 pt-1">
                    <button onClick={() => (window as any).__exportData?.()} className="w-full text-left px-4 py-2.5 hover:bg-[#f1f3f4] text-[13px] text-[#3c4043] transition-colors">Export notes (JSON)</button>
                    <button onClick={() => { const input = document.createElement('input'); input.type = 'file'; input.accept = '.json'; input.onchange = () => { const f = input.files?.[0]; if (!f) return; const r = new FileReader(); r.onload = () => { const d = JSON.parse(r.result as string); (window as any).__doImport?.(d); }; r.readAsText(f); }; input.click(); }} className="w-full text-left px-4 py-2.5 hover:bg-[#f1f3f4] text-[13px] text-[#3c4043] transition-colors">Import notes (JSON)</button>
                  </div>
                </div>
              </>
            )}
          </div>
      </div>
    </header>
  )
}
