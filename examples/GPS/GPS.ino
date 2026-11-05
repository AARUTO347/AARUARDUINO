// ============================================================
//  AARUARDUINO GPS Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.gps(2, 3, 9600);
}

void loop() {
  AARUARDUINO.gpsUpdate();
  if (AARUARDUINO.gpsFix()) {
    Serial.print("Lat: ");
    Serial.print(AARUARDUINO.gpsLat(), 6);
    Serial.print(", Lon: ");
    Serial.print(AARUARDUINO.gpsLon(), 6);
    Serial.print(", Alt: ");
    Serial.print(AARUARDUINO.gpsAlt(), 1);
    Serial.print("m, Sats: ");
    Serial.println(AARUARDUINO.gpsSats());
  } else {
    Serial.println("Waiting for GPS fix...");
  }
  delay(1000);
}