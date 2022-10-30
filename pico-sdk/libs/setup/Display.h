/*
  Graphics display adapter for TSDesktop
       implemented on TSD_ILI9341

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include "TSD_ILI9341.h"
#include <Setup.h>

//#define BLACK        RGB(    0,    0,    0)
#define NAVY         RGB(    0,    0, 0x7B)
#define BLUE         RGB(    0,    0, 0xFF)
#define DARK_GREEN   RGB(    0, 0x7D,    0)
#define DARK_CYAN    RGB(    0, 0x7D, 0x7B)
#define TEAL         RGB(    0, 0x80, 0x80)
#define GREEN        RGB(    0, 0x80,    0)
#define CYAN         RGB(    0, 0xFF, 0xFF)
#define DIMM_CYAN    RGB( 0x09, 0x73, 0xC0)
#define DODGER_BLUE  RGB( 0x1E, 0x90, 0xFF)
#define CADET_BLUE   RGB( 0x5F, 0x9E, 0xA0)
#define AQUA         RGB( 0x68, 0xC0, 0xE0)
#define MAROON       RGB( 0x7B,    0,    0)
#define PURPLE       RGB( 0x7B,    0, 0x7B)
#define OLIVE        RGB( 0x7B, 0x7D,    0)
#define GRAY         RGB( 0x7B, 0x7D, 0x7B)
#define LIME         RGB( 0x80, 0xFC,    0)
#define VIOLET       RGB( 0xA0, 0x30, 0xC8)
#define SILVER       RGB( 0xC0, 0xC0, 0x80)
#define BROWN        RGB( 0x80, 0x2A, 0x2A)
//#define BROWN        RGB( 0x96, 0x4B, 0x00)
#define GREEN_YELLOW RGB( 0xAD, 0xFF, 0x29)
#define LIGHT_GRAY   RGB( 0xD3, 0xD3, 0xD3)
#define LIGHT_CYAN   RGB( 0xE0, 0xFF, 0xFF)
#define RED          RGB( 0xFF,    0,    0)
#define MAGENTA      RGB( 0xFF,    0, 0xFF)
#define CORAL        RGB( 0xFF, 0x7F, 0x50)
#define PINK         RGB( 0xFF, 0x82, 0xC6)
//#define ORANGE       RGB( 0xFF, 0x98,    0)
#define ORANGE       RGB( 0xFF, 0xA5,    0)
#define GOLDEN       RGB( 0xFF, 0xD7,    0)
#define YELLOW       RGB( 0xFF, 0xFF,    0)
#define LIGHT_YELLOW RGB( 0xFF, 0xFF, 0xE0)
//#define WHITE        RGB( 0xFF, 0xFF, 0xFF)

class Display : public TSD_ILI9341 {
public:
  Display() : TSD_ILI9341(DISPLAY_WIDTH, DISPLAY_HEIGHT) {}

  void drawText(const int16_t aLeft, const int16_t aTop, const char* aText, const int8_t aFontSize = 1, const uint16_t aTextColor = WHITE, const int8_t aSpacing = 0);
  void clearDisplay();
};

extern bool screenEnabled;
extern Display display;

