# AI-Based Autonomous Cyber-Physical Security System

An **AI-powered, IoT-based smart security platform** that combines **computer vision, environmental sensors, cloud messaging, and autonomous decision making** to detect intrusions, evaluate threat levels, trigger alarms, and store historical data for analysis.

This project integrates **YOLO-based human detection**, **ESP32 sensor fusion**, **Node-RED automation**, and **InfluxDB time-series storage** into a complete **real-world security system**.

---

## 🚀 System Overview

The system continuously monitors an area using a camera and physical sensors.  
All inputs are fused into a **risk score**, which determines whether an **alarm is triggered**.

### Main Components

| Module | Function |
|--------|----------|
| USB Camera | Captures live video |
| Vision AI (YOLO) | Detects humans and motion |
| ESP32 Sensor Node | Reads IR, MQ-2 gas, Ultrasonic, and LDR |
| MQTT Cloud | Transfers data between devices |
| Node-RED Brain | Computes risk and controls alarms |
| ESP32 Alarm | Activates LED/Buzzer |
| ESP32 OLED | Displays live system status |
| InfluxDB | Stores all AI and sensor data |
| Grafana | Displays trends and system behavior |

---

## 🧠 System Architecture

Camera → Vision AI → MQTT → Node-RED Brain
Sensors → MQTT → Node-RED Brain
Node-RED → Alarm ESP32
Node-RED → OLED ESP32
Node-RED → InfluxDB → Grafana

yaml
Copy code

---

## ✨ Features

- AI-based human detection  
- Multi-sensor fusion (IR, Gas, Light, Distance)  
- Autonomous risk scoring  
- Automatic alarm triggering  
- Live OLED dashboard  
- Cloud MQTT communication  
- Time-series data logging  
- Control-room style visualization  

---

## 🛠 Installation Guide

### 1️⃣ Arduino IDE

Download:
https://www.arduino.cc/en/software  

Add ESP32 board:
File → Preferences → Additional Boards URL
https://dl.espressif.com/dl/package_esp32_index.json

makefile
Copy code

Then:
Tools → Board Manager → Install ESP32

yaml
Copy code

---

### 2️⃣ Node-RED

Install NodeJS:
https://nodejs.org  

Then:
npm install -g node-red
node-red

makefile
Copy code

Open:
http://localhost:1880

yaml
Copy code

Install dashboard:
node-red-dashboard

yaml
Copy code

---

### 3️⃣ InfluxDB

Download:
https://portal.influxdata.com/downloads  

Extract and run:
influxd

makefile
Copy code

Open:
http://localhost:8086

yaml
Copy code

Create:
- Organization
- Bucket: `maheshphand`
- Copy API Token

---

### 4️⃣ Grafana

Download:
https://grafana.com/grafana/download  

Open:
http://localhost:3000

yaml
Copy code

Add Data Source:
- Type: InfluxDB
- Query Language: Flux
- URL: http://localhost:8086
- Bucket: maheshphand
- Token: InfluxDB API Token

---

### 5️⃣ Vision AI Setup

Install Python packages:
pip install opencv-python ultralytics numpy paho-mqtt

makefile
Copy code

Run:
python vision_ai.py

yaml
Copy code

---

### 6️⃣ ESP32 Setup

Flash:
- `esp32_sensor_mqtt.ino` to Sensor ESP32  
- `esp32_oled_dashboard.ino` to OLED ESP32  

MQTT broker:
dev.coppercloud.in

yaml
Copy code

---

### 7️⃣ Node-RED Setup

Import:
- Autonomous Brain flow
- Dashboard UI flow

Deploy and open:
http://localhost:1880/ui

yaml
Copy code

---

## 🔍 How It Works

1. Camera detects humans using YOLO  
2. ESP32 sends sensor readings  
3. Node-RED computes a **risk score**  
4. If risk exceeds threshold → alarm ON  
5. OLED displays live state  
6. Data is stored in InfluxDB  
7. Grafana shows trends and history  

---

## 🏆 Why This Project Is Special

This is not a basic IoT demo.  
It is a **distributed cyber-physical AI system** with:

- Vision AI  
- Sensor fusion  
- Cloud messaging  
- Autonomous decision making  
- Data persistence  
- Real-time dashboards  

This architecture is used in **smart cities, factories, and professional security systems**.

---

## 👨‍💻 Author

**Mahesh Phand**  
