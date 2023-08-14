/*==================================================
        Master Node Code (Uno)
==================================================*/

// Libraries
#include <Arduino.h>
#include <Wire.h>

const int baudRate = 9600 // Serial baud rate
const int wifiModuleID = 4; // Specify the ESP32 as having a device id of '8'
byte x = 0; 

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(baudRate);  // start serial for output
}

// Function to send data to a slave node

void sendDataI2C(char dataToSend){
    Wire.beginTransmission(wifiModuleID); // transmit to device with id of 'wifiModuleID'
    Wire.write(dataToSend);        // sends data in bytes 
    Wire.endTransmission();    // stop transmitting

    Serial.println("Data Sent...");
    x++;
    delay(500);
}


void loop(){

    // Send data to slave node
    char dataToSendToWifiModule[] = "ur FGM data";
    sendDataI2C(dataToSendToWifiModule);

    delay(100);
}

/*==================================================
        Slave Node Code (Wifi Module)
==================================================*/

#include <Arduino.h>
#include <Wire.h>

// Global variables

const int baudRate = 9600; // Serial baud rate
const int deviceI2CId = 8; // Give the slave node a way to be identified by the master node  

void setup() {
    Serial.begin();

    Wire.begin(deviceI2CId);              // join i2c bus with an address that is the device ID 
    Wire.onReceive(receiveDataI2C);       // handle data sent from master node --> recieve data from master node
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveDataI2C(int howMany)
{
    while(1 < Wire.available()) {   // loop through all but the last
        char c = Wire.read();       // receive byte as a character
        Serial.print(c);            // print the character
    }
    int x = Wire.read();            // receive byte as an integer
    Serial.println(x);              // print the integer
}

void loop() {
    delay(100);
}