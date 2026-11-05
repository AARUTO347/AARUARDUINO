# AARUARDUINO API Reference
## MADE BY AARUTO

Full function list, units, and error codes. Singleton usage:

```cpp
#include <AARUARDUINO.h>
void setup() { AARUARDUINO.begin(); }
void loop() { AARUARDUINO.update(); }
```

## Core

| Function | Description |
|---|---|
| `begin()` | Initialize library and I²C bus |
| `update()` | Call in `loop()` for non-blocking drivers |
| `version()` | Returns `"1.0.0"` |
| `madeBy()` | Returns `"MADE BY AARUTO"` |
| `credits()` | Full version + attribution string |
| `boardFamily()` | `AaruarduinoFamily` enum |
| `boardName()` | e.g. `"Uno R3"`, `"ESP32"` |
| `lastError()` | Last `AaruarduinoError` |
| `clearError()` | Reset error to `OK` |
| `errorString(e)` | Human-readable error message |
| `setDebugStream(Stream*)` | Enable debug output |

## Ultrasonic (HC-SR04)

`ultrasonic(trig, echo)` / `readUltrasonic()` (cm) / `readUltrasonicInches()` / `setUltrasonicTimeout(us)`

## DHT11 / DHT22

`dht11(pin)` / `dht22(pin)` / `readTemperature()` (°C) / `readHumidity()` (%) / `readTemperatureF()` / `readDHT(temp, hum)` / `dhtFresh()` / `dhtAgeMs()`

## Analog engine (one active kind)

`analogSensor(pin, kind)` / kind presets: `ldr`, `potentiometer`, `lm35`, `tmp36`, `thermistor(pin,seriesR,beta,r0,t0C)`, `soilMoisture`, `rainSensor`, `waterLevel`, `soundSensor`, `forceSensor`, `flameAnalog`, MQ: `mq2`…`mq138`, `genericMQ(pin,r0,a,b)`, current: `acs712_5A`, `acs712_20A`, `acs712_30A`, `acs712(pin,mvPerA,zeroMv)`, `voltageDivider(pin,ratio)`, `acVoltageZMPT101B(pin)`

Reads: `readAnalogRaw()`, `readAnalogPercent()`, `readAnalogVoltage()`, `readAnalogValue()`, `readLDR()`, `readPotentiometer()`, `readTemperatureAnalog()`, `readGas()`, `readCurrent()`, `readSoil()`, `readRain()`, `readWaterLevel()`, `readSound()`, `readForce()`, `readFlameAnalog()`, `readAnalogPin(pin)`, `readVoltagePin(pin,divider)`

Params: `setThermistorParams()`, `setCurrentParams()`, `setMQParams()`, `setAnalogSamples(n)`, `setAnalogSettleUs(us)`

## Digital engine (one active kind)

`digitalSensor(pin, kind, activeLow, pullup)` / presets: `button`, `pir`, `radarRCWL`, `irObstacle`, `lineFollower`, `flameSensor`, `smokeDigital`, `touchTTP223`, `hallDigital`, `reedSwitch`, `limitSwitch`, `vibrationSW420`, `tiltSwitch`, `shockSW18010`, `pulseCounter`, `flowSensor`, `tachometer`, `keypad4x4`, `irReceiver`

Reads: `readDigitalSensor()`, `readPIR()`, `readRadar()`, `readIR()`, `readLine()`, `readFlame()`, `readSmoke()`, `readTouch()`, `readHall()`, `readReed()`, `readLimit()`, `readVibration()`, `readTilt()`, `readShock()`, `readButton()`, `buttonPressed()`, `buttonRose()`, `buttonFell()`, `pulseCount()`, `resetPulseCount()`, `readFrequencyHz()`, `readFlowRateLmin()`, `readIRNEC()`, `irDataReady()`, `readKey()`

## Encoder / Joystick

`rotaryEncoder(clk, dt, sw)` / `encoderCount()` / `encoderReset()` / `encoderDirection()` / `encoderButton()` / `joystick(x, y, sw)` / `readJoyX()` / `readJoyY()` / `readJoyButton()`

## MPU6050 IMU

`mpu6050(addr=0x68)` / `readAccel(x,y,z)` (g) / `readGyro(x,y,z)` (deg/s) / `readIMUTemp()` (°C) / `calibrateAccel(samples)` / `calibrateGyro(samples)` / `mpuSleep()` / `mpuWake()`

