// ============================================================
//  AARUARDUINO Native Host Tests - SSD1306 framebuffer
//  MADE BY AARUTO
//
//  Exercises the real driver against a host build of the I2C engine, so the
//  framebuffer, the graphics primitives and the 5x7 font renderer are all
//  verified without any hardware.
//
//  Build & run:
//     g++ -std=c++11 -I src -I src/engines -I src/drivers
//         -o ssd1306_tests tests/host/native/test_ssd1306.cpp
//         src/drivers/AaruSSD1306.cpp src/engines/AaruI2CEngine.cpp
//     ./ssd1306_tests
// ============================================================
#include <cstdio>
#include <cstring>

#include "drivers/AaruSSD1306.h"
#include "engines/AaruI2CEngine.h"

static int g_pass = 0;
static int g_fail = 0;

static void check(bool cond, const char* what) {
    if (cond) { g_pass++; printf("  PASS  %s\n", what); }
    else { g_fail++; printf("  FAIL  %s\n", what); }
}

int main() {
    printf("AARUARDUINO native SSD1306 tests\n");
    printf("--------------------------------\n");

    AaruI2CEngine i2c;
    i2c.begin();

    AaruSSD1306 oled;
    AaruarduinoError e = oled.beginI2C(&i2c, 0x3C, 128, 64);
    check(e == AARUARDUINO_OK, "beginI2C succeeds");
    check(oled.isConfigured(), "driver reports configured");
    check(oled.width() == 128 && oled.height() == 64, "geometry is 128x64");
    check(oled.bufferSize() == 1024, "framebuffer is 1024 bytes");
    check(oled.getBuffer() != nullptr, "framebuffer allocated");

    // --- pixel access ------------------------------------------------------
    oled.clear();
    check(!oled.getPixel(10, 10), "cleared pixel reads false");
    oled.setPixel(10, 10, true);
    check(oled.getPixel(10, 10), "set pixel reads true");
    oled.setPixel(10, 10, false);
    check(!oled.getPixel(10, 10), "cleared pixel reads false again");

    // Out of bounds must not corrupt anything.
    oled.setPixel(-1, 0, true);
    oled.setPixel(0, -1, true);
    oled.setPixel(128, 0, true);
    oled.setPixel(0, 64, true);
    check(!oled.getPixel(-1, 0) && !oled.getPixel(128, 0), "out-of-range writes are ignored");

    // --- page addressing ---------------------------------------------------
    oled.clear();
    oled.setPixel(3, 0, true);     // page 0
    oled.setPixel(3, 8, true);     // page 1
    uint8_t* buf = oled.getBuffer();
    check(buf[3] == 0x01, "page 0 pixel is in byte 3 bit 0");
    check(buf[128 + 3] == 0x01, "page 1 pixel is in byte 128+3 bit 0");

    // --- text --------------------------------------------------------------
    oled.clear();
    oled.setCursor(0, 0);
    size_t n = oled.print("A");
    check(n == 1, "print returns the character count");
    check(oled.getPixel(1, 0), "glyph 'A' has a pixel in its second column");
    bool any = false;
    for (int16_t x = 0; x < 8; x++) {
        for (int16_t y = 0; y < 8; y++) {
            if (oled.getPixel(x, y)) any = true;
        }
    }
    check(any, "glyph 'A' renders pixels");

    oled.clear();
    oled.setCursor(0, 0);
    n = oled.print("Hello");
    check(n == 5, "print returns 5 for \"Hello\"");

    oled.clear();
    oled.setCursor(0, 0);
    n = oled.print(1234);
    check(n == 4, "print(int) returns the digit count");
    // '1' is the leftmost glyph -> column 0 of the first glyph.
    check(oled.getPixel(2, 0), "digit '1' renders pixels");

    oled.clear();
    oled.setCursor(0, 0);
    n = oled.print(-42);
    check(n == 3, "print(negative) includes the sign");

    oled.clear();
    oled.setCursor(0, 0);
    n = oled.print(3.14159f, 2);
    check(n == 4, "print(float, 2) returns 4 characters");
    check(oled.getPixel(2, 0) || oled.getPixel(3, 0), "float renders pixels");

    // --- graphics ----------------------------------------------------------
    oled.clear();
    oled.drawLine(0, 0, 10, 0, true);
    check(oled.getPixel(5, 0), "horizontal line pixel set");
    check(!oled.getPixel(5, 1), "horizontal line does not bleed");

    oled.clear();
    oled.drawRect(2, 2, 6, 4, true);
    check(oled.getPixel(2, 2) && oled.getPixel(7, 5), "rectangle corners set");
    check(!oled.getPixel(4, 3), "rectangle interior is empty");

    oled.clear();
    oled.fillRect(0, 0, 4, 4, true);
    check(oled.getPixel(3, 3), "filled rectangle sets the interior");
    check(!oled.getPixel(4, 3), "filled rectangle respects its size");

    oled.clear();
    oled.drawCircle(16, 16, 8, true);
    check(oled.getPixel(24, 16), "circle right edge set");
    check(!oled.getPixel(16, 16), "circle centre is empty");

    oled.clear();
    oled.fillCircle(16, 16, 6, true);
    check(oled.getPixel(16, 16), "filled circle sets the centre");

    oled.clear();
    const uint8_t bitmap[2] = {0xF0, 0xF0};   // 4 pixels wide, 2 rows
    oled.drawBitmap(0, 0, bitmap, 4, 2, true);
    check(oled.getPixel(0, 0) && oled.getPixel(3, 0), "bitmap row 0 rendered");
    check(oled.getPixel(0, 1) && oled.getPixel(3, 1), "bitmap row 1 rendered");

    printf("--------------------------------\n");
    printf("%d passed, %d failed\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
