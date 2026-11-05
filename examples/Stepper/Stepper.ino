// ============================================================
//  AARUARDUINO Stepper Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.stepper(4, 5, 6);
  AARUARDUINO.stepperSetRpm(60);
  AARUARDUINO.stepperMove(200);
}

void loop() {
  if (AARUARDUINO.stepperRun()) {
    // still moving
  } else {
    delay(1000);
    AARUARDUINO.stepperMove(-200);
  }
}