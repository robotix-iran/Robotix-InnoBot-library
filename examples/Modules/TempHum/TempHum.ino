#include <RobotixInnoBot.h>

#define TEMPH_PORT bot.port_7

RobotixInnoBot bot;

void setup() {
  bot.begin();
  bot.rjTempHumInit(TEMPH_PORT);
  Serial.begin(115200);
}

void loop() {
  Serial.print("Temp: ");
  Serial.print(bot.rjTempRead(TEMPH_PORT));
  Serial.print("°C");
  Serial.print("  Humidity: ");
  Serial.print(bot.rjHumRead(TEMPH_PORT));
  Serial.println("%");
  delay(500);
}