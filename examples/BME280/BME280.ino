// ============================================================
//  AARUARDUINO BME280 Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.bme280(0x76);
}

void loop() {
  float temp, hum, press;
  if (AARUARDUINO.readBME(temp, hum, press)) {
    Serial.print("Temp: ");
    Serial.print(temp, 2);
    Serial.print(" C, Humidity: ");
    Serial.print(hum, 1);
    Serial.print(" %, Pressure: ");
    Serial.print(press, 1);
    Serial.print(" hPa, Altitude: ");
    Serial.print(AARUARDUINO.readBMEaltitude(1013.25), 1);
    Serial.println(" m");
  } else {
    Serial.print("BME Error: ");
    Serial.println(AARUARDUINO.errorString());
  }
  delay(2000);
}