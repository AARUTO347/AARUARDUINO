// ============================================================
//  AARUARDUINO v1.0.0 — Universal Arduino Library
//  MADE BY AARUTO
// ============================================================
#include "AARUARDUINO.h"

#if defined(ARDUINO)
#include <Arduino.h>
#endif

// ---- Construction ----
AaruarduinoClass::AaruarduinoClass() {}

// ---- Core ----
void AaruarduinoClass::begin() {
    if (_begun) return;
    _i2c.begin();
    _pulse.beginFreqCount(2); // default freq count pin (non-blocking setup)
    _begun = true;
    _err = AARUARDUINO_OK;
}

void AaruarduinoClass::update() {
    // Non-blocking updates for drivers needing polling
    // IR remote, GPS, pulse counting handled in read/interrupt paths
}

void AaruarduinoClass::dbg(const char* msg) {
    if (_dbg) _dbg->println(msg);
}

const char* AaruarduinoClass::errorString() const {
    return aaruarduinoErrorString(_err);
}

const char* AaruarduinoClass::errorString(AaruarduinoError e) {
    return aaruarduinoErrorString(e);
}

AaruarduinoFamily AaruarduinoClass::boardFamily() const {
    return aaruarduinoGetBoardFamily();
}

const char* AaruarduinoClass::boardName() const {
    return aaruarduinoGetBoardName();
}

// ---- Ultrasonic ----
AaruarduinoError AaruarduinoClass::ultrasonic(uint8_t trig_pin, uint8_t echo_pin) {
    AaruarduinoError e = aaruarduinoValidateDigitalPin(trig_pin, true);
    if (e != AARUARDUINO_OK) return setErr(e);
    e = aaruarduinoValidateDigitalPin(echo_pin, false);
    if (e != AARUARDUINO_OK) return setErr(e);
    return _sonic.begin(trig_pin, echo_pin);
}

AaruarduinoError AaruarduinoClass::setUltrasonicTimeout(unsigned long timeout_us) {
    _sonic.setTimeoutUs(timeout_us);
    return AARUARDUINO_OK;
}

float AaruarduinoClass::readUltrasonic() {
    _err = _sonic.lastError();
    return _sonic.readCm();
}

bool AaruarduinoClass::readUltrasonic(float& cm) {
    cm = _sonic.readCm();
    _err = _sonic.lastError();
    return !isnan(cm);
}

float AaruarduinoClass::readUltrasonicInches() {
    _err = _sonic.lastError();
    return _sonic.readInches();
}

// ---- DHT ----
AaruarduinoError AaruarduinoClass::dht11(uint8_t pin) { return dht(pin, AARU_DHT11); }
AaruarduinoError AaruarduinoClass::dht22(uint8_t pin) { return dht(pin, AARU_DHT22); }

AaruarduinoError AaruarduinoClass::dht(uint8_t pin, AaruDHTType type) {
    AaruarduinoError e = aaruarduinoValidateDigitalPin(pin, true);
    if (e != AARUARDUINO_OK) return setErr(e);
    return _dht.begin(pin, type);
}

float AaruarduinoClass::readTemperature() { _err = _dht.lastError(); return _dht.readTemperature(); }
float AaruarduinoClass::readHumidity() { _err = _dht.lastError(); return _dht.readHumidity(); }
float AaruarduinoClass::readTemperatureF() { _err = _dht.lastError(); return _dht.readTemperatureF(); }

bool AaruarduinoClass::readDHT(float& temp_c, float& hum_pct) {
    bool ok = _dht.read(temp_c, hum_pct);
    _err = _dht.lastError();
    return ok;
}

bool AaruarduinoClass::dhtFresh() const { return _dht.fresh(); }
unsigned long AaruarduinoClass::dhtAgeMs() const { return _dht.ageMs(); }

// ---- Analog Engine ----
AaruarduinoError AaruarduinoClass::analogSensor(uint8_t pin, AaruAnalogKind kind) {
    AaruarduinoError e = aaruarduinoValidateAnalogPin(pin);
    if (e != AARUARDUINO_OK) return setErr(e);
    return _aeng.begin(pin, kind);
}

AaruarduinoError AaruarduinoClass::ldr(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_LDR); }
AaruarduinoError AaruarduinoClass::potentiometer(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_POTENTIOMETER); }
AaruarduinoError AaruarduinoClass::lm35(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_LM35); }
AaruarduinoError AaruarduinoClass::tmp36(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_TMP36); }

AaruarduinoError AaruarduinoClass::thermistor(uint8_t pin, float series_r, float beta, float r0, float t0_c) {
    AaruarduinoError e = aaruarduinoValidateAnalogPin(pin);
    if (e != AARUARDUINO_OK) return setErr(e);
    return _aeng.beginWithPreset(pin, AARU_AN_KIND_NTC_BETA, series_r, beta, r0, t0_c);
}

AaruarduinoError AaruarduinoClass::soilMoisture(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_SOIL_MOISTURE); }
AaruarduinoError AaruarduinoClass::rainSensor(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_RAIN_SENSOR); }
AaruarduinoError AaruarduinoClass::waterLevel(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_WATER_LEVEL); }
AaruarduinoError AaruarduinoClass::soundSensor(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_SOUND_SENSOR); }
AaruarduinoError AaruarduinoClass::forceSensor(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_FSR); }
AaruarduinoError AaruarduinoClass::flameAnalog(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_FLAME_ANALOG); }

