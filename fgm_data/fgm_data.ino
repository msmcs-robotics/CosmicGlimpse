/* 
Objectives:

1. Send power to flux-gate-magnatometer power pins from arduino pins one at a time
2. Read from flux-gate-magnatometer data pins to arduino one at a time
3. Interpret data as a frequency
4. Store resulting frequency from a specified time interval in a single variable

*/

// Libraries

#include <Arduino.h>
#include <FreqCount.h> // https://github.com/PaulStoffregen/FreqCount/releases/tag/1.3

// Pinout

const int fgmPowerPinX = 4;
const int fgmPowerPinY = 5;
const int fgmPowerPinZ = 6;

const int fgmDataPinX = 3;
const int fgmDataPinY = 9;
const int fgmDataPinZ = 10;

// Global Variables

const int baudRate = 9600; // Serial Baud Rate
const int gateInterval = 1000; // The time in milliseconds for each measurement. - https://www.pjrc.com/teensy/td_libs_FreqCount.html#:~:text=FreqCount.begin,a%20scale%20factor


// Setup

void setup(){

    Serial.begin(baudRate);
    FreqCount.begin(gateInterval);

    pinMode(fgmDataPin1, INPUT);
    pinMode(fgmDataPin2, INPUT);
    pinMode(fgmDataPin3, INPUT);

};

void shiftPin(int powerPin, int dataPin, bool collectState) {
    // Change which pin is being used to collect frequencies - X, Y, or Z

    if ( collectState == false ) {
        digitalWrite(powerPin, LOW); // Turn off power to sensor pin
    } else if ( collectState == true ) {
        digitalWrite(powerPin, HIGH); // Turn on power to sensor pin
    } else {
        Serial.println("Incorrect 'collectState' in shiftPin function");
    }
};

void collectData() {
    // Collect an average of frequencies and process the data into a single variable
};

void loop() {
    // Collect frequencies from X, Y, and Z axi 
    if (FreqCount.available()) {
        unsigned long count = FreqCount.read();
        Serial.println(count);
    }
    
    // Map frequencies to magnetic field
    // int bField = freqToMag();
    
    // Send data to wifi module over I2C
    // sendDataToWifi(data); 
};