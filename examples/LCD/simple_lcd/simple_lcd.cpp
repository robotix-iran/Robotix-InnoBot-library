#include <RobotixInnoBot.h>

RobotixInnoBot bot;

void setup() {
  bot.begin();

  bot.clearLCD();
  bot.drawString(8, 4, "Arduino Logo", FONT_SMALL);
  delay(2000);
  bot.showLogo(LOGO_ARDUINO);
  delay(2000);
  bot.clearLCD();
  bot.drawString(8, 4, "Robotix Logo", FONT_SMALL);
  delay(2000);
  bot.showLogo(LOGO_ROBOTIX);
  delay(2000);
  bot.clearLCD();
  bot.drawString(0, 1, "Small Font", FONT_SMALL);
  delay(2000);
  bot.drawString(0, 3, "Normal font", FONT_NORMAL);
  delay(2000);
  bot.drawString(0, 4, "LARGE", FONT_LARGE);
}

void loop() {
  // put your main code here, to run repeatedly:
}
