#!/usr/bin/env python3
"""
AARUARDUINO Keywords Generator
MADE BY AARUTO
Generates keywords.txt from registry for Arduino IDE syntax highlighting.
Run: python tools/gen_keywords.py
"""
import csv
import os
import re

REGISTRY_PATH = os.path.join(os.path.dirname(__file__), '..', 'registry', 'modules.csv')
OUTPUT_PATH = os.path.join(os.path.dirname(__file__), '..', 'keywords.txt')

CORE_KEYWORDS = [
    ('AARUARDUINO', 'KEYWORD1'), ('begin', 'KEYWORD2'), ('update', 'KEYWORD2'),
    ('version', 'KEYWORD2'), ('madeBy', 'KEYWORD2'), ('credits', 'KEYWORD2'),
    ('boardFamily', 'KEYWORD2'), ('boardName', 'KEYWORD2'), ('lastError', 'KEYWORD2'),
    ('clearError', 'KEYWORD2'), ('errorString', 'KEYWORD2'), ('setDebugStream', 'KEYWORD2'),
    ('ultrasonic', 'KEYWORD2'), ('setUltrasonicTimeout', 'KEYWORD2'),
    ('readUltrasonic', 'KEYWORD2'), ('readUltrasonicInches', 'KEYWORD2'),
    ('dht11', 'KEYWORD2'), ('dht22', 'KEYWORD2'), ('dht', 'KEYWORD2'),
    ('readTemperature', 'KEYWORD2'), ('readHumidity', 'KEYWORD2'), ('readTemperatureF', 'KEYWORD2'),
    ('readDHT', 'KEYWORD2'), ('dhtFresh', 'KEYWORD2'), ('dhtAgeMs', 'KEYWORD2'),
    ('analogSensor', 'KEYWORD2'), ('ldr', 'KEYWORD2'), ('potentiometer', 'KEYWORD2'),
    ('lm35', 'KEYWORD2'), ('tmp36', 'KEYWORD2'), ('thermistor', 'KEYWORD2'),
    ('soilMoisture', 'KEYWORD2'), ('rainSensor', 'KEYWORD2'), ('waterLevel', 'KEYWORD2'),
    ('soundSensor', 'KEYWORD2'), ('forceSensor', 'KEYWORD2'), ('flameAnalog', 'KEYWORD2'),
    ('mq2', 'KEYWORD2'), ('mq3', 'KEYWORD2'), ('mq4', 'KEYWORD2'), ('mq5', 'KEYWORD2'),
    ('mq6', 'KEYWORD2'), ('mq7', 'KEYWORD2'), ('mq8', 'KEYWORD2'), ('mq9', 'KEYWORD2'),
    ('mq131', 'KEYWORD2'), ('mq135', 'KEYWORD2'), ('mq136', 'KEYWORD2'),
    ('mq137', 'KEYWORD2'), ('mq138', 'KEYWORD2'), ('genericMQ', 'KEYWORD2'),
    ('acs712_5A', 'KEYWORD2'), ('acs712_20A', 'KEYWORD2'), ('acs712_30A', 'KEYWORD2'),
    ('acs712', 'KEYWORD2'), ('voltageDivider', 'KEYWORD2'),
    ('acVoltageZMPT101B', 'KEYWORD2'), ('readAnalogRaw', 'KEYWORD2'),
    ('readAnalogPercent', 'KEYWORD2'), ('readAnalogVoltage', 'KEYWORD2'),
    ('readAnalogValue', 'KEYWORD2'), ('readLDR', 'KEYWORD2'),
    ('readPotentiometer', 'KEYWORD2'), ('readTemperatureAnalog', 'KEYWORD2'),
    ('readGas', 'KEYWORD2'), ('readCurrent', 'KEYWORD2'), ('readSoil', 'KEYWORD2'),
    ('readRain', 'KEYWORD2'), ('readWaterLevel', 'KEYWORD2'), ('readSound', 'KEYWORD2'),
    ('readForce', 'KEYWORD2'), ('readFlameAnalog', 'KEYWORD2'),
    ('readAnalogPin', 'KEYWORD2'), ('readVoltagePin', 'KEYWORD2'),
    ('setThermistorParams', 'KEYWORD2'), ('setCurrentParams', 'KEYWORD2'),
    ('setMQParams', 'KEYWORD2'), ('setAnalogSamples', 'KEYWORD2'),
    ('setAnalogSettleUs', 'KEYWORD2'), ('digitalSensor', 'KEYWORD2'),
    ('button', 'KEYWORD2'), ('pir', 'KEYWORD2'), ('radarRCWL', 'KEYWORD2'),
    ('irObstacle', 'KEYWORD2'), ('lineFollower', 'KEYWORD2'), ('flameSensor', 'KEYWORD2'),
    ('smokeDigital', 'KEYWORD2'), ('touchTTP223', 'KEYWORD2'), ('hallDigital', 'KEYWORD2'),
    ('reedSwitch', 'KEYWORD2'), ('limitSwitch', 'KEYWORD2'), ('vibrationSW420', 'KEYWORD2'),
    ('tiltSwitch', 'KEYWORD2'), ('shockSW18010', 'KEYWORD2'), ('pulseCounter', 'KEYWORD2'),
    ('flowSensor', 'KEYWORD2'), ('tachometer', 'KEYWORD2'), ('keypad4x4', 'KEYWORD2'),
    ('irReceiver', 'KEYWORD2'), ('readDigitalSensor', 'KEYWORD2'), ('readPIR', 'KEYWORD2'),
    ('readRadar', 'KEYWORD2'), ('readIR', 'KEYWORD2'), ('readLine', 'KEYWORD2'),
    ('readFlame', 'KEYWORD2'), ('readSmoke', 'KEYWORD2'), ('readTouch', 'KEYWORD2'),
    ('readHall', 'KEYWORD2'), ('readReed', 'KEYWORD2'), ('readLimit', 'KEYWORD2'),
    ('readVibration', 'KEYWORD2'), ('readTilt', 'KEYWORD2'), ('readShock', 'KEYWORD2'),
    ('readButton', 'KEYWORD2'), ('buttonPressed', 'KEYWORD2'), ('buttonRose', 'KEYWORD2'),
    ('buttonFell', 'KEYWORD2'), ('pulseCount', 'KEYWORD2'), ('resetPulseCount', 'KEYWORD2'),
    ('readFrequencyHz', 'KEYWORD2'), ('readFlowRateLmin', 'KEYWORD2'),
    ('readIRNEC', 'KEYWORD2'), ('irDataReady', 'KEYWORD2'), ('readKey', 'KEYWORD2'),
    ('rotaryEncoder', 'KEYWORD2'), ('encoderCount', 'KEYWORD2'), ('encoderReset', 'KEYWORD2'),
    ('encoderDirection', 'KEYWORD2'), ('encoderButton', 'KEYWORD2'),
    ('joystick', 'KEYWORD2'), ('readJoyX', 'KEYWORD2'), ('readJoyY', 'KEYWORD2'),
    ('readJoyButton', 'KEYWORD2'), ('mpu6050', 'KEYWORD2'), ('readAccel', 'KEYWORD2'),
    ('readGyro', 'KEYWORD2'), ('readIMUTemp', 'KEYWORD2'), ('readAccelX', 'KEYWORD2'),
    ('readAccelY', 'KEYWORD2'), ('readAccelZ', 'KEYWORD2'), ('readGyroX', 'KEYWORD2'),
    ('readGyroY', 'KEYWORD2'), ('readGyroZ', 'KEYWORD2'), ('calibrateAccel', 'KEYWORD2'),
    ('calibrateGyro', 'KEYWORD2'), ('mpuSleep', 'KEYWORD2'), ('mpuWake', 'KEYWORD2'),
    ('ds18b20', 'KEYWORD2'), ('ds18b20Count', 'KEYWORD2'), ('readDS18B20', 'KEYWORD2'),
    ('readDS18B20F', 'KEYWORD2'), ('ds18b20Request', 'KEYWORD2'), ('ds18b20Ready', 'KEYWORD2'),
    ('ds18b20SetResolution', 'KEYWORD2'), ('rtcDS3231', 'KEYWORD2'), ('rtcSet', 'KEYWORD2'),
    ('rtcGet', 'KEYWORD2'), ('rtcUnixTime', 'KEYWORD2'), ('rtcTemp', 'KEYWORD2'),
    ('rtcAlarm1', 'KEYWORD2'), ('rtcAlarm2', 'KEYWORD2'), ('rtcAlarmTriggered', 'KEYWORD2'),
    ('rtcSquareWave', 'KEYWORD2'), ('bme280', 'KEYWORD2'), ('bmp280', 'KEYWORD2'),
    ('readBME', 'KEYWORD2'), ('readBMEtemp', 'KEYWORD2'), ('readBMEhumidity', 'KEYWORD2'),
    ('readBMEpressure', 'KEYWORD2'), ('readBMEaltitude', 'KEYWORD2'),
    ('bh1750', 'KEYWORD2'), ('readLux', 'KEYWORD2'), ('oled', 'KEYWORD2'),
    ('lcdI2C', 'KEYWORD2'), ('lcd', 'KEYWORD2'), ('max7219', 'KEYWORD2'),
    ('tm1637', 'KEYWORD2'), ('ws2812', 'KEYWORD2'), ('displayClear', 'KEYWORD2'),
    ('displayText', 'KEYWORD2'), ('displayTextAt', 'KEYWORD2'), ('displayBacklight', 'KEYWORD2'),
    ('displayPrint', 'KEYWORD2'), ('displaySetCursor', 'KEYWORD2'), ('displayContrast', 'KEYWORD2'),
    ('displayInvert', 'KEYWORD2'), ('led', 'KEYWORD2'), ('setLED', 'KEYWORD2'),
    ('toggleLED', 'KEYWORD2'), ('setBrightness', 'KEYWORD2'), ('buzzer', 'KEYWORD2'),
    ('beep', 'KEYWORD2'), ('tone', 'KEYWORD2'), ('noTone', 'KEYWORD2'),
    ('buzzerOff', 'KEYWORD2'), ('relay', 'KEYWORD2'), ('setRelay', 'KEYWORD2'),
    ('toggleRelay', 'KEYWORD2'), ('solenoid', 'KEYWORD2'), ('setSolenoid', 'KEYWORD2'),
    ('laser', 'KEYWORD2'), ('setLaser', 'KEYWORD2'), ('rgb', 'KEYWORD2'),
    ('setRGB', 'KEYWORD2'), ('setRGBHSV', 'KEYWORD2'), ('rgbOff', 'KEYWORD2'),
    ('pwmOut', 'KEYWORD2'), ('writePWM', 'KEYWORD2'), ('writePWMPercent', 'KEYWORD2'),
    ('motor', 'KEYWORD2'), ('motorSpeed', 'KEYWORD2'), ('motorBrake', 'KEYWORD2'),
    ('motorCoast', 'KEYWORD2'), ('servo', 'KEYWORD2'), ('servoWrite', 'KEYWORD2'),
    ('servoWriteUs', 'KEYWORD2'), ('servoDetach', 'KEYWORD2'), ('stepper', 'KEYWORD2'),
    ('stepper4Wire', 'KEYWORD2'), ('stepperSetRpm', 'KEYWORD2'), ('stepperStep', 'KEYWORD2'),
    ('stepperMove', 'KEYWORD2'), ('stepperMoveTo', 'KEYWORD2'), ('stepperRun', 'KEYWORD2'),
    ('stepperStop', 'KEYWORD2'), ('stepperRelease', 'KEYWORD2'), ('gps', 'KEYWORD2'),
    ('gpsUpdate', 'KEYWORD2'), ('gpsFix', 'KEYWORD2'), ('gpsLat', 'KEYWORD2'),
    ('gpsLon', 'KEYWORD2'), ('gpsAlt', 'KEYWORD2'), ('gpsSpeedKmph', 'KEYWORD2'),
    ('gpsCourse', 'KEYWORD2'), ('gpsSats', 'KEYWORD2'), ('gpsDate', 'KEYWORD2'),
    ('gpsTime', 'KEYWORD2'), ('irRecv', 'KEYWORD2'), ('irSend', 'KEYWORD2'),
    ('irDecode', 'KEYWORD2'), ('irResume', 'KEYWORD2'), ('irSendNEC', 'KEYWORD2'),
    ('irSendSony', 'KEYWORD2'), ('irSendRaw', 'KEYWORD2'),
    ('isValidDigitalPin', 'KEYWORD2'), ('isValidAnalogPin', 'KEYWORD2'),
    ('isValidPwmPin', 'KEYWORD2'), ('i2cScan', 'KEYWORD2'), ('i2cScanCount', 'KEYWORD2'),
    ('ultrasonic_uno', 'KEYWORD2'), ('ultrasonic_esp32', 'KEYWORD2'),
    ('ultrasonic_esp8266', 'KEYWORD2'),
    ('AARUARDUINO_OK', 'LITERAL1'), ('AARUARDUINO_ERROR', 'LITERAL1'),
    ('AARUARDUINO_INVALID_PIN', 'LITERAL1'), ('AARUARDUINO_INVALID_CONFIG', 'LITERAL1'),
    ('AARUARDUINO_TIMEOUT', 'LITERAL1'), ('AARUARDUINO_READ_FAIL', 'LITERAL1'),
    ('AARUARDUINO_WRITE_FAIL', 'LITERAL1'), ('AARUARDUINO_UNSUPPORTED', 'LITERAL1'),
    ('AARUARDUINO_MISSING_DEP', 'LITERAL1'), ('AARUARDUINO_INIT_FAIL', 'LITERAL1'),
    ('AARUARDUINO_NOT_INITIALIZED', 'LITERAL1'), ('AARUARDUINO_BUSY', 'LITERAL1'),
    ('AARUARDUINO_BUFFER_OVERFLOW', 'LITERAL1'), ('AARUARDUINO_CRC_ERROR', 'LITERAL1'),
    ('AARUARDUINO_NOT_FOUND', 'LITERAL1'),
]


