// ============================================================
//  AARUARDUINO Basic Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.ultrasonic(7, 8);
  AARUARDUINO.led(13);
  AARUARDUINO.buzzer(9);
  AARUARDUINO.lcdI2C(0x27, 16, 2);
  AARUARDUINO.displayText("AARUARDUINO Ready");
  delay(1000);
}

void loop() {
  AARUARDUINO.update();
  float cm = AARUARDUINO.readUltrasonic();
  if (AARUARDUINO.lastError() != AARUARDUINO_OK) return;
  if (cm > 0 && cm < 20) {
    AARUARDUINO.setLED(true);
    AARUARDUINO.beep(2000, 100);
    AARUARDUINO.displayText("Object Close!");
  } else {
    AARUARDUINO.setLED(false);
    AARUARDUINO.displayText("Monitoring...");
  }
  delay(100);
}