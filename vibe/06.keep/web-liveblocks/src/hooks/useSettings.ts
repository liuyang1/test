import { createContext, useContext } from 'react'
import { Settings, DEFAULT_SETTINGS } from '../types/note'

export const SettingsContext = createContext<Settings>(DEFAULT_SETTINGS)
export const useSettings = () => useContext(SettingsContext)
