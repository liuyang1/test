import SwiftUI

@main
struct KeepApp: App {
    @UIApplicationDelegateAdaptor(AppDelegate.self) var appDelegate
    @State private var roomName: String? = KeychainStore.get("roomName")

    var body: some Scene {
        WindowGroup {
            if let room = roomName {
                KeepWebView(initialJS: appDelegate.pendingAction?.js, roomName: room)
                    .ignoresSafeArea()
                    .onOpenURL { url in
                        if let action = QuickAction.from(url: url) {
                            appDelegate.pendingAction = action
                        }
                    }
            } else {
                RoomSetupView { name in
                    KeychainStore.set(name, for: "roomName")
                    roomName = name
                }
            }
        }
    }
}

// MARK: - Room Setup (first launch only)

struct RoomSetupView: View {
    var onConnect: (String) -> Void
    @State private var input = ""

    var body: some View {
        VStack(spacing: 20) {
            Image(systemName: "link.circle.fill")
                .font(.system(size: 48))
                .foregroundColor(.orange)
            Text("Connect to Room")
                .font(.title2.bold())
            Text("Enter the room name from your\nweb app's .env file (VITE_DB_NAME)")
                .font(.caption)
                .foregroundColor(.secondary)
                .multilineTextAlignment(.center)
            TextField("Room name", text: $input)
                .textFieldStyle(.roundedBorder)
                .autocapitalization(.none)
                .disableAutocorrection(true)
                .padding(.horizontal, 40)
            Button("Connect") { onConnect(input.trimmingCharacters(in: .whitespaces)) }
                .buttonStyle(.borderedProminent)
                .tint(.orange)
                .disabled(input.trimmingCharacters(in: .whitespaces).isEmpty)
        }
        .padding()
    }
}

// MARK: - Quick Actions

enum QuickAction {
    case newNote, newChecklist, newNoteWithText(String)

    var js: String {
        switch self {
        case .newNote:
            return "document.querySelector('[class*=\"cursor-text\"]')?.click()"
        case .newChecklist:
            return "document.dispatchEvent(new KeyboardEvent('keydown', {key: 'l', bubbles: true}))"
        case .newNoteWithText(let text):
            let escaped = text.replacingOccurrences(of: "'", with: "\\'").replacingOccurrences(of: "\n", with: "\\n")
            return """
            (function() {
                document.querySelector('[class*="cursor-text"]')?.click();
                setTimeout(() => {
                    const t = document.querySelector('input[placeholder="Title"]');
                    if (t) { t.focus(); t.value = '\(escaped)'; t.dispatchEvent(new Event('input', {bubbles:true})); }
                }, 300);
            })()
            """
        }
    }

    static func from(url: URL) -> QuickAction? {
        guard url.scheme == "keep" else { return nil }
        switch url.host {
        case "new": return .newNote
        case "checklist": return .newChecklist
        case "share":
            let text = URLComponents(url: url, resolvingAgainstBaseURL: false)?
                .queryItems?.first(where: { $0.name == "text" })?.value ?? ""
            return .newNoteWithText(text)
        default: return nil
        }
    }

    static func from(shortcutType: String) -> QuickAction? {
        switch shortcutType {
        case "NewNote": return .newNote
        case "NewChecklist": return .newChecklist
        default: return nil
        }
    }
}

// MARK: - App Delegate

class AppDelegate: NSObject, UIApplicationDelegate, ObservableObject {
    @Published var pendingAction: QuickAction?

    func application(_ application: UIApplication, configurationForConnecting session: UISceneSession, options: UIScene.ConnectionOptions) -> UISceneConfiguration {
        application.shortcutItems = [
            UIApplicationShortcutItem(type: "NewNote", localizedTitle: "New Note", localizedSubtitle: nil, icon: UIApplicationShortcutIcon(systemImageName: "square.and.pencil")),
            UIApplicationShortcutItem(type: "NewChecklist", localizedTitle: "New Checklist", localizedSubtitle: nil, icon: UIApplicationShortcutIcon(systemImageName: "checklist")),
        ]
        return UISceneConfiguration(name: nil, sessionRole: session.role)
    }

    func application(_ application: UIApplication, performActionFor shortcutItem: UIApplicationShortcutItem, completionHandler: @escaping (Bool) -> Void) {
        pendingAction = QuickAction.from(shortcutType: shortcutItem.type)
        completionHandler(pendingAction != nil)
    }
}
