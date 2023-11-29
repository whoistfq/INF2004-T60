
## Project: User Location Tracking and Monitoring System

## PROJECT OVERVIEW
Our team aims to develop a user location tracking and monitoring system for our project using the Raspberry Pi Pico W configured as a Mesh-connected rogue access point (AP). This system is designed for scenarios where users voluntarily participate and provide consent for tracking. It can have various applications, such as tracking people within a campus. Here is a scenario of how the Pico should function:

COMPONENTS: Raspberry Pi Pico W, ESP-01, ESP32, and LoRa transceiver modules (Min 2) are required. 

## Objective 

SD Card Driver
* Create an SD Card Driver such that it can read/write from the SD card.
* This component is necessary to store stolen credentials.
* A User Interface for configuring and managing the system, displaying real-time network statistics. 

NAT ROUTER AP CONFIGURATION SETUP 
* Develop an ethernet driver that will create the mesh connection between two devices.
* Ensure that the device driver supports sending and receiving ethernet connections
* Ensure that it is configured such that anyone connected to the AP can still resume using the internet normally.

Deauthentication Packets Broadcasting Device
* Ensure that broadcast packets can be sent to a targetted SSID of choice.
* While the tool is active, no devices should remain connected to the network.
* A User Interface should be developed for easy target selection and attack.

An Evil-Twin Webpage Hosting 
* Cloning existing known pages and hosting the webpage on a Pico Device.
* Log and Store Credentials entered by the user.

Mesh Location Tracking and Monitoring 
* Implement mesh packet capture capabilities using Pico's Lora module or ESP-01/3266 module.
* Develop packet analysis routines to determine user location based on which AP point they are connected to and backup their activity (Online time and location). 


## Overview of Project (Modular Design)
[Block Diagram](https://github.com/whoistfq/INF2004-T60/blob/main/block_diagram.png)
[Flow Diagram](https://github.com/whoistfq/INF2004-T60/blob/main/flow_diagram.png)

## Requirements to run (i.e. what libraries are needed with their version),
* ESP-IDF: https://github.com/espressif/esp-idf
* PlatformIO (VSCode): https://platformio.org/
* Web


## How to run the program
Deauther: Flash the (filename) program into the ESP processor. 

## Citations on any referenced code used
awokdnopwainjdopwand

## Task Allocation
* Lang Jun Feng (2200566): Evil Twin webpage hosting and saving extracted stolen login credentials to SD card
* Chua Tee Kiat (2201304): Deauthentication broadcaster 
* Goh Yue Jun (2201471): UI, Logging & Reporting on SD Card Reader
* Muhammad Irsyad Bin Abdul Kadir (2200543): 
* Muhammad Taufiq Bin Ismail (2200653): 


