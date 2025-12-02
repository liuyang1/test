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
            var hotKeyID = EventHotKeyID()
            GetEventParameter(theEvent, EventParamName(kEventParamDirectObject), EventParamType(typeEventHotKeyID), nil, MemoryLayout<EventHotKeyID>.size, nil, &hotKeyID)
            
            if hotKeyID.id == 1 {
                NotificationCenter.default.post(name: .toggleTimer, object: nil)
            } else if hotKeyID.id == 2 {
                NotificationCenter.default.post(name: .resetTimer, object: nil)
            }
            return noErr
        }, 1, &eventType, nil, &eventHandler)
        
        // ⌃⌘M: controlKey + cmdKey + M(46)
        let toggleHotKeyID = EventHotKeyID(signature: OSType(0x54494D52), id: 1)
        RegisterEventHotKey(46, UInt32(controlKey | cmdKey), toggleHotKeyID, GetApplicationEventTarget(), 0, &hotKeyRef)
        
        // ⌃⌘R: controlKey + cmdKey + R(15)
        let resetHotKeyID = EventHotKeyID(signature: OSType(0x54494D52), id: 2)
        var resetHotKeyRef: EventHotKeyRef?
        RegisterEventHotKey(15, UInt32(controlKey | cmdKey), resetHotKeyID, GetApplicationEventTarget(), 0, &resetHotKeyRef)
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
    static let resetTimer = Notification.Name("resetTimer")
}
