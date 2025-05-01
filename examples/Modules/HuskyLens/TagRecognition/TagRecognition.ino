#include "HUSKYLENS.h"

HUSKYLENS huskylens;

void printResult(HUSKYLENSResult result);

void setup() {

  Serial.begin(115200);
  Serial1.begin(9600);

  while (!huskylens.begin(Serial1)) {
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }

  huskylens.writeAlgorithm(ALGORITHM_TAG_RECOGNITION);
  delay(500);
  huskylens.customText("Robotix InnoBot", 100, 10);
}

void loop() {
  if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
  else if (!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  else if (!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
  else {
    Serial.println(F("###########"));
    while (huskylens.available()) {
      HUSKYLENSResult result = huskylens.read();
      printResult(result);
    }
  }
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