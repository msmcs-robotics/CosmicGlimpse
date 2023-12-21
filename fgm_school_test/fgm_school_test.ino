 /*
  Find Code at Examples>ThingSpeak>ArduinoEthernet>WriteMultipleFields
  Change Ethernet.init(10); to Ethernet.init(33); //  pin 33 for ESP32 Board !!!!!!!!!!!
  https://randomnerdtutorials.com/esp32-thingspeak-publish-arduino/
  Description: Writes values to fields 1,2,3 and 4  in a single ThingSpeak update every 20 seconds.
  
  Hardware: Arduino Ethernet
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
 - Requires the Ethernet library
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2020, The MathWorks, Inc.
*/

#include <Ethernet.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

byte mac[] = SECRET_MAC;

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);

EthernetClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
int number1 = 1;
int number2 = 2;//random(0,100);
int number3 = 3;//random(0,100);
int number4 = 4;//random(0,100);

float Bx; 
float By;
float Bz;
float fx;
float fy;
float fz;



//  magnetometer readings from Adafruit LIS3MDL and ESP32 Huzaah Feather

#include <Wire.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SensorLab.h>
#include <Adafruit_Sensor_Calibration.h>

//FreqCounter3ChanESP32AdafruitFeather3.ino
//Measure 3 frequencies and displays
// Include the ESP32 Arduino core library Eorks with ESP32 Adafruit Feather
#include <Arduino.h>

// Define the pins for frequency measurement
const int fxON = 26; // This is pin A1  By  Look at the adafruit web site pin out 
const int fyON = 25; // This is pin A0  Bx
const int fzON = 4; // This is pin A2  Bz
const int fxIN = 34; // fx and gate enable
const int fyIN = 39; // fy and gate enable
const int fzIN = 36; // fz and gate enable


// Variables to store the pulse counts
volatile long count1 = 0;
volatile long count2 = 0;
volatile long count3 = 0;

// Variables to store the last recorded time
unsigned long lastTime1 = 0;
unsigned long lastTime2 = 0;
unsigned long lastTime3 = 0;
unsigned long elapsedTime1;
unsigned long elapsedTime2;
unsigned long elapsedTime3;
unsigned long currentTime;

// Define the ISR (Interrupt Service Routine) functions for each pin
void IRAM_ATTR isr1() {
  count1++;
}

void IRAM_ATTR isr2() {
  count2++;
}

void IRAM_ATTR isr3() {
  count3++;
}








Adafruit_Sensor *mag = NULL, *gyro = NULL, *accel = NULL;
sensors_event_t mag_event, gyro_event, accel_event;


Adafruit_LIS3MDL lis3mdl;
#define LIS3MDL_CLK 13
#define LIS3MDL_MISO 12
#define LIS3MDL_MOSI 11
#define LIS3MDL_CS 10

// Hard-iron calibration settings
const float hard_iron[3] = {-49.46,  35.64,  -126.24};

// Soft-iron calibration settings
const float soft_iron[3][3] = {
  {  0.984,  0.005, -0.012 },
  {  0.005,  1.123, 0.041 },
  { -0.012,  0.041, 0.907  }
};

// Magnetic declination from magnetic-declination.com
// East is positive ( ), west is negative (-)
// mag_decl = ( /-)(deg   min/60   sec/3600)
// Set to 0 to get magnetic heading instead of geo heading
const float mag_decl = -1.233;