AaruarduinoError AaruarduinoClass::mq2(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_MQ2); }
AaruarduinoError AaruarduinoClass::mq3(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_MQ3); }
AaruarduinoError AaruarduinoClass::mq4(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_MQ4); }
AaruarduinoError AaruarduinoClass::mq5(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_MQ5); }
AaruarduinoError AaruarduinoClass::mq6(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_MQ6); }
AaruarduinoError AaruarduinoClass::mq7(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_MQ7); }
AaruarduinoError AaruarduinoClass::mq8(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_MQ8); }
AaruarduinoError AaruarduinoClass::mq9(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_MQ9); }
AaruarduinoError AaruarduinoClass::mq131(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_MQ131); }
AaruarduinoError AaruarduinoClass::mq135(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_MQ135); }
AaruarduinoError AaruarduinoClass::mq136(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_MQ136); }
AaruarduinoError AaruarduinoClass::mq137(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_MQ137); }
AaruarduinoError AaruarduinoClass::mq138(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_MQ138); }

AaruarduinoError AaruarduinoClass::genericMQ(uint8_t pin, float r0, float a, float b) {
    AaruarduinoError e = aaruarduinoValidateAnalogPin(pin);
    if (e != AARUARDUINO_OK) return setErr(e);
    e = _aeng.beginWithPreset(pin, AARU_AN_KIND_GENERIC_MQ, r0, a, b);
    if (e == AARUARDUINO_OK) _aeng.setParams(r0, a, b);
    return e;
}

AaruarduinoError AaruarduinoClass::acs712_5A(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_ACS712_5A); }
AaruarduinoError AaruarduinoClass::acs712_20A(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_ACS712_20A); }
AaruarduinoError AaruarduinoClass::acs712_30A(uint8_t pin) { return cfgAnalog(pin, AARU_AN_KIND_ACS712_30A); }

AaruarduinoError AaruarduinoClass::acs712(uint8_t pin, float mv_per_amp, float zero_mv) {
    AaruarduinoError e = aaruarduinoValidateAnalogPin(pin);
    if (e != AARUARDUINO_OK) return setErr(e);
    e = _aeng.beginWithPreset(pin, AARU_AN_KIND_ACS712_5A, mv_per_amp, zero_mv);
    if (e == AARUARDUINO_OK) _aeng.setParams(mv_per_amp, zero_mv);
    return e;
}

AaruarduinoError AaruarduinoClass::voltageDivider(uint8_t pin, float ratio) {
    AaruarduinoError e = aaruarduinoValidateAnalogPin(pin);
    if (e != AARUARDUINO_OK) return setErr(e);
    e = _aeng.beginWithPreset(pin, AARU_AN_KIND_VOLTAGE_DIVIDER, ratio);
    if (e == AARUARDUINO_OK) _aeng.setDividerRatio(ratio);
    return e;
}

AaruarduinoError AaruarduinoClass::acVoltageZMPT101B(uint8_t pin) {
    return cfgAnalog(pin, AARU_AN_KIND_AC_VOLTAGE_ZMPT101B);
}

int AaruarduinoClass::readAnalogRaw() { _err = _aeng.lastError(); return _aeng.readRaw(); }
float AaruarduinoClass::readAnalogPercent() { _err = _aeng.lastError(); return _aeng.readPercent(); }
float AaruarduinoClass::readAnalogVoltage() { _err = _aeng.lastError(); return _aeng.readVoltage(); }
float AaruarduinoClass::readAnalogValue() { _err = _aeng.lastError(); return _aeng.readValue(); }
int AaruarduinoClass::readLDR() { _err = _aeng.lastError(); return _aeng.readRaw(); }
int AaruarduinoClass::readPotentiometer() { _err = _aeng.lastError(); return _aeng.readRaw(); }
float AaruarduinoClass::readTemperatureAnalog() { _err = _aeng.lastError(); return _aeng.readValue(); }
float AaruarduinoClass::readGas() { _err = _aeng.lastError(); return _aeng.readValue(); }
float AaruarduinoClass::readCurrent() { _err = _aeng.lastError(); return _aeng.readValue(); }
float AaruarduinoClass::readSoil() { _err = _aeng.lastError(); return _aeng.readValue(); }
float AaruarduinoClass::readRain() { _err = _aeng.lastError(); return _aeng.readValue(); }
float AaruarduinoClass::readWaterLevel() { _err = _aeng.lastError(); return _aeng.readValue(); }
float AaruarduinoClass::readSound() { _err = _aeng.lastError(); return _aeng.readValue(); }
float AaruarduinoClass::readForce() { _err = _aeng.lastError(); return _aeng.readValue(); }
float AaruarduinoClass::readFlameAnalog() { _err = _aeng.lastError(); return _aeng.readValue(); }

int AaruarduinoClass::readAnalogPin(uint8_t pin) { _err = _aeng.lastError(); return _aeng.readRawPin(pin); }
float AaruarduinoClass::readVoltagePin(uint8_t pin, float divider) {
    _err = _aeng.lastError(); return _aeng.readVoltagePin(pin, divider);
}

void AaruarduinoClass::setThermistorParams(float series_r, float beta, float r0, float t0_c) {
    _aeng.setParams(series_r, beta, r0, t0_c);
}
void AaruarduinoClass::setCurrentParams(float mv_per_amp, float zero_mv) {
    _aeng.setParams(mv_per_amp, zero_mv);
}
void AaruarduinoClass::setMQParams(float r0, float a, float b) {
    _aeng.setParams(r0, a, b);
}

// ---- Digital Engine ----
AaruarduinoError AaruarduinoClass::digitalSensor(uint8_t pin, AaruDigitalKind kind,
                                                 bool active_low, bool pullup) {
    return cfgDigital(pin, kind, active_low, pullup);
}

