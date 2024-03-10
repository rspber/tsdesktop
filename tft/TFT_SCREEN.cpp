/*
  TFT SCREEN

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include "TFT_SCREEN.h"
#include <overlaid.h>

#include <TFT_API.h>

// pio implementation conflict
#if defined(ARDUINO_ARCH_RP2040)
  #include <api/Common.h>
#else
  #include <Arduino.h>
#endif

#include <stdlib.h>

#include "TFT_Defines.hh"

const char* TFT_SCREEN::protocol() { return tft_identification(); }

void TFT_SCREEN::begin()
{
  tft_write_begin();
  tft_read_begin();

  tft_startWriteCmd();

#include "TFT_Init.hh"

  tft_endWrite();
}

void TFT_SCREEN::setRotation(const uint8_t r, const uint8_t REV)
{
  tft_startWriteCmd();

#include "TFT_Rotation.hh"

  if (r & 1) {
    setSize(getHEIGHT(), getWIDTH());
  }
  else {
    setSize(getWIDTH(), getHEIGHT());
  }
  tft_endWrite();
}

void v_drawPixel1(const int16_t x, const int16_t y, const rgb_t color)
{
  tft_writeAddrWindow(x, y, 1, 1);
  tft_sendMDTColor(mdt_color(color));
}

void v_drawPixels(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const rgb_t color)
{
  tft_writeAddrWindow(x, y, w, h);
  tft_sendMDTColor(mdt_color(color), w * h);
}

void v_drawMDTBuffer(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const uint8_t* buffer)
{
  tft_writeAddrWindow(x, y, w, h);
  if (MDT_SIZE > 2) {
    tft_sendMDTBuffer24(buffer, w * h);
  }
  else {
    tft_sendMDTBuffer16(buffer, w * h);
  }
}

void TFT_SCREEN::startWrite()
{
  tft_startWrite();
}

void TFT_SCREEN::endWrite()
{
  tft_endWrite();
}

void TFT_SCREEN::sendCmd(const uint8_t cmd)
{
  tft_sendCmd(cmd);
}

void TFT_SCREEN::sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t size)
{
  tft_sendCmdData(cmd, data, size);
}

void TFT_SCREEN::sendCmdByte(const uint8_t cmd, const uint8_t b)
{
  tft_sendCmdData(cmd, &b, 1);
}

void TFT_SCREEN::writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h)
{
  tft_writeAddrWindow(x, y, w, h);
}

void TFT_SCREEN::sendMDTColor1(const mdt_t c)
{
  tft_sendMDTColor(c);
}

void TFT_SCREEN::sendMDTColor(const mdt_t c, const int32_t len)
{
  tft_sendMDTColor(c, len);
}

void TFT_SCREEN::drawMDTBuffer(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const uint8_t* buffer)
{
  tft_startWrite();
  v_drawMDTBuffer(x, y, w, h, buffer);
  tft_endWrite();
}

void v_storePixels(const int16_t x, const int16_t y, const int16_t w, const int16_t h, over_t* t);

void TFT_SCREEN::drawPixel1(const int16_t x, const int16_t y, const rgb_t color)
{
#ifdef OVERLAID
  // very dubious method to detect pointer in rgb_t type
  // in rp2040 pointers are 4 byte:
  // code: 1x xx xx xx
  // data: 2x xx xx xx
  // RGB(r,g,b) adds 0xFF000000 to color definition
  if ((color & 0xFF000000) != 0xFF000000) {	// not a RGB color
    over_t* t = (over_t*)color;
    if (t->mode == 1) {   // store underlaing pixels
      v_storePixels(x, y, 1, 1, t);
    }
    if (t->mode == 2) {   // restore background from buf
      v_drawMDTBuffer(x, y, 1, 1, &t->buf[t->len]);
      t->len += MDT_SIZE;
      return;
    }
    v_drawPixel1(x, y, t->color);
  }
  else {
    v_drawPixel1(x, y, color);
  }
#else
  v_drawPixel1(x, y, color);
#endif
}

void TFT_SCREEN::drawPixels(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const rgb_t color)
{
#ifdef OVERLAID
  // very dubious method to detect pointer in rgb_t type
  // in rp2040 pointers are 4 byte:
  // code: 1x xx xx xx
  // data: 2x xx xx xx
  // RGB(r,g,b) adds 0xFF000000 to color definition
  if ((color & 0xFF000000) != 0xFF000000) {	// not a RGB color
    over_t* t = (over_t*)color;
    if (t->mode == 1) {   // store underlaing pixels
      v_storePixels(x, y, w, h, t);
    }
    if (t->mode == 2) {   // restore background from buf
      v_drawMDTBuffer(x, y, w, h, &t->buf[t->len]);
      t->len += w * h * MDT_SIZE;
      return;
    }
    v_drawPixels(x, y, w, h, t->color);
  }
  else {
    v_drawPixels(x, y, w, h, color);
  }
#else
  v_drawPixels(x, y, w, h, color);
#endif
}



void initPin(const int16_t pin, PinMode mode)
{
  if (pin >= 0) {
    pinMode(pin, mode);
    digitalWrite(pin, HIGH);
  }
}

void tft_hardReset(const int16_t RST)
{
  // Toggle RST low to reset
  if (RST >= 0) {
//    sendCmd(0x00); // Put SPI bus in known state for TFT with CS tied low
//    digitalWrite(RST, HIGH);
//    delay(5);
    digitalWrite(RST, LOW);
    delay(20);
    digitalWrite(RST, HIGH);
  }
  delay(150); // Wait for reset to complete
}


void tft_softReset()
{
  tft_startWriteCmd();   // slow down
  tft_sendCmd(TFT_SWRESET); // Engage software reset
  tft_endWrite();
  delay(150);
}




void TFT_SCREEN::displayOff()
{
  tft_startWriteCmd();
  tft_sendCmd(TFT_DISPOFF);
  tft_endWrite();
}

void TFT_SCREEN::displayOn()
{
  tft_startWriteCmd();
  tft_sendCmd(TFT_DISPON);
  tft_endWrite();
}

void TFT_SCREEN::invertDisplay(bool invert)
{
  tft_startWriteCmd();
  tft_sendCmd(invert ? TFT_INVON : TFT_INVOFF);
  tft_endWrite();
}

// currently not used
void TFT_SCREEN::scrollTo(int16_t y) {
  tft_startWriteCmd();
  uint16_t xbuf = swap16(y);
  tft_sendCmdData(TFT_VSCRSADD, (uint8_t*)&xbuf, 2);
  tft_endWrite();
}

// currently not used
void TFT_SCREEN::setScrollMargins(int16_t top, int16_t bottom) {
  // TFA+VSA+BFA must equal 320
  if (top + bottom <= getHEIGHT()) {
    tft_startWriteCmd();
    uint16_t xbuf[3];
    xbuf[0] = swap16(top);
    xbuf[1] = swap16(getHEIGHT() - (top + bottom));
    xbuf[2] = swap16(bottom);
    tft_sendCmdData(TFT_VSCRDEF, (uint8_t*)xbuf, 6);
    tft_endWrite();
  }
}




// read

/**************************************************************************/
/*!
    @brief  Read len * 8 bits of data from ILI9341 register.
       This is highly undocumented, it's really a hack but kinda works?
    @param    buf  The result, first byte is the reg, rest is a data read
    @param    reg  The command register to read data from
    @param    len  The number of bytes to read from register
 */
