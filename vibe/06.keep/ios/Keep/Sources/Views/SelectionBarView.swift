import SwiftUI

struct SelectionBarView: View {
    let count: Int
    var onPin: () -> Void = {}
    var onArchive: () -> Void = {}
    var onDelete: () -> Void = {}
    var onClear: () -> Void = {}

    var body: some View {
        HStack(spacing: 16) {
            Button { onClear() } label: {
                Image(systemName: "xmark")
            }
            Text("\(count) selected")
                .font(.system(size: 16, weight: .medium))
            Spacer()
            Button { onPin() } label: { Image(systemName: "pin") }
            Button { onArchive() } label: { Image(systemName: "archivebox") }
            Button(role: .destructive) { onDelete() } label: { Image(systemName: "trash") }
        }
        .padding(.horizontal, 16)
        .padding(.vertical, 10)
        .background(.ultraThinMaterial)
    }
}
