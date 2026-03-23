import Foundation

/// Local JSON persistence — mirrors IndexedDB on web.
/// Stores notes and labels as JSON files in the app's documents directory.
actor LocalStore {
    private let notesURL: URL
    private let labelsURL: URL

    init(directory: URL? = nil) {
        let dir = directory ?? FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first!.appendingPathComponent("keep-data")
        try? FileManager.default.createDirectory(at: dir, withIntermediateDirectories: true)
        notesURL = dir.appendingPathComponent("notes.json")
        labelsURL = dir.appendingPathComponent("labels.json")
    }

    func loadNotes() -> [Note] {
        guard let data = try? Data(contentsOf: notesURL),
              let notes = try? JSONDecoder().decode([Note].self, from: data) else { return [] }
        return notes
    }

    func saveNotes(_ notes: [Note]) {
        guard let data = try? JSONEncoder().encode(notes) else { return }
        try? data.write(to: notesURL, options: .atomic)
    }

    func loadLabels() -> [String] {
        guard let data = try? Data(contentsOf: labelsURL),
              let labels = try? JSONDecoder().decode([String].self, from: data) else { return [] }
        return labels
    }

    func saveLabels(_ labels: [String]) {
        guard let data = try? JSONEncoder().encode(labels) else { return }
        try? data.write(to: labelsURL, options: .atomic)
    }
}
