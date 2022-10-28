/*
  Graphics display adapter for TSDesktop
       implemented on Adafruit_ILI9341

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

/*
  added support for 2.4" TFT SPI 240X320 v1.3
*/

#include "Display.h"
#include "Setup.h"

#define ILI9341_MADCTL      0x36    // Memory access control

#define MADCTL_MY 0x80  ///< Bottom to top
#define MADCTL_MX 0x40  ///< Right to left
#define MADCTL_MV 0x20  ///< Reverse Mode
#define MADCTL_ML 0x10  ///< LCD refresh Bottom to top
#define MADCTL_RGB 0x00 ///< Red-Green-Blue pixel order
#define MADCTL_BGR 0x08 ///< Blue-Green-Red pixel order
#define MADCTL_MH 0x04  ///< LCD refresh right to left

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Display::setRotation(const int8_t rotation)
{
  int8_t r = rotation % 4; // can't be higher than 3
  uint8_t m = 0;

  uint8_t m0, m1, m2, m3;
  if( ILI9341_VERSION < 3 ) {     // < v1.2
    m0 = MADCTL_MX | MADCTL_BGR;
    m2 = MADCTL_MY | MADCTL_BGR;
    m1 = MADCTL_MV | MADCTL_BGR;
    m3 = MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR;
  }
  else {
    m3 = MADCTL_MX;
    m1 = MADCTL_MY;
    m0 = MADCTL_MV;
    m2 = MADCTL_MX | MADCTL_MY | MADCTL_MV;
  }

  switch (r) {
  case 0:
    m = m0;
    _width = ILI9341_TFTWIDTH;
    _height = ILI9341_TFTHEIGHT;
    break;
  case 1:
    m = m1;
    _width = ILI9341_TFTHEIGHT;
    _height = ILI9341_TFTWIDTH;
    break;
  case 2:
    m = m2;
    _width = ILI9341_TFTWIDTH;
    _height = ILI9341_TFTHEIGHT;
    break;
  case 3:
    m = m3;
    _width = ILI9341_TFTHEIGHT;
    _height = ILI9341_TFTWIDTH;
    break;
  }
  sendCommand(ILI9341_MADCTL, &m, 1);
}

void Display::drawText(const int16_t aLeft, const int16_t aTop, const char* aText, const int8_t aFontSize, const uint16_t aTextColor)
{
  setCursor(aLeft, aTop);
  setTextSize(aFontSize);
  setTextColor(aTextColor);
  print(aText);
}

void Display::drawTextLine(cursor_t* cursor, font_t* font, const char* aText, const uint16_t aTextColor)
{
  GFXfont* oldfont = gfxFont;
  setFont(font->gfxFont);
  int16_t x = cursor->x, y = cursor->y;
  font->cursorAdjust(&x, &y);
  setCursor(x, y);
  setTextSize(font->fontSizeX, font->fontSizeY);
  setTextColor(aTextColor);
  print(aText);
  setFont(oldfont);
}

void Display::clearDisplay()
{
  fillScreen(BLACK);
}