AaruarduinoError AaruarduinoClass::button(uint8_t pin, bool active_low, bool pullup) {
    return cfgDigital(pin, AARU_DIG_KIND_BUTTON, active_low, pullup);
}
AaruarduinoError AaruarduinoClass::pir(uint8_t pin) { return cfgDigital(pin, AARU_DIG_KIND_PIR); }
AaruarduinoError AaruarduinoClass::radarRCWL(uint8_t pin) { return cfgDigital(pin, AARU_DIG_KIND_RADAR_RCWL); }
AaruarduinoError AaruarduinoClass::irObstacle(uint8_t pin) { return cfgDigital(pin, AARU_DIG_KIND_IR_OBSTACLE, true); }
AaruarduinoError AaruarduinoClass::lineFollower(uint8_t pin) { return cfgDigital(pin, AARU_DIG_KIND_IR_LINE, true); }
AaruarduinoError AaruarduinoClass::flameSensor(uint8_t pin) { return cfgDigital(pin, AARU_DIG_KIND_FLAME_DIGITAL, true); }
AaruarduinoError AaruarduinoClass::smokeDigital(uint8_t pin) { return cfgDigital(pin, AARU_DIG_KIND_SMOKE_DIGITAL); }
AaruarduinoError AaruarduinoClass::touchTTP223(uint8_t pin) { return cfgDigital(pin, AARU_DIG_KIND_TOUCH_TTP223, false, false); }
AaruarduinoError AaruarduinoClass::hallDigital(uint8_t pin) { return cfgDigital(pin, AARU_DIG_KIND_HALL_DIGITAL, true); }
AaruarduinoError AaruarduinoClass::reedSwitch(uint8_t pin) { return cfgDigital(pin, AARU_DIG_KIND_REED_SWITCH, false, true); }
AaruarduinoError AaruarduinoClass::limitSwitch(uint8_t pin, bool pullup, bool active_low) {
    return cfgDigital(pin, AARU_DIG_KIND_LIMIT_SWITCH, active_low, pullup);
}
AaruarduinoError AaruarduinoClass::vibrationSW420(uint8_t pin) { return cfgDigital(pin, AARU_DIG_KIND_VIBRATION_SW420, false); }
AaruarduinoError AaruarduinoClass::tiltSwitch(uint8_t pin) { return cfgDigital(pin, AARU_DIG_KIND_TILT_SWITCH); }
AaruarduinoError AaruarduinoClass::shockSW18010(uint8_t pin) { return cfgDigital(pin, AARU_DIG_KIND_SHOCK_SW18010); }
AaruarduinoError AaruarduinoClass::pulseCounter(uint8_t pin) { return cfgDigital(pin, AARU_DIG_KIND_PULSE_COUNTER); }

AaruarduinoError AaruarduinoClass::flowSensor(uint8_t pin, float pulses_per_litre) {
    AaruarduinoError e = cfgDigital(pin, AARU_DIG_KIND_FLOW_SENSOR);
    // flow-sensor config stored in digital engine not exposed; keep simple
    return e;
}

AaruarduinoError AaruarduinoClass::tachometer(uint8_t pin, float pulses_per_rev) {
    return cfgDigital(pin, AARU_DIG_KIND_TACHOMETER);
}

AaruarduinoError AaruarduinoClass::keypad4x4(uint8_t r0, uint8_t r1, uint8_t r2, uint8_t r3,
                                            uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3) {
    uint8_t pins[8] = {r0, r1, r2, r3, c0, c1, c2, c3};
    for (int i = 0; i < 8; i++) {
        AaruarduinoError e = aaruarduinoValidateDigitalPin(pins[i], i >= 4);
        if (e != AARUARDUINO_OK) return setErr(e);
    }
    return setErr(AARUARDUINO_INIT_FAIL);
}

AaruarduinoError AaruarduinoClass::irReceiver(uint8_t pin, IRProtocol proto) {
    return _ir.begin(pin, proto);
}

bool AaruarduinoClass::readDigitalSensor() { _err = _deng.lastError(); return _deng.readState(); }
bool AaruarduinoClass::readPIR() { _err = _deng.lastError(); return _deng.readState(); }
bool AaruarduinoClass::readRadar() { _err = _deng.lastError(); return _deng.readState(); }
bool AaruarduinoClass::readIR() { _err = _deng.lastError(); return _deng.readState(); }
bool AaruarduinoClass::readLine() { _err = _deng.lastError(); return _deng.readState(); }
bool AaruarduinoClass::readFlame() { _err = _deng.lastError(); return _deng.readState(); }
bool AaruarduinoClass::readSmoke() { _err = _deng.lastError(); return _deng.readState(); }
bool AaruarduinoClass::readTouch() { _err = _deng.lastError(); return _deng.readState(); }
bool AaruarduinoClass::readHall() { _err = _deng.lastError(); return _deng.readState(); }
bool AaruarduinoClass::readReed() { _err = _deng.lastError(); return _deng.readState(); }
bool AaruarduinoClass::readLimit() { _err = _deng.lastError(); return _deng.readState(); }
bool AaruarduinoClass::readVibration() { _err = _deng.lastError(); return _deng.readState(); }
bool AaruarduinoClass::readTilt() { _err = _deng.lastError(); return _deng.readState(); }
bool AaruarduinoClass::readShock() { _err = _deng.lastError(); return _deng.readState(); }

