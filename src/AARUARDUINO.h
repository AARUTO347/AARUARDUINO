// ============================================================
//  AARUARDUINO v1.0.0 — Universal Arduino Library
//  MADE BY AARUTO
//  1000+ modules, one API.
// ============================================================
#ifndef AARUARDUINO_H
#define AARUARDUINO_H

#include "AARUARDUINOTypes.h"
#include "AARUARDUINOConfig.h"
#include "AARUARDUINOBoard.h"
#include "engines/AaruAnalogEngine.h"
#include "engines/AaruDigitalEngine.h"
#include "engines/AaruPWMEngine.h"
#include "engines/AaruI2CEngine.h"
#include "engines/AaruSPIEngine.h"
#include "engines/AaruUARTEngine.h"
#include "engines/AaruOneWireEngine.h"
#include "engines/AaruPulseEngine.h"
#include "drivers/AaruHCSR04.h"
#include "drivers/AaruDHT.h"
#include "drivers/AaruMPU6050.h"
#include "drivers/AaruDS18B20.h"
#include "drivers/AaruDS3231.h"
#include "drivers/AaruBME280.h"
#include "drivers/AaruBH1750.h"
#include "drivers/AaruSSD1306.h"
#include "drivers/AaruLCD.h"
#include "drivers/AaruMAX7219.h"
#include "drivers/AaruTM1637.h"
#include "drivers/AaruWS2812.h"
#include "drivers/AaruGPS.h"
#include "drivers/AaruIRRemote.h"
#include "drivers/AaruStepper.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <stdint.h>
class Stream {
public:
    virtual size_t print(const char*) { return 0; }
    virtual size_t println(const char*) { return 0; }
    virtual size_t write(uint8_t) { return 0; }
};
#endif

class AaruarduinoClass {
public:
    AaruarduinoClass();

    // ---- Core ----
    void begin();
    void update();
    void setDebugStream(Stream* s) { _dbg = s; }
    const char* version() const { return AARUARDUINO_VERSION_STRING; }
    const char* madeBy() const { return "MADE BY AARUTO"; }
    const char* credits() const { return "AARUARDUINO v" AARUARDUINO_VERSION_STRING " — MADE BY AARUTO"; }
    AaruarduinoFamily boardFamily() const;
    const char* boardName() const;
    AaruarduinoError lastError() const { return _err; }
    void clearError() { _err = AARUARDUINO_OK; }
    const char* errorString() const;
    static const char* errorString(AaruarduinoError e);

    // ---- Shared bus setup (I2C / SPI / UART) ----
    AaruarduinoError i2cBegin(int sda = -1, int scl = -1, uint32_t speed_hz = 400000);
    AaruarduinoError spiBegin(int sck = -1, int miso = -1, int mosi = -1);
    AaruarduinoError uartBegin(int uart_num, uint32_t baud, int rx = -1, int tx = -1);
    bool i2cDevicePresent(uint8_t addr);
    int i2cScanFirst();

    // ---- Ultrasonic ----
    AaruarduinoError ultrasonic(uint8_t trig_pin, uint8_t echo_pin);
    AaruarduinoError setUltrasonicTimeout(unsigned long timeout_us);
    float readUltrasonic();
    bool readUltrasonic(float& cm);
    float readUltrasonicInches();

    // ---- DHT ----
    AaruarduinoError dht11(uint8_t pin);
    AaruarduinoError dht22(uint8_t pin);
    AaruarduinoError dht(uint8_t pin, AaruDHTType type);
    float readTemperature();
    float readHumidity();
    float readTemperatureF();
    bool readDHT(float& temp_c, float& hum_pct);
    bool dhtFresh() const;
    unsigned long dhtAgeMs() const;