## DS18B20 (1-Wire)

`ds18b20(pin)` / `ds18b20Count()` / `readDS18B20(index)` (°C) / `readDS18B20F(index)` / `ds18b20Request()` / `ds18b20Ready()` / `ds18b20SetResolution(index, bits)`

## DS3231 RTC

`rtcDS3231(addr=0x68)` / `rtcSet(y,mo,d,h,mi,s)` / `rtcGet(...)` / `rtcUnixTime()` / `rtcTemp()` (°C) / `rtcAlarm1(...)` / `rtcAlarm2(...)` / `rtcAlarmTriggered(n)` / `rtcSquareWave(freq)`

## BME280 / BMP280

`bme280(addr=0x76)` / `bmp280(addr=0x76)` / `readBME(temp,hum,press)` / `readBMEtemp()` / `readBMEhumidity()` / `readBMEpressure()` / `readBMEaltitude(seaLevel=1013.25)`

## BH1750 Light

`bh1750(addr=0x23)` / `readLux()`

## Displays

`oled(addr=0x3C, w=128, h=64)` / `lcdI2C(addr=0x27, cols=16, rows=2)` / `lcd(addr, cols, rows)` / `max7219(cs, devices=1)` / `tm1637(clk, dio)` / `ws2812(pin, numLeds)`

Helpers: `displayClear()`, `displayText(line)`, `displayText(row, line)`, `displayTextAt(row, col, line)`, `displayBacklight(on)`, `displayPrint(n|f)`, `displaySetCursor(col,row)`, `displayContrast(val)`, `displayInvert(inv)`

## Actuators

LED: `led(pin)` / `setLED(on)` / `toggleLED()` / `setBrightness(0-255)`
Buzzer: `buzzer(pin)` / `beep(ms)` / `beep(freq, ms)` / `tone(freq, ms)` / `noTone()` / `buzzerOff()`
Relay: `relay(pin, activeHigh)` / `setRelay(on)` / `toggleRelay()`
Solenoid: `solenoid(pin, activeHigh)` / `setSolenoid(on)`
Laser: `laser(pin)` / `setLaser(on)`
RGB: `rgb(r,g,b)` / `setRGB(r,g,b)` / `setRGBHSV(h,s,v)` / `rgbOff()`
PWM: `pwmOut(pin)` / `writePWM(0-255)` / `writePWMPercent(pct)`
Motor: `motor(in1,in2,en=-1)` / `motorSpeed(-255..255)` / `motorBrake()` / `motorCoast()`
Servo: `servo(pin, minUs=544, maxUs=2400)` / `servoWrite(0-180)` / `servoWriteUs(us)` / `servoDetach()`
Stepper: `stepper(step, dir, en=-1)` / `stepper4Wire(in1..in4)` / `stepperSetRpm(rpm)` / `stepperStep(steps)` / `stepperMove(steps)` / `stepperMoveTo(pos)` / `stepperRun()` / `stepperStop()` / `stepperRelease()`

## GPS (NMEA)

`gps(rx, tx, baud=9600)` / `gpsUpdate()` / `gpsFix()` / `gpsLat()` / `gpsLon()` / `gpsAlt()` / `gpsSpeedKmph()` / `gpsCourse()` / `gpsSats()` / `gpsDate()` / `gpsTime()`

## IR Remote

`irRecv(pin, proto=NEC)` / `irSend(pin)` / `irDecode(&result)` / `irResume()` / `irSendNEC(data, nbits=32)` / `irSendSony(data, nbits=12)` / `irSendRaw(buf, len, freqKhz=38)`

## Pin helpers

`isValidDigitalPin(pin, needOutput)` / `isValidAnalogPin(pin)` / `isValidPwmPin(pin)` / `i2cScan(addrs, max)` / `i2cScanCount()`

## Error codes

`OK, ERROR, INVALID_PIN, INVALID_CONFIG, TIMEOUT, READ_FAIL, WRITE_FAIL, UNSUPPORTED, MISSING_DEP, INIT_FAIL, NOT_INITIALIZED, BUSY, BUFFER_OVERFLOW, CRC_ERROR, NOT_FOUND`

Failures return `NaN` (float), `-1` (int) or `false` (bool) plus a code in `lastError()`.
