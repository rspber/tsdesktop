/*
  PIC32 TFT SPI for Arduino

  Copyright (c) 2024, rspber (https://github.com/rspber)

*/

#include <Setup.h>

#if defined(TFT_SPI_WRITE) || defined(TFT_SPI_READ)

#include "PIC32_TFT_SPI.h"
#include "SPI.h"

SPIClass* pic32_SPI = 0;

#define TFT_CASET       0x2A    // Column address set
#define TFT_PASET       0x2B    // Page address set
#define TFT_RAMWR       0x2C    // Memory write
#define TFT_RAMRD       0x2E    // Memory read
#define TFT_IDXRD       0xD9    // undocumented

#if defined(TFT_SPI_CS) && (TFT_SPI_CS >= 0)
  #define SPI_CS_L digitalWrite(TFT_SPI_CS, 0)
  #define SPI_CS_H digitalWrite(TFT_SPI_CS, 1)
#else
  #define SPI_CS_L
  #define SPI_CS_H
#endif

#if defined(TFT_SPI_DC) && (TFT_SPI_DC >= 0)
  #define SPI_DC_C digitalWrite(TFT_SPI_DC, 0)
  #define SPI_DC_D digitalWrite(TFT_SPI_DC, 1)
#else
  #define SPI_DC_C
  #define SPI_DC_D
#endif

#if defined(TFT_SPI_WRITE)
  SPISettings settings_cmd;
  SPISettings settings_write;
#endif
#if defined(TFT_SPI_READ)
  SPISettings settings_read;
#endif

extern void tft_hardReset(const int16_t RST);

void pic32_spi_initBus()
{
#ifdef TOUCH_SPI_CS 
  if (TOUCH_SPI_CS >= 0) {
    pinMode(TOUCH_SPI_CS, OUTPUT);
    digitalWrite(TOUCH_SPI_CS, HIGH);
  }
#endif
  if (TFT_SPI_CS >= 0) {
    pinMode(TFT_SPI_CS, OUTPUT);
    digitalWrite(TFT_SPI_CS, HIGH);
  }

  if (TFT_SPI_DC >= 0) {
    pinMode(TFT_SPI_DC, OUTPUT);
    digitalWrite(TFT_SPI_DC, HIGH);
  }
#if defined(TFT_SPI_WRITE)
  settings_cmd = SPISettings(TFT_SPI_SETUP_SPEED, MSBFIRST, SPI_MODE0);
  settings_write = SPISettings(TFT_SPI_WRITE_SPEED, MSBFIRST, SPI_MODE0);
#endif
#if defined(TFT_SPI_READ)
  settings_read = SPISettings(TFT_SPI_READ_SPEED, MSBFIRST, SPI_MODE0);
#endif
#if defined(TFT_SPI_WRITE)
  if (TFT_SPI_RST >= 0) {
    pinMode(TFT_SPI_RST, OUTPUT);
    digitalWrite(TFT_SPI_RST, HIGH);
  }
  tft_hardReset(TFT_SPI_RST);
#endif
}

void spi_send(const uint8_t data)
{
  pic32_SPI->transfer(data);
}

void spi_send16(const uint16_t w)
{
  spi_send(w >> 8);
  spi_send(w);
}

#endif



#ifdef TFT_SPI_WRITE

const char* tft_identification() { return "PIC32 SPI"; }

void tft_write_begin()
{
  pic32_spi_initBus();
  pic32_SPI = &SPI;
  pic32_SPI->begin();
}

void tft_startWriteCmd()
{
  pic32_SPI->beginTransaction(settings_cmd);
  SPI_CS_L;
}

void tft_sendCmd(const uint8_t cmd)
{
  SPI_CS_H;
  SPI_CS_L;
  SPI_DC_C;
  spi_send(cmd);
  SPI_DC_D;
}

void tft_sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t size)
{
  SPI_CS_H;
  SPI_CS_L;
  SPI_DC_C;
  spi_send(cmd);
  SPI_DC_D;
  for (int16_t i = 0; i < size; ++i) {
    spi_send(data[i]);
  }
}

void tft_startWrite()
{
  pic32_SPI->beginTransaction(settings_write);
  SPI_CS_L;
}

void tft_endWrite()
{
  SPI_CS_H;
  pic32_SPI->endTransaction();
}

void tft_writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h)
{
  SPI_DC_C;
  spi_send(TFT_CASET);
  SPI_DC_D;
  spi_send16(x);
  spi_send16(x + w - 1);
  SPI_DC_C;
  spi_send(TFT_PASET);
  SPI_DC_D;
  spi_send16(y);
  spi_send16(y + h - 1);
  SPI_DC_C;
  spi_send(TFT_RAMWR);
  SPI_DC_D;
}

  void tft_writeMDTColor(const mdt_t c)
  {
  #if MDT_SIZE > 2
    spi_send(c >> 16);
  #endif
    spi_send(c >> 8);
    spi_send(c);
  }

void tft_endWriteColor()
{
}

void tft_sendMDTColor(const mdt_t c, int32_t len)
{
  tft_startWriteColor();
  while (--len >= 0) {
    tft_writeMDTColor(c);
  }
  tft_endWriteColor();
}

void tft_sendMDTBuffer16(const uint8_t* p, int32_t len)
{
  while (--len >= 0) {
    spi_send(*p++);
    spi_send(*p++);
  }
}

void tft_sendMDTBuffer24(const uint8_t* p, int32_t len)
{
  while (--len >= 0) {
    spi_send(*p++);
    spi_send(*p++);
    spi_send(*p++);
  }
}

#endif




#if defined(TFT_SPI_READ)

void tft_read_begin()
{
#ifndef TFT_SPI_WRITE
  pic32_spi_initBus();
#endif
}

void tft_startReading()
{
  if (!pic32_SPI) {
    pic32_SPI = &SPI;
    pic32_SPI->begin();
  }
  pic32_SPI->beginTransaction(settings_read);
  SPI_CS_L;
}

void tft_endReading()
{
  SPI_CS_H;
  pic32_SPI->endTransaction();
}

void tft_readAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h)
{
  SPI_DC_C;
  spi_send(TFT_CASET);
  SPI_DC_D;
  spi_send16(x);
  spi_send16(x + w - 1);
  SPI_DC_C;
  spi_send(TFT_PASET);
  SPI_DC_D;
  spi_send16(y);
  spi_send16(y + h - 1);
  SPI_DC_C;
  spi_send(TFT_RAMRD);
  SPI_DC_D;
}

const uint8_t tft_transfer(const uint8_t cmd)
{
  return pic32_SPI->transfer(cmd);
}

const uint16_t tft_transfer16(const uint8_t cmd)
{
  return pic32_SPI->transfer16(cmd);
}

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
    SPI_CS_H;
    SPI_CS_L;
    SPI_DC_C;
    spi_send(TFT_IDXRD);
    SPI_DC_D;
    spi_send(0x10 + len);
  }
  int i = 0;
  buf[i++] = reg;
  SPI_CS_H;
  SPI_CS_L;
  SPI_DC_C;
  spi_send(reg);
  SPI_DC_D;
//  delay(1);
  while (--len >= 0) {
    buf[i++] = tft_transfer(0);
  }
  tft_endReading();
}

#endif
