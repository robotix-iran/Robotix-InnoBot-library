#include <RobotixInnoBot.h>

#define NUMPIXELS 16

#define RGBM_PORT bot.port_5

RobotixInnoBot bot;

void setup() {
  bot.begin();
  bot.rjRgbInit(RGBM_PORT);
  Serial.begin(115200);
}

void loop() {
  for(int i=0; i<NUMPIXELS; i++) {
  bot.rjRgbWrite(RGBM_PORT,i,150, 150, 0);
  delay(500);
  }
}