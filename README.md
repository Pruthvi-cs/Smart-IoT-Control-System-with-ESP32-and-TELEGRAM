
<div align="center">

# Smart IoT Control System
### Telegram Bot + Web Dashboard + ESP32

[![ESP32](https://img.shields.io/badge/MCU-ESP32-0066FF?style=flat-square&logo=espressif)]()
[![Telegram](https://img.shields.io/badge/Interface-Telegram_Bot-26A5E4?style=flat-square&logo=telegram)]()
[![Web](https://img.shields.io/badge/Interface-Web_Dashboard-4CAF50?style=flat-square&logo=html5)]()
[![License](https://img.shields.io/badge/License-MIT-yellow?style=flat-square)]()
[![Arduino](https://img.shields.io/badge/Platform-Arduino_IDE-00979D?style=flat-square&logo=arduino)]()

**Dual-interface IoT control system — command your ESP32 via Telegram or a browser-based dashboard.**

[Features](#-features) · [Architecture](#-architecture) · [Setup](#-setup--configuration) · [Usage](#-usage) · [License](#-license)

</div>

---

## Overview

This project implements a **dual-channel IoT control system** on the ESP32 platform. A single onboard LED (GPIO 2) can be monitored and toggled through two independent interfaces:

| Channel | Description |
|---------|-------------|
| **Telegram Bot** | Send commands directly in a Telegram chat to control the LED remotely |
| **Web Dashboard** | Open the ESP32's IP in any browser for a visual, button-based control panel |

Both interfaces share the same underlying GPIO state, ensuring a **consistent real-time experience** regardless of the control method used.

> **Ideal for:** College mini-project submissions, GitHub portfolio pieces, and live IoT demonstrations.

---

## ✨ Features

### Telegram Bot Commands
| Command | Action |
|---------|--------|
| `/start` | Greeting message with quick-start guide |
| `/help` | Full list of available commands |
| `/on` | Turn the internal LED ON |
| `/off` | Turn the internal LED OFF |
| `/blink` | Execute a 5-flash blink sequence |
| `/status` | Query and return the current LED state |
| `/web` | Receive the web dashboard URL |

### Web Dashboard
- **Live LED status** display (ON / OFF) with dynamic color coding
- **TURN ON** / **TURN OFF** action buttons
- **BLINK LED** one-click trigger
- **REFRESH** to poll the latest state
- Fully responsive — works on mobile, tablet, and desktop
- Styled with **Tailwind CSS** and branded SVG icons

### System-Level Features
- **Chat ID authorization** — only the predefined user can issue commands
- **HTTPS Telegram communication** via `WiFiClientSecure`
- **Zero external servers** — everything runs self-contained on the ESP32
- **Non-blocking polling** — web server and Telegram bot run concurrently in the main loop
- **Serial monitoring** — all actions are logged to the Serial Monitor for debugging

---

## 🏗 Architecture

```
┌─────────────┐         ┌─────────────┐
│  Telegram   │         │   Browser   │
│  App / Bot  │         │  Dashboard  │
└──────┬──────┘         └──────┬──────┘
       │                       │
       │  HTTPS               │  HTTP
       │  (Bot API)           │  (Port 80)
       │                       │
┌──────┴───────────────────────┴──────┐
│            ESP32 (Wi-Fi)            │
│                                     │
│  ┌─────────────┐ ┌───────────────┐  │
│  │  Telegram    │ │  Web Server   │  │
│  │  Handler     │ │  (Routes)     │  │
│  └──────┬──────┘ └───────┬───────┘  │
│         │                │          │
│         └────────┬───────┘          │
│                  ▼                  │
│         ┌──────────────┐           │
│         │  GPIO 2      │           │
│         │  (LED)       │           │
│         └──────────────┘           │
└─────────────────────────────────────┘
```

### Request Flow
```
User Input (Telegram / Browser)
        │
        ▼
  Command Parsing
        │
        ▼
  Authorization Check ──► Unauthorized ──► Reject
        │
      Authorized
        │
        ▼
  GPIO Manipulation (digitalWrite / digitalRead)
        │
        ▼
  Status Response (Telegram reply / Web page update)
```

---

## 🔧 Hardware Required

| Component | Specification | Notes |
|-----------|--------------|-------|
| ESP32 DevKit | Any variant (ESP32-WROOM-32) | Internal LED on GPIO 2 |
| USB Cable | Type-A to Micro-B or Type-C | For power and programming |
| Wi-Fi Router | 2.4 GHz network | ESP32 does not support 5 GHz |

> **Optional:** External LED + 220Ω resistor on GPIO 2 if your board lacks a built-in LED.

---

## 💻 Software Requirements

| Tool | Version | Purpose |
|------|---------|---------|
| Arduino IDE | ≥ 2.0 | Code editor and uploader |
| ESP32 Board Package | Latest | Compiler toolchain for ESP32 |
| UniversalTelegramBot | Latest | Telegram Bot API wrapper |
| ArduinoJson | v6 | JSON parsing for Telegram responses |

### Install ESP32 Board Package
1. Open Arduino IDE → **File → Preferences**
2. Add this URL to **Additional Boards Manager URLs:**
   ```
   https://arduino.esp32.com/package_esp32_index.json
   ```
3. Go to **Tools → Board → Boards Manager** → search `esp32` → install

### Install Libraries
Go to **Sketch → Include Library → Manage Libraries**, then search and install:
- `UniversalTelegramBot`
- `ArduinoJson`

---

## ⚙ Setup & Configuration

### 1. Create a Telegram Bot
1. Open Telegram and search for **@BotFather**
2. Send `/newbot` and follow the prompts
3. Copy the **Bot Token** (format: `123456789:ABCdefGHIjklMNOpqrsTUVwxyz`)

### 2. Get Your Chat ID
1. Send any message to your newly created bot
2. Visit `https://api.telegram.org/bot<TOKEN>/getUpdates` in a browser
3. Find `"chat":{"id": 123456789}` — that number is your Chat ID
4. Alternatively, message **@userinfobot** for your numeric ID

### 3. Configure the Sketch
Open `final_smart_iot_control.ino` and update these four values:

```cpp
const char* ssid     = "YOUR_WIFI_NAME";       // Your Wi-Fi SSID
const char* password = "YOUR_WIFI_PASSWORD";   // Your Wi-Fi password
#define BOT_TOKEN    "YOUR_BOT_TOKEN"          // From BotFather
#define CHAT_ID      "YOUR_CHAT_ID"            // Your numeric Chat ID
```

### 4. Upload & Verify
1. Select your board: **Tools → Board → ESP32 Dev Module**
2. Select the correct COM port
3. Click **Upload**
4. Open **Serial Monitor** at `115200` baud
5. Wait for the output:
   ```
   Wi-Fi Connected!
   ESP32 IP Address: 192.168.x.x
   Web Server Started
   ```

---

## 📖 Usage

### Via Telegram
```
You → /start
Bot → Welcome <YourName>
      Available Commands:
      /on → Turn LED ON
      /off → Turn LED OFF
      /blink → Blink LED
      /status → Check LED Status
      /web → Open Web Dashboard
      /help → Show Commands

You → /on
Bot → LED turned ON

You → /status
Bot → LED is currently ON

You → /web
Bot → Open Web Dashboard:
      http://192.168.1.105
```

### Via Web Dashboard
1. Open the IP address from Serial Monitor in any browser
2. The dashboard renders with the current LED state and a professional UI
3. Click any button to execute the corresponding action
4. The page can auto-poll the status every 5 seconds without a full reload

### Security Behavior
Any user **not** matching `CHAT_ID` who messages the bot receives:
```
Unauthorized User
```
No GPIO action is performed.

---
<div align="center">
       <h1>Images</h1>
  <!-- Replace 'hardware_setup.jpg' with your actual file name -->
       <p><i>ESP32 DevKit connected and ready for dual-channel control</i></p>
  <img src="images/desktop view.png" width="500" alt="Runninng on desktop"><br><img src="images/mobile view.png" width="500" alt="Runninng on mobile"><br><img src="images/teleligram charts.png" width="500" alt="teleligram charts"><br>
 
</div>
---
### 📁 Repository Structure

```
SmartIoT-Control/
│
├── final_smart_iot_control.ino   # Complete Arduino sketch (Tailwind CSS UI)
├── README.md                      # Project documentation
├── LICENSE                        # MIT License
└── .gitignore                     # Arduino build artifacts
```

---

## 🧩 Code Structure

The sketch is organized into clearly separated sections:

| Section | Responsibility |
|---------|----------------|
| Credentials & Config | Wi-Fi, bot token, chat ID |
| Object Initialization | `WiFiClientSecure`, `UniversalTelegramBot`, `WebServer` |
| `blinkLED()` | 5-iteration LED blink pattern |
| `getHTML()` | Generates responsive Tailwind CSS dashboard using raw string literals |
| Web Route Handlers | Routes: `/`, `/on`, `/off`, `/blink` |
| `handleNewMessages()` | Parses Telegram messages, auth check, command dispatch |
| `setup()` | Serial init, Wi-Fi connect, server start |
| `loop()` | Non-blocking web + Telegram polling (2 s interval) |

---

## 🚀 Future Enhancements

- [ ] **PWM dimming** — add `/dim <value>` for brightness control
- [ ] **Multiple LEDs** — extend to GPIO relays for home appliances
- [ ] **Sensor integration** — report temperature/humidity via `/sensor`
- [ ] **OTA updates** — push firmware over Wi-Fi without USB
- [ ] **Scheduled actions** — time-based LED on/off using NTP
- [ ] **MQTT bridge** — integrate with Home Assistant or Node-RED

---

## ⚠ Troubleshooting

| Problem | Solution |
|---------|----------|
| Wi-Fi won't connect | Verify SSID/password; ensure 2.4 GHz network |
| Bot doesn't respond | Check `BOT_TOKEN` and `CHAT_ID`; confirm bot is not in a group |
| Web page won't load | Ensure device and phone/PC are on the **same LAN** |
| LED always off | Some ESP32 boards use a different GPIO; try pin 4 or check your board schematic |
| Blank Serial Monitor | Set baud rate to `115200`; check correct COM port |
| `getUpdates` timeout | Ensure `client.setInsecure()` is called; some networks block Telegram API |
| Web UI looks unstyled | Ensure the ESP32 is connected to the internet to load the Tailwind CSS CDN |

---

## 🤝 Contributing

Contributions are welcome. To contribute:

1. **Fork** this repository
2. Create a **feature branch** (`git checkout -b feature/improvement`)
3. **Commit** your changes (`git commit -m "Add description"`)
4. **Push** to the branch (`git push origin feature/improvement`)
5. Open a **Pull Request**

---

## 📄 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

---

<div align="center">

**Built with ❤️ using ESP32, Arduino, and Telegram**

</div>

