# EspScope

An ESP32-based oscilloscope with web interface, migrated to PlatformIO.

<p align="center">
  <a href="./LICENSE">
    <img src="https://img.shields.io/badge/License-MIT-yellow.svg" alt="License: MIT">
  </a>
  <a href="https://www.espressif.com/en/products/socs/esp32">
    <img src="https://img.shields.io/badge/Platform-ESP32-blue.svg" alt="Platform">
  </a>
  <a href="https://www.arduino.cc/">
    <img src="https://img.shields.io/badge/Framework-Arduino-orange.svg" alt="Framework">
  </a>
  <a href="https://platformio.org/">
    <img src="https://img.shields.io/badge/IDE-PlatformIO-purple.svg" alt="IDE">
  </a>
</p>

EspScope is a modernized fork of [Esp32_oscilloscope](https://github.com/BojanJurca/Esp32_oscilloscope) by Bojan Jurca. It provides real-time signal visualization through a web browser using an ESP32 microcontroller.

## Features

- Real-time signal visualization via web browser
- Digital and analog signal sampling
- Dual channel support (CH1 & CH2)
- Configurable sampling frequency (0.1 Hz - 10 kHz)
- Adjustable sensitivity and vertical position
- Positive and negative slope triggers
- WebSocket-based real-time data streaming
- Responsive web interface
- Settings persistence via cookies

## WiFi Modes

EspScope supports two WiFi modes:

### Station Mode (STA)

Connect to an existing WiFi network. ESP32 will obtain an IP address from the router via DHCP or use a static IP configuration.

```cpp
#define DEFAULT_STA_SSID      "YOUR_WIFI_SSID"
#define DEFAULT_STA_PASSWORD  "YOUR_WIFI_PASSWORD"
```

### Access Point Mode (AP)

ESP32 acts as a WiFi hotspot. You can connect directly to ESP32 without a router.

```cpp
#define DEFAULT_AP_SSID     "espScope"
#define DEFAULT_AP_PASSWORD "12345678"
```

Both modes can be used simultaneously (STA+AP) by defining both credentials.

## Hardware Requirements

| Component       | Specification     |
| --------------- | ----------------- |
| Microcontroller | ESP32 DevKit V1   |
| ADC             | 12-bit (0-4095)   |
| WiFi            | 802.11 b/g/n      |
| Input Voltage   | 0 - 3.3V (analog) |

## Project Structure

```
EspScope/
├── data/
│   └── var/
│       └── www/
│           └── html/
│               ├── oscilloscope.html    # Web interface
│               ├── oscilloscope.css     # Styles
│               └── oscilloscope.js      # Client-side logic
├── include/
│   ├── oscilloscope.h                   # Oscilloscope core
│   └── server_config.h                  # WiFi & server configuration
│
├── src/
│   └── main.cpp                         # Main firmware
├── lib/                                 # External libraries
├── platformio.ini                       # PlatformIO configuration
├── LICENSE                              # MIT License
└── README.md
```

## Getting Started

### Prerequisites

- [PlatformIO CLI](https://docs.platformio.org/en/latest/core/installation.html) or [VS Code](https://code.visualstudio.com/) with PlatformIO Extension
- ESP32 DevKit V1 board
- USB cable for flashing

### Installation

1. Clone this repository:

```bash
git clone https://github.com/haanzx/EspScope.git
cd EspScope
```

2. Open the project in PlatformIO IDE or VS Code.

### Configuration

Edit `include/server_config.h` to configure:

- WiFi credentials (STA and/or AP mode)
- Static IP address (optional)
- Hostname
- NTP servers

### Upload Filesystem

Upload the web interface files to ESP32's LittleFS:

```bash
pio run -t uploadfs
```

### Upload Firmware

Build and upload the firmware:

```bash
pio run -t upload
```

### Monitor Serial Output

```bash
pio device monitor
```

## Usage

1. Power on the ESP32
2. Connect to the WiFi network (either the ESP32 AP or your router if using STA mode)
3. Open a web browser and navigate to:
   - STA mode: Check serial monitor for IP address
   - AP mode: `http://192.168.1.1`
4. Configure GPIO pins and sampling parameters
5. Click **START** to begin oscilloscope

## GPIO Limitations

| GPIO           | Analog | Digital |
| -------------- | ------ | ------- |
| 4, 16          | ❌     | ✅      |
| 25, 32, 33, 34 | ✅     | ❌      |
| Others         | ✅     | ✅      |

## Contributing

Contributions are welcome! Feel free to open issues or submit pull requests.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Security

If you discover a security vulnerability, please report it responsibly. Please do not open a public issue for security-related vulnerabilities.

Instead, report it privately via email:

Email: [roysetiono25@gmail.com](mailto:roysetiono25@gmail.com)

### Security Best Practices

- Change the default AP password before deployment
- Use WPA2 or stronger encryption for WiFi connections
- Keep the firmware updated

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Credits

- **Original Project**: [Esp32_oscilloscope](https://github.com/BojanJurca/Esp32_oscilloscope) by [Bojan Jurca](https://github.com/BojanJurca)
- **Libraries Used**:
  - [ESP32 Multitasking Network Suite](https://github.com/BojanJurca/Multitasking-Http-Ftp-Telnet-Ntp-Smtp-Servers-and-clients-for-ESP32-Arduino-Library) by Bojan Jurca
  - [Thread-safe filesystem wrapper](https://github.com/BojanJurca/Thread-safe-file-sytem-wrapper-Arduino-library-for-ESP32) by Bojan Jurca
  - [Lightweight STL](https://github.com/BojanJurca/Lightweight-Standard-Template-Library-STL-for-Arduino) by Bojan Jurca

## Original Project

This project is based on: https://github.com/BojanJurca/Esp32_oscilloscope

Thanks to Bojan Jurca for the original ESP32 oscilloscope project.
