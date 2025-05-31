//////////////////////////////////////////////////////////////////
// Test RJ Modules (SRF,SWITCH)

// Connection:
// Port 1: SRF
// Port 2: SWITCH
//////////////////////////////////////////////////////////////////

#include <RobotixInnoBot.h>

RobotixInnoBot bot;

#define SRF_PORT bot.port_1
#define SW_PORT bot.port_2

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 1000;

char temp[20];

void setup()
{
  bot.begin();
  bot.showLogo(LOGO_ROBOTIX);
  bot.rjSrfInit(SRF_PORT);
  bot.rjSwRead(SW_PORT);



  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop()
{

  bot.setLED(LED_1, bot.rjSwRead(SW_PORT));

  currentMillis = millis();
  if (currentMillis - startMillis >= period)
  {
    float distance = bot.rjSrfRead(SRF_PORT);
    sprintf(temp, "distance: %3.1f ",distance);
    bot.clearLCD();
    bot.drawString(0,5,temp, FONT_SMALL);
    Serial.print(temp);
    startMillis = currentMillis;
  }
}