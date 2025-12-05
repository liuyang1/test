#!/bin/bash

APP_NAME="AssembleTimeW"

# 复制到 Applications
echo "📦 Installing $APP_NAME.app to /Applications..."
cp -r "$APP_NAME.app" /Applications/

# 添加到登录项
echo "🚀 Adding to login items..."
osascript -e "tell application \"System Events\" to make login item at end with properties {path:\"/Applications/$APP_NAME.app\", hidden:false}"

echo "✅ Installation complete!"
echo "   App installed at: /Applications/$APP_NAME.app"
echo "   Auto-start enabled"
