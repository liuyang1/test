import SwiftUI
import Carbon

@main
struct TimerApp: App {
    @NSApplicationDelegateAdaptor(AppDelegate.self) var appDelegate
    @StateObject private var timerModel = TimerModel()
    
    var body: some Scene {
        WindowGroup {
            ContentView(timer: timerModel)
        }
        .windowStyle(.hiddenTitleBar)
        .commands {
            CommandGroup(replacing: .newItem) {}
        }
    }
}

class AppDelegate: NSObject, NSApplicationDelegate {
    var hotKeyRef: EventHotKeyRef?
    var eventHandler: EventHandlerRef?
    
    func applicationDidFinishLaunching(_ notification: Notification) {
        if let window = NSApplication.shared.windows.first {
            window.level = .floating
            window.isMovableByWindowBackground = true
            window.styleMask = [.borderless, .resizable]
            window.backgroundColor = .clear
            window.isOpaque = false
            window.titlebarAppearsTransparent = true
            window.minSize = NSSize(width: 80, height: 60)
            window.collectionBehavior = [.canJoinAllSpaces, .fullScreenAuxiliary]
            
            // 恢复窗口位置和大小
            if let frameString = UserDefaults.standard.string(forKey: "windowFrame"),
               let frame = NSRectFromString(frameString) as NSRect? {
                window.setFrame(frame, display: true)
            } else {
                window.setContentSize(NSSize(width: 200, height: 100))
            }
            
            // 监听窗口移动和调整大小
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
        
        registerGlobalHotKey()
    }
    
    func registerGlobalHotKey() {
        var eventType = EventTypeSpec(eventClass: OSType(kEventClassKeyboard), eventKind: UInt32(kEventHotKeyPressed))
        
        InstallEventHandler(GetApplicationEventTarget(), { (nextHandler, theEvent, userData) -> OSStatus in
            NotificationCenter.default.post(name: .toggleTimer, object: nil)
            return noErr
        }, 1, &eventType, nil, &eventHandler)
        
        let hotKeyID = EventHotKeyID(signature: OSType(0x54494D52), id: 1) // 'TIMR'
        
        // ⌃⌘M: controlKey + cmdKey + M(46)
        RegisterEventHotKey(46, UInt32(controlKey | cmdKey), hotKeyID, GetApplicationEventTarget(), 0, &hotKeyRef)
    }
    
    func applicationShouldTerminateAfterLastWindowClosed(_ sender: NSApplication) -> Bool {
        return true
    }
    
    func applicationWillTerminate(_ notification: Notification) {
        if let hotKey = hotKeyRef {
            UnregisterEventHotKey(hotKey)
        }
    }
}

extension Notification.Name {
    static let toggleTimer = Notification.Name("toggleTimer")
}
