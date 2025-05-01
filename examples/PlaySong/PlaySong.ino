#include <RobotixInnoBot.h>

RobotixInnoBot bot;

void setup() {
  bot.begin();

  bot.clearLCD();
  bot.drawString(25, 3, "Playing:", FONT_SMALL);
  bot.drawString(35, 5, "NOKIA", FONT_SMALL);
  bot.playSong(NOKIA);

  bot.clearLCD();
  bot.drawString(25, 3, "Playing:", FONT_SMALL);
  bot.drawString(17, 5, "PINKPANTHER", FONT_SMALL);
  bot.playSong(PINKPANTHER);

  bot.clearLCD();
  bot.drawString(25, 3, "Playing:", FONT_SMALL);
  bot.drawString(10, 5, "GAMEOFTHRONES", FONT_SMALL);
  bot.playSong(GAMEOFTHRONES);

  bot.clearLCD();
  bot.drawString(25, 3, "Playing:", FONT_SMALL);
  bot.drawString(10, 5, "HAPPYBIRTHDAY", FONT_SMALL);
  bot.playSong(HAPPYBIRTHDAY);

  bot.clearLCD();
  bot.drawString(25, 3, "Playing:", FONT_SMALL);
  bot.drawString(30, 5, "PACMAN", FONT_SMALL);
  bot.playSong(PACMAN);

  bot.clearLCD();
  bot.drawString(25, 3, "Playing:", FONT_SMALL);
  bot.drawString(30, 5, "TETRIS", FONT_SMALL);
  bot.playSong(TETRIS);

  bot.clearLCD();
  bot.drawString(25, 3, "Playing:", FONT_SMALL);
  bot.drawString(15, 5, "THEGODFATHER", FONT_SMALL);
  bot.playSong(THEGODFATHER);

  bot.clearLCD();
  bot.drawString(30, 3, "FINISH", FONT_SMALL);
}

// the loop function runs over and over again forever
void loop() {
}