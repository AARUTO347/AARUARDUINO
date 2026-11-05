// ============================================================
//  AARUARDUINO RGB LED Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.rgb(9, 10, 11);
}

void loop() {
  AARUARDUINO.setRGB(255, 0, 0); delay(500);
  AARUARDUINO.setRGB(0, 255, 0); delay(500);
  AARUARDUINO.setRGB(0, 0, 255); delay(500);
  AARUARDUINO.setRGB(255, 255, 0); delay(500);
  AARUARDUINO.setRGB(0, 255, 255); delay(500);
  AARUARDUINO.setRGB(255, 0, 255); delay(500);
  AARUARDUINO.setRGB(255, 255, 255); delay(500);
  AARUARDUINO.rgbOff(); delay(500);
}