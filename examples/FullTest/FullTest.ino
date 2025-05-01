//////////////////////////////////////////////////////////////////
// Test RJ Modules (LED,SRF,IR)

// Connection:
// Port 1: SRF
// Port 2: IR
// Port 3: LED
//////////////////////////////////////////////////////////////////

#include <RobotixInnoBot.h>
#include <Dynamixel2Arduino.h>

#define DXL_SERIAL Serial3
#define DEBUG_SERIAL Serial
#define DXL_DIR_PIN 53
const uint8_t DXL_ID = 5;
const float DXL_PROTOCOL_VERSION = 1.0;
Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);


RobotixInnoBot bot;
#define PORT1 bot.port_1
#define PORT2 bot.port_2
#define PORT3 bot.port_3



unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 500;

char temp[20];



void setup() {
  bot.begin();
  delay(1000);
  bot.clearLCD();
  bot.drawString(25, 10, "Playing:", FONT_SMALL);
  bot.drawString(35, 30, "NOKIA", FONT_SMALL);
  bot.playSong(NOKIA);
  bot.clearLCD();

  bot.rjIrInit(PORT2);
  bot.rjLedInit(PORT3);

  Serial.begin(115200);

  bot.servo_1.attach(PIN_SERVO_1);
  bot.servo_2.attach(PIN_SERVO_2);
  bot.servo_3.attach(PIN_SERVO_3);
  bot.servo_4.attach(PIN_SERVO_4);
  bot.servo_5.attach(PIN_SERVO_5);
  bot.servo_6.attach(PIN_SERVO_6);
  bot.servo_7.attach(PIN_SERVO_7);
  bot.servo_8.attach(PIN_SERVO_8);

  ////Motors //////////////////////////////////////////////////////////////
  bot.setMotorSpeed(bot.motor_1, 100);
  bot.setMotorSpeed(bot.motor_2, 100);
  bot.setMotorSpeed(bot.motor_3, 100);
  bot.setMotorSpeed(bot.motor_4, 100);
  delay(500);
  bot.setMotorSpeed(bot.motor_1, -200);
  bot.setMotorSpeed(bot.motor_2, -200);
  bot.setMotorSpeed(bot.motor_3, -200);
  bot.setMotorSpeed(bot.motor_4, -200);
  /////////////////////////////////////////////////////////////////////////

  ////Servos //////////////////////////////////////////////////////////////
  bot.setServoPositions(bot.servo_1, 180);
  bot.setServoPositions(bot.servo_2, 180);
  bot.setServoPositions(bot.servo_3, 180);
  bot.setServoPositions(bot.servo_4, 180);
  bot.setServoPositions(bot.servo_5, 180);
  bot.setServoPositions(bot.servo_6, 180);
  bot.setServoPositions(bot.servo_7, 180);
  bot.setServoPositions(bot.servo_8, 180);
  delay(500);
  bot.setServoPositions(bot.servo_1, 90);
  bot.setServoPositions(bot.servo_2, 90);
  bot.setServoPositions(bot.servo_3, 90);
  bot.setServoPositions(bot.servo_4, 90);
  bot.setServoPositions(bot.servo_5, 90);
  bot.setServoPositions(bot.servo_6, 90);
  bot.setServoPositions(bot.servo_7, 90);
  bot.setServoPositions(bot.servo_8, 90);
  delay(500);
  /////////////////////////////////////////////////////////////////////////

  ////Dynamixel ///////////////////////////////////////////////////////////
  dxl.begin(9600);
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  dxl.ping(DXL_ID);

  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_VELOCITY);
  dxl.torqueOn(DXL_ID);
  if (dxl.setGoalVelocity(DXL_ID, 128)) {
    delay(1000);
    DEBUG_SERIAL.print("Present Velocity : ");
    DEBUG_SERIAL.println(dxl.getPresentVelocity(DXL_ID));
    DEBUG_SERIAL.println();
  }
  /////////////////////////////////////////////////////////////////////////
}

// the loop function runs over and over again forever
void loop() {
  bot.rjLedWrite(PORT3, bot.rjIrRead(PORT2));
}
