/*
  Setup for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "Display.h"
#include "Touch.h"

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

void media_begin(const int8_t rotation)
{
  display.begin();
  display.setRotation(rotation);

  touch.begin();
  touch.setRotation(rotation & 1 ? rotation : (2 - rotation) % 4);

  display.clearDisplay();
}
