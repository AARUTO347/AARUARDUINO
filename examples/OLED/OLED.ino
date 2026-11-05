// ============================================================
//  AARUARDUINO SSD1306 OLED Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.oled(0x3C, 128, 64);
  AARUARDUINO.displayClear();
  AARUARDUINO.displayText("AARUARDUINO");
  AARUARDUINO.displayTextAt(1, 0, "SSD1306 OLED");
  AARUARDUINO.displayTextAt(2, 0, "MADE BY AARUTO");
  delay(2000);
}

void loop() {
  AARUARDUINO.displayClear();
  AARUARDUINO.displayText("Counter:");
  AARUARDUINO.displayTextAt(1, 0, "Value: ");
  static int count = 0;
  AARUARDUINO.displayPrint(count++);
  delay(500);
}