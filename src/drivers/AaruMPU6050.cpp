// ============================================================
//  AARUARDUINO MPU6050 Driver
//  MADE BY AARUTO
// ============================================================
#include "AaruMPU6050.h"

AaruarduinoError AaruMPU6050::begin(uint8_t addr) {
    return begin(nullptr, addr);
}

AaruarduinoError AaruMPU6050::begin(AaruI2CEngine* i2c, uint8_t addr) {
    _addr = addr; _i2c = i2c; _configured = true; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    if (!_i2c) _i2c = new AaruI2CEngine();
    writeReg(MPU6050_PWR_MGMT_1, 0);
#endif
    return _err;
}

AaruarduinoError AaruMPU6050::setAccelRange(MPU6050AccelRange range) {
    _err = writeReg(MPU6050_ACCEL_CFG, (uint8_t)range << 3);
    return _err;
}
AaruarduinoError AaruMPU6050::setGyroRange(MPU6050GyroRange range) {
    _err = writeReg(MPU6050_GYRO_CFG, (uint8_t)range << 3);
    return _err;
}
AaruarduinoError AaruMPU6050::setDLPF(uint8_t dlpf) {
    _err = writeReg(0x1A, dlpf);
    return _err;
}
AaruarduinoError AaruMPU6050::setSampleRate(uint8_t div) {
    _err = writeReg(0x19, div);
    return _err;
}

bool AaruMPU6050::readAccel(float& x, float& y, float& z) {
    uint8_t buf[6];
    _err = readRegs(MPU6050_ACCEL_XOUT, buf, 6);
    if (_err != AARUARDUINO_OK) return false;
    int16_t ax = (buf[0] << 8) | buf[1];
    int16_t ay = (buf[2] << 8) | buf[3];
    int16_t az = (buf[4] << 8) | buf[5];
    x = ax / _accel_scale + _accel_off[0];
    y = ay / _accel_scale + _accel_off[1];
    z = az / _accel_scale + _accel_off[2];
    return true;
}

bool AaruMPU6050::readGyro(float& x, float& y, float& z) {
    uint8_t buf[6];
    _err = readRegs(MPU6050_GYRO_XOUT, buf, 6);
    if (_err != AARUARDUINO_OK) return false;
    int16_t gx = (buf[0] << 8) | buf[1];
    int16_t gy = (buf[2] << 8) | buf[3];
    int16_t gz = (buf[4] << 8) | buf[5];
    x = gx / _gyro_scale + _gyro_off[0];
    y = gy / _gyro_scale + _gyro_off[1];
    z = gz / _gyro_scale + _gyro_off[2];
    return true;
}

float AaruMPU6050::readTemp() {
    uint8_t buf[2];
    _err = readRegs(MPU6050_TEMP_OUT, buf, 2);
    if (_err != AARUARDUINO_OK) return NAN;
    int16_t temp = (buf[0] << 8) | buf[1];
    return (float)temp / 340.0f + 36.53f;
}

float AaruMPU6050::readAccelX() { float x, y, z; readAccel(x, y, z); return x; }
float AaruMPU6050::readAccelY() { float x, y, z; readAccel(x, y, z); return y; }
float AaruMPU6050::readAccelZ() { float x, y, z; readAccel(x, y, z); return z; }
float AaruMPU6050::readGyroX() { float x, y, z; readGyro(x, y, z); return x; }
float AaruMPU6050::readGyroY() { float x, y, z; readGyro(x, y, z); return y; }
float AaruMPU6050::readGyroZ() { float x, y, z; readGyro(x, y, z); return z; }

void AaruMPU6050::calibrateAccel(int samples) { _accel_off[0] = 0; _accel_off[1] = 0; _accel_off[2] = 0; }
void AaruMPU6050::calibrateGyro(int samples) { _gyro_off[0] = 0; _gyro_off[1] = 0; _gyro_off[2] = 0; }
void AaruMPU6050::setAccelOffset(float x, float y, float z) {
    _accel_off[0] = x; _accel_off[1] = y; _accel_off[2] = z;
}
void AaruMPU6050::setGyroOffset(float x, float y, float z) {
    _gyro_off[0] = x; _gyro_off[1] = y; _gyro_off[2] = z;
}

AaruarduinoError AaruMPU6050::sleep() {
    _err = writeReg(MPU6050_PWR_MGMT_1, 0x40);
    return _err;
}
AaruarduinoError AaruMPU6050::wake() {
    _err = writeReg(MPU6050_PWR_MGMT_1, 0x00);
    return _err;
}

uint8_t AaruMPU6050::addr() const { return _addr; }
bool AaruMPU6050::isConfigured() const { return _configured; }
AaruarduinoError AaruMPU6050::lastError() const { return _err; }
const char* AaruMPU6050::errorString() const { return aaruarduinoErrorString(_err); }

AaruarduinoError AaruMPU6050::writeReg(uint8_t reg, uint8_t val) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    return _i2c->write8(_addr, reg, val);
}
AaruarduinoError AaruMPU6050::readRegs(uint8_t reg, uint8_t* buf, uint8_t len) {
    if (!_i2c) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    return _i2c->readBytes(_addr, reg, buf, len);
}
