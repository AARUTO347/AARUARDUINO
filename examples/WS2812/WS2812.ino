// ============================================================
//  AARUARDUINO WS2812 NeoPixel Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.ws2812(6, 10);
  AARUARDUINO.setBrightness(50);
}

void loop() {
  AARUARDUINO.rainbow();
  AARUARDUINO.show();
  delay(50);
}