import SwiftUI

struct NoteCardView: View {
    let note: Note
    let isSelected: Bool
    let selectionActive: Bool
    var onTap: () -> Void = {}
    var onToggleSelect: () -> Void = {}
    var onToggleCheck: (String) -> Void = { _ in }

    var body: some View {
        VStack(alignment: .leading, spacing: 6) {
            if !note.title.isEmpty {
                Text(note.title)
                    .font(.system(size: 16, weight: .medium))
                    .foregroundStyle(Color(hex: "#202124"))
                    .lineLimit(nil)
            }

            if note.type == .checklist {
                checklistPreview
            } else if !note.content.isEmpty && !isEmptyHtml(note.content) {
                Text(stripHtml(note.content))
                    .font(.custom("Google Sans Text", size: 14))
                    .foregroundStyle(Color(hex: "#3c4043"))
                    .lineLimit(nil)
            }

            if !note.labels.isEmpty {
                FlowLayout(spacing: 4) {
                    ForEach(note.labels, id: \.self) { label in
                        Text(label)
                            .font(.system(size: 11))
                            .foregroundStyle(Color(hex: "#3c4043"))
                            .padding(.horizontal, 8)
                            .padding(.vertical, 4)
                            .background(Color.black.opacity(0.06))
                            .clipShape(Capsule())
                    }
                }
                .padding(.top, 4)
            }
        }
        .frame(maxWidth: .infinity, alignment: .leading)
        .padding(12)
        .background(note.backgroundColor)
        .clipShape(RoundedRectangle(cornerRadius: 8))
        .overlay(
            RoundedRectangle(cornerRadius: 8)
                .stroke(isSelected ? Color.blue : Color(hex: "#e0e0e0"), lineWidth: isSelected ? 2 : 0.5)
        )
        .contentShape(Rectangle())
        .onTapGesture {
            if selectionActive { onToggleSelect() } else { onTap() }
        }
        .onLongPressGesture { onToggleSelect() }
    }

    @ViewBuilder
    private var checklistPreview: some View {
        let unchecked = note.checklist.filter { !$0.checked }.sorted { $0.sortOrder < $1.sortOrder }
        let checkedCount = note.checklist.filter(\.checked).count

        VStack(alignment: .leading, spacing: 2) {
            ForEach(unchecked) { item in
                HStack(alignment: .top, spacing: 8) {
                    Image(systemName: item.checked ? "checkmark.square" : "square")
                        .font(.system(size: 16))
                        .foregroundStyle(Color.black.opacity(0.54))
                        .onTapGesture { onToggleCheck(item.id) }
                    Text(item.text.isEmpty ? " " : item.text)
                        .font(.custom("Google Sans Text", size: 14))
                        .foregroundStyle(item.checked ? Color(hex: "#80868b") : Color(hex: "#3c4043"))
                        .strikethrough(item.checked)
                }
                .padding(.vertical, 1)
            }
            if checkedCount > 0 {
                Text("+\(checkedCount) checked")
                    .font(.system(size: 12))
                    .foregroundStyle(Color(hex: "#80868b"))
                    .padding(.leading, 24)
                    .padding(.top, 2)
            }
        }
    }
}

// MARK: - Helpers

private func isEmptyHtml(_ html: String) -> Bool {
    html.replacingOccurrences(of: "<[^>]*>", with: "", options: .regularExpression)
        .replacingOccurrences(of: "\\s", with: "", options: .regularExpression)
        .replacingOccurrences(of: "&nbsp;", with: "")
        .isEmpty
}

private func stripHtml(_ html: String) -> String {
    html.replacingOccurrences(of: "<[^>]+>", with: "\n", options: .regularExpression)
        .components(separatedBy: "\n")
        .map { $0.trimmingCharacters(in: .whitespaces) }
        .filter { !$0.isEmpty }
        .joined(separator: "\n")
}

// MARK: - FlowLayout for label chips

struct FlowLayout: Layout {
    var spacing: CGFloat = 4

    func sizeThatFits(proposal: ProposedViewSize, subviews: Subviews, cache: inout ()) -> CGSize {
        let result = arrange(proposal: proposal, subviews: subviews)
        return result.size
    }

    func placeSubviews(in bounds: CGRect, proposal: ProposedViewSize, subviews: Subviews, cache: inout ()) {
        let result = arrange(proposal: proposal, subviews: subviews)
        for (idx, pos) in result.positions.enumerated() {
            subviews[idx].place(at: CGPoint(x: bounds.minX + pos.x, y: bounds.minY + pos.y), proposal: .unspecified)
        }
    }

    private func arrange(proposal: ProposedViewSize, subviews: Subviews) -> (size: CGSize, positions: [CGPoint]) {
        let maxW = proposal.width ?? .infinity
        var positions: [CGPoint] = []
        var x: CGFloat = 0, y: CGFloat = 0, rowH: CGFloat = 0
        for sub in subviews {
            let size = sub.sizeThatFits(.unspecified)
            if x + size.width > maxW && x > 0 { x = 0; y += rowH + spacing; rowH = 0 }
            positions.append(CGPoint(x: x, y: y))
            rowH = max(rowH, size.height)
            x += size.width + spacing
        }
        return (CGSize(width: maxW, height: y + rowH), positions)
    }
}