    // ---- Analog engine ----
    AaruarduinoError analogSensor(uint8_t pin, AaruAnalogKind kind);
    AaruarduinoError ldr(uint8_t pin);
    AaruarduinoError potentiometer(uint8_t pin);
    AaruarduinoError lm35(uint8_t pin);
    AaruarduinoError tmp36(uint8_t pin);
    AaruarduinoError thermistor(uint8_t pin, float series_r, float beta, float r0, float t0_c);
    AaruarduinoError soilMoisture(uint8_t pin);
    AaruarduinoError rainSensor(uint8_t pin);
    AaruarduinoError waterLevel(uint8_t pin);
    AaruarduinoError soundSensor(uint8_t pin);
    AaruarduinoError forceSensor(uint8_t pin);
    AaruarduinoError flameAnalog(uint8_t pin);
    // MQ Gas sensors
    AaruarduinoError mq2(uint8_t pin); AaruarduinoError mq3(uint8_t pin);
    AaruarduinoError mq4(uint8_t pin); AaruarduinoError mq5(uint8_t pin);
    AaruarduinoError mq6(uint8_t pin); AaruarduinoError mq7(uint8_t pin);
    AaruarduinoError mq8(uint8_t pin); AaruarduinoError mq9(uint8_t pin);
    AaruarduinoError mq131(uint8_t pin); AaruarduinoError mq135(uint8_t pin);
    AaruarduinoError mq136(uint8_t pin); AaruarduinoError mq137(uint8_t pin);
    AaruarduinoError mq138(uint8_t pin);
    AaruarduinoError genericMQ(uint8_t pin, float r0, float a, float b);
    // Current / voltage
    AaruarduinoError acs712_5A(uint8_t pin); AaruarduinoError acs712_20A(uint8_t pin);
    AaruarduinoError acs712_30A(uint8_t pin);
    AaruarduinoError acs712(uint8_t pin, float mv_per_amp, float zero_mv);
    AaruarduinoError voltageDivider(uint8_t pin, float ratio);
    AaruarduinoError acVoltageZMPT101B(uint8_t pin);
    // Read helpers
    int readAnalogRaw();
    float readAnalogPercent();
    float readAnalogVoltage();
    float readAnalogValue();
    int readLDR();
    int readPotentiometer();
    float readTemperatureAnalog();
    float readGas();
    float readCurrent();
    float readSoil();
    float readRain();
    float readWaterLevel();
    float readSound();
    float readForce();
    float readFlameAnalog();
    int readAnalogPin(uint8_t pin);
    float readVoltagePin(uint8_t pin, float divider = 1.0f);

    // Analog param overrides
    void setThermistorParams(float series_r, float beta, float r0, float t0_c);
    void setCurrentParams(float mv_per_amp, float zero_mv);
    void setMQParams(float r0, float a, float b);
    void setAnalogSamples(uint8_t n) { _aeng.setSamples(n); }
    void setAnalogSettleUs(uint16_t us) { _aeng.setSettleUs(us); }

    // ---- Digital engine ----
    AaruarduinoError digitalSensor(uint8_t pin, AaruDigitalKind kind, bool active_low = false, bool pullup = false);
    AaruarduinoError button(uint8_t pin, bool active_low = true, bool pullup = true);
    AaruarduinoError pir(uint8_t pin);
    AaruarduinoError radarRCWL(uint8_t pin);
    AaruarduinoError irObstacle(uint8_t pin);
    AaruarduinoError lineFollower(uint8_t pin);
    AaruarduinoError flameSensor(uint8_t pin);
    AaruarduinoError smokeDigital(uint8_t pin);
    AaruarduinoError touchTTP223(uint8_t pin);
    AaruarduinoError hallDigital(uint8_t pin);
    AaruarduinoError reedSwitch(uint8_t pin);
    AaruarduinoError limitSwitch(uint8_t pin, bool pullup = true, bool active_low = true);
    AaruarduinoError vibrationSW420(uint8_t pin);
    AaruarduinoError tiltSwitch(uint8_t pin);
    AaruarduinoError shockSW18010(uint8_t pin);
    AaruarduinoError pulseCounter(uint8_t pin);
    AaruarduinoError flowSensor(uint8_t pin, float pulses_per_litre);
    AaruarduinoError tachometer(uint8_t pin, float pulses_per_rev);
    AaruarduinoError keypad4x4(uint8_t r0, uint8_t r1, uint8_t r2, uint8_t r3,
                               uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3);
    AaruarduinoError irReceiver(uint8_t pin, IRProtocol proto = IR_PROTO_NEC);

