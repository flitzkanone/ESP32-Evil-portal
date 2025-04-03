# ESP32 Captive Portal with Login Logging

## Overview
This project is an ESP32-based **Captive Portal** that forces users to enter their credentials (email and password) before accessing the internet. It logs the entered credentials and provides an **Admin Panel** to view stored logins.

## Features
- **Captive Portal**: Redirects all user traffic to the login page.
- **WiFi Access Point**: The ESP32 acts as an open or password-protected access point.
- **DNS Redirection**: Ensures all web requests go to the login page.
- **Login Page**: HTML-based page where users enter credentials.
- **Admin Panel**: Displays stored credentials and allows clearing logs.
- **Redirect After Login**: Users are redirected to a predefined website after authentication.

## Hardware Requirements
- ESP32-WROOM-32 (Compatible with ESP32-WROOM-32D, ESP32-WROOM-32E, ESP32-WROVER)
- Micro USB cable
- Computer for flashing firmware

## Installation
### Flashing the ESP32
#### Using Arduino IDE
1. Install **Arduino IDE** and add the ESP32 board support package:
   - Open Arduino IDE.
   - Go to **File > Preferences**.
   - In **Additional Board Manager URLs**, add:  
     `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Go to **Tools > Board > Board Manager** and install **esp32 by Espressif Systems**.

2. Install required libraries via the **Library Manager**:
   - **ESPAsyncWebServer**
   - **DNSServer**
   - **ArduinoJson**

3. Download the preconfigured **evilportal_esp32.ide** file from the repository.
- Open Arduino IDE and go to File > Open.
- Select the downloaded .ide file.

4. Compile and upload the code to your ESP32.

### Setting Up SPIFFS Filesystem
1. Install the **ESP32 Sketch Data Upload** plugin for Arduino IDE.
2. Upload `data/` folder contents to SPIFFS.
3. Restart the ESP32 to apply changes.

## Usage
1. Connect to the WiFi network broadcasted by the ESP32.
2. Open any web page – you will be redirected to the login portal.
3. Enter email and password.
4. Credentials are logged and accessible via `/admin` (default password: `admin`).
5. Admin can view and clear logs.

## Configuration
Modify `config.h` to change WiFi name, password, redirect URL, and admin credentials.

## Security Considerations
This project **does not encrypt** login credentials. Do not use it for sensitive applications.

## License
This project is licensed under the MIT License.

## Disclaimer
This project is for educational purposes only. The author is not responsible for any misuse of the software.

