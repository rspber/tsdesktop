/*
  Graphics library for TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

  More information in TSD_GFX.cpp

*/

#pragma once

#include "TSD_GFX.h"
#include <t_cursor.h>
#include "tsd_font.h"

class TSD_CHAR : public TSD_GFX {
public:
  TSD_CHAR() : TSD_GFX() {}

  // utf-8
  void drawChar(clip_t& clip, cursor_t& cursor, tsd_font_t& font, char** c, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);
  const char* drawTextLine(clip_t& clip, cursor_t& cursor, tsd_font_t& font, const char* text, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);

  // unicode
  void drawChar(clip_t& clip, cursor_t& cursor, tsd_font_t& font, const uint16_t uchar, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);
  const uint16_t* drawTextLine(clip_t& clip, cursor_t& cursor, tsd_font_t& font, const uint16_t* utext, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing);

};
