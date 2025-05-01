#include <RobotixInnoBot.h>

#define IRA_PORT bot.port_7

RobotixInnoBot bot;
uint8_t irRegister[4];

void setup() {
  bot.begin();
  bot.rjIrArrayInit(IRA_PORT);
  Serial.begin(115200);
}

void loop() {
  bot.rjIrArrayRead(IRA_PORT, irRegister);
  print_irRegister();
  delay(500);
}

void print_irRegister() {
  Serial.println("IR Register Data (Binary):");
  for (int i = 0; i < 4; i++) {
    Serial.print("Register ");
    Serial.print(i);
    Serial.print(": ");
    for (int bit = 7; bit >= 0; bit--) {  // Print each bit manually
      Serial.print((irRegister[i] >> bit) & 1);
    }
    Serial.println();
  }
  Serial.println();
}