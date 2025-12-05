import SwiftUI

@main
struct AssembleTimeWApp: App {
    @NSApplicationDelegateAdaptor(AppDelegate.self) var appDelegate
    @StateObject private var timewModel = TimewModel()
    
    var body: some Scene {
        WindowGroup {
            ContentView(timew: timewModel)
        }
        .windowStyle(.hiddenTitleBar)
        .commands {
            CommandGroup(replacing: .newItem) {}
        }
    }
}

class AppDelegate: NSObject, NSApplicationDelegate {
    func applicationDidFinishLaunching(_ notification: Notification) {
        if let window = NSApplication.shared.windows.first {
            window.level = .floating
            window.isMovableByWindowBackground = true
            window.styleMask = [.borderless, .resizable]
            window.backgroundColor = .clear
            window.isOpaque = false
            window.titlebarAppearsTransparent = true
            window.minSize = NSSize(width: 80, height: 40)
            window.collectionBehavior = [.canJoinAllSpaces, .fullScreenAuxiliary]
            
            if let frameString = UserDefaults.standard.string(forKey: "windowFrame"),
               let frame = NSRectFromString(frameString) as NSRect? {
                window.setFrame(frame, display: true)
            } else {
                window.setContentSize(NSSize(width: 120, height: 50))
            }
            
            NotificationCenter.default.addObserver(
                forName: NSWindow.didMoveNotification,
                object: window,
                queue: .main
            ) { _ in
                UserDefaults.standard.set(NSStringFromRect(window.frame), forKey: "windowFrame")
            }
            
            NotificationCenter.default.addObserver(
                forName: NSWindow.didResizeNotification,
                object: window,
                queue: .main
            ) { _ in
                UserDefaults.standard.set(NSStringFromRect(window.frame), forKey: "windowFrame")
            }
        }
    }
    
    func applicationShouldTerminateAfterLastWindowClosed(_ sender: NSApplication) -> Bool {
        return true
    }
}
