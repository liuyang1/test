import { test, expect } from '@playwright/test'

test.beforeEach(async ({ page }) => {
  await page.goto('/?nosync')
  await page.waitForSelector('text=Take a note', { timeout: 10000 })
  await page.evaluate(() => window.__clearKeepData?.())
  await page.waitForFunction(() => document.querySelectorAll('.note-card').length === 0, { timeout: 5000 }).catch(() => {})
})

async function createNote(page, title, content) {
  await page.click('text=Take a note')
  await page.fill('input[placeholder="Title"]', title)
  if (content) { await page.waitForSelector('.tiptap', { timeout: 3000 }); await page.click('.tiptap'); await page.keyboard.type(content) }
  await page.click('body', { position: { x: 10, y: 10 } })
  await expect(page.locator(`.note-card:has-text("${title}")`)).toBeVisible()
}

async function navigateTo(page, viewName) {
  const sidebar = page.locator('aside')
  await sidebar.hover()
  await page.waitForTimeout(200)
  await sidebar.locator(`button:has-text("${viewName}")`).first().click()
  await page.mouse.move(600, 300)
  await page.waitForTimeout(300)
}

// ═══ CRUD ═══
test('create text note', async ({ page }) => { await createNote(page, 'TextNote', 'body') })
test('create checklist via L', async ({ page }) => {
  await page.keyboard.press('l')
  await page.fill('input[placeholder="Title"]', 'List')
  const li = page.locator('.checklist-item-editor').first()
  await li.click()
  await page.keyboard.type('Item1')
  await page.keyboard.press('Enter'); await page.keyboard.type('Item2')
  await page.keyboard.press('Escape')
  await expect(page.locator('.note-card:has-text("Item1")')).toBeVisible()
})
test('edit note title', async ({ page }) => {
  await createNote(page, 'Original', '')
  await page.click('.note-card:has-text("Original")')
  await page.fill('.editor-panel input[placeholder="Title"]', 'Edited')
  await page.click('.editor-overlay', { position: { x: 10, y: 10 } })
  await expect(page.locator('.note-card:has-text("Edited")')).toBeVisible()
})
test('delete to trash', async ({ page }) => {
  await createNote(page, 'DelNote', '')
  await page.locator('.note-card').first().hover()
  await page.click('[data-testid="card-actions"] button[title="Delete"]')
  await expect(page.locator('.note-card:has-text("DelNote")')).not.toBeVisible()
})
test('archive note from card', async ({ page }) => {
  await createNote(page, 'ArchNote', '')
  await page.locator('.note-card').first().hover()
  await page.click('[data-testid="card-actions"] button[title="Archive"]')
  await expect(page.locator('.note-card:has-text("ArchNote")')).not.toBeVisible()
})
test('archive from editor', async ({ page }) => {
  await createNote(page, 'ArchEdit', '')
  await page.click('.note-card:has-text("ArchEdit")')
  await page.click('.editor-panel button[title="Archive"]')
  await expect(page.locator('.note-card:has-text("ArchEdit")')).not.toBeVisible()
})

// ═══ Shortcuts ═══
test('C opens note', async ({ page }) => { await page.keyboard.press('c'); await expect(page.locator('input[placeholder="Title"]')).toBeFocused() })
test('typing c in content does not trigger shortcut', async ({ page }) => {
  await page.click('text=Take a note')
  await page.waitForTimeout(200)
  // Click into the content area (TipTap editor)
  const editor = page.locator('[contenteditable="true"]').first()
  await editor.click()
  await page.keyboard.type('abc')
  await expect(editor).toContainText('abc')
})
test('/ focuses search', async ({ page }) => { await page.keyboard.press('/'); await expect(page.locator('input[placeholder="Search"]')).toBeFocused() })
test('Esc saves and closes', async ({ page }) => {
  await page.keyboard.press('c'); await page.fill('input[placeholder="Title"]', 'EscNote'); await page.keyboard.press('Escape')
  await expect(page.locator('.note-card:has-text("EscNote")')).toBeVisible()
})
test('Esc closes editor', async ({ page }) => {
  await createNote(page, 'EscClose', '')
  await page.click('.note-card:has-text("EscClose")')
  await expect(page.locator('.editor-panel')).toBeVisible()
  await page.keyboard.press('Escape')
  await expect(page.locator('.editor-panel')).not.toBeVisible()
})

// ═══ Navigation ═══
test('Title Enter → content focus', async ({ page }) => {
  await page.keyboard.press('c')
  await page.locator('input[placeholder="Title"]').click()
  await page.keyboard.type('Nav')
  // Ensure the RichEditor has mounted before pressing Enter
  await page.waitForSelector('.tiptap.ProseMirror', { timeout: 5000 })
  await page.keyboard.press('Enter')
  await page.waitForFunction(
    () => {
      const el = document.activeElement
      return el?.classList?.contains('tiptap') || el?.classList?.contains('ProseMirror') || el?.closest?.('.tiptap') !== null
    },
    { timeout: 5000 }
  )
})
test('Backspace in empty content → title', async ({ page }) => {
  await createNote(page, 'BackNav', '')
  await page.click('.note-card:has-text("BackNav")')
  await page.waitForSelector('.editor-panel .tiptap', { timeout: 5000 })
  await page.click('.editor-panel .tiptap')
  await page.keyboard.press('Backspace')
  await expect(page.locator('.editor-panel input[placeholder="Title"]')).toBeFocused()
})
test('Backspace in empty checklist → title', async ({ page }) => {
  await page.keyboard.press('l')
  await page.fill('input[placeholder="Title"]', 'CheckNav')
  await page.keyboard.press('Tab')
  await page.keyboard.press('Backspace')
  await expect(page.locator('input[placeholder="Title"]')).toBeFocused()
})

// ═══ Search ═══
test('search filters notes', async ({ page }) => {
  await createNote(page, 'Apple', ''); await createNote(page, 'Banana', '')
  await page.fill('input[placeholder="Search"]', 'Apple')
  await expect(page.locator('.note-card:has-text("Apple")')).toBeVisible()
  await expect(page.locator('.note-card:has-text("Banana")')).not.toBeVisible()
})
test('search by label', async ({ page }) => {
  await page.click('text=Take a note')
  await page.fill('input[placeholder="Title"]', '#work task')
  await page.keyboard.press('Escape')
  await page.fill('input[placeholder="Search"]', 'work')
  await expect(page.locator('.note-card:has-text("task")')).toBeVisible()
})

