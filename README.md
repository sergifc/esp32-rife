# ESP32 RIFE Controller

RIFE frequency generator device using ESP32-S3 + JC3248W535 480x320 display.

## Hardware

- **MCU**: ESP32-S3 (WROOM or N16R8)
- **Display**: JC3248W535 3.95" 480x320 IPS (AXS15231B driver, QSPI, I2C touch)
- **DAC**: MCP4725 or built-in ESP32 DAC for frequency output
- **Storage**: SPIFFS (16MB Flash)

## Build Instructions

### 1. Install PlatformIO

```bash
# Install VS Code extension or use CLI
pip install platformio
```

### 2. Configure WiFi

Edit `src/config.h`:
```cpp
#define WIFI_SSID "YourNetwork"
#define WIFI_PASS "YourPassword"
```

### 3. Build & Upload

```bash
pio run --target upload        # Upload firmware
pio run --target uploadfs      # Upload SPIFFS data (frequencies.json)
pio device monitor             # Open serial monitor
```

## Project Structure

```
esp32-rife/
├── data/
│   └── frequencies.json    # 131 Spooky2 presets (auto-generated)
├── src/
│   ├── main.cpp            # Entry point
│   ├── config.h            # Pinout & WiFi config
│   ├── PresetManager.*     # JSON preset loading
│   ├── FrequencyEngine.*   # Frequency generation
│   ├── WifiManager.*       # WiFi + HTTP download
│   └── DisplayUI.*         # LovyanGFX UI
├── convert_spooky2.py      # Spooky2 converter
└── platformio.ini          # Build config
```

## Converter Usage

```bash
python convert_spooky2.py
# Outputs: data/frequencies.json
```

## UI Screens

1. **Home**: Device status, quick stats
2. **Categories**: Animal categories (Dog, Cat, Horse...)
3. **Preset List**: Scrollable presets per category
4. **Preset Detail**: Program breakdown
5. **Player**: Frequency display, timer, progress bar

## Color Scheme

| Color | Hex | Usage |
|-------|-----|-------|
| Obsidian Black | `#0a0a0f` | Background |
| Cooper Silk | `#b87333` | Primary accent, selection |
| Shimmer Gold | `#c9a227` | Secondary accent |
| Quantum Cyan | `#00fff7` | Frequency values |
| Medical Green | `#00ff88` | Playing state |

## Pinout (JC3248W535)

| Signal | GPIO |
|--------|------|
| CS | 10 |
| DC | 11 |
| RST | 12 |
| BL | 9 |
| SCL (I2C) | 39 |
| SDA (I2C) | 38 |
| INT | 37 |

## Frequency Output

- GPIO 25: DAC output (0-3.3V analog)
- GPIO 26: Buzzer (optional audio feedback)
