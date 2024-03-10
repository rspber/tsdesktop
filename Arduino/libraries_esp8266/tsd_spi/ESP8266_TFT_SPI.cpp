/*
  ESP8266 TFT SPI for Arduino

  Copyright (c) 2024, rspber (https://github.com/rspber)

  Based on:

  https://github.com/Bodmer/TFT_eSPI/Processors/TFT_eSPI_ESP8266.h
  https://github.com/Bodmer/TFT_eSPI/Processors/TFT_eSPI_ESP8266.c

*/

#include <Setup.h>

#if defined(TFT_SPI_WRITE)

#include "SPI.h"
#include <rgb.h>

  volatile uint32_t *dcport, *csport;
  uint32_t cspinmask, dcpinmask, wrpinmask, sclkpinmask;

#define spi SPI

#if (TFT_SPI_DC == 16)
  #define SPI_DC_C digitalWrite(TFT_SPI_DC, LOW)
  #define SPI_DC_D digitalWrite(TFT_SPI_DC, HIGH)
#elif (TFT_SPI_DC >= 0)
  #define SPI_DC_C GPOC=dcpinmask
  #define SPI_DC_D GPOS=dcpinmask
#else
  #define SPI_DC_C
  #define SPI_DC_D
#endif

#if (TFT_SPI_CS == 16)
  #define SPI_CS_L digitalWrite(TFT_SPI_CS, LOW)
  #define SPI_CS_H digitalWrite(TFT_SPI_CS, HIGH)
#elif (TFT_SPI_CS >= 0)
  #define SPI_CS_L GPOC=cspinmask
  #define SPI_CS_H GPOS=cspinmask
#else
  #define SPI_CS_L
  #define SPI_CS_H
#endif

#if defined (TFT_SPI_OVERLAP)
  #undef TFT_SPI_CS
  #define SPI1U_WRITE (SPIUMOSI | SPIUSSE | SPIUCSSETUP | SPIUCSHOLD)
  #define SPI1U_READ  (SPIUMOSI | SPIUSSE | SPIUCSSETUP | SPIUCSHOLD | SPIUDUPLEX)
#else
  #define SPI1U_WRITE (SPIUMOSI | SPIUSSE)
  #define SPI1U_READ  (SPIUMOSI | SPIUSSE | SPIUDUPLEX)
#endif

void tft_setBUSWriteMode()
{
  SPI1U = SPI1U_WRITE;
}

void tft_setBUSReadMode()
{
  SPI1U = SPI1U_READ;
}


#define TFT_CASET       0x2A    // Column address set
#define TFT_PASET       0x2B    // Page address set
#define TFT_RAMWR       0x2C    // Memory write
#define TFT_RAMRD       0x2E    // Memory read
#define TFT_IDXRD       0xD9    // undocumented

#if defined(TFT_SPI_WRITE)
  SPISettings settings_cmd;
  SPISettings settings_write;
#endif
#if defined(TFT_SPI_READ)
  SPISettings settings_read;
#endif

extern void initPin(const int16_t pin, PinMode mode);
extern void tft_hardReset(const int16_t RST);

void esp8266_spi_initBus()
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
  SPI1W0 = C; \
  SPI1U1 = ((L-1) << SPILMOSI) | ((L-1) << SPILMISO); \
  SPI1CMD |= SPIBUSY; \
  while(SPI1CMD & SPIBUSY);

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




const char* tft_identification() { return "ESP8266 SPI"; }

void tft_write_begin()
{
  esp8266_spi_initBus();

  // Legacy bitmasks for GPIO
  #if (TFT_SPI_CS >= 0)
    cspinmask = (uint32_t) digitalPinToBitMask(TFT_SPI_CS);
  #endif

  #if (TFT_SPI_DC >= 0)
    dcpinmask = (uint32_t) digitalPinToBitMask(TFT_SPI_DC);
  #endif

  #if (TFT_SPI_CLK >= 0)
    sclkpinmask = (uint32_t) digitalPinToBitMask(TFT_SPI_CLK);
  #endif

  #if defined (TFT_SPI_OVERLAP) && defined (ARDUINO_ARCH_ESP8266)
    // Overlap mode SD0=MISO, SD1=MOSI, CLK=SCLK must use D3 as CS
    //    pins(int8_t sck, int8_t miso, int8_t mosi, int8_t ss);
    //spi.pins(        6,          7,           8,          0);
    spi.pins(6, 7, 8, 0);
  #endif

  spi.begin();

//  spi.setBitOrder(MSBFIRST);
//  spi.setDataMode(SPI_MODE0);
//  spi.setFrequency(TFT_SPI_WRITE_SPEED);

  if (TFT_SPI_CS >= 0) {
  }
  else {
    spi.setHwCs(1); // Use hardware SS toggling
  }

}

