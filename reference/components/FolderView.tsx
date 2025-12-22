import React from 'react';
import { FileSystemItem, ItemType } from '../types';
import { FolderIcon, DocIcon } from '../constants';

interface FolderViewProps {
  items: FileSystemItem[];
  onItemClick: (item: FileSystemItem) => void;
}

export const FolderView: React.FC<FolderViewProps> = ({ items, onItemClick }) => {
  if (items.length === 0) {
    return (
      <div className="flex flex-col items-center justify-center h-64 text-amber-700/50">
        <div className="text-4xl mb-4 font-mono">DIR_EMPTY</div>
        <div>[NO DATA FOUND ON TAPE]</div>
      </div>
    );
  }

  return (
    <div className="grid grid-cols-2 md:grid-cols-4 lg:grid-cols-6 gap-6">
      {items.map((item) => (
        <button
          key={item.id}
          onClick={() => onItemClick(item)}
          className="group flex flex-col items-center p-4 border border-amber-900/30 hover:border-amber-500 bg-black/40 hover:bg-amber-500/10 transition-all duration-200 rounded-sm relative overflow-hidden"
        >
          {/* Scanline hover effect specific to item */}
          <div className="absolute inset-0 bg-amber-500/5 translate-y-full group-hover:translate-y-0 transition-transform duration-300 pointer-events-none" />
          
          <div className={`mb-3 ${item.type === ItemType.FOLDER ? 'text-amber-600' : 'text-amber-400'} group-hover:text-amber-200 transition-colors`}>
            {item.icon ? item.icon : (item.type === ItemType.FOLDER ? <FolderIcon /> : <DocIcon />)}
          </div>
          
          <span className="font-mono text-sm tracking-wider uppercase truncate w-full text-center text-amber-500 group-hover:text-amber-100 group-hover:shadow-[0_0_8px_rgba(255,176,0,0.6)] transition-all">
            {item.name}
          </span>
          
          {item.description && (
             <span className="text-[10px] text-amber-800 mt-1 uppercase group-hover:text-amber-500/70">
               {item.description}
             </span>
          )}
        </button>
      ))}
    </div>
  );
};