# Real-Time Intrusion Monitoring System (ESP32)

An event-driven **IoT edge device** built using **ESP32** that detects physical obstacles and synchronizes system state across **hardware indicators (LED + OLED)** and a **real-time web dashboard** using **WebSockets**.

This project focuses on **systems-level design**, not demos — avoiding polling, minimizing latency, and cleanly separating sensing, logic, and presentation.

---

## 🚀 Features

* **IR Obstacle Detection** (digital, active-LOW)
* **Edge-first processing** on ESP32
* **Physical feedback** via LED
* **Embedded UI** using SSD1306 OLED (I²C)
* **Real-time Web Dashboard** (no refresh, no polling)
* **WebSocket-based event streaming**
* **State-synchronized outputs** (hardware + web stay in sync)

---

## 🧠 System Architecture

<!-- ARCHITECTURE IMAGE PLACEHOLDER -->

> 📸 **Project Image**
>
> Example:
>
> ```md
> ![Project Image](https://drive.google.com/file/d/1R0KsO-QKCXed2YU9DOEDZZZqTg1v8lnO/view?usp=sharing)
> ```

```
IR Sensor
   ↓
ESP32 (edge logic)
   ├── LED (GPIO)
   ├── OLED Display (I²C)
   └── WebSocket Server
            ↓
        Browser Dashboard
```

IR Sensor
↓
ESP32 (edge logic)
├── LED (GPIO)
├── OLED Display (I²C)
└── WebSocket Server
↓
Browser Dashboard

````

- All detection logic runs on the **edge**
- UI updates are **event-driven**, not time-based
- Web clients receive **instant push updates**

---

## 🔌 Hardware Components

| Component | Description |
|--------|------------|
| ESP32 | Main controller |
| IR Obstacle Sensor | Digital output, active-LOW |
| LED + Resistor | Physical alert indicator |
| OLED Display | 0.96" SSD1306 (128×64, I²C) |

### Pin Mapping

| Function | ESP32 GPIO |
|-------|-----------|
| IR Sensor OUT | GPIO 18 |
| LED | GPIO 4 |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |

---

## 🧩 Software Stack

- **ESP32 Arduino Framework**
- **Async TCP (ESP32Async)**
- **ESP Async WebServer**
- **WebSockets (Markus Sattler)**
- **Adafruit SSD1306 + GFX**
- **HTML / CSS / JavaScript** (served from ESP32)

---

## ⚙️ How It Works

1. IR sensor continuously monitors for obstacles
2. ESP32 detects **state transitions** (CLEAR ↔ DETECTED)
3. On detection:
   - LED turns ON
   - OLED displays alert state
   - WebSocket event is broadcast to all clients
4. Browser UI updates instantly (no refresh)
5. Clearing the obstacle reverts all states synchronously

---

## 🌐 Web Dashboard

<!-- WEB DASHBOARD IMAGE PLACEHOLDER -->

> 📸 **Add web dashboard screenshot / GIF here**
>
> Example:
> ```md
> ![Web Dashboard](docs/web-dashboard.png)
> ```

- Hosted directly on ESP32
- Uses **WebSockets** for real-time updates
- Animated visual indicator (alert / clear)
- Zero polling, zero refresh

Access:
````

http://<ESP32-IP>

````

---

## 🛠️ Setup & Installation

### Prerequisites

- ESP32 board support installed in Arduino IDE
- Libraries (install via Library Manager):
  - Async TCP (by ESP32Async)
  - ESP Async WebServer
  - WebSockets
  - Adafruit SSD1306
  - Adafruit GFX Library

Restart Arduino IDE after installing libraries.

### Configuration

Update WiFi credentials in the code:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
````

Upload the sketch and open Serial Monitor to get the device IP.

---

## 🔍 Key Concepts Demonstrated

* Event-driven embedded systems
* Edge computing principles
* GPIO & I²C handling on ESP32
* Real-time communication with WebSockets
* Synchronization between physical and digital states
* Embedded web servers

---

## 🚧 Future Enhancements

* Detection duration tracking
* Intrusion pattern analysis (passing vs lingering)
* Event history + charts
* MQTT / cloud integration
* MCP (Model Context Protocol) bridge for LLM control
* Mobile PWA dashboard

---

## 📌 Why This Project

This project goes beyond basic IoT examples by:

* Avoiding blocking delays and polling
* Using production-style real-time communication
* Treating ESP32 as an **edge system**, not just a microcontroller

The architecture mirrors patterns used in **industrial monitoring**, **security systems**, and **edge analytics**.

---

Built as a learning + systems engineering exercise, focused on clean architecture and real-world design choices.
