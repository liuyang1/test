import Foundation

/// WebSocket sync client.
/// Connects to the Keep sync server and exchanges note state.
/// Since there's no Swift Yjs library, this uses a simple JSON-based
/// last-write-wins merge over WebSocket. For full CRDT sync, a future
/// version could integrate a native Yjs port or use a REST API bridge.
final class SyncClient: NSObject, ObservableObject, @unchecked Sendable {
    enum Status: String { case disconnected, connecting, connected, error }

    @Published private(set) var status: Status = .disconnected

    private var ws: URLSessionWebSocketTask?
    private var session: URLSession?
    private let serverURL: String
    private let room: String
    private let token: String
    private var onReceive: (([Note], [String]) -> Void)?

    init(serverURL: String = "ws://localhost:4444", room: String = "keep-notes", token: String = "") {
        self.serverURL = serverURL
        self.room = room
        self.token = token
        super.init()
    }

    func connect(onReceive: @escaping ([Note], [String]) -> Void) {
        self.onReceive = onReceive
        guard var components = URLComponents(string: "\(serverURL)/\(room)") else { return }
        components.queryItems = [URLQueryItem(name: "token", value: token)]
        guard let url = components.url else { return }

        session = URLSession(configuration: .default, delegate: self, delegateQueue: nil)
        ws = session?.webSocketTask(with: url)
        status = .connecting
        ws?.resume()
        listen()
    }

    func send(notes: [Note], labels: [String]) {
        guard let ws, ws.state == .running else { return }
        let payload = SyncPayload(notes: notes, labels: labels)
        guard let data = try? JSONEncoder().encode(payload) else { return }
        ws.send(.data(data)) { _ in }
    }

    func disconnect() {
        ws?.cancel(with: .normalClosure, reason: nil)
        ws = nil
        status = .disconnected
    }

    private func listen() {
        ws?.receive { [weak self] result in
            switch result {
            case .success(.data(let data)):
                if let payload = try? JSONDecoder().decode(SyncPayload.self, from: data) {
                    DispatchQueue.main.async { self?.onReceive?(payload.notes, payload.labels) }
                }
            case .success(.string(let text)):
                if let data = text.data(using: .utf8),
                   let payload = try? JSONDecoder().decode(SyncPayload.self, from: data) {
                    DispatchQueue.main.async { self?.onReceive?(payload.notes, payload.labels) }
                }
            case .failure:
                DispatchQueue.main.async { self?.status = .error }
                return
            @unknown default: break
            }
            self?.listen()
        }
    }
}

extension SyncClient: URLSessionWebSocketDelegate {
    func urlSession(_ session: URLSession, webSocketTask: URLSessionWebSocketTask, didOpenWithProtocol protocol: String?) {
        DispatchQueue.main.async { self.status = .connected }
    }

    func urlSession(_ session: URLSession, webSocketTask: URLSessionWebSocketTask, didCloseWith closeCode: URLSessionWebSocketTask.CloseCode, reason: Data?) {
        DispatchQueue.main.async { self.status = .disconnected }
    }
}

private struct SyncPayload: Codable {
    let notes: [Note]
    let labels: [String]
}
