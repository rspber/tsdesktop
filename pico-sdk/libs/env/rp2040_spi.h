/*
  rp2040_spi

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include "hardware/spi.h"

// section 0:  spi0 gpio's: RX(MISO)  0, CS(available)  1, SCK(CLK)  2, TX(MOSI)  3
// section 1:  spi0 gpio's: RX(MISO)  4, CS(available)  5, SCK(CLK)  6, TX(MOSI)  7
// section 2:  spi1 gpio's: RX(MISO)  8, CS(available)  9, SCK(CLK) 10, TX(MOSI) 11
// section 3:  spi1 gpio's: RX(MISO) 12, CS(available) 13, SCK(CLK) 14, TX(MOSI) 15
// section 4:  spi0 gpio's: RX(MISO) 16, CS(available) 17, SCK(CLK) 18, TX(MOSI) 19    // spi0 pico-sdk default

void setup_spi(const uint8_t RX, const uint8_t SCK, const uint8_t TX, const uint8_t spi_nr);
spi_inst_t* init_spi(const uint8_t spi_nr, const uint Hz);
void deinit_spi(spi_inst_t* spi);

void set_spi_speed(spi_inst_t* spi, const uint Hz);