    bool readDigitalSensor();
    bool readPIR();
    bool readRadar();
    bool readIR();
    bool readLine();
    bool readFlame();
    bool readSmoke();
    bool readTouch();
    bool readHall();
    bool readReed();
    bool readLimit();
    bool readVibration();
    bool readTilt();
    bool readShock();
    int readButton();
    bool buttonPressed();
    bool buttonRose();
    bool buttonFell();
    unsigned long pulseCount();
    void resetPulseCount();
    float readFrequencyHz();
    float readFlowRateLmin();
    uint32_t readIRNEC();
    bool irDataReady();
    char readKey();

    // ---- Rotary Encoder ----
    AaruarduinoError rotaryEncoder(uint8_t clk, uint8_t dt, int sw_pin = -1);
    long encoderCount();
    void encoderReset();
    int8_t encoderDirection();
    bool encoderButton();

    // ---- Joystick ----
    AaruarduinoError joystick(uint8_t x_pin, uint8_t y_pin, int sw_pin = -1);
    int readJoyX();
    int readJoyY();
    bool readJoyButton();

    // ---- MPU6050 ----
    AaruarduinoError mpu6050(uint8_t addr = 0x68);
    bool readAccel(float& x, float& y, float& z);
    bool readGyro(float& x, float& y, float& z);
    float readIMUTemp();
    float readAccelX(); float readAccelY(); float readAccelZ();
    float readGyroX();  float readGyroY();  float readGyroZ();
    void calibrateAccel(int samples = 200);
    void calibrateGyro(int samples = 200);
    AaruarduinoError mpuSleep();
    AaruarduinoError mpuWake();

    // ---- DS18B20 ----
    AaruarduinoError ds18b20(uint8_t pin);
    int ds18b20Count();
    float readDS18B20(int index = 0);
    float readDS18B20F(int index = 0);
    AaruarduinoError ds18b20Request();
    bool ds18b20Ready();
    float readDS18B20Async(int index);
    AaruarduinoError ds18b20SetResolution(int index, uint8_t bits);

    // ---- DS3231 RTC ----
    AaruarduinoError rtcDS3231(uint8_t addr = 0x68);
    AaruarduinoError rtcSet(int year, int month, int day, int hour, int min, int sec);
    AaruarduinoError rtcGet(int& year, int& month, int& day, int& hour, int& min, int& sec);
    uint32_t rtcUnixTime();
    float rtcTemp();
    AaruarduinoError rtcAlarm1(int sec, int min, int hour, int day, bool dow);
    AaruarduinoError rtcAlarm2(int min, int hour, int day, bool dow);
    bool rtcAlarmTriggered(int alarm_num);
    AaruarduinoError rtcSquareWave(uint8_t freq);

    // ---- BME280/BMP280 ----
    AaruarduinoError bme280(uint8_t addr = 0x76);
    AaruarduinoError bmp280(uint8_t addr = 0x76);
    bool readBME(float& temp, float& hum, float& press);
    float readBMEtemp();
    float readBMEhumidity();
    float readBMEpressure();
    float readBMEaltitude(float sea_level = 1013.25f);

    // ---- BH1750 ----
    AaruarduinoError bh1750(uint8_t addr = 0x23);
    float readLux();

    // ---- Displays ----
    AaruarduinoError oled(uint8_t addr = 0x3C, uint8_t w = 128, uint8_t h = 64);
    AaruarduinoError lcdI2C(uint8_t addr = 0x27, uint8_t cols = 16, uint8_t rows = 2);
    AaruarduinoError lcd(uint8_t addr, uint8_t cols, uint8_t rows);
    AaruarduinoError max7219(uint8_t cs, uint8_t devices = 1);
    AaruarduinoError tm1637(uint8_t clk, uint8_t dio);
    AaruarduinoError ws2812(uint8_t pin, uint16_t num_leds);