def load_registry():
    with open(REGISTRY_PATH, 'r', encoding='utf-8') as f:
        return list(csv.DictReader(f))


def load_public_api():
    """Read the public methods straight out of the main header so the keyword
    list can never drift away from the real API."""
    path = os.path.join(os.path.dirname(__file__), '..', 'src', 'AARUARDUINO.h')
    with open(path, 'r', encoding='utf-8') as f:
        text = f.read()

    # Only the public part of AaruarduinoClass.
    m = re.search(r'class\s+AaruarduinoClass\s*\{', text)
    if not m:
        return []
    body = text[m.end():]
    body = body.split('\nprivate:', 1)[0]

    names = []
    for line in body.split('\n'):
        line = line.strip()
        if not line.endswith(';'):
            continue
        decl = line[:-1].strip()
        if '(' not in decl:
            continue
        head, _, args = decl.partition('(')
        # Default arguments may contain '=' - only look before the parameter list.
        if '=' in head:
            continue
        head = head.strip()
        if not head:
            continue
        name = head.split()[-1].lstrip('*&')
        if not name or not name[0].isalpha() or not name[0].islower():
            continue
        names.append(name)
    return names


def main():
    rows = load_registry()
    keywords = set()
    for kw, ktype in CORE_KEYWORDS:
        keywords.add((kw, ktype))
    for row in rows:
        if row['status'].strip().upper() == 'IMPLEMENTED':
            api = row['api_function'].strip()
            if api:
                keywords.add((api, 'KEYWORD2'))
    # Every public method of the singleton, straight from the header.
    for name in load_public_api():
        keywords.add((name, 'KEYWORD2'))
    with open(OUTPUT_PATH, 'w', encoding='utf-8') as f:
        f.write("# AARUARDUINO keywords.txt - MADE BY AARUTO\n")
        f.write("# Auto-generated from registry/modules.csv and src/AARUARDUINO.h\n\n")
        for kw, ktype in sorted(keywords):
            f.write(f"{kw}\t{ktype}\n")
    print(f"Generated {OUTPUT_PATH} with {len(keywords)} keywords")


if __name__ == '__main__':
    main()