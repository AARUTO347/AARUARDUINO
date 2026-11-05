#!/usr/bin/env python3
"""
AARUARDUINO Host Tests
MADE BY AARUTO
Mirrors AaruarduinoMath.h for CI without Arduino toolchain.
Run: python tests/host/test_aaruarduino.py
"""
import math
import sys


def distance_cm(echo_us):
    return float('nan') if echo_us == 0 else echo_us / 58.0


def distance_inches(echo_us):
    return float('nan') if echo_us == 0 else echo_us / 148.0


def echo_us_from_cm(cm):
    return 0 if cm <= 0 else int(cm * 58.0)


def adc_to_mv(raw, adc_max, vref_v, divider):
    if adc_max <= 0 or vref_v <= 0 or divider <= 0:
        return -1
    if raw < 0 or raw > adc_max:
        return -1
    return int((raw / adc_max) * vref_v * divider * 1000)


def percent(raw, mx):
    if mx <= 0 or raw < 0 or raw > mx:
        return float('nan')
    return raw / mx * 100.0


def lm35_c(mv):
    return mv * 0.1


def tmp36_c(mv):
    return (mv - 500.0) / 10.0


def acs712_a(mv, mpa, zero):
    return (mv - zero) / mpa


def mq_ratio(raw, mx):
    if raw <= 0 or raw > mx:
        return float('nan')
    return (mx - raw) / raw


def mq_ppm(ratio, r0, a, b):
    if ratio <= 0 or r0 <= 0:
        return float('nan')
    return a * (ratio / r0) ** b


def ntc_beta_c(raw, adc_max, series_r, beta, r0, t0_c):
    if raw <= 0 or raw >= adc_max or series_r <= 0 or beta <= 0 or r0 <= 0:
        return float('nan')
    r_therm = series_r * (adc_max / raw - 1.0)
    inv_t = (1.0 / (t0_c + 273.15)) + (1.0 / beta) * math.log(r_therm / r0)
    return (1.0 / inv_t) - 273.15


def altitude_from_pressure(pressure_hpa, sea_level_hpa):
    if pressure_hpa <= 0 or sea_level_hpa <= 0:
        return float('nan')
    return 44330.0 * (1.0 - (pressure_hpa / sea_level_hpa) ** 0.1903)


def servo_us(angle, min_us, max_us):
    angle = max(0, min(180, angle))
    return min_us + ((max_us - min_us) * angle) // 180


def stepper_step_delay_us(rpm, steps_per_rev):
    if rpm <= 0 or steps_per_rev <= 0:
        return 0
    return int(60000000.0 / (rpm * steps_per_rev))


def debounce_step(stable, raw, now_ms, last_change, debounce_ms):
    if raw != stable:
        if (now_ms - last_change[0]) >= debounce_ms:
            last_change[0] = now_ms
            return raw
    else:
        last_change[0] = now_ms
    return stable


def map_val(x, in_min, in_max, out_min, out_max):
    if in_max == in_min:
        return out_min
    return (x - in_min) * (out_max - out_min) // (in_max - in_min) + out_min


def constrain_val(val, min_v, max_v):
    if val < min_v:
        return min_v
    if val > max_v:
        return max_v
    return val


def nmea_checksum(sentence):
    cs = 0
    if not sentence:
        return 0
    if sentence[0] == '$':
        sentence = sentence[1:]
    for ch in sentence:
        if ch == '*':
            break
        cs ^= ord(ch)
    return cs


def check(name, cond):
    ok = bool(cond)
    print(("PASS " if ok else "FAIL ") + name)
    if not ok:
        raise SystemExit(1)


# Ultrasonic
check("580us -> 10cm", abs(distance_cm(580) - 10.0) < 1e-6)
check("0us -> NaN", math.isnan(distance_cm(0)))
check("123cm roundtrip", echo_us_from_cm(123) == 123 * 58)
check("nonpositive cm -> 0", echo_us_from_cm(0) == 0 and echo_us_from_cm(-5) == 0)
check("30000us range", 500 < distance_cm(30000) < 600)
check("148us -> 1 inch", abs(distance_inches(148) - 1.0) < 1e-9)

# ADC
check("midscale 5V 10-bit", 2490 <= adc_to_mv(512, 1023, 5.0, 1.0) < 2520)
check("ESP32 fullscale 12-bit", 3290 <= adc_to_mv(4095, 4095, 3.3, 1.0) < 3310)
check("divider x2", 4990 <= adc_to_mv(512, 1023, 5.0, 2.0) < 5030)
check("bad args -> -1", adc_to_mv(-1, 1023, 5.0, 1.0) == -1 and adc_to_mv(0, 0, 5.0, 1.0) == -1)

# Percent
check("percent 512/1023 ~50%", 50.0 <= percent(512, 1023) < 50.1)

# Temperature
check("LM35 500mV -> 50°C", abs(lm35_c(500) - 50.0) < 1e-9)
check("TMP36 750mV -> 25°C", abs(tmp36_c(750) - 25.0) < 1e-9)

# Current
check("ACS712 2500mV -> 0A", abs(acs712_a(2500, 185.0, 2500.0)) < 1e-9)

# Gas MQ
check("MQ ratio 512/1023 ~1.0", 0.99 <= mq_ratio(512, 1023) < 1.01)
check("MQ PPM", abs(mq_ppm(1.0, 1.0, 100, 1.0) - 100) < 1e-9)

# NTC Beta
check("NTC 25°C ~25", abs(ntc_beta_c(512, 1023, 10000, 3950, 10000, 25) - 25.0) < 1.0)

# Altitude
check("sea level -> 0m", abs(altitude_from_pressure(1013.25, 1013.25)) < 1.0)
check("900hPa -> ~1000m", 950 < altitude_from_pressure(900, 1013.25) < 1050)

# Servo
check("servo 0° -> 544us", servo_us(0, 544, 2400) == 544)
check("servo 180° -> 2400us", servo_us(180, 544, 2400) == 2400)
check("servo 90° -> 1472us", servo_us(90, 544, 2400) == 1472)

# Stepper
check("60 RPM, 200 steps/rev -> 5000us", stepper_step_delay_us(60, 200) == 5000)
check("120 RPM, 400 steps/rev -> 1250us", stepper_step_delay_us(120, 400) == 1250)

# Debounce
last = [0]
stable = 1
stable = debounce_step(stable, 0, 10, last, 50)
check("debounce holds at 10ms", stable == 1)
stable = debounce_step(stable, 0, 100, last, 50)
check("debounce flips at 100ms", stable == 0)

# Map
check("map 50% -> 127", map_val(512, 0, 1023, 0, 255) == 127)
check("map invert", map_val(0, 0, 1023, 255, 0) == 255)

# Constrain
check("constrain low", constrain_val(-10, 0, 100) == 0)
check("constrain high", constrain_val(150, 0, 100) == 100)
check("constrain mid", constrain_val(50, 0, 100) == 50)

# NMEA checksum
check("NMEA checksum", nmea_checksum("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47") == 0x47)

print("\nAll host tests passed.")