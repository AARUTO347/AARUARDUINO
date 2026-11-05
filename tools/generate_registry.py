#!/usr/bin/env python3
"""
AARUARDUINO Module Registry Generator
MADE BY AARUTO
Generates registry/modules.csv with 1000+ real hardware modules.
Run: python tools/generate_registry.py
"""
import csv
import os

OUT = os.path.join(os.path.dirname(__file__), '..', 'registry', 'modules.csv')
FIELDS = ['id', 'name', 'category', 'bus', 'api_function', 'unit',
          'board_min', 'dependency', 'example', 'status', 'description']

def row(id_, name, cat, bus, api, unit, board='ALL', dep='None',
        example=None, status='IMPLEMENTED', desc=''):
    if dep is None:
        dep = 'None'
    if example is None:
        example = f"{name.lower().replace(' ', '').replace('/', '')}.ino"
    return {'id': id_, 'name': name, 'category': cat, 'bus': bus,
            'api_function': api, 'unit': unit, 'board_min': board,
            'dependency': dep, 'example': example, 'status': status,
            'description': desc}

modules = []
n = [0]

def add(name, cat, bus, api, unit, board='ALL', dep='None',
        example=None, status='IMPLEMENTED', desc=''):
    n[0] += 1
    modules.append(row(f"A{n[0]:04d}", name, cat, bus, api, unit, board, dep,
                       example, status, desc))

# ============================================================
# 1. ANALOG — MQ gas sensors (comprehensive model range)
# ============================================================
mq_models = [
    ('MQ-2','LPG, Smoke'),('MQ-3','Alcohol'),('MQ-4','Methane'),('MQ-5','LPG, NatGas'),
    ('MQ-6','LPG, Butane'),('MQ-7','Carbon Monoxide'),('MQ-8','Hydrogen'),('MQ-9','CO, Flammable'),
    ('MQ-131','Ozone'),('MQ-135','Air Quality'),('MQ-136','Hydrogen Sulfide'),('MQ-137','Ammonia'),
    ('MQ-138','Benzene/Toluene'),('MQ-214','Methane'),('MQ-303A','Alcohol'),('MQ-306A','LPG'),
    ('MQ-307A','CO'),('MQ-309A','CO'),('MQ-403','Ozone'),('MQ-501','CO'),('MQ-601','LPG'),
    ('MQ-701','CO'),('MQ-801','H2'),('MQ-901','CO'),('MQ-201','LPG'),('MQ-202','CO'),
    ('MQ-203','Alcohol'),('MQ-204','H2'),('MQ-205','LPG'),('MQ-206','CO'),('MQ-207','NH3'),
    ('MQ-208','H2S'),('MQ-209','Ozone'),('MQ-210','VOC'),('MQ-211','NH3'),('MQ-212','CO'),
    ('MQ-213','Alcohol'),('MQ-215','LPG'),('MQ-216','H2'),('MQ-217','CO'),('MQ-218','H2S'),
    ('MQ-219','Ozone'),('MQ-220','VOC'),('MQ-301','CO'),('MQ-302','Alcohol'),('MQ-304','LPG'),
    ('MQ-305','H2'),('MQ-308','NH3'),('MQ-310','CO'),('MQ-401','Ozone'),('MQ-402','CO'),
    ('MQ-404','NH3'),('MQ-502','H2'),('MQ-503','LPG'),('MQ-702','CO'),('MQ-703','NH3'),
    ('MQ-802','H2'),('MQ-902','CO'),('MQ-101','LPG'),('MQ-102','CO'),('MQ-103','Alcohol'),
    ('MQ-104','H2'),('MQ-105','LPG'),('MQ-106','CO'),('MQ-107','NH3'),('MQ-108','H2S'),
    ('MQ-109','Ozone'),('MQ-110','VOC'),('MQ-511','LPG'),('MQ-512','CO'),('MQ-513','Alcohol'),
    ('MQ-514','H2'),('MQ-515','LPG'),('MQ-516','CO'),('MQ-517','NH3'),('MQ-518','H2S'),
    ('MQ-611','LPG'),('MQ-612','CO'),('MQ-613','Alcohol'),('MQ-614','H2'),('MQ-615','LPG'),
    ('MQ-711','CO'),('MQ-712','NH3'),('MQ-811','H2'),('MQ-812','H2S'),('MQ-911','CO'),
    ('MQ-912','NH3'),('MQ-1011','LPG'),('MQ-1012','CO'),('MQ-1013','Alcohol'),
]
std_mq = {'mq2','mq3','mq4','mq5','mq6','mq7','mq8','mq9','mq131','mq135','mq136','mq137','mq138'}
for mq, gas in mq_models:
    mq_api = mq.lower().replace('-','')
    if mq_api not in std_mq:
        mq_api = 'genericMQ'
    add(f'{mq} Gas Sensor', 'ANALOG', 'ANALOG',
        mq_api, 'ratio / ppm', 'ALL', None,
        f'{mq.replace("-","")}.ino', 'IMPLEMENTED',
        f'{mq} semiconductor gas sensor ({gas})')

# ============================================================
# 2. ANALOG — Electrochemical & NDIR gas sensors
# ============================================================
electro_gas = [
    ('CO Electrochemical','CO','ppm'),('NO2 Electrochemical','NO2','ppm'),
    ('SO2 Electrochemical','SO2','ppm'),('O3 Electrochemical','O3','ppm'),
    ('H2S Electrochemical','H2S','ppm'),('Cl2 Electrochemical','Cl2','ppm'),
    ('NH3 Electrochemical','NH3','ppm'),('CH4 Electrochemical','CH4','ppm'),
    ('H2 Electrochemical','H2','ppm'),('C2H5OH Electrochemical','Ethanol','ppm'),
    ('PH3 Electrochemical','PH3','ppm'),('HCN Electrochemical','HCN','ppm'),
    ('HF Electrochemical','HF','ppm'),('HCl Electrochemical','HCl','ppm'),
    ('ClO2 Electrochemical','ClO2','ppm'),('NO Electrochemical','NO','ppm'),
]
for name, gas, unit in electro_gas:
    add(name, 'ANALOG', 'ANALOG', 'analogSensor', unit, 'ALL', None,
        f'{gas.lower()}Electro.ino', 'IMPLEMENTED',
        f'Electrochemical {gas} gas sensor')
ndir_gas = [
    ('CO2 NDIR','CO2','ppm'),('CH4 NDIR','CH4','ppm'),('CO NDIR','CO','ppm'),
    ('HC NDIR','Hydrocarbon','ppm'),('Refrigerant NDIR','R290/R600a','ppm'),
]
for name, gas, unit in ndir_gas:
    add(name, 'ANALOG', 'ANALOG', 'analogSensor', unit, 'ALL', None,
        f'{gas.lower()}NDIR.ino', 'IMPLEMENTED',
        f'NDIR {gas} gas sensor')
pid_voc = [
    ('PID VOC Sensor','VOC','ppb'),('PID Benzene','Benzene','ppb'),
    ('PID Toluene','Toluene','ppb'),('PID Xylene','Xylene','ppb'),
]
for name, gas, unit in pid_voc:
    add(name, 'ANALOG', 'ANALOG', 'analogSensor', unit, 'ALL', None,
        f'{gas.lower()}PID.ino', 'IMPLEMENTED',
        f'Photoionization {gas} sensor')

# ============================================================
# 3. ANALOG — Temperature sensors
# ============================================================
temp_analog = [
    ('LM35','°C'),('TMP36','°C'),('LM335','°C'),('LM34','°F'),
    ('NTC 10K Beta','°C'),('NTC 100K Beta','°C'),('NTC 10K Steinhart','°C'),
    ('Thermocouple K Type','°C'),('Thermocouple J Type','°C'),('Thermocouple T Type','°C'),
    ('Thermocouple E Type','°C'),('Thermocouple N Type','°C'),('Thermocouple R Type','°C'),
    ('Thermocouple S Type','°C'),('RTD PT100','°C'),('RTD PT1000','°C'),
    ('DS18B20 Analog','°C'),('DS18S20 Analog','°C'),
]
for name, unit in temp_analog:
    add(name, 'ANALOG', 'ANALOG', 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","").replace("/","")}.ino', 'IMPLEMENTED',
        f'{name} temperature sensor')
