#include <RobotixInnoBot.h>

RobotixInnoBot bot;

#define SRF_1 bot.port_1
#define SRF_2 bot.port_2
#define SRF_3 bot.port_3

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 100;

char temp1[20];
char temp2[20];
char temp3[20];

#define Distance 20
#define speed 150

void setup() {
  bot.begin();
  bot.showLogo(LOGO_ROBOTIX);
  delay(1000);
  bot.clearLCD();

  bot.rjSrfInit(SRF_1);
  bot.rjSrfInit(SRF_2);
  bot.rjSrfInit(SRF_3);

  Serial.begin(115200);
}

void loop() {


  currentMillis = millis();
  if (currentMillis - startMillis >= period) {

    float distance1 = bot.rjSrfRead(SRF_1);
    float distance2 = bot.rjSrfRead(SRF_2);
    float distance3 = bot.rjSrfRead(SRF_3);

    sprintf(temp1, "distance1: %3.1f \r\n distance2: %3.1f \r\n distance3: %3.1f \r\n", distance1, distance2, distance3);

    if (distance1 < Distance) {
      bot.setMotorSpeed(bot.motor_2, -speed);
      bot.setMotorSpeed(bot.motor_3, 0);
    } else if (distance3 < Distance) {
      bot.setMotorSpeed(bot.motor_2, 0);
      bot.setMotorSpeed(bot.motor_3, speed);
    } else if (distance2 < Distance) {
      bot.setMotorSpeed(bot.motor_2, 0);
      bot.setMotorSpeed(bot.motor_3, 0);
    } else {
      bot.setMotorSpeed(bot.motor_2, -speed);
      bot.setMotorSpeed(bot.motor_3, speed);
    }

    Serial.print(temp1);
    startMillis = currentMillis;
  }
}
