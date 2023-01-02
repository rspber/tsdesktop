/*
  Setup for TSDesktop

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#include "Setup.h"
#include "Display.h"
#include "Touch.h"
#include "Wire.h"
#include "RP2040_TFT_SPI.h"

void init_hardware()
{
}

void init_spi()
{
}

void init_i2c0()
{
  Wire.setSDA(I2C0_SDA_PIN);
  Wire.setSCL(I2C0_SCL_PIN);
  Wire.begin();
  Wire.setClock(I2C0_SPEED);
}

void init_i2c1()
{
  Wire1.setSDA(I2C1_SDA_PIN);
  Wire1.setSCL(I2C1_SCL_PIN);
  Wire1.begin();
  Wire1.setClock(I2C1_SPEED);
}

RP2040_TFT_SPI displaySPI(TFT_CS, SPI0_DC, TFT_SPI_WRITE_SPEED);

#define MAD_MY 0x80
#define MAD_MX 0x40
#define MAD_MV 0x20
#define MAD_SS 0x02
#define MAD_GS 0x01

void media_begin(uint8_t rotation)
{
  display.begin(&displaySPI, TFT_RST);

#if defined(GC9A01)
  uint8_t rev = 1 - REVERSE_MODE;
#else
  uint8_t rev = REVERSE_MODE;
#endif

#if defined(HX8357D)
  uint8_t REV = rev ? 0 : MAD_MY;
#else
  uint8_t REV = rev ? MAD_MX : 0;
#endif

  display.setRotation(rotation, REV);

//  display.invertDisplay(true);    // invert display colors  WHITE <-> BLACK

  touch.begin();

#if defined(ILI9341) || defined(ILI9481) || defined(ILI9486) || defined(ILI9488)
  rotation = rotation & 1 ? rotation : ((rotation + 2) % 4);
#endif  

  touch.setRotation(rotation, rev);

  display.clearDisplay();
}
