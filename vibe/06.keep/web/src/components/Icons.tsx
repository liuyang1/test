// Using Material Symbols Outlined font (same as Google Keep)
// Font loaded in index.html

interface Props extends React.HTMLAttributes<HTMLSpanElement> {
  size?: number
  filled?: boolean
}

function Icon({ children, size = 20, filled, className = '', style, ...rest }: Props & { children: string }) {
  return (
    <span
      className={`material-symbols-outlined ${className}`}
      style={{ fontSize: size, fontVariationSettings: filled ? "'FILL' 1" : "'FILL' 0", lineHeight: 1, ...style }}
      {...rest}
    >
      {children}
    </span>
  )
}

// Keep-specific icons
export const PinIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>push_pin</Icon>
export const PaletteIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>palette</Icon>
export const LabelIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>label</Icon>
export const CheckBoxIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>check_box_outline_blank</Icon>
export const TextIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>text_fields</Icon>
export const ArchiveIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>archive</Icon>
export const UnarchiveIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>unarchive</Icon>
export const DeleteIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>delete</Icon>
export const SearchIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>search</Icon>
export const MenuIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>menu</Icon>
export const CloseIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>close</Icon>
export const RestoreIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>restore</Icon>
export const LightbulbIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>lightbulb</Icon>
export const AddIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>add</Icon>
export const CheckIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>check</Icon>
export const GridViewIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>grid_view</Icon>
export const ListViewIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>view_agenda</Icon>
export const SettingsIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>settings</Icon>
export const DragIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>drag_indicator</Icon>
export const MoreIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>more_vert</Icon>
export const ImageIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>image</Icon>
export const PersonIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>person_add</Icon>
export const UndoIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>undo</Icon>
export const RedoIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>redo</Icon>
export const CheckCircleIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>check_circle</Icon>
export const EditIcon = (p: Omit<Props, 'children'>) => <Icon {...p}>edit</Icon>
