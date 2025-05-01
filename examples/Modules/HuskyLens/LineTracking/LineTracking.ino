#include "HUSKYLENS.h"
#include "PIDLoop.h"
#include <RobotixInnoBot.h>

#define ROBOT_FAST 100

RobotixInnoBot bot;
PIDLoop headingLoop(500, 0, 0, false);
HUSKYLENS huskylens;
//HUSKYLENS green line >> SDA; blue line >> SCL
int ID1 = 1;
void printResult(HUSKYLENSResult result);



void setup() {
  bot.begin();
  Serial.begin(115200);


  Wire.begin();
  while (!huskylens.begin(Wire)) {
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protol Type>>I2C)"));
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }
  huskylens.writeAlgorithm(ALGORITHM_LINE_TRACKING);  //Switch the algorithm to line tracking.
}
int left = 0, right = 0;

void loop() {
  int32_t error;
  if (!huskylens.request(ID1)) {
    Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    left = 0;
    right = 0;
  } else if (!huskylens.isLearned()) {
    Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    left = 0;
    right = 0;
  } else if (!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
  else {
    HUSKYLENSResult result = huskylens.read();
    printResult(result);

    // Calculate the error:
    error = (int32_t)result.xTarget - (int32_t)160;

    // Perform PID algorithm.
    headingLoop.update(error);

    // separate heading into left and right wheel velocities.
    left = headingLoop.m_command;
    right = -headingLoop.m_command;

    left += ROBOT_FAST;
    right += ROBOT_FAST;
  }

  Serial.println(String() + left + "," + right);
  bot.speed(left, right);
}

void printResult(HUSKYLENSResult result) {
  if (result.command == COMMAND_RETURN_BLOCK) {
    Serial.println(String() + F("Block:xCenter=") + result.xCenter + F(",yCenter=") + result.yCenter + F(",width=") + result.width + F(",height=") + result.height + F(",ID=") + result.ID);
  } else if (result.command == COMMAND_RETURN_ARROW) {
    Serial.println(String() + F("Arrow:xOrigin=") + result.xOrigin + F(",yOrigin=") + result.yOrigin + F(",xTarget=") + result.xTarget + F(",yTarget=") + result.yTarget + F(",ID=") + result.ID);
  } else {
    Serial.println("Object unknown!");
  }
}