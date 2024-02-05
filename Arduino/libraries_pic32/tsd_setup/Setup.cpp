/*
  Setup for TSDesktop

  Copyright (c) 2024, rspber (https://github.com/rspber)

*/

#include "Setup.h"
#include "Wire.h"

#define I2C_SPEED 100 * 1000

TwoWire* init_i2c(const uint8_t i2c_nr)
{
//  if (i2c_nr == 0) {
    Wire.begin();
    Wire.setClock(I2C_SPEED);
    return &Wire;
//  }
}
