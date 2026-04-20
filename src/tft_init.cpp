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
    SPI.beginTransaction(SPISettings(80000000, MSBFIRST, SPI_MODE0));

    digitalWrite(_rst, LOW);  delay(50);
    digitalWrite(_rst, HIGH); delay(120);

    cmd(0x11); delay(120);   // Sleep out
    cmd(0x3A); data(0x55);   // Set color mode to 16-bit
    cmd(0x36); data(0x00);   // Memory access
    cmd(0x29);               // Display ON
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

void TFT_init::setAddr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    cmd(0x2A); data(x0 >> 8); data(x0); data(x1 >> 8); data(x1);
    cmd(0x2B); data(y0 >> 8); data(y0); data(y1 >> 8); data(y1);
    cmd(0x2C);
}

void TFT_init::fillScreen(uint16_t color) {
    setAddr(0, 0, _width - 1, _height - 1);
    for (uint32_t i = 0; i < (uint32_t)_width * _height; i++) {
        data(color >> 8);
        data(color & 0xFF);
    }
}

void TFT_init::pushColors(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, lv_color_t* color_p) {
    setAddr(x0, y0, x1, y1);  // private is fine because called from inside the class
    for (int i = 0; i < (x1 - x0 + 1) * (y1 - y0 + 1); i++) {
        uint16_t c = color_p->full;
        data(c >> 8);
        data(c & 0xFF);
        color_p++;
    }
}

void TFT_init::drawImage(uint16_t x, uint16_t y,
                         uint16_t w, uint16_t h,
                         const uint16_t *image)
{
    setAddr(x, y, x + w - 1, y + h - 1);

    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);

    for (uint32_t i = 0; i < (uint32_t)w * h; i++) {
        uint16_t color = pgm_read_word(&image[i]);  // read from PROGMEM
        SPI.transfer(color >> 8);
        SPI.transfer(color & 0xFF);
    }

    digitalWrite(_cs, HIGH);
}
