import SwiftUI

struct LabelPickerView: View {
    let allLabels: [String]
    @Binding var selected: [String]
    var onCreateLabel: (String) -> Void = { _ in }
    @State private var search = ""

    private var filtered: [String] {
        search.isEmpty ? allLabels : allLabels.filter { $0.localizedCaseInsensitiveContains(search) }
    }

    private var canCreate: Bool {
        let q = search.trimmingCharacters(in: .whitespaces)
        return !q.isEmpty && !allLabels.contains(where: { $0.caseInsensitiveCompare(q) == .orderedSame })
    }

    var body: some View {
        List {
            Section {
                TextField("Enter label name", text: $search)
                    .font(.system(size: 14))
            }

            Section {
                ForEach(filtered, id: \.self) { label in
                    Button {
                        if selected.contains(label) { selected.removeAll { $0 == label } }
                        else { selected.append(label) }
                    } label: {
                        HStack {
                            Image(systemName: selected.contains(label) ? "checkmark.square.fill" : "square")
                                .foregroundStyle(selected.contains(label) ? .blue : .secondary)
                            Text(label).foregroundStyle(.primary)
                        }
                        .font(.system(size: 14))
                    }
                }

                if canCreate {
                    Button {
                        let name = search.trimmingCharacters(in: .whitespaces)
                        onCreateLabel(name)
                        selected.append(name)
                        search = ""
                    } label: {
                        HStack {
                            Image(systemName: "plus")
                            Text("Create \"\(search.trimmingCharacters(in: .whitespaces))\"")
                        }
                        .font(.system(size: 14))
                        .foregroundStyle(.blue)
                    }
                }
            }
        }
    }
}
