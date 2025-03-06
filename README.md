# Smart Temperature Monitoring

## Embedded Systems and IoT - a.y. 2024-2025

**Assignment #03 - Smart Temperature Monitoring**  
**Version:** v1.0.0-20241211

## Overview
This project aims to develop an IoT system to monitor the temperature of a closed environment and control the opening of a window based on the temperature readings. The system consists of four interconnected subsystems:

1. **Temperature Monitoring Subsystem (ESP-based)**
2. **Control Unit Subsystem (Backend - PC-based)**
3. **Window Controller Subsystem (Arduino-based)**
4. **Dashboard Subsystem (Frontend/Web App - PC-based)**

## Project Structure
The project folder contains the following subdirectories:

```
assignment-03/
│-- temperature-monitoring-subsystem/
│-- control-unit-backend/
│-- window-controller/
│-- dashboard-frontend/
│-- doc/
    │-- report.pdf
    │-- schema.png
    │-- video_demo_link.txt
```

### Subsystems
#### 1. Temperature Monitoring Subsystem
- Runs on ESP32/ESP8266 (or equivalent SoC board)
- Monitors the temperature using a temperature sensor
- Sends data to the Control Unit via MQTT
- Uses LEDs to indicate network status:
  - **Green LED ON, Red LED OFF:** System operational
  - **Green LED OFF, Red LED ON:** Network issue detected
- The monitoring frequency is set by the Control Unit

#### 2. Control Unit Subsystem (Backend)
- Runs on a PC
- Main coordinator of the system
- Communicates with:
  - Temperature Monitoring Subsystem via **MQTT**
  - Window Controller Subsystem via **Serial Communication**
  - Dashboard via **HTTP API**
- Maintains historical temperature data (last N measurements, min/max/average per day)
- Determines window opening levels based on temperature:
  - **NORMAL state (T < T1):** Frequency = F1, Window Closed
  - **HOT state (T1 ≤ T ≤ T2):** Frequency = F2 > F1, Window Opens proportionally
  - **TOO HOT state (T > T2):** Window fully open
  - **ALARM state:** Triggered if TOO HOT state persists for DT time

#### 3. Window Controller Subsystem
- Runs on Arduino (or equivalent MCU board)
- Controls window opening via a **servo motor** (0° = closed, 90° = fully open)
- Interacts with an operator through:
  - **Button:** Toggles between AUTOMATIC and MANUAL mode
  - **Potentiometer:** Manually adjusts window opening in MANUAL mode
  - **LCD Display:** Shows current mode, temperature, and window position
- Communicates with Control Unit via **Serial Communication**

#### 4. Dashboard Subsystem (Frontend)
- Runs on a PC
- Provides a web-based interface for monitoring and controlling the system
- Communicates with the Control Unit via **HTTP API**
- Displays:
  - Temperature graph (last N measurements)
  - Current temperature stats (average, min, max)
  - System state (NORMAL, HOT, TOO HOT, ALARM)
  - Window opening level (percentage)
- Allows manual window control and ALARM acknowledgment

## Hardware Components
### Temperature Monitoring Subsystem
- ESP32/ESP8266 (or equivalent)
- 1 Temperature sensor
- 1 Green LED
- 1 Red LED

### Window Controller Subsystem
- Arduino Uno (or equivalent)
- 1 Servo motor
- 1 Potentiometer
- 1 Tactile button
- 1 LCD display

## Development Requirements
### Temperature Monitoring Subsystem
- Implement finite state machines (FSMs) for control logic
- Use MQTT for communication with Control Unit

### Window Controller Subsystem
- Implement finite state machines (FSMs) for control logic
- Use Serial Communication to interact with Control Unit

### Control Unit Subsystem
- Runs on a PC
- Can be implemented using **[..DA DEFINIRE..]** programming language
- Uses MQTT (Temperature Monitoring), HTTP (Dashboard), Serial (Window Controller)

### Dashboard Subsystem
- Runs on a PC
- Can be implemented using **[..DA DEFINIRE..]** web technologies
- Uses HTTP API for communication with Control Unit

## Installation & Usage
1. Deploy each subsystem in its corresponding environment.
2. Ensure MQTT broker is running.
3. Start the Control Unit backend.
4. Launch the Dashboard web app.
5. Verify communication between subsystems.

## Authors
- **Michele Farneti** (Matricola: [..DA DEFINIRE..], Email: michele.farneti@studio.unibo.it)
- **Samuele Casadei** (Matricola: [..DA DEFINIRE..], Email: [..DA DEFINIRE..])

## License
This project is licensed under the **MIT License**.

