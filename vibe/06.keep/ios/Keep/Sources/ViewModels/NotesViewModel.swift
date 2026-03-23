import Foundation
import SwiftUI

@MainActor
final class NotesViewModel: ObservableObject {
    @Published var notes: [Note] = []
    @Published var labels: [String] = []
    @Published var search = ""
    @Published var currentView: NoteView = .notes
    @Published var activeLabel = ""
    @Published var selectedIds: Set<String> = []

    private let store = LocalStore()

    init() {
        Task { await load() }
    }

    // MARK: - Persistence

    private func load() async {
        notes = await store.loadNotes()
        labels = await store.loadLabels()
    }

    private func persist() {
        Task {
            await store.saveNotes(notes)
            await store.saveLabels(labels)
        }
    }

    // MARK: - Filtered notes

    var filteredNotes: (pinned: [Note], unpinned: [Note]) {
        let base: [Note]
        switch currentView {
        case .notes:
            base = notes.filter { !$0.archived && !$0.deleted }
        case .archive:
            base = notes.filter { $0.archived && !$0.deleted }
        case .trash:
            base = notes.filter { $0.deleted }
        case .label:
            base = notes.filter { !$0.archived && !$0.deleted && $0.labels.contains(activeLabel) }
        }

        let searched = search.isEmpty ? base : base.filter { note in
            let text = [note.title, note.content, note.checklist.map(\.text).joined(separator: " "), note.labels.joined(separator: " ")].joined(separator: " ").lowercased()
            return text.contains(search.lowercased())
        }

        let sorted = searched.sorted { $0.sortOrder > $1.sortOrder }
        return (
            pinned: sorted.filter(\.pinned),
            unpinned: sorted.filter { !$0.pinned }
        )
    }

    // MARK: - CRUD

    func add(_ partial: Note = Note()) -> Note {
        var note = partial
        if note.id == partial.id && notes.contains(where: { $0.id == note.id }) {
            note.id = UUID().uuidString
        }
        notes.append(note)
        persist()
        return note
    }

    func save(_ note: Note) {
        var updated = note
        updated.updatedAt = Date().timeIntervalSince1970 * 1000
        if let idx = notes.firstIndex(where: { $0.id == note.id }) {
            notes[idx] = updated
        }
        persist()
    }

    func delete(_ id: String) {
        guard let idx = notes.firstIndex(where: { $0.id == id }) else { return }
        if notes[idx].deleted {
            notes.remove(at: idx)
        } else {
            notes[idx].deleted = true
            notes[idx].deletedAt = Date().timeIntervalSince1970 * 1000
            notes[idx].updatedAt = Date().timeIntervalSince1970 * 1000
        }
        persist()
    }

    func restore(_ id: String) {
        guard let idx = notes.firstIndex(where: { $0.id == id }) else { return }
        notes[idx].deleted = false
        notes[idx].deletedAt = nil
        notes[idx].updatedAt = Date().timeIntervalSince1970 * 1000
        persist()
    }

    func emptyTrash() {
        notes.removeAll { $0.deleted }
        persist()
    }

    // MARK: - Selection

    func toggleSelect(_ id: String) {
        if selectedIds.contains(id) { selectedIds.remove(id) } else { selectedIds.insert(id) }
    }

    func clearSelection() { selectedIds.removeAll() }

    func bulkDelete() {
        for id in selectedIds { delete(id) }
        clearSelection()
    }

    func bulkArchive() {
        for id in selectedIds {
            if let idx = notes.firstIndex(where: { $0.id == id }) {
                notes[idx].archived = true
                notes[idx].updatedAt = Date().timeIntervalSince1970 * 1000
            }
        }
        clearSelection()
        persist()
    }

    func bulkPin(_ pinned: Bool) {
        for id in selectedIds {
            if let idx = notes.firstIndex(where: { $0.id == id }) {
                notes[idx].pinned = pinned
                notes[idx].updatedAt = Date().timeIntervalSince1970 * 1000
            }
        }
        clearSelection()
        persist()
    }

    // MARK: - Labels

    func addLabel(_ label: String) {
        guard !labels.contains(label) else { return }
        labels.append(label)
        persist()
    }

    func removeLabel(_ label: String) {
        labels.removeAll { $0 == label }
        for i in notes.indices { notes[i].labels.removeAll { $0 == label } }
        persist()
    }

    func renameLabel(_ old: String, to new: String) {
        if let idx = labels.firstIndex(of: old) { labels[idx] = new }
        for i in notes.indices {
            if let li = notes[i].labels.firstIndex(of: old) { notes[i].labels[li] = new }
        }
        persist()
    }
}
