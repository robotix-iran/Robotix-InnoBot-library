/*
    This file is part of the RobotixInnoBot library.
    Writed by Mahdi Hamzeh mahdi2001h.ir
*/

#include "RobotixInnoBot.h"

RobotixInnoBot::RobotixInnoBot()
{

  monitor_ports[0] = &portMonitor.port_1;
  monitor_ports[1] = &portMonitor.port_2;
  monitor_ports[2] = &portMonitor.port_3;
  monitor_ports[3] = &portMonitor.port_4;
  monitor_ports[4] = &portMonitor.port_5;
  monitor_ports[5] = &portMonitor.port_6;
  monitor_ports[6] = &portMonitor.port_7;
  monitor_ports[7] = &portMonitor.port_8;
  monitor_ports[8] = &portMonitor.port_9;
  monitor_ports[9] = &portMonitor.port_10;
  monitor_ports[10] = &portMonitor.port_11;
  monitor_ports[11] = &portMonitor.port_12;
}

FspTimer timer0;
FspTimer timer5;

int RobotixInnoBot::begin()
{
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);

  timer0.begin_pwm(GPT_TIMER, 0, CHANNEL_B);
  timer5.begin_pwm(GPT_TIMER, 5, CHANNEL_AB);

  R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[PIN_M1_IN1].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
  R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[PIN_M1_IN2].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);

  R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[PIN_M3_IN1].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
  R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[PIN_M3_IN2].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);

  R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[PIN_M4_IN1].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
  R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[PIN_M4_IN2].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);

  servo_1.attach(PIN_SERVO_1);
  servo_2.attach(PIN_SERVO_2);
  servo_3.attach(PIN_SERVO_3);
  servo_4.attach(PIN_SERVO_4);
  servo_5.attach(PIN_SERVO_5);
  servo_6.attach(PIN_SERVO_6);
  servo_7.attach(PIN_SERVO_7);
  servo_8.attach(PIN_SERVO_8);

  pinMode(PIN_BTN_1, INPUT);
  pinMode(PIN_BTN_2, INPUT);
  pinMode(PIN_BTN_3, INPUT);
  pinMode(PIN_BTN_4, INPUT);

  STE2007Init(PIN_LCD_RESET, 0xFF, PIN_LCD_CS, PIN_LCD_SCK, PIN_LCD_MOSI, 0, 0);
  STE2007Fill(0);

  int status = imu.begin_I2C(IMU_ADDRESS, &Wire1, 0);

  showLogo(LOGO_ROBOTIX);
  analogWrite(PIN_LCD_BKL, 50);
  digitalWrite(PIN_LED_1, ON);
  delay(50);
  digitalWrite(PIN_LED_1, LOW);
  delay(20);
  analogWrite(PIN_LCD_BKL, 100);
  digitalWrite(PIN_LED_2, ON);
  delay(50);
  digitalWrite(PIN_LED_2, LOW);
  delay(20);
  analogWrite(PIN_LCD_BKL, 150);
  digitalWrite(PIN_LED_3, ON);
  delay(50);
  digitalWrite(PIN_LED_3, LOW);
  delay(20);
  analogWrite(PIN_LCD_BKL, 200);
  digitalWrite(PIN_LED_4, ON);
  delay(50);
  digitalWrite(PIN_LED_4, LOW);
  delay(20);
  analogWrite(PIN_LCD_BKL, 255);
  return 0;
}

void RobotixInnoBot::setLED(uint8_t led, uint8_t state)
{
  digitalWrite(led, state);
}

uint8_t RobotixInnoBot::readBTN(uint8_t btn)
{
  return digitalRead(btn);
}

void RobotixInnoBot::playSongPri(const int *melody, int notes)
{

  // this calculates the duration of a whole note in ms
  int wholenote = (60000 * 4) / 180;

  int divider = 0, noteDuration = 0;

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2)
  {
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0)
    {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    }
    else if (divider < 0)
    {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(PIN_BUZZER, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(PIN_BUZZER);
  }
}

