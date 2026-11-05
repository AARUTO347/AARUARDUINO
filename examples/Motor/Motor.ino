// ============================================================
//  AARUARDUINO Motor Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.motor(4, 5, 6);
}

void loop() {
  AARUARDUINO.motorSpeed(200);
  delay(2000);
  AARUARDUINO.motorSpeed(0);
  delay(500);
  AARUARDUINO.motorSpeed(-200);
  delay(2000);
  AARUARDUINO.motorBrake();
  delay(500);
}