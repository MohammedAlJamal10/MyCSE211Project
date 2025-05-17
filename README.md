# ⏱️ 4-Digit 7-Segment Display with Timer & Voltmeter (Mbed)

This project implements a **4-digit 7-segment display** controller using an STM32 board and Mbed OS. It supports two modes:
- **Timer mode**: Displays elapsed time in MM:SS format.
- **Voltmeter mode**: Displays analog voltage (from a potentiometer) in XX.XX volts.

The user can toggle between modes and reset the timer using push buttons.

---

## 🧰 Hardware Components

| Component        | Pin on STM32     | Description                          |
|------------------|------------------|--------------------------------------|
| 74HC595 Latch    | `PB_5`           | Serial-to-parallel latch (ST_CP)     |
| 74HC595 Clock    | `PA_8`           | Shift register clock (SH_CP)         |
| 74HC595 Data     | `PA_9`           | Serial data input (DS)               |
| Button 1         | `PA_1`           | Timer reset (active-low)             |
| Button 3         | `PB_0`           | Mode toggle (active-low)             |
| Potentiometer    | `PA_0`           | Analog input for voltage reading     |

---

## ⚙️ Features

- ⏱ Timer: Increments every 1 second, rolls over after 99 minutes and 59 seconds.
- 🔌 Voltmeter: Reads analog input and converts it to a voltage value (0–3.3V).
- 📟 4-digit 7-segment multiplexing using shift registers (74HC595).
- 💡 Periodic display refresh every 2 milliseconds for smooth output.
- 🧮 Decimal point is displayed in the appropriate digit in each mode.

---

## 📸 Project Demonstration

Watch the full project demonstration in this Drive folder:

👉 [Google Drive Video Link](https://drive.google.com/drive/folders/1xXHXIRpl0xQ1C8-Y3YCTPF1xxbJQvPD)

---

## 📁 Code Overview

The main source code is in [`main.cpp`](main.cpp). It includes:

- `Ticker` interrupts for time tracking and display refresh.
- Shift register communication using `DigitalOut` for segment control.
- Input debouncing using internal pull-ups and active-low logic.
- Floating-point math to handle voltage reading and conversion.

---

## 🚀 Getting Started

1. Import the project into Mbed Studio or Online Compiler.
2. Wire your 7-segment display with 2 × 74HC595 shift registers.
3. Connect the buttons and potentiometer.
4. Build and flash to your STM32 board.
5. Press:
   - **Button 3 (PB0)** to switch between timer and voltmeter.
   - **Button 1 (PA1)** to reset the timer.

---

## 📦 Dependencies

- Mbed OS 6.x
- STM32-compatible development board (NUCLEO-F401RE)
- 74HC595 shift register
- 4-digit common-cathode 7-segment display

---

## 👨‍💻 Author

Mohammed_AlJamal

---

## 📄 License

This project is licensed under the MIT License.

