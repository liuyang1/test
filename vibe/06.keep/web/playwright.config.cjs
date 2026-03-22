const { defineConfig } = require('@playwright/test')

module.exports = defineConfig({
  testDir: './e2e',
  timeout: 30000,
  workers: 4,
  fullyParallel: true,
  use: {
    baseURL: 'http://localhost:5137',
    headless: true,
    screenshot: 'only-on-failure',
  },
  webServer: {
    command: 'npx vite --host 0.0.0.0 --port 5137',
    port: 5137,
    reuseExistingServer: true,
    timeout: 15000,
  },
})
