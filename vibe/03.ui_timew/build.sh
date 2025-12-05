#!/bin/bash

APP_NAME="AssembleTimeW"
BUNDLE_ID="com.assembletimew.app"

swiftc -o "$APP_NAME" \
    -framework SwiftUI \
    -framework AppKit \
    AssembleTimeWApp.swift \
    ContentView.swift

mkdir -p "$APP_NAME.app/Contents/MacOS"
mkdir -p "$APP_NAME.app/Contents/Resources"

mv "$APP_NAME" "$APP_NAME.app/Contents/MacOS/"
cp Info.plist "$APP_NAME.app/Contents/"

echo "✅ Build complete: $APP_NAME.app"