int AaruarduinoClass::readButton() { _err = _deng.lastError(); return _deng.readButton(); }
bool AaruarduinoClass::buttonPressed() { _err = _deng.lastError(); return _deng.pressed(); }
bool AaruarduinoClass::buttonRose() { _err = _deng.lastError(); return _deng.rose(); }
bool AaruarduinoClass::buttonFell() { _err = _deng.lastError(); return _deng.fell(); }

unsigned long AaruarduinoClass::pulseCount() { _err = _deng.lastError(); return _deng.pulseCount(); }
void AaruarduinoClass::resetPulseCount() { _deng.resetPulseCount(); }
float AaruarduinoClass::readFrequencyHz() { _err = _deng.lastError(); return _deng.readFrequencyHz(); }
float AaruarduinoClass::readFlowRateLmin() { _err = _deng.lastError(); return _deng.readFlowRateLmin(); }

uint32_t AaruarduinoClass::readIRNEC() { _err = _deng.lastError(); return _deng.readIRNEC(); }
bool AaruarduinoClass::irDataReady() { return _deng.irDataReady(); }
char AaruarduinoClass::readKey() { return 0; }

// ---- Rotary Encoder ----
AaruarduinoError AaruarduinoClass::rotaryEncoder(uint8_t clk, uint8_t dt, int sw_pin) {
    return _pulse.beginEncoder(clk, dt, sw_pin);
}
long AaruarduinoClass::encoderCount() { return _pulse.encoderCount(); }
void AaruarduinoClass::encoderReset() { _pulse.encoderReset(); }
int8_t AaruarduinoClass::encoderDirection() { return _pulse.encoderDirection(); }
bool AaruarduinoClass::encoderButton() { return _pulse.encoderButton(); }

// ---- Joystick ----
AaruarduinoError AaruarduinoClass::joystick(uint8_t x_pin, uint8_t y_pin, int sw_pin) {
    AaruarduinoError e = aaruarduinoValidateAnalogPin(x_pin);
    if (e != AARUARDUINO_OK) return setErr(e);
    e = aaruarduinoValidateAnalogPin(y_pin);
    if (e != AARUARDUINO_OK) return setErr(e);
    if (sw_pin >= 0) {
        e = aaruarduinoValidateDigitalPin(sw_pin, false);
        if (e != AARUARDUINO_OK) return setErr(e);
    }
    _aeng.begin(x_pin, AARU_AN_KIND_RAW);
    return AARUARDUINO_OK;
}

int AaruarduinoClass::readJoyX() { return _aeng.readRaw(); }
int AaruarduinoClass::readJoyY() { return _aeng.readRawPin(255); }
bool AaruarduinoClass::readJoyButton() { return false; }

// ---- MPU6050 ----
AaruarduinoError AaruarduinoClass::mpu6050(uint8_t addr) { return _mpu.begin(&_i2c, addr); }
bool AaruarduinoClass::readAccel(float& x, float& y, float& z) {
    _err = _mpu.lastError(); return _mpu.readAccel(x, y, z);
}
bool AaruarduinoClass::readGyro(float& x, float& y, float& z) {
    _err = _mpu.lastError(); return _mpu.readGyro(x, y, z);
}
float AaruarduinoClass::readIMUTemp() { _err = _mpu.lastError(); return _mpu.readTemp(); }
float AaruarduinoClass::readAccelX() { _err = _mpu.lastError(); return _mpu.readAccelX(); }
float AaruarduinoClass::readAccelY() { _err = _mpu.lastError(); return _mpu.readAccelY(); }
float AaruarduinoClass::readAccelZ() { _err = _mpu.lastError(); return _mpu.readAccelZ(); }
float AaruarduinoClass::readGyroX() { _err = _mpu.lastError(); return _mpu.readGyroX(); }
float AaruarduinoClass::readGyroY() { _err = _mpu.lastError(); return _mpu.readGyroY(); }
float AaruarduinoClass::readGyroZ() { _err = _mpu.lastError(); return _mpu.readGyroZ(); }
void AaruarduinoClass::calibrateAccel(int samples) { _mpu.calibrateAccel(samples); }
void AaruarduinoClass::calibrateGyro(int samples) { _mpu.calibrateGyro(samples); }
AaruarduinoError AaruarduinoClass::mpuSleep() { return _mpu.sleep(); }
AaruarduinoError AaruarduinoClass::mpuWake() { return _mpu.wake(); }

// ---- DS18B20 ----
AaruarduinoError AaruarduinoClass::ds18b20(uint8_t pin) { return _ds.begin(pin); }
int AaruarduinoClass::ds18b20Count() { return _ds.deviceCount(); }
float AaruarduinoClass::readDS18B20(int index) { _err = _ds.lastError(); return _ds.readTempC(index); }
float AaruarduinoClass::readDS18B20F(int index) { _err = _ds.lastError(); return _ds.readTempF(index); }
AaruarduinoError AaruarduinoClass::ds18b20Request() { return _ds.requestTemperatures(); }
bool AaruarduinoClass::ds18b20Ready() { return _ds.conversionReady(); }
float AaruarduinoClass::readDS18B20Async(int index) { return _ds.readTempC(index); }
AaruarduinoError AaruarduinoClass::ds18b20SetResolution(int index, uint8_t bits) {
    return _ds.setResolution(index, bits);
}

