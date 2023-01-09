/*
  Setup for TSDesktop

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <stdint.h>
#include "Arduino.h"
#include "Wire.h"

#define COLOR_565     // comment it for 666 colors

//#define GC9A01            // -- not tested

//#define HX8357            // define it also if any HX8357*
//#define HX8357B           // -- not tested
//#define HX8357C           // -- not tested
//#define HX8357D           // -- not tested

#define ILI9341       // 2.4" TFT SPI 240x320 no version - tested
// TouchScreen
// edges of the display 2.4" TFT SPI 240x320 no version
#define TS_LEFT 150
#define TS_TOP 400
#define TS_RIGHT 150
#define TS_BOTTOM 150

//#define ILI9481           // define it also if any ILI9481*
//#define ILI9481_1         // -- not tested
//#define ILI9481_AUO317    // -- not tested
//#define ILI9481_CMO35     // -- not tested
//#define ILI9481_CPT29     // -- not tested
//#define ILI9481_PVI35     // -- not tested
//#define ILI9481_RGB       // -- not tested
//#define ILI9481_YA1       // -- not tested
//#define ILI9481_YA2       // -- not tested

//#define ILI9486           // -- not tested
//#define ILI9488           // -- not tested

//#define R61581            // -- not tested
//#define RM68140           // -- not tested

//#define SSD1963           // define it also if any SSD1963*
//#define SSD1963_480       // -- not tested
//#define SSD1963_800       // -- not tested
//#define SSD1963_800ALT    // -- not tested
//#define SSD1963_800BD     // -- not tested

//#define ST7789        // 2.4" TFT SPI 240x320 v1.0, v1.1, v1.2, (v1.3 - tested)
// TouchScreen
// edges of the display 2.4" TFT SPI 240x320 v1.0, v1.1, v1.2, v1.3
//#define TS_LEFT 200
//#define TS_TOP 350
//#define TS_RIGHT 200
//#define TS_BOTTOM 150

//#define ST7796            // -- not tested

#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 320

#define ROTATION_VTB 2    // vertical top to bottom
#define ROTATION_VBT 0    // vertical bottom to top
#define ROTATION_HLR 1    // horizontal left to right
#define ROTATION_HRL 3    // horizontal right to left

#define REVERSE_MODE 0    // 0 or other display & touch orientation

//SPI0
//#define TFT_MISO 16  // RX
#define TFT_CS   17    // Chip select control pin
//#define TFT_CLK  18  // SCK
//#define TFT_MOSI 19  // TX
#define TFT_RST  20    // Reset pin (could connect to Arduino RESET pin)
#define TFT_DC   21    // Data Command control pin
//#define TFT_BL       // LED back-light

#define TOUCH_CS 22     // Chip select pin (T_CS) of touch screen

#define TFT_SETUP_SPEED         2 * 1000 * 1000          // 2 MHz

  #define TFT_WRITE_SPEED    60 * 1000 * 1000          // 60 MHz

  #if defined ST7789
    #define TFT_READ_SPEED    2 * 1000 * 1000          //  2 MHz
  #else
    #define TFT_READ_SPEED   20 * 1000 * 1000          // 20 MHz
  #endif

void init_hardware();

#define I2C0_SDA_PIN 4
#define I2C0_SCL_PIN 5

#define I2C1_SDA_PIN 6
#define I2C1_SCL_PIN 7

TwoWire* init_i2c(const uint8_t i2c_nr);

void init_i2c1();

