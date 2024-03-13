/*
  STM32 TFT SPI for Arduino

  Copyright (c) 2024, rspber (https://github.com/rspber)

  Based on:

  https://github.com/Bodmer/TFT_eSPI/Processors/TFT_eSPI_STM32.h
  https://github.com/Bodmer/TFT_eSPI/Processors/TFT_eSPI_STM32.c

*/

#include <Setup.h>

#if defined(TFT_SPI_WRITE)

#include "SPI.h"

#include "rgb.h"

#define TFT_CASET       0x2A    // Column address set
#define TFT_PASET       0x2B    // Page address set
#define TFT_RAMWR       0x2C    // Memory write
#define TFT_RAMRD       0x2E    // Memory read
#define TFT_IDXRD       0xD9    // undocumented

////////////////////////////////////////////////////////////////////////////////////////
// Define the DC (TFT Data/Command or Register Select (RS))pin drive code
////////////////////////////////////////////////////////////////////////////////////////
#if !defined (TFT_SPI_DC) || (TFT_SPI_DC < 0)
  #define SPI_DC_C // No macro allocated so it generates no code
  #define SPI_DC_D // No macro allocated so it generates no code
  #undef  TFT_SPI_DC
#else
  // Convert Arduino pin reference Dn or STM pin reference PXn to port and mask
  #define DC_PORT     digitalPinToPort(TFT_SPI_DC)
  #define DC_PIN_MASK digitalPinToBitMask(TFT_SPI_DC)
  // Use bit set reset register
  #define SPI_DC_C DC_DELAY; DC_PORT->BSRR = DC_PIN_MASK<<16
  #define SPI_DC_D DC_DELAY; DC_PORT->BSRR = DC_PIN_MASK
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Define the CS (TFT chip select) pin drive code
////////////////////////////////////////////////////////////////////////////////////////
#if !defined (TFT_SPI_CS) || (TFT_SPI_CS < 0)
  #define SPI_CS_L // No macro allocated so it generates no code
  #define SPI_CS_H // No macro allocated so it generates no code
  #undef  TFT_SPI_CS
#else
  // Convert Arduino pin reference Dx or STM pin reference PXn to port and mask
  #define CS_PORT      digitalPinToPort(TFT_SPI_CS)
  #define CS_PIN_MASK  digitalPinToBitMask(TFT_SPI_CS)
  // Use bit set reset register
  #define SPI_CS_L CS_PORT->BSRR = CS_PIN_MASK<<16
  #define SPI_CS_H CS_PORT->BSRR = CS_PIN_MASK
#endif

#ifndef DC_DELAY
  //#define DC_DELAY delayMicroseconds(1) // Premature BSY clear hardware bug?
  #define DC_DELAY
#endif

  // Use STM32 default SPI port
  #if !defined (TFT_SPI_MOSI) || !defined (TFT_SPI_MISO) || !defined (TFT_SPI_SCLK)
    SPIClass& stm32_SPI = SPI;
  #else
    SPIClass stm32_SPI(TFT_SPI_MOSI, TFT_SPI_MISO, TFT_SPI_SCLK);
  #endif
  // SPI HAL peripheral handle
  SPI_HandleTypeDef spiHal;

  // Use SPI1 as default if not defined
  #ifndef TFT_SPI_PORT
    #define TFT_SPI_PORT 1
  #endif

  #if (TFT_SPI_PORT == 1)
    #define SPI_X SPI1
  #elif (TFT_SPI_PORT == 2)
    #define SPI_X SPI2
  #elif (TFT_SPI_PORT == 3)
    #define SPI_X SPI3
  #elif (TFT_SPI_PORT == 4)
    #define SPI_X SPI4
  #endif

  // Global define is _VARIANT_ARDUINO_STM32_, see board package stm32_def.h for specific variants
  #if defined (STM32F2xx) || defined (STM32F4xx) || defined (STM32F7xx)

    #define STM32_DMA // DMA is available with these processors

    #if (TFT_SPI_PORT == 1)
      // Initialise processor specific SPI and DMA instances - used by init()
      #define INIT_TFT_DATA_BUS spiHal.Instance = SPI1; \
                                dmaHal.Instance = DMA2_Stream3
      // The DMA hard-coding for SPI1 is in TFT_eSPI_STM32.c as follows:
      //     DMA_CHANNEL_3
      //     DMA2_Stream3_IRQn and DMA2_Stream3_IRQHandler()
    #elif (TFT_SPI_PORT == 2)
      // Initialise processor specific SPI and DMA instances - used by init()
      #define INIT_TFT_DATA_BUS spiHal.Instance = SPI2; \
                                dmaHal.Instance = DMA1_Stream4
      // The DMA hard-coding for SPI2 is in TFT_eSPI_STM32.c as follows:
      //     DMA_CHANNEL_4
      //     DMA1_Stream4_IRQn and DMA1_Stream4_IRQHandler()
    #elif (TFT_SPI_PORT == 3)
      // Initialise processor specific SPI and DMA instances - used by init()
      #define INIT_TFT_DATA_BUS spiHal.Instance = SPI3; \
                                dmaHal.Instance = DMA1_Stream5
      // The DMA hard-coding for SPI3 is in TFT_eSPI_STM32.c as follows:
      //     DMA_CHANNEL_4
      //     DMA1_Stream5_IRQn and DMA1_Stream5_IRQHandler()
    #endif

  #elif defined (STM32F1xx)
    // For Blue Pill and STM32F1xx processors with DMA support
    #define STM32_DMA // DMA is available with these processors
    #if (TFT_SPI_PORT == 1)
      #define INIT_TFT_DATA_BUS spiHal.Instance = SPI1; \
                                dmaHal.Instance = DMA1_Channel3
    #elif (TFT_SPI_PORT == 2)
      #define INIT_TFT_DATA_BUS spiHal.Instance = SPI2; \
                                dmaHal.Instance = DMA1_Channel5
    #endif
  #else
    // For STM32 processor with no implemented DMA support (yet)
    #if (TFT_SPI_PORT == 1)
      #define INIT_TFT_DATA_BUS spiHal.Instance = SPI1
    #elif (TFT_SPI_PORT == 2)
      #define INIT_TFT_DATA_BUS spiHal.Instance = SPI2
    #endif
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

