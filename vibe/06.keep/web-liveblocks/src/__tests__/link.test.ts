import { describe, it, expect } from 'vitest'
import { extractUrls, getDomain, getFavicon, shortenUrl } from '../components/LinkPreview'

describe('extractUrls', () => {
  it('http', () => expect(extractUrls('visit http://example.com')).toEqual(['http://example.com']))
  it('https', () => expect(extractUrls('see https://google.com/search?q=test')).toEqual(['https://google.com/search?q=test']))
  it('multiple', () => expect(extractUrls('a https://a.com b https://b.com')).toEqual(['https://a.com', 'https://b.com']))
  it('dedup', () => expect(extractUrls('https://a.com https://a.com')).toEqual(['https://a.com']))
  it('none', () => expect(extractUrls('no links')).toEqual([]))
  it('with path', () => expect(extractUrls('https://example.com/path/to/page')).toEqual(['https://example.com/path/to/page']))
  it('with port', () => expect(extractUrls('http://localhost:3000/api')).toEqual(['http://localhost:3000/api']))
  it('mixed text', () => {
    const urls = extractUrls('Check https://a.com and also http://b.org/page for info')
    expect(urls).toEqual(['https://a.com', 'http://b.org/page'])
  })
})

describe('getDomain', () => {
  it('strips www', () => expect(getDomain('https://www.google.com/path')).toBe('google.com'))
  it('keeps subdomain', () => expect(getDomain('https://docs.google.com')).toBe('docs.google.com'))
  it('invalid url', () => expect(getDomain('not-a-url')).toBe('not-a-url'))
  it('with port', () => expect(getDomain('http://localhost:3000')).toBe('localhost'))
})

describe('getFavicon', () => {
  it('returns google favicon url', () => {
    const url = getFavicon('https://example.com/page')
    expect(url).toContain('google.com/s2/favicons')
    expect(url).toContain('example.com')
  })
  it('invalid url returns empty', () => expect(getFavicon('bad')).toBe(''))
})

describe('shortenUrl', () => {
  it('domain only for root url', () => expect(shortenUrl('https://www.google.com')).toBe('google.com'))
  it('domain only for root with slash', () => expect(shortenUrl('https://example.com/')).toBe('example.com'))
  it('shows meaningful path segment', () => {
    const result = shortenUrl('https://example.com/products/blue-widget')
    expect(result).toBe('example.com › blue widget')
  })
  it('amazon product url shows product name', () => {
    const url = 'https://www.amazon.com/OREI-Extractor-Extract-Passthrough-HDA-929/dp/B0CDNYH3WN/ref=sr_1_4_sspa'
    const result = shortenUrl(url)
    expect(result).toContain('amazon.com')
    expect(result).toContain('OREI Extractor Extract Passthrough HDA 9')
    expect(result).not.toContain('B0CDNYH3WN')
  })
  it('skips ID-like segments', () => {
    const url = 'https://example.com/items/ABC12345678/details'
    const result = shortenUrl(url)
    expect(result).toContain('example.com')
    expect(result).toContain('details')
  })
  it('truncates very long segment', () => {
    const url = 'https://example.com/' + 'a'.repeat(60)
    const result = shortenUrl(url)
    expect(result.length).toBeLessThan(60)
    expect(result).toContain('…')
  })
  it('invalid url returns as-is', () => expect(shortenUrl('not-a-url')).toBe('not-a-url'))
  it('decodes percent-encoded path', () => {
    const result = shortenUrl('https://example.com/caf%C3%A9-menu')
    expect(result).toContain('café menu')
  })
})
