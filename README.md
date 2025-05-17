# ⏱️ Real-Time Clock & Voltage Display using Nucleo + Arduino Shield

This project implements a **real-time clock (RTC)** and **analog voltage display** using an STM32 Nucleo board and an Arduino Multi-Function Shield.

## 🔧 Features

- **Real-Time Clock (RTC)**:
  - Displays elapsed time in **MM:SS** format on a 4-digit 7-segment display.
  - Time starts from 00:00 after reset.
  - Pressing **S1** resets the clock to zero.

- **Analog Voltage Display**:
  - Pressing **S3** shows the voltage from the on-board potentiometer.
  - Voltage is displayed in **volts** with 3 decimal places (e.g., 2.750).
  - Releasing **S3** returns to time display without interrupting the RTC.

## 🧪 Hardware Used

- **STM32 Nucleo Board**
- **Arduino Multi-Function Shield**
  - 4-digit 7-segment display
  - On-board potentiometer
  - Push buttons (S1, S3)
  - 74HC595 shift register

## ⚙️ How It Works

- The 7-segment display is driven using a **74HC595 shift register**.
- The **RTC** is implemented using a `Ticker` that increments time every second.
- The **potentiometer** voltage is read using the on-chip ADC and scaled to volts.
- The display is **multiplexed** to show 4 digits with optional decimal points.

## 📏 Voltage Range

- The potentiometer provides a voltage between **0V and 3.3V**.
- This is read and displayed as a 4-digit number with a decimal point (e.g., 3.300V → `3300`).

## 🛠️ Setup Instructions

1. Connect the Arduino Multi-Function Shield to the Nucleo board.
2. Flash the provided code using Mbed Studio or another compatible IDE.
3. Power the board and observe the display:
   - Time starts counting.
   - Press **S1** to reset.
   - Press **S3** to view voltage.

## 📁 File Structure

- `main.cpp` — Main application code with RTC and voltage display logic.
- `README.md` — Project overview and documentation.

## 📸 Demo

> *(Add a photo or video of the working project here if available)*

---

Made with ❤️ using STM32 and Mbed OS.
