/*
  Setup for TSDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <stdint.h>
#include "Picoino.h"

#define COLOR_565     // comment it for 666 colors

#define ILI9341_VERSION 0    // no version old devices  - tested
// TouchScreen
// edges of the display for no version models
#define TS_LEFT 150
#define TS_TOP 150
#define TS_RIGHT 150
#define TS_BOTTOM 400

//#define ILI9341_VERSION 1    // v1.0  ?
//#define ILI9341_VERSION 2    // v1.1  ?
//#define ILI9341_VERSION 3    // v1.2  ?   - maybe has changes doing in v1.3, if not set to 2
//#define ILI9341_VERSION 4    // v1.3      - tested
// TouchScreen
// edges of the display for v1.3
//#define TS_LEFT 200
//#define TS_TOP 150
//#define TS_RIGHT 200
//#define TS_BOTTOM 350

#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 320

#define ROTATION_VTB 0    // vertical top to bottom
#define ROTATION_VBT 2    // vertical bottom to top
#define ROTATION_HLR 1    // horizontal left to right
#define ROTATION_HRL 3    // horizontal right to left

//SPI0
//#define TFT_MISO 16  // RX
#define TFT_CS   17    // Chip select control pin
//#define TFT_CLK  18  // SCK
//#define TFT_MOSI 19  // TX
#define TFT_RST  20    // Reset pin (could connect to Arduino RESET pin)
#define SPI0_DC  21    // Data Command control pin
//#define TFT_BL       // LED back-light

#define TOUCH_CS 22     // Chip select pin (T_CS) of touch screen

#define TFT_SPI_SPEED     40 * 1000 * 1000          // 40 MHz
#define TOUCH_SPI_SPEED    2 * 1000 * 1000          // 2 MHz

void init_hardware();

void media_begin(int8_t rotation);