// ---- DS3231 RTC ----
AaruarduinoError AaruarduinoClass::rtcDS3231(uint8_t addr) { return _rtc.begin(&_i2c, addr); }
AaruarduinoError AaruarduinoClass::rtcSet(int year, int month, int day, int hour, int min, int sec) {
    AaruDateTime dt = {(uint16_t)year, (uint8_t)month, (uint8_t)day, (uint8_t)hour, (uint8_t)min, (uint8_t)sec, 1};
    return _rtc.setDateTime(dt);
}
AaruarduinoError AaruarduinoClass::rtcGet(int& year, int& month, int& day, int& hour, int& min, int& sec) {
    AaruDateTime dt;
    AaruarduinoError e = _rtc.getDateTime(dt);
    if (e == AARUARDUINO_OK) {
        year = dt.year; month = dt.month; day = dt.day;
        hour = dt.hour; min = dt.minute; sec = dt.second;
    }
    return e;
}
uint32_t AaruarduinoClass::rtcUnixTime() { return _rtc.getUnixTime(); }
float AaruarduinoClass::rtcTemp() { return _rtc.readTempC(); }
AaruarduinoError AaruarduinoClass::rtcAlarm1(int sec, int min, int hour, int day, bool dow) {
    return _rtc.setAlarm1(sec, min, hour, day, dow);
}
AaruarduinoError AaruarduinoClass::rtcAlarm2(int min, int hour, int day, bool dow) {
    return _rtc.setAlarm2(min, hour, day, dow);
}
bool AaruarduinoClass::rtcAlarmTriggered(int alarm_num) { return _rtc.alarmTriggered(alarm_num); }
AaruarduinoError AaruarduinoClass::rtcSquareWave(uint8_t freq) { return _rtc.setSquareWave(freq); }

// ---- BME280/BMP280 ----
AaruarduinoError AaruarduinoClass::bme280(uint8_t addr) { return _bme.beginI2C(&_i2c, addr); }
AaruarduinoError AaruarduinoClass::bmp280(uint8_t addr) { return _bme.beginI2C(&_i2c, addr); }
bool AaruarduinoClass::readBME(float& temp, float& hum, float& press) {
    AaruBME280Data data;
    bool ok = _bme.read(data);
    temp = data.temperature; hum = data.humidity; press = data.pressure;
    _err = _bme.lastError();
    return ok;
}
float AaruarduinoClass::readBMEtemp() { _err = _bme.lastError(); return _bme.readTemp(); }
float AaruarduinoClass::readBMEhumidity() { _err = _bme.lastError(); return _bme.readHumidity(); }
float AaruarduinoClass::readBMEpressure() { _err = _bme.lastError(); return _bme.readPressure(); }
float AaruarduinoClass::readBMEaltitude(float sea_level) { _err = _bme.lastError(); return _bme.readAltitude(sea_level); }

// ---- BH1750 ----
AaruarduinoError AaruarduinoClass::bh1750(uint8_t addr) { return _bh.begin(&_i2c, addr); }
float AaruarduinoClass::readLux() {
    _err = _bh.lastError();
    float lux = 0;
    _bh.read(lux);
    return lux;
}

// ---- Displays ----
AaruarduinoError AaruarduinoClass::oled(uint8_t addr, uint8_t w, uint8_t h) {
    AaruarduinoError e = _oled.beginI2C(&_i2c, addr, w, h);
    if (e == AARUARDUINO_OK) _display_kind = 1;
    return e;
}
AaruarduinoError AaruarduinoClass::lcdI2C(uint8_t addr, uint8_t cols, uint8_t rows) {
    AaruarduinoError e = _lcd.begin(&_i2c, addr, cols, rows);
    if (e == AARUARDUINO_OK) _display_kind = 2;
    return e;
}
AaruarduinoError AaruarduinoClass::lcd(uint8_t addr, uint8_t cols, uint8_t rows) {
    return lcdI2C(addr, cols, rows);
}
AaruarduinoError AaruarduinoClass::max7219(uint8_t cs, uint8_t devices) {
    AaruarduinoError e = _max7219.begin(&_spi, cs, devices);
    if (e == AARUARDUINO_OK) _display_kind = 3;
    return e;
}
AaruarduinoError AaruarduinoClass::tm1637(uint8_t clk, uint8_t dio) {
    AaruarduinoError e = _tm1637.begin(clk, dio);
    if (e == AARUARDUINO_OK) _display_kind = 4;
    return e;
}
AaruarduinoError AaruarduinoClass::ws2812(uint8_t pin, uint16_t num_leds) {
    return _ws2812.begin(pin, num_leds);
}

AaruarduinoError AaruarduinoClass::displayClear() {
    switch (_display_kind) {
        case 1: _oled.clear(); _oled.display(); break;
        case 2: _lcd.clear(); break;
        case 3: _max7219.clear(255); break;
        case 4: _tm1637.clear(); break;
        default: return setErr(AARUARDUINO_NOT_INITIALIZED);
    }
    return AARUARDUINO_OK;
}

AaruarduinoError AaruarduinoClass::displayText(const char* line) {
    switch (_display_kind) {
        case 1: _oled.clear(); _oled.setCursor(0, 0); _oled.print(line); _oled.display(); break;
        case 2: _lcd.clear(); _lcd.print(line); break;
        default: return setErr(AARUARDUINO_NOT_INITIALIZED);
    }
    return AARUARDUINO_OK;
}

AaruarduinoError AaruarduinoClass::displayText(int row, const char* line) {
    switch (_display_kind) {
        case 1: _oled.setCursor(0, row * 8); _oled.print(line); _oled.display(); break;
        case 2: _lcd.setCursor(0, row); _lcd.print(line); break;
        default: return setErr(AARUARDUINO_NOT_INITIALIZED);
    }
    return AARUARDUINO_OK;
}