void setup(void) {
  Ethernet.init(33);  // Most Arduino Ethernet hardware !!!!!!!!!!!!!! pin 33 for ESP32 Board !!!!!!!!!!!
  Serial.begin(115200);  //Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
      
  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak







  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit LIS3MDL test!");
  
  // Try to initialize!
  if (! lis3mdl.begin_I2C()) {          // hardware I2C mode, can pass in address & alt Wire
  //if (! lis3mdl.begin_SPI(LIS3MDL_CS)) {  // hardware SPI mode
  //if (! lis3mdl.begin_SPI(LIS3MDL_CS, LIS3MDL_CLK, LIS3MDL_MISO, LIS3MDL_MOSI)) { // soft SPI
    Serial.println("Failed to find LIS3MDL chip");
    while (1) { delay(10); }
  }
  Serial.println("LIS3MDL Found!");

  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
  Serial.print("Performance mode set to: ");
  switch (lis3mdl.getPerformanceMode()) {
    case LIS3MDL_LOWPOWERMODE: Serial.println("Low"); break;
    case LIS3MDL_MEDIUMMODE: Serial.println("Medium"); break;
    case LIS3MDL_HIGHMODE: Serial.println("High"); break;
    case LIS3MDL_ULTRAHIGHMODE: Serial.println("Ultra-High"); break;
  }

  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  Serial.print("Operation mode set to: ");
  // Single shot mode will complete conversion and go into power down
  switch (lis3mdl.getOperationMode()) {
    case LIS3MDL_CONTINUOUSMODE: Serial.println("Continuous"); break;
    case LIS3MDL_SINGLEMODE: Serial.println("Single mode"); break;
    case LIS3MDL_POWERDOWNMODE: Serial.println("Power-down"); break;
  }

  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  // You can check the datarate by looking at the frequency of the DRDY pin
  Serial.print("Data rate set to: ");
  switch (lis3mdl.getDataRate()) {
    case LIS3MDL_DATARATE_0_625_HZ: Serial.println("0.625 Hz"); break;
    case LIS3MDL_DATARATE_1_25_HZ: Serial.println("1.25 Hz"); break;
    case LIS3MDL_DATARATE_2_5_HZ: Serial.println("2.5 Hz"); break;
    case LIS3MDL_DATARATE_5_HZ: Serial.println("5 Hz"); break;
    case LIS3MDL_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case LIS3MDL_DATARATE_20_HZ: Serial.println("20 Hz"); break;
    case LIS3MDL_DATARATE_40_HZ: Serial.println("40 Hz"); break;
    case LIS3MDL_DATARATE_80_HZ: Serial.println("80 Hz"); break;
    case LIS3MDL_DATARATE_155_HZ: Serial.println("155 Hz"); break;
    case LIS3MDL_DATARATE_300_HZ: Serial.println("300 Hz"); break;
    case LIS3MDL_DATARATE_560_HZ: Serial.println("560 Hz"); break;
    case LIS3MDL_DATARATE_1000_HZ: Serial.println("1000 Hz"); break;
  }
  
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
  Serial.print("Range set to: ");
  switch (lis3mdl.getRange()) {
    case LIS3MDL_RANGE_4_GAUSS: Serial.println("+-4 gauss"); break;
    case LIS3MDL_RANGE_8_GAUSS: Serial.println("+-8 gauss"); break;
    case LIS3MDL_RANGE_12_GAUSS: Serial.println("+-12 gauss"); break;
    case LIS3MDL_RANGE_16_GAUSS: Serial.println("+-16 gauss"); break;
  }

  lis3mdl.setIntThreshold(500);
  lis3mdl.configInterrupt(false, false, true, // enable z axis
                          true, // polarity
                          false, // don't latch
                          true); // enabled!



  // Set the pins as input
   
  pinMode(fxIN, INPUT);
  pinMode(fyIN, INPUT);
  pinMode(fzIN, INPUT);
  pinMode(fxON, OUTPUT);
  pinMode(fyON, OUTPUT);
  pinMode(fzON, OUTPUT);

  // Attach interrupts to the pins
  attachInterrupt(digitalPinToInterrupt(fxIN), isr1, RISING);
  attachInterrupt(digitalPinToInterrupt(fyIN), isr2, RISING);
  attachInterrupt(digitalPinToInterrupt(fzIN), isr3, RISING);
  // detachInterrupt(GPIOPin); Stops the interupt


}

void loop() {

GetDataApplyCalibration();
PrintMags();//
getfreaqs();
PrintFreqs();
//Out2MatLab2Fit();
OutToThingSpeak();

}
void PrintMags()
{
  Serial.print("[Bx,By, Bz] = [");
  Serial.print(Bx, 2);   // Bx
  Serial.print("\t");
  Serial.print(By, 2);   // By
  Serial.print("\t");
  Serial.print(Bz, 2);   //Bz
  Serial.print("] uTesla     ");
  Serial.println(" ");


}

void PrintFreqs(){
 // Print the frequencies to MATLAB[
 Serial.print("fx,fy,fz =  [");
  Serial.print(fx);
  Serial.print(",  ");
  Serial.print(fy);
  Serial.print(",  ");
  Serial.print(fz);
  Serial.println(" ]  HZ");  

}
void Out2MatLab2Fit(){
  Serial.println("");
  //Serial.println("B & f Out To MATLAB!");
  //Serial.flush(); 
  // Send data as CSV
  Serial.print(fx);Serial.print(",");Serial.print(fy);Serial.print(",");Serial.print(fz);Serial.print(",");
  Serial.print(Bx);Serial.print(",");Serial.print(By);Serial.print(",");Serial.print(Bz);
  Serial.println("");  // End the line with a newline character
  //delay(20); // Adjust the delay based on your needs
}


