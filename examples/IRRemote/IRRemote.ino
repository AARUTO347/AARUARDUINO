// ============================================================
//  AARUARDUINO IR Remote Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.irRecv(2);
}

void loop() {
  AaruIRResult result;
  if (AARUARDUINO.irDecode(&result)) {
    Serial.print("Protocol: ");
    Serial.print(result.protocol);
    Serial.print(", Value: 0x");
    Serial.print(result.value, HEX);
    Serial.print(", Addr: 0x");
    Serial.print(result.address, HEX);
    Serial.print(", Cmd: 0x");
    Serial.println(result.command, HEX);
    AARUARDUINO.irResume();
  }
  delay(10);
}