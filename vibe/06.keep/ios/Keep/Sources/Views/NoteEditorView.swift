import SwiftUI

struct NoteEditorView: View {
    @Binding var note: Note
    let labels: [String]
    var onSave: () -> Void = {}
    var onDelete: () -> Void = {}
    var onAddLabel: (String) -> Void = { _ in }
    @Environment(\.dismiss) private var dismiss

    @State private var showColors = false
    @State private var showLabels = false
    @FocusState private var titleFocused: Bool

    var body: some View {
        NavigationStack {
            ScrollView {
                VStack(alignment: .leading, spacing: 12) {
                    // Title
                    TextField("Title", text: $note.title, axis: .vertical)
                        .font(.system(size: 22, weight: .medium))
                        .focused($titleFocused)

                    // Content
                    if note.type == .checklist {
                        ChecklistEditorView(items: $note.checklist)
                    } else {
                        TextField("Take a note...", text: $note.content, axis: .vertical)
                            .font(.custom("Google Sans Text", size: 15))
                            .lineLimit(nil)
                    }

                    // Labels
                    if !note.labels.isEmpty {
                        FlowLayout(spacing: 6) {
                            ForEach(note.labels, id: \.self) { label in
                                HStack(spacing: 4) {
                                    Text(label)
                                        .font(.system(size: 12))
                                    Image(systemName: "xmark")
                                        .font(.system(size: 8, weight: .bold))
                                        .foregroundStyle(.secondary)
                                }
                                .padding(.horizontal, 10)
                                .padding(.vertical, 5)
                                .background(Color.black.opacity(0.06))
                                .clipShape(Capsule())
                                .onTapGesture { note.labels.removeAll { $0 == label } }
                            }
                        }
                    }
                }
                .padding()
            }
            .background(note.backgroundColor)
            .navigationBarTitleDisplayMode(.inline)
            .toolbar {
                ToolbarItem(placement: .topBarLeading) {
                    Button { save(); dismiss() } label: {
                        Image(systemName: "arrow.left")
                    }
                }
                ToolbarItem(placement: .topBarTrailing) {
                    Button { note.pinned.toggle() } label: {
                        Image(systemName: note.pinned ? "pin.fill" : "pin")
                    }
                }
            }
            .toolbar {
                ToolbarItemGroup(placement: .bottomBar) {
                    toolbarButtons
                }
            }
            .sheet(isPresented: $showColors) { colorPicker }
            .sheet(isPresented: $showLabels) { labelPicker }
            .onAppear { titleFocused = true }
        }
    }

    @ViewBuilder
    private var toolbarButtons: some View {
        Button { showColors = true } label: { Image(systemName: "paintpalette") }
        Button { showLabels = true } label: { Image(systemName: "tag") }
        Button { toggleType() } label: {
            Image(systemName: note.type == .text ? "checklist" : "text.alignleft")
        }
        Button { note.archived.toggle(); save(); dismiss() } label: {
            Image(systemName: note.archived ? "tray.and.arrow.up" : "archivebox")
        }
        Button(role: .destructive) { onDelete(); dismiss() } label: {
            Image(systemName: "trash")
        }
        Spacer()
        Text(formatDate(note.updatedAt))
            .font(.system(size: 11))
            .foregroundStyle(.secondary)
    }

    @ViewBuilder
    private var colorPicker: some View {
        NavigationStack {
            LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 4), spacing: 12) {
                ForEach(Constants.colors) { c in
                    Circle()
                        .fill(Color(hex: c.value))
                        .frame(width: 44, height: 44)
                        .overlay(Circle().stroke(note.color == c.value ? Color.blue : Color(hex: "#e0e0e0"), lineWidth: note.color == c.value ? 2 : 0.5))
                        .onTapGesture { note.color = c.value }
                }
            }
            .padding()
            .navigationTitle("Color")
            .navigationBarTitleDisplayMode(.inline)
            .toolbar { ToolbarItem(placement: .confirmationAction) { Button("Done") { showColors = false } } }
        }
        .presentationDetents([.height(200)])
    }

    @ViewBuilder
    private var labelPicker: some View {
        NavigationStack {
            LabelPickerView(allLabels: labels, selected: $note.labels, onCreateLabel: onAddLabel)
                .navigationTitle("Labels")
                .navigationBarTitleDisplayMode(.inline)
                .toolbar { ToolbarItem(placement: .confirmationAction) { Button("Done") { showLabels = false } } }
        }
        .presentationDetents([.medium])
    }

    private func toggleType() {
        if note.type == .text {
            let lines = note.content.components(separatedBy: "\n").filter { !$0.trimmingCharacters(in: .whitespaces).isEmpty }
            note.checklist = lines.isEmpty ? [ChecklistItem()] : lines.map { ChecklistItem(text: $0) }
            note.content = ""
            note.type = .checklist
        } else {
            note.content = note.checklist.map(\.text).filter { !$0.isEmpty }.joined(separator: "\n")
            note.checklist = []
            note.type = .text
        }
    }

    private func save() {
        note.updatedAt = Date().timeIntervalSince1970 * 1000
        onSave()
    }

    private func formatDate(_ ts: Double) -> String {
        let date = Date(timeIntervalSince1970: ts / 1000)
        let fmt = DateFormatter()
        fmt.dateStyle = .medium
        fmt.timeStyle = .short
        return "Edited \(fmt.string(from: date))"
    }
}
