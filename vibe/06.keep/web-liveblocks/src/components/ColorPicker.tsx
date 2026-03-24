import { NOTE_COLORS } from '../types/note'
import { CheckIcon } from './Icons'

interface Props {
  current: string
  onChange: (color: string) => void
  currentBg?: string
  onBgChange?: (bg: string) => void
}

const BACKGROUNDS = [
  { name: 'None', value: '' },
  { name: 'Groceries', value: 'groceries', emoji: '🛒' },
  { name: 'Food', value: 'food', emoji: '🍕' },
  { name: 'Music', value: 'music', emoji: '🎵' },
  { name: 'Travel', value: 'travel', emoji: '✈️' },
  { name: 'Celebration', value: 'celebration', emoji: '🎉' },
  { name: 'Nature', value: 'nature', emoji: '🌿' },
]

export function ColorPicker({ current, onChange, currentBg, onBgChange }: Props) {
  return (
    <div className="py-2.5 space-y-2">
      <div className="flex gap-1.5 flex-wrap">
        {NOTE_COLORS.map(c => (
          <button key={c.value} title={c.name} onClick={e => { e.stopPropagation(); onChange(c.value) }}
            className="w-8 h-8 rounded-full transition-all duration-150 hover:scale-110 active:scale-95 flex items-center justify-center"
            style={{ backgroundColor: c.value, border: current === c.value ? '2px solid #1a73e8' : c.value === '#ffffff' ? '2px solid #e0e0e0' : '2px solid transparent' }}>
            {current === c.value && <CheckIcon size={14} style={{ color: '#1a73e8' }} />}
          </button>
        ))}
      </div>
      {onBgChange && (
        <div className="flex gap-1.5 flex-wrap border-t border-black/[0.06] pt-2">
          {BACKGROUNDS.map(b => (
            <button key={b.name} title={b.name} onClick={e => { e.stopPropagation(); onBgChange(b.value) }}
              className="w-8 h-8 rounded-full transition-all duration-150 hover:scale-110 active:scale-95 flex items-center justify-center text-[14px]"
              style={{
                border: (currentBg || '') === b.value ? '2px solid #1a73e8' : '1px solid #e0e0e0',
                backgroundColor: '#fff',
              }}>
              {b.value === '' ? <span className="text-[#bdc1c6] text-[16px]">⊘</span> : b.emoji}
            </button>
          ))}
        </div>
      )}
    </div>
  )
}

// Helper to get CSS background for a note
export function getNoteBackground(bg: string, color: string): React.CSSProperties {
  if (!bg) return { backgroundColor: color }
  const emojis: Record<string, string> = {
    groceries: '🛒 🍎 🥦 🧀 🥚 🍞',
    food: '🍕 🍔 🌮 🍣 🍰 🍜',
    music: '🎵 🎸 🎹 🎤 🎶 🎷',
    travel: '✈️ 🏖️ 🗺️ 🧳 🌍 ⛰️',
    celebration: '🎉 🎂 🎈 🎁 🥳 🎊',
    nature: '🌿 🌸 🦋 🌻 🍃 🌺',
  }
  const text = emojis[bg] || ''
  if (!text) return { backgroundColor: color }
  const svg = `<svg xmlns='http://www.w3.org/2000/svg' width='240' height='44'><text x='4' y='34' font-size='28' opacity='.07' letter-spacing='8'>${text}</text></svg>`
  const encoded = encodeURIComponent(svg)
  return {
    backgroundColor: color,
    backgroundImage: `url("data:image/svg+xml,${encoded}")`,
    backgroundRepeat: 'repeat-x',
    backgroundPosition: 'bottom',
  }
}