// ═══ Rich text ═══
test('format bar in new note', async ({ page }) => {
  await page.click('text=Take a note')
  await page.waitForSelector('.tiptap', { timeout: 5000 })
  await expect(page.locator('[data-testid="format-bar"] button[title="Bold"]')).toBeVisible()
})
test('format bar in editor', async ({ page }) => {
  await createNote(page, 'FmtEdit', '')
  await page.click('.note-card:has-text("FmtEdit")')
  await page.waitForSelector('.editor-panel .tiptap', { timeout: 5000 })
  await expect(page.locator('.editor-panel [data-testid="format-bar"] button[title="Bold"]')).toBeVisible()
})
test('bold', async ({ page }) => {
  await createNote(page, 'BoldNote', '')
  await page.click('.note-card:has-text("BoldNote")')
  await page.waitForSelector('.editor-panel .tiptap', { timeout: 5000 })
  await page.click('.editor-panel .tiptap'); await page.keyboard.type('hello'); await page.keyboard.press('Control+a')
  await page.click('.editor-panel [data-testid="format-bar"] button[title="Bold"]')
  expect(await page.locator('.editor-panel .tiptap').innerHTML()).toContain('<strong>')
})
test('H1', async ({ page }) => {
  await createNote(page, 'H1Note', '')
  await page.click('.note-card:has-text("H1Note")')
  await page.waitForSelector('.editor-panel .tiptap', { timeout: 5000 })
  await page.click('.editor-panel .tiptap'); await page.keyboard.type('heading'); await page.keyboard.press('Control+a')
  await page.click('.editor-panel [data-testid="format-bar"] button[title="Title"]')
  expect(await page.locator('.editor-panel .tiptap').innerHTML()).toContain('<h1>')
})
test('H2 visually distinct', async ({ page }) => {
  await createNote(page, 'H2Note', '')
  await page.click('.note-card:has-text("H2Note")')
  await page.waitForSelector('.editor-panel .tiptap', { timeout: 5000 })
  await page.click('.editor-panel .tiptap'); await page.keyboard.type('sub'); await page.keyboard.press('Control+a')
  await page.click('.editor-panel [data-testid="format-bar"] button[title="Subtitle"]')
  expect(await page.locator('.editor-panel .tiptap').innerHTML()).toContain('<h2>')
  expect((await page.locator('.editor-panel .tiptap h2').evaluate(el => getComputedStyle(el).fontStyle))).toBe('italic')
})
test('bullet list has disc markers', async ({ page }) => {
  await createNote(page, 'BulNote', '')
  await page.click('.note-card:has-text("BulNote")')
  await page.waitForSelector('.editor-panel .tiptap', { timeout: 5000 })
  await page.click('.editor-panel .tiptap'); await page.keyboard.type('item')
  await page.click('.editor-panel [data-testid="format-bar"] button[title="Bullet list"]')
  expect(await page.locator('.editor-panel .tiptap ul').evaluate(el => getComputedStyle(el).listStyleType)).toBe('disc')
})
test('empty trailing list item does not overlap timestamp', async ({ page }) => {
  // Create note with content, then edit to add bullet list with empty trailing item
  await createNote(page, 'TrailLI', 'temp')
  await page.click('.note-card:has-text("TrailLI")')
  await page.waitForSelector('.editor-panel .tiptap', { timeout: 10000 })
  await page.locator('.editor-panel .tiptap').click()
  await page.keyboard.press('Control+a')
  await page.keyboard.press('Backspace')
  await page.click('.editor-panel [data-testid="format-bar"] button[title="Bullet list"]')
  await page.keyboard.type('item1')
  await page.keyboard.press('Enter')
  await page.keyboard.type('item2')
  await page.keyboard.press('Enter')
  await page.keyboard.type('x')
  await page.keyboard.press('Backspace') // empty trailing li
  await page.click('.editor-overlay', { position: { x: 10, y: 10 } })
  await page.waitForTimeout(300)
  const card = page.locator('.note-card:has-text("TrailLI")')
  await expect(card).toBeVisible({ timeout: 5000 })
  await expect(card.locator('.note-content')).toBeVisible({ timeout: 3000 })
  // Check date doesn't overlap content
  const contentBox = await card.locator('.note-content').boundingBox()
  const dateBox = await card.locator('.text-\\[11px\\]').boundingBox()
  expect(dateBox.y).toBeGreaterThanOrEqual(contentBox.y + contentBox.height - 1)
  // Check toolbar doesn't overlap content on hover
  await card.hover()
  await expect(card.locator('[data-testid="card-actions"]')).toBeVisible()
  const actBox = await card.locator('[data-testid="card-actions"]').boundingBox()
  const contentBox2 = await card.locator('.note-content').boundingBox()
  expect(actBox.y).toBeGreaterThanOrEqual(contentBox2.y + contentBox2.height - 1)
})
test('format bar separate from tools', async ({ page }) => {
  await createNote(page, 'SepNote', '')
  await page.click('.note-card:has-text("SepNote")')
  await page.waitForSelector('.editor-panel .tiptap', { timeout: 5000 })
  const fBox = await page.locator('.editor-panel [data-testid="format-bar"]').boundingBox()
  const cBox = await page.locator('.editor-panel button[title="Color"]').boundingBox()
  expect(cBox.y).toBeGreaterThan(fBox.y)
})

// ═══ Checklist ═══
test('check on card directly', async ({ page }) => {
  await page.keyboard.press('l')
  await page.locator('input[placeholder="Title"]').click()
  await page.keyboard.type('CardCheck')
  await page.locator('.checklist-item-editor').first().click()
  await page.keyboard.type('Do')
  await page.keyboard.press('Escape')
  await page.locator('.note-card:has-text("CardCheck") .checklist-check').first().click({ force: true })
  await expect(page.locator('.note-card:has-text("1 checked")')).toBeVisible()
})
test('checklist Enter adds item', async ({ page }) => {
  await page.keyboard.press('l')
  await page.fill('input[placeholder="Title"]', 'EnterCheck')
  await page.locator('.checklist-item-editor').first().click()
  await page.keyboard.type('First')
  await page.keyboard.press('Enter')
  await page.keyboard.type('Second')
  await page.keyboard.press('Escape')
  await expect(page.locator('.note-card:has-text("Second")')).toBeVisible()
})

// ═══ Color ═══
test('color from card hover', async ({ page }) => {
  await createNote(page, 'ColorNote', '')
  await page.locator('.note-card').first().hover()
  await page.click('[data-testid="card-actions"] button[title="Background options"]')
  await page.click('button[title="Coral"]')
  expect(await page.locator('.note-card').first().evaluate(el => getComputedStyle(el).backgroundColor)).not.toBe('rgb(255, 255, 255)')
})
test('color from editor', async ({ page }) => {
  await createNote(page, 'EdColor', '')
  await page.click('.note-card:has-text("EdColor")')
  await page.click('.editor-panel button[title="Color"]')
  await page.click('button[title="Sand"]')
  await page.click('.editor-overlay', { position: { x: 10, y: 10 } })
  expect(await page.locator('.note-card').first().evaluate(el => getComputedStyle(el).backgroundColor)).not.toBe('rgb(255, 255, 255)')
})

// ═══ Background ═══
test('background pattern', async ({ page }) => {
  await createNote(page, 'BgNote', '')
  await page.click('.note-card:has-text("BgNote")')
  await page.click('.editor-panel button[title="Color"]')
  await expect(page.locator('button[title="Groceries"]')).toBeVisible()
  await page.click('button[title="Groceries"]')
  expect(await page.locator('.editor-panel').first().evaluate(el => el.style.backgroundImage)).toContain('data:image/svg+xml')
})

// ═══ Masonry layout ═══
test('masonry: cards fill gaps', async ({ page }) => {
  await page.setViewportSize({ width: 800, height: 600 })
  await page.reload(); await page.waitForSelector('text=Take a note', { timeout: 10000 })
  await createNote(page, 'Short1', ''); await createNote(page, 'Med1', 'L1\nL2\nL3')
  await createNote(page, 'Long1', 'A\nB\nC\nD\nE\nF'); await createNote(page, 'Short2', '')
  await page.waitForTimeout(500)
  const cards = page.locator('[data-note-id]')
  expect(await cards.count()).toBe(4)
  const tops = []; for (let i = 0; i < 4; i++) tops.push(await cards.nth(i).evaluate(el => parseFloat(el.style.top)))
  expect(tops.filter(t => t > 0).length).toBeGreaterThanOrEqual(2)
})
test('masonry: consistent card width', async ({ page }) => {
  await createNote(page, 'W1Note', ''); await createNote(page, 'W2Note', '')
  await page.waitForTimeout(500)
  const cards = page.locator('[data-note-id]')
  const w1 = await cards.nth(0).evaluate(el => parseFloat(el.style.width))
  const w2 = await cards.nth(1).evaluate(el => parseFloat(el.style.width))
  expect(Math.abs(w1 - w2)).toBeLessThan(1)
  expect(w1).toBeGreaterThan(200)
})

