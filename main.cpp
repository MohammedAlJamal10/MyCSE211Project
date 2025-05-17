#include "mbed.h"

// Define pins for shift register control and input devices
DigitalOut latch(PB_5);      // Latch pin for shift register (to output data)
DigitalOut clk(PA_8);        // Clock pin for shift register
DigitalOut data(PA_9);       // Data pin for shift register
DigitalIn button1(PA_1);     // Button to reset the timer
DigitalIn button3(PB_0);     // Button to switch between timer and voltmeter modes
AnalogIn pot(PA_0);          // Potentiometer input for voltage reading

// 7-segment encoding for digits 0–9 (common cathode; 0 = ON)
const uint8_t SEGMENT_MAP[10] = {
    0xC0, // 0
    0xF9, // 1
    0xA4, // 2
    0xB0, // 3
    0x99, // 4
    0x92, // 5
    0x82, // 6
    0xF8, // 7
    0x80, // 8
    0x90  // 9
};

// Values to activate each of the 4 digits (digit select)
const uint8_t SEGMENT_SELECT[4] = {
    0xF1, // Digit 0
    0xF2, // Digit 1
    0xF4, // Digit 2
    0xF8  // Digit 3
};

volatile int total_seconds = 0;     // Counter for elapsed time in seconds
volatile bool updateDisplay = true; // Flag to trigger display refresh
Ticker second_tick, refresh_tick;   // Tickers for time counting and display multiplexing
volatile int currDigit = 0;         // Tracks which digit is being updated (0-3)
bool modeVoltage = false;           // Mode flag: false = timer, true = voltmeter

// Function called every second to increment timer
void tick() {
    total_seconds = (total_seconds + 1) % 6000; // Wrap around after 99:59 (6000 seconds)
}

// Sends segment and digit select data to the shift registers
void outputToDisplay(uint8_t segments, uint8_t digitSelect) {
    latch = 0; // Prepare to shift data

    // Shift segment bits (MSB first)
    for (int i = 7; i >= 0; --i) {
        data = (segments >> i) & 0x1;
        clk = 0;
        clk = 1;
    }

    // Shift digit select bits (MSB first)
    for (int i = 7; i >= 0; --i) {
        data = (digitSelect >> i) & 0x1;
        clk = 0;
        clk = 1;
    }

    latch = 1; // Output the shifted data to the display
}

// Function called every 2ms to trigger a display update
void refreshISR() {
    updateDisplay = true;
}

int main() {
    // Configure buttons with internal pull-up resistors
    button1.mode(PullUp);
    button3.mode(PullUp);

    // Set up periodic interrupt for 1-second timer updates
    second_tick.attach(&tick, 1.0);

    // Set up periodic interrupt for display refresh every 2 ms
    refresh_tick.attach(&refreshISR, 0.002);

    while (true) {
        // If button1 is pressed (logic low), reset the timer
        if (!button1.read()) total_seconds = 0;

        // If button3 is pressed (logic low), switch to voltage mode
        modeVoltage = !button3.read();

        // Only refresh display when flag is set
        if (updateDisplay) {
            updateDisplay = false;

            uint8_t segByte = 0xFF;                          // Default all segments off
            uint8_t selByte = SEGMENT_SELECT[currDigit];    // Select the current digit

            // Voltmeter mode
            if (modeVoltage) {
                float volts = pot.read() * 3.3f;             // Read analog voltage (0 to 3.3V)
                int centivolts = (int)(volts * 100.0f);      // Convert to centivolts
                int intPart = centivolts / 100;              // Integer part (volts)
                int fracPart = centivolts % 100;             // Fractional part (centivolts)

                // Assign segment values based on current digit
                if (currDigit == 0) segByte = SEGMENT_MAP[intPart] & 0x7F; // Add decimal point
                if (currDigit == 1) segByte = SEGMENT_MAP[fracPart / 10]; // First decimal
                if (currDigit == 2) segByte = SEGMENT_MAP[fracPart % 10]; // Second decimal
                // currDigit == 3 is unused in voltmeter mode
            } else {
                // Timer mode
                int minutes = total_seconds / 60;
                int seconds = total_seconds % 60;

                // Assign segment values based on current digit
                if (currDigit == 0) segByte = SEGMENT_MAP[minutes / 10];         // Tens of minutes
                if (currDigit == 1) segByte = SEGMENT_MAP[minutes % 10] & 0x7F;  // Units of minutes with decimal
                if (currDigit == 2) segByte = SEGMENT_MAP[seconds / 10];         // Tens of seconds
                if (currDigit == 3) segByte = SEGMENT_MAP[seconds % 10];         // Units of seconds
            }

            // Output the segment and digit selection to the display
            outputToDisplay(segByte, selByte);

            // Move to next digit for the next refresh
            currDigit = (currDigit + 1) % 4;
        }
    }
}