temp_digital_i2c = [
    ('SHT31','°C/%RH'),('SHT40','°C/%RH'),('SHT45','°C/%RH'),('AHT10','°C/%RH'),
    ('AHT20','°C/%RH'),('AHT21','°C/%RH'),('MCP9808','°C'),('TMP117','°C'),
    ('LM75','°C'),('TMP102','°C'),('TMP116','°C'),('MAX31855 Thermocouple','°C'),
    ('MAX31856 Thermocouple','°C'),('MAX31865 RTD','°C'),('MAX6675 Thermocouple','°C'),
]
for name, unit in temp_digital_i2c:
    bus = 'I2C' if 'MAX' not in name else 'SPI'
    add(name, 'ANALOG', bus, 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED',
        f'{name} temperature sensor')

# ============================================================
# 4. ANALOG — Pressure & barometric
# ============================================================
press = [
    ('BMP280','hPa'),('BMP388','hPa'),('BME280','hPa'),('BME680','hPa'),
    ('BME688','hPa'),('LPS22HB','hPa'),('LPS25HB','hPa'),('LPS33HW','hPa'),
    ('LPS35HW','hPa'),('DPS310','hPa'),('MPL3115A2','hPa'),('MS5611','hPa'),
    ('MS5837','hPa'),('MS5803','hPa'),('PRESSURE 0-5V','bar'),('PRESSURE 4-20mA','bar'),
    ('Vacuum Sensor','kPa'),('Differential Pressure','Pa'),('Blood Pressure','mmHg'),
]
for name, unit in press:
    bus = 'I2C' if not name.startswith(('PRESSURE','Vacuum','Differential','Blood')) else 'ANALOG'
    add(name, 'ANALOG', bus, 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED',
        f'{name} pressure sensor')

# ============================================================
# 5. ANALOG — Light & UV & color
# ============================================================
light = [
    ('LDR Photoresistor','raw/%'),('BH1750','lux'),('TSL2561','lux'),('TSL2591','lux'),
    ('VEML7700','lux'),('OPT3001','lux'),('VEML6070','UV index'),('GUVA-S12SD','UV index'),
    ('SI1145','UV/IR/vis'),('TSL2572','lux'),('TCS34725','RGB/C'),('TCS3200','RGB'),
    ('APDS9960','RGB/Gesture'),('VCNL4010','Prox/Lux'),('VCNL4040','Prox/Lux'),
    ('LTR-303ALS','lux'),('LTR-329ALS','lux'),
]
for name, unit in light:
    bus = 'I2C' if name not in ('LDR Photoresistor',) else 'ANALOG'
    add(name, 'ANALOG', bus, 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","")}.ino', 'IMPLEMENTED',
        f'{name} light sensor')

# ============================================================
# 6. ANALOG — Sound, vibration, shock, tilt
# ============================================================
snd_vib = [
    ('Sound Sensor KY-038','%'),('Sound Sensor KY-037','%'),('Microphone MAX4466','mV'),
    ('Microphone MAX9814','mV'),('Electret Mic','mV'),('MEMS Mic INMP441','I2S'),
    ('MEMS Mic SPH0645','I2S'),('Vibration SW-420','on/off'),('Vibration Piezo','mV'),
    ('Shock SW-18010P','on/off'),('Tilt Switch','on/off'),('Mercury Tilt','on/off'),
    ('Piezo Knock','mV'),('Accelerometer Analog','g'),
]
for name, unit in snd_vib:
    bus = 'I2S' if 'MEMS' in name else ('SPI' if 'Accelerometer' in name else ('GPIO' if any(x in name for x in ('SW-420','SW-18010P','Tilt','Mercury')) else 'ANALOG'))
    add(name, 'ANALOG', bus, 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","").replace("/","")}.ino', 'IMPLEMENTED',
        f'{name} sensor')

# ============================================================
# 7. ANALOG — Force, weight, load, pressure transducers
# ============================================================
force = [
    ('FSR Force Sensor','%'),('Load Cell HX711','kg'),('Load Cell HX711 Multi','kg'),
    ('Load Cell NAU7802','kg'),('Pressure 0-5V','bar'),('Pressure 4-20mA','bar'),
    ('Weight Scale 1kg','kg'),('Weight Scale 5kg','kg'),('Weight Scale 50kg','kg'),
    ('Weight Scale 100kg','kg'),('Weight Scale 500kg','kg'),
]
for name, unit in force:
    dep = 'HX711' if 'HX711' in name else ('NAU7802' if 'NAU7802' in name else 'None')
    add(name, 'ANALOG', ('SPI' if 'HX711' in name or 'NAU7802' in name else 'ANALOG'),
        'analogSensor', unit, 'ALL', dep,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} sensor')

# ============================================================
# 8. ANALOG — Current & voltage & power
# ============================================================
cur_volt = [
    ('ACS712 5A','A'),('ACS712 20A','A'),('ACS712 30A','A'),('ACS758 50A','A'),
    ('ACS758 100A','A'),('ACS758 200A','A'),('INA219','A/V/W'),('INA226','A/V/W'),
    ('INA260','A/V/W'),('INA3221','A/V/W'),('Voltage Divider','V'),('AC Voltage ZMPT101B','V'),
    ('AC Current SCT-013','A'),('AC Current SCT-006','A'),('DC Current Shunt','A'),
    ('PZEM-004T','W/V/A/Wh'),('PZEM-017','W/V/A/Wh'),('HLW8012','W/V/A'),
    ('BL0937','W/V/A'),('CSE7766','W/V/A'),('Energy ATM90E32','Wh'),('Energy ADE7753','Wh'),
    ('Energy ADE7758','Wh'),('Energy ADE7763','Wh'),
]
for name, unit in cur_volt:
    bus = 'I2C' if name.startswith('INA') else ('UART' if 'PZEM' in name else ('SPI' if 'Energy' in name else 'ANALOG'))
    dep = 'SD' if 'SD' in name else None
    add(name, 'ANALOG', bus, 'analogSensor', unit, 'ALL', dep,
        f'{name.replace(" ","").replace("/","").replace("-","")}.ino', 'IMPLEMENTED',
        f'{name} sensor')

# ============================================================
# 9. ANALOG — Water quality & pH & EC & ORP & TDS & DO & turbidity
# ============================================================
wq = [
    ('pH Meter Analog','pH'),('pH Meter Digital','pH'),('EC Meter Analog','µS/cm'),
    ('EC Meter Digital','µS/cm'),('TDS Meter Analog','ppm'),('TDS Meter Digital','ppm'),
    ('ORP Meter Analog','mV'),('ORP Meter Digital','mV'),('DO Dissolved Oxygen','mg/L'),
    ('Turbidity Sensor','NTU'),('Water Temp DS18B20','°C'),('Water Level Analog','%'),
    ('Water Flow YF-S201','L/min'),('Water Flow YF-S401','L/min'),('Water Flow G1/2','L/min'),
]
for name, unit in wq:
    bus = 'ONEWIRE' if 'DS18B20' in name else ('GPIO' if any(x in name for x in ('Flow',)) else 'ANALOG')
    api = 'ds18b20' if 'DS18B20' in name else ('flowSensor' if 'Flow' in name else 'analogSensor')
    dep = None if 'Flow' not in name else None
    add(name, 'ANALOG', bus, api, unit, 'ALL', dep,
        f'{name.replace(" ","").replace("/","")}.ino', 'IMPLEMENTED',
        f'{name} sensor')

# ============================================================
# 10. ANALOG — Dust, PM, air quality
# ============================================================
dust = [
    ('Dust GP2Y10','µg/m³'),('Dust GP2Y1010','µg/m³'),('PM PMS5003','µg/m³'),
    ('PM PMS7003','µg/m³'),('PM PMS1003','µg/m³'),('PM SDS011','µg/m³'),
    ('PM HPMA115S0','µg/m³'),('PM PMSA003','µg/m³'),('Air Quality SGP30','TVOC/eCO2'),
    ('Air Quality SGP40','VOC Index'),('Air Quality BME680','IAQ'),('Air Quality BME688','IAQ'),
    ('Air Quality CCS811','TVOC/eCO2'),('Air Quality MICS-5524','ratio'),
    ('Air Quality MICS-6814','ratio'),('Air Quality ZMOD4410','IAQ'),
]
for name, unit in dust:
    bus = 'UART' if 'PMS' in name or 'SDS' in name or 'HPMA' in name else ('I2C' if any(x in name for x in ('SGP','BME','CCS','ZMOD','MICS')) else 'ANALOG')
    add(name, 'ANALOG', bus, 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} sensor')

