#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>

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
  // Your setup code remains the same

  // ...
}

void loop() {
  switch (Ethernet.maintain()) {
    // Your loop code remains the same
  }

  // Read temperature from the sensor (you might need to use the correct method for your sensor)
  temperature = analogRead(sensorPin);

  // Send data to API
  if (client.connect(server, serverPort)) {
    // Create the POST request body
    String postData = "temperature=" + String(temperature);

    // Make the HTTP POST request
    client.println("POST /api/temperature HTTP/1.1");
    client.println("Host: your-server.com"); // Change this to your server's hostname or IP
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.println(postData);
    client.println();

    delay(1000); // Wait for a second before sending the next request

    client.stop(); // Close the connection
  }
}
