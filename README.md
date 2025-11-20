# Ultrasonic Distance Measurement using ATmega32 📏

![Project Demo](circuit_setup.jpg)
*(Above: The working prototype measuring distance in real-time)*

## 📝 Overview
This project implements a digital distance meter using the **AVR ATmega32 Microcontroller**. It interfaces with an **HC-SR04 Ultrasonic Sensor** to calculate distance and displays the result on a **16x2 LCD**.

Unlike simple blocking implementations, this project utilizes **Hardware Interrupts (INT0)** and **Timer1** to measure the echo pulse width with microsecond precision, ensuring the CPU remains efficient.

## ⚙️ Features
- **Real-time monitoring:** Instant distance updates.
- **Event-Driven Design:** Uses `ISR` (Interrupt Service Routines) for echo detection.
- **Precision Timing:** Uses 16-bit Timer1 for pulse width measurement.
- **Custom Drivers:** LCD driver written from scratch (GPIO manipulation).

## 🔌 Hardware Required
- **Microcontroller:** ATmega32 (or ATmega16)
- **Sensor:** HC-SR04 Ultrasonic Module
- **Display:** 16x2 Character LCD (HD44780 controller)
- **Oscillator:** 1MHz Internal or External Crystal
- Breadboard and Jumper Wires

## 🧠 How It Works (The Logic)
1. **Trigger:** The MCU sends a 10µs HIGH pulse to the Sensor's Trigger pin.
2. **Echo Start:** The Sensor pulls the Echo pin HIGH. The MCU detects this Rising Edge via **INT0** and starts **Timer1**.
3. **Echo End:** When the sound wave returns, the Sensor pulls the Echo pin LOW. The MCU detects this Falling Edge, stops **Timer1**, and captures the count.
4. **Calculation:** 
   > Distance (cm) = Timer Count / 58

## 📂 Code Structure
- `main.c`: Contains the main application logic, ISR implementation, and LCD driver functions.

## 🙏 Acknowledgements
Special thanks to **Miral Sir** and **Brijesh Sir** for their guidance on AVR Architecture, specifically regarding the implementation of Interrupts and Timers.

---
*Author: [Your Name]*
