/*
  ESP32 TFT SPI for Arduino

  Copyright (c) 2024, rspber (https://github.com/rspber)

  Based on:

  https://github.com/Bodmer/TFT_eSPI/Processors/TFT_eSPI_ESP32.h
  https://github.com/Bodmer/TFT_eSPI/Processors/TFT_eSPI_ESP32.c

*/

#include <Setup.h>

#if defined(TFT_SPI_WRITE)

#include "ESP32_TFT_SPI.h"
#include "SPI.h"
#include "soc/spi_reg.h"
//#include "driver/spi_master.h"
#include <Arduino.h>

/*
ESP32:
FSPI not defined
HSPI = 2, uses SPI2
VSPI = 3, uses SPI3

ESP32-S2:
FSPI = 1, uses SPI2
HSPI = 2, uses SPI3
VSPI not defined

ESP32 C3:
FSPI = 0, uses SPI2 ???? To be checked
HSPI = 1, uses SPI3 ???? To be checked
VSPI not defined

For ESP32/S2/C3:
SPI1_HOST = 0
SPI2_HOST = 1
SPI3_HOST = 2
*/

// ESP32 specific SPI port selection
#ifdef USE_HSPI_PORT
  #ifdef CONFIG_IDF_TARGET_ESP32
    #define SPI_PORT HSPI  //HSPI is port 2 on ESP32
  #else
    #define SPI_PORT 3     //HSPI is port 3 on ESP32 S2
  #endif
#elif defined(USE_FSPI_PORT)
    #define SPI_PORT 2 //FSPI(ESP32 S2)
#else
  #ifdef CONFIG_IDF_TARGET_ESP32
    #define SPI_PORT VSPI
  #else
    #define SPI_PORT 2 //FSPI(ESP32 S2)
  #endif
#endif

  // Volatile for register reads:
  volatile uint32_t* _spi_cmd       = (volatile uint32_t*)(SPI_CMD_REG(SPI_PORT));
//  volatile uint32_t* _spi_user      = (volatile uint32_t*)(SPI_USER_REG(SPI_PORT));
  // Register writes only:
  volatile uint32_t* _spi_mosi_dlen = (volatile uint32_t*)(SPI_MOSI_DLEN_REG(SPI_PORT));
  volatile uint32_t* _spi_w         = (volatile uint32_t*)(SPI_W0_REG(SPI_PORT));

#ifdef CONFIG_IDF_TARGET_ESP32
  #ifdef USE_HSPI_PORT
    SPIClass e_SPI_ = SPIClass(HSPI);
  #elif defined(USE_FSPI_PORT)
    SPIClass e_SPI_ = SPIClass(FSPI);
  #else // use default VSPI port
    SPIClass e_SPI_ = SPIClass(VSPI);
  #endif
#else
  #ifdef USE_HSPI_PORT
    SPIClass e_SPI_ = SPIClass(HSPI);
  #elif defined(USE_FSPI_PORT)
    SPIClass e_SPI_ = SPIClass(FSPI);
  #else // use FSPI port
    SPIClass& e_SPI_ = SPI;
  #endif
#endif

  SPIClass* esp32_SPI = &e_SPI_;

#define TFT_CASET       0x2A    // Column address set
#define TFT_PASET       0x2B    // Page address set
#define TFT_RAMWR       0x2C    // Memory write
#define TFT_RAMRD       0x2E    // Memory read
#define TFT_IDXRD       0xD9    // undocumented

  #if (TFT_SPI_DC >= 32)
    #define SPI_DC_C    GPIO_32_PIN_LOW(TFT_SPI_DC)
    #define SPI_DC_D    GPIO_32_PIN_HIGH(TFT_SPI_DC)
  #elif (TFT_DC >= 0)
    #define SPI_DC_C    GPIO_00_PIN_LOW(TFT_SPI_DC)
    #define SPI_DC_D    GPIO_00_PIN_HIGH(TFT_SPI_DC)
  #else
    #define SPI_DC_C
    #define SPI_DC_D
  #endif

  #if (TFT_SPI_CS >= 32)
    #define SPI_CS_L    GPIO_32_PIN_LOW(TFT_SPI_CS);   GPIO_32_PIN_LOW(TFT_SPI_CS)
    #define SPI_CS_H    GPIO_32_PIN_HIGH(TFT_SPI_CS)
  #elif (TFT_SPI_CS >= 0)
    #define SPI_CS_L    GPIO_00_PIN_LOW(TFT_SPI_CS);   GPIO_00_PIN_LOW(TFT_SPI_CS)
    #define SPI_CS_H    GPIO_00_PIN_HIGH(TFT_SPI_CS)
  #else
    #define SPI_CS_L
    #define SPI_CS_H
  #endif

