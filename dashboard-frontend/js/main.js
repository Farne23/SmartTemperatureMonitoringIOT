//Poll interval of the dashboard updates
const POLL_INTERVAL_MS = 500;

//Address used for HTTP comunication with the main contro lunit.
const CONTROL_UNIT_ADDRESS = "prova-prova";

//Array for temperatures samples
const samples = [];

const svg = d3.select("svg");
const margin = { top: 20, right: 30, bottom: 30, left: 50 };
const width = +svg.attr("width") - margin.left - margin.right;
const height = +svg.attr("height") - margin.top - margin.bottom;

const g = svg.append("g").attr("transform", `translate(${margin.left},${margin.top})`);

// Scale
const x = d3.scaleTime()
    .domain(d3.extent(samples, s => s.date))
    .range([0, width]);

const y = d3.scaleLinear()
    .domain([15, 26])
    .range([height, 0]);

//Lines
const line = d3.line()
    .x(s => x(s.date))
    .y(s => y(s.temp));

//Axis
g.append("g")
    .attr("transform", `translate(0,${height})`)
    .call(d3.axisBottom(x).ticks(5));

g.append("g")
    .call(d3.axisLeft(y));

//Lines for data
g.append("path")
    .datum(samples)
    .attr("fill", "none")
    .attr("stroke", "steelblue")
    .attr("stroke-width", 2)
    .attr("d", line);

//Circles for every sample
g.selectAll("circle")
    .data(samples)
    .enter().append("circle")
    .attr("cx", s => x(s.date))
    .attr("cy", s => y(s.temp))
    .attr("r", 4)
    .attr("fill", "orange");

//Threshold lines
const thresholds = [
    { temp: 20, color: "orange", label: "HOT STATE" },
    { temp: 22, color: "red", label: "TOO HOT STATE" }
];

g.selectAll(".threshold-line")
    .data(thresholds)
    .enter()
    .append("line")
    .attr("class", "threshold-line")
    .attr("x1", 0)
    .attr("x2", width)
    .attr("y1", s => y(s.temp))
    .attr("y2", s => y(s.temp))
    .attr("stroke", s => s.color)
    .attr("stroke-dasharray", "4 4")
    .attr("stroke-width", 2);

// Etichette per le soglie
g.selectAll(".threshold-label")
    .data(thresholds)
    .enter()
    .append("text")
    .attr("x", width - 60)
    .attr("y", s => y(s.temp) - 5)
    .attr("fill", s => s.color)
    .attr("font-size", "12px")
    .text(s => s.label);

//Function updating the chart
function updateChart() {
    x.domain(d3.extent(samples, s => s.date));
    g.select("g") 
        .transition()
        .call(d3.axisBottom(x).ticks(5));
    g.select("path")
        .datum(samples)
        .transition()
        .attr("d", line);

    const circles = g.selectAll("circle")
        .data(samples);

    circles.enter()
        .append("circle")
        .attr("r", 4)
        .attr("fill", "orange")
        .merge(circles)
        .transition()
        .attr("cx", s => x(s.date))
        .attr("cy", s => y(s.temp));

    circles.exit().remove();
}

function updateLevel(level) {
    const bar = document.getElementById('currentLevel');
    bar.style.height = level + '%';
    document.getElementById("openingLevel").innerText = level;
}

function handleSliderChange() {
    const slider = document.getElementById('setLevelSlider');
    const newLevel = slider.value;
    console.log("Nuovo livello selezionato:", newLevel);
    updateLevel(newLevel);
}

function updateLevelSlider(newLevel) {
    const slider = document.getElementById('setLevelSlider');
    slider.value = newLevel;
    updateLevel(newLevel)
}

document.addEventListener('DOMContentLoaded', () => {
    const slider = document.getElementById('setLevelSlider');
    slider.addEventListener('input', handleSliderChange);
});

//Function retrieving data from the main unit.
async function fetchDashboardData() {
    try {
        const response = await fetch(CONTROL_UNIT_ADDRESS);
        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }
        const data = await response.json();
        handleDashboardData(data);
    } catch (error) {
        console.error('Failed to fetch dashboard data:', error);
    }
}

//Function handling the updates received from the main unit.
function handleDashboardData(data) {
    //LOGGING
    console.log('Max Temperature:', data.maxTemperature);
    console.log('Min Temperature:', data.minTemperature);
    console.log('Avg Temperature:', data.avgTemperature);
    console.log('Control Mode:', data.controlMode);
    console.log('System State:', data.systemState);
    console.log('Opening Level:', data.openingLevel);
    console.log('Temperatures (last N samples):');
    data.temperatures.forEach(sample => {
        console.log(` - Time: ${sample.time}, Value: ${sample.value}`);
    });

    document.getElementById("sysState").innerText = data.systemState;
    if(data.systemState == "Alarm"){
        document.getElementById("dangerBtn").classList.remove("hidden");
    }else{
        document.getElementById("dangerBtn").classList.add("hidden");
    }
    document.getElementById("controlMode").innerText = data.controlMode;
    document.getElementById("avgTemp").innerText = data.avgTemperature;
    document.getElementById("minTemp").innerText = data.minTemperature;
    document.getElementById("maxTemp").innerText = data.maxTemperature;
    updateLevelSlider(data.openingLevel);
    samples = data.temperatures.map(sample => ({
        date: new Date(sample.time),
        temp: sample.value
    }));
    updateChart();
}