// ============================================================
//  AARUARDUINO PWM Engine
//  MADE BY AARUTO
// ============================================================
#include "AaruPWMEngine.h"

AaruarduinoError AaruPWMEngine::begin(uint8_t pin) {
    _pin = pin; _configured = true; _mode = 1; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    pinMode(pin, OUTPUT);
    analogWrite(pin, 0);
#endif
    return _err;
}

AaruarduinoError AaruPWMEngine::write(uint8_t value) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
#if defined(ARDUINO)
    analogWrite(_pin, value);
#endif
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruPWMEngine::writePercent(float pct) {
    if (pct < 0) pct = 0; if (pct > 100) pct = 100;
    return write((uint8_t)(pct * 255.0f / 100.0f));
}

AaruarduinoError AaruPWMEngine::writeUs(uint16_t microseconds) {
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruPWMEngine::setFrequency(uint32_t hz) {
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruPWMEngine::setResolution(uint8_t bits) {
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruPWMEngine::beginRGB(uint8_t r, uint8_t g, uint8_t b) {
    _rgb_pins[0] = r; _rgb_pins[1] = g; _rgb_pins[2] = b;
    _mode = 2; _configured = true; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    pinMode(r, OUTPUT); pinMode(g, OUTPUT); pinMode(b, OUTPUT);
    analogWrite(r, 0); analogWrite(g, 0); analogWrite(b, 0);
#endif
    return _err;
}

AaruarduinoError AaruPWMEngine::setRGB(uint8_t r, uint8_t g, uint8_t b) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
#if defined(ARDUINO)
    analogWrite(_rgb_pins[0], r);
    analogWrite(_rgb_pins[1], g);
    analogWrite(_rgb_pins[2], b);
#endif
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruPWMEngine::setRGBPercent(float r, float g, float b) {
    return setRGB((uint8_t)(r * 2.55f), (uint8_t)(g * 2.55f), (uint8_t)(b * 2.55f));
}

AaruarduinoError AaruPWMEngine::setHSV(uint16_t h, uint8_t s, uint8_t v) {
    uint8_t r, g, b;
    uint16_t region = h / 60;
    uint16_t remainder = (h % 60) * 256 / 60;
    uint8_t p = (uint8_t)((v * (255 - s)) >> 8);
    uint8_t q = (uint8_t)((v * (255 - ((s * remainder) >> 8))) >> 8);
    uint8_t t = (uint8_t)((v * (255 - ((s * (255 - remainder)) >> 8))) >> 8);
    switch (region) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        default: r = v; g = p; b = q; break;
    }
    return setRGB(r, g, b);
}

AaruarduinoError AaruPWMEngine::rgbOff() {
    return setRGB(0, 0, 0);
}

AaruarduinoError AaruPWMEngine::beginBuzzer(uint8_t pin) {
    _buzzer_pin = pin; _mode = 3; _configured = true; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    pinMode(pin, OUTPUT);
#endif
    return _err;
}

AaruarduinoError AaruPWMEngine::tone(uint16_t freq_hz, unsigned long duration_ms) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
#if defined(ARDUINO)
    ::tone(_buzzer_pin != 255 ? _buzzer_pin : _pin, freq_hz, duration_ms);
#endif
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruPWMEngine::noTone() {
#if defined(ARDUINO)
    ::noTone(_buzzer_pin != 255 ? _buzzer_pin : _pin);
#endif
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruPWMEngine::beginServo(uint8_t pin, int min_us, int max_us) {
    _servo_pin = pin; _servo_min_us = min_us; _servo_max_us = max_us;
    _mode = 4; _configured = true; _servo_attached = false; _err = AARUARDUINO_OK;
#if defined(ARDUINO) && AARUARDUINO_HAS_SERVO
    _servo.attach(pin, min_us, max_us);
    _servo_attached = true;
#endif
    return _err;
}

AaruarduinoError AaruPWMEngine::servoWrite(int angle) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
#if defined(ARDUINO) && AARUARDUINO_HAS_SERVO
    if (_servo_attached) _servo.write(angle);
#endif
    _servo_angle = angle;
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruPWMEngine::servoWriteUs(uint16_t us) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
#if defined(ARDUINO) && AARUARDUINO_HAS_SERVO
    if (_servo_attached) _servo.writeMicroseconds(us);
#endif
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruPWMEngine::servoDetach() {
#if defined(ARDUINO) && AARUARDUINO_HAS_SERVO
    if (_servo_attached) { _servo.detach(); _servo_attached = false; }
#endif
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruPWMEngine::beginMotor(uint8_t in1, uint8_t in2, int en_pin) {
    _motor_in1 = in1; _motor_in2 = in2; _motor_en = (en_pin >= 0) ? (uint8_t)en_pin : 255;
    _motor_en_pwm = (en_pin >= 0); _mode = 5; _configured = true; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
    if (en_pin >= 0) pinMode(en_pin, OUTPUT);
    digitalWrite(in1, LOW); digitalWrite(in2, LOW);
    if (en_pin >= 0) analogWrite(en_pin, 0);
#endif
    return _err;
}

AaruarduinoError AaruPWMEngine::motorSpeed(int speed) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    if (speed > 255) speed = 255; if (speed < -255) speed = -255;
    _motor_speed = speed;
#if defined(ARDUINO)
    if (speed >= 0) {
        digitalWrite(_motor_in1, HIGH); digitalWrite(_motor_in2, LOW);
        if (_motor_en != 255) analogWrite(_motor_en, speed);
    } else {
        digitalWrite(_motor_in1, LOW); digitalWrite(_motor_in2, HIGH);
        if (_motor_en != 255) analogWrite(_motor_en, -speed);
    }
#endif
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruPWMEngine::motorBrake() {
#if defined(ARDUINO)
    digitalWrite(_motor_in1, HIGH); digitalWrite(_motor_in2, HIGH);
    if (_motor_en != 255) analogWrite(_motor_en, 255);
#endif
    _motor_speed = 0;
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruPWMEngine::motorCoast() {
#if defined(ARDUINO)
    digitalWrite(_motor_in1, LOW); digitalWrite(_motor_in2, LOW);
    if (_motor_en != 255) analogWrite(_motor_en, 0);
#endif
    _motor_speed = 0;
    _err = AARUARDUINO_OK;
    return _err;
}
