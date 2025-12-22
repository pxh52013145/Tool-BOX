import React, { useState, useEffect, useCallback } from 'react';
import { Breadcrumb, FileSystemItem, ItemType, ToolType } from './types';
import { ROOT_FILE_SYSTEM } from './constants';
import { Breadcrumbs } from './components/Breadcrumbs';
import { FolderView } from './components/FolderView';
import { GeminiTerminal } from './components/tools/GeminiTerminal';
import { SystemMonitor } from './components/tools/SystemMonitor';

const App: React.FC = () => {
  const [currentPath, setCurrentPath] = useState<Breadcrumb[]>([{ id: 'root', name: 'ROOT' }]);
  const [currentFolder, setCurrentFolder] = useState<FileSystemItem>(ROOT_FILE_SYSTEM);
  const [activeTool, setActiveTool] = useState<FileSystemItem | null>(null);

  // Audio for clicks (Simulated)
  const playClick = () => {
    // In a real app, we would play a short blip sound here.
  };

  const navigateTo = useCallback((folderId: string) => {
    playClick();
    
    // Find index of the folder in the current path to slice the path
    const pathIndex = currentPath.findIndex(p => p.id === folderId);
    if (pathIndex === -1) return; // Should not happen if logic is correct

    const newPath = currentPath.slice(0, pathIndex + 1);
    setCurrentPath(newPath);

    // Reconstruct current folder based on new path
    let folder = ROOT_FILE_SYSTEM;
    for (let i = 1; i < newPath.length; i++) {
      const child = folder.children?.find(c => c.id === newPath[i].id);
      if (child) folder = child;
    }
    setCurrentFolder(folder);
    setActiveTool(null); // Close any active tool when navigating
  }, [currentPath]);

  const handleItemClick = (item: FileSystemItem) => {
    playClick();

    if (item.type === ItemType.FOLDER) {
      setCurrentPath(prev => [...prev, { id: item.id, name: item.name }]);
      setCurrentFolder(item);
    } else {
      // Launch Tool
      setActiveTool(item);
    }
  };

  const closeTool = () => {
    playClick();
    setActiveTool(null);
  };

  const renderTool = () => {
    if (!activeTool) return null;

    switch (activeTool.toolType) {
      case ToolType.GEMINI_TERMINAL:
        return <GeminiTerminal />;
      case ToolType.SYSTEM_MONITOR:
        return <SystemMonitor />;
      case ToolType.README:
        return (
           <div className="bg-black/90 p-8 border border-amber-900/50 h-full overflow-y-auto">
             <pre className="font-mono text-amber-500 whitespace-pre-wrap">{activeTool.content}</pre>
           </div>
        );
      default:
        return <div className="text-amber-500 font-mono p-10">UNKNOWN_EXECUTABLE_FORMAT</div>;
    }
  };

  // Clock
  const [time, setTime] = useState(new Date());
  useEffect(() => {
    const timer = setInterval(() => setTime(new Date()), 1000);
    return () => clearInterval(timer);
  }, []);

  return (
    <div className="min-h-screen bg-neutral-900 text-amber-500 font-mono relative selection:bg-amber-500 selection:text-black">
      {/* CRT Overlay Effects */}
      <div className="fixed inset-0 z-50 pointer-events-none scanlines opacity-30"></div>
      <div className="fixed inset-0 z-50 pointer-events-none radial-gradient opacity-50 shadow-[inset_0_0_100px_rgba(0,0,0,0.9)]"></div>
      
      {/* Main Layout Container */}
      <div className="relative z-10 flex flex-col h-screen p-2 md:p-6 max-w-7xl mx-auto crt-flicker">
        
        {/* Header / Menu Bar */}
        <header className="flex justify-between items-end border-b-4 border-amber-600 pb-2 mb-6 bg-black/40 p-4 rounded-t-lg">
          <div>
            <h1 className="text-3xl md:text-5xl font-bold tracking-tighter text-amber-500 drop-shadow-[0_0_10px_rgba(245,158,11,0.8)]">
              CASSETTE<span className="text-amber-700">OS</span>
            </h1>
            <div className="text-xs md:text-sm text-amber-800 tracking-[0.2em] mt-1">
              SYSTEM_READY // V.2.0.4
            </div>
          </div>
          <div className="text-right">
             <div className="text-2xl md:text-3xl font-bold text-amber-500">
               {time.toLocaleTimeString([], { hour12: false })}
             </div>
             <div className="text-xs text-amber-700 uppercase">
               {time.toLocaleDateString()}
             </div>
          </div>
        </header>

        {/* Main Workspace */}
        <main className="flex-1 flex flex-col relative bg-[#0a0a0a] border-2 border-amber-800/60 rounded-lg shadow-[0_0_20px_rgba(180,83,9,0.1)] overflow-hidden">
          
          {/* Top Status Bar of Window */}
          <div className="bg-amber-900/20 p-2 flex justify-between items-center border-b border-amber-800/40">
             <div className="text-xs text-amber-600 uppercase tracking-widest">
               User: ADMIN // Mem: 64K OK
             </div>
             <div className="flex gap-2">
                <div className="w-3 h-3 bg-amber-900 rounded-full border border-amber-700"></div>
                <div className="w-3 h-3 bg-amber-900 rounded-full border border-amber-700"></div>
             </div>
          </div>

          <div className="flex-1 p-4 md:p-6 overflow-hidden flex flex-col">
            
            {activeTool ? (
              // Active Tool View
              <div className="flex flex-col h-full animate-in fade-in duration-300">
                <div className="flex items-center justify-between mb-4 border-b border-amber-500/30 pb-2">
                   <h2 className="text-xl text-amber-400 font-bold uppercase flex items-center gap-2">
                     <span className="w-2 h-2 bg-amber-500 animate-pulse inline-block"></span>
                     EXECUTING: {activeTool.name}
                   </h2>
                   <button 
                     onClick={closeTool}
                     className="px-4 py-1 border border-amber-500 text-amber-500 hover:bg-amber-500 hover:text-black uppercase text-sm font-bold transition-all"
                   >
                     [X] TERMINATE
                   </button>
                </div>
                <div className="flex-1 overflow-hidden relative">
                   {renderTool()}
                </div>
              </div>
            ) : (
              // File Explorer View
              <div className="flex flex-col h-full animate-in zoom-in-95 duration-200">
                <Breadcrumbs path={currentPath} onNavigate={navigateTo} />
                <div className="flex-1 overflow-y-auto pr-2 custom-scrollbar">
                  <FolderView items={currentFolder.children || []} onItemClick={handleItemClick} />
                </div>
                
                {/* Footer Info in Folder View */}
                <div className="mt-4 pt-4 border-t border-amber-900/30 text-xs text-amber-800 flex justify-between">
                   <span>OBJECTS: {currentFolder.children?.length || 0}</span>
                   <span>FREE SPACE: 0KB</span>
                </div>
              </div>
            )}

          </div>
        </main>

        {/* System Footer */}
        <footer className="mt-4 flex justify-between text-xs text-amber-900/60 uppercase font-mono">
           <div>REACT_ENGINE: MOUNTED</div>
           <div className="animate-pulse">Waiting for command...</div>
        </footer>

      </div>
    </div>
  );
};

export default App;