void tft_startWriteCmd()
{
  spi.beginTransaction(settings_cmd);
  SPI_CS_L;
  tft_setBUSWriteMode();
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
  spi.beginTransaction(settings_write);
  SPI_CS_L;
  tft_setBUSWriteMode();
}

void tft_endWrite()
{
  SPI_CS_H;
  spi.endTransaction();
  tft_setBUSReadMode();
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



/*
void TFT_eSPI::pushBlock(uint16_t color, uint32_t len)
{
/*
while (len>1) { tft_Write_32(color<<16 | color); len-=2;}
if (len) tft_Write_16(color);
return;
//* /
  uint16_t color16 = (color >> 8) | (color << 8);
  uint32_t color32 = color16 | color16 << 16;
/*
  while(len--) {
    SPI1U1 = ((16-1) << SPILMOSI) | ((16-1) << SPILMISO);
    SPI1W0 = color16;
    SPI1CMD |= SPIBUSY;
    while(SPI1CMD & SPIBUSY) {}
  }
  return;
//* /

  SPI1W0 = color32;
  SPI1W1 = color32;
  SPI1W2 = color32;
  SPI1W3 = color32;
  if (len > 8)
  {
    SPI1W4 = color32;
    SPI1W5 = color32;
    SPI1W6 = color32;
    SPI1W7 = color32;
  }
  if (len > 16)
  {
    SPI1W8 = color32;
    SPI1W9 = color32;
    SPI1W10 = color32;
    SPI1W11 = color32;
  }
  if (len > 24)
  {
    SPI1W12 = color32;
    SPI1W13 = color32;
    SPI1W14 = color32;
    SPI1W15 = color32;
  }
  if (len > 31)
  {
    SPI1U1 = (511 << SPILMOSI);
    while(len>31)
    {
#if (defined (SPI_FREQUENCY) && (SPI_FREQUENCY == 80000000))
      if(SPI1CMD & SPIBUSY) // added to sync with flag change
#endif
      while(SPI1CMD & SPIBUSY) {}
      SPI1CMD |= SPIBUSY;
      len -= 32;
    }
    while(SPI1CMD & SPIBUSY) {}
  }

  if (len)
  {
    len = (len << 4) - 1;
    SPI1U1 = (len << SPILMOSI);
    SPI1CMD |= SPIBUSY;
    while(SPI1CMD & SPIBUSY) {}
  }

}
*/
/***************************************************************************************
** Function name:           pushPixels - for ESP8266
** Description:             Write a sequence of pixels
***************************************************************************************/
/*
void TFT_eSPI::pushPixels(const void* data_in, uint32_t len){

  if(_swapBytes) {
    pushSwapBytePixels(data_in, len);
    return;
  }

  uint16_t *data = (uint16_t*) data_in;

  uint32_t color[8];

  SPI1U1 = (255 << SPILMOSI) | (255 << SPILMISO);


  while(len>15)
  {
    memcpy(color,data,32);
    data+=16;

    len -= 16;

    // ESP8266 wait time here at 40MHz SPI is ~5.45us
    while(SPI1CMD & SPIBUSY) {}
    SPI1W0 = color[0];
    SPI1W1 = color[1];
    SPI1W2 = color[2];
    SPI1W3 = color[3];
    SPI1W4 = color[4];
    SPI1W5 = color[5];
    SPI1W6 = color[6];
    SPI1W7 = color[7];
    SPI1CMD |= SPIBUSY;
  }

  if(len)
  {
    uint32_t bits = (len*16-1); // bits left to shift - 1

    memcpy(color,data,len<<1);

    while(SPI1CMD & SPIBUSY) {}
    SPI1U1 = (bits << SPILMOSI) | (bits << SPILMISO);
    SPI1W0 = color[0];
    SPI1W1 = color[1];
    SPI1W2 = color[2];
    SPI1W3 = color[3];
    SPI1W4 = color[4];
    SPI1W5 = color[5];
    SPI1W6 = color[6];
    SPI1W7 = color[7];
    SPI1CMD |= SPIBUSY;
  }

  while(SPI1CMD & SPIBUSY) {}

}
*/


#if defined(TFT_SPI_READ)

void tft_read_begin()
{
}

void tft_startReading()
{
  spi.beginTransaction(settings_read);
  SPI_CS_L;
}

void tft_endReading()
{
  SPI_CS_H;
  spi.endTransaction();

  tft_setBUSWriteMode();
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
  return spi.transfer(cmd);
}

const uint16_t tft_transfer16(const uint8_t cmd)
{
  return spi.transfer16(cmd);
}

#endif
#endif
