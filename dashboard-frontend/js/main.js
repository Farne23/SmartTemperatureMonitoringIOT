// Dati di esempio: temperature su 10 giorni
const data = [
    { day: new Date(2025, 3, 8), temp: 15 },
    { day: new Date(2025, 3, 9), temp: 17 },
    { day: new Date(2025, 3, 10), temp: 14 },
    { day: new Date(2025, 3, 11), temp: 18 },
    { day: new Date(2025, 3, 12), temp: 20 },
    { day: new Date(2025, 3, 13), temp: 19 },
    { day: new Date(2025, 3, 14), temp: 22 },
    { day: new Date(2025, 3, 15), temp: 21 },
    { day: new Date(2025, 3, 16), temp: 23 },
    { day: new Date(2025, 3, 17), temp: 24 }
];

const svg = d3.select("svg");
const margin = { top: 20, right: 30, bottom: 30, left: 50 };
const width = +svg.attr("width") - margin.left - margin.right;
const height = +svg.attr("height") - margin.top - margin.bottom;

const g = svg.append("g").attr("transform", `translate(${margin.left},${margin.top})`);

// Scale
const x = d3.scaleTime()
    .domain(d3.extent(data, d => d.day))
    .range([0, width]);

const y = d3.scaleLinear()
    .domain([12, d3.max(data, d => d.temp)])
    .range([height, 0]);

// Linea
const line = d3.line()
    .x(d => x(d.day))
    .y(d => y(d.temp));

// Assi
g.append("g")
    .attr("transform", `translate(0,${height})`)
    .call(d3.axisBottom(x).ticks(5));

g.append("g")
    .call(d3.axisLeft(y));

// Linea dati
g.append("path")
    .datum(data)
    .attr("fill", "none")
    .attr("stroke", "steelblue")
    .attr("stroke-width", 2)
    .attr("d", line);

// Pallini per ogni dato
g.selectAll("circle")
    .data(data)
    .enter().append("circle")
    .attr("cx", d => x(d.day))
    .attr("cy", d => y(d.temp))
    .attr("r", 4)
    .attr("fill", "orange");

// Linee di soglia
const thresholds = [
    { temp: 18, color: "green", label: "Soglia Bassa" },
    { temp: 20, color: "orange", label: "Soglia Attenzione" },
    { temp: 22, color: "red", label: "Soglia Alta" }
];

g.selectAll(".threshold-line")
    .data(thresholds)
    .enter()
    .append("line")
    .attr("class", "threshold-line")
    .attr("x1", 0)
    .attr("x2", width)
    .attr("y1", d => y(d.temp))
    .attr("y2", d => y(d.temp))
    .attr("stroke", d => d.color)
    .attr("stroke-dasharray", "4 4")
    .attr("stroke-width", 2);

// Etichette per le soglie
g.selectAll(".threshold-label")
    .data(thresholds)
    .enter()
    .append("text")
    .attr("x", width - 60)
    .attr("y", d => y(d.temp) - 5)
    .attr("fill", d => d.color)
    .attr("font-size", "12px")
    .text(d => d.label);

function updateLevel(level) {
    const bar = document.getElementById('currentLevel');
    bar.style.height = level + '%';
}

function handleSliderChange() {
    const slider = document.getElementById('setLevelSlider');
    const newLevel = slider.value;
    console.log("Nuovo livello selezionato:", newLevel);
    updateLevel(newLevel);
}

// collego l'evento subito
document.addEventListener('DOMContentLoaded', () => {
    const slider = document.getElementById('setLevelSlider');
    slider.addEventListener('input', handleSliderChange);
});