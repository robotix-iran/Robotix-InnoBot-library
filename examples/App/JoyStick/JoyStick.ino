#include <WiFiS3.h>
#include <RobotixInnoBot.h>

const char *ssid = "InnoBot Controller";  // WiFi Hotspot Name
const char *password = "12345678";     // WiFi Password

WiFiServer server(8888);  // Create WiFi Server on port 80

RobotixInnoBot bot;

void setup() {
  bot.begin();
  Serial.begin(115200);

  // Start WiFi Hotspot
  Serial.print("Starting AP...");
  WiFi.beginAP(ssid, password);

  while (WiFi.status() != WL_AP_LISTENING) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Hotspot Started!");
  Serial.print("Connect to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();  // Start server
}

void loop() {
  WiFiClient client = server.available();  // Check for a client connection

  if (client) {
    Serial.println("Client Connected!");

    while (client.connected()) {
      if (client.available()) {
        String receivedData = client.readStringUntil('\n');  // Read data from client
        receivedData.trim();                                 // Remove spaces/newlines

        Serial.print("Received: ");
        Serial.println(receivedData);
        parseSpeed(receivedData);
      }
    }
    client.stop();  // Close connection
    Serial.println("Client Disconnected!");
  } else {
    digitalWrite(PIN_LED_1, HIGH);
    delay(100);
    digitalWrite(PIN_LED_1, LOW);
    delay(100);
  }
}

void parseSpeed(String command) {
  if (command.startsWith("SPEED:")) {
    command = command.substring(6);  // Remove "SPEED:"

    int commaIndex = command.indexOf(',');
    if (commaIndex > 0) {
      int rightSpeed = command.substring(0, commaIndex).toInt();
      int leftSpeed = command.substring(commaIndex + 1).toInt();
      bot.speed(leftSpeed*-1, rightSpeed);
    }
  }
}
