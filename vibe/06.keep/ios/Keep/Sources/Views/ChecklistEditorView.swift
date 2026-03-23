import SwiftUI

struct ChecklistEditorView: View {
    @Binding var items: [ChecklistItem]
    @FocusState private var focusedId: String?

    private var sorted: [ChecklistItem] {
        items.sorted { $0.sortOrder < $1.sortOrder }
    }

    var body: some View {
        VStack(alignment: .leading, spacing: 0) {
            ForEach(sorted) { item in
                checklistRow(item)
            }
            Button { addItem() } label: {
                HStack(spacing: 8) {
                    Image(systemName: "plus")
                        .font(.system(size: 14))
                        .foregroundStyle(Color(hex: "#80868b"))
                    Text("List item")
                        .font(.system(size: 14))
                        .foregroundStyle(Color(hex: "#80868b"))
                }
                .padding(.leading, 28)
                .padding(.vertical, 6)
            }
        }
    }

    @ViewBuilder
    private func checklistRow(_ item: ChecklistItem) -> some View {
        HStack(alignment: .top, spacing: 8) {
            Button { toggleCheck(item.id) } label: {
                Image(systemName: item.checked ? "checkmark.square" : "square")
                    .font(.system(size: 18))
                    .foregroundStyle(Color.black.opacity(0.54))
            }
            .buttonStyle(.plain)
            .padding(.top, 3)

            TextField("List item", text: binding(for: item.id), axis: .vertical)
                .font(.custom("Google Sans Text", size: 14))
                .foregroundStyle(item.checked ? Color(hex: "#80868b") : Color(hex: "#3c4043"))
                .strikethrough(item.checked)
                .focused($focusedId, equals: item.id)
                .onSubmit { addItemAfter(item.id) }

            Button { removeItem(item.id) } label: {
                Image(systemName: "xmark")
                    .font(.system(size: 12))
                    .foregroundStyle(Color(hex: "#80868b"))
            }
            .buttonStyle(.plain)
            .opacity(0.5)
            .padding(.top, 4)
        }
        .padding(.vertical, 2)
    }

    private func binding(for id: String) -> Binding<String> {
        Binding(
            get: { items.first { $0.id == id }?.text ?? "" },
            set: { newVal in if let idx = items.firstIndex(where: { $0.id == id }) { items[idx].text = newVal } }
        )
    }

    private func toggleCheck(_ id: String) {
        if let idx = items.firstIndex(where: { $0.id == id }) { items[idx].checked.toggle() }
    }

    private func addItem() {
        let item = ChecklistItem()
        items.append(item)
        focusedId = item.id
    }

    private func addItemAfter(_ afterId: String) {
        guard let idx = items.firstIndex(where: { $0.id == afterId }) else { return }
        let newItem = ChecklistItem(sortOrder: items[idx].sortOrder + 1)
        items.insert(newItem, at: idx + 1)
        focusedId = newItem.id
    }

    private func removeItem(_ id: String) {
        items.removeAll { $0.id == id }
    }
}
