import { useState, useRef, useEffect } from 'react'
import { Note, NoteView, Settings, DEFAULT_SETTINGS } from './types/note'
import { useNotes } from './hooks/useNotes'
import { NoteInput, NoteInputHandle, Toolbar, Sidebar, MobileSidebar } from './components/Toolbar'
import { NoteGrid } from './components/NoteGrid'
import { NoteEditor } from './components/NoteEditor'
import { EditLabelsDialog } from './components/EditLabelsDialog'
import { SettingsContext } from './hooks/useSettings'

export default function App() {
  const { notes, labels, loading, syncStatus, save, add, remove, restore, emptyTrash, getFiltered, addLabel, removeLabel, renameLabel } = useNotes()
  const [view, setView] = useState<NoteView>('notes')
  const [activeLabel, setActiveLabel] = useState('')
  const [search, setSearch] = useState('')
  const [editing, setEditing] = useState<Note | null>(null)
  const [inputExpanded, setInputExpanded] = useState<false | 'text' | 'checklist'>(false)
  const [sidebarCollapsed, setSidebarCollapsed] = useState(true)
  const [mobileNavOpen, setMobileNavOpen] = useState(false)
  const [editLabelsOpen, setEditLabelsOpen] = useState(false)
  const [settings, setSettings] = useState<Settings>(() => {
    try { return { ...DEFAULT_SETTINGS, ...JSON.parse(localStorage.getItem('keep-settings') || '{}') } } catch { return DEFAULT_SETTINGS }
  })
  const searchRef = useRef<HTMLInputElement>(null)
  const noteInputRef = useRef<NoteInputHandle>(null)

  const updateSettings = (s: Settings) => { setSettings(s); localStorage.setItem('keep-settings', JSON.stringify(s)) }
  const handleViewChange = (v: NoteView, label?: string) => { setView(v); setActiveLabel(label || '') }

  useEffect(() => {
    const handler = (e: KeyboardEvent) => {
      const tag = (e.target as HTMLElement).tagName
      const isTyping = tag === 'INPUT' || tag === 'TEXTAREA'
      if (e.key === 'Escape') {
        e.preventDefault()
        if (editing) { setEditing(null); return }
        if (inputExpanded) { noteInputRef.current?.submit(); return }
        if (search) { setSearch(''); searchRef.current?.blur(); return }
        return
      }
      if (isTyping || editing) return
      if (e.key === 'c') { e.preventDefault(); setInputExpanded('text') }
      else if (e.key === 'l') { e.preventDefault(); setInputExpanded('checklist') }
      else if (e.key === '/') { e.preventDefault(); searchRef.current?.focus() }
    }
    document.addEventListener('keydown', handler)
    return () => document.removeEventListener('keydown', handler)
  }, [editing, search, inputExpanded])

  if (loading) return <div className="flex items-center justify-center h-screen text-[#80868b]">Loading...</div>

  const { pinned, unpinned } = getFiltered(view, search, activeLabel)

  return (
    <SettingsContext.Provider value={settings}>
    <div className="min-h-screen bg-white">
      <Toolbar
        view={view} activeLabel={activeLabel}
        search={search} onSearchChange={setSearch} onEmptyTrash={emptyTrash} searchRef={searchRef}
        layout={settings.layout} onLayoutChange={l => updateSettings({ ...settings, layout: l })}
        settings={settings} onSettingsChange={updateSettings}
        onMenuClick={() => {
          // Desktop: toggle sidebar collapse. Mobile: open overlay
          if (window.innerWidth >= 768) setSidebarCollapsed(p => !p)
          else setMobileNavOpen(true)
        }}
        sidebarCollapsed={sidebarCollapsed}
      />

      {/* Desktop sidebar */}
      <Sidebar view={view} activeLabel={activeLabel} labels={labels} onViewChange={handleViewChange}
        collapsed={sidebarCollapsed} onToggle={() => setSidebarCollapsed(p => !p)} onEditLabels={() => setEditLabelsOpen(true)} />

      {/* Mobile sidebar overlay */}
      <MobileSidebar open={mobileNavOpen} onClose={() => setMobileNavOpen(false)}
        view={view} activeLabel={activeLabel} labels={labels}
        onViewChange={(v, l) => { handleViewChange(v, l); setMobileNavOpen(false) }}
        collapsed={false} onToggle={() => {}} onEditLabels={() => { setMobileNavOpen(false); setEditLabelsOpen(true) }} />

      {/* Edit labels dialog */}
      {editLabelsOpen && (
        <EditLabelsDialog labels={labels} onAdd={addLabel} onRemove={removeLabel} onRename={renameLabel} onClose={() => setEditLabelsOpen(false)} />
      )}

      {/* Main content — offset by sidebar width on desktop */}
      <main className={`pt-4 pb-8 transition-all duration-200 md:ml-[72px] ${!sidebarCollapsed ? 'md:ml-[280px]' : ''}`}>
        <div className="max-w-[1200px] mx-auto">
          {(view === 'notes' || view === 'label') && (
            <NoteInput ref={noteInputRef} expandAs={inputExpanded} onExpandChange={setInputExpanded}
              allLabels={labels} onAddLabel={addLabel}
              onAdd={partial => {
                const extra = view === 'label' && activeLabel ? { labels: [activeLabel] } : {}
                add({ ...extra, ...partial })
              }} />
          )}
          {view === 'trash' && unpinned.length > 0 && (
            <div className="text-center text-[13px] text-[#80868b] mb-4">Notes in trash are deleted after 30 days</div>
          )}
          <NoteGrid pinned={pinned} unpinned={unpinned} layout={settings.layout}
            onSelect={note => { if (view === 'trash') { if (confirm('Restore this note?')) restore(note.id) } else setEditing(note) }}
            onReorder={(id, s) => { const n = notes.find(x => x.id === id); if (n) save({ ...n, sortOrder: s }) }}
            onUpdate={save} />
        </div>
      </main>

      {editing && (
        <NoteEditor note={editing} labels={labels}
          onSave={u => { save(u); setEditing(u) }} onClose={() => setEditing(null)}
          onDelete={() => { remove(editing.id); setEditing(null) }} onAddLabel={addLabel} />
      )}

      {/* Sync status */}
      <div className="fixed bottom-3 left-3 flex items-center gap-1.5 text-[11px] text-[#80868b] select-none" title={`Sync: ${syncStatus}`}>
        <span className={`w-2 h-2 rounded-full ${syncStatus === 'connected' ? 'bg-green-400' : syncStatus === 'connecting' ? 'bg-yellow-400 animate-pulse' : 'bg-red-300'}`} />
        {syncStatus !== 'connected' && <span>{syncStatus === 'connecting' ? 'Syncing...' : 'Offline'}</span>}
      </div>
    </div>
    </SettingsContext.Provider>
  )
}
