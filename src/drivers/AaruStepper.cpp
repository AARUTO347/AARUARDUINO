// ============================================================
//  AARUARDUINO Stepper Driver
//  MADE BY AARUTO
// ============================================================
#include "AaruStepper.h"

AaruarduinoError AaruStepper::begin(uint8_t step_pin, uint8_t dir_pin, int en_pin) {
    _step_pin = step_pin; _dir_pin = dir_pin; _en_pin = (en_pin >= 0) ? (uint8_t)en_pin : 255;
    _type = STEPPER_TYPE_2WIRE; _configured = true; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    pinMode(_step_pin, OUTPUT);
    pinMode(_dir_pin, OUTPUT);
    digitalWrite(_step_pin, LOW);
    digitalWrite(_dir_pin, _dir_high ? HIGH : LOW);
    if (_en_pin != 255) {
        pinMode(_en_pin, OUTPUT);
        enablePin(true);
    }
#endif
    return _err;
}

AaruarduinoError AaruStepper::begin(uint8_t step_pin, uint8_t dir_pin, uint8_t en_pin, StepperType type) {
    return begin(step_pin, dir_pin, (int)en_pin);
}

AaruarduinoError AaruStepper::begin4Wire(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4) {
    _in1 = in1; _in2 = in2; _in3 = in3; _in4 = in4;
    _type = STEPPER_TYPE_4WIRE; _configured = true; _err = AARUARDUINO_OK;
#if defined(ARDUINO)
    pinMode(_in1, OUTPUT); pinMode(_in2, OUTPUT);
    pinMode(_in3, OUTPUT); pinMode(_in4, OUTPUT);
    digitalWrite(_in1, LOW); digitalWrite(_in2, LOW);
    digitalWrite(_in3, LOW); digitalWrite(_in4, LOW);
#endif
    return _err;
}

void AaruStepper::setStepsPerRev(int steps) { _steps_per_rev = steps; }
void AaruStepper::setMicrosteps(uint8_t ms) { _microsteps = ms; }
void AaruStepper::setRpm(double rpm) { _rpm = rpm; updateInterval(); }
void AaruStepper::setAcceleration(double accel) { _accel = accel; }
void AaruStepper::setMaxSpeed(double speed) { _max_speed = speed; }
void AaruStepper::enable() { _enabled = true; enablePin(_enabled); }
void AaruStepper::disable() { _enabled = false; enablePin(_enabled); }

AaruarduinoError AaruStepper::step(long steps) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    _dir = (steps >= 0) ? 1 : -1;
    dirPin(_dir > 0);
    long steps_abs = (steps < 0) ? -steps : steps;
    for (long i = 0; i < steps_abs; i++) {
        stepPin(HIGH);
        delayMicroseconds(_step_interval / 2);
        stepPin(LOW);
        delayMicroseconds(_step_interval / 2);
        _pos += _dir;
    }
    _err = AARUARDUINO_OK;
    return _err;
}

AaruarduinoError AaruStepper::moveTo(long position) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    long steps = position - _pos;
    return step(steps);
}

AaruarduinoError AaruStepper::rotate(double degrees) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    long steps = (long)((degrees / 360.0) * _steps_per_rev * _microsteps);
    return step(steps);
}

AaruarduinoError AaruStepper::rotateTo(double degrees) {
    if (!_configured) { _err = AARUARDUINO_NOT_INITIALIZED; return _err; }
    long target = (long)((degrees / 360.0) * _steps_per_rev * _microsteps);
    return moveTo(target);
}

void AaruStepper::move(long steps) {
    step(steps);
}

bool AaruStepper::run() {
    if (!_configured || !_running) return false;
    unsigned long now = micros();
    if (now - _last_step >= _step_interval) {
        stepPin(HIGH);
        delayMicroseconds(_step_interval / 2);
        stepPin(LOW);
        _last_step = now;
        _pos += _dir;
        if (_pos == _target) {
            _running = false;
        }
    }
    return _running;
}

bool AaruStepper::runSpeed() {
    if (!_configured || _speed == 0) return false;
    unsigned long now = micros();
    unsigned long interval = (unsigned long)(1000000.0 / fabs(_speed));
    if (now - _last_step >= interval) {
        stepPin(HIGH);
        delayMicroseconds(interval / 2);
        stepPin(LOW);
        _last_step = now;
        _pos += (_speed > 0) ? 1 : -1;
    }
    return true;
}

void AaruStepper::stop() { _running = false; }
void AaruStepper::setCurrentPosition(long pos) { _pos = pos; _target = pos; }

long AaruStepper::currentPosition() const { return _pos; }
long AaruStepper::targetPosition() const { return _target; }
bool AaruStepper::isRunning() const { return _running; }
double AaruStepper::currentSpeed() const { return _speed; }
double AaruStepper::maxSpeed() const { return _max_speed; }
bool AaruStepper::isConfigured() const { return _configured; }
AaruarduinoError AaruStepper::lastError() const { return _err; }
const char* AaruStepper::errorString() const { return aaruarduinoErrorString(_err); }

void AaruStepper::stepPin(bool level) {
    if (_type == STEPPER_TYPE_4WIRE) {
        static const uint8_t sequence[4][4] = {
            {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}
        };
        static int step_idx = 0;
        if (level) {
            step_idx = (step_idx + ((_dir > 0) ? 1 : 3)) & 3;
            digitalWrite(_in1, sequence[step_idx][0]);
            digitalWrite(_in2, sequence[step_idx][1]);
            digitalWrite(_in3, sequence[step_idx][2]);
            digitalWrite(_in4, sequence[step_idx][3]);
        }
    } else {
        digitalWrite(_step_pin, level ? HIGH : LOW);
    }
}

void AaruStepper::dirPin(bool level) {
    if (_type != STEPPER_TYPE_4WIRE) {
        digitalWrite(_dir_pin, level ? HIGH : LOW);
    }
}

void AaruStepper::enablePin(bool on) {
    if (_en_pin != 255) {
        digitalWrite(_en_pin, on ? LOW : HIGH);
    }
}

void AaruStepper::updateInterval() {
    if (_rpm > 0 && _steps_per_rev > 0) {
        double steps_per_sec = (_rpm * _steps_per_rev * _microsteps) / 60.0;
        _step_interval = (unsigned long)(1000000.0 / steps_per_sec);
    } else {
        _step_interval = 1000;
    }
}
