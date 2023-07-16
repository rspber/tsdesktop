/*
  Setup for TSDesktop

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#include "Setup.h"
#include "Wire.h"

#define I2C_SPEED 100 * 1000

TwoWire* init_i2c(const uint8_t i2c_nr)
{
  if (i2c_nr == 0) {
    Wire.setSDA(I2C0_SDA_PIN);
    Wire.setSCL(I2C0_SCL_PIN);
    Wire.begin();
    Wire.setClock(I2C_SPEED);
    return &Wire;
  }
  else {
    Wire1.setSDA(I2C1_SDA_PIN);
    Wire1.setSCL(I2C1_SCL_PIN);
    Wire1.begin();
    Wire1.setClock(I2C_SPEED);
    return &Wire1;
  }
}
