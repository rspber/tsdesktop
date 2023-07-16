/*
  rp2040_i2c

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include "hardware/i2c.h"

#define DEF_I2C_HZ 100 * 1000

// section 0:  gpio's:  i2c0:  0,  1   i2c1:  2,  3
// section 1:  gpio's:  i2c0:  4,  5   i2c1:  6,  7
// section 2:  gpio's:  i2c0:  8,  9   i2c1: 10, 11
// section 3:  gpio's:  i2c0: 12, 13   i2c1: 14, 15
// section 4:  gpio's:  i2c0: 16, 17   i2c1: 18, 19

//#define PICO_DEFAULT_I2C_SDA_PIN 4    // i2c0 SDA pico-sdk default
//#define PICO_DEFAULT_I2C_SCL_PIN 5    // i2c0 SCL pico-sdk default

i2c_inst_t* init_i2c(const uint8_t SDA, const uint8_t SCL, const uint8_t i2c_nr, const uint Hz = DEF_I2C_HZ);

