import React from 'react';
import { FileSystemItem, ItemType, ToolType } from './types';

// Icons as SVG components
export const FolderIcon = () => (
  <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="currentColor" className="w-12 h-12">
    <path d="M19.5 21a3 3 0 0 0 3-3v-4.5a3 3 0 0 0-3-3h-15a3 3 0 0 0-3 3V18a3 3 0 0 0 3 3h15ZM1.5 10.146V6a3 3 0 0 1 3-3h5.379a2.25 2.25 0 0 1 1.59.659l2.122 2.121c.14.141.331.22.53.22H19.5a3 3 0 0 1 3 3v1.146A4.483 4.483 0 0 0 19.5 9h-15a4.483 4.483 0 0 0-3 1.146Z" />
  </svg>
);

export const TerminalIcon = () => (
  <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="currentColor" className="w-12 h-12">
    <path fillRule="evenodd" d="M2.25 6a3 3 0 0 1 3-3h13.5a3 3 0 0 1 3 3v12a3 3 0 0 1-3 3H5.25a3 3 0 0 1-3-3V6Zm3.97 6.22a.75.75 0 0 1 1.06 0l5.25 5.25a.75.75 0 0 1-1.06 1.06L6.22 13.28a.75.75 0 0 1 0-1.06Zm0-4.44a.75.75 0 0 1 1.06 0l5.25 5.25a.75.75 0 0 1-1.06 1.06L6.22 8.84a.75.75 0 0 1 0-1.06ZM15 7.5a.75.75 0 0 1 .75.75v1.5a.75.75 0 0 1-1.5 0v-1.5a.75.75 0 0 1 .75-.75Zm0 3a.75.75 0 0 1 .75.75v1.5a.75.75 0 0 1-1.5 0v-1.5a.75.75 0 0 1 .75-.75ZM15 13.5a.75.75 0 0 1 .75.75v1.5a.75.75 0 0 1-1.5 0v-1.5a.75.75 0 0 1 .75-.75Z" clipRule="evenodd" />
  </svg>
);

export const ChartIcon = () => (
  <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="currentColor" className="w-12 h-12">
    <path fillRule="evenodd" d="M2.25 13.5a8.25 8.25 0 0 1 8.25-8.25.75.75 0 0 1 .75.75v6.75H18a.75.75 0 0 1 .75.75 8.25 8.25 0 0 1-16.5 0Z" clipRule="evenodd" />
    <path fillRule="evenodd" d="M12.75 3a.75.75 0 0 1 .75-.75 8.25 8.25 0 0 1 8.25 8.25.75.75 0 0 1-.75.75h-7.5a.75.75 0 0 1-.75-.75V3Z" clipRule="evenodd" />
  </svg>
);

export const DocIcon = () => (
  <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="currentColor" className="w-12 h-12">
    <path fillRule="evenodd" d="M5.625 1.5H9a3.75 3.75 0 0 1 3.75 3.75v1.875c0 1.036.84 1.875 1.875 1.875H16.5a3.75 3.75 0 0 1 3.75 3.75v7.875c0 1.035-.84 1.875-1.875 1.875H5.625a1.875 1.875 0 0 1-1.875-1.875V3.375c0-1.036.84-1.875 1.875-1.875ZM12.75 12a.75.75 0 0 0-1.5 0v2.25H9a.75.75 0 0 0 0 1.5h2.25V18a.75.75 0 0 0 1.5 0v-2.25H15a.75.75 0 0 0 0-1.5h-2.25V12Z" clipRule="evenodd" />
    <path d="M14.25 5.25a5.23 5.23 0 0 0-1.279-3.434 9.768 9.768 0 0 1 6.963 6.963A5.23 5.23 0 0 0 16.5 7.5h-1.875a.375.375 0 0 1-.375-.375V5.25Z" />
  </svg>
);


export const ROOT_FILE_SYSTEM: FileSystemItem = {
  id: 'root',
  name: 'ROOT',
  type: ItemType.FOLDER,
  children: [
    {
      id: 'apps',
      name: 'APPLICATIONS',
      type: ItemType.FOLDER,
      description: 'Executable modules',
      children: [
        {
          id: 'gemini-term',
          name: 'AI_CONSOLE.EXE',
          type: ItemType.FILE,
          toolType: ToolType.GEMINI_TERMINAL,
          description: 'Gemini 3 Flash Interface',
          icon: <TerminalIcon />
        },
        {
          id: 'sys-mon',
          name: 'VISUALIZER.DAT',
          type: ItemType.FILE,
          toolType: ToolType.SYSTEM_MONITOR,
          description: 'Data Visualization Core',
          icon: <ChartIcon />
        }
      ]
    },
    {
      id: 'docs',
      name: 'DOCUMENTS',
      type: ItemType.FOLDER,
      description: 'System manuals',
      children: [
        {
          id: 'readme',
          name: 'README.TXT',
          type: ItemType.FILE,
          toolType: ToolType.README,
          description: 'Read Me First',
          content: `
WELCOME TO CASSETTE OS v2.0
===========================

This is a simulated operating environment running on the React engine.
Navigation is directory-based. 

INSTRUCTIONS:
1. Click FOLDERS to navigate.
2. Click FILES to launch tools.
3. Use the BREADCRUMB bar to return to previous directories.
4. DO NOT TURN OFF THE POWER while the tape is saving.

DESIGN PHILOSOPHY:
"High Tech, Low Life." - The Cyberpunk Mantra.
          `,
          icon: <DocIcon />
        }
      ]
    },
    {
      id: 'system',
      name: 'SYSTEM',
      type: ItemType.FOLDER,
      description: 'Core files',
      children: []
    }
  ]
};