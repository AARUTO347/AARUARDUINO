// ============================================================
//  AARUARDUINO PWM Engine
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_PWM_ENGINE_H
#define AARUARDUINO_PWM_ENGINE_H

#include "AARUARDUINOTypes.h"
#include "AARUARDUINOBoard.h"

#if defined(ARDUINO)
#include <Arduino.h>
#if __has_include(<Servo.h>)
  #include <Servo.h>
  #define AARUARDUINO_HAS_SERVO 1
#else
  #define AARUARDUINO_HAS_SERVO 0
#endif
#else
#include <stdint.h>
#define AARUARDUINO_HAS_SERVO 0
#endif

class AaruPWMEngine {
public:
    AaruPWMEngine() = default;

    AaruarduinoError begin(uint8_t pin);
    AaruarduinoError write(uint8_t value);
    AaruarduinoError writePercent(float pct);
    AaruarduinoError writeUs(uint16_t microseconds);

    AaruarduinoError setFrequency(uint32_t hz);
    AaruarduinoError setResolution(uint8_t bits);

    AaruarduinoError beginRGB(uint8_t r, uint8_t g, uint8_t b);
    AaruarduinoError setRGB(uint8_t r, uint8_t g, uint8_t b);
    AaruarduinoError setRGBPercent(float r, float g, float b);
    AaruarduinoError setHSV(uint16_t h, uint8_t s, uint8_t v);
    AaruarduinoError rgbOff();

    AaruarduinoError beginBuzzer(uint8_t pin);
    AaruarduinoError tone(uint16_t freq_hz, unsigned long duration_ms = 0);
    AaruarduinoError noTone();

    AaruarduinoError beginServo(uint8_t pin, int min_us = 544, int max_us = 2400);
    AaruarduinoError servoWrite(int angle);
    AaruarduinoError servoWriteUs(uint16_t us);
    AaruarduinoError servoDetach();

    AaruarduinoError beginMotor(uint8_t in1, uint8_t in2, int en_pin);
    AaruarduinoError motorSpeed(int speed);
    AaruarduinoError motorBrake();
    AaruarduinoError motorCoast();

    bool isConfigured() const;
    uint8_t getPin() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _configured = false;
    uint8_t _pin = 0;
    uint8_t _mode = 0;
    AaruarduinoError _err = AARUARDUINO_OK;

    uint8_t _rgb_pins[3] = {};
    uint8_t _buzzer_pin = 0;

    uint8_t _motor_in1 = 0, _motor_in2 = 0, _motor_en = 0;
    bool _motor_en_pwm = false;
    int _motor_speed = 0;

#if defined(ARDUINO) && AARUARDUINO_HAS_SERVO
    class Servo _servo;
#endif
    uint8_t _servo_pin = 0;
    int _servo_min_us = 0;
    int _servo_max_us = 0;
    int _servo_angle = 0;
    bool _servo_attached = false;
};

#endif // AARUARDUINO_PWM_ENGINE_H