AaruarduinoError AaruarduinoClass::displayTextAt(int row, int col, const char* line) {
    switch (_display_kind) {
        case 1: _oled.setCursor(col * 6, row * 8); _oled.print(line); _oled.display(); break;
        case 2: _lcd.setCursor(col, row); _lcd.print(line); break;
        default: return setErr(AARUARDUINO_NOT_INITIALIZED);
    }
    return AARUARDUINO_OK;
}

AaruarduinoError AaruarduinoClass::displayBacklight(bool on) {
    if (_display_kind != 2) return setErr(AARUARDUINO_UNSUPPORTED);
    return on ? _lcd.backlightOn() : _lcd.backlightOff();
}

// ---- displayPrint overloads (explicit integral types avoid ambiguity) ----
AaruarduinoError AaruarduinoClass::displayPrint(int n, int base) {
    switch (_display_kind) {
        case 1: _oled.print(n, base); _oled.display(); break;
        case 2: _lcd.print(n, base); break;
        case 3: _max7219.printNumber(0, n); break;
        case 4: _tm1637.printNumber(n); break;
        default: return setErr(AARUARDUINO_NOT_INITIALIZED);
    }
    return AARUARDUINO_OK;
}

AaruarduinoError AaruarduinoClass::displayPrint(unsigned int n, int base) {
    return displayPrint((int)n, base);
}

AaruarduinoError AaruarduinoClass::displayPrint(long n, int base) {
    return displayPrint((int)n, base);
}

AaruarduinoError AaruarduinoClass::displayPrint(unsigned long n, int base) {
    return displayPrint((long)n, base);
}

AaruarduinoError AaruarduinoClass::displayPrint(float f, int digits) {
    switch (_display_kind) {
        case 1: _oled.print(f, digits); _oled.display(); break;
        case 2: _lcd.print(f, digits); break;
        case 3: _max7219.printFloat(0, f, (uint8_t)digits); break;
        case 4: _tm1637.printFloat(f, (uint8_t)digits); break;
        default: return setErr(AARUARDUINO_NOT_INITIALIZED);
    }
    return AARUARDUINO_OK;
}

AaruarduinoError AaruarduinoClass::displaySetCursor(int col, int row) {
    switch (_display_kind) {
        case 1: _oled.setCursor(col * 6, row * 8); return AARUARDUINO_OK;
        case 2: return _lcd.setCursor((uint8_t)col, (uint8_t)row);
        default: return setErr(AARUARDUINO_NOT_INITIALIZED);
    }
}

AaruarduinoError AaruarduinoClass::displayContrast(uint8_t val) {
    if (_display_kind != 1) return setErr(AARUARDUINO_UNSUPPORTED);
    return _oled.setContrast(val);
}

AaruarduinoError AaruarduinoClass::displayInvert(bool inv) {
    if (_display_kind != 1) return setErr(AARUARDUINO_UNSUPPORTED);
    return _oled.invert(inv);
}

AaruarduinoError AaruarduinoClass::displayNumber(int num, bool leading_zeros) {
    if (_display_kind == 4) return _tm1637.printNumber(num, leading_zeros);
    if (_display_kind == 3) return _max7219.printNumber(0, num, leading_zeros);
    return setErr(AARUARDUINO_UNSUPPORTED);
}

AaruarduinoError AaruarduinoClass::displayFloat(float num, uint8_t decimals) {
    if (_display_kind == 4) return _tm1637.printFloat(num, decimals);
    if (_display_kind == 3) return _max7219.printFloat(0, num, decimals);
    return displayPrint(num, (int)decimals);
}

// ---- WS2812 / NeoPixel ----
AaruarduinoError AaruarduinoClass::ws2812Set(uint16_t index, uint8_t r, uint8_t g, uint8_t b) {
    return _ws2812.setPixel(index, r, g, b);
}
AaruarduinoError AaruarduinoClass::ws2812Fill(uint8_t r, uint8_t g, uint8_t b) {
    return _ws2812.fill(r, g, b);
}
AaruarduinoError AaruarduinoClass::ws2812Clear() {
    return _ws2812.clear();
}
AaruarduinoError AaruarduinoClass::ws2812Rainbow(uint16_t first_hue, uint16_t hue_step) {
    return _ws2812.rainbow(first_hue, hue_step);
}
AaruarduinoError AaruarduinoClass::ws2812Show() {
    return _ws2812.show();
}
AaruarduinoError AaruarduinoClass::ws2812Brightness(uint8_t b) {
    _ws2812.setBrightness(b);
    return AARUARDUINO_OK;
}

// ---- Actuators ----
AaruarduinoError AaruarduinoClass::led(uint8_t pin) {
    AaruarduinoError e = aaruarduinoValidatePwmPin(pin);
    if (e != AARUARDUINO_OK) return setErr(e);
    _led_pin = pin;
    e = _pwm.begin(pin);
    if (e == AARUARDUINO_OK) _pwm.write(0);
    return e;
}
AaruarduinoError AaruarduinoClass::setLED(bool on) {
    _led_state = on;
    return _pwm.write(on ? 255 : 0);
}
AaruarduinoError AaruarduinoClass::toggleLED() { return setLED(!_led_state); }
AaruarduinoError AaruarduinoClass::setBrightness(uint8_t val) { return _pwm.write(val); }

