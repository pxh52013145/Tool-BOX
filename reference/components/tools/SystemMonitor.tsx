import React, { useEffect, useRef, useState } from 'react';
import * as d3 from 'd3';

export const SystemMonitor: React.FC = () => {
  const svgRef = useRef<SVGSVGElement>(null);
  const [data, setData] = useState<number[]>(Array(20).fill(0).map(() => Math.random() * 100));

  useEffect(() => {
    const interval = setInterval(() => {
      setData(prev => {
        const next = [...prev.slice(1), Math.random() * 100];
        return next;
      });
    }, 500);
    return () => clearInterval(interval);
  }, []);

  useEffect(() => {
    if (!svgRef.current) return;

    const svg = d3.select(svgRef.current);
    const width = svgRef.current.clientWidth;
    const height = svgRef.current.clientHeight;
    
    // Clear previous
    svg.selectAll("*").remove();

    // Scales
    const xScale = d3.scaleLinear().domain([0, data.length - 1]).range([0, width]);
    const yScale = d3.scaleLinear().domain([0, 100]).range([height, 0]);

    // Line generator
    const line = d3.line<number>()
      .x((d, i) => xScale(i))
      .y(d => yScale(d))
      .curve(d3.curveStepAfter); // Retro step look

    // Draw Grid
    const gridGroup = svg.append("g").attr("class", "grid");
    
    // Horizontal lines
    for(let i=0; i<=10; i++) {
        gridGroup.append("line")
            .attr("x1", 0)
            .attr("y1", yScale(i*10))
            .attr("x2", width)
            .attr("y2", yScale(i*10))
            .attr("stroke", "#451a03")
            .attr("stroke-width", 1);
    }
    // Vertical lines
    for(let i=0; i<data.length; i++) {
        gridGroup.append("line")
            .attr("x1", xScale(i))
            .attr("y1", 0)
            .attr("x2", xScale(i))
            .attr("y2", height)
            .attr("stroke", "#451a03")
            .attr("stroke-width", 1);
    }

    // Draw Line
    svg.append("path")
      .datum(data)
      .attr("fill", "none")
      .attr("stroke", "#d97706") // Amber-600
      .attr("stroke-width", 2)
      .attr("d", line)
      .attr("filter", "drop-shadow(0 0 4px #d97706)");

    // Draw Area under line
    const area = d3.area<number>()
        .x((d, i) => xScale(i))
        .y0(height)
        .y1(d => yScale(d))
        .curve(d3.curveStepAfter);

    svg.append("path")
        .datum(data)
        .attr("fill", "#d97706")
        .attr("opacity", 0.1)
        .attr("d", area);

    // Add some random "glitch" rects
    svg.append("rect")
        .attr("x", width - 50)
        .attr("y", 10)
        .attr("width", 40)
        .attr("height", 20)
        .attr("fill", "#d97706")
        .attr("opacity", Math.random() > 0.8 ? 0.8 : 0.1);

  }, [data]);

  return (
    <div className="flex flex-col h-full bg-black/90 border border-amber-900/50 p-4">
      <div className="flex justify-between items-center mb-4 border-b border-amber-900/50 pb-2">
        <h2 className="text-amber-500 font-mono text-xl tracking-widest">CPU_CORE_TEMP_VISUALIZER</h2>
        <span className="text-amber-700 font-mono text-xs animate-pulse">LIVE_FEED_ACTIVE</span>
      </div>
      <div className="flex-1 w-full h-full relative overflow-hidden bg-black">
         <svg ref={svgRef} className="w-full h-full block" />
      </div>
      <div className="mt-4 grid grid-cols-3 gap-2 text-xs font-mono text-amber-800">
          <div className="border border-amber-900/30 p-1">MEM: {Math.floor(Math.random() * 1024)}MB</div>
          <div className="border border-amber-900/30 p-1">SWAP: {Math.floor(Math.random() * 512)}MB</div>
          <div className="border border-amber-900/30 p-1">TAPE: RW</div>
      </div>
    </div>
  );
};