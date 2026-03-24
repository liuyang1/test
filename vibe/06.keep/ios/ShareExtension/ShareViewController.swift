import UIKit
import Social

/// Share Extension — receives shared text and opens the main app with it.
class ShareViewController: SLComposeServiceViewController {
    override func didSelectPost() {
        guard let text = contentText, !text.isEmpty else {
            extensionContext?.completeRequest(returningItems: nil)
            return
        }

        // Open main app via URL scheme
        let encoded = text.addingPercentEncoding(withAllowedCharacters: .urlQueryAllowed) ?? ""
        if let url = URL(string: "keep://share?text=\(encoded)") {
            // Share extensions can't open URLs directly, use openURL workaround
            var responder: UIResponder? = self
            while let r = responder {
                if let app = r as? UIApplication {
                    app.open(url)
                    break
                }
                responder = r.next
            }
        }

        extensionContext?.completeRequest(returningItems: nil)
    }

    override func configurationItems() -> [Any]! { [] }
}
