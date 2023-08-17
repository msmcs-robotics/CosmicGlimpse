/*
Connect To Ethernet - setup();
Collect Data From FGM - shiftPin(); collectData();
Convert Data To Magnetic Field - freqToMag();
Get A Timestamp - loop();
Send Data To API - sendDataToAPI();
*/


#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <FreqCount.h> // https://github.com/PaulStoffregen/FreqCount/releases/tag/1.3

// Global Variables

const int baudRate = 9600; // Serial Baud Rate
const int gateInterval = 1000; // The time in milliseconds for each measurement. - https://www.pjrc.com/teensy/td_libs_FreqCount.html#:~:text=FreqCount.begin,a%20scale%20factor
unsigned long lastDataSentTime = 0;

const char* hostName = "msmcs-robotics.github.io"; // Change this to your server's hostname or domain
const char* apiPath = "/CosmicGlimpse/api/temperature"; // Change this to your API endpoint path

const int fgmPowerPinX = 4;
const int fgmPowerPinY = 5;
const int fgmPowerPinZ = 6;

const int fgmDataPinX = 3;
const int fgmDataPinY = 9;
const int fgmDataPinZ = 10;

const int sensorPin = A0; // Assuming your temperature sensor is connected to analog pin A0
int temperature = 0;

// Enter your server details here
byte server[] = { 192, 168, 1, 100 }; // Change this to your server's IP address
int serverPort = 80; // Change this to your server's port

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

EthernetClient client;

/* 1. Send power to flux-gate-magnatometer power pins from arduino pins one at a time
2. Read from flux-gate-magnatometer data pins to arduino one at a time
3. Interpret data as a frequency
4. Store resulting frequency from a specified time interval in a single variable
*/


// Produce a function to map X, Y, and Z frequencies into a numerical representation of a magnetic field
int freqToMag() {
    int bigB = 0; // Set magnetic field to 0 to start 

    // Pass frequencies from X, Y, and Z axi
    // Use the equation: B = a + bf + cf^2

    return bigB;
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
    if (FreqCount.available()) {
        unsigned long count = FreqCount.read();
        Serial.println(count);
    }
};

void sendDataToAPI(int data) {
  if (client.connect(server, serverPort)) {
    // Create the POST request body
    String postData = "temperature=" + String(data);

    // Get the current timestamp in seconds since the Arduino started
    unsigned long currentTime = millis() / 1000;
    postData += "&timestamp=" + String(currentTime);

    // Make the HTTP POST request
    client.println("POST " + String(apiPath) + " HTTP/1.1");
    client.print("Host: ");
    client.println(String(hostName));
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.println(postData);
    client.println();

    delay(1000); // Wait for a second before sending the next request

    client.stop(); // Close the connection
  }
  Serial.print("Sent ");
  Serial.print(data);
  Serial.print(" to ");
  Serial.println(String(hostName) + String(apiPath));
}

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(baudRate);
  FreqCount.begin(gateInterval);

  pinMode(fgmDataPin1, INPUT);
  pinMode(fgmDataPin2, INPUT);
  pinMode(fgmDataPin3, INPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // no point in carrying on, so do nothing forevermore:
    while (true) {
      delay(1);
    }
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  switch (Ethernet.maintain()) {
    case 1:
      //renewed fail
      Serial.println("Error: renewed fail");
      break;

    case 2:
      //renewed success
      Serial.println("Renewed success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    case 3:
      //rebind fail
      Serial.println("Error: rebind fail");
      break;

    case 4:
      //rebind success
      Serial.println("Rebind success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    default:
      //nothing happened
      break;
  }

  // Read temperature from the sensor (you might need to use the correct method for your sensor)
  temperature = 10;

  // Timestamp
  // Check if enough time has passed since the last data was sent
  if (millis() - lastDataSentTime >= 1000) { // Adjust the time interval as needed
    // Send data to API
    sendDataToAPI(temperature);

    // Update the last data sent time
    lastDataSentTime = millis();
  }
}