void RobotixInnoBot::playSong(uint8_t song)
{
  int notes = 0;

  switch (song)
  {
  case NOKIA:
    notes = sizeof(nokia_ringtones) / sizeof(nokia_ringtones[0]) / 2;
    playSongPri(nokia_ringtones, notes);
    break;
  case PINKPANTHER:
    notes = sizeof(pink_panther) / sizeof(pink_panther[0]) / 2;
    playSongPri(pink_panther, notes);
    break;
  case GAMEOFTHRONES:
    notes = sizeof(game_of_thrones) / sizeof(game_of_thrones[0]) / 2;
    playSongPri(game_of_thrones, notes);
    break;
  case HAPPYBIRTHDAY:
    notes = sizeof(happy_birthday) / sizeof(happy_birthday[0]) / 2;
    playSongPri(happy_birthday, notes);
    break;
  case PACMAN:
    notes = sizeof(pacman) / sizeof(pacman[0]) / 2;
    playSongPri(pacman, notes);
    break;
  case TETRIS:
    notes = sizeof(tetris) / sizeof(tetris[0]) / 2;
    playSongPri(tetris, notes);
    break;
  case THEGODFATHER:
    notes = sizeof(godfather) / sizeof(godfather[0]) / 2;
    playSongPri(godfather, notes);
    break;
  default:
    break;
  }
}

void RobotixInnoBot::buzzerTone(uint32_t note)
{
  tone(PIN_BUZZER, note);
}

void RobotixInnoBot::buzzerNoTone()
{
  noTone(PIN_BUZZER);
}

void RobotixInnoBot::setMotorSpeed(motor_port motor, int32_t speed)
{
  if (motor == motor_1 || motor == motor_3)
  {
    if (speed > 0)
    {
      R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[motor.IN_1].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
      R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[motor.IN_2].pin, IOPORT_CFG_PWM);

      timer5.set_duty_cycle((24489 * speed) / 256.0f, (motor == motor_1) ? CHANNEL_A : CHANNEL_B);
    }
    else if (speed < 0)
    {
      R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[motor.IN_1].pin, IOPORT_CFG_PWM);
      R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[motor.IN_2].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);

      timer5.set_duty_cycle((24489 * abs(speed)) / 256.0f, (motor == motor_1) ? CHANNEL_A : CHANNEL_B);
    }
    else
    {
      R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[motor.IN_1].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
      R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[motor.IN_2].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
      digitalWrite(motor.IN_1, 1);
      digitalWrite(motor.IN_2, 1);
    }
  }
  else if (motor == motor_2)
  {
    if (speed > 0)
    {
      analogWrite(motor.IN_2, speed);
      pinMode(motor.IN_1, OUTPUT);
      digitalWrite(motor.IN_1, 0);
    }
    else if (speed < 0)
    {
      analogWrite(motor.IN_1, abs(speed));
      pinMode(motor.IN_2, OUTPUT);
      digitalWrite(motor.IN_2, 0);
    }
    else
    {
      R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[motor.IN_1].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
      R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[motor.IN_2].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
      digitalWrite(motor.IN_1, 1);
      digitalWrite(motor.IN_2, 1);
    }
  }
  else if (motor == motor_4)
  {
    if (speed > 0)
    {
      R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[motor.IN_1].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
      R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[motor.IN_2].pin, IOPORT_CFG_PWM);

      timer0.set_duty_cycle((97959 * speed) / 256.0f, CHANNEL_B);
    }
    else if (speed < 0)
    {
      R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[motor.IN_1].pin, IOPORT_CFG_PWM);
      R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[motor.IN_2].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);

      timer0.set_duty_cycle((97959 * abs(speed)) / 256.0f, CHANNEL_B);
    }
    else
    {
      R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[motor.IN_1].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
      R_IOPORT_PinCfg(&g_ioport_ctrl, g_pin_cfg[motor.IN_2].pin, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
      digitalWrite(motor.IN_1, 1);
      digitalWrite(motor.IN_2, 1);
    }
  }
}

void RobotixInnoBot::speed(int32_t left, int32_t right)
{
  setMotorSpeed(motor_1, -left);
  setMotorSpeed(motor_2, -left);

  setMotorSpeed(motor_3, right);
  setMotorSpeed(motor_4, right);
}

void RobotixInnoBot::rotate(uint8_t direction, int32_t speed)
{
}

void RobotixInnoBot::move(uint8_t direction, int32_t speed)
{
}

void RobotixInnoBot::brake()
{
}

float RobotixInnoBot::getTemp()
{
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;
  imu.getEvent(&accel, &gyro, &temp, &mag);
  return temp.temperature;
}

void RobotixInnoBot::getOrientation(float &roll, float &pitch, float &yaw)
{
}

