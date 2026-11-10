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
  AARUARDUINO.ws2812Rainbow();
  AARUARDUINO.ws2812Show();
  delay(50);
}