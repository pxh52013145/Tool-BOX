export enum ItemType {
  FOLDER = 'FOLDER',
  FILE = 'FILE'
}

export enum ToolType {
  NONE = 'NONE',
  GEMINI_TERMINAL = 'GEMINI_TERMINAL',
  SYSTEM_MONITOR = 'SYSTEM_MONITOR',
  README = 'README'
}

export interface FileSystemItem {
  id: string;
  name: string;
  type: ItemType;
  toolType?: ToolType;
  children?: FileSystemItem[];
  content?: string; // For text files
  icon?: React.ReactNode;
  description?: string;
}

export interface Breadcrumb {
  id: string;
  name: string;
}