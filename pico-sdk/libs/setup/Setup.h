/*
  Setup for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <stdint.h>

//SPI0
//#define TFT_MISO 16  // RX
#define TFT_CS   17    // Chip select control pin
//#define TFT_CLK  18  // SCK
//#define TFT_MOSI 19  // TX
#define TFT_RST  20    // Reset pin (could connect to Arduino RESET pin)
#define SPI0_DC  21    // Data Command control pin
//#define TFT_BL       // LED back-light

#define TOUCH_CS 22     // Chip select pin (T_CS) of touch screen

// TouchScreen
// edges of the display
#define TS_LEFT 150
#define TS_TOP 150
#define TS_RIGHT 150
#define TS_BOTTOM 350

void init_hardware();

void media_begin(const int8_t rotation);
