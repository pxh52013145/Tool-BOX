import React, { useState, useEffect, useRef } from 'react';
import { generateAIResponse } from '../../services/geminiService';

interface Message {
  role: 'user' | 'model';
  text: string;
}

export const GeminiTerminal: React.FC = () => {
  const [input, setInput] = useState('');
  const [messages, setMessages] = useState<Message[]>([
    { role: 'model', text: 'MOTHER SYSTEM ONLINE. WAITING FOR INPUT...' }
  ]);
  const [loading, setLoading] = useState(false);
  const bottomRef = useRef<HTMLDivElement>(null);

  useEffect(() => {
    bottomRef.current?.scrollIntoView({ behavior: 'smooth' });
  }, [messages]);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    if (!input.trim() || loading) return;

    const userMsg = input;
    setInput('');
    setMessages(prev => [...prev, { role: 'user', text: userMsg }]);
    setLoading(true);

    const response = await generateAIResponse(userMsg);
    
    setMessages(prev => [...prev, { role: 'model', text: response }]);
    setLoading(false);
  };

  return (
    <div className="flex flex-col h-full bg-black/80 border border-amber-900/50 p-2 font-mono text-sm md:text-base">
      <div className="flex-1 overflow-y-auto space-y-4 p-4 scrollbar-hide">
        {messages.map((msg, idx) => (
          <div key={idx} className={`flex ${msg.role === 'user' ? 'justify-end' : 'justify-start'}`}>
            <div className={`max-w-[80%] ${msg.role === 'user' ? 'text-amber-300 text-right' : 'text-amber-500'}`}>
              <span className="opacity-50 text-xs block mb-1">
                {msg.role === 'user' ? '>> OPERATOR' : '>> MOTHER'}
              </span>
              <pre className="whitespace-pre-wrap font-mono font-inherit">{msg.text}</pre>
            </div>
          </div>
        ))}
        {loading && (
          <div className="text-amber-700 animate-pulse">
            >> PROCESSING... [||||||    ]
          </div>
        )}
        <div ref={bottomRef} />
      </div>

      <form onSubmit={handleSubmit} className="border-t border-amber-900/50 p-2 bg-black/90 flex gap-2">
        <span className="text-amber-500 py-2 pl-2">{'>'}</span>
        <input
          type="text"
          value={input}
          onChange={(e) => setInput(e.target.value)}
          className="flex-1 bg-transparent border-none outline-none text-amber-400 font-mono placeholder-amber-900"
          placeholder="ENTER COMMAND..."
          autoFocus
        />
        <button 
          type="submit" 
          disabled={loading}
          className="px-4 py-1 bg-amber-900/20 text-amber-500 border border-amber-700/50 hover:bg-amber-500 hover:text-black uppercase text-xs transition-colors"
        >
          Send
        </button>
      </form>
    </div>
  );
};