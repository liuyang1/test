import Foundation

struct ChecklistItem: Identifiable, Codable, Equatable, Hashable {
    var id: String
    var text: String
    var checked: Bool
    var sortOrder: Double

    init(id: String = UUID().uuidString, text: String = "", checked: Bool = false, sortOrder: Double = Date().timeIntervalSince1970 * 1000) {
        self.id = id
        self.text = text
        self.checked = checked
        self.sortOrder = sortOrder
    }
}

struct Note: Identifiable, Codable, Equatable {
    var id: String
    var title: String
    var content: String
    var type: NoteType
    var checklist: [ChecklistItem]
    var color: String
    var background: String
    var pinned: Bool
    var archived: Bool
    var deleted: Bool
    var deletedAt: Double?
    var labels: [String]
    var sortOrder: Double
    var createdAt: Double
    var updatedAt: Double

    init(
        id: String = UUID().uuidString,
        title: String = "",
        content: String = "",
        type: NoteType = .text,
        checklist: [ChecklistItem] = [],
        color: String = "#ffffff",
        background: String = "",
        pinned: Bool = false,
        archived: Bool = false,
        deleted: Bool = false,
        deletedAt: Double? = nil,
        labels: [String] = [],
        sortOrder: Double = Date().timeIntervalSince1970 * 1000,
        createdAt: Double = Date().timeIntervalSince1970 * 1000,
        updatedAt: Double = Date().timeIntervalSince1970 * 1000
    ) {
        self.id = id
        self.title = title
        self.content = content
        self.type = type
        self.checklist = checklist
        self.color = color
        self.background = background
        self.pinned = pinned
        self.archived = archived
        self.deleted = deleted
        self.deletedAt = deletedAt
        self.labels = labels
        self.sortOrder = sortOrder
        self.createdAt = createdAt
        self.updatedAt = updatedAt
    }
}

enum NoteType: String, Codable {
    case text
    case checklist
}

enum NoteView: String {
    case notes, archive, trash, label
}

struct NoteColor: Identifiable {
    let id: String
    let name: String
    let value: String

    init(_ name: String, _ value: String) {
        self.id = value
        self.name = name
        self.value = value
    }
}

enum Constants {
    static let colors: [NoteColor] = [
        NoteColor("Default", "#ffffff"),
        NoteColor("Coral", "#faafa8"),
        NoteColor("Peach", "#f39f76"),
        NoteColor("Sand", "#fff8b8"),
        NoteColor("Mint", "#e2f6d3"),
        NoteColor("Sage", "#b4ddd3"),
        NoteColor("Fog", "#d3e4ec"),
        NoteColor("Storm", "#aeccdc"),
        NoteColor("Dusk", "#d3bfdb"),
        NoteColor("Blossom", "#f6e2dd"),
        NoteColor("Clay", "#e9e3d4"),
        NoteColor("Chalk", "#efeff1"),
    ]

    static let trashRetentionDays = 30
}
