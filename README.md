Smart Mailbox: Secure IoT Package Protection System
Overview

Package theft (“porch piracy”) has become a widespread issue, with billions of dollars in losses annually. The Smart Mailbox is an end-to-end embedded and distributed system designed to secure package deliveries through physical locking, remote access, live monitoring, and intelligent detection.

This project integrates embedded systems, full-stack web development, real-time communication, and computer vision to create a secure and user-friendly solution for protecting delivered packages.

System Architecture

The Smart Mailbox is composed of multiple interacting subsystems:

TM4C123 Microcontroller: Handles physical lock control, keypad input, and sensor logic
Raspberry Pi 4 (Backend + Streaming): Acts as a server, bridges communication, and streams live video
Raspberry Pi 5 (Detection System): Performs real-time human detection and triggers alerts
Web Application (React + AWS): Provides remote access, monitoring, and control
Communication Flow
Frontend (React Web App)
        ↓ HTTP (ngrok / AWS)
Raspberry Pi 4 (Flask Backend)
        ↓ UART
TM4C123 Microcontroller
        ↓
Servo Lock + Sensors

Raspberry Pi 5 (Detection)
        ↓
Email Alerts (with image)
Key Features
Secure Locking Mechanism
Servo-driven sliding lock controlled by TM4C123
Lock/unlock via keypad or remote web interface
Remote Web Control
Lock/unlock mailbox from anywhere
View system status and logs
Live Video Streaming
Real-time camera feed streamed via FFmpeg to YouTube
Embedded into web interface
Human Detection and Alerts
Computer vision model detects nearby individuals
Automatically captures image and sends email notification
Keypad Authentication
Matrix keypad scanning using row/column logic
Secure passcode-based access
Safety Mechanism
Ultrasonic sensor ensures lid is closed before locking
Backup Access
Physical key override in case of power failure
Tech Stack
Embedded Systems
C (TM4C123 microcontroller)
UART communication
PWM (servo control)
Interrupts and timers
Backend / Systems
Python (Flask)
Serial communication (pyserial)
Ngrok (tunneling)
AWS Elastic Beanstalk (deployment)
Frontend
React (JavaScript)
REST API communication
Computer Vision
OpenCV (DNN module)
MobileNet SSD (COCO dataset)
Raspberry Pi Camera Module
Streaming
libcamera
FFmpeg
YouTube RTMP
Repository Structure
smart-mailbox/
│
├── frontend/              # React web application
│
├── tm4c_firmware/        # Embedded firmware (C)
│
├── pi_server/            # Raspberry Pi 4 backend + streaming
│   ├── server.py
│   └── stream_to_youtube.py
│
├── pi_detection/         # Raspberry Pi 5 detection system
│   └── object_detection_email.py
│
├── docs/                 # Report, presentation, etc.
│
└── README.md
Demo

(Add your videos here)

## Demo Videos
- Full System Demo: YOUR_LINK_HERE
- Detection and Alerts Demo: YOUR_LINK_HERE


Design Highlights
Designed a distributed IoT system combining embedded firmware, backend services, and machine learning-based detection
Implemented real-time UART communication between Raspberry Pi and microcontroller
Built a full-stack application integrating hardware control with a web interface
Engineered event-driven system behavior for lock control, detection alerts, and streaming
Balanced hardware constraints, network latency, and real-time system requirements
Challenges
Ensuring stable power delivery across multiple hardware components
Maintaining reliable network connectivity for streaming and backend communication
Selecting compatible camera and software stack for real-time processing
Synchronizing hardware and software interactions across distributed systems
Future Improvements
Mobile application integration
Enhanced authentication (multi-user support, two-factor authentication)
Edge-based machine learning optimization for faster detection
Improved battery management and power efficiency
Cloud-based logging and analytics
Summary

The Smart Mailbox demonstrates a complete embedded, full-stack, and machine learning system, combining hardware design, real-time embedded programming, backend communication, frontend development, and computer vision to solve a real-world problem at scale.
