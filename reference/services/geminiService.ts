import { GoogleGenAI } from "@google/genai";

// Initialize Gemini Client
// In a real app, strict error handling for missing API key should be here.
const ai = new GoogleGenAI({ apiKey: process.env.API_KEY || '' });

export const generateAIResponse = async (prompt: string): Promise<string> => {
  try {
    const response = await ai.models.generateContent({
      model: 'gemini-3-flash-preview',
      contents: prompt,
      config: {
        systemInstruction: "You are a retro-futuristic AI assistant named 'MOTHER'. You exist inside a cassette futurism OS. Your responses should be technical, slightly robotic but helpful. Keep responses concise and use monospace formatting where possible. Avoid markdown bolding if possible, prefer UPPERCASE for emphasis.",
      }
    });
    
    return response.text || "ERR: NO DATA RECEIVED.";
  } catch (error) {
    console.error("Gemini API Error:", error);
    return `CRITICAL ERROR: CONNECTION FAILED.\nDETAILS: ${error instanceof Error ? error.message : "UNKNOWN"}`;
  }
};