/*
  RP2040 TOUCH PIO_SPI READ

  Copyright (c) 2023-2024, rspber (https://github.com/rspber)

*/

#include <Setup.h>

#if defined(TFT_PIO_SPI_WRITE) && defined(TOUCH_PIO_SPI)

#include <RP2040_TFT_PIO_SPI.h>
#include <RP2040_TOUCH_PIO_SPI.h>

#if defined(TOUCH_SPI_CS) && (TOUCH_SPI_CS >= 0)
  #define PIO_TOUCH_CS_L sio_hw->gpio_clr = (1ul << TOUCH_SPI_CS)
  #define PIO_TOUCH_CS_H PIO_WAIT_FOR_STALL; sio_hw->gpio_set = (1ul << TOUCH_SPI_CS)
#else
  #define PIO_TOUCH_CS_L
  #define PIO_TOUCH_CS_H
#endif

void rp2040_touch_spi_initBus()
{
}

void touch_begin()
{
}

void touch_startReading()
{
  rp2040_pio_spi_setClockDiv(TOUCH_PIO_SPI_READ_DIV);
  PIO_TOUCH_CS_L;
}

void touch_endReading()
{
  PIO_TOUCH_CS_H;
}

const uint16_t touch_transfer16(const uint8_t cmd)
{
  PIO_DC_C;
  PIO_START_SEND_8;
  PIO_SEND(cmd);
  PIO_DC_D;
  PIO_DC_C;   // according to XPT2046 datasheet, but it's not needed
  tft_setBUSReadMode();
  while (PIO_RX_FIFO_IS_EMPTY);
  uint8_t b1 = PIO_RX_FIFO;
  while (PIO_RX_FIFO_IS_EMPTY);
  uint8_t b0 = PIO_RX_FIFO;
  tft_setBUSWriteMode();
  PIO_DC_D;
  return (b1 << 8) | b0;
}

#endif
