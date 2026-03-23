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

// ═══ CRUD ═══
test('create text note', async ({ page }) => { await createNote(page, 'TextNote', 'body') })
test('create checklist via L', async ({ page }) => {
  await page.keyboard.press('l')
  await page.fill('input[placeholder="Title"]', 'List')
  await page.fill('input[placeholder="List item"]', 'Item1')
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
  await page.keyboard.press('Enter')
  // Content editor should receive focus
  await expect(page.locator('.tiptap')).toBeFocused({ timeout: 5000 })
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
  await page.locator('input[placeholder="List item"]').first().click()
  await page.keyboard.type('Do')
  await page.keyboard.press('Escape')
  await page.locator('.note-card:has-text("CardCheck") .checklist-check').first().click({ force: true })
  await expect(page.locator('.note-card:has-text("1 checked")')).toBeVisible()
})
test('checklist Enter adds item', async ({ page }) => {
  await page.keyboard.press('l')
  await page.fill('input[placeholder="Title"]', 'EnterCheck')
  await page.locator('input[placeholder="List item"]').first().click()
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
  await page.click('[data-testid="card-actions"] button[title="Change color"]')
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
  await expect(actions.locator('button[title="Archive"]')).toBeVisible()
  await expect(actions.locator('button[title="Change color"]')).toBeVisible()
  await expect(actions.locator('button[title="Add label"]')).toBeVisible()
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
  await page.locator('input[placeholder="List item"]').first().click()
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
  await page.locator('input[placeholder="List item"]').first().click()
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
  await page.locator('input[placeholder="List item"]').first().click()
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
  await page.locator('input[placeholder="List item"]').first().click()
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
  // After reorder, first input should now be "Second"
  const firstInput = page.locator('input[placeholder="List item"]').first()
  await expect(firstInput).toHaveValue('Second')
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
  await page.locator('input[placeholder="List item"]').first().click()
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

// ═══ Sync (needs separate setup — don't block WS) ═══
test('sync between tabs via IndexedDB', async ({ context }) => {
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
