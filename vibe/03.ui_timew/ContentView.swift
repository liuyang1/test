import SwiftUI

struct ContentView: View {
    @ObservedObject var timew: TimewModel
    
    var body: some View {
        ZStack {
            RoundedRectangle(cornerRadius: 8)
                .fill(.ultraThinMaterial)
            
            Text(timew.status)
                .font(.system(size: 20, weight: .medium, design: .rounded))
                .foregroundColor(.primary)
                .padding(.horizontal, 12)
                .padding(.vertical, 8)
        }
        .contextMenu {
            Button("退出") {
                NSApplication.shared.terminate(nil)
            }
        }
    }
}

class TimewModel: ObservableObject {
    @Published var status = "💤"
    private var timer: Timer?
    
    init() {
        updateStatus()
        timer = Timer.scheduledTimer(withTimeInterval: 1.0, repeats: true) { [weak self] _ in
            self?.updateStatus()
        }
    }
    
    private func updateStatus() {
        let task = Process()
        task.executableURL = URL(fileURLWithPath: "/opt/homebrew/bin/timew")
        
        let pipe = Pipe()
        task.standardOutput = pipe
        
        do {
            try task.run()
            task.waitUntilExit()
            
            let data = pipe.fileHandleForReading.readDataToEndOfFile()
            if let output = String(data: data, encoding: .utf8) {
                parseOutput(output)
            }
        } catch {
            DispatchQueue.main.async {
                self.status = "💤"
            }
        }
    }
    
    private func parseOutput(_ output: String) {
        let lines = output.split(separator: "\n")
        for line in lines {
            if line.contains("Total") {
                let parts = line.split(separator: " ")
                if let time = parts.last {
                    DispatchQueue.main.async {
                        self.status = "🔥 \(time)"
                    }
                    return
                }
            }
        }
        DispatchQueue.main.async {
            self.status = "💤"
        }
    }
}
