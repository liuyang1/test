import Foundation

/// Configuration loaded from Config.plist or environment.
enum AppConfig {
    private static let config: [String: Any] = {
        guard let url = Bundle.main.url(forResource: "Config", withExtension: "plist"),
              let data = try? Data(contentsOf: url),
              let dict = try? PropertyListSerialization.propertyList(from: data, format: nil) as? [String: Any]
        else { return [:] }
        return dict
    }()

    static var liveblocksSecretKey: String {
        config["LIVEBLOCKS_SECRET_KEY"] as? String ?? ""
    }

    static var roomId: String {
        config["ROOM_ID"] as? String ?? "keep-notes"
    }
}
