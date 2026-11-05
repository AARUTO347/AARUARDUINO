// ============================================================
//  AARUARDUINO Relay Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.relay(7, true);
}

void loop() {
  AARUARDUINO.setRelay(true);
  Serial.println("Relay ON");
  delay(1000);
  AARUARDUINO.setRelay(false);
  Serial.println("Relay OFF");
  delay(1000);
}