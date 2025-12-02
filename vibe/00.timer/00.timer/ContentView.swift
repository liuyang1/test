import SwiftUI
import AVFoundation

struct ContentView: View {
    @ObservedObject var timer: TimerModel
    @FocusState private var isInputFocused: Bool
    
    var body: some View {
        ZStack {
            VisualEffectBlur()
                .cornerRadius(8)
            
            HStack(spacing: 8) {
                Button(action: { timer.reset() }) {
                    Image(systemName: "arrow.counterclockwise.circle.fill")
                        .font(.system(size: 14))
                        .foregroundColor(.secondary)
                        .frame(width: 24, height: 24)
                        .contentShape(Rectangle())
                }
                .buttonStyle(.plain)
                
                Divider()
                    .frame(height: 20)
                
                TextField("", text: $timer.inputText)
                    .font(.system(size: 24, weight: .medium, design: .rounded))
                    .multilineTextAlignment(.center)
                    .textFieldStyle(.plain)
                    .disabled(timer.isRunning || timer.isPaused)
                    .focused($isInputFocused)
                    .frame(width: 80)
                    .foregroundColor(.primary)
                    .padding(.horizontal, 8)
                    .padding(.vertical, 4)
                    .background(
                        RoundedRectangle(cornerRadius: 4)
                            .fill(Color.primary.opacity(0.05))
                    )
                    .overlay(
                        RoundedRectangle(cornerRadius: 4)
                            .stroke(Color.primary.opacity(0.1), lineWidth: 0.5)
                    )
                    .onChange(of: isInputFocused) { oldValue, newValue in
                        if newValue && !timer.isRunning && !timer.isPaused {
                            timer.inputText = ""
                        }
                    }
                
                Divider()
                    .frame(height: 20)
                
                Button(action: { timer.toggleStartPause() }) {
                    Image(systemName: timer.isRunning ? "pause.fill" : "play.fill")
                        .font(.system(size: 14))
                        .foregroundColor(.secondary)
                        .frame(width: 24, height: 24)
                        .contentShape(Rectangle())
                }
                .buttonStyle(.plain)
            }
            .padding(.horizontal, 8)
            .padding(.vertical, 6)
        }
        .onReceive(NotificationCenter.default.publisher(for: .toggleTimer)) { _ in
            timer.toggleStartPause()
        }
        .contextMenu {
            Button("退出") {
                NSApplication.shared.terminate(nil)
            }
        }
    }
}

struct VisualEffectBlur: NSViewRepresentable {
    func makeNSView(context: Context) -> NSVisualEffectView {
        let view = NSVisualEffectView()
        view.material = .hudWindow
        view.blendingMode = .behindWindow
        view.state = .active
        return view
    }
    
    func updateNSView(_ nsView: NSVisualEffectView, context: Context) {}
}

class TimerModel: ObservableObject {
    @Published var inputText = "00:00"
    @Published var isRunning = false
    @Published var isPaused = false
    
    private var seconds = 0
    private var targetSeconds = 0
    private var isCountdown = false
    private var timerInstance: Timer?
    private var originalInput = ""
    
    func toggleStartPause() {
        if isRunning {
            pause()
        } else {
            start()
        }
    }
    
    func start() {
        if isPaused {
            // 继续计时
            isPaused = false
            isRunning = true
        } else {
            // 开始新计时
            originalInput = inputText
            
            // 只提取数字
            let cleanInput = inputText.filter { $0.isNumber }
            let minutes = Int(cleanInput) ?? 0
            
            if minutes > 0 {
                isCountdown = true
                targetSeconds = minutes * 60
                seconds = targetSeconds
            } else {
                isCountdown = false
                seconds = 0
            }
            
            isRunning = true
            isPaused = false
            updateDisplay()
        }
        
        timerInstance = Timer.scheduledTimer(withTimeInterval: 1.0, repeats: true) { [weak self] _ in
            self?.tick()
        }
    }
    
    func pause() {
        timerInstance?.invalidate()
        timerInstance = nil
        isRunning = false
        isPaused = true
    }
    
    func reset() {
        timerInstance?.invalidate()
        timerInstance = nil
        isRunning = false
        isPaused = false
        seconds = 0
        inputText = "00:00"
    }
    
    private func tick() {
        if isCountdown {
            seconds -= 1
            if seconds <= 0 {
                seconds = 0
                pause()
                playSound()
            }
        } else {
            seconds += 1
        }
        updateDisplay()
    }
    
    private func updateDisplay() {
        let mins = seconds / 60
        let secs = seconds % 60
        inputText = String(format: "%02d:%02d", mins, secs)
    }
    
    private func playSound() {
        NSSound.beep()
    }
}