#if defined(TFT_SPI_WRITE)
  SPISettings settings_cmd;
  SPISettings settings_write;
#endif
#if defined(TFT_SPI_READ)
  SPISettings settings_read;
#endif

extern void initPin(const int16_t pin, PinMode mode);
extern void tft_hardReset(const int16_t RST);

void esp32_spi_initBus()
{
#ifdef TOUCH_SPI_CS 
  initPin(TOUCH_SPI_CS, OUTPUT);
#endif
#ifdef TFT_SPI_CS
  initPin(TFT_SPI_CS, OUTPUT);
#endif
#ifdef TFT_SPI_DC
  initPin(TFT_SPI_DC, OUTPUT);
#endif
#if defined(TFT_SPI_WRITE)
  settings_cmd = SPISettings(TFT_SPI_SETUP_SPEED, MSBFIRST, SPI_MODE0);
  settings_write = SPISettings(TFT_SPI_WRITE_SPEED, MSBFIRST, SPI_MODE0);
#endif
#if defined(TFT_SPI_READ)
  settings_read = SPISettings(TFT_SPI_READ_SPEED, MSBFIRST, SPI_MODE0);
#endif
#ifdef TFT_SPI_RST
  initPin(TFT_SPI_RST, OUTPUT);
  tft_hardReset(TFT_SPI_RST);
#endif
}

#define SPI_SEND(C, L) \
  *_spi_w = C; \
  *_spi_mosi_dlen = L-1; \
  *_spi_cmd = SPI_USR; \
  while (*_spi_cmd & SPI_USR)

inline void spi_send(const uint8_t b)
{
  SPI_SEND(b, 8);
}

inline void spi_send16(const uint16_t w)
{
  SPI_SEND(w << 8 | w >> 8, 16);
}

inline void spi_send24(const uint32_t c)
{
  SPI_SEND((c & 0xff) << 16 | c & 0xff00 | (c >> 16) & 0xff, 24);
}




const char* tft_identification() { return "ESP32 SPI"; }

void tft_write_begin()
{
  esp32_spi_initBus();
  esp32_SPI->begin();
}

void tft_startWriteCmd()
{
  esp32_SPI->beginTransaction(settings_cmd);
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
  esp32_SPI->beginTransaction(settings_write);
  SPI_CS_L;
}

void tft_endWrite()
{
  SPI_CS_H;
  esp32_SPI->endTransaction();
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

void tft_sendMDTColor(const mdt_t c)
{
  #if MDT_SIZE > 2
    spi_send24(c);
  #else
    spi_send16(c);
  #endif
}

void tft_sendMDTColor(const mdt_t c, int32_t len)
{
  while (--len >= 0) {
  #if MDT_SIZE > 2
    spi_send24(c);
  #else
    spi_send16(c);
  #endif
  }
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





#if defined(TFT_SPI_READ)

void tft_read_begin()
{
}

void tft_startReading()
{
  esp32_SPI->beginTransaction(settings_read);
  SPI_CS_L;
}

void tft_endReading()
{
  SPI_CS_H;
  esp32_SPI->endTransaction();
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
  return esp32_SPI->transfer(cmd);
}

const uint16_t tft_transfer16(const uint8_t cmd)
{
  return esp32_SPI->transfer16(cmd);
}

#endif
#endif
