// ============================================================
//  AARUARDUINO Stepper Driver (A4988, DRV8825, TB6600, etc.)
//  MADE BY AARUTO
// ============================================================
#ifndef AARUARDUINO_STEPPER_H
#define AARUARDUINO_STEPPER_H

#include "AARUARDUINOTypes.h"
#include "AARUARDUINOBoard.h"
#include "utility/AaruarduinoMath.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
#endif

typedef enum {
    STEPPER_TYPE_2WIRE = 0,
    STEPPER_TYPE_3WIRE = 1,
    STEPPER_TYPE_4WIRE = 2,
} StepperType;

class AaruStepper {
public:
    AaruStepper() = default;

    AaruarduinoError begin(uint8_t step_pin, uint8_t dir_pin, int en_pin = -1);
    AaruarduinoError begin(uint8_t step_pin, uint8_t dir_pin, uint8_t en_pin, StepperType type);
    AaruarduinoError begin4Wire(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4);

    void setStepsPerRev(int steps);
    void setMicrosteps(uint8_t ms);
    void setRpm(double rpm);
    void setAcceleration(double accel);
    void setMaxSpeed(double speed);
    void enable();
    void disable();

    AaruarduinoError step(long steps);
    AaruarduinoError moveTo(long position);
    AaruarduinoError rotate(double degrees);
    AaruarduinoError rotateTo(double degrees);

    void move(long steps);
    bool run();
    bool runSpeed();
    void stop();
    void setCurrentPosition(long pos);

    long currentPosition() const;
    long targetPosition() const;
    bool isRunning() const;
    double currentSpeed() const;
    double maxSpeed() const;
    bool isConfigured() const;
    AaruarduinoError lastError() const;
    const char* errorString() const;

private:
    bool _configured;
    int _type;
    uint8_t _step_pin, _dir_pin, _en_pin;
    uint8_t _in1, _in2, _in3, _in4;
    int _steps_per_rev;
    uint8_t _microsteps;
    double _rpm;
    double _max_speed;
    double _accel;
    bool _enabled;
    bool _dir_high;

    long _pos;
    long _target;
    double _speed;
    bool _running;
    unsigned long _last_step;
    unsigned long _step_interval;
    int8_t _dir;
    AaruarduinoError _err;

    void stepPin(bool level);
    void dirPin(bool level);
    void enablePin(bool on);
    void updateInterval();
};

#endif // AARUARDUINO_STEPPER_H
