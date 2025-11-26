import SwiftUI
import AVFoundation

struct ContentView: View {
    @StateObject private var timer = TimerModel()
    @State private var isHovering = false
    
    var body: some View {
        HStack(spacing: 4) {
            if isHovering {
                Button(action: { NSApplication.shared.terminate(nil) }) {
                    Image(systemName: "xmark.circle.fill")
                        .font(.system(size: 14))
                        .foregroundColor(.gray.opacity(0.7))
                }
                .buttonStyle(.plain)
                .frame(width: 20)
            } else {
                Spacer().frame(width: 20)
            }
            
            Spacer()
            
            TextField("", text: $timer.inputText)
                .font(.system(size: 24, weight: .medium, design: .monospaced))
                .multilineTextAlignment(.center)
                .textFieldStyle(.plain)
                .disabled(timer.isRunning)
                .frame(width: 80)
                .foregroundColor(.black)
                .shadow(color: .black.opacity(0.3), radius: 2, x: 0, y: 1)
                .padding(4)
                .background(Color.white.opacity(0.1))
                .cornerRadius(4)
                .overlay(
                    RoundedRectangle(cornerRadius: 4)
                        .stroke(Color.gray.opacity(0.3), lineWidth: 1)
                )
            
            Spacer()
            
            if isHovering {
                Button(action: {
                    if timer.isRunning {
                        timer.stop()
                    } else {
                        timer.start()
                    }
                }) {
                    Image(systemName: timer.isRunning ? "stop.fill" : "play.fill")
                        .font(.system(size: 14))
                }
                .buttonStyle(.plain)
                .frame(width: 20)
            } else {
                Spacer().frame(width: 20)
            }
        }
        .padding(.horizontal, 8)
        .padding(.vertical, 6)
        .background(Color(NSColor.windowBackgroundColor).opacity(0.85))
        .cornerRadius(8)
        .overlay(
            RoundedRectangle(cornerRadius: 8)
                .stroke(Color.gray.opacity(0.2), lineWidth: 1)
        )
        .onHover { hovering in
            isHovering = hovering
        }
    }
}

class TimerModel: ObservableObject {
    @Published var inputText = "00:00"
    @Published var isRunning = false
    
    private var seconds = 0
    private var targetSeconds = 0
    private var isCountdown = false
    private var timerInstance: Timer?
    private var originalInput = ""
    
    func start() {
        originalInput = inputText
        let cleanInput = inputText.replacingOccurrences(of: ":", with: "").replacingOccurrences(of: " ", with: "")
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
        updateDisplay()
        
        timerInstance = Timer.scheduledTimer(withTimeInterval: 1.0, repeats: true) { [weak self] _ in
            self?.tick()
        }
    }
    
    func stop() {
        timerInstance?.invalidate()
        timerInstance = nil
        isRunning = false
        seconds = 0
        inputText = "00:00"
    }
    
    private func tick() {
        if isCountdown {
            seconds -= 1
            if seconds <= 0 {
                seconds = 0
                stop()
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