void stm32_spi_initBus()
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

inline void spi_send(const uint8_t b)
{
  while(!__HAL_SPI_GET_FLAG(&spiHal, SPI_FLAG_TXE));
  //BSY check must allow for APB clock delay by checking TXE flag first

  *((__IO uint8_t *)&SPI_X->DR) = b;    // TX_FIFO
}

inline void spi_send16(const uint16_t w)
{
  spi_send(w >> 8);
  spi_send(w);
}

inline void spi_endSending()
{
  while(!__HAL_SPI_GET_FLAG(&spiHal, SPI_FLAG_TXE));
  //BSY check must allow for APB clock delay by checking TXE flag first

  while( __HAL_SPI_GET_FLAG(&spiHal, SPI_FLAG_BSY));
}


const char* tft_identification() { return "STM32 SPI"; }

void tft_write_begin()
{
  stm32_spi_initBus();
  stm32_SPI.begin();
}

void tft_startWriteCmd()
{
  stm32_SPI.beginTransaction(settings_cmd);
  SPI_CS_L;
}

void tft_sendCmd(const uint8_t cmd)
{
  SPI_CS_H;
  SPI_CS_L;
  SPI_DC_C;
  spi_send(cmd);
  spi_endSending();
  SPI_DC_D;
}

void tft_sendCmdData(const uint8_t cmd, const uint8_t* data, const int16_t size)
{
  SPI_CS_H;
  SPI_CS_L;
  SPI_DC_C;
  spi_send(cmd);
  spi_endSending();
  SPI_DC_D;
  for (int16_t i = 0; i < size; ++i) {
    spi_send(data[i]);
  }
  spi_endSending();
}

void tft_startWrite()
{
  stm32_SPI.beginTransaction(settings_write);
  SPI_CS_L;
}

void tft_endWrite()
{
  SPI_CS_H;
  stm32_SPI.endTransaction();
}

void tft_writeAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h)
{
  SPI_DC_C;
  spi_send(TFT_CASET);
  spi_endSending();
  SPI_DC_D;
  spi_send16(x);
  spi_send16(x + w - 1);
  spi_endSending();
  SPI_DC_C;
  spi_send(TFT_PASET);
  spi_endSending();
  SPI_DC_D;
  spi_send16(y);
  spi_send16(y + h - 1);
  spi_endSending();
  SPI_DC_C;
  spi_send(TFT_RAMWR);
  spi_endSending();
  SPI_DC_D;
}

void tft_sendMDTColor(const mdt_t c)
{
  #if MDT_SIZE > 2
    spi_send(c >> 16);
  #endif
    spi_send(c >> 8);
    spi_send(c);

  spi_endSending();
}

void tft_sendMDTColor(const mdt_t c, int32_t len)
{
  while (--len >= 0) {
  #if MDT_SIZE > 2
    spi_send(c >> 16);
  #endif
    spi_send(c >> 8);
    spi_send(c);
  }
  spi_endSending();
}

void tft_sendMDTBuffer16(const uint8_t* p, int32_t len)
{
  while (--len >= 0) {
    spi_send(*p++);
    spi_send(*p++);
  }
  spi_endSending();
}

void tft_sendMDTBuffer24(const uint8_t* p, int32_t len)
{
  while (--len >= 0) {
    spi_send(*p++);
    spi_send(*p++);
    spi_send(*p++);
  }
  spi_endSending();
}





#if defined(TFT_SPI_READ)

void tft_read_begin()
{
}

void tft_startReading()
{
  stm32_SPI.beginTransaction(settings_read);
  SPI_CS_L;
}

void tft_endReading()
{
  SPI_CS_H;
  stm32_SPI.endTransaction();
}

void tft_readAddrWindow(const int16_t x, const int16_t y, const int16_t w, const int16_t h)
{
  SPI_DC_C;
  spi_send(TFT_CASET);
  spi_endSending();
  SPI_DC_D;
  spi_send16(x);
  spi_send16(x + w - 1);
  spi_endSending();
  SPI_DC_C;
  spi_send(TFT_PASET);
  spi_endSending();
  SPI_DC_D;
  spi_send16(y);
  spi_send16(y + h - 1);
  spi_endSending();
  SPI_DC_C;
  spi_send(TFT_RAMRD);
  spi_endSending();
  SPI_DC_D;
}

const uint8_t tft_transfer(const uint8_t cmd)
{
  return stm32_SPI.transfer(cmd);
}

const uint16_t tft_transfer16(const uint8_t cmd)
{
  return stm32_SPI.transfer16(cmd);
}

#endif
#endif
