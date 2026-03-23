const URL_REGEX = /(https?:\/\/[^\s<>'")\]]+)/g

export function extractUrls(text: string): string[] {
  return [...new Set(text.match(URL_REGEX) || [])]
}

export function getDomain(url: string): string {
  try { return new URL(url).hostname.replace(/^www\./, '') } catch { return url }
}

export function getFavicon(url: string): string {
  try { return `https://www.google.com/s2/favicons?domain=${new URL(url).origin}&sz=32` } catch { return '' }
}

export function Linkified({ text, className }: { text: string; className?: string }) {
  const parts = text.split(URL_REGEX)
  return (
    <span className={className}>
      {parts.map((part, i) =>
        URL_REGEX.test(part)
          ? <a key={i} href={part} target="_blank" rel="noopener noreferrer" onClick={e => e.stopPropagation()} className="text-[#1a73e8] hover:underline break-all">{part}</a>
          : <span key={i}>{part}</span>
      )}
    </span>
  )
}

export function LinkPreview({ urls }: { urls: string[] }) {
  if (!urls.length) return null
  return (
    <div className="mt-2 space-y-1">
      {urls.slice(0, 2).map(url => (
        <a key={url} href={url} target="_blank" rel="noopener noreferrer" onClick={e => e.stopPropagation()}
          className="flex items-center gap-2 p-2 rounded-lg bg-black/[0.04] hover:bg-black/[0.08] transition-colors text-[12px] truncate">
          <img src={getFavicon(url)} alt="" className="w-4 h-4 flex-shrink-0 rounded-sm" onError={e => { (e.target as HTMLImageElement).style.display = 'none' }} />
          <span className="truncate text-[#3c4043]">{getDomain(url)}</span>
        </a>
      ))}
      {urls.length > 2 && <div className="text-[11px] text-[#80868b] pl-1">+{urls.length - 2} more</div>}
    </div>
  )
}
