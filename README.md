# Live GPS Tracking with ESP8266

This project implements a live GPS tracking system using an ESP8266 microcontroller, a GPS module, and WiFi. The ESP8266 connects to a WiFi network using WiFiManager, and the GPS module provides real-time location data (latitude, longitude, altitude, speed, course, and HDOP). The data is displayed in a simple web interface hosted by the ESP8266.

## Features

- **WiFi Connectivity:** The ESP8266 connects to a WiFi network automatically or through a configuration portal if no saved network is found.
- **GPS Tracking:** Real-time GPS data such as latitude, longitude, altitude, speed, course, and HDOP.
- **Web Interface:** Displays live GPS information on a web page. You can also view the location on Google Maps.
- **LED Indicators:** The on-board LED indicates WiFi connection status (blinks when connected, solid when disconnected).

## Hardware Required

- **ESP8266 (e.g., NodeMCU, Wemos D1 Mini)**
- **GPS Module (e.g., NEO-6M)**
- **LED (optional for status indication)**

## Circuit Diagram

- **GPS Module to ESP8266 :**
  - GPS TX -> ESP8266 RX (D7)
  - GPS RX -> ESP8266 TX (D6)
  - GPS GND -> ESP8266 GND
  - GPS VCC -> ESP8266 3.3V or 5V (depending on GPS module)

- **LED (optional for status indication) :**
  - LED Anode -> D4
  - LED Cathode -> GND

## Installation

### 1. Clone the repository :

```bash
git clone https://github.com/Devansh-66/GPS_Tracker_Neo-6m.git
cd GPS_Tracker_Neo-6m
```
### 2. Install the necessary libraries in the Arduino IDE :

File > Preferences > Additional Boards Manager URLs
```bash
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```
Next :

Go to Tools > Board > Boards Manager, search esp8266 and install it.

### 3. Other required libraries :

```
SoftwareSerial, ESP8266WiFi, WiFiManager, TinyGPS++
```
Install them through Sketch > Include Library > Manage Libraries and search for each library.
