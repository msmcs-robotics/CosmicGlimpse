/*

Objectives:

1. Connect ESP32 to lcd screen
2. Get mac address of ESP32
3. Get IP address of ESP32 if successfullly connected to network
4. Display information on LCD screen

*/


// Pinout
// ESP32 Wiring Diagram - https://randomnerdtutorials.com/esp32-esp8266-i2c-lcd-arduino-ide/#:~:text=Wiring%20the%20LCD%20to%20the%20ESP32

// Libraries
#include <Wire.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>

// Global variables
const int baudRate = 9600; // Serial baud rate

char macAddr[]; // Mac Address of ESP32
char ipAddr[]; // IP Address of ESP32

int lcdColumns = 16; // Number of horizontal areas on LCD screen
int lcdRows = 2; // Number of vertical areas on LCD screen
byte lcdAddress = 0xFF; // Initialize with an invalid I2C address


// Auxillary Functions

void lcdI2CScanner() {
    Serial.println("Starting I2C Scanner To Find LCD Address...");
    byte error, address;
    int nDevices;
    Serial.println("Scanning...");
    nDevices = 0;
    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address<16) {
                Serial.print("0");
            }
            Serial.println(address,HEX);
            lcdAddress = address;
            nDevices++;
        }
        else if (error==4) {
            Serial.print("Unknow error at address 0x");
            if (address<16) {
                Serial.print("0");
            }
            Serial.println(address,HEX);
            lcdAddress = address;
        }    
    }
    if (nDevices == 0) {
        Serial.println("No I2C devices found\n");
    }
    else {
        Serial.println("done\n");
    }
    delay(5000);          
}

void displayInfoOnLCD(char *macAddress, char *ipAddress){
    // set cursor to first column, first row
    lcd.setCursor(0, 0);
    // print message for first row
    lcd.print(macAddress);
    // set cursor to first column, second row
    lcd.setCursor(0,1);
    // print message for second row
    lcd.print(ipAddress);
}

void removeHyphens(char* str) {
    char* src = str;
    char* dest = str;
    
    while (*src) {
        if (*src != '-') {
            *dest = *src;
            dest++;
        }
        src++;
    }
    
    *dest = '\0'; // Null-terminate the new string
}

void removePeriods(char* str) {
    char* src = str;
    char* dest = str;
    
    while (*src) {
        if (*src != '.') {
            *dest = *src;
            dest++;
        }
        src++;
    }
    
    *dest = '\0'; // Null-terminate the new string
}

void updateNetInfo(){
    macAddr = WiFi.macAddress();
    ipAddr = WiFi.localIP();
    if (ipAddr == "") {
        ipAddr == "None";
    }
    removeHyphens(macAddr);
    removePeriods(ipAddr);
    displayInfoOnLCD(macAddr, ipAddr);
}




// Main Functions



void setup() {
    Wire.begin();
    Serial.begin(baudRate);

    // Try to find the I2C address of the LCD screen
    Serial.println("Starting I2C Scanner...");
    lcdI2CScanner();
    if (lcdAddress == 0xFF) {
        Serial.println("LCD screen address not found.");
    } else {
        Serial.print("LCD screen address: 0x");
        if (lcdAddress < 16) {
            Serial.print("0");
        }
        Serial.println(lcdAddress, HEX);
        // Write known network info to LCD screen
        updateNetInfo();
    }


    // Connect to Wifi
    Serial.println("Connecting to Wifi")
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
    updateNetInfo();
}

void loop(){

    // Get Mac Address and IP address of ESP32 and display it on LCD screen
    updateNetInfo();
    sleep(1000);

}