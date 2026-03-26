# Smart Mailbox: Secure IoT Package Protection System

Developed a distributed embedded and full-stack system to secure package deliveries using microcontroller-based locking, remote web control, live video streaming, and real-time human detection.

---

## Overview
This project implements a Smart Mailbox system designed to prevent package theft through a combination of embedded control, cloud-connected interfaces, and computer vision.

The system integrates a microcontroller, multiple Raspberry Pi subsystems, and a web application to provide secure physical access, remote monitoring, and automated alerts.

---

## Motivation
Package theft (“porch piracy”) is a widespread and costly problem, yet most existing solutions are limited:

- Passive (cameras without action)  
- Not integrated with physical locking systems  
- Lack real-time response or remote control  

This project addresses these limitations by combining physical security, remote access, and intelligent detection into a single system.

---

## Key Idea
Instead of treating security, monitoring, and control as separate systems, we:

1. Secure the mailbox physically using embedded control  
2. Enable remote interaction through a web interface  
3. Monitor activity using live video and detection  
4. Trigger alerts when suspicious behavior is detected  

The result is a fully integrated, real-time IoT security system.

---

## System Architecture

The system is composed of four main components:

- **TM4C123 Microcontroller**  
  Controls lock mechanism, keypad input, and sensors  

- **Raspberry Pi 4 (Backend + Streaming)**  
  Hosts API server and manages UART communication  
  Streams live video feed  

- **Raspberry Pi 5 (Detection System)**  
  Performs real-time human detection  
  Sends email alerts with captured images  

- **Web Application (React + AWS)**  
  Provides user interface for control and monitoring  

---

## System Pipeline

### 1. User Interaction (Frontend)
- User accesses web application  
- Can lock/unlock mailbox  
- View system status  
- Watch live video feed  

---

### 2. Backend Communication (Pi 4)
- Flask server receives HTTP requests  
- Converts commands into UART signals  
- Sends control messages to TM4C  

---

### 3. Embedded Control (TM4C123)
- Processes UART commands  
- Controls:
  - Servo motor (lock mechanism)  
  - Keypad authentication  
  - Ultrasonic sensor (safety check)  

- Ensures mailbox is closed before locking  

---

### 4. Live Streaming (Pi 4)
- Captures video using camera module  
- Streams via FFmpeg to YouTube  
- Embedded into web interface  

---

### 5. Detection and Alerts (Pi 5)
- Captures frames using Pi camera  
- Runs object detection (MobileNet SSD via OpenCV)  
- Detects presence of a person  

If detected:
- Saves image  
- Sends email notification with attachment  
- Uses cooldown to prevent spam  

---


---

## Key Features

- Remote lock/unlock via web interface  
- Keypad-based local authentication  
- Live video streaming integrated into UI  
- Real-time human detection with email alerts  
- Safety mechanism using ultrasonic sensor  
- Distributed system across multiple devices  
- Physical key override for reliability  

---

## Design Highlights

- Built a distributed IoT architecture across microcontroller and multiple compute nodes  
- Integrated hardware control with web-based interfaces  
- Implemented real-time UART communication pipeline  
- Designed event-driven system behavior across subsystems  
- Combined embedded systems, backend, and computer vision in one platform  

---

## Challenges

- Synchronizing communication across distributed components  
- Maintaining reliable UART communication between Pi and TM4C  
- Ensuring stable video streaming under network constraints  
- Managing power and hardware reliability across devices  
- Integrating real-time detection without excessive latency  

---

## Key Insights

- System integration is often harder than individual components  
- Real-time hardware/software interaction introduces complex edge cases  
- Simpler models can be effective for embedded detection  
- Reliability is critical in physical systems  
- Clear system separation improves debugging and scalability  

---

## Future Work

- Mobile application integration  
- Multi-user authentication and permissions  
- Edge optimization for faster detection  
- Battery and power management improvements  
- Cloud-based logging and analytics  

---

## Tech Stack

**Embedded Systems**
- C (TM4C123)  
- UART, PWM, interrupts, timers  

**Backend**
- Python (Flask)  
- Serial communication (pyserial)  
- Ngrok / AWS  

**Frontend**
- React (JavaScript)  
- REST APIs  

**Computer Vision**
- OpenCV (DNN)  
- MobileNet SSD (COCO dataset)  

**Streaming**
- libcamera  
- FFmpeg  
- YouTube RTMP  

---

## Repository Structure


---

## Demo

- Full System Demo: https://youtu.be/WmfT31LRui8
- Detection Demo: https://youtu.be/29SjOkDS30s

---

## Summary

This project demonstrates a complete system combining embedded control, distributed computing, web interfaces, and computer vision to solve a real-world security problem.

It highlights the challenges and design considerations involved in building reliable, real-time, multi-component engineering systems.

## Communication Flow
