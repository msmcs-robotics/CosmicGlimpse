#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>

const char* hostName = "msmcs-robotics.github.io"; // Change this to your server's hostname or domain
const char* apiPath = "/CosmicGlimpse/api/temperature"; // Change this to your API endpoint path

const int sensorPin = A0; // Assuming your temperature sensor is connected to analog pin A0
int temperature = 0;

// Enter your server details here
byte server[] = { 192, 168, 1, 100 }; // Change this to your server's IP address
int serverPort = 80; // Change this to your server's port

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

EthernetClient client;

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH Shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit FeatherWing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit FeatherWing Ethernet

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
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

void sendDataToAPI(int data) {
  if (client.connect(server, serverPort)) {
    // Create the POST request body
    String postData = "temperature=" + String(data);

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

  // Send data to API
  sendDataToAPI(temperature);
}