AaruarduinoError AaruarduinoClass::buzzer(uint8_t pin) {
    AaruarduinoError e = aaruarduinoValidatePwmPin(pin);
    if (e != AARUARDUINO_OK) return setErr(e);
    _buzzer_pin = pin;
    return _pwm.beginBuzzer(pin);
}
AaruarduinoError AaruarduinoClass::beep(unsigned long duration_ms) {
    return _pwm.tone(2000, duration_ms);
}
AaruarduinoError AaruarduinoClass::beep(uint16_t freq_hz, unsigned long duration_ms) {
    return _pwm.tone(freq_hz, duration_ms);
}
AaruarduinoError AaruarduinoClass::tone(uint16_t freq_hz, unsigned long duration_ms) {
    return _pwm.tone(freq_hz, duration_ms);
}
AaruarduinoError AaruarduinoClass::noTone() { return _pwm.noTone(); }
AaruarduinoError AaruarduinoClass::buzzerOff() { return _pwm.noTone(); }

AaruarduinoError AaruarduinoClass::relay(uint8_t pin, bool active_high) {
    AaruarduinoError e = aaruarduinoValidateDigitalPin(pin, true);
    if (e != AARUARDUINO_OK) return setErr(e);
    _relay_pin = pin; _relay_active_high = active_high;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, active_high ? LOW : HIGH);
    _relay_state = false;
    return AARUARDUINO_OK;
}
AaruarduinoError AaruarduinoClass::setRelay(bool on) {
    _relay_state = on;
    digitalWrite(_relay_pin, (_relay_active_high == on) ? HIGH : LOW);
    return AARUARDUINO_OK;
}
AaruarduinoError AaruarduinoClass::toggleRelay() { return setRelay(!_relay_state); }

AaruarduinoError AaruarduinoClass::solenoid(uint8_t pin, bool active_high) {
    return relay(pin, active_high);
}
AaruarduinoError AaruarduinoClass::setSolenoid(bool on) { return setRelay(on); }

AaruarduinoError AaruarduinoClass::laser(uint8_t pin) {
    AaruarduinoError e = aaruarduinoValidateDigitalPin(pin, true);
    if (e != AARUARDUINO_OK) return setErr(e);
    _laser_pin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    return AARUARDUINO_OK;
}
AaruarduinoError AaruarduinoClass::setLaser(bool on) {
    digitalWrite(_laser_pin, on ? HIGH : LOW);
    return AARUARDUINO_OK;
}

AaruarduinoError AaruarduinoClass::rgb(uint8_t r, uint8_t g, uint8_t b) {
    AaruarduinoError e = aaruarduinoValidatePwmPin(r);
    if (e != AARUARDUINO_OK) return setErr(e);
    e = aaruarduinoValidatePwmPin(g);
    if (e != AARUARDUINO_OK) return setErr(e);
    e = aaruarduinoValidatePwmPin(b);
    if (e != AARUARDUINO_OK) return setErr(e);
    _rgb_pins[0] = r; _rgb_pins[1] = g; _rgb_pins[2] = b;
    _rgb_setup = true;
    return _pwm.beginRGB(r, g, b);
}
AaruarduinoError AaruarduinoClass::setRGB(uint8_t r, uint8_t g, uint8_t b) {
    return _pwm.setRGB(r, g, b);
}
AaruarduinoError AaruarduinoClass::setRGBHSV(uint16_t h, uint8_t s, uint8_t v) {
    return _pwm.setHSV(h, s, v);
}
AaruarduinoError AaruarduinoClass::rgbOff() { return _pwm.rgbOff(); }

AaruarduinoError AaruarduinoClass::pwmOut(uint8_t pin) {
    AaruarduinoError e = aaruarduinoValidatePwmPin(pin);
    if (e != AARUARDUINO_OK) return setErr(e);
    _pwm_pin = pin;
    return _pwm.begin(pin);
}
AaruarduinoError AaruarduinoClass::writePWM(uint8_t val) { return _pwm.write(val); }
AaruarduinoError AaruarduinoClass::writePWMPercent(float pct) { return _pwm.writePercent(pct); }

AaruarduinoError AaruarduinoClass::motor(uint8_t in1, uint8_t in2, int en_pin) {
    AaruarduinoError e = aaruarduinoValidateDigitalPin(in1, true);
    if (e != AARUARDUINO_OK) return setErr(e);
    e = aaruarduinoValidateDigitalPin(in2, true);
    if (e != AARUARDUINO_OK) return setErr(e);
    if (en_pin >= 0) {
        e = aaruarduinoValidatePwmPin(en_pin);
        if (e != AARUARDUINO_OK) return setErr(e);
    }
    _motor_in1 = in1; _motor_in2 = in2; _motor_en = en_pin;
    pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
    if (en_pin >= 0) pinMode(en_pin, OUTPUT);
    return _pwm.beginMotor(in1, in2, en_pin);
}
AaruarduinoError AaruarduinoClass::motorSpeed(int speed) { return _pwm.motorSpeed(speed); }
AaruarduinoError AaruarduinoClass::motorBrake() { return _pwm.motorBrake(); }
AaruarduinoError AaruarduinoClass::motorCoast() { return _pwm.motorCoast(); }

AaruarduinoError AaruarduinoClass::servo(uint8_t pin, int min_us, int max_us) {
    AaruarduinoError e = aaruarduinoValidateDigitalPin(pin, true);
    if (e != AARUARDUINO_OK) return setErr(e);
    _servo_pin = pin;
    return _pwm.beginServo(pin, min_us, max_us);
}
AaruarduinoError AaruarduinoClass::servoWrite(int angle) { return _pwm.servoWrite(angle); }
AaruarduinoError AaruarduinoClass::servoWriteUs(uint16_t us) { return _pwm.servoWriteUs(us); }
AaruarduinoError AaruarduinoClass::servoDetach() { return _pwm.servoDetach(); }

