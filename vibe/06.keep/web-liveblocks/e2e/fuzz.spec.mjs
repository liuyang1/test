import { test, expect } from '@playwright/test'

/**
 * Fuzz test — randomly performs UI actions and checks the app never crashes.
 * Run: npx playwright test e2e/fuzz.spec.mjs --timeout=120000
 */

const ACTIONS = [
  'createNote', 'createChecklist', 'clickCard', 'deleteCard',
  'archiveCard', 'pinCard', 'editTitle', 'typeContent',
  'pressEscape', 'pressEnter', 'search', 'clearSearch',
  'toggleColor', 'toggleLabel', 'undo', 'clickRandom',
  'keyboardShortcut', 'dragChecklist',
]

function pick(arr) { return arr[Math.floor(Math.random() * arr.length)] }
function rand(min, max) { return Math.floor(Math.random() * (max - min)) + min }
function randStr(len = 8) { return Array.from({ length: len }, () => String.fromCharCode(rand(97, 123))).join('') }

test('fuzz: random actions do not crash the app', async ({ page }) => {
  test.setTimeout(60000)
  page.setDefaultTimeout(3000) // 3s max per action
  await page.goto('/?nosync')
  await page.waitForSelector('text=Take a note', { timeout: 10000 })
  await page.evaluate(() => window.__clearKeepData?.())
  await page.waitForFunction(() => document.querySelectorAll('.note-card').length === 0, { timeout: 5000 }).catch(() => {})

  const errors = []
  page.on('pageerror', err => errors.push(err.message))

  // Seed some notes
  for (let i = 0; i < 3; i++) {
    await page.click('text=Take a note')
    await page.fill('input[placeholder="Title"]', `Fuzz${i}`)
    await page.click('body', { position: { x: 10, y: 10 } })
    await page.waitForTimeout(100)
  }

  // Run random actions
  const iterations = 30
  for (let i = 0; i < iterations; i++) {
    const action = pick(ACTIONS)
    try {
      switch (action) {
        case 'createNote': {
          await page.click('text=Take a note')
          await page.fill('input[placeholder="Title"]', randStr())
          await page.click('body', { position: { x: 10, y: 10 } })
          break
        }
        case 'createChecklist': {
          await page.keyboard.press('l')
          const titleInput = page.locator('input[placeholder="Title"]')
          if (await titleInput.count()) {
            await titleInput.fill(randStr())
            const li = page.locator('input[placeholder="List item"]')
            if (await li.count()) { await li.first().click(); await page.keyboard.type(randStr(10)) }
            await page.keyboard.press('Escape')
          }
          break
        }
        case 'clickCard': {
          const cards = page.locator('.note-card')
          const count = await cards.count()
          if (count > 0) {
            await cards.nth(rand(0, count)).click()
            await page.waitForTimeout(100)
            // Close editor if open
            if (await page.locator('.editor-panel').count()) {
              await page.keyboard.press('Escape')
            }
          }
          break
        }
        case 'deleteCard': {
          const cards = page.locator('.note-card')
          const count = await cards.count()
          if (count > 0) {
            const card = cards.nth(rand(0, count))
            await card.hover()
            const del = card.locator('[data-testid="card-actions"] button[title="Delete"]')
            if (await del.count()) await del.click()
          }
          break
        }
        case 'archiveCard': {
          const cards = page.locator('.note-card')
          const count = await cards.count()
          if (count > 0) {
            const card = cards.nth(rand(0, count))
            await card.hover()
            const arch = card.locator('[data-testid="card-actions"] button[title="Archive"]')
            if (await arch.count()) await arch.click()
          }
          break
        }
        case 'pinCard': {
          const cards = page.locator('.note-card')
          const count = await cards.count()
          if (count > 0) {
            const card = cards.nth(rand(0, count))
            await card.hover()
            const pin = card.locator('.pin-btn')
            if (await pin.count()) await pin.click()
          }
          break
        }
        case 'editTitle': {
          const cards = page.locator('.note-card')
          const count = await cards.count()
          if (count > 0) {
            await cards.nth(rand(0, count)).click()
            const titleInput = page.locator('.editor-panel input[placeholder="Title"]')
            if (await titleInput.count()) {
              await titleInput.fill(randStr())
            }
            await page.keyboard.press('Escape')
          }
          break
        }
        case 'typeContent': {
          const editor = page.locator('.editor-panel .tiptap')
          if (await editor.count()) {
            await editor.click()
            await page.keyboard.type(randStr(rand(5, 30)))
          }
          break
        }
        case 'pressEscape': {
          await page.keyboard.press('Escape')
          break
        }
        case 'pressEnter': {
          await page.keyboard.press('Enter')
          break
        }
        case 'search': {
          await page.fill('input[placeholder="Search"]', randStr(3))
          await page.waitForTimeout(100)
          break
        }
        case 'clearSearch': {
          await page.fill('input[placeholder="Search"]', '')
          break
        }
        case 'toggleColor': {
          const cards = page.locator('.note-card')
          const count = await cards.count()
          if (count > 0) {
            const card = cards.nth(rand(0, count))
            await card.hover()
            const colorBtn = card.locator('[data-testid="card-actions"] button[title="Background options"]')
            if (await colorBtn.count()) {
              await colorBtn.click()
              await page.waitForTimeout(100)
              const colors = page.locator('button[title]').filter({ hasText: '' })
              const cCount = await colors.count()
              if (cCount > 2) await colors.nth(rand(0, Math.min(cCount, 12))).click().catch(() => {})
            }
          }
          break
        }
        case 'toggleLabel': {
          const cards = page.locator('.note-card')
          const count = await cards.count()
          if (count > 0) {
            const card = cards.nth(rand(0, count))
            await card.hover()
            const lblBtn = card.locator('[data-testid="card-actions"] button[title="Add label"]')
            if (await lblBtn.count()) {
              await lblBtn.click()
              await page.waitForTimeout(100)
              await page.keyboard.press('Escape')
            }
          }
          break
        }
        case 'undo': {
          await page.keyboard.press('Control+z')
          break
        }
        case 'clickRandom': {
          const vp = page.viewportSize()
          await page.mouse.click(rand(100, vp.width - 50), rand(100, vp.height - 50))
          break
        }
        case 'keyboardShortcut': {
          const keys = ['c', 'l', '/', 'Escape']
          await page.keyboard.press(pick(keys))
          break
        }
        case 'dragChecklist': {
          const handles = page.locator('[data-testid="drag-handle"]')
          const count = await handles.count()
          if (count > 1) {
            const from = handles.nth(0)
            const to = handles.nth(Math.min(1, count - 1))
            const fromBox = await from.boundingBox()
            const toBox = await to.boundingBox()
            if (fromBox && toBox) {
              await page.mouse.move(fromBox.x + 5, fromBox.y + 5)
              await page.mouse.down()
              await page.mouse.move(toBox.x + 5, toBox.y + 5, { steps: 5 })
              await page.mouse.up()
            }
          }
          break
        }
      }
    } catch (e) {
      // Action failed — skip and continue
    }
    try { await page.waitForTimeout(50) } catch { break }
  }

  // Final check: app should still be functional
  if (!page.isClosed()) {
    await page.fill('input[placeholder="Search"]', '').catch(() => {})
    await page.waitForTimeout(500)

    const criticalErrors = errors.filter(e =>
      !e.includes('ResizeObserver') &&
      !e.includes('Script error')
    )
    expect(criticalErrors).toEqual([])

    await expect(page.locator('text=Take a note')).toBeVisible()
    await page.click('text=Take a note')
    await page.fill('input[placeholder="Title"]', 'PostFuzz')
    await page.click('body', { position: { x: 10, y: 10 } })
    await expect(page.locator('.note-card:has-text("PostFuzz")')).toBeVisible()
  }
})