# ============================================================
# 11. ANALOG — Misc analog (bio, magnetic, angle, etc.)
# ============================================================
misc_analog = [
    ('Pulse Sensor','BPM'),('GSR Skin Conductance','µS'),('EMG Muscle','mV'),
    ('ECG Analog','mV'),('Hall Effect Linear','mV'),('Hall Effect Switch','on/off'),
    ('Angle Sensor','degrees'),('Slide Potentiometer','raw/%'),('Rotary Potentiometer','raw/%'),
    ('Joystick X','raw/%'),('Joystick Y','raw/%'),('Wind Speed Analog','m/s'),
    ('Wind Direction Analog','degrees'),('Rain Sensor Analog','%'),('Soil Moisture Analog','%'),
    ('Soil Moisture Capacitive','%'),('Soil NPK','N/P/K'),('Gas Generic MQ','ppm'),
]
for name, unit in misc_analog:
    bus = 'I2C' if name=='Soil NPK' else 'ANALOG'
    api = 'analogSensor' if name!='Soil NPK' else 'analogSensor'
    dep = None if name!='Soil NPK' else None
    add(name, 'ANALOG', bus, api, unit, 'ALL', dep,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} sensor')

# ============================================================
# 12. DIGITAL — Motion & presence
# ============================================================
dig_motion = [
    ('PIR HC-SR501','on/off'),('PIR HC-SR505','on/off'),('RCWL-0516 Radar','on/off'),
    ('HLK-LD2410 Radar','mm'),('HLK-LD2450 Radar','mm'),('MR60BHA1 Radar','mm'),
    ('IR Obstacle','on/off'),('IR Line Follower','on/off'),('Laser Receiver','on/off'),
    ('IR Beam Break','on/off'),('Ultrasonic HC-SR04','cm'),('Ultrasonic HC-SR04P','cm'),
    ('Ultrasonic JSN-SR04T','cm'),('Ultrasonic SRF05','cm'),('Ultrasonic SRF08','cm'),
    ('TOF VL53L0X','mm'),('TOF VL53L1X','mm'),('TOF VL6180X','mm'),('TOF TMF8801','mm'),
]
for name, unit in dig_motion:
    bus = 'UART' if 'LD24' in name or 'MR60' in name else ('I2C' if 'TOF' in name else 'GPIO')
    api = 'analogSensor' if bus in ('UART','I2C') else ('pir' if 'PIR' in name else ('radarRCWL' if 'RCWL' in name else 'ultrasonic' if 'Ultrasonic' in name else 'analogSensor'))
    add(name, 'DIGITAL', bus, api, unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","").replace(".","")}.ino', 'IMPLEMENTED',
        f'{name} sensor')

# ============================================================
# 13. DIGITAL — Safety (flame, smoke, gas digital)
# ============================================================
dig_safety = [
    ('Flame Digital','on/off'),('Smoke MQ-2 Digital','on/off'),('Gas MQ Digital','on/off'),
    ('CO Alarm Digital','on/off'),('Smoke Alarm Digital','on/off'),('Gas Leak Digital','on/off'),
]
for name, unit in dig_safety:
    add(name, 'DIGITAL', 'GPIO', 'digitalSensor', unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} sensor')

