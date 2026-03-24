import SwiftUI
import WebKit

struct KeepWebView: UIViewRepresentable {
    var initialJS: String? = nil
    var roomName: String

    func makeCoordinator() -> Coordinator { Coordinator() }

    func makeUIView(context: Context) -> WKWebView {
        let config = WKWebViewConfiguration()
        config.websiteDataStore = .default()
        config.allowsInlineMediaPlayback = true

        // Inject room name override before page JS runs
        let roomScript = WKUserScript(
            source: "window.__KEEP_ROOM__ = '\(roomName)';",
            injectionTime: .atDocumentStart,
            forMainFrameOnly: true
        )
        config.userContentController.addUserScript(roomScript)

        let wv = WKWebView(frame: .zero, configuration: config)
        wv.isOpaque = false
        wv.backgroundColor = .white
        context.coordinator.webView = wv

        if let webDir = Bundle.main.url(forResource: "web", withExtension: nil),
           let indexURL = Bundle.main.url(forResource: "web/index", withExtension: "html") {
            wv.loadFileURL(indexURL, allowingReadAccessTo: webDir)
        }

        if let js = initialJS {
            context.coordinator.pendingJS = js
            wv.navigationDelegate = context.coordinator
        }

        return wv
    }

    func updateUIView(_ uv: WKWebView, context: Context) {}

    class Coordinator: NSObject, WKNavigationDelegate {
        var webView: WKWebView?
        var pendingJS: String?

        func webView(_ webView: WKWebView, didFinish navigation: WKNavigation!) {
            guard let js = pendingJS else { return }
            pendingJS = nil
            DispatchQueue.main.asyncAfter(deadline: .now() + 1.5) {
                webView.evaluateJavaScript(js) { _, _ in }
            }
        }
    }
}