void GetDataApplyCalibration()
{
float hi_cal[3];
  float heading = 0;
 
 //Get  new sensor event, normalized to uTesla */
  sensors_event_t event; 
  lis3mdl.getEvent(&event);

  // Put raw magnetometer readings into an array
  float mag_data[] = {event.magnetic.x, event.magnetic.y, event.magnetic.z};
 
  // Apply hard-iron offsets
   for (uint8_t i = 0; i < 3; i++  ) 
   hi_cal[i] = mag_data[i] - hard_iron[i];


  // // Apply soft-iron scaling
  for (uint8_t i = 0; i < 3; i++ ) 
    mag_data[i] = (soft_iron[i][0] * hi_cal[0]) + (soft_iron[i][1] * hi_cal[1]) + (soft_iron[i][2] * hi_cal[2]);
  

  // Calculate angle for heading, assuming board is parallel to
  // the ground and  Y points toward heading.
  heading = -1 * (atan2(mag_data[0], mag_data[1]) * 180) / M_PI;

  // Apply magnetic declination to convert magnetic heading
  // to geographic heading
  heading  = heading + mag_decl;

  // Convert heading to 0..360 degrees
  if (heading < 0) {
    heading  = heading + 360;
  }

  // Print calibrated results  Comment these lines out when calibrating with MotionCal
  Bx=mag_data[0];
  By=mag_data[1];
  Bz=mag_data[2];

  // Serial.print("[Bx, By, Bz] = [");
  // Serial.print(Bx, 2);   // Bx
  // Serial.print("\t");
  // Serial.print(By, 2);   // By
  // Serial.print("\t");
  // Serial.print(Bz, 2);   //Bz
  // Serial.print("] uTesla     ");
  // Serial.println(" ");





  //Uncomment to Send data To MotionCal
  /*
//'Raw' values to match expectation of MotionCal
  
  Serial.print("Raw:");
  Serial.print(int(accel_event.acceleration.x*8192/9.8)); Serial.print(",");
  Serial.print(int(accel_event.acceleration.y*8192/9.8)); Serial.print(",");
  Serial.print(int(accel_event.acceleration.z*8192/9.8)); Serial.print(",");
  Serial.print(int(gyro_event.gyro.x*Adafruit_SensorLab::DEGREES_PER_RADIAN*16)); Serial.print(",");
  Serial.print(int(gyro_event.gyro.y*Adafruit_SensorLab::DEGREES_PER_RADIAN*16)); Serial.print(",");
  Serial.print(int(gyro_event.gyro.z*Adafruit_SensorLab::DEGREES_PER_RADIAN*16)); Serial.print(",");
  Serial.print(int(mag_data[0]*10)); Serial.print(",");
  Serial.print(int(mag_data[1]*10)); Serial.print(",");
  Serial.print(int(mag_data[2]*10)); Serial.println("");
  // unified data
  Serial.print("Uni:");
  Serial.print(accel_event.acceleration.x); Serial.print(",");
  Serial.print(accel_event.acceleration.y); Serial.print(",");
  Serial.print(accel_event.acceleration.z); Serial.print(",");
  Serial.print(gyro_event.gyro.x, 4); Serial.print(",");
  Serial.print(gyro_event.gyro.y, 4); Serial.print(",");
  Serial.print(gyro_event.gyro.z, 4); Serial.print(",");
  Serial.print(mag_event.magnetic.x); Serial.print(",");
  Serial.print(mag_event.magnetic.y); Serial.print(",");
  Serial.print(mag_event.magnetic.z); Serial.println("");

*/


}

void getfreaqs(){
  digitalWrite(fxON, LOW); 
  digitalWrite(fyON, LOW);
  digitalWrite(fzON, HIGH);
  delay(1000);
  fx=getfreaqs2();

  digitalWrite(fxON, LOW); 
  digitalWrite(fyON, HIGH);
  digitalWrite(fzON, LOW);
  delay(1000);
  fy=getfreaqs2();
 

  digitalWrite(fxON, HIGH); 
  digitalWrite(fyON, LOW);
  digitalWrite(fzON, LOW);
  delay(1000);
  fz=getfreaqs2();

 
}

float getfreaqs2()
{
float fx1;
float fy1;
float fz1;
 
  //Calculate frequency for fx
  currentTime = millis();
 

  elapsedTime1 = currentTime - lastTime1;
  fx1 = count1 / (elapsedTime1 / 1000.0);
  count1 = 0;
  lastTime1 = currentTime;
 
  elapsedTime2 = currentTime - lastTime2;
  fy1= count2 / (elapsedTime2 / 1000.0);
  count2 = 0;
  lastTime2 = currentTime;
 
  elapsedTime3 = currentTime - lastTime3;
   fz1 = count3 / (elapsedTime3 / 1000.0);
  count3 = 0;
  lastTime3 = currentTime;
 
  // Reset the pulse counts and update the last recorded time
  count1 = 0;
  count2 = 0;
  count3 = 0;
  lastTime1 = currentTime;
  lastTime2 = currentTime;
  lastTime3 = currentTime;

  // Delay for some time before the next measurement
  //delay(1000); // You can adjust the delay as needed
 float maxf=max(fx1,fy1);
//Serial.print("Max of fx and fy");Serial.println(maxf);
 maxf=max(maxf,fz1);
//Serial.print("Max of fx, fy, fz");Serial.println(maxf); 
return maxf;

}




void OutToThingSpeak(){
// set the fields with the values
  ThingSpeak.setField(1, number1);
  ThingSpeak.setField(2, number2);
  ThingSpeak.setField(3, number3);
  ThingSpeak.setField(4, number4);

  
  // write to the ThingSpeak channel 
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  // change the values
  number1++;
  if(number1 > 99){
    number1 = 0;
  }
  number1 = random(0,100);
  number2 = random(0,100);
  number3 = random(0,100);
  number4 = random(0,100);
  
  delay(20000); // Wait 20 seconds to update the channel again

}