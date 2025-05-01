#include <RobotixInnoBot.h>

#define TCS_PORT bot.port_7

RobotixInnoBot bot;

void setup() {
  bot.begin();
  bot.rjTcsInit(TCS_PORT);
  Serial.begin(115200);
}

void loop() {
  uint16_t r, g, b, c, colorTemp, lux;

  bot.rjTcsRead(TCS_PORT, r, g, b, c, colorTemp, lux);

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
 
  delay(500);
}