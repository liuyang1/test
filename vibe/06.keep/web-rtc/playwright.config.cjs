const { defineConfig } = require('@playwright/test')

module.exports = defineConfig({
  testDir: './e2e',
  timeout: 30000,
  workers: 4,
  fullyParallel: true,
  retries: 1,
  use: {
    baseURL: 'http://localhost:5138',
    headless: true,
    screenshot: 'only-on-failure',
  },
  webServer: {
    command: 'npx vite --host 0.0.0.0 --port 5138',
    port: 5138,
    reuseExistingServer: true,
    timeout: 15000,
    env: { VITE_DB_NAME: 'keep-notes-test' },
  },
})