void RobotixInnoBot::getAccelerations(float &ax, float &ay, float &az)
{
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;
  imu.getEvent(&accel, &gyro, &temp, &mag);

  ax = accel.acceleration.x;
  ay = accel.acceleration.y;
  az = accel.acceleration.z;
}

void RobotixInnoBot::getGyros(float &gx, float &gy, float &gz)
{
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;
  imu.getEvent(&accel, &gyro, &temp, &mag);

  gx = gyro.gyro.x;
  gy = gyro.gyro.y;
  gz = gyro.gyro.z;
}

void RobotixInnoBot::getMagno(float &mx, float &my, float &mz)
{
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;
  imu.getEvent(&accel, &gyro, &temp, &mag);

  mx = mag.magnetic.x;
  my = mag.magnetic.y;
  mz = mag.magnetic.z;
}

void RobotixInnoBot::getIMU(float &ax, float &ay, float &az, float &gx, float &gy, float &gz)
{
}

void RobotixInnoBot::setServoPositions(Servo servo, uint8_t position)
{
  servo.write(position);
}

void RobotixInnoBot::showLogo(uint8_t logo)
{
  switch (logo)
  {
  case LOGO_ROBOTIX:
    STE2007LoadBMP(LogoRobotix);
    break;
  case LOGO_ARDUINO:
    STE2007LoadBMP(LogoArduino);
    break;
  default:
    break;
  }
}

void RobotixInnoBot::drawString(uint8_t x, uint8_t y,  char *s, uint8_t size)
{
  STE2007WriteString(x, y, s, size, 0);
}

void RobotixInnoBot::clearRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
}

void RobotixInnoBot::clearLCD()
{
  STE2007Fill(0);
}

// RJ PORTS/////////////////////////////////
uint8_t RobotixInnoBot::rjSrfInit(rj_port &port)
{
  port.type = MOD_SRF;
  pinMode(port.pin_B, OUTPUT); // TRIG
  pinMode(port.pin_A, INPUT);  // ECHO
}

float RobotixInnoBot::rjSrfRead(rj_port port)
{
  long duration;

  digitalWrite(port.pin_B, LOW);
  delayMicroseconds(2);
  // Set the TriggerPin on HIGH state for 10 microseconds
  digitalWrite(port.pin_B, HIGH);
  delayMicroseconds(10);
  digitalWrite(port.pin_B, LOW);
  // Read the EchoPin, returning the sound wave travel time in microseconds
  duration = pulseIn(port.pin_A, HIGH);
  // Calculate and return the distance
  return duration * 0.034 / 2;
}

uint8_t RobotixInnoBot::rjVlxInit(rj_port &port)
{
  port.type = MOD_VLX;
  Wire.begin();
  vl53l1.setTimeout(500);
  vl53l1.init();
  vl53l1.setDistanceMode(VL53L1X::Long);
  vl53l1.setMeasurementTimingBudget(50000);
  vl53l1.startContinuous(50);
}

int RobotixInnoBot::rjVlxRead(rj_port port)
{
  return vl53l1.read();
}

uint8_t RobotixInnoBot::rjSwInit(rj_port &port)
{
  port.type = MOD_SWITCH;
  pinMode(port.pin_A, INPUT);
}

uint8_t RobotixInnoBot::rjSwRead(rj_port port)
{
  return digitalRead(port.pin_A);
}

uint8_t RobotixInnoBot::rjLedInit(rj_port &port)
{
  port.type = MOD_LED;
  pinMode(port.pin_B, OUTPUT);
}

void RobotixInnoBot::rjLedWrite(rj_port port, uint8_t state)
{
  digitalWrite(port.pin_B, state);
}

uint8_t RobotixInnoBot::rjIrInit(rj_port &port)
{
  port.type = MOD_IR;
}

int RobotixInnoBot::rjIrRead(rj_port port)
{
  return analogRead(port.pin_A);
}

uint8_t RobotixInnoBot::rjTempHumInit(rj_port &port)
{
  port.type = MOD_TE_HU;
  Wire.begin();
  DHT.begin();
}

uint8_t RobotixInnoBot::rjLdrInit(rj_port &port)
{
  port.type = MOD_LDR;
}

int RobotixInnoBot::rjLdrRead(rj_port port)
{
  return analogRead(port.pin_A);
}

float RobotixInnoBot::rjHumRead(rj_port port)
{
  DHT.read();
  return DHT.getHumidity();
}

float RobotixInnoBot::rjTempRead(rj_port port)
{
  DHT.read();
  return DHT.getTemperature();
}