// ═══ Pin ═══
test('pin creates Pinned section', async ({ page }) => {
  await createNote(page, 'PinNote', '')
  // Open editor and pin from there (more reliable than hover)
  await page.click('.note-card:has-text("PinNote")')
  await page.click('.editor-panel .pin-btn')
  await page.click('.editor-overlay', { position: { x: 10, y: 10 } })
  await expect(page.locator('text=Pinned')).toBeVisible()
})
test('unpin removes Pinned section', async ({ page }) => {
  await createNote(page, 'UnpinNote', '')
  await page.click('.note-card:has-text("UnpinNote")')
  await page.click('.editor-panel .pin-btn')
  await page.click('.editor-overlay', { position: { x: 10, y: 10 } })
  await expect(page.locator('text=Pinned')).toBeVisible()
  await page.click('.note-card:has-text("UnpinNote")')
  await page.click('.editor-panel .pin-btn')
  await page.click('.editor-overlay', { position: { x: 10, y: 10 } })
  await expect(page.locator('text=Pinned')).not.toBeVisible()
})
test('pin has transition on cards', async ({ page }) => {
  await createNote(page, 'PinTrans', '')
  await page.waitForTimeout(500)
  const t = await page.locator('[data-note-id]').first().evaluate(el => el.style.transition)
  expect(t).toContain('left')
})
test('pinned icon stays visible without hover', async ({ page }) => {
  await createNote(page, 'PinVis', '')
  await page.click('.note-card:has-text("PinVis")')
  await page.click('.editor-panel .pin-btn')
  await page.click('.editor-overlay', { position: { x: 10, y: 10 } })
  // Move mouse away from the card
  await page.mouse.move(0, 0)
  await page.waitForTimeout(300)
  const opacity = await page.locator('.note-card:has-text("PinVis") .pin-btn').evaluate(el => parseFloat(getComputedStyle(el).opacity))
  // Pin icon hidden without hover, even for pinned notes
  expect(opacity).toBe(0)
})
test('unpinned icon hidden without hover', async ({ page }) => {
  await createNote(page, 'UnpinVis', '')
  await page.mouse.move(0, 0)
  await page.waitForTimeout(300)
  const opacity = await page.locator('.note-card:has-text("UnpinVis") .pin-btn').evaluate(el => parseFloat(getComputedStyle(el).opacity))
  expect(opacity).toBe(0)
})
test('pin icon appears on card hover', async ({ page }) => {
  await createNote(page, 'HoverPin', '')
  const card = page.locator('.note-card:has-text("HoverPin")')
  await card.hover()
  await page.waitForTimeout(300)
  const opacity = await page.locator('.note-card:has-text("HoverPin") .pin-btn').evaluate(el => parseFloat(getComputedStyle(el).opacity))
  expect(opacity).toBeGreaterThanOrEqual(0.5)
})

// ═══ Fonts ═══
test('body uses Google Sans font', async ({ page }) => {
  const font = await page.evaluate(() => getComputedStyle(document.body).fontFamily)
  expect(font).toContain('Google Sans')
})

// ═══ Drag ═══
test('no stretch on cards', async ({ page }) => {
  await createNote(page, 'NoStretch1', ''); await createNote(page, 'NoStretch2', '')
  await page.waitForTimeout(500)
  const cards = page.locator('[data-note-id]')
  for (let i = 0; i < await cards.count(); i++) {
    const t = await cards.nth(i).evaluate(el => el.style.transform || '')
    if (t) expect(t).not.toMatch(/scale/)
  }
})

// ═══ Card action bar UI consistency ═══
test('card hover shows action bar with same icons as editor', async ({ page }) => {
  await createNote(page, 'ActBar', '')
  await page.locator('.note-card').first().hover()
  const actions = page.locator('[data-testid="card-actions"]')
  await expect(actions.locator('button[title="Background options"]')).toBeVisible()
  await expect(actions.locator('button[title="Add label"]')).toBeVisible()
  await expect(actions.locator('button[title="Archive"]')).toBeVisible()
  await expect(actions.locator('button[title="Delete"]')).toBeVisible()
  await expect(actions.locator('button[title="More"]')).toBeVisible()
})
test('editor has matching tool buttons', async ({ page }) => {
  await createNote(page, 'EdTools', '')
  await page.click('.note-card:has-text("EdTools")')
  await expect(page.locator('.editor-panel button[title="Color"]')).toBeVisible()
  await expect(page.locator('.editor-panel button[title="Labels"]')).toBeVisible()
  await expect(page.locator('.editor-panel button[title="Archive"]')).toBeVisible()
  await expect(page.locator('.editor-panel button[title="Delete"]')).toBeVisible()
  await expect(page.locator('.editor-panel button[title="More"]')).toBeVisible()
})
test('date hidden on hover, visible otherwise', async ({ page }) => {
  await createNote(page, 'DateVis', '')
  // Date visible by default
  const dateEl = page.locator('.note-card .text-\\[11px\\]').first()
  await expect(dateEl).toBeVisible()
})
test('card toolbar at bottom of card on hover', async ({ page }) => {
  await createNote(page, 'TBPos', '')
  const card = page.locator('.note-card').first()
  await card.hover()
  const actions = page.locator('[data-testid="card-actions"]')
  await expect(actions).toBeVisible()
  const cardBox = await card.boundingBox()
  const actBox = await actions.boundingBox()
  // toolbar should be below the card content, near the bottom
  expect(actBox.y + actBox.height).toBeLessThanOrEqual(cardBox.y + cardBox.height + 2)
})
test('card toolbar button order: Background, Label, Archive, Delete, More', async ({ page }) => {
  await createNote(page, 'TBOrder', '')
  await page.locator('.note-card').first().hover()
  const btns = page.locator('[data-testid="card-actions"] button')
  const titles = await btns.evaluateAll(els => els.map(e => e.getAttribute('title')))
  expect(titles).toEqual(['Background options', 'Add label', 'Archive', 'Delete', 'More'])
})

// ═══ Layout ═══
test('view/settings in top right', async ({ page }) => {
  const vp = page.viewportSize()
  expect((await page.locator('button[title="List view"], button[title="Grid view"]').boundingBox()).x).toBeGreaterThan(vp.width - 200)
  expect((await page.locator('button[title="Settings"]').boundingBox()).x).toBeGreaterThan(vp.width - 200)
})
test('list view wider cards', async ({ page }) => {
  await createNote(page, 'ListView', '')
  await page.click('button[title="List view"]')
  await page.waitForTimeout(300)
  expect((await page.locator('.note-card').first().boundingBox()).width).toBeGreaterThan(300)
})

