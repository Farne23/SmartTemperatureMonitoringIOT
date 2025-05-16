//Poll interval of the dashboard updates
const POLL_INTERVAL_MS = 100;

//Address used for HTTP comunication with the main control unit.
const CONTROL_UNIT_ADDRESS = "https://ca07-109-54-180-105.ngrok-free.app";

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
        .duration(100)
        .attr("d", line);

    const circles = g.selectAll("circle.data-point")
        .data(samples, d => d.date);

    circles.enter()
        .append("circle")
        .attr("class", "data-point")
        .attr("r", 4)
        .attr("fill", "orange")
        .attr("cx", s => x(s.date))
        .attr("cy", s => {
            return s === samples[samples.length - 1] ? y(s.temp) - 200 : y(s.temp);
        })
        .merge(circles)
        .transition()
        .duration(200)
        .ease(d3.easeLinear)
        .attr("cx", s => x(s.date))
        .attr("cy", s => y(s.temp));

    circles.exit().remove();
}

function updateLevelBar(level) {
    const bar = document.getElementById('currentLevel');
    bar.style.height = level + '%';
    document.getElementById("openingLevel").innerText = level;
}

function updateLevelSlider(newLevel) {
    const slider = document.getElementById('setLevelSlider');
    //slider.value = newLevel;
    updateLevelBar(newLevel)
}
const slider = document.getElementById('setLevelSlider');
slider.addEventListener('input', () => {
    const newLevel = slider.value;
    debouncedSendWindowLevel(newLevel);
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
setInterval(() => fetchDashboardData(), POLL_INTERVAL_MS);

//Function handling the updates received from the main unit.
function handleDashboardData(data) {

    const sysStateTxt = document.getElementById("sysState");
    sysStateTxt.innerHTML = "&nbsp;" + data.systemState.replace(/_/g, ' ') + "&nbsp;";

    if (data.controlMode == "AUTOMATIC") {

    }
    switch (data.systemState) {
        case "ALARM":
            document.getElementById("dangerBtn").classList.remove("hidden");
            sysStateTxt.style.color = "white";
            sysStateTxt.style.backgroundColor = "red";
            break;
        case "NORMAL":
            document.getElementById("dangerBtn").classList.add("hidden");
            sysStateTxt.style.color = "green";
            sysStateTxt.style.backgroundColor = "white";
            break;
        case "HOT":
            document.getElementById("dangerBtn").classList.add("hidden");
            sysStateTxt.style.color = "orange";
            sysStateTxt.style.backgroundColor = "white";
            break;
        case "TOO_HOT":
            document.getElementById("dangerBtn").classList.add("hidden");
            sysStateTxt.style.color = "red";
            sysStateTxt.style.backgroundColor = "white";
            break;
        default:
            document.getElementById("dangerBtn").classList.add("hidden");
            sysStateTxt.style.color = "gray";
            sysStateTxt.style.backgroundColor = "white";
            break;
    }

    const oldMode = document.getElementById("controlMode").innerText;
    const modeTxt = document.getElementById("controlMode");
    modeTxt.innerText = data.controlMode;
    if (data.controlMode == "AUTOMATIC") {
        slider.disabled = true;
        slider.value = 0;
        modeTxt.style.color = "green";
    } else {
        modeTxt.style.color = "red";
        slider.disabled = false;
        if (oldMode == "AUTOMATIC") {
            slider.value = data.openingLevel;
        }
    }
    document.getElementById("avgTemp").innerText = data.avgTemperature;
    document.getElementById("minTemp").innerText = data.minTemperature;
    document.getElementById("maxTemp").innerText = data.maxTemperature;
    updateLevelBar(data.openingLevel);

    const existingSampleMap = new Map(samples.map(s => [s.date.getTime(), s]));
    //Update of the samples array avoiding to reset it completely.
    samples = data.temperatures.map(sample => {
        const timeMs = new Date(sample.time).getTime();
        if (existingSampleMap.has(timeMs)) {
            const existing = existingSampleMap.get(timeMs);
            existing.temp = sample.value; 
            return existing;
        } else {
            return {
                date: new Date(sample.time),
                temp: sample.value
            };
        }
    });
    updateChart();
}

document.getElementById("switchModeBtn").addEventListener("click", () => {
    sendControl("switchControlMode");
});

document.getElementById("dangerBtn").addEventListener("click", () => {
    sendControl("stopAlarm");
});

async function sendControl(controlType) {
    try {
        const response = await fetch(CONTROL_UNIT_ADDRESS + "/api/commands", {
            method: 'POST',
            headers: {
                "Content-Type": "application/json",
                "ngrok-skip-browser-warning": "true"
            },
            body: JSON.stringify({
                type: controlType
            })
        });
        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }
    } catch (error) {
        console.error('Failed to fetch', error);
    }
}

async function sendWindowLevel(newLevel) {
    try {
        const response = await fetch(CONTROL_UNIT_ADDRESS + "/api/commands", {
            method: 'POST',
            headers: {
                "Content-Type": "application/json",
                "ngrok-skip-browser-warning": "true"
            },
            body: JSON.stringify({
                type: "updateWindowLevel",
                level: parseInt(newLevel, 10)
            })
        });
        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }
    } catch (error) {
        console.error('Failed to fetch', error);
    }
}

//Debounce function neeeded to avoid sending lots of useless messages when the slider level is changing.
function debounce(func, delay) {
    let timeoutId;
    return (...args) => {
        clearTimeout(timeoutId);
        timeoutId = setTimeout(() => {
            func.apply(null, args);
        }, delay);
    };
}

const debouncedSendWindowLevel = debounce(sendWindowLevel, 100);