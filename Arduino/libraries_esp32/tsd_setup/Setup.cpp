/*
  Setup for TSDesktop

  Copyright (c) 2024, rspber (https://github.com/rspber)

*/

#include "Setup.h"
#include "Wire.h"

#define I2C_SPEED 100 * 1000

TwoWire* init_i2c(const uint8_t i2c_nr)
{
  if (i2c_nr == 0) {
    Wire.begin(I2C0_SDA_PIN, I2C0_SCL_PIN, I2C_SPEED);
    return &Wire;
  }
  else {
    Wire1.begin(I2C1_SDA_PIN, I2C1_SCL_PIN, I2C_SPEED);
    return &Wire1;
  }
}