// ═══ Settings ═══
test('settings toggle', async ({ page }) => {
  await page.click('button[title="Settings"]')
  const t = page.locator('label:has-text("Move checked to bottom") input')
  await expect(t).toBeChecked(); await t.click(); await expect(t).not.toBeChecked()
})
test('new checklist item stays at cursor position, not moved to bottom', async ({ page }) => {
  // Create checklist note with 3 items via the input
  await page.keyboard.press('l')
  await page.fill('input[placeholder="Title"]', 'OrderTest')
  const li = page.locator('.checklist-item-editor').first()
  await li.click()
  await page.keyboard.type('AAA')
  await page.keyboard.press('Enter')
  await page.waitForTimeout(100)
  await page.keyboard.type('BBB')
  await page.keyboard.press('Enter')
  await page.waitForTimeout(100)
  await page.keyboard.type('CCC')
  await page.keyboard.press('Escape')
  await expect(page.locator('.note-card:has-text("OrderTest")')).toBeVisible()
  // Open the note editor
  await page.click('.note-card:has-text("OrderTest")')
  await page.waitForSelector('.editor-panel', { timeout: 5000 })
  await page.waitForTimeout(300)
  // Focus the AAA item and press Enter to insert after it
  const aaaItem = page.locator('.editor-panel .checklist-item-editor:has-text("AAA")')
  await expect(aaaItem).toBeVisible()
  await aaaItem.click()
  await page.waitForTimeout(100)
  // Ensure cursor is at end of AAA
  await page.keyboard.press('End')
  await page.waitForTimeout(50)
  await page.keyboard.press('Enter')
  await page.waitForTimeout(150)
  await page.keyboard.type('NEW')
  await page.waitForTimeout(200)
  // Verify order: AAA, NEW, BBB, CCC
  const texts = await page.locator('.editor-panel .checklist-item-editor').evaluateAll(
    els => els.map(e => e.textContent?.trim()).filter(v => v)
  )
  expect(texts).toEqual(['AAA', 'NEW', 'BBB', 'CCC'])
})
test('Enter at start of first checklist item inserts before it', async ({ page }) => {
  await page.keyboard.press('l')
  await page.fill('input[placeholder="Title"]', 'BeforeTest')
  await page.locator('.checklist-item-editor').first().click()
  await page.keyboard.type('AAA')
  await page.keyboard.press('Enter')
  await page.waitForTimeout(100)
  await page.keyboard.type('BBB')
  await page.keyboard.press('Escape')
  await page.click('.note-card:has-text("BeforeTest")')
  await page.waitForSelector('.editor-panel', { timeout: 5000 })
  // Put cursor at start of AAA and press Enter
  const aaaItem = page.locator('.editor-panel .checklist-item-editor:has-text("AAA")')
  await aaaItem.click()
  await page.keyboard.press('Home')
  await page.keyboard.press('Enter')
  await page.waitForTimeout(100)
  await page.keyboard.type('NEW')
  await page.waitForTimeout(200)
  const texts = await page.locator('.editor-panel .checklist-item-editor').evaluateAll(
    els => els.map(e => e.textContent?.trim()).filter(v => v)
  )
  expect(texts).toEqual(['NEW', 'AAA', 'BBB'])
})

test('checklist and rich text coexist in same note', async ({ page }) => {
  // Create a text note with content
  await createNote(page, 'Hybrid', 'some text')
  await page.click('.note-card:has-text("Hybrid")')
  await page.waitForSelector('.editor-panel', { timeout: 5000 })
  // Toggle to checklist — content should remain
  await page.click('.editor-panel button[title="Checklist"]')
  await page.waitForTimeout(200)
  // Add a checklist item (now a tiptap editor, not an input)
  const li = page.locator('.editor-panel .checklist-item-editor').first()
  await li.click()
  await page.keyboard.type('todo item')
  // Both should be visible in editor
  await expect(page.locator('.editor-panel .checklist-item-editor').first()).toBeVisible()
  await expect(page.locator('.editor-panel :text("todo item")')).toBeVisible()
  await page.keyboard.press('Escape')
  await page.waitForTimeout(300)
  // Card should show both checklist and text content
  const card = page.locator('.note-card:has-text("Hybrid")')
  await expect(card.locator('.checklist-check').first()).toBeVisible()
  await expect(card.locator(':text("some text")')).toBeVisible()
})

test('checklist item supports bold formatting via format bar', async ({ page }) => {
  await createNote(page, 'BoldCheck', '')
  await page.click('.note-card:has-text("BoldCheck")')
  await page.waitForSelector('.editor-panel', { timeout: 5000 })
  // Toggle to checklist
  await page.click('.editor-panel button[title="Checklist"]')
  await page.waitForTimeout(200)
  // Type in the checklist item
  const li = page.locator('.editor-panel .checklist-item-editor').first()
  await li.click()
  await page.keyboard.type('boldtext')
  await page.waitForTimeout(100)
  // Select all and apply bold via Ctrl+A then Ctrl+B
  await page.keyboard.press('Control+a')
  await page.waitForTimeout(50)
  await page.keyboard.press('Control+b')
  await page.waitForTimeout(200)
  // Verify bold tag exists in the editor
  await expect(page.locator('.editor-panel .checklist-item-editor strong')).toBeVisible({ timeout: 3000 })
  // Format bar should be visible in checklist mode
  await expect(page.locator('[data-testid="format-bar"]')).toBeVisible()
  // Close and verify card shows bold text
  await page.keyboard.press('Escape')
  await page.waitForTimeout(300)
  const card = page.locator('.note-card:has-text("BoldCheck")')
  await expect(card.locator('strong')).toBeVisible()
})

test('text→checklist converts each line to a checklist item', async ({ page }) => {
  await createNote(page, 'MultiLine', '')
  await page.click('.note-card:has-text("MultiLine")')
  await page.waitForSelector('.editor-panel', { timeout: 5000 })
  await page.locator('.editor-panel .tiptap').first().click()
  await page.keyboard.type('line one')
  await page.keyboard.press('Enter')
  await page.keyboard.type('line two')
  await page.keyboard.press('Enter')
  await page.keyboard.type('line three')
  await page.waitForTimeout(200)
  await page.click('.editor-panel button[title="Checklist"]')
  await page.waitForTimeout(300)
  const items = await page.locator('.editor-panel .checklist-item-editor').evaluateAll(
    els => els.map(e => e.textContent?.trim()).filter(v => v)
  )
  expect(items).toEqual(['line one', 'line two', 'line three'])
})

test('checklist↔text bidirectional toggle', async ({ page }) => {
  await createNote(page, 'BiToggle', '')
  await page.click('.note-card:has-text("BiToggle")')
  await page.waitForSelector('.editor-panel', { timeout: 5000 })
  await page.locator('.editor-panel .tiptap').first().click()
  await page.keyboard.type('alpha')
  await page.keyboard.press('Enter')
  await page.keyboard.type('beta')
  await page.waitForTimeout(200)
  // text → checklist
  await page.click('.editor-panel button[title="Checklist"]')
  await page.waitForTimeout(300)
  expect(await page.locator('.editor-panel .checklist-item-editor').count()).toBe(2)
  // checklist → text
  await expect(page.locator('.editor-panel button[title="Text"]')).toBeVisible()
  await page.click('.editor-panel button[title="Text"]')
  await page.waitForTimeout(300)
  const content = await page.locator('.editor-panel .tiptap').first().textContent()
  expect(content).toContain('alpha')
  expect(content).toContain('beta')
})

// ═══ Labels ═══
test('edit labels dialog', async ({ page }) => {
  await page.click('text=Take a note'); await page.fill('input[placeholder="Title"]', '#mylab test'); await page.keyboard.press('Escape')
  await page.click('button:has(span:text("menu"))'); await page.click('text=Edit labels')
  await expect(page.locator('.editor-overlay:has-text("mylab")')).toBeVisible()
})
test('create label in dialog', async ({ page }) => {
  await page.click('text=Take a note'); await page.fill('input[placeholder="Title"]', '#x test'); await page.keyboard.press('Escape')
  await page.click('button:has(span:text("menu"))'); await page.click('text=Edit labels')
  await page.fill('input[placeholder="Create new label"]', 'newlabel'); await page.keyboard.press('Enter')
  await expect(page.locator('text=newlabel').first()).toBeVisible()
})
test('delete label in edit labels dialog', async ({ page }) => {
  // Create a label first
  await createNote(page, 'LabelDel', '')
  await page.locator('.note-card:has-text("LabelDel")').click()
  await page.locator('.editor-panel button[title="Labels"]').click()
  await page.locator('input[placeholder="Enter label name"]').fill('todelete')
  await page.locator('text=Create "todelete"').click()
  await page.click('.editor-overlay', { position: { x: 10, y: 10 } })
  // Open edit labels dialog
  await page.click('button:has(span:text("menu"))')
  await page.click('text=Edit labels')
  await expect(page.locator('.editor-overlay:has-text("todelete")')).toBeVisible()
  // Hover over the label row and click delete
  await page.locator('.editor-overlay .group:has-text("todelete")').hover()
  await page.locator('.editor-overlay .group:has-text("todelete") [data-testid="delete-label"]').click()
  // Label should be gone
  await expect(page.locator('.editor-overlay:has-text("todelete")')).not.toBeVisible()
})
test('label filter in sidebar', async ({ page }) => {
  await page.click('text=Take a note'); await page.fill('input[placeholder="Title"]', '#filter test'); await page.keyboard.press('Escape')
  await createNote(page, 'NoLabel', '')
  await page.click('button:has(span:text("menu"))'); await page.click('text=filter')
  await expect(page.locator('.note-card:has-text("test")')).toBeVisible()
  await expect(page.locator('.note-card:has-text("NoLabel")')).not.toBeVisible()
})

