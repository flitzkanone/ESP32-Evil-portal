# ESP32-Evil-portal
ESP32 Captive Portal with Login Logging

# Overview

This project is an ESP32-based Captive Portal that forces users to enter their credentials (email and password) before accessing the internet. It logs the entered credentials and provides an Admin Panel to view stored logins.

# Features

Captive Portal: Redirects all user traffic to the login page.

WiFi Access Point: The ESP32 acts as an open or password-protected access point.

DNS Redirection: Ensures all web requests go to the login page.

Login Page: HTML-based page where users enter credentials.

Admin Panel: Displays stored credentials and allows clearing logs.

Redirect After Login: Users are redirected to a predefined website after authentication.

# Hardware Requirements

ESP32-WROOM-32
ESP32-WROVER 
ESP32-S2
ESP32-S3 
ESP32-C3 
ESP32-PICO

Micro USB cable

Computer for flashing firmware
