#include <RobotixInnoBot.h>

#define LED_PORT bot.port_12

RobotixInnoBot bot;

void setup() {
  bot.begin();
  bot.rjLedInit(LED_PORT);
}

void loop() {
  bot.rjLedWrite(LED_PORT, HIGH);
  delay(500);
  bot.rjLedWrite(LED_PORT, LOW);
  delay(500);
}