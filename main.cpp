#include "mbed.h"

// Shift register control pins for 7-segment display
DigitalOut latchPin(D4); // ST_CP - Latch pin
DigitalOut clockPin(D7); // SH_CP - Clock pin
DigitalOut dataPin(D8);  // DS - Data pin

// Button inputs (active LOW)
DigitalIn s1(A1), s3(A3); // S1 resets clock, S3 shows voltage

// Analog input from potentiometer
AnalogIn potentiometer(A0); // Reads voltage from potentiometer

// Segment patterns for common anode 7-segment display (inverted logic)
const uint8_t digitPattern[10] = {
    static_cast<uint8_t>(~0x3F), // 0
    static_cast<uint8_t>(~0x06), // 1
    static_cast<uint8_t>(~0x5B), // 2
    static_cast<uint8_t>(~0x4F), // 3
    static_cast<uint8_t>(~0x66), // 4
    static_cast<uint8_t>(~0x6D), // 5
    static_cast<uint8_t>(~0x7D), // 6
    static_cast<uint8_t>(~0x07), // 7
    static_cast<uint8_t>(~0x7F), // 8
    static_cast<uint8_t>(~0x6F)  // 9
};

// Digit selection for 4-digit display (1-hot encoding)
const uint8_t digitPos[4] = {
    0x01, // Leftmost digit
    0x02, // Second digit
    0x04, // Third digit
    0x08  // Rightmost digit
};

// Time counters
volatile int seconds = 0, minutes = 0;
Ticker timerTicker; // Ticker to update time every second

// Called every second to update time
void updateTime() {
  seconds++;
  if (seconds >= 60) {
    seconds = 0;
    minutes++;
    if (minutes >= 100) // Limit to 2-digit minutes
      minutes = 0;
  }
}

// Sends a byte to the shift register (MSB first)
void shiftOutMSBFirst(uint8_t value) {
  for (int i = 7; i >= 0; i--) {
    dataPin = (value & (1 << i)) ? 1 : 0;
    clockPin = 1;
    clockPin = 0;
  }
}

// Sends segment and digit data to the shift register
void writeToShiftRegister(uint8_t segments, uint8_t digit) {
  latchPin = 0;
  shiftOutMSBFirst(segments); // Segment data
  shiftOutMSBFirst(digit);    // Digit position
  latchPin = 1;
}

// Displays a 4-digit number with optional decimal point
void displayNumber(int number, bool showDecimalPoint = false,
                   int decimalPosition = 1) {
  int digits[4] = {(number / 1000) % 10, (number / 100) % 10,
                   (number / 10) % 10, number % 10};

  for (int i = 0; i < 4; i++) {
    uint8_t segments = digitPattern[digits[i]];

    // Enable decimal point if required (bit 7 = decimal point)
    if (showDecimalPoint && i == decimalPosition) {
      segments &= ~0x80; // Turn ON decimal point (clear bit)
    }

    writeToShiftRegister(segments, digitPos[i]);
    ThisThread::sleep_for(2ms); // Short delay for multiplexing
  }
}

int main() {
  // Initialize shift register pins
  latchPin = 0;
  clockPin = 0;
  dataPin = 0;

  // Enable internal pull-up resistors for buttons
  s1.mode(PullUp);
  s3.mode(PullUp);

  // Start the real-time clock
  timerTicker.attach(&updateTime, 1.0f); // Call updateTime every 1 second

  while (1) {
    // Reset time if S1 is pressed
    if (!s1) {
      seconds = minutes = 0;
      ThisThread::sleep_for(200ms); // Debounce delay
    }

    // If S3 is pressed, show voltage from potentiometer
    if (!s3) {
      float voltage = potentiometer.read_u16() / 65535.0f * 3.3f; // Convert to volts
      int voltageValue = (int)(voltage * 1000); // Convert to mV for display (e.g., 2.75V → 2750)
      displayNumber(voltageValue, true, 0); // Show decimal after first digit
    } else {
      // Otherwise, show time in MM:SS format
      int timeValue = minutes * 100 + seconds;
      displayNumber(timeValue, true, 1); // Decimal after minutes
    }
  }
}
