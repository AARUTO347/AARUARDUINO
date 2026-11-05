// ============================================================
//  AARUARDUINO LED PWM Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.led(9);
}

void loop() {
  for (int i = 0; i <= 255; i++) {
    AARUARDUINO.setBrightness(i);
    delay(5);
  }
  for (int i = 255; i >= 0; i--) {
    AARUARDUINO.setBrightness(i);
    delay(5);
  }
}