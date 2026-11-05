// ============================================================
//  AARUARDUINO MAX7219 Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.max7219(10, 1);
  AARUARDUINO.displayClear();
}

void loop() {
  static long count = 0;
  AARUARDUINO.displayPrint(count++);
  delay(100);
}