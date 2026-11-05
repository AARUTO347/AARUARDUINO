// ============================================================
//  AARUARDUINO DHT11/DHT22 Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.dht11(2);
}

void loop() {
  float temp, hum;
  if (AARUARDUINO.readDHT(temp, hum)) {
    Serial.print("Temp: ");
    Serial.print(temp, 1);
    Serial.print(" C / ");
    Serial.print(AARUARDUINO.readTemperatureF(), 1);
    Serial.print(" F, Humidity: ");
    Serial.print(hum, 1);
    Serial.println(" %");
  } else {
    Serial.print("DHT Error: ");
    Serial.println(AARUARDUINO.errorString());
  }
  delay(2000);
}