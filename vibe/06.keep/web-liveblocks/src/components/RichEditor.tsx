import { useEditor, EditorContent, Editor } from '@tiptap/react'
import StarterKit from '@tiptap/starter-kit'
import Placeholder from '@tiptap/extension-placeholder'
import Underline from '@tiptap/extension-underline'
import { forwardRef, useImperativeHandle, useEffect } from 'react'

export interface RichEditorHandle {
  focus: () => void
  getHTML: () => string
  editor: Editor | null
}

interface Props {
  content: string
  onChange: (html: string) => void
  placeholder?: string
  onEditorReady?: (editor: Editor) => void
  onBackspaceAtStart?: () => void
}

export const RichEditor = forwardRef<RichEditorHandle, Props>(({ content, onChange, placeholder = 'Take a note...', onEditorReady, onBackspaceAtStart }, ref) => {
  const editor = useEditor({
    extensions: [
      StarterKit.configure({
        heading: { levels: [1, 2] },
        blockquote: false, codeBlock: false, code: false, horizontalRule: false,
        bulletList: { HTMLAttributes: { class: 'list-disc pl-6' } },
        orderedList: { HTMLAttributes: { class: 'list-decimal pl-6' } },
      }),
      Underline,
      Placeholder.configure({ placeholder }),
    ],
    content,
    onUpdate: ({ editor }) => onChange(editor.getHTML()),
    editorProps: {
      attributes: { class: 'outline-none min-h-[60px] text-[15px] leading-[1.6] text-[#3c4043]' },
      handleKeyDown: (view, event) => {
        if (event.key === 'Backspace' && onBackspaceAtStart) {
          const { from, empty } = view.state.selection
          if (empty && from === 1 && view.state.doc.textContent === '') {
            onBackspaceAtStart()
            return true
          }
        }
        return false
      },
    },
    onCreate: ({ editor }) => onEditorReady?.(editor),
  })

  useImperativeHandle(ref, () => ({
    focus: () => { editor?.commands.focus(); return editor?.isFocused ?? false },
    getHTML: () => editor?.getHTML() || '',
    editor,
  }), [editor])

  useEffect(() => {
    if (editor && content !== editor.getHTML()) editor.commands.setContent(content)
  }, [content])

  return <EditorContent editor={editor} />
})

// ─── Format toolbar (separate row above tool buttons, Google Keep style) ───
export function FormatBar({ editor }: { editor: Editor | null }) {
  if (!editor) return null

  return (
    <div className="flex items-center gap-1 px-3 py-1.5 border-t border-black/[0.06] overflow-x-auto" data-testid="format-bar">
      <FBtn title="Bold" active={editor.isActive('bold')} onClick={() => editor.chain().focus().toggleBold().run()}>
        <span className="font-bold text-[14px]">B</span>
      </FBtn>
      <FBtn title="Italic" active={editor.isActive('italic')} onClick={() => editor.chain().focus().toggleItalic().run()}>
        <span className="italic text-[14px] font-serif">I</span>
      </FBtn>
      <FBtn title="Underline" active={editor.isActive('underline')} onClick={() => editor.chain().focus().toggleUnderline().run()}>
        <span className="underline text-[14px]">U</span>
      </FBtn>
      <FBtn title="Strikethrough" active={editor.isActive('strike')} onClick={() => editor.chain().focus().toggleStrike().run()}>
        <span className="line-through text-[14px]">S</span>
      </FBtn>

      <span className="w-px h-5 bg-[#dadce0] mx-1" />

      <FBtn title="Title" active={editor.isActive('heading', { level: 1 })} onClick={() => editor.chain().focus().toggleHeading({ level: 1 }).run()}>
        <span className="text-[16px] font-medium leading-none">T</span>
      </FBtn>
      <FBtn title="Subtitle" active={editor.isActive('heading', { level: 2 })} onClick={() => editor.chain().focus().toggleHeading({ level: 2 }).run()}>
        <span className="text-[12px] font-medium leading-none">T</span>
      </FBtn>

      <span className="w-px h-5 bg-[#dadce0] mx-1" />

      <FBtn title="Bullet list" active={editor.isActive('bulletList')} onClick={() => editor.chain().focus().toggleBulletList().run()}>
        <svg width="18" height="18" viewBox="0 0 24 24" fill="currentColor"><circle cx="4" cy="7" r="2"/><circle cx="4" cy="12" r="2"/><circle cx="4" cy="17" r="2"/><rect x="8" y="6" width="13" height="2" rx="1"/><rect x="8" y="11" width="13" height="2" rx="1"/><rect x="8" y="16" width="13" height="2" rx="1"/></svg>
      </FBtn>
      <FBtn title="Numbered list" active={editor.isActive('orderedList')} onClick={() => editor.chain().focus().toggleOrderedList().run()}>
        <svg width="18" height="18" viewBox="0 0 24 24" fill="currentColor"><text x="1" y="9" fontSize="8" fontWeight="500">1</text><text x="1" y="14.5" fontSize="8" fontWeight="500">2</text><text x="1" y="20" fontSize="8" fontWeight="500">3</text><rect x="8" y="6" width="13" height="2" rx="1"/><rect x="8" y="11" width="13" height="2" rx="1"/><rect x="8" y="16" width="13" height="2" rx="1"/></svg>
      </FBtn>
    </div>
  )
}

function FBtn({ children, title, active, onClick }: {
  children: React.ReactNode; title: string; active: boolean; onClick: () => void
}) {
  return (
    <button title={title} tabIndex={-1} onClick={onClick}
      className={`w-8 h-8 rounded flex items-center justify-center transition-colors ${
        active ? 'bg-[#d2e3fc] text-[#1967d2]' : 'text-[#5f6368] hover:bg-[#f1f3f4]'
      }`}>
      {children}
    </button>
  )
}
