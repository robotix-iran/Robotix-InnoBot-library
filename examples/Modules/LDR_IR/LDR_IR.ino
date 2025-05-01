#include <RobotixInnoBot.h>

#define LDR_PORT bot.port_11
#define IR_PORT bot.port_10

RobotixInnoBot bot;

void setup() {
  bot.begin();
  bot.rjIrInit(IR_PORT);
  bot.rjLdrInit(LDR_PORT);
  Serial.begin(115200);
}

void loop() {
  Serial.print("IR: ");
  Serial.print(bot.rjIrRead(IR_PORT));
  Serial.print("  LDR: ");
  Serial.println(bot.rjLdrRead(LDR_PORT));
  delay(500);
}