    AaruarduinoError displayClear();
    AaruarduinoError displayText(const char* line);
    AaruarduinoError displayText(int row, const char* line);
    AaruarduinoError displayTextAt(int row, int col, const char* line);
    AaruarduinoError displayBacklight(bool on);
    AaruarduinoError displayPrint(int n, int base = 10);
    AaruarduinoError displayPrint(unsigned int n, int base = 10);
    AaruarduinoError displayPrint(long n, int base = 10);
    AaruarduinoError displayPrint(unsigned long n, int base = 10);
    AaruarduinoError displayPrint(float f, int digits);
    AaruarduinoError displaySetCursor(int col, int row);
    AaruarduinoError displayContrast(uint8_t val);
    AaruarduinoError displayInvert(bool inv);
    // TM1637 / MAX7219 numeric helpers
    AaruarduinoError displayNumber(int num, bool leading_zeros = false);
    AaruarduinoError displayFloat(float num, uint8_t decimals);

    // ---- WS2812 / NeoPixel ----
    AaruarduinoError ws2812Set(uint16_t index, uint8_t r, uint8_t g, uint8_t b);
    AaruarduinoError ws2812Fill(uint8_t r, uint8_t g, uint8_t b);
    AaruarduinoError ws2812Clear();
    AaruarduinoError ws2812Rainbow(uint16_t first_hue = 0, uint16_t hue_step = 256);
    AaruarduinoError ws2812Show();
    AaruarduinoError ws2812Brightness(uint8_t b);

    // ---- Actuators ----
    AaruarduinoError led(uint8_t pin);
    AaruarduinoError setLED(bool on);
    AaruarduinoError toggleLED();
    AaruarduinoError setBrightness(uint8_t val);

    AaruarduinoError buzzer(uint8_t pin);
    AaruarduinoError beep(unsigned long duration_ms);
    AaruarduinoError beep(uint16_t freq_hz, unsigned long duration_ms);
    AaruarduinoError tone(uint16_t freq_hz, unsigned long duration_ms = 0);
    AaruarduinoError noTone();
    AaruarduinoError buzzerOff();

    AaruarduinoError relay(uint8_t pin, bool active_high = true);
    AaruarduinoError setRelay(bool on);
    AaruarduinoError toggleRelay();
    AaruarduinoError solenoid(uint8_t pin, bool active_high = true);
    AaruarduinoError setSolenoid(bool on);

    AaruarduinoError laser(uint8_t pin);
    AaruarduinoError setLaser(bool on);

    AaruarduinoError rgb(uint8_t r, uint8_t g, uint8_t b);
    AaruarduinoError setRGB(uint8_t r, uint8_t g, uint8_t b);
    AaruarduinoError setRGBHSV(uint16_t h, uint8_t s, uint8_t v);
    AaruarduinoError rgbOff();

    AaruarduinoError pwmOut(uint8_t pin);
    AaruarduinoError writePWM(uint8_t val);
    AaruarduinoError writePWMPercent(float pct);

    AaruarduinoError motor(uint8_t in1, uint8_t in2, int en_pin = -1);
    AaruarduinoError motorSpeed(int speed);
    AaruarduinoError motorBrake();
    AaruarduinoError motorCoast();

    AaruarduinoError servo(uint8_t pin, int min_us = 544, int max_us = 2400);
    AaruarduinoError servoWrite(int angle);
    AaruarduinoError servoWriteUs(uint16_t us);
    AaruarduinoError servoDetach();

    AaruarduinoError stepper(uint8_t step, uint8_t dir, int en = -1);
    AaruarduinoError stepper4Wire(uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4);
    AaruarduinoError stepperSetRpm(double rpm);
    AaruarduinoError stepperStep(long steps);
    AaruarduinoError stepperMove(long steps);
    AaruarduinoError stepperMoveTo(long pos);
    bool stepperRun();
    void stepperStop();
    AaruarduinoError stepperRelease();

