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
    bool _configured = false;
    int _type = 0;
    uint8_t _step_pin = 0, _dir_pin = 0, _en_pin = 0;
    uint8_t _in1 = 0, _in2 = 0, _in3 = 0, _in4 = 0;
    int _steps_per_rev = 0;
    uint8_t _microsteps = 0;
    double _rpm = 0.0;
    double _max_speed = 0.0;
    double _accel = 0.0;
    bool _enabled = false;
    bool _dir_high = false;

    long _pos = 0;
    long _target = 0;
    double _speed = 0.0;
    bool _running = false;
    unsigned long _last_step = 0;
    unsigned long _step_interval = 0;
    int8_t _dir = 0;
    AaruarduinoError _err = AARUARDUINO_OK;

    void stepPin(bool level);
    void dirPin(bool level);
    void enablePin(bool on);
    void updateInterval();
};

#endif // AARUARDUINO_STEPPER_H
