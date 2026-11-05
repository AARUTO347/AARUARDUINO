// ============================================================
//  AARUARDUINO TM1637 Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.tm1637(2, 3);
  AARUARDUINO.setBrightness(7);
}

void loop() {
  static int count = 0;
  AARUARDUINO.printNumber(count++);
  delay(100);
}