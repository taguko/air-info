# Air-info

## Motivation

Our club room is relatively small, and the air quality can easily deteriorate when many people are present. This project was created to provide a simple and convenient way to monitor indoor air conditions in real time.

## Hardware

### Components

- Seeed Studio XIAO ESP32-C6
- 0.96-inch OLED Display
- ENS160 + AHT21 Environmental Sensor Module

### Connection

All peripherals are connected via the I²C bus.

```
XIAO ESP32-C6
├── OLED Display (I²C)
└── ENS160 + AHT21 Module (I²C)
```

## Features

### Automatic Webpage Refresh

The web dashboard automatically reloads every 30 seconds. Simply leave the page open to keep the displayed measurements up to date.

### Easy Access via mDNS

After power-on, the device connects to the club room Wi-Fi network and becomes accessible at:

http://air-info.local/

Hostname resolution is handled using mDNS, so users do not need to know the device's IP address.

### CSV Export

Click **Generate CSV** or visit:

http://air-info.local/generateCSV

The system will generate and download a CSV file containing the recorded measurement data.

The downloaded file can be opened directly in Microsoft Excel or other spreadsheet software.

## Measured Parameters

- Air Quality Index (AQI)
- TVOC (Total Volatile Organic Compounds)
- eCO₂ (Equivalent Carbon Dioxide)
- Temperature
- Relative Humidity

## Future Plans (Maybe)

The following features may be added in the future:

### Remote Monitoring

A Discord bot or similar service could be developed to allow users to check the air quality remotely.

### Data Logging

Measurement results could be periodically saved to a microSD card for long-term storage and analysis.

## Purpose

This project is intended to provide a quick and easy way to assess the air quality of the club room and determine when ventilation may be needed.
