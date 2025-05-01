#include <WiFiS3.h>

const char *ssid = "InnoBot Controller";  // WiFi Hotspot Name
const char *password = "12345678";     // WiFi Password

WiFiServer server(8888);  // Create WiFi Server on port 80

// LED and Button Pin Definitions
#define LED1_PIN 13
#define LED2_PIN 39
#define LED3_PIN 40
#define LED4_PIN 41

#define BTN1_PIN 33
#define BTN2_PIN 34
#define BTN3_PIN 35
#define BTN4_PIN 69

bool lastBtn1State = HIGH;
bool lastBtn2State = HIGH;
bool lastBtn3State = HIGH;
bool lastBtn4State = HIGH;

bool currentBtn1State = HIGH;
bool currentBtn2State = HIGH;
bool currentBtn3State = HIGH;
bool currentBtn4State = HIGH;

void setup() {
  Serial.begin(115200);

  // Set LED pins as output
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);

  // Set button pins as input with pull-up resistors
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(BTN3_PIN, INPUT_PULLUP);
  pinMode(BTN4_PIN, INPUT_PULLUP);

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

        // Control LEDs based on received commands
        if (receivedData == "BTN5PRESS") {
          digitalWrite(LED1_PIN, HIGH);
          Serial.println("LED1 ON");
        } else if (receivedData == "BTN5RELEA") {
          digitalWrite(LED1_PIN, LOW);
          Serial.println("LED1 OFF");
        } else if (receivedData == "BTN6PRESS") {
          digitalWrite(LED2_PIN, HIGH);
          Serial.println("LED2 ON");
        } else if (receivedData == "BTN6RELEA") {
          digitalWrite(LED2_PIN, LOW);
          Serial.println("LED2 OFF");
        } else if (receivedData == "BTN7PRESS") {
          digitalWrite(LED3_PIN, HIGH);
          Serial.println("LED2 ON");
        } else if (receivedData == "BTN7RELEA") {
          digitalWrite(LED3_PIN, LOW);
          Serial.println("LED2 OFF");
        } else if (receivedData == "BTN8PRESS") {
          digitalWrite(LED4_PIN, HIGH);
          Serial.println("LED2 ON");
        } else if (receivedData == "BTN8RELEA") {
          digitalWrite(LED4_PIN, LOW);
          Serial.println("LED2 OFF");
        }
      }

      currentBtn1State = digitalRead(BTN1_PIN);
      currentBtn2State = digitalRead(BTN2_PIN);
      currentBtn3State = digitalRead(BTN3_PIN);
      currentBtn4State = digitalRead(BTN4_PIN);

      if (currentBtn1State != lastBtn1State) {
        if (currentBtn1State == LOW) {
          client.println("LED1ON");
          Serial.println("LED1ON");
        } else {
          client.println("LED1OFF");
          Serial.println("LED1OFF");
        }
        lastBtn1State = currentBtn1State;  // Update last state
      }
      if (currentBtn2State != lastBtn2State) {
        if (currentBtn2State == LOW) {
          client.println("LED2ON");
          Serial.println("LED2ON");
        } else {
          client.println("LED2OFF");
          Serial.println("LED2OFF");
        }
        lastBtn2State = currentBtn2State;  // Update last state
      }
      if (currentBtn2State != lastBtn2State) {
        if (currentBtn2State == LOW) {
          client.println("LED2ON");
          Serial.println("LED2ON");
        } else {
          client.println("LED2OFF");
          Serial.println("LED2OFF");
        }
        lastBtn2State = currentBtn2State;  // Update last state
      }
      if (currentBtn3State != lastBtn3State) {
        if (currentBtn3State == LOW) {
          client.println("LED3ON");
          Serial.println("LED3ON");
        } else {
          client.println("LED3OFF");
          Serial.println("LED3OFF");
        }
        lastBtn3State = currentBtn3State;  // Update last state
      }
      if (currentBtn4State != lastBtn4State) {
        if (currentBtn4State == LOW) {
          client.println("LED4ON");
          Serial.println("LED4ON");
        } else {
          client.println("LED4OFF");
          Serial.println("LED4OFF");
        }
        lastBtn4State = currentBtn4State;  // Update last state
      }
    }

    client.stop();  // Close connection
    Serial.println("Client Disconnected!");
  } else {
    digitalWrite(LED1_PIN, HIGH);
    delay(100);
    digitalWrite(LED1_PIN, LOW);
    delay(100);
  }
}
