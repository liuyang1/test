import XCTest
@testable import Keep

final class NoteTests: XCTestCase {

    func testNoteDefaults() {
        let note = Note()
        XCTAssertFalse(note.id.isEmpty)
        XCTAssertEqual(note.title, "")
        XCTAssertEqual(note.content, "")
        XCTAssertEqual(note.type, .text)
        XCTAssertTrue(note.checklist.isEmpty)
        XCTAssertEqual(note.color, "#ffffff")
        XCTAssertFalse(note.pinned)
        XCTAssertFalse(note.archived)
        XCTAssertFalse(note.deleted)
        XCTAssertNil(note.deletedAt)
        XCTAssertTrue(note.labels.isEmpty)
    }

    func testNoteUniqueIds() {
        let a = Note()
        let b = Note()
        XCTAssertNotEqual(a.id, b.id)
    }

    func testChecklistItemDefaults() {
        let item = ChecklistItem()
        XCTAssertFalse(item.id.isEmpty)
        XCTAssertEqual(item.text, "")
        XCTAssertFalse(item.checked)
    }

    func testChecklistItemWithText() {
        let item = ChecklistItem(text: "Buy milk")
        XCTAssertEqual(item.text, "Buy milk")
        XCTAssertFalse(item.checked)
    }

    func testNoteEncodeDecode() throws {
        let note = Note(title: "Test", content: "Hello", labels: ["work"])
        let data = try JSONEncoder().encode(note)
        let decoded = try JSONDecoder().decode(Note.self, from: data)
        XCTAssertEqual(decoded.title, "Test")
        XCTAssertEqual(decoded.content, "Hello")
        XCTAssertEqual(decoded.labels, ["work"])
        XCTAssertEqual(decoded.id, note.id)
    }

    func testConstants() {
        XCTAssertEqual(Constants.colors.count, 12)
        XCTAssertEqual(Constants.colors.first?.value, "#ffffff")
        XCTAssertEqual(Constants.trashRetentionDays, 30)
    }

    func testColorHexInit() {
        // Just verify it doesn't crash
        let _ = Note(color: "#faafa8").backgroundColor
        let _ = Note().backgroundColor
    }
}
