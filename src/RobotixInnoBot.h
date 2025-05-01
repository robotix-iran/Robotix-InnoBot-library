/*
    This file is part of the RobotixInnoBot library.
    Writed by Mahdi Hamzeh mahdi2001h.ir
*/

#ifndef __ROBOTIX_INNOBOT_H__
#define __ROBOTIX_INNOBOT_H__

#include "Arduino.h"
#include "Wire.h"
#include "string"
#include "definitions.h"
#include "SongNotes.h"
#include "Logo.h"
#include "ste-2007.h"
#include "Servo.h"
#include "FspTimer.h"
#include "Adafruit_ICM20948.h"
#include "Adafruit_TCS34725.h"
#include "rgb.h"
#include "DHT20.h"
#include "VL53L1X.h"

class RobotixInnoBot
{
private:
  void playSongPri(const int *melody, int notes);

  enum ModulesTypes
  {
    MOD_NONE,     // A
    MOD_IR,       // B
    MOD_LED,      // C
    MOD_LDR,      // D
    MOD_TE_HU,    // E
    MOD_SWITCH,   // F
    MOD_SRF,      // G
    MOD_IR_ARRAY, // H
    MOD_VLX,      // I
    MOD_TCS,      // J
    MOD_RGB       // K
  };

  struct rj_port
  {
    uint8_t num;
    ModulesTypes type;
    uint8_t pin_A;
    uint8_t pin_B;
  };

  struct rjPortMonitor
  {
    ModulesTypes type;
    unsigned int value;
  };

  struct
  {
    rjPortMonitor port_1;
    rjPortMonitor port_2;
    rjPortMonitor port_3;
    rjPortMonitor port_4;
    rjPortMonitor port_5;
    rjPortMonitor port_6;
    rjPortMonitor port_7;
    rjPortMonitor port_8;
    rjPortMonitor port_9;
    rjPortMonitor port_10;
    rjPortMonitor port_11;
    rjPortMonitor port_12;
  } portMonitor;

  rjPortMonitor* monitor_ports[12];

    


  struct motor_port
  {
    uint8_t IN_1;
    uint8_t IN_2;

    bool operator==(const motor_port &other) const
    {
      return (IN_1 == other.IN_1) && (IN_2 == other.IN_2);
    }
  };

  char getMappedChar(int num)
  {
    if (num >= 0 && num <= 12)
    {
      return 'A' + num; // Maps 0 -> 'A', 1 -> 'B', ..., 12 -> 'M'
    }
    return '-'; // Default for out-of-range values
  }

public:
  Adafruit_ICM20948 imu;
  DHT20 DHT;
  VL53L1X vl53l1;
  Adafruit_TCS34725 tcs;
  Adafruit_NeoPixel pixels;

  rj_port port_1{1, MOD_NONE, PIN_PORT1_A, PIN_PORT1_B};
  rj_port port_2{2, MOD_NONE, PIN_PORT2_A, PIN_PORT2_B};
  rj_port port_3{3, MOD_NONE, PIN_PORT3_A, PIN_PORT3_B};
  rj_port port_4{4, MOD_NONE, PIN_PORT4_A, PIN_PORT4_B};
  rj_port port_5{5, MOD_NONE, PIN_PORT5_A, PIN_PORT5_B};
  rj_port port_6{6, MOD_NONE, PIN_PORT6_A, PIN_PORT6_B};
  rj_port port_7{7, MOD_NONE, PIN_PORT7_A, PIN_PORT7_B};
  rj_port port_8{8, MOD_NONE, PIN_PORT8_A, PIN_PORT8_B};
  rj_port port_9{9, MOD_NONE, PIN_PORT9_A, PIN_PORT9_B};
  rj_port port_10{10, MOD_NONE, PIN_PORT10_A, PIN_PORT10_B};
  rj_port port_11{11, MOD_NONE, PIN_PORT11_A, PIN_PORT11_B};
  rj_port port_12{12, MOD_NONE, PIN_PORT12_A, PIN_PORT12_B};

  motor_port motor_1{PIN_M1_IN1, PIN_M1_IN2};
  motor_port motor_2{PIN_M2_IN1, PIN_M2_IN2};
  motor_port motor_3{PIN_M3_IN1, PIN_M3_IN2};
  motor_port motor_4{PIN_M4_IN1, PIN_M4_IN2};

  Servo servo_1;
  Servo servo_2;
  Servo servo_3;
  Servo servo_4;
  Servo servo_5;
  Servo servo_6;
  Servo servo_7;
  Servo servo_8;

  char monitorBuffer[100];

  RobotixInnoBot();

  int begin();

  void setLED(uint8_t led, uint8_t state);
  uint8_t readBTN(uint8_t btn);

  void playSong(uint8_t song);
  void buzzerTone(uint32_t tone);
  void buzzerNoTone();

  void setMotorSpeed(motor_port motor, int32_t speed);

  void speed(int32_t left, int32_t right);
  void rotate(uint8_t direction, int32_t speed);
  void move(uint8_t direction, int32_t speed);

  void brake();

  float getTemp();
  void getOrientation(float &roll, float &pitch, float &yaw);
  void getAccelerations(float &ax, float &ay, float &az);
  void getGyros(float &gx, float &gy, float &gz);
  void getMagno(float &mx, float &my, float &mz);
  void getIMU(float &ax, float &ay, float &az, float &gx, float &gy, float &gz);

  void setServoPositions(Servo servo, uint8_t position);

  void showLogo(uint8_t logo);

  void drawString(uint8_t x, uint8_t y,  char *s, uint8_t size);
  void clearRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  void clearLCD();

  // SRF-HC05
  uint8_t rjSrfInit(rj_port &port);
  float rjSrfRead(rj_port port);

  // VLX
  uint8_t rjVlxInit(rj_port &port);
  int rjVlxRead(rj_port port);

  // IR
  uint8_t rjIrInit(rj_port &port);
  int rjIrRead(rj_port port);

  // Switch
  uint8_t rjSwInit(rj_port &port);
  uint8_t rjSwRead(rj_port port);

  // LED
  uint8_t rjLedInit(rj_port &port);
  void rjLedWrite(rj_port port, uint8_t state);

  // Temp
  uint8_t rjTempHumInit(rj_port &port);
  float rjHumRead(rj_port port);
  float rjTempRead(rj_port port);

  // LDR
  uint8_t rjLdrInit(rj_port &port);
  int rjLdrRead(rj_port port);

  // RGB
  uint8_t rjRgbInit(rj_port &port);
  void rjRgbWrite(rj_port port, uint8_t pixel, uint8_t r, uint8_t g, uint8_t b);
  void rjRgbWrite(rj_port port, uint8_t pixel, uint32_t c);
  void rjRgbClear(rj_port port);

  uint8_t rjIrArrayInit(rj_port &port);
  void rjIrArrayRead(rj_port port, uint8_t *sensors);

  uint8_t rjTcsInit(rj_port &port);
  void rjTcsRead(rj_port port, uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c, uint16_t &colorTemp, uint16_t &lux);

  uint8_t addModToMonitor(rj_port port);
  const char *getMonitorData();
};

#endif