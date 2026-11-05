// ============================================================
//  AARUARDUINO MPU6050 IMU Example
//  MADE BY AARUTO
// ============================================================
#include <AARUARDUINO.h>

void setup() {
  Serial.begin(115200);
  AARUARDUINO.begin();
  AARUARDUINO.mpu6050(0x68);
}

void loop() {
  float ax, ay, az, gx, gy, gz;
  if (AARUARDUINO.readAccel(ax, ay, az) && AARUARDUINO.readGyro(gx, gy, gz)) {
    Serial.print("Accel: ");
    Serial.print(ax, 2); Serial.print("g, ");
    Serial.print(ay, 2); Serial.print("g, ");
    Serial.print(az, 2); Serial.println("g");
    Serial.print("Gyro:  ");
    Serial.print(gx, 1); Serial.print("d/s, ");
    Serial.print(gy, 1); Serial.print("d/s, ");
    Serial.print(gz, 1); Serial.println("d/s");
    Serial.print("Temp: ");
    Serial.print(AARUARDUINO.readIMUTemp(), 1);
    Serial.println(" C");
  } else {
    Serial.print("MPU Error: ");
    Serial.println(AARUARDUINO.errorString());
  }
  Serial.println("---");
  delay(500);
}