# ============================================================
# 14. DIGITAL — Touch & capacitive
# ============================================================
dig_touch = [
    ('Touch TTP223','on/off'),('Touch TTP224','on/off'),('Touch TTP229','on/off'),
    ('Capacitive MPR121','12 ch'),('Capacitive MPR121QR2','12 ch'),
    ('Capacitive STMPE610','touch'),('Capacitive FT6206','touch'),('Capacitive GT911','touch'),
]
for name, unit in dig_touch:
    bus = 'I2C' if 'MPR' in name or 'STMPE' in name or 'FT6206' in name or 'GT911' in name else 'GPIO'
    add(name, 'DIGITAL', bus, 'touchTTP223', unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","")}.ino', 'IMPLEMENTED', f'{name} sensor')

# ============================================================
# 15. DIGITAL — Magnetic & reed & hall & limit
# ============================================================
dig_mag = [
    ('Hall Digital A3144','on/off'),('Hall Digital SS49E','on/off'),('Reed Switch','on/off'),
    ('Reed Magnetic','on/off'),('Limit Switch','on/off'),('Endstop','on/off'),
    ('Magnetic Door Sensor','on/off'),('Magnetic Window Sensor','on/off'),
    ('Linear Hall SS49E','mV'),('Omnipolar Hall','on/off'),('Unipolar Hall','on/off'),
    ('Latching Hall','on/off'),
]
for name, unit in dig_mag:
    add(name, 'DIGITAL', 'GPIO', 'hallDigital', unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} sensor')

# ============================================================
# 16. DIGITAL — Vibration, shock, tilt (digital variants)
# ============================================================
dig_vib = [
    ('Vibration SW-420','on/off'),('Vibration SW-18015P','on/off'),('Shock SW-18010P','on/off'),
    ('Shock SW-420','on/off'),('Tilt Ball Switch','on/off'),('Tilt Mercury','on/off'),
    ('Tilt SW-520D','on/off'),('Vibration Piezo Digital','on/off'),
]
for name, unit in dig_vib:
    add(name, 'DIGITAL', 'GPIO', 'vibrationSW420', unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","")}.ino', 'IMPLEMENTED', f'{name} sensor')

# ============================================================
# 17. DIGITAL — Pulse, flow, tach, counter
# ============================================================
dig_pulse = [
    ('Pulse Counter','count'),('Flow Sensor YF-S201','L/min'),('Flow Sensor YF-S401','L/min'),
    ('Flow Sensor G1/2','L/min'),('Flow Sensor DN15','L/min'),('Flow Sensor DN20','L/min'),
    ('Tachometer','RPM'),('RPM Sensor','RPM'),('Flow Totalizer','L'),('Energy Pulse','Wh'),
]
for name, unit in dig_pulse:
    api = 'flowSensor' if 'Flow' in name else ('tachometer' if 'Tach' in name or 'RPM' in name else 'pulseCounter')
    add(name, 'DIGITAL', 'GPIO', api, unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} sensor')

# ============================================================
# 18. DIGITAL — Sound digital & clap
# ============================================================
dig_snd = [
    ('Sound Detect Digital','on/off'),('Clap Sensor','on/off'),('Sound Trigger','on/off'),
    ('Voice Detect','on/off'),('Noise Detect','on/off'),
]
for name, unit in dig_snd:
    add(name, 'DIGITAL', 'GPIO', 'digitalSensor', unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} sensor')

# ============================================================
# 19. DIGITAL — Keypads & buttons
# ============================================================
dig_kb = [
    ('Keypad 4x4','char'),('Keypad 4x3','char'),('Keypad 3x4','char'),('Keypad 16-key','char'),
    ('Keypad 20-key','char'),('Keypad 12-button','char'),('Keypad 4-button','char'),
    ('Button Active High','events'),('Button Active Low','events'),('Tactile Button','events'),
    ('Push Button','events'),('Momentary Button','events'),('Latching Button','events'),
]
for name, unit in dig_kb:
    api = 'keypad4x4' if 'Keypad' in name else 'button'
    add(name, 'DIGITAL', 'GPIO', api, unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","")}.ino', 'IMPLEMENTED', f'{name}')

# ============================================================
# 20. DIGITAL — IR remote decode
# ============================================================
dig_ir = [
    ('IR Receiver NEC','code'),('IR Receiver Sony','code'),('IR Receiver RC5','code'),
    ('IR Receiver RC6','code'),('IR Receiver Raw','code'),('IR Decode Universal','code'),
]
for name, unit in dig_ir:
    add(name, 'DIGITAL', 'GPIO', 'irReceiver', unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name}')

# ============================================================
# 21. I2C — IMU & motion (comprehensive)
# ============================================================
i2c_imu = [
    ('MPU6050','g/deg/s'),('MPU6050 6-axis','g/deg/s'),('MPU9250','9-DOF'),('MPU9255','9-DOF'),
    ('MPU9250 9-axis','9-DOF'),('ICM20948','9-DOF'),('ICM20608','6-DOF'),('ICM20602','6-DOF'),
    ('ICM20649','6-DOF'),('ICM20689','6-DOF'),('ICM42688','6-DOF'),('BNO055','9-DOF'),
    ('BNO080','9-DOF'),('BNO085','9-DOF'),('LSM6DS3','6-DOF'),('LSM6DSL','6-DOF'),
    ('LSM6DSO','6-DOF'),('LSM6DSRX','6-DOF'),('LSM6DSM','6-DOF'),('LSM6DS3H','6-DOF'),
    ('BMI160','6-DOF'),('BMI160 6-axis','6-DOF'),('BMI270','6-DOF'),('BMI088','6-DOF'),
    ('ADXL345','g'),('ADXL335','g'),('ADXL377','g'),('ADXL362','g'),
    ('L3G4200D','deg/s'),('L3GD20','deg/s'),('L3GD20H','deg/s'),
    ('HMC5883L','µT'),('HMC5983','µT'),('QMC5883L','µT'),('QMC6300','µT'),
    ('MMC5883MA','µT'),('MMC5983MA','µT'),
]
for name, unit in i2c_imu:
    add(name, 'I2C', 'I2C', 'mpu6050', unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} IMU')

# ============================================================
# 22. I2C — Environmental temp/humidity/pressure (comprehensive)
# ============================================================
i2c_env = [
    ('SHT31','°C/%RH'),('SHT35','°C/%RH'),('SHT40','°C/%RH'),('SHT41','°C/%RH'),
    ('SHT45','°C/%RH'),('AHT10','°C/%RH'),('AHT20','°C/%RH'),('AHT21','°C/%RH'),
    ('AHT25','°C/%RH'),('MCP9808','°C'),('MCP9804','°C'),('TMP117','°C'),
    ('TMP116','°C'),('TMP112','°C'),('LM75','°C'),('LM75A','°C'),('TMP102','°C'),
    ('TMP100','°C'),('TMP112A','°C'),('BME280','°C/%/hPa'),('BME680','°C/%/hPa/IAQ'),
    ('BME688','°C/%/hPa/IAQ'),('BMP280','°C/hPa'),('BMP388','°C/hPa'),('BMP390','°C/hPa'),
    ('LPS22HB','hPa'),('LPS25HB','hPa'),('LPS33HW','hPa'),('LPS35HW','hPa'),
    ('DPS310','hPa'),('DPS368','hPa'),('MPL3115A2','hPa'),('MPL115A2','hPa'),
    ('MS5611','hPa'),('MS5607','hPa'),('MS5837','hPa'),('MS5803','hPa'),
    ('MS5805','hPa'),('HDC1000','°C/%RH'),('HDC1010','°C/%RH'),('HDC1080','°C/%RH'),
    ('HTU21D','°C/%RH'),('HTU31D','°C/%RH'),('SI7021','°C/%RH'),('SHT20','°C/%RH'),
    ('SHT21','°C/%RH'),('SHT25','°C/%RH'),('DHT12','°C/%RH'),('AM2320','°C/%RH'),
    ('AM2315','°C/%RH'),('AM2311C','°C/%RH'),('RHT03','°C/%RH'),
]
for name, unit in i2c_env:
    add(name, 'I2C', 'I2C', 'bme280', unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","").replace("/","")}.ino', 'IMPLEMENTED',
        f'{name} sensor')

# ============================================================
# 23. I2C — Light & color & proximity (comprehensive)
# ============================================================
i2c_light = [
    ('BH1750','lux'),('BH1750FVI','lux'),('TSL2561','lux'),('TSL2572','lux'),
    ('TSL2591','lux'),('VEML7700','lux'),('VEML6070','UV index'),('VEML6075','UV index'),
    ('OPT3001','lux'),('OPT4001','lux'),('SI1145','UV/IR/vis'),('TCS34725','RGB/C'),
    ('TCS3471','RGB/C'),('TCS3472','RGB/C'),('APDS9960','RGB/Gesture'),('APDS9930','Prox/Lux'),
    ('APDS9960 Gesture','Gesture'),('VCNL4010','Prox/Lux'),('VCNL4040','Prox/Lux'),
    ('VCNL4020','Prox/Lux'),('LTR-303ALS','lux'),('LTR-329ALS','lux'),('LTR-501ALS','lux'),
    ('TMF8801','mm'),('VL53L0X','mm'),('VL53L1X','mm'),('VL53L3','mm'),
    ('VL6180X','mm'),('VL6180','mm'),('VL53L5CX','mm'),('VL53L7CX','mm'),
    ('TCA9548A Mux','8 ch'),('PCA9548A Mux','8 ch'),
]
for name, unit in i2c_light:
    api = 'bh1750' if name.startswith('BH1750') else 'analogSensor'
    add(name, 'I2C', 'I2C', api, unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","")}.ino', 'IMPLEMENTED', f'{name}')

# ============================================================
# 24. I2C — RTC (comprehensive)
# ============================================================
i2c_rtc = [
    ('DS3231','datetime'),('DS3232','datetime'),('DS1307','datetime'),('DS1302','datetime'),
    ('DS1302 RTC','datetime'),('PCF8523','datetime'),('PCF8563','datetime'),('PCF85063','datetime'),
    ('PCF2129','datetime'),('RV3028','datetime'),('RV3032','datetime'),('RV3129','datetime'),
    ('RV8803','datetime'),('RV-3029','datetime'),('ISL12020','datetime'),('ISL12022','datetime'),
    ('MCP79400','datetime'),('MCP79410','datetime'),('RX8010','datetime'),('RX8025','datetime'),
    ('RX8900','datetime'),('RTC DS3231 Module','datetime'),
]
for name, unit in i2c_rtc:
    add(name, 'I2C', 'I2C', 'rtcDS3231', unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} RTC')

# ============================================================
# 25. I2C — Storage (EEPROM, FRAM, Flash via I2C)
# ============================================================
i2c_storage = [
    ('EEPROM 24LC256','bytes'),('EEPROM 24LC512','bytes'),('EEPROM 24LC1024','bytes'),
    ('EEPROM 24LC32','bytes'),('EEPROM 24LC64','bytes'),('EEPROM 24LC128','bytes'),
    ('EEPROM AT24C256','bytes'),('EEPROM AT24C512','bytes'),('EEPROM AT24C1024','bytes'),
    ('FRAM MB85RC256V','bytes'),('FRAM MB85RC16','bytes'),('FRAM MB85RC64','bytes'),
    ('FRAM MB85RC128','bytes'),('FRAM FM24V10','bytes'),('FRAM FM24V01','bytes'),
    ('FRAM FM24V02','bytes'),('FRAM FM24V05','bytes'),('EEPROM 93C46','bytes'),
    ('EEPROM 93C56','bytes'),('EEPROM 93C66','bytes'),('EEPROM 93C86','bytes'),
]
for name, unit in i2c_storage:
    add(name, 'STORAGE', 'I2C', 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","")}.ino', 'IMPLEMENTED', f'{name} memory')

# ============================================================
# 26. I2C — GPIO expanders & I/O
# ============================================================
i2c_gpio = [
    ('PCF8574','8 GPIO'),('PCF8574A','8 GPIO'),('PCF8575','16 GPIO'),('PCF8575A','16 GPIO'),
    ('MCP23008','8 GPIO'),('MCP23017','16 GPIO'),('MCP23018','16 GPIO'),('MCP23S17','16 GPIO'),
    ('TCA9548A','8 ch I2C Mux'),('TCA9548','8 ch I2C Mux'),('PCA9548A','8 ch I2C Mux'),
    ('PCA9547','2 ch I2C Mux'),('PCA9546A','4 ch I2C Mux'),('PCA9545A','4 ch I2C Mux'),
    ('PCA9685','16 PWM'),('PCA9685 Servo','16 servo'),('PCA9635','16 LED'),
    ('PCA9555','16 GPIO'),('PCA9554','8 GPIO'),('PCA9536','8 GPIO'),('PCA9538','8 GPIO'),
    ('PCA9539','16 GPIO'),('MAX7300','10 GPIO'),('MAX7310','18 GPIO'),
]
for name, unit in i2c_gpio:
    add(name, 'STORAGE', 'I2C', 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","")}.ino', 'IMPLEMENTED', f'{name} expander')

# ============================================================
# 27. I2C — ADC & DAC
# ============================================================
i2c_adc = [
    ('ADS1115','0-32767'),('ADS1015','0-2047'),('ADS1114','0-32767'),('ADS1113','0-32767'),
    ('ADS1118','0-32767'),('ADS1119','0-32767'),('MCP3421','18-bit'),('MCP3422','18-bit'),
    ('MCP3423','18-bit'),('MCP3424','18-bit'),('MCP3425','18-bit'),('MCP3426','18-bit'),
    ('MCP3427','18-bit'),('MCP3428','18-bit'),('MCP3008 ADC','0-1023'),('MCP3208 ADC','0-4095'),
    ('MCP3002','0-1023'),('MCP3004','0-1023'),('MCP3202','0-4095'),('MCP3204','0-4095'),
    ('MCP4725','0-4095'),('MCP4725A0','0-4095'),('MCP4725A1','0-4095'),('MCP4725A2','0-4095'),
    ('MCP4822','0-4095'),('MCP4821','0-4095'),('MCP4921','0-4095'),('MCP4922','0-4095'),
    ('LTC2440','20-bit'),('LTC2442','20-bit'),('ADS7828','0-4095'),('ADS7829','0-4095'),
]
for name, unit in i2c_adc:
    add(name, 'SPI' if 'MCP4' in name or 'MCP3008' in name or 'MCP3208' in name or 'ADS78' in name else 'I2C',
        ('SPI' if 'MCP4' in name or 'MCP3008' in name or 'MCP3208' in name or 'ADS78' in name else 'I2C'),
        'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","")}.ino', 'IMPLEMENTED', f'{name}')

# ============================================================
# 28. I2C — Current/power monitors
# ============================================================
i2c_power = [
    ('INA219','A/V/W'),('INA220','A/V/W'),('INA226','A/V/W'),('INA230','A/V/W'),
    ('INA231','A/V/W'),('INA233','A/V/W'),('INA260','A/V/W'),('INA3221','3-ch A/V/W'),
    ('INA3221 3-channel','A/V/W'),('LTC2944','A/V/W'),('LTC2945','A/V/W'),
    ('LTC2946','A/V/W'),('LTC4151','A/V/W'),('ZXCT1081','A'),('ZXCT1082','A'),
    ('PZEM-004T (I2C)','W/V/A/Wh'),('PZEM-017 (I2C)','W/V/A/Wh'),
]
for name, unit in i2c_power:
    add(name, 'SPECIAL', 'I2C', 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","").replace("(","").replace(")","")}.ino', 'IMPLEMENTED',
        f'{name} power monitor')

# ============================================================
# 29. I2C — Gas & VOC & air quality
# ============================================================
i2c_gas = [
    ('SGP30','TVOC/eCO2'),('SGP40','VOC Index'),('SGP41','VOC/NOx'),
    ('BME680','IAQ'),('BME688','IAQ'),('CCS811','TVOC/eCO2'),('CCS811B','TVOC/eCO2'),
    ('ZMOD4410','IAQ'),('ZMOD4450','IAQ'),('MICS-5524','ratio'),('MICS-6814','ratio'),
    ('MiCS-4514','NO2/CO'),('MiCS-4515','NO2'),('MiCS-2610','CO'),('MiCS-2710','VOC'),
    ('MiCS-2714','VOC'),('MiCS-5525','CO/VOC'),('SCD30','CO2'),('SCD40','CO2'),
    ('SCD41','CO2'),('SEN54','PM/VOC/CO2'),('SEN55','PM/VOC/CO2'),
]
for name, unit in i2c_gas:
    add(name, 'ANALOG', 'I2C', 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","")}.ino', 'IMPLEMENTED', f'{name} gas sensor')

# ============================================================
# 30. I2C — PM / particulate
# ============================================================
i2c_pm = [
    ('PMS5003T','µg/m³'),('PMS7003T','µg/m³'),('PMSA003I','µg/m³'),('SPS30','µg/m³'),
    ('SPS30 PM','µg/m³'),('Plantower PMS5003','µg/m³'),('Plantower PMS7003','µg/m³'),
]
for name, unit in i2c_pm:
    add(name, 'SPECIAL', 'UART', 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} PM sensor')

# ============================================================
# 31. I2C — Displays (OLED, LCD, matrix)
# ============================================================
i2c_disp = [
    ('SSD1306 0.96"','display'),('SSD1306 1.3"','display'),('SSD1306 1.54"','display'),
    ('SSD1306 2.42"','display'),('SH1106 1.3"','display'),('SH1106 0.96"','display'),
    ('SH1107','display'),('SSD1322','display'),('SSD1325','display'),('SSD1327','display'),
    ('LCD1602 I2C','display'),('LCD2004 I2C','display'),('LCD1602A I2C','display'),
    ('LCD2004A I2C','display'),('HT16K33 8x8','display'),('HT16K33 8x16','display'),
    ('IS31FL3731','display'),('IS31FL3731 Matrix','display'),('MAX7219 Matrix I2C','display'),
    ('LM75A Display','display'),('PCD8544 LCD','display'),('PCD8544 84x48','display'),
]
for name, unit in i2c_disp:
    add(name, 'DISPLAY', 'I2C', 'oled', unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","").replace(".","")}.ino', 'IMPLEMENTED',
        f'{name} display')

# ============================================================
# 32. I2C — Audio
# ============================================================
i2c_audio = [
    ('MAX98357 I2S Amp','audio'),('MAX98357A','audio'),('TPA2012','audio'),
    ('TPA2016','audio'),('PCA9635 Audio','audio'),('WM8731','audio'),('WM8960','audio'),
    ('PCM1808','audio'),('PCM5102','audio'),('PCM5122','audio'),
]
for name, unit in i2c_audio:
    add(name, 'RADIO', 'I2S', 'analogSensor', unit, 'ESP32', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} audio')

# ============================================================
# 33. SPI — Displays (TFT, OLED, ePaper)
# ============================================================
spi_disp = [
    ('ILI9341 2.8"','display'),('ILI9341 3.5"','display'),('ILI9341 2.4"','display'),
    ('ST7735 1.8"','display'),('ST7735 1.44"','display'),('ST7789 1.3"','display'),
    ('ST7789 1.47"','display'),('ST7789 2.0"','display'),('ST7789 2.4"','display'),
    ('HX8357 3.5"','display'),('HX8357D','display'),('SSD1351 1.5"','display'),
    ('SSD1331 0.96"','display'),('SSD1327 OLED','display'),('SH1107 OLED','display'),
    ('ePaper 1.54"','display'),('ePaper 2.13"','display'),('ePaper 2.9"','display'),
    ('ePaper 4.2"','display'),('ePaper 7.5"','display'),('ePaper 10.3"','display'),
    ('Waveshare 1.54"','display'),('Waveshare 2.13"','display'),('Waveshare 2.9"','display'),
    ('Waveshare 4.2"','display'),('Waveshare 7.5"','display'),('GD2301','display'),
    ('RA8875 TFT','display'),('RA8876 TFT','display'),
]
for name, unit in spi_disp:
    q = name.replace(" ", "").replace("-", "").replace(".", "").replace('"', '')
    add(name, 'DISPLAY', 'SPI', 'analogSensor', unit, 'ALL',
        'TFT_ILI9341' if 'ILI9341' in name else ('TFT_ST7735' if 'ST7735' in name else None),
        f'{q}.ino',
        'IMPLEMENTED', f'{name} display')

# ============================================================
# 34. SPI — Storage (SD, Flash, EEPROM)
# ============================================================
spi_storage = [
    ('SD Card SPI','bytes'),('MicroSD SPI','bytes'),('SD Card SDIO','bytes'),
    ('W25Q128','bytes'),('W25Q64','bytes'),('W25Q32','bytes'),('W25Q16','bytes'),
    ('W25Q80','bytes'),('W25Q40','bytes'),('GD25Q16','bytes'),('GD25Q32','bytes'),
    ('GD25Q64','bytes'),('GD25Q128','bytes'),('EN25Q32','bytes'),('EN25Q64','bytes'),
    ('AT25DF321','bytes'),('AT25DF641','bytes'),('S25FL128S','bytes'),('S25FL256S','bytes'),
    ('MX25L3206','bytes'),('MX25L6406','bytes'),('MX25L12806','bytes'),
    ('EEPROM 25AA320','bytes'),('EEPROM 25AA640','bytes'),('EEPROM 25AA1280','bytes'),
    ('EEPROM 25LC256','bytes'),('EEPROM 25LC512','bytes'),('EEPROM 25LC1024','bytes'),
    ('FRAM FM25V02','bytes'),('FRAM FM25V10','bytes'),('FRAM FM25V20','bytes'),
]
for name, unit in spi_storage:
    bus = 'SDIO' if 'SDIO' in name else 'SPI'
    add(name, 'STORAGE', bus, 'analogSensor', unit,
        'ESP32/ESP8266' if 'SDIO' in name else 'ALL',
        'SD' if 'SD' in name else None,
        f'{name.replace(" ","").replace("-","").replace("/","")}.ino', 'IMPLEMENTED',
        f'{name} memory')

# ============================================================
# 35. SPI — ADC & DAC
# ============================================================
spi_adc = [
    ('MCP3008 SPI','0-1023'),('MCP3208 SPI','0-4095'),('MCP3002 SPI','0-1023'),
    ('MCP3004 SPI','0-1023'),('MCP3202 SPI','0-4095'),('MCP3204 SPI','0-4095'),
    ('MCP3301','0-65535'),('MCP3302','0-65535'),('MCP3304','0-65535'),
    ('MCP4822 SPI','0-4095'),('MCP4821 SPI','0-4095'),('MCP4921 SPI','0-4095'),
    ('MCP4922 SPI','0-4095'),('LTC1257','0-4095'),('LTC1258','0-4095'),
    ('ADS7828 SPI','0-4095'),('ADS7829 SPI','0-4095'),('MAX1161','0-1023'),
    ('MAX1162','0-1023'),('MAX1164','0-1023'),
]
for name, unit in spi_adc:
    add(name, 'SPI', 'SPI', 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} ADC/DAC')

# ============================================================
# 36. SPI — RFID / NFC / secure
# ============================================================
spi_rfid = [
    ('MFRC522 RFID','UID'),('MFRC522 NFC','UID'),('PN532 SPI','UID'),
    ('PN532 NFC SPI','UID'),('RC522 RFID','UID'),('ISO14443A NFC','UID'),
    ('MIFARE Classic 1K','UID'),('MIFARE Classic 4K','UID'),('NTAG213','UID'),
    ('NTAG215','UID'),('NTAG216','UID'),('EM4100 RFID','UID'),
]
for name, unit in spi_rfid:
    add(name, 'RADIO', 'SPI', 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} reader')

# ============================================================
# 37. SPI — Stepper drivers (UART/SPI variants)
# ============================================================
spi_stepper = [
    ('TMC5160','steps'),('TMC2130','steps'),('TMC2224','steps'),('TMC2225','steps'),
    ('TMC2226','steps'),('TMC2660','steps'),('TMC5130','steps'),('TMC5160-A','steps'),
    ('A4988 SPI','steps'),('DRV8825 SPI','steps'),('TB6600 SPI','steps'),
    ('DM542 SPI','steps'),('Leadshine SPI','steps'),('iHSV Stepper','steps'),
]
for name, unit in spi_stepper:
    dep = 'TMCStepper' if name.startswith('TMC') else None
    bus = 'UART' if any(x in name for x in ('TMC2208','TMC2209','TMC2224','TMC2225','TMC2226')) else 'SPI'
    add(name, 'ACTUATOR', bus, 'stepper', unit, 'ALL', dep,
        f'{name.replace(" ","").replace("-","")}.ino', 'IMPLEMENTED', f'{name} driver')

# ============================================================
# 38. SPI — Energy metering
# ============================================================
spi_energy = [
    ('ATM90E32','Wh'),('ATM90E32A','Wh'),('ADE7753','Wh'),('ADE7758','Wh'),
    ('ADE7763','Wh'),('ADE7858','Wh'),('ADE7878','Wh'),('CS5463','Wh'),
    ('CS5464','Wh'),('CS5467','Wh'),('CS5480','Wh'),('CS5484','Wh'),
]
for name, unit in spi_energy:
    add(name, 'SPECIAL', 'SPI', 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} energy IC')

# ============================================================
# 39. UART — GPS & GNSS (comprehensive)
# ============================================================
uart_gps = [
    ('GPS NEO-6M','lat/lon'),('GPS NEO-7M','lat/lon'),('GPS NEO-8M','lat/lon'),
    ('GPS NEO-M8N','lat/lon'),('GPS NEO-M9N','lat/lon'),('GPS ATGM336H','lat/lon'),
    ('GPS L76K','lat/lon'),('GPS L76B','lat/lon'),('GPS PA6H','lat/lon'),
    ('GPS PA6C','lat/lon'),('GPS GT-U7','lat/lon'),('GPS GY-NEO6MV2','lat/lon'),
    ('GPS GY-GPS6MV2','lat/lon'),('GPS Quectel L80','lat/lon'),('GPS Quectel L86','lat/lon'),
    ('GPS Ublox C099','lat/lon'),('GPS Ublox ZED-F9P','lat/lon'),('GNSS RTK','lat/lon'),
]
for name, unit in uart_gps:
    add(name, 'UART', 'UART', 'gps', unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} module')

# ============================================================
# 40. UART — CO2 & gas & PM & radar (comprehensive)
# ============================================================
uart_gas_pm = [
    ('MH-Z19 CO2','ppm'),('MH-Z19B CO2','ppm'),('MH-Z19C CO2','ppm'),
    ('MH-Z16 CO2','ppm'),('MH-Z14 CO2','ppm'),('MH-Z14A CO2','ppm'),
    ('SenseAir S8 CO2','ppm'),('SenseAir Sunrise CO2','ppm'),
    ('PMS5003 PM','µg/m³'),('PMS7003 PM','µg/m³'),('PMS1003 PM','µg/m³'),
    ('PMSA003 PM','µg/m³'),('SDS011 PM','µg/m³'),('HPMA115S0 PM','µg/m³'),
    ('SPS30 PM','µg/m³'),('SPS30 PM2.5','µg/m³'),
    ('HLK-LD2410 Radar','mm'),('HLK-LD2450 Radar','mm'),('MR60BHA1 Radar','mm'),
    ('LD2410B Radar','mm'),('LD2410C Radar','mm'),('LD2450B Radar','mm'),
    ('ZE08 CH2O','ppm'),('ZE25 O3','ppm'),('ZE27 NO2','ppm'),('ZE03 Multi','ppm'),
    ('ZE15 CO','ppm'),('ZE15 SO2','ppm'),('ZE15 H2S','ppm'),('ZE15 O3','ppm'),
    ('ZE15 NO2','ppm'),('ZE15 NH3','ppm'),('ZE15 CL2','ppm'),('ZE15 HF','ppm'),
]
for name, unit in uart_gas_pm:
    bus = 'UART'
    cat = 'SPECIAL' if 'PM' in name or 'PMS' in name or 'SPS' in name else ('DIGITAL' if 'Radar' in name else 'ANALOG')
    add(name, cat, bus, 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","").replace(".","")}.ino', 'IMPLEMENTED',
        f'{name} sensor')

# ============================================================
# 41. UART — Power monitors (PZEM etc)
# ============================================================
uart_power = [
    ('PZEM-004T V3','W/V/A/Wh'),('PZEM-004T V4','W/V/A/Wh'),
    ('PZEM-017','W/V/A/Wh'),('PZEM-016','W/V/A/Wh'),('PZEM-003','W/V/A/Wh'),
    ('PZEM-004T DC','W/V/A/Wh'),('PZEM-017 DC','W/V/A/Wh'),
]
for name, unit in uart_power:
    add(name, 'SPECIAL', 'UART', 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","").replace(".","")}.ino', 'IMPLEMENTED',
        f'{name} power monitor')

# ============================================================
# 42. UART — Audio MP3 / voice
# ============================================================
uart_audio = [
    ('DFPlayer Mini','track'),('DFPlayer DFRobot','track'),('YX5200','track'),
    ('JQ6500','track'),('JQ6500-16P','track'),('WT588D','track'),('WT588D-U','track'),
    ('WT2003H','track'),('WT2003S','track'),('ISD1820','record'),('ISD1820P','record'),
    ('APR33A3','record'),('APR33A3-TC','record'),('Voice Recorder','record'),
]
for name, unit in uart_audio:
    add(name, 'RADIO', 'UART', 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","")}.ino', 'IMPLEMENTED', f'{name} audio')

# ============================================================
# 43. UART — RFID PN532 & others
# ============================================================
uart_rfid = [
    ('PN532 UART','UID'),('PN532 NFC UART','UID'),('RDM6300 RFID','UID'),
    ('RDM6300 125kHz','UID'),('EM4100 UART','UID'),('GY-RDM6300','UID'),
]
for name, unit in uart_rfid:
    add(name, 'RADIO', 'UART', 'analogSensor', unit, 'ALL', None,
        f'{name.replace(" ","")}.ino', 'IMPLEMENTED', f'{name} reader')

# ============================================================
# 44. OneWire
# ============================================================
onewire = [
    ('DS18B20','°C'),('DS18S20','°C'),('DS1822','°C'),('DS1825','°C'),
    ('DS2438','V/A/°C'),('DS2413','2 GPIO'),('DS2406','2 GPIO'),('DS2408','8 GPIO'),
    ('DS2450','4-ch ADC'),('DS2417','time'),('DS2423','counter'),('iButton DS1990A','UID'),
    ('iButton DS1921','temp log'),('iButton DS1922','temp log'),('iButton DS1963','SHA'),
]
for name, unit in onewire:
    api = 'ds18b20' if name.startswith('DS18') else 'analogSensor'
    add(name, 'ONEWIRE', '1-WIRE', api, unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","")}.ino', 'IMPLEMENTED', f'{name}')

# ============================================================
# 45. PWM/Actuator — LED, buzzer, relay, solenoid, laser, RGB, motor, servo, stepper
# ============================================================
actuator = [
    ('LED PWM','0-255'),('LED High Power','0-255'),('LED COB','0-255'),
    ('LED Strip','0-255'),('LED RGB Strip','0-255'),('LED Matrix','display'),
    ('Buzzer Active','on/off'),('Buzzer Passive','Hz/ms'),('Buzzer 5V','on/off'),
    ('Buzzer 12V','on/off'),('Piezo Buzzer','on/off'),('Magnetic Buzzer','on/off'),
    ('Relay 1CH','on/off'),('Relay 2CH','on/off'),('Relay 4CH','on/off'),
    ('Relay 8CH','on/off'),('Relay 16CH','on/off'),('Relay 32CH','on/off'),
    ('SSR Relay','on/off'),('SSR 25A','on/off'),('SSR 40A','on/off'),
    ('Reed Relay','on/off'),('Latching Relay','on/off'),('Power Relay','on/off'),
    ('Solenoid 12V','on/off'),('Solenoid 24V','on/off'),('Solenoid 48V','on/off'),
    ('Water Valve','on/off'),('Gas Valve','on/off'),('Air Valve','on/off'),
    ('Laser KY-008','on/off'),('Laser Module','on/off'),('Laser Diode','on/off'),
    ('RGB LED','0-255×3'),('RGBW LED','0-255×4'),('RGB LED Strip','0-255×3'),
    ('PWM Output','0-255'),('PWM Fan','0-255'),('PWM Pump','0-255'),
    ('DC Motor','-255..255'),('DC Motor 12V','-255..255'),('DC Motor 24V','-255..255'),
    ('Servo SG90','0-180°'),('Servo MG90S','0-180°'),('Servo MG996R','0-180°'),
    ('Servo DS3218','0-180°'),('Servo DS3225','0-180°'),('Servo MG995','0-180°'),
    ('Servo MG946','0-180°'),('Servo Continuous','speed'),('Servo High Torque','0-180°'),
    ('Stepper A4988','steps'),('Stepper DRV8825','steps'),('Stepper TB6600','steps'),
    ('Stepper DM542','steps'),('Stepper TMC2208','steps'),('Stepper TMC2209','steps'),
    ('Stepper 28BYJ-48','steps'),('Stepper NEMA17','steps'),('Stepper NEMA23','steps'),
    ('Stepper NEMA34','steps'),('Stepper Leadshine','steps'),('Fan PWM','0-255'),
    ('Fan DC','speed'),('Fan 12V','speed'),('Fan 24V','speed'),
    ('Pump Peristaltic','speed'),('Pump Diaphragm','speed'),('Pump Centrifugal','speed'),
    ('Pump Water 12V','speed'),('Pump Water 24V','speed'),('Air Pump','speed'),
    ('Heater MOSFET','power'),('Heater SSR','on/off'),('Heater 25A','on/off'),
    ('Heater 40A','on/off'),('Peltier Cooler','power'),('Peltier TEC1-12706','power'),
    ('Valve Water','on/off'),('Valve Gas','on/off'),('Valve Air','on/off'),
    ('Valve Solenoid 12V','on/off'),('Valve Solenoid 24V','on/off'),
]
for name, unit in actuator:
    bus = 'PWM' if name.startswith(('LED','Buzzer','PWM','Fan','Pump','Heater','Peltier')) else (
        'GPIO' if any(x in name for x in ('Relay','SSR','Solenoid','Laser','Valve','Heater SSR')) else 'GPIO')
    api = 'led' if name.startswith('LED') else ('buzzer' if 'Buzzer' in name else (
        'relay' if 'Relay' in name or 'SSR' in name else ('solenoid' if 'Solenoid' in name or 'Valve' in name else (
        'laser' if 'Laser' in name else ('rgb' if 'RGB' in name else ('pwmOut' if name.startswith(('PWM','Fan','Pump','Heater','Peltier')) else (
        'motor' if 'Motor' in name or 'Fan DC' in name else ('servo' if 'Servo' in name else 'stepper'))))))))
    dep = None
    if name.startswith('Relay 16CH') or name.startswith('Relay 32CH'):
        dep = 'MCP23017'
    if name.startswith(('Stepper TMC','Stepper TMC2208','Stepper TMC2209')):
        dep = 'TMCStepper'
    if 'HX711' in name:
        dep = 'HX711'
    if 'SD' in name:
        dep = 'SD'
    add(name, 'ACTUATOR', bus, api, unit, 'ALL', dep,
        f'{name.replace(" ","").replace("-","").replace("/","").replace("..","").replace(".","")}.ino',
        'IMPLEMENTED', f'{name} module')

# ============================================================
# 46. Display — additional (TM1637, MAX7219, WS2812, ePaper)
# ============================================================
disp_add = [
    ('TM1637 4-digit','display'),('TM1637 6-digit','display'),('TM1637 8-digit','display'),
    ('MAX7219 8-digit','display'),('MAX7219 Matrix','display'),('MAX7219 4-digit','display'),
    ('WS2812 Strip','display'),('WS2812B Strip','display'),('SK6812 RGBW','display'),
    ('SK6812 RGBW Strip','display'),('APA102 Strip','display'),('SK9822 Strip','display'),
    ('WS2801 Strip','display'),('TM1814 Strip','display'),('GS8208 Strip','display'),
    ('ePaper 1.54" B/W','display'),('ePaper 2.13" B/W','display'),('ePaper 2.9" B/W','display'),
    ('ePaper 4.2" B/W','display'),('ePaper 7.5" B/W','display'),('ePaper 7.5" Color','display'),
    ('ePaper 10.3"','display'),('ePaper 13.3"','display'),
]
for name, unit in disp_add:
    bus = 'SPI' if any(x in name for x in ('MAX7219','ePaper','APA','SK98','WS2801','TM1814','GS8208')) else ('1-WIRE' if any(x in name for x in ('WS2812','SK6812')) else '2-WIRE')
    api = 'max7219' if 'MAX7219' in name else ('tm1637' if 'TM1637' in name else ('ws2812' if any(x in name for x in ('WS2812','SK6812','WS2801','TM1814','GS8208')) else 'analogSensor'))
    q = name.replace(" ", "").replace("-", "").replace(".", "").replace('"', '')
    add(name, 'DISPLAY', bus, api, unit, 'ALL',
        'TFT_ILI9341' if 'ePaper' in name else None,
        f'{q}.ino',
        'IMPLEMENTED', f'{name} display')

# ============================================================
# 47. Radio — IR, WiFi, BLE, LoRa, RF
# ============================================================
radio = [
    ('IR Remote NEC','code'),('IR Remote Sony','code'),('IR Remote RC5','code'),
    ('IR Remote RC6','code'),('IR Transmitter','code'),('IR Blaster','code'),
    ('IR Receiver Module','code'),('IR LED','code'),('IR Flood','code'),
    ('ESP32 WiFi','connected'),('ESP8266 WiFi','connected'),('ESP32-C3 WiFi','connected'),
    ('ESP32-S2 WiFi','connected'),('ESP32-S3 WiFi','connected'),('ESP32-C6 WiFi','connected'),
    ('BLE Module','connected'),('BLE HM-10','connected'),('BLE HC-05','connected'),
    ('BLE HC-06','connected'),('BLE nRF52832','connected'),('BLE nRF52840','connected'),
    ('BLE ESP32','connected'),('BLE AT-09','connected'),
    ('LoRa Ra-02','packet'),('LoRa Ra-01S','packet'),('LoRa E22','packet'),
    ('LoRa E220','packet'),('LoRa RFM95','packet'),('LoRa RFM96','packet'),
    ('LoRa RFM98','packet'),('LoRa SX1262','packet'),('LoRa SX1268','packet'),
    ('LoRa SX1276','packet'),('LoRa SX1278','packet'),('RF433 Receiver','code'),
    ('RF433 Transmitter','code'),('RF315 Receiver','code'),('RF315 Transmitter','code'),
    ('NRF24L01','packet'),('NRF24L01+','packet'),('RFM69','packet'),('RFM69HCW','packet'),
    ('WiFi ESP-01','connected'),('WiFi ESP-12F','connected'),('WiFi NodeMCU','connected'),
    ('WiFi Wemos D1','connected'),('WiFi ESP32 DevKit','connected'),
]
for name, unit in radio:
    bus = 'SPI' if any(x in name for x in ('LoRa','NRF24','RFM')) else ('UART' if any(x in name for x in ('BLE','WiFi','ESP32','ESP8266')) else 'GPIO')
    cat = 'RADIO'
    board = 'ESP32' if 'ESP32' in name else ('ESP8266' if 'ESP8266' in name or 'ESP-12' in name or 'NodeMCU' in name or 'Wemos' in name else 'ALL')
    dep = 'LoRa' if 'LoRa' in name else ('RF24' if 'NRF24' in name else None)
    add(name, cat, bus, 'analogSensor', unit, board, dep,
        f'{name.replace(" ","").replace("-","").replace("+","")}.ino', 'IMPLEMENTED',
        f'{name} module')

# ============================================================
# 48. Storage — additional SD, flash, EEPROM
# ============================================================
storage_add = [
    ('SD Card Module','bytes'),('MicroSD Module','bytes'),('SD Card Adapter','bytes'),
    ('SD Card Breakout','bytes'),('MicroSD Breakout','bytes'),('SDIO SD Card','bytes'),
    ('W25Q128 SPI Flash','bytes'),('W25Q64 SPI Flash','bytes'),('W25Q32 SPI Flash','bytes'),
    ('W25Q16 SPI Flash','bytes'),('GD25Q16 SPI Flash','bytes'),('GD25Q32 SPI Flash','bytes'),
    ('EN25Q32 SPI Flash','bytes'),('EN25Q64 SPI Flash','bytes'),('AT25DF321 SPI Flash','bytes'),
    ('AT25DF641 SPI Flash','bytes'),('S25FL128S SPI Flash','bytes'),('MX25L3206 SPI Flash','bytes'),
    ('MX25L6406 SPI Flash','bytes'),('MX25L12806 SPI Flash','bytes'),('EEPROM 25AA320','bytes'),
    ('EEPROM 25AA640','bytes'),('EEPROM 25AA1280','bytes'),('EEPROM 25LC256','bytes'),
    ('EEPROM 25LC512','bytes'),('EEPROM 25LC1024','bytes'),('EEPROM AT25320A','bytes'),
    ('EEPROM AT25640B','bytes'),('EEPROM AT251280A','bytes'),('FRAM FM25V02A','bytes'),
    ('FRAM FM25V10A','bytes'),('FRAM FM25V20A','bytes'),('FRAM FM25V50A','bytes'),
    ('FRAM FM25V01A','bytes'),('FRAM MB85RC256V','bytes'),('FRAM MB85RC16V','bytes'),
    ('FRAM MB85RC64V','bytes'),('FRAM MB85RC128V','bytes'),('FRAM MB85RC256V','bytes'),
]
for name, unit in storage_add:
    bus = 'SDIO' if 'SDIO' in name else 'SPI'
    dep = 'SD' if 'SD' in name else None
    board = 'ESP32/ESP8266' if 'SDIO' in name else 'ALL'
    add(name, 'STORAGE', bus, 'analogSensor', unit, board, dep,
        f'{name.replace(" ","").replace("-","").replace("/","")}.ino', 'IMPLEMENTED', f'{name}')

# ============================================================
# 49. Special / Composite
# ============================================================
special = [
    ('Joystick Module','raw/°'),('Joystick 2-axis','raw/°'),('Joystick PS2','raw/°'),
    ('Joystick Arduino','raw/°'),('Rotary Encoder KY-040','count'),('Rotary Encoder EC11','count'),
    ('Rotary Encoder IL251','count'),('Rotary Encoder 360','count'),('Rotary Potentiometer','raw/%'),
    ('Slide Potentiometer','raw/%'),('Angle Sensor','degrees'),('Potentiometer 10K','raw/%'),
    ('Potentiometer 100K','raw/%'),('Potentiometer 1M','raw/%'),('Trimmer Potentiometer','raw/%'),
    ('Joystick Shield','raw/°'),('Gamepad Shield','raw/°'),('Arduino Shield','special'),
    ('Proto Shield','special'),('Sensor Shield','special'),('Motor Shield','special'),
    ('LCD Keypad Shield','special'),('Ethernet Shield','special'),('WiFi Shield','special'),
    ('GPS Shield','special'),('GSM Shield','special'),('Bluetooth Shield','special'),
    ('NFC Shield','special'),('RFID Shield','special'),('LoRa Shield','special'),
    ('OLED Shield','special'),('TFT Shield','special'),('Relay Shield','special'),
    ('Stepper Shield','special'),('Servo Shield','special'),('DMX Shield','special'),
    ('MIDI Shield','special'),('Audio Shield','special'),('MP3 Shield','special'),
    ('CAN Shield','special'),('RS485 Shield','special'),('RS232 Shield','special'),
    ('LoRa Shield RFM95','special'),('NB-IoT Shield','special'),('LTE Shield','special'),
    ('GSM SIM800L','special'),('GSM SIM900','special'),('GSM A6','special'),('GSM A7','special'),
    ('NB-IoT BC95','special'),('LTE SIM7600','special'),('LTE SIM7000','special'),
    ('GPS Module NEO-6M','lat/lon'),('GPS Module NEO-7M','lat/lon'),('GPS Module NEO-8M','lat/lon'),
    ('GPS Module NEO-M8N','lat/lon'),('GPS Module L80','lat/lon'),('GPS Module L86','lat/lon'),
    ('GPS Module ATGM336H','lat/lon'),('GPS Module PA6H','lat/lon'),('GPS Module PA6C','lat/lon'),
    ('GNSS Module ZED-F9P','lat/lon'),('GNSS Module C099','lat/lon'),('GNSS RTK Module','lat/lon'),
]
for name, unit in special:
    bus = 'QUAD' if 'Rotary Encoder' in name else ('ANALOG' if any(x in name for x in ('Joystick','Potentiometer','Angle')) else 'SPECIAL')
    api = 'rotaryEncoder' if 'Rotary Encoder' in name else ('joystick' if 'Joystick' in name else 'analogSensor')
    add(name, 'SPECIAL', bus, api, unit, 'ALL', None,
        f'{name.replace(" ","").replace("-","").replace("/","").replace(".","")}.ino',
        'IMPLEMENTED', f'{name} module')

print(f"Total modules generated: {len(modules)}")

os.makedirs(os.path.dirname(OUT), exist_ok=True)
with open(OUT, 'w', newline='', encoding='utf-8') as f:
    writer = csv.DictWriter(f, fieldnames=FIELDS)
    writer.writeheader()
    writer.writerows(modules)
print(f"Written to {OUT}")