/***********************+***************************************************/
void tft_readRegister(uint8_t* buf, const uint8_t reg, int8_t len)
{
  tft_startReading();
  if (reg) {
    const uint8_t buf = 0x10 + len;
    tft_sendCmdData(TFT_IDXRD, &buf, 1);
  }
  tft_sendCmd(reg);

  tft_setBUSReadMode();
//  delay(1);

  int i = 0;
  buf[i++] = reg;
  while (--len >= 0) {
    buf[i++] = tft_transfer(0);
  }
  tft_endReading();
}



rgb_t TFT_SCREEN::readPixel(clip_t& clip, int16_t x, int16_t y)
{
  if (x >= clip.x1 && y >= clip.y1 && x < clip.x2 && y < clip.y2) {
    return innerReadPixel(x, y);
  }
  return BLACK;
}


// read

void TFT_SCREEN::readRegister(uint8_t* buf, const uint8_t reg, int8_t len)
{
  tft_readRegister(buf, reg, len);
}

rgb_t TFT_SCREEN::innerReadPixel(int16_t x, int16_t y)
{
  tft_startReading();
  tft_readAddrWindow(x, y, 1, 1);
  tft_setBUSReadMode();
  tft_transfer(0);  // the first is thorough
  rgb_t color;
#if defined(ST7796)
  if (MDT_SIZE <= 2) {
  // Read 2 byte 565 color
    uint8_t b0 = tft_transfer(0);
    uint8_t b1 = tft_transfer(0);
    color = rgb(((uint16_t)b0 << 8) | b1);
  }
  else {
#else
  {
#endif
  // Read the 3 RGB bytes
    uint8_t r = tft_transfer(0);
    uint8_t g = tft_transfer(0);
    uint8_t b = tft_transfer(0);
    color = RGB(r,g,b);
  }
  tft_endReading();
  return color;
}

void v_storePixels(const int16_t x, const int16_t y, const int16_t w, const int16_t h, over_t* t)
{
  tft_endWrite();

  tft_startReading();
  tft_readAddrWindow(x, y, w, h);
  tft_setBUSReadMode();
  tft_transfer(0);  // the first is thorough
  for (int i = w * h; --i >= 0; ) {
    if (!t->buf) {
      t->size = 300;
      t->buf = (uint8_t*)malloc(t->size);
    }
    if (t->len + 3 > t->size) {
      t->size += 100;
      t->buf = (uint8_t*)realloc(t->buf, t->size);
    }
    if (MDT_SIZE > 2) {
      // PIXFMT >= 0x?6
      // Read the 3 RGB bytes
      uint8_t r = tft_transfer(0);
      uint8_t g = tft_transfer(0);
      uint8_t b = tft_transfer(0);
      t->buf[t->len++] = r;
      t->buf[t->len++] = g;
      t->buf[t->len++] = b;
    }
    else {
      uint8_t b0, b1;
#if defined(ST7796)
      // Read 2 byte 565 color
      b0 = tft_transfer(0);
      b1 = tft_transfer(0);
#else
      // Read the 3 RGB bytes
      uint8_t r = tft_transfer(0);
      uint8_t g = tft_transfer(0);
      uint8_t b = tft_transfer(0);
      uint16_t c = RGB565(r,g,b);
      b0 = c >> 8;
      b1 = c & 0xff;
#endif
      t->buf[t->len++] = b0;
      t->buf[t->len++] = b1;
    }
  }
  tft_endReading();

  tft_startWrite();
}
