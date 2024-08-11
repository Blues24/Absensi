# RFID Data Writer with ESP8266 and I2C LCD

This project allows you to write data to an RFID card using an ESP8266 microcontroller and an MFRC522 RFID reader module. The project also includes an I2C LCD to display status messages.

## Features
- Write custom data to RFID cards via Serial Monitor.
- Display status messages on a 16x2 I2C LCD.
- Easily configurable pins for RFID reader and LCD.

## Requirements
- **Hardware:**
  - ESP8266 (e.g., NodeMCU or Wemos D1 Mini)
  - MFRC522 RFID Reader Module
  - RFID cards or tags
  - I2C LCD (16x2)
  - Jumper wires and breadboard (optional)

- **Software:**
  - Arduino IDE
  - MFRC522 Library
  - LiquidCrystal_I2C Library
  - ESP8266 Board Package

## Wiring
- **RFID Reader to ESP8266:**
  - SDA: D4 (GPIO 2)
  - SCK: D5 (GPIO 14)
  - MOSI: D7 (GPIO 13)
  - MISO: D6 (GPIO 12)
  - RST: D3 (GPIO 0)
  - GND: GND
  - 3.3V: 3.3V

- **I2C LCD to ESP8266:**
  - SDA: D2 (GPIO 4)
  - SCL: D1 (GPIO 5)
  - GND: GND
  - VCC: 3.3V or 5V

## Installation
1. **Install Arduino IDE:** Download and install the Arduino IDE from the [official website](https://www.arduino.cc/en/software).

2. **Install ESP8266 Board Package:**
   - In Arduino IDE, go to `File` > `Preferences`.
   - Add the following URL to the "Additional Boards Manager URLs" field:
     ```
     http://arduino.esp8266.com/stable/package_esp8266com_index.json
     ```
   - Go to `Tools` > `Board` > `Boards Manager` and search for "esp8266". Install the package.

3. **Install Required Libraries:**
   - Go to `Sketch` > `Include Library` > `Manage Libraries`.
   - Search for and install the following libraries:
     - `MFRC522`
     - `LiquidCrystal_I2C`

4. **Upload the Code:**
   - Connect the ESP8266 to your computer.
   - Open the provided Arduino sketch.
   - Select the correct board and port under `Tools`.
   - Upload the code to the ESP8266.

## Usage
1. **Connect the hardware** as per the wiring instructions.
2. **Open Serial Monitor** in Arduino IDE with a baud rate of 115200.
3. **Place an RFID card** on the RFID reader.
4. **Enter the data** you want to write to the RFID card in the Serial Monitor and press Enter.
5. The data will be written to the card, and the LCD will display the status.

## Troubleshooting
- **No output on the LCD:** Check the wiring and ensure the correct I2C address is set in the code.
- **RFID not detected:** Ensure the RFID reader is properly connected and powered.

## License
This project is under license GPL-3.0 license
 
