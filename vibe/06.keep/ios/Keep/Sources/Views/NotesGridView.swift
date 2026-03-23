import SwiftUI

struct NotesGridView: View {
    @ObservedObject var vm: NotesViewModel
    var onSelect: (Note) -> Void

    private let columns = [GridItem(.adaptive(minimum: 160), spacing: 8)]

    var body: some View {
        let (pinned, unpinned) = vm.filteredNotes

        ScrollView {
            LazyVStack(alignment: .leading, spacing: 0) {
                if !pinned.isEmpty {
                    sectionHeader("Pinned")
                    grid(pinned)
                }
                if !pinned.isEmpty && !unpinned.isEmpty {
                    sectionHeader("Others")
                }
                if !unpinned.isEmpty {
                    grid(unpinned)
                }
                if pinned.isEmpty && unpinned.isEmpty {
                    emptyState
                }
            }
            .padding(.horizontal, 8)
            .padding(.top, 8)
        }
    }

    @ViewBuilder
    private func sectionHeader(_ title: String) -> some View {
        Text(title)
            .font(.system(size: 11, weight: .medium))
            .foregroundStyle(Color(hex: "#5f6368"))
            .textCase(.uppercase)
            .padding(.horizontal, 8)
            .padding(.top, 12)
            .padding(.bottom, 4)
    }

    @ViewBuilder
    private func grid(_ notes: [Note]) -> some View {
        LazyVGrid(columns: columns, spacing: 8) {
            ForEach(notes) { note in
                NoteCardView(
                    note: note,
                    isSelected: vm.selectedIds.contains(note.id),
                    selectionActive: !vm.selectedIds.isEmpty,
                    onTap: { onSelect(note) },
                    onToggleSelect: { vm.toggleSelect(note.id) },
                    onToggleCheck: { itemId in
                        var updated = note
                        if let idx = updated.checklist.firstIndex(where: { $0.id == itemId }) {
                            updated.checklist[idx].checked.toggle()
                            vm.save(updated)
                        }
                    }
                )
            }
        }
    }

    @ViewBuilder
    private var emptyState: some View {
        VStack(spacing: 12) {
            Image(systemName: vm.currentView == .trash ? "trash" : vm.currentView == .archive ? "archivebox" : "lightbulb")
                .font(.system(size: 48))
                .foregroundStyle(Color(hex: "#e0e0e0"))
            Text(vm.currentView == .trash ? "No notes in Trash" : vm.currentView == .archive ? "No archived notes" : "Notes you add appear here")
                .font(.system(size: 14))
                .foregroundStyle(Color(hex: "#80868b"))
        }
        .frame(maxWidth: .infinity)
        .padding(.top, 80)
    }
}
