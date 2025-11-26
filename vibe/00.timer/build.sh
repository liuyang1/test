#!/bin/bash

APP_NAME="Timer"
BUNDLE_ID="com.timer.app"

# 创建 app 目录结构
mkdir -p "${APP_NAME}.app/Contents/MacOS"
mkdir -p "${APP_NAME}.app/Contents/Resources"

# 复制 Info.plist
cp Info.plist "${APP_NAME}.app/Contents/"

# 编译 Swift 代码
swiftc -o "${APP_NAME}.app/Contents/MacOS/${APP_NAME}" \
    -framework SwiftUI \
    -framework AppKit \
    -framework AVFoundation \
    TimerApp.swift ContentView.swift

if [ $? -eq 0 ]; then
    echo "✅ 编译成功！"
    echo "运行: open ${APP_NAME}.app"
else
    echo "❌ 编译失败"
    exit 1
fi