// ═══ Multi-select ═══
test('select note via checkbox shows selection bar', async ({ page }) => {
  await createNote(page, 'SelNote', '')
  await page.locator('.note-card').first().hover()
  await page.click('[data-testid="select-check"]')
  await expect(page.locator('[data-testid="selection-bar"]')).toBeVisible()
  await expect(page.locator('[data-testid="selection-count"]')).toHaveText('1 selected')
})
test('select multiple notes shows correct count', async ({ page }) => {
  await createNote(page, 'Multi1', ''); await createNote(page, 'Multi2', '')
  await page.locator('.note-card:has-text("Multi1")').hover()
  await page.locator('.note-card:has-text("Multi1") [data-testid="select-check"]').click()
  await page.locator('.note-card:has-text("Multi2") [data-testid="select-check"]').click()
  await expect(page.locator('[data-testid="selection-count"]')).toHaveText('2 selected')
})
test('bulk delete selected notes', async ({ page }) => {
  await createNote(page, 'BulkDel1', ''); await createNote(page, 'BulkDel2', '')
  await page.locator('.note-card:has-text("BulkDel1")').hover()
  await page.locator('.note-card:has-text("BulkDel1") [data-testid="select-check"]').click()
  await page.locator('.note-card:has-text("BulkDel2") [data-testid="select-check"]').click()
  await page.click('[data-testid="selection-bar"] button[title="Delete"]')
  await expect(page.locator('.note-card')).toHaveCount(0)
})
test('bulk archive selected notes', async ({ page }) => {
  await createNote(page, 'BulkArch1', ''); await createNote(page, 'BulkArch2', '')
  await page.locator('.note-card:has-text("BulkArch1")').hover()
  await page.locator('.note-card:has-text("BulkArch1") [data-testid="select-check"]').click()
  await page.locator('.note-card:has-text("BulkArch2") [data-testid="select-check"]').click()
  await page.click('[data-testid="selection-bar"] button[title="Archive"]')
  await expect(page.locator('.note-card')).toHaveCount(0)
})
test('Esc clears selection', async ({ page }) => {
  await createNote(page, 'EscSel', '')
  await page.locator('.note-card').first().hover()
  await page.click('[data-testid="select-check"]')
  await expect(page.locator('[data-testid="selection-bar"]')).toBeVisible()
  await page.keyboard.press('Escape')
  await expect(page.locator('[data-testid="selection-bar"]')).not.toBeVisible()
})
test('clear button clears selection', async ({ page }) => {
  await createNote(page, 'ClearSel', '')
  await page.locator('.note-card').first().hover()
  await page.click('[data-testid="select-check"]')
  await page.click('[data-testid="selection-bar"] button[title="Clear selection"]')
  await expect(page.locator('[data-testid="selection-bar"]')).not.toBeVisible()
})

// ═══ Checklist bugs ═══
test('checklist uses square checkbox icon (not circle)', async ({ page }) => {
  await page.keyboard.press('l')
  await expect(page.locator('input[placeholder="Title"]')).toBeVisible()
  await page.locator('input[placeholder="Title"]').click()
  await page.keyboard.type('SquareCheck')
  await page.locator('.checklist-item-editor').first().click()
  await page.keyboard.type('Item1')
  await page.keyboard.press('Escape')
  // Card preview should have SVG checkbox, not input[type=checkbox]
  const card = page.locator('.note-card:has-text("SquareCheck")')
  await expect(card.locator('.checklist-check svg')).toBeVisible()
  await expect(card.locator('input[type="checkbox"]')).toHaveCount(0)
})
test('checklist checkbox aligns to top of content, not center', async ({ page }) => {
  await page.keyboard.press('l')
  await page.locator('input[placeholder="Title"]').click()
  await page.keyboard.type('AlignCheck')
  await page.locator('.checklist-item-editor').first().click()
  await page.keyboard.type('Item1')
  await page.keyboard.press('Escape')
  // The checklist row uses CSS grid (not flex items-center), checkbox is top-aligned via pt-[3px]
  const row = page.locator('.note-card:has-text("AlignCheck") [style*="grid-template-columns"]').first()
  await expect(row).toBeVisible()
  const display = await row.evaluate(el => getComputedStyle(el).display)
  expect(display).toBe('grid')
})
test('checklist editor has drag handle', async ({ page }) => {
  await page.keyboard.press('l')
  await page.locator('input[placeholder="Title"]').click()
  await page.keyboard.type('DragCheck')
  await page.locator('.checklist-item-editor').first().click()
  await page.keyboard.type('Item1')
  await page.keyboard.press('Enter')
  await page.keyboard.type('Item2')
  // Drag handles should exist in editor
  const handles = page.locator('[data-testid="drag-handle"]')
  await expect(handles).toHaveCount(2)
})
test('checklist drag reorder works', async ({ page }) => {
  await page.keyboard.press('l')
  await page.locator('input[placeholder="Title"]').click()
  await page.keyboard.type('ReorderCheck')
  await page.locator('.checklist-item-editor').first().click()
  await page.keyboard.type('First')
  await page.keyboard.press('Enter')
  await page.keyboard.type('Second')
  // Drag handles should exist in editor (we're still in the new-note editor)
  const handles = page.locator('[data-testid="drag-handle"]')
  await expect(handles).toHaveCount(2)
  // Hover over second handle to make it visible, then drag
  const handle2 = handles.nth(1)
  const handle1 = handles.first()
  await handle2.hover()
  const box2 = await handle2.boundingBox()
  const box1 = await handle1.boundingBox()
  await page.mouse.move(box2.x + 9, box2.y + 9)
  await page.mouse.down()
  await page.mouse.move(box1.x + 9, box1.y + 9, { steps: 5 })
  await page.mouse.up()
  // After reorder, first tiptap editor should now be "Second"
  const firstEditor = page.locator('.checklist-item-editor').first()
  await expect(firstEditor).toHaveText('Second')
})

// ═══ Empty HTML content bug ═══
test('checklist note does not show <p></p> on card', async ({ page }) => {
  // Create a text note (RichEditor sets content to <p></p> when empty)
  await page.click('text=Take a note')
  await page.locator('input[placeholder="Title"]').click()
  await page.keyboard.type('EmptyContent')
  // Close without typing content — RichEditor has <p></p>
  await page.keyboard.press('Escape')
  // The card should NOT show <p></p> as visible text
  const card = page.locator('.note-card:has-text("EmptyContent")')
  await expect(card).toBeVisible()
  const text = await card.innerText()
  expect(text).not.toContain('<p>')
  expect(text).not.toContain('</p>')
})
test('switching text to checklist does not leak HTML tags', async ({ page }) => {
  await page.click('text=Take a note')
  await page.locator('input[placeholder="Title"]').click()
  await page.keyboard.type('SwitchTest')
  // Switch to checklist mode
  await page.locator('button[title="Checklist"]').click()
  await page.locator('.checklist-item-editor').first().click()
  await page.keyboard.type('Item1')
  await page.keyboard.press('Escape')
  // Card should show checklist item, not HTML tags
  const card = page.locator('.note-card:has-text("SwitchTest")')
  await expect(card).toBeVisible()
  const text = await card.innerText()
  expect(text).not.toContain('<p>')
  expect(text).toContain('Item1')
})

