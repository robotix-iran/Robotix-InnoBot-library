#include <RobotixInnoBot.h>

#define IRA_PORT bot.port_7

RobotixInnoBot mega;
uint8_t irRegister[4];
 
#define Speed 150

#define SR9  ((irRegister[0] >> 4) & 1)
#define SR8  ((irRegister[0] >> 3) & 1)
#define SR7  ((irRegister[0] >> 2) & 1)
#define SR6  ((irRegister[0] >> 1) & 1)
#define SR5  ((irRegister[0] >> 0) & 1)
#define SR4  ((irRegister[1] >> 6) & 1)
#define SR3  ((irRegister[1] >> 5) & 1)
#define SR2  ((irRegister[1] >> 4) & 1)
#define SR1  ((irRegister[1] >> 3) & 1)
#define SM   ((irRegister[1] >> 2) & 1)
#define SL1  ((irRegister[1] >> 1) & 1)
#define SL2  ((irRegister[1] >> 0) & 1)
#define SL3  ((irRegister[2] >> 6) & 1)
#define SL4  ((irRegister[2] >> 5) & 1)
#define SL5  ((irRegister[2] >> 4) & 1)
#define SL6  ((irRegister[2] >> 3) & 1)
#define SL7  ((irRegister[2] >> 2) & 1)
#define SL8  ((irRegister[2] >> 1) & 1)
#define SL9  ((irRegister[2] >> 0) & 1)

void setup() {
  bot.begin();
  bot.rjIrArrayInit(IRA_PORT);
  Serial.begin(115200);
}

void loop() {
  bot.rjIrArrayRead(IRA_PORT, irRegister);
  if(SM)  bot.speed(Speed, Speed);
  else if(SR1)  bot.speed(Speed, Speed/2);
  else if(SR2)  bot.speed(Speed, 0);
  else if(SR3)  bot.speed(Speed, -Speed/2);
  else if(SR4)  bot.speed(Speed, -Speed);
  else if(SR5)  bot.speed(Speed, -Speed);
  else if(SR6)  bot.speed(Speed, -Speed);
  else if(SR7)  bot.speed(Speed, -Speed);
  else if(SR8)  bot.speed(Speed, -Speed);
  else if(SR9)  bot.speed(Speed, -Speed);
  else if(SL1)  bot.speed(Speed/2, Speed);
  else if(SL2)  bot.speed(0, Speed);
  else if(SL3)  bot.speed(-Speed/2, Speed);
  else if(SL4)  bot.speed(-Speed, Speed);
  else if(SL5)  bot.speed(-Speed, Speed);
  else if(SL6)  bot.speed(-Speed, Speed);
  else if(SL7)  bot.speed(-Speed, Speed);
  else if(SL8)  bot.speed(-Speed, Speed);
  else if(SL9)  bot.speed(-Speed, Speed);
  else bot.speed(Speed, Speed);

  // print_irRegister();
  // Serial.println("sm :");
  // Serial.println(SM);
  // delay(500);
}

void print_irRegister() {
  Serial.println("IR Register Data (Binary):");
  for (int i = 0; i < 4; i++) {
    Serial.print("Register ");
    Serial.print(i);
    Serial.print(": ");
    for (int bit = 7; bit >= 0; bit--) {  // Print each bit manually
      Serial.print((irRegister[i] >> bit) & 1);
    }
    Serial.println();
  }
  Serial.println();
}