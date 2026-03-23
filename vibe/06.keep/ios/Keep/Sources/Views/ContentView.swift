import SwiftUI

public struct ContentView: View {
    @StateObject private var vm = NotesViewModel()
    @State private var editingNote: Note?
    @State private var isCreating = false
    @State private var newNote = Note()
    @State private var showSidebar = false

    public init() {}

    public var body: some View {
        NavigationSplitView {
            sidebar
        } detail: {
            mainContent
        }
    }

    // MARK: - Sidebar

    @ViewBuilder
    private var sidebar: some View {
        List(selection: Binding(get: { vm.currentView.rawValue }, set: { _ in })) {
            Section {
                sidebarItem("Notes", icon: "lightbulb", view: .notes)
            }
            if !vm.labels.isEmpty {
                Section("Labels") {
                    ForEach(vm.labels, id: \.self) { label in
                        Button {
                            vm.currentView = .label
                            vm.activeLabel = label
                        } label: {
                            Label(label, systemImage: "tag")
                                .foregroundStyle(vm.currentView == .label && vm.activeLabel == label ? .blue : .primary)
                        }
                    }
                }
            }
            Section {
                sidebarItem("Archive", icon: "archivebox", view: .archive)
                sidebarItem("Trash", icon: "trash", view: .trash)
            }
        }
        .navigationTitle("Keep")
        .listStyle(.sidebar)
    }

    @ViewBuilder
    private func sidebarItem(_ title: String, icon: String, view: NoteView) -> some View {
        Button {
            vm.currentView = view
            vm.activeLabel = ""
        } label: {
            Label(title, systemImage: icon)
                .foregroundStyle(vm.currentView == view && vm.activeLabel.isEmpty ? .blue : .primary)
        }
    }

    // MARK: - Main content

    @ViewBuilder
    private var mainContent: some View {
        ZStack(alignment: .bottom) {
            VStack(spacing: 0) {
                // Search bar
                HStack {
                    Image(systemName: "magnifyingglass")
                        .foregroundStyle(.secondary)
                    TextField("Search", text: $vm.search)
                        .font(.system(size: 15))
                }
                .padding(10)
                .background(Color(.systemGray6))
                .clipShape(RoundedRectangle(cornerRadius: 8))
                .padding(.horizontal)
                .padding(.top, 8)

                // Notes grid
                NotesGridView(vm: vm) { note in
                    editingNote = note
                }

                // Trash empty button
                if vm.currentView == .trash && !vm.filteredNotes.unpinned.isEmpty {
                    Button("Empty Trash") { vm.emptyTrash() }
                        .foregroundStyle(.red)
                        .padding()
                }
            }

            // Selection bar
            if !vm.selectedIds.isEmpty {
                SelectionBarView(
                    count: vm.selectedIds.count,
                    onPin: { vm.bulkPin(true) },
                    onArchive: { vm.bulkArchive() },
                    onDelete: { vm.bulkDelete() },
                    onClear: { vm.clearSelection() }
                )
                .transition(.move(edge: .bottom))
            }

            // FAB
            if vm.selectedIds.isEmpty {
                HStack {
                    Spacer()
                    Menu {
                        Button { createNote(type: .text) } label: { Label("Text note", systemImage: "square.and.pencil") }
                        Button { createNote(type: .checklist) } label: { Label("Checklist", systemImage: "checklist") }
                    } label: {
                        Image(systemName: "plus")
                            .font(.system(size: 24, weight: .medium))
                            .foregroundStyle(.white)
                            .frame(width: 56, height: 56)
                            .background(Color(hex: "#fbbc04"))
                            .clipShape(RoundedRectangle(cornerRadius: 16))
                            .shadow(color: .black.opacity(0.2), radius: 6, y: 3)
                    }
                    .padding(.trailing, 16)
                    .padding(.bottom, 16)
                }
            }
        }
        .navigationTitle(navTitle)
        .navigationBarTitleDisplayMode(.inline)
        .sheet(item: $editingNote) { note in
            editorSheet(for: note)
        }
        .sheet(isPresented: $isCreating) {
            editorSheet(for: newNote, isNew: true)
        }
    }

    private var navTitle: String {
        switch vm.currentView {
        case .notes: return "Keep"
        case .archive: return "Archive"
        case .trash: return "Trash"
        case .label: return vm.activeLabel
        }
    }

    // MARK: - Editor

    @ViewBuilder
    private func editorSheet(for note: Note, isNew: Bool = false) -> some View {
        let binding = isNew
            ? $newNote
            : Binding(
                get: { vm.notes.first { $0.id == note.id } ?? note },
                set: { vm.save($0) }
            )

        NoteEditorView(
            note: binding,
            labels: vm.labels,
            onSave: {
                if isNew {
                    let hasContent = !newNote.title.isEmpty || !newNote.content.isEmpty || newNote.checklist.contains { !$0.text.isEmpty }
                    if hasContent { _ = vm.add(newNote) }
                    newNote = Note()
                    isCreating = false
                } else {
                    vm.save(binding.wrappedValue)
                }
            },
            onDelete: {
                vm.delete(note.id)
            },
            onAddLabel: { vm.addLabel($0) }
        )
    }

    private func createNote(type: NoteType) {
        newNote = Note(type: type, checklist: type == .checklist ? [ChecklistItem()] : [])
        if vm.currentView == .label && !vm.activeLabel.isEmpty {
            newNote.labels = [vm.activeLabel]
        }
        isCreating = true
    }
}