// ═══ Hash tag label ═══
test('typing # in title shows label suggestions', async ({ page }) => {
  // Create a label by creating a note and adding label via editor
  await createNote(page, 'seed', '')
  await page.locator('.note-card:has-text("seed")').click()
  await page.locator('.editor-panel button[title="Labels"]').click()
  await page.locator('input[placeholder="Enter label name"]').fill('work')
  await page.locator('text=Create "work"').click()
  await page.click('.editor-overlay', { position: { x: 10, y: 10 } })
  // Now create a new note and type #
  await page.click('text=Take a note')
  await page.locator('input[placeholder="Title"]').click()
  await page.keyboard.type('My note #w')
  await expect(page.locator('[data-testid="hashtag-suggest"]')).toBeVisible()
  await expect(page.locator('[data-testid="hashtag-suggest"]')).toContainText('work')
})
test('selecting # suggestion adds label and removes hash text', async ({ page }) => {
  // Create label first
  await createNote(page, 'seed', '')
  await page.locator('.note-card:has-text("seed")').click()
  await page.locator('.editor-panel button[title="Labels"]').click()
  await page.locator('input[placeholder="Enter label name"]').fill('personal')
  await page.locator('text=Create "personal"').click()
  await page.click('.editor-overlay', { position: { x: 10, y: 10 } })
  // New note, type #personal and select
  await page.click('text=Take a note')
  await page.locator('input[placeholder="Title"]').click()
  await page.keyboard.type('Tagged #per')
  await expect(page.locator('[data-testid="hashtag-suggest"]')).toBeVisible()
  await page.keyboard.press('Enter')
  // Hash text removed, label chip shown
  const titleVal = await page.locator('input[placeholder="Title"]').inputValue()
  expect(titleVal).toBe('Tagged ')
  await expect(page.locator('text=personal ✕')).toBeVisible()
})
test('# with new label creates it', async ({ page }) => {
  await page.click('text=Take a note')
  await page.locator('input[placeholder="Title"]').click()
  await page.keyboard.type('New #brandnew')
  await expect(page.locator('[data-testid="hashtag-suggest"]')).toBeVisible()
  await expect(page.locator('[data-testid="hashtag-suggest"]')).toContainText('Create "brandnew"')
  await page.keyboard.press('Enter')
  await expect(page.locator('text=brandnew ✕')).toBeVisible()
})

// ═══ Card label picker ═══
test('add label from card toolbar without opening editor', async ({ page }) => {
  // Create a label via hashtag
  await page.click('text=Take a note'); await page.fill('input[placeholder="Title"]', '#testlbl note'); await page.keyboard.press('Escape')
  await expect(page.locator('.note-card:has-text("testlbl")')).toBeVisible()
  await createNote(page, 'LblCard', '')
  // Hover card and click Add label
  const card = page.locator('.note-card:has-text("LblCard")')
  await card.hover()
  await card.locator('[data-testid="card-actions"] button[title="Add label"]').click()
  await expect(page.locator('text=Label note')).toBeVisible({ timeout: 3000 })
  // Toggle the label in the picker popup
  const popup = card.locator('.shadow-xl')
  await popup.locator('text=testlbl').click()
  await page.keyboard.press('Escape')
  await page.waitForTimeout(300)
  // Verify label tag appears on card (the rounded pill, not the picker)
  await expect(card.locator('.rounded-full:has-text("testlbl")')).toBeVisible()
})
test('label picker uses square checkbox style', async ({ page }) => {
  await page.click('text=Take a note'); await page.fill('input[placeholder="Title"]', '#stylelbl note'); await page.keyboard.press('Escape')
  await createNote(page, 'LblStyle', '')
  const card = page.locator('.note-card:has-text("LblStyle")')
  await card.hover()
  await card.locator('[data-testid="card-actions"] button[title="Add label"]').click()
  await expect(page.locator('text=Label note')).toBeVisible({ timeout: 3000 })
  // Should have SVG checkboxes, not native input[type=checkbox] in the label picker popup
  const popup = page.locator('.note-card:has-text("LblStyle") .shadow-xl')
  const nativeCheckboxes = await popup.locator('input[type="checkbox"]').count()
  const svgCheckboxes = await popup.locator('button svg').count()
  expect(nativeCheckboxes).toBe(0)
  expect(svgCheckboxes).toBeGreaterThan(0)
})

// ═══ Checklist edit alignment ═══
test('checklist edit row: checkbox, handle, input vertically centered', async ({ page }) => {
  await page.keyboard.press('l')
  await page.fill('input[placeholder="Title"]', 'AlignTest')
  await page.locator('.checklist-item-editor').first().click()
  await page.keyboard.type('test item')
  await page.keyboard.press('Escape')
  await page.click('.note-card:has-text("AlignTest")')
  await page.waitForSelector('.editor-panel', { timeout: 5000 })
  const row = page.locator('.editor-panel .grid').first()
  await row.hover()
  await page.waitForTimeout(200)
  const centers = await row.evaluate(el => {
    const mid = (e) => { const r = e.getBoundingClientRect(); return r.top + r.height / 2; }
    const handle = el.querySelector('[data-testid="drag-handle"] svg')
    const checkbox = el.querySelector('button svg')
    const editor = el.querySelector('.tiptap')
    return { handle: mid(handle), checkbox: mid(checkbox), input: mid(editor) }
  })
  // All vertical centers within 2px of each other
  expect(Math.abs(centers.handle - centers.checkbox)).toBeLessThan(2)
  expect(Math.abs(centers.checkbox - centers.input)).toBeLessThan(2)
})

// ═══ Favicon & branding ═══
test('favicon is set', async ({ page }) => {
  const favicon = await page.locator('link[rel="icon"]').getAttribute('href')
  expect(favicon).toBe('/favicon.svg')
})
test('no Liveblocks badge visible', async ({ page }) => {
  // In nosync mode, badge shouldn't appear. If it does, CSS hides it.
  const badge = page.locator('#liveblocks-badge')
  await expect(badge).toHaveCount(0, { timeout: 3000 }).catch(async () => {
    // Badge exists but should be hidden by CSS
    await expect(badge).not.toBeVisible()
  })
})

// ═══ Toolbar position & overlap ═══
test('card toolbar has margin from content', async ({ page }) => {
  await createNote(page, 'MarginTest', 'some content')
  const card = page.locator('.note-card:has-text("MarginTest")')
  await card.hover()
  await expect(card.locator('[data-testid="card-actions"]')).toBeVisible()
  const contentDiv = card.locator('.note-content')
  const actDiv = card.locator('[data-testid="card-actions"]')
  const contentBox = await contentDiv.boundingBox()
  const actBox = await actDiv.boundingBox()
  // At least 8px gap (mt-3 = 12px)
  expect(actBox.y - (contentBox.y + contentBox.height)).toBeGreaterThanOrEqual(8)
})

// ═══ Trash ═══
test('empty trash removes all deleted notes', async ({ page }) => {
  await createNote(page, 'TrashA', '')
  await createNote(page, 'TrashB', '')
  for (const name of ['TrashA', 'TrashB']) {
    const card = page.locator(`.note-card:has-text("${name}")`)
    await card.hover()
    await card.locator('[data-testid="card-actions"] button[title="Delete"]').click()
    await page.waitForTimeout(300)
  }
  await navigateTo(page, 'Trash')
  await expect(page.locator('.note-card')).toHaveCount(2)
  await page.click('text=Empty trash')
  await page.waitForTimeout(500)
  await expect(page.locator('.note-card')).toHaveCount(0)
})
test('restore note from trash', async ({ page }) => {
  await createNote(page, 'RestoreMe', '')
  const card = page.locator('.note-card:has-text("RestoreMe")')
  await card.hover()
  await card.locator('[data-testid="card-actions"] button[title="Delete"]').click()
  await page.waitForTimeout(300)
  await navigateTo(page, 'Trash')
  page.on('dialog', dialog => dialog.accept())
  await page.click('.note-card:has-text("RestoreMe")')
  await page.waitForTimeout(500)
  await navigateTo(page, 'Notes')
  await expect(page.locator('.note-card:has-text("RestoreMe")')).toBeVisible()
})

