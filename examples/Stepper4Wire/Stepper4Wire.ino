// ============================================================
//  AARUARDUINO 28BYJ-48 Stepper Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.stepper4Wire(8, 9, 10, 11);
  AARUARDUINO.stepperSetRpm(10);
  AARUARDUINO.stepperMove(4096);
}

void loop() {
  if (AARUARDUINO.stepperRun()) {
    // moving
  } else {
    delay(1000);
    AARUARDUINO.stepperMove(-4096);
  }
}