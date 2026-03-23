import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import './index.css'
import App from './App'
import { clearAllData } from './sync/yjs-sync'

// Expose for e2e tests
;(window as any).__clearKeepData = clearAllData

createRoot(document.getElementById('root')!).render(
  <StrictMode>
    <App />
  </StrictMode>,
)
