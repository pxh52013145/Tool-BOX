import React from 'react';
import { Breadcrumb } from '../types';

interface BreadcrumbsProps {
  path: Breadcrumb[];
  onNavigate: (id: string) => void;
}

export const Breadcrumbs: React.FC<BreadcrumbsProps> = ({ path, onNavigate }) => {
  return (
    <div className="flex items-center space-x-2 text-amber-500/80 text-lg uppercase tracking-widest border-b-2 border-amber-500/30 pb-2 mb-4 font-mono">
      <span className="text-amber-700 animate-pulse">root@system:</span>
      {path.map((item, index) => (
        <React.Fragment key={item.id}>
          <span className="text-amber-700 select-none">/</span>
          <button
            onClick={() => onNavigate(item.id)}
            className={`hover:bg-amber-500/20 px-1 rounded transition-colors ${
              index === path.length - 1 ? 'font-bold text-amber-400' : 'text-amber-600 hover:text-amber-500'
            }`}
          >
            {item.name}
          </button>
        </React.Fragment>
      ))}
      <span className="animate-pulse ml-2 w-2 h-4 bg-amber-500 inline-block align-middle"></span>
    </div>
  );
};