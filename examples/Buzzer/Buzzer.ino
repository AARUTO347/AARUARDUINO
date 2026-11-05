// ============================================================
//  AARUARDUINO Buzzer Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.buzzer(8);
}

void loop() {
  AARUARDUINO.beep(1000, 200);
  delay(300);
  AARUARDUINO.beep(1500, 200);
  delay(300);
  AARUARDUINO.beep(2000, 200);
  delay(1000);
}