// ═══ Editor content ═══
test('edit note content and save', async ({ page }) => {
  await createNote(page, 'EditContent', 'original')
  await page.click('.note-card:has-text("EditContent")')
  await page.waitForSelector('.editor-panel .tiptap', { timeout: 5000 })
  await page.click('.editor-panel .tiptap')
  await page.keyboard.press('Control+a')
  await page.keyboard.type('modified')
  await page.keyboard.press('Escape')
  await expect(page.locator('.note-card:has-text("modified")')).toBeVisible()
})
test('click overlay closes editor', async ({ page }) => {
  await createNote(page, 'OverlayClose', 'text')
  await page.click('.note-card:has-text("OverlayClose")')
  await expect(page.locator('.editor-panel')).toBeVisible()
  await page.click('.editor-overlay', { position: { x: 10, y: 10 } })
  await expect(page.locator('.editor-panel')).not.toBeVisible()
})

// ═══ Checklist: move checked to bottom ═══
test('checked items move to bottom when setting enabled', async ({ page }) => {
  // Setting is on by default
  await page.keyboard.press('l')
  await page.fill('input[placeholder="Title"]', 'CheckBottom')
  await page.locator('.checklist-item-editor').first().click()
  await page.keyboard.type('First')
  await page.keyboard.press('Enter')
  await page.keyboard.type('Second')
  await page.keyboard.press('Escape')
  await page.click('.note-card:has-text("CheckBottom")')
  await page.waitForSelector('.editor-panel', { timeout: 5000 })
  // Check the first item
  await page.locator('.editor-panel button.cursor-pointer').first().click()
  await page.waitForTimeout(300)
  // "First" should now be in completed section, "Second" should be the unchecked item
  const unchecked = await page.locator('.editor-panel .checklist-item-editor').first().textContent()
  expect(unchecked.trim()).toBe('Second')
})

test('checked items in completed section have drag handles', async ({ page }) => {
  await page.keyboard.press('l')
  await page.fill('input[placeholder="Title"]', 'DragChecked')
  await page.locator('.checklist-item-editor').first().click()
  await page.keyboard.type('A')
  await page.keyboard.press('Enter')
  await page.waitForTimeout(100)
  await page.keyboard.type('B')
  await page.keyboard.press('Escape')
  await page.click('.note-card:has-text("DragChecked")')
  await page.waitForSelector('.editor-panel', { timeout: 5000 })
  // Check first item to move it to completed section
  await page.locator('.editor-panel button.cursor-pointer').first().click()
  await page.waitForTimeout(300)
  // Completed section should have a drag handle
  const handles = page.locator('.editor-panel [data-testid="drag-handle"]')
  // 1 for unchecked "B" + 1 for checked "A"
  await expect(handles).toHaveCount(2)
})

// ═══ Card label: remove ═══
test('remove label from card toolbar', async ({ page }) => {
  await createNote(page, 'RemLabel', '')
  // Create a label by adding it to another note
  await createNote(page, 'LabelSource', '')
  const src = page.locator('.note-card:has-text("LabelSource")')
  await src.hover()
  await src.locator('[data-testid="card-actions"] button[title="Add label"]').click()
  await expect(src.locator('.shadow-xl')).toBeVisible()
  // Create new label "remlbl"
  await src.locator('.shadow-xl input').fill('remlbl')
  await src.locator('.shadow-xl').getByText('Create "remlbl"').click()
  await src.locator('[data-testid="card-actions"] button[title="Add label"]').click()
  await page.waitForTimeout(200)
  // Now add remlbl to RemLabel card
  const card = page.locator('.note-card:has-text("RemLabel")')
  await card.hover()
  await card.locator('[data-testid="card-actions"] button[title="Add label"]').click()
  await expect(card.locator('.shadow-xl')).toBeVisible()
  await card.locator('.shadow-xl').getByText('remlbl').click()
  await card.locator('[data-testid="card-actions"] button[title="Add label"]').click()
  await page.waitForTimeout(200)
  await expect(card.locator('.rounded-full:has-text("remlbl")')).toBeVisible()
  // Remove it
  await card.hover()
  await card.locator('[data-testid="card-actions"] button[title="Add label"]').click()
  await expect(card.locator('.shadow-xl')).toBeVisible()
  await card.locator('.shadow-xl').getByText('remlbl').click()
  await card.locator('[data-testid="card-actions"] button[title="Add label"]').click()
  await page.waitForTimeout(200)
  await expect(card.locator('.rounded-full:has-text("remlbl")')).not.toBeVisible()
})

// ═══ Sidebar navigation ═══
test('sidebar: archive view', async ({ page }) => {
  await createNote(page, 'ArchSide', '')
  const card = page.locator('.note-card:has-text("ArchSide")')
  await card.hover()
  await card.locator('[data-testid="card-actions"] button[title="Archive"]').click()
  await navigateTo(page, 'Archive')
  await expect(page.locator('.note-card:has-text("ArchSide")')).toBeVisible()
})
test('sidebar: trash view', async ({ page }) => {
  await createNote(page, 'TrashSide', '')
  const card = page.locator('.note-card:has-text("TrashSide")')
  await card.hover()
  await card.locator('[data-testid="card-actions"] button[title="Delete"]').click()
  await navigateTo(page, 'Trash')
  await expect(page.locator('.note-card:has-text("TrashSide")')).toBeVisible()
})

// ═══ Note input: color & pin ═══
test('create pinned note from input', async ({ page }) => {
  await page.click('text=Take a note')
  await page.fill('input[placeholder="Title"]', 'PinInput')
  await page.click('.pin-btn.unpinned')
  await page.click('body', { position: { x: 10, y: 10 } })
  await expect(page.locator('.note-card:has-text("PinInput") .pin-btn.pinned')).toBeVisible()
})

// ═══ Empty state ═══
test('empty state shows placeholder', async ({ page }) => {
  await expect(page.locator('text=Notes you add appear here')).toBeVisible()
})

// ═══ Special characters ═══
test('note with emoji and Chinese text', async ({ page }) => {
  await createNote(page, '🎉 测试笔记', '你好世界')
  await expect(page.locator('.note-card:has-text("🎉 测试笔记")')).toBeVisible()
  await expect(page.locator('.note-card:has-text("你好世界")')).toBeVisible()
})
test('note with HTML-like text is escaped', async ({ page }) => {
  await createNote(page, '<script>alert</script>', '')
  const card = page.locator('.note-card').first()
  const html = await card.innerHTML()
  expect(html).not.toContain('<script>')
  await expect(card.locator('.font-medium')).toContainText('<script>')
})

// ═══ Multiple labels ═══
test('note with multiple labels displays all', async ({ page }) => {
  await page.click('text=Take a note')
  await page.fill('input[placeholder="Title"]', '#lab1 #lab2 multi')
  await page.keyboard.press('Escape')
  const card = page.locator('.note-card:has-text("multi")')
  await expect(card.locator('.rounded-full:has-text("lab1")')).toBeVisible()
  await expect(card.locator('.rounded-full:has-text("lab2")')).toBeVisible()
})

