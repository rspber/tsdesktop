/*
  Setup for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

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

void media_begin(int8_t rotation)
{
#if ILI9341_VERSION < 3     // < v1.2
    rotation = rotation & 1 ? rotation : (rotation + 2) % 4;
#endif
  display.begin(&displaySPI, TFT_RST);
  display.setRotation(rotation);
//  display.invertDisplay(true);    // invert display colors  WHITE <-> BLACK

  touch.begin();
  touch.setRotation(rotation & 1 ? rotation : (2 - rotation) % 4);

  display.clearDisplay();
}
