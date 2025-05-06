//Poll interval of the dashboard updates
const POLL_INTERVAL_MS = 500;

//Address used for HTTP comunication with the main control unit.
const CONTROL_UNIT_ADDRESS = "https://9671-137-204-20-123.ngrok-free.app";

//Array for temperatures samples
let samples = [];

const svg = d3.select("svg");
const margin = { top: 20, right: 30, bottom: 30, left: 50 };
const width = +svg.attr("width") - margin.left - margin.right;
const height = +svg.attr("height") - margin.top - margin.bottom;

const g = svg.append("g").attr("transform", `translate(${margin.left},${margin.top})`);

const x = d3.scaleTime().range([0, width]);
const y = d3.scaleLinear().domain([18, 30]).range([height, 0]);

const line = d3.line()
    .x(s => x(s.date))
    .y(s => y(s.temp));

// Axes with identifiable classes
g.append("g")
    .attr("class", "axis-x")
    .attr("transform", `translate(0,${height})`)
    .call(d3.axisBottom(x).ticks(5).tickFormat(d3.timeFormat("%H:%M:%S")));

g.append("g")
    .attr("class", "axis-y")
    .call(d3.axisLeft(y));

// Main data line
g.append("path")
    .attr("class", "data-line")
    .attr("fill", "none")
    .attr("stroke", "steelblue")
    .attr("stroke-width", 2);

// Threshold lines and labels
const thresholds = [
    { temp: 23, color: "orange", label: "HOT STATE" },
    { temp: 26, color: "red", label: "TOO HOT STATE" }
];

thresholds.forEach(s => {
    g.append("line")
        .attr("class", "threshold-line")
        .attr("x1", 0)
        .attr("x2", width)
        .attr("y1", y(s.temp))
        .attr("y2", y(s.temp))
        .attr("stroke", s.color)
        .attr("stroke-dasharray", "4 4")
        .attr("stroke-width", 2);

    g.append("text")
        .attr("class", "threshold-label")
        .attr("x", width - 60)
        .attr("y", y(s.temp) - 5)
        .attr("fill", s.color)
        .attr("font-size", "12px")
        .text(s.label);
});

// Update function
function updateChart() {
    samples.sort((a, b) => d3.ascending(a.date, b.date));
    if (samples.length === 0) return;

    const extent = d3.extent(samples, s => s.date);
    const minDate = extent[0];
    const maxDate = extent[1];

    const maxDatePlus = new Date(maxDate.getTime() + 5000);
    x.domain([minDate, maxDatePlus]);

    g.select(".axis-x")
        .transition()
        .call(d3.axisBottom(x).ticks(5).tickFormat(d3.timeFormat("%H:%M:%S")));

    g.select(".data-line")
        .datum(samples)
        .transition()
        .attr("d", line);

    const circles = g.selectAll("circle.data-point")
        .data(samples, d => d.date);

    circles.enter()
        .append("circle")
        .attr("class", "data-point")
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
        const response = await fetch(CONTROL_UNIT_ADDRESS + "/api/data", {
            headers: {
                "ngrok-skip-browser-warning": "true"
            }
        });
        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }
        const data = await response.json();
        console.log(data);
        handleDashboardData(data);
    } catch (error) {
        console.error('Failed to fetch dashboard data:', error);
    }
}
setInterval(() => fetchDashboardData(), 1000);

//Function handling the updates received from the main unit.
function handleDashboardData(data) {
    //LOGGING
    // console.log('Max Temperature:', data.maxTemperature);
    // console.log('Min Temperature:', data.minTemperature);
    // console.log('Avg Temperature:', data.avgTemperature);
    // console.log('Control Mode:', data.controlMode);
    // console.log('System State:', data.systemState);
    // console.log('Opening Level:', data.openingLevel);
    // console.log('Temperatures (last N samples):');
    // data.temperatures.forEach(sample => {
    //     console.log(` - Time: ${sample.time}, Value: ${sample.value}`);
    // });

    document.getElementById("sysState").innerText = data.systemState;
    if (data.systemState == "ALARM") {
        document.getElementById("dangerBtn").classList.remove("hidden");
    } else {
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