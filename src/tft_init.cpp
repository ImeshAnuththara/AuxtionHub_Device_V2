#include "tft_init.h"

TFT_init::TFT_init(uint8_t mosi, uint8_t sclk, uint8_t cs, uint8_t dc, uint8_t rst,
                   uint16_t width, uint16_t height) {
    _mosi = mosi;
    _sclk = sclk;
    _cs   = cs;
    _dc   = dc;
    _rst  = rst;
    _width  = width;
    _height = height;
}

void TFT_init::begin() {
    pinMode(_cs, OUTPUT);
    pinMode(_dc, OUTPUT);
    pinMode(_rst, OUTPUT);
    digitalWrite(_cs, HIGH);

    SPI.begin(_sclk, -1, _mosi, _cs);
    SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));

    // Hardware reset
    digitalWrite(_rst, LOW);  delay(100);
    digitalWrite(_rst, HIGH); delay(200);

    // Software reset
    cmd(0x01); delay(150);

    // Sleep out
    cmd(0x11); delay(120);

    // Pixel format (16-bit color) - single data byte OK
    cmd(0x3A); data(0x55);

    // Memory access (MADCTL) - single data byte OK
    cmd(0x36); data(0x00);

    // Display Inversion ON - CRITICAL for ST7789V IPS displays
    cmd(0x21);

    // Porch Setting - 5 data bytes, must keep CS LOW
    cmd(0xB2);
    dataBegin();
    SPI.transfer(0x0C); SPI.transfer(0x0C); SPI.transfer(0x00);
    SPI.transfer(0x33); SPI.transfer(0x33);
    dataEnd();

    // Gate Control
    cmd(0xB7); data(0x35);
    // VCOM Setting
    cmd(0xBB); data(0x19);
    // LCM Control
    cmd(0xC0); data(0x2C);
    // VDV and VRH Command Enable
    cmd(0xC2); data(0x01);
    // VRH Set
    cmd(0xC3); data(0x12);
    // VDV Set
    cmd(0xC4); data(0x20);
    // Frame Rate Control in Normal Mode
    cmd(0xC6); data(0x0F);

    // Power Control 1 - 2 data bytes, must keep CS LOW
    cmd(0xD0);
    dataBegin();
    SPI.transfer(0xA4); SPI.transfer(0xA1);
    dataEnd();

    // Turn Display ON
    cmd(0x29); delay(150);
}

void TFT_init::cmd(uint8_t c) {
    digitalWrite(_dc, LOW);
    digitalWrite(_cs, LOW);
    SPI.transfer(c);
    digitalWrite(_cs, HIGH);
}

void TFT_init::data(uint8_t d) {
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);
    SPI.transfer(d);
    digitalWrite(_cs, HIGH);
}

void TFT_init::dataBegin() {
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);
}

void TFT_init::dataEnd() {
    digitalWrite(_cs, HIGH);
}

void TFT_init::setAddr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    cmd(0x2A);
    dataBegin();
    SPI.transfer(x0 >> 8); SPI.transfer(x0);
    SPI.transfer(x1 >> 8); SPI.transfer(x1);
    dataEnd();

    cmd(0x2B);
    dataBegin();
    SPI.transfer(y0 >> 8); SPI.transfer(y0);
    SPI.transfer(y1 >> 8); SPI.transfer(y1);
    dataEnd();

    cmd(0x2C);
}

void TFT_init::fillScreen(uint16_t color) {
    setAddr(0, 0, _width - 1, _height - 1);
    dataBegin();
    for (uint32_t i = 0; i < (uint32_t)_width * _height; i++) {
        SPI.transfer(color >> 8);
        SPI.transfer(color & 0xFF);
    }
    dataEnd();
}

void TFT_init::pushColors(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, lv_color_t* color_p) {
    setAddr(x0, y0, x1, y1);
    dataBegin();
    for (int i = 0; i < (x1 - x0 + 1) * (y1 - y0 + 1); i++) {
        uint16_t c = color_p->full;
        SPI.transfer(c >> 8);
        SPI.transfer(c & 0xFF);
        color_p++;
    }
    dataEnd();
}

void TFT_init::drawImage(uint16_t x, uint16_t y,
                         uint16_t w, uint16_t h,
                         const uint16_t *image)
{
    setAddr(x, y, x + w - 1, y + h - 1);
    dataBegin();
    for (uint32_t i = 0; i < (uint32_t)w * h; i++) {
        uint16_t color = pgm_read_word(&image[i]);
        SPI.transfer(color >> 8);
        SPI.transfer(color & 0xFF);
    }
    dataEnd();
}
