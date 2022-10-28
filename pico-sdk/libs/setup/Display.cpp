/*
  Graphics display adapter for TSDesktop
       implemented on TSD_ILI9341

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "Display.h"

void Display::drawText(const int16_t aLeft, const int16_t aTop, const char* aText, const int8_t aFontSize, const uint16_t aTextColor, const int8_t aSpacing)
{
  cursor_t cursor{ aLeft, aTop };
  font_t font{ NULL, aFontSize, aFontSize };
  drawTextLine(&cursor, &font, aText, aTextColor, aSpacing);
}

void Display::clearDisplay()
{
  fillScreen(BLACK);
}
