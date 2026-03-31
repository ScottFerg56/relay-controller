# Relay Controller

An ESP32-based relay controller with soft PWM, a web interface, rotary encoder controls, and an OLED display. Built for the **Seeed XIAO ESP32-C6**.

## Features

- **Soft PWM relay control** — configurable duty cycle (0–100%) and cycle duration
- **Three relay states** — PWM mode, always ON, and disabled
- **Web interface** — live parameter control and relay status from any browser on the network
- **Rotary encoders** — physical control via two Adafruit I2C Stemma QT encoder breakouts
- **OLED display** — 128×64 two-tone display showing relay state and parameters
- **NeoPixel indicators** — encoder ring LEDs reflect relay state at a glance

---

## Hardware

| Component | Details |
|---|---|
| Microcontroller | Seeed XIAO ESP32-C6 |
| Relay | Connected to pin `D1` |
| Left encoder | Adafruit I2C QT Rotary Encoder w/ NeoPixel ([#4991](https://www.adafruit.com/product/4991)) — I2C address `0x36` |
| Right encoder | Adafruit I2C QT Rotary Encoder w/ NeoPixel ([#4991](https://www.adafruit.com/product/4991)) — I2C address `0x37` (one address jumper set) |
| Display | SSD1306 128×64 OLED (two-tone: yellow top strip, blue main area) — I2C address `0x3C` |

### Wiring

All peripherals connect via I2C. Solder wires to the XIAO's SDA/SCL pads and connect to the first Stemma QT encoder; daisy-chain the second encoder and display from there.

**Stemma QT pinout:**
| Wire | Signal |
|---|---|
| Black | GND |
| Red | 3.3V |
| Blue | SDA |
| Yellow | SCL |

---

## Controls

### Left Encoder
| Action | Effect |
|---|---|
| Rotate CW | Increase duty cycle (+5% per detent) |
| Rotate CCW | Decrease duty cycle (−5% per detent) |
| Press | Toggle between PWM/ON and **Disabled** |

### Right Encoder
| Action | Effect |
|---|---|
| Rotate CW | Increase cycle duration (+0.5s per detent) |
| Rotate CCW | Decrease cycle duration (−0.5s per detent, min 1s) |
| Press | Toggle between **PWM** and **Always ON** |

### Relay States

| State | Description | NeoPixel | Display |
|---|---|---|---|
| **PWM** | Relay switches on/off per duty cycle and duration | Green (ON phase) / Blue (OFF phase) | Duty % and duration |
| **Always ON** | Relay held on continuously, params preserved | Green | `ALWAYS ON` |
| **Disabled** | Relay held off, params preserved | Yellow | `DISABLED` |

---

## Display Layout

```
┌──────────────────────────────┐  ← yellow strip (rows 0–15)
│  ████████████████████████   │     solid = relay ON, outline = relay OFF
├──────────────────────────────┤  ← blue zone (rows 16–63)
│                              │
│       25%        4.0         │     duty cycle and duration (PWM state)
│                              │
└──────────────────────────────┘
```

---

## Web Interface

Once connected to WiFi the device is reachable at its static IP (default `192.168.0.210`).

- Duty cycle and duration fields update in real time as encoders are turned
- Changes typed or spun in the web UI take effect immediately — no Apply button needed
- The Apply button still works for committing typed values

---

## Configuration

All tunable constants live in `include/config.h`:

```cpp
#define RELAY_PIN           D1
#define DEFAULT_DUTY_CYCLE  25      // percent
#define DEFAULT_DURATION    4.0f    // seconds
#define DUTY_STEP           5       // % per encoder detent
#define DURATION_STEP       0.5f    // seconds per encoder detent
#define BUTTON_DEBOUNCE_MS  50
#define ENCODER_LEFT_ADDR   0x36
#define ENCODER_RIGHT_ADDR  0x37
```

Network settings (static IP, gateway, subnet) are also in `config.h`.

### WiFi Credentials

Credentials are kept **outside the repository** to avoid accidental commits. Copy `secrets.h.template` to `d:/Projects/secrets.h` (one level above the repo root) and fill in your SSID and password:

```cpp
#define WIFI_SSID     "your_ssid"
#define WIFI_PASSWORD "your_password"
```

---

## Building

Requires [PlatformIO](https://platformio.org/). With the project open:

```
pio run
```

To build and flash:

```
pio run --target upload
```

---

## Credits

Created collaboratively by **Scott Ferguson** and **[Claude](https://claude.ai)** (Anthropic), who handled the bulk of the code generation, debugging, and architecture under Scott's direction.
