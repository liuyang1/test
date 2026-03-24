#!/bin/bash
# Builds the web app and copies it into the iOS app's Resources/web/ folder.
# Run from the project root: ./ios/build-web.sh

set -e
cd "$(dirname "$0")/.."

echo "Building web app..."
cd web-liveblocks
npx vite build

echo "Copying to iOS resources..."
rm -rf ../ios/Keep/Resources/web
cp -r dist ../ios/Keep/Resources/web
cp public/favicon.svg ../ios/Keep/Resources/web/

# Make asset paths relative and remove crossorigin for file:// loading in WKWebView
sed -i 's|href="/|href="./|g; s|src="/|src="./|g; s| crossorigin||g' ../ios/Keep/Resources/web/index.html

echo "Done. Open ios/KeepApp.xcodeproj in Xcode to build."
