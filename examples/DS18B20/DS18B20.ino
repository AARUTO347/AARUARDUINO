// ============================================================
//  AARUARDUINO DS18B20 Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.ds18b20(2);
  Serial.print("Found ");
  Serial.print(AARUARDUINO.ds18b20Count());
  Serial.println(" DS18B20 sensor(s)");
}

void loop() {
  for (int i = 0; i < AARUARDUINO.ds18b20Count(); i++) {
    float c = AARUARDUINO.readDS18B20(i);
    float f = AARUARDUINO.readDS18B20F(i);
    if (!isnan(c)) {
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(c, 2);
      Serial.print(" C / ");
      Serial.print(f, 2);
      Serial.println(" F");
    }
  }
  Serial.println("---");
  delay(1000);
}