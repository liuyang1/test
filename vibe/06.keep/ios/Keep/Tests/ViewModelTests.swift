import XCTest
@testable import Keep

@MainActor
final class ViewModelTests: XCTestCase {

    func testAddNote() {
        let vm = NotesViewModel()
        let note = vm.add(Note(title: "Test"))
        XCTAssertEqual(vm.notes.count, 1)
        XCTAssertEqual(vm.notes.first?.title, "Test")
        XCTAssertEqual(vm.notes.first?.id, note.id)
    }

    func testDeleteNote() {
        let vm = NotesViewModel()
        let note = vm.add(Note(title: "Del"))
        vm.delete(note.id)
        XCTAssertTrue(vm.notes.first?.deleted ?? false)
    }

    func testPermanentDelete() {
        let vm = NotesViewModel()
        let note = vm.add(Note(title: "Perm"))
        vm.delete(note.id) // soft delete
        vm.delete(note.id) // permanent
        XCTAssertTrue(vm.notes.isEmpty)
    }

    func testRestore() {
        let vm = NotesViewModel()
        let note = vm.add(Note(title: "Restore"))
        vm.delete(note.id)
        vm.restore(note.id)
        XCTAssertFalse(vm.notes.first?.deleted ?? true)
        XCTAssertNil(vm.notes.first?.deletedAt)
    }

    func testEmptyTrash() {
        let vm = NotesViewModel()
        let n1 = vm.add(Note(title: "A"))
        let _ = vm.add(Note(title: "B"))
        vm.delete(n1.id)
        vm.emptyTrash()
        XCTAssertEqual(vm.notes.count, 1)
        XCTAssertEqual(vm.notes.first?.title, "B")
    }

    func testFilteredNotes() {
        let vm = NotesViewModel()
        let _ = vm.add(Note(title: "Normal"))
        let _ = vm.add(Note(title: "Pinned", pinned: true))
        let _ = vm.add(Note(title: "Archived", archived: true))
        vm.currentView = .notes
        let (pinned, unpinned) = vm.filteredNotes
        XCTAssertEqual(pinned.count, 1)
        XCTAssertEqual(unpinned.count, 1)
        XCTAssertEqual(pinned.first?.title, "Pinned")
    }

    func testSearch() {
        let vm = NotesViewModel()
        let _ = vm.add(Note(title: "Apple"))
        let _ = vm.add(Note(title: "Banana"))
        vm.search = "apple"
        let (_, unpinned) = vm.filteredNotes
        XCTAssertEqual(unpinned.count, 1)
        XCTAssertEqual(unpinned.first?.title, "Apple")
    }

    func testLabels() {
        let vm = NotesViewModel()
        vm.addLabel("work")
        vm.addLabel("personal")
        XCTAssertEqual(vm.labels, ["work", "personal"])
        vm.addLabel("work") // duplicate
        XCTAssertEqual(vm.labels.count, 2)
    }

    func testRemoveLabel() {
        let vm = NotesViewModel()
        vm.addLabel("work")
        let _ = vm.add(Note(title: "T", labels: ["work"]))
        vm.removeLabel("work")
        XCTAssertTrue(vm.labels.isEmpty)
        XCTAssertTrue(vm.notes.first?.labels.isEmpty ?? false)
    }

    func testRenameLabel() {
        let vm = NotesViewModel()
        vm.addLabel("old")
        let _ = vm.add(Note(title: "T", labels: ["old"]))
        vm.renameLabel("old", to: "new")
        XCTAssertEqual(vm.labels, ["new"])
        XCTAssertEqual(vm.notes.first?.labels, ["new"])
    }

    func testSelection() {
        let vm = NotesViewModel()
        let n1 = vm.add(Note(title: "A"))
        let n2 = vm.add(Note(title: "B"))
        vm.toggleSelect(n1.id)
        XCTAssertEqual(vm.selectedIds.count, 1)
        vm.toggleSelect(n2.id)
        XCTAssertEqual(vm.selectedIds.count, 2)
        vm.toggleSelect(n1.id)
        XCTAssertEqual(vm.selectedIds.count, 1)
        vm.clearSelection()
        XCTAssertTrue(vm.selectedIds.isEmpty)
    }

    func testBulkArchive() {
        let vm = NotesViewModel()
        let n1 = vm.add(Note(title: "A"))
        let n2 = vm.add(Note(title: "B"))
        vm.toggleSelect(n1.id)
        vm.toggleSelect(n2.id)
        vm.bulkArchive()
        XCTAssertTrue(vm.notes.allSatisfy(\.archived))
        XCTAssertTrue(vm.selectedIds.isEmpty)
    }

    func testBulkDelete() {
        let vm = NotesViewModel()
        let n1 = vm.add(Note(title: "A"))
        let n2 = vm.add(Note(title: "B"))
        vm.toggleSelect(n1.id)
        vm.toggleSelect(n2.id)
        vm.bulkDelete()
        XCTAssertTrue(vm.notes.allSatisfy(\.deleted))
        XCTAssertTrue(vm.selectedIds.isEmpty)
    }

    func testLabelFilter() {
        let vm = NotesViewModel()
        let _ = vm.add(Note(title: "Work", labels: ["work"]))
        let _ = vm.add(Note(title: "Personal", labels: ["personal"]))
        vm.currentView = .label
        vm.activeLabel = "work"
        let (_, unpinned) = vm.filteredNotes
        XCTAssertEqual(unpinned.count, 1)
        XCTAssertEqual(unpinned.first?.title, "Work")
    }
}
