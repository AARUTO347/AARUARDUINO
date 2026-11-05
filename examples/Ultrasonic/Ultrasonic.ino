// ============================================================
//  AARUARDUINO HC-SR04 Ultrasonic Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.ultrasonic(7, 8);
  AARUARDUINO.setUltrasonicTimeout(30000);
}

void loop() {
  float cm = AARUARDUINO.readUltrasonic();
  if (AARUARDUINO.lastError() == AARUARDUINO_OK) {
    Serial.print("Distance: ");
    Serial.print(cm, 1);
    Serial.print(" cm / ");
    Serial.print(AARUARDUINO.readUltrasonicInches(), 1);
    Serial.println(" in");
  } else {
    Serial.print("Error: ");
    Serial.println(AARUARDUINO.errorString());
  }
  delay(200);
}