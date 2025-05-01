/*
    This file is part of the RobotixInnoBot library.

*/

#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

#include "Arduino.h"

#define ON 1
#define OFF 0

#define IOPORT_CFG_PWM (IOPORT_CFG_PERIPHERAL_PIN | IOPORT_PERIPHERAL_GPT1)

#define IMU_ADDRESS 0x69
#define IR_ARRAY_ADDRESS 0x08

// SONG////////////////////////////////
#define NOKIA 0
#define PINKPANTHER 1
#define GAMEOFTHRONES 2
#define HAPPYBIRTHDAY 3
#define PACMAN 4
#define TETRIS 5
#define THEGODFATHER 6
///////////////////////////////////////



// MOTOR///////////////////////////////
#define LEFT 0
#define RIGHT 1

#define FORWARD 0
#define BACKWARD 1
///////////////////////////////////////



// LOGO////////////////////////////////
#define LOGO_ROBOTIX 0
#define LOGO_ARDUINO 1
///////////////////////////////////////



// PINS////////////////////////////////

////////////////////LCD
#define PIN_LCD_MOSI 28
#define PIN_LCD_SCK 29
#define PIN_LCD_RESET 30
#define PIN_LCD_CS 31
#define PIN_LCD_BKL 32
///////////////////Button
#define PIN_BTN_1 33
#define PIN_BTN_2 34
#define PIN_BTN_3 35
#define PIN_BTN_4 69
///////////////////MPU
#define PIN_IMU_SCK 37
#define PIN_IMU_SDA 38
///////////////////LED
#define PIN_LED_1 LED_BUILTIN
#define PIN_LED_2 39
#define PIN_LED_3 40
#define PIN_LED_4 41

#define LED_1 PIN_LED_1
#define LED_2 PIN_LED_2
#define LED_3 PIN_LED_3
#define LED_4 PIN_LED_4
////////////////////BUZZ
#define PIN_BUZZER 68
///////////////////MOTOR
#define PIN_M1_IN1 66
#define PIN_M1_IN2 67
#define PIN_M2_IN1 65
#define PIN_M2_IN2 64
#define PIN_M3_IN1 62
#define PIN_M3_IN2 63
#define PIN_M4_IN1 60
#define PIN_M4_IN2 61
////////////////////IO
#define PIN_PORT1_A 42
#define PIN_PORT1_B 43
#define PIN_PORT2_A 44
#define PIN_PORT2_B 45
#define PIN_PORT3_A 46
#define PIN_PORT3_B 47
////////////////////SERVO
#define PIN_SERVO_1 49
#define PIN_SERVO_2 50
#define PIN_SERVO_3 51
#define PIN_SERVO_4 52

#define PIN_SERVO_5 57
#define PIN_SERVO_6 48
#define PIN_SERVO_7 56
#define PIN_SERVO_8 59
////////////////////Dynamixel
#define PIN_MCU_RX_Dynamixel 54
#define PIN_MCU_TX_Dynamixel 55
#define PIN_Dynamixel_TX_EN 53

////////////////////RJ-SHIELD
#define PIN_PORT4_B 0
#define PIN_PORT4_A 1
#define PIN_PORT5_B 2
#define PIN_PORT5_A 3
#define PIN_PORT6_B 4
#define PIN_PORT6_A 5
#define PIN_PORT7_B 18
#define PIN_PORT7_A 19
#define PIN_PORT8_B 18
#define PIN_PORT8_A 19
#define PIN_PORT9_B 18
#define PIN_PORT9_A 19
#define PIN_PORT10_B 14
#define PIN_PORT10_A 15
#define PIN_PORT11_B 7
#define PIN_PORT11_A 16
#define PIN_PORT12_B 6
#define PIN_PORT12_A 17

///////////////////////////////////////



#endif
