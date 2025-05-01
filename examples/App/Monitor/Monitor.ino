#include <WiFiS3.h>
#include <RobotixInnoBot.h>

const char *ssid = "InnoBot Controller";  // WiFi Hotspot Name
const char *password = "12345678";     // WiFi Password

WiFiServer server(8888);  // Create WiFi Server on port 80

#define SRF_PORT bot.port_1
#define SW_PORT bot.port_2
#define IR_PORT bot.port_10
#define TEHU_PORT bot.port_7
#define TCS_PORT bot.port_7
#define VLX_PORT bot.port_7

unsigned long previousMillis = 0;  // Stores the last time the function ran
const long interval = 1000;        // Interval time in milliseconds

RobotixInnoBot bot;

void setup() {
  bot.begin();
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  bot.rjSrfInit(SRF_PORT);
  bot.rjIrInit(IR_PORT);
  //bot.rjTempHumInit(TEHU_PORT);
  //bot.rjTcsInit(TCS_PORT);
  bot.rjVlxInit(VLX_PORT);
  bot.rjSwInit(SW_PORT);

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

      }

      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;  // Update last execution time

        bot.addModToMonitor(SRF_PORT);
        bot.addModToMonitor(IR_PORT);
        //bot.addModToMonitor(TEHU_PORT);
        bot.addModToMonitor(TCS_PORT);
        bot.addModToMonitor(SW_PORT);
        bot.addModToMonitor(VLX_PORT);

        client.println(bot.getMonitorData());
        Serial.println(bot.getMonitorData());
      }
    }
    client.stop();  // Close connection
    Serial.println("Client Disconnected!");
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }
}