    // ---- GPS ----
    AaruarduinoError gps(int rx, int tx, uint32_t baud = 9600);
    void gpsUpdate();
    bool gpsFix();
    float gpsLat();
    float gpsLon();
    float gpsAlt();
    float gpsSpeedKmph();
    float gpsCourse();
    uint8_t gpsSats();
    uint32_t gpsDate();
    uint32_t gpsTime();

    // ---- IR Remote ----
    AaruarduinoError irRecv(uint8_t pin, IRProtocol proto = IR_PROTO_NEC);
    AaruarduinoError irSend(uint8_t pin);
    bool irDecode(AaruIRResult* result);
    void irResume();
    AaruarduinoError irSendNEC(uint32_t data, uint8_t nbits = 32);
    AaruarduinoError irSendSony(uint32_t data, uint8_t nbits = 12);
    AaruarduinoError irSendRaw(const uint16_t* buf, uint16_t len, uint16_t freq_khz = 38);

    // ---- Pin Helpers ----
    bool isValidDigitalPin(uint8_t pin, bool need_output = false);
    bool isValidAnalogPin(int pin);
    bool isValidPwmPin(uint8_t pin);
    AaruarduinoError i2cScan(uint8_t* addrs, uint8_t max_addrs);
    uint8_t i2cScanCount();

private:
    void dbg(const char* msg);
    AaruarduinoError setErr(AaruarduinoError e) { _err = e; return e; }
    AaruarduinoError cfgAnalog(uint8_t pin, AaruAnalogKind kind);
    AaruarduinoError cfgDigital(uint8_t pin, AaruDigitalKind kind, bool active_low = false, bool pullup = false);

    bool _begun = false;
    AaruarduinoError _err = AARUARDUINO_OK;
    Stream* _dbg = nullptr;

    AaruAnalogEngine _aeng;
    AaruDigitalEngine _deng;
    AaruPWMEngine _pwm;
    AaruI2CEngine _i2c;
    AaruSPIEngine _spi;
    AaruUARTEngine _uart;
    AaruOneWireEngine _ow;
    AaruPulseEngine _pulse;

    AaruHCSR04 _sonic;
    AaruDHT _dht;
    AaruMPU6050 _mpu;
    AaruDS18B20 _ds;
    AaruDS3231 _rtc;
    AaruBME280 _bme;
    AaruBH1750 _bh;
    AaruSSD1306 _oled;
    AaruLCD _lcd;
    AaruMAX7219 _max7219;
    AaruTM1637 _tm1637;
    AaruWS2812 _ws2812;
    AaruGPS _gps;
    AaruIRRemote _ir;
    AaruStepper _stepper;

    int _led_pin = -1;
    int _buzzer_pin = -1;
    int _relay_pin = -1;
    int _solenoid_pin = -1;
    int _laser_pin = -1;
    int _pwm_pin = -1;
    int _motor_in1 = -1, _motor_in2 = -1, _motor_en = -1;
    int _servo_pin = -1;
    uint8_t _rgb_pins[3] = {255, 255, 255};
    bool _relay_active_high = true;
    bool _solenoid_active_high = true;
    bool _led_state = false;
    bool _relay_state = false;
    bool _solenoid_state = false;
    bool _rgb_setup = false;

    bool _gps_setup = false;
    bool _ir_send_setup = false;

    // Active display routing (0=none 1=oled 2=lcd 3=max7219 4=tm1637)
    uint8_t _display_kind = 0;
};

extern AaruarduinoClass AARUARDUINO;

// ---- Board-specific aliases ----
AaruarduinoError ultrasonic_uno(uint8_t trig, uint8_t echo);
float ultrasonic_uno();
AaruarduinoError ultrasonic_esp32(uint8_t trig, uint8_t echo);
float ultrasonic_esp32();
AaruarduinoError ultrasonic_esp8266(uint8_t trig, uint8_t echo);
float ultrasonic_esp8266();

#endif // AARUARDUINO_H