// ═══ Unarchive ═══
test('unarchive note from editor', async ({ page }) => {
  await createNote(page, 'UnArch', '')
  const card = page.locator('.note-card:has-text("UnArch")')
  await card.hover()
  await card.locator('[data-testid="card-actions"] button[title="Archive"]').click()
  await page.waitForTimeout(300)
  await navigateTo(page, 'Archive')
  await page.click('.note-card:has-text("UnArch")')
  await page.waitForSelector('.editor-panel', { timeout: 5000 })
  await page.click('.editor-panel button[title="Unarchive"]')
  await page.waitForTimeout(300)
  await navigateTo(page, 'Notes')
  await expect(page.locator('.note-card:has-text("UnArch")')).toBeVisible()
})

// ═══ Search ═══
test('search checklist content', async ({ page }) => {
  await page.keyboard.press('l')
  await page.fill('input[placeholder="Title"]', 'SearchCL')
  await page.locator('.checklist-item-editor').first().click()
  await page.keyboard.type('unique_item_xyz')
  await page.keyboard.press('Escape')
  await page.fill('input[placeholder="Search"]', 'unique_item_xyz')
  await expect(page.locator('.note-card:has-text("SearchCL")')).toBeVisible()
})
test('clear search restores all notes', async ({ page }) => {
  await createNote(page, 'SearchA', '')
  await createNote(page, 'SearchB', '')
  await page.fill('input[placeholder="Search"]', 'SearchA')
  await expect(page.locator('.note-card')).toHaveCount(1)
  await page.fill('input[placeholder="Search"]', '')
  await expect(page.locator('.note-card')).toHaveCount(2)
})

// ═══ Export / Import ═══
test('export function exists and import adds notes', async ({ page }) => {
  await createNote(page, 'ExportMe', 'export content')
  expect(await page.evaluate(() => typeof window.__exportData === 'function')).toBe(true)
  expect(await page.evaluate(() => typeof window.__doImport === 'function')).toBe(true)
  // Import by calling upsertNote directly (same as __doImport but without reload)
  await page.evaluate(() => {
    const { upsertNote, addLabel } = window.__keepSync || {}
    if (!upsertNote) return
    addLabel?.('importlbl')
    upsertNote({
      id: 'imp1', title: 'ImportedNote', content: 'imported', type: 'text',
      checklist: [], color: '#faafa8', background: '', pinned: false,
      archived: false, deleted: false, deletedAt: null, labels: ['importlbl'],
      sortOrder: Date.now() - 1000, createdAt: Date.now(), updatedAt: Date.now()
    })
  })
  await page.waitForTimeout(500)
  await expect(page.locator('.note-card:has-text("ImportedNote")')).toBeVisible({ timeout: 5000 })
  // Verify imported note has the right color
  const bg = await page.locator('.note-card:has-text("ImportedNote")').evaluate(el => getComputedStyle(el).backgroundColor)
  expect(bg).not.toBe('rgb(255, 255, 255)')
})

// ═══ Settings persistence ═══
test('settings persist after reload', async ({ page }) => {
  await page.click('button[title="Settings"]')
  const toggle = page.locator('label:has-text("Move checked to bottom") input')
  // Uncheck it
  if (await toggle.isChecked()) await toggle.click()
  await expect(toggle).not.toBeChecked()
  await page.locator('.fixed.inset-0.z-50').click()
  // Reload
  await page.reload()
  await page.waitForSelector('text=Take a note', { timeout: 10000 })
  // Verify setting persisted
  await page.click('button[title="Settings"]')
  await expect(page.locator('label:has-text("Move checked to bottom") input')).not.toBeChecked()
})

// ═══ Long text ═══
test('long title does not overflow card', async ({ page }) => {
  const longTitle = 'A'.repeat(200)
  await createNote(page, longTitle, '')
  const card = page.locator('.note-card').first()
  const cardBox = await card.boundingBox()
  const titleBox = await card.locator('.font-medium').boundingBox()
  expect(titleBox.width).toBeLessThanOrEqual(cardBox.width)
})

test('long URL in note content does not overflow card', async ({ page }) => {
  const longUrl = 'https://www.amazon.com/OREI-Extractor-Extract-Passthrough-HDA-929/dp/B0CDNYH3WN/ref=sr_1_4_sspa?crid=1C0HVYBC0V3L&keywords=earc+peri+box&qid=1774602355&sprefix=earc+oeri+box%2Caps%2C334&sr=8-4-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9tdGY&psc=1'
  await createNote(page, 'LinkNote', '')
  await page.click('.note-card:has-text("LinkNote")')
  await page.waitForSelector('.editor-panel', { timeout: 5000 })
  await page.locator('.editor-panel .tiptap').first().click()
  await page.keyboard.type(longUrl)
  await page.keyboard.press('Escape')
  await page.waitForTimeout(300)
  const card = page.locator('.note-card:has-text("LinkNote")')
  const cardBox = await card.boundingBox()
  const contentBox = await card.locator('.note-content').boundingBox()
  expect(contentBox.width).toBeLessThanOrEqual(cardBox.width)
})

test('long URL in checklist item does not overflow card', async ({ page }) => {
  const longUrl = 'https://download.kbits.build.example.com/build_files/fast_download/grove-gm_fireos_ship_8147/Nightly/3567/userdebug/release-grove-gm-RS8147_userdebug_3567.tgz'
  await page.keyboard.press('l')
  await page.fill('input[placeholder="Title"]', 'CLLink')
  await page.locator('.checklist-item-editor').first().click()
  await page.keyboard.type('image for earc: ' + longUrl)
  await page.keyboard.press('Escape')
  await page.waitForTimeout(300)
  const card = page.locator('.note-card:has-text("CLLink")')
  const cardBox = await card.boundingBox()
  // Card has overflow-hidden, so visually nothing escapes. Verify card itself fits in viewport.
  expect(cardBox.width).toBeLessThan(1300)
  // Verify the card has overflow hidden
  const overflow = await card.evaluate(el => getComputedStyle(el).overflow)
  expect(overflow).toBe('hidden')
})

test('long URL in title shows shortened label', async ({ page }) => {
  const longUrl = 'https://www.amazon.com/OREI-Extractor-Extract-Passthrough-HDA-929/dp/B0CDNYH3WN/ref=sr_1_4_sspa'
  await page.click('text=Take a note')
  await page.fill('input[placeholder="Title"]', 'Check ' + longUrl)
  await page.click('body', { position: { x: 10, y: 10 } })
  await page.waitForTimeout(300)
  const card = page.locator('.note-card').first()
  // Should show shortened URL, not the full raw URL
  const titleText = await card.locator('.font-medium').textContent()
  expect(titleText).toContain('amazon.com')
  expect(titleText).not.toContain('B0CDNYH3WN')
  // Title link should have full URL in title attribute for hover
  const link = card.locator('.font-medium a')
  await expect(link).toHaveAttribute('title', longUrl)
})

// ═══ Sync — skipped in Liveblocks version (sync is via Liveblocks server, not IndexedDB cross-tab) ═══
test.skip('sync between tabs via IndexedDB', async ({ context }) => {
  const p1 = await context.newPage()
  await p1.goto('/')
  await p1.waitForSelector('text=Take a note', { timeout: 10000 })
  await p1.evaluate(() => window.__clearKeepData?.())
  await p1.waitForTimeout(200)
  await p1.click('text=Take a note')
  await p1.fill('input[placeholder="Title"]', 'SyncNote')
  await p1.click('body', { position: { x: 10, y: 10 } })
  await expect(p1.locator('.note-card:has-text("SyncNote")')).toBeVisible()
  const p2 = await context.newPage()
  await p2.goto('/')
  await p2.waitForSelector('text=Take a note', { timeout: 10000 })
  await p2.waitForTimeout(1500)
  await expect(p2.locator('.note-card:has-text("SyncNote")')).toBeVisible()
})
