import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import './index.css'
import App from './App'
import { clearAllData, getAllNotes, getAllLabels, upsertNote, addLabel } from './sync/yjs-sync'

// Expose for e2e tests
;(window as any).__clearKeepData = clearAllData
;(window as any).__keepSync = { upsertNote, addLabel, getAllNotes, getAllLabels }

// Export: downloads a JSON file
;(window as any).__exportData = () => {
  const data = { notes: getAllNotes(), labels: getAllLabels() }
  const json = JSON.stringify(data)
  const blob = new Blob([json], { type: 'application/json' })
  const a = document.createElement('a')
  a.href = URL.createObjectURL(blob)
  a.download = `keep-backup-${new Date().toISOString().slice(0, 10)}.json`
  a.click()
  URL.revokeObjectURL(a.href)
  console.log(`Exported ${data.notes.length} notes, ${data.labels.length} labels (${(json.length / 1024).toFixed(0)} KB)`)
}

// Import helper (called from Settings UI button)
;(window as any).__doImport = (data: any) => {
  data.labels?.forEach((l: string) => addLabel(l))
  data.notes?.forEach((n: any) => upsertNote(n))
  console.log(`Imported ${data.notes?.length ?? 0} notes, ${data.labels?.length ?? 0} labels`)
  location.reload()
}

createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <App />
  </StrictMode>,
)