uint8_t RobotixInnoBot::rjRgbInit(rj_port &port)
{
  port.type = MOD_RGB;
  pixels = Adafruit_NeoPixel(16, port.pin_A, NEO_GRB + NEO_KHZ800);
  pixels.begin();
  pixels.clear();
}

void RobotixInnoBot::rjRgbWrite(rj_port port, uint8_t pixel, uint8_t r, uint8_t g, uint8_t b)
{
  pixels.setPixelColor(pixel, pixels.Color(r, g, b));
  pixels.show();
}

void RobotixInnoBot::rjRgbWrite(rj_port port, uint8_t pixel, uint32_t c)
{
  pixels.setPixelColor(pixel, c);
  pixels.show();
}

void RobotixInnoBot::rjRgbClear(rj_port port)
{
  pixels.clear();
  pixels.show();
}

uint8_t RobotixInnoBot::rjIrArrayInit(rj_port &port)
{
  port.type = MOD_IR_ARRAY;
  Wire.begin();
}

void RobotixInnoBot::rjIrArrayRead(rj_port port, uint8_t *sensors)
{
  uint8_t send_byte[] = {0, 1, 2, 3};

  for (int i = 0; i < 4; i++)
  {
    Wire.beginTransmission(IR_ARRAY_ADDRESS);
    Wire.write(send_byte[i]);    // Send register address
    Wire.endTransmission(false); // Restart condition (repeated start)

    Wire.requestFrom(IR_ARRAY_ADDRESS, 1); // Request 1 byte from slave
    if (Wire.available())
    {
      sensors[i] = Wire.read(); // Read received byte
    }
  }
}

uint8_t RobotixInnoBot::rjTcsInit(rj_port &port)
{
  tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
  port.type = MOD_TCS;
  tcs.begin();
}

void RobotixInnoBot::rjTcsRead(rj_port port, uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c, uint16_t &colorTemp, uint16_t &lux)
{
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);
}

uint8_t RobotixInnoBot::addModToMonitor(rj_port port)
{
  unsigned int val = 0;
  switch (port.type)
  {
  case MOD_SRF:
    val = (int)(rjSrfRead(port) * 10);
    break;

  case MOD_VLX:
    val = rjVlxRead(port);
    break;

  case MOD_IR:
    val = rjIrRead(port);
    break;

  case MOD_LDR:
    val = rjIrRead(port);
    break;

  case MOD_TE_HU:
    val = (unsigned int)(rjTempRead(port) + 50) * 1000 + (unsigned int)rjHumRead(port);
    break;

  case MOD_SWITCH:
    val = rjSwRead(port);
    break;

  case MOD_TCS:
    uint16_t r, g, b, c, colorTemp, lux;
    rjTcsRead(port, r, g, b, c, colorTemp, lux);
    val = ((byte)r << 16) | ((byte)g << 8) | (byte)b;
    break;

  default:
    break;
  }

  monitor_ports[port.num - 1]->type = port.type;
  monitor_ports[port.num - 1]->value = val;
}

const char *RobotixInnoBot::getMonitorData()
{
  static char buffer[100];
  char types[13];

  types[0] = getMappedChar(portMonitor.port_1.type);
  types[1] = getMappedChar(portMonitor.port_2.type);
  types[2] = getMappedChar(portMonitor.port_3.type);
  types[3] = getMappedChar(portMonitor.port_4.type);
  types[4] = getMappedChar(portMonitor.port_5.type);
  types[5] = getMappedChar(portMonitor.port_6.type);
  types[6] = getMappedChar(portMonitor.port_7.type);
  types[7] = getMappedChar(portMonitor.port_8.type);
  types[8] = getMappedChar(portMonitor.port_9.type);
  types[9] = getMappedChar(portMonitor.port_10.type);
  types[10] = getMappedChar(portMonitor.port_11.type);
  types[11] = getMappedChar(portMonitor.port_12.type);
  types[12] = '\0';

  sprintf(buffer, "COU12,%s,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,END", types,
          portMonitor.port_1.value, portMonitor.port_2.value, portMonitor.port_3.value, portMonitor.port_4.value, portMonitor.port_5.value,
          portMonitor.port_6.value, portMonitor.port_7.value, portMonitor.port_8.value, portMonitor.port_9.value, portMonitor.port_10.value,
          portMonitor.port_11.value, portMonitor.port_12.value);

  return buffer;
}
