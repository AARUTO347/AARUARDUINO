// ============================================================
//  AARUARDUINO LCD I2C Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.lcdI2C(0x27, 16, 2);
  AARUARDUINO.displayClear();
  AARUARDUINO.displayText("AARUARDUINO");
  AARUARDUINO.displayTextAt(1, 0, "LCD 16x2 I2C");
  delay(2000);
}

void loop() {
  AARUARDUINO.displayClear();
  AARUARDUINO.displayText("Time:");
  AARUARDUINO.displayTextAt(1, 0, "Sec: ");
  AARUARDUINO.displayPrint(millis() / 1000);
  delay(1000);
}