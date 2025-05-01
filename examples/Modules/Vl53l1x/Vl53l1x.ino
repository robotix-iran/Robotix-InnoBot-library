#include <RobotixInnoBot.h>

#define VLX_PORT bot.port_7

RobotixInnoBot bot;

void setup() {
  bot.begin();
  bot.rjVlxInit(VLX_PORT);
  Serial.begin(115200);
}

void loop() {
  Serial.print("Distance: ");
  Serial.print(bot.rjVlxRead(VLX_PORT));
  Serial.println("cm");
  delay(500);
}