AaruarduinoError AaruarduinoClass::stepper(uint8_t step, uint8_t dir, int en) {
    return _stepper.begin(step, dir, en);
}
AaruarduinoError AaruarduinoClass::stepper4Wire(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4) {
    return _stepper.begin4Wire(in1, in2, in3, in4);
}
AaruarduinoError AaruarduinoClass::stepperSetRpm(double rpm) {
    _stepper.setRpm(rpm);
    return AARUARDUINO_OK;
}
AaruarduinoError AaruarduinoClass::stepperStep(long steps) { return _stepper.step(steps); }
AaruarduinoError AaruarduinoClass::stepperMove(long steps) { _stepper.move(steps); return AARUARDUINO_OK; }
AaruarduinoError AaruarduinoClass::stepperMoveTo(long pos) { _stepper.moveTo(pos); return AARUARDUINO_OK; }
bool AaruarduinoClass::stepperRun() { return _stepper.run(); }
void AaruarduinoClass::stepperStop() { _stepper.stop(); }
AaruarduinoError AaruarduinoClass::stepperRelease() { _stepper.disable(); return AARUARDUINO_OK; }

// ---- GPS ----
AaruarduinoError AaruarduinoClass::gps(int rx, int tx, uint32_t baud) {
    _gps_setup = true;
    return _gps.begin(rx, tx, baud);
}
void AaruarduinoClass::gpsUpdate() { if (_gps_setup) _gps.update(); }
bool AaruarduinoClass::gpsFix() { return _gps.hasFix(); }
float AaruarduinoClass::gpsLat() { return _gps.lat(); }
float AaruarduinoClass::gpsLon() { return _gps.lon(); }
float AaruarduinoClass::gpsAlt() { return _gps.alt(); }
float AaruarduinoClass::gpsSpeedKmph() { return _gps.speedKmph(); }
float AaruarduinoClass::gpsCourse() { return _gps.course(); }
uint8_t AaruarduinoClass::gpsSats() { return _gps.sats(); }
uint32_t AaruarduinoClass::gpsDate() { return _gps.date(); }
uint32_t AaruarduinoClass::gpsTime() { return _gps.time(); }

// ---- IR Remote ----
AaruarduinoError AaruarduinoClass::irRecv(uint8_t pin, IRProtocol proto) { return _ir.begin(pin, proto); }
AaruarduinoError AaruarduinoClass::irSend(uint8_t pin) {
    _ir_send_setup = true;
    return _ir.beginSend(pin);
}
bool AaruarduinoClass::irDecode(AaruIRResult* result) { return _ir.decode(result); }
void AaruarduinoClass::irResume() { _ir.resume(); }
AaruarduinoError AaruarduinoClass::irSendNEC(uint32_t data, uint8_t nbits) { return _ir.sendNEC(data, nbits); }
AaruarduinoError AaruarduinoClass::irSendSony(uint32_t data, uint8_t nbits) { return _ir.sendSony(data, nbits); }
AaruarduinoError AaruarduinoClass::irSendRaw(const uint16_t* buf, uint16_t len, uint16_t freq_khz) {
    return _ir.sendRaw(buf, len, freq_khz);
}

// ---- Pin Helpers ----
bool AaruarduinoClass::isValidDigitalPin(uint8_t pin, bool need_output) {
    return aaruarduinoValidateDigitalPin(pin, need_output) == AARUARDUINO_OK;
}
bool AaruarduinoClass::isValidAnalogPin(int pin) {
    return aaruarduinoValidateAnalogPin(pin) == AARUARDUINO_OK;
}
bool AaruarduinoClass::isValidPwmPin(uint8_t pin) {
    return aaruarduinoValidatePwmPin(pin) == AARUARDUINO_OK;
}
AaruarduinoError AaruarduinoClass::i2cScan(uint8_t* addrs, uint8_t max_addrs) {
    if (!_i2c.isBegun()) _i2c.begin();
    return _i2c.scanList(addrs, max_addrs) ? AARUARDUINO_OK : AARUARDUINO_ERROR;
}
uint8_t AaruarduinoClass::i2cScanCount() {
    if (!_i2c.isBegun()) _i2c.begin();
    return _i2c.scanCount();
}

// ---- Private helpers ----
AaruarduinoError AaruarduinoClass::cfgAnalog(uint8_t pin, AaruAnalogKind kind) {
    AaruarduinoError e = aaruarduinoValidateAnalogPin(pin);
    if (e != AARUARDUINO_OK) return setErr(e);
    return _aeng.begin(pin, kind);
}
AaruarduinoError AaruarduinoClass::cfgDigital(uint8_t pin, AaruDigitalKind kind, bool active_low, bool pullup) {
    AaruarduinoError e = aaruarduinoValidateDigitalPin(pin, false);
    if (e != AARUARDUINO_OK) return setErr(e);
    return _deng.begin(pin, kind, active_low, pullup);
}

// ---- Global singleton ----
AaruarduinoClass AARUARDUINO;

// ---- Board-specific aliases ----
AaruarduinoError ultrasonic_uno(uint8_t trig, uint8_t echo) { return AARUARDUINO.ultrasonic(trig, echo); }
float ultrasonic_uno() { return AARUARDUINO.readUltrasonic(); }
AaruarduinoError ultrasonic_esp32(uint8_t trig, uint8_t echo) { return AARUARDUINO.ultrasonic(trig, echo); }
float ultrasonic_esp32() { return AARUARDUINO.readUltrasonic(); }
AaruarduinoError ultrasonic_esp8266(uint8_t trig, uint8_t echo) { return AARUARDUINO.ultrasonic(trig, echo); }
float ultrasonic_esp8266() { return AARUARDUINO.readUltrasonic(); }