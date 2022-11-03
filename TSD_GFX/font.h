/*
  Font container

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include <stdint.h>
#include <gfxfont.h>

typedef struct {
  GFXfont* gfxFont;
  int8_t fontSizeX, fontSizeY;

  // some magic code
  void cursorAdjust(int16_t* x, int16_t* y);

  // character size
  // parameters:
  //   c - character
  //   *letterHeight - returns character height
  // returns character width,
  const int16_t charSize(const char c, int16_t* letterHeight);

  // text line size to \n or eol
  // parameters:
  //   text - begin of text line in text
  //   *textHeight - returns text line height
  // returns text line width,
  const int16_t textLineSize(const char* text, int16_t* textHeight);

  // approximately len chars line size
  // parameters:
  //   len - number of chars
  // returns text line width,
  const int16_t textLineWidth(const int16_t len);

  // text line width to \n or eol
  // parameters:
  //   text - begin of text line in text
  // returns text line width,
  const int16_t textLineWidth(const char* text);

  // text line height to \n or eol
  // parameters:
  //   text - begin of text line in text
  // returns text line height,
  const int16_t textLineHeight();

  // text size
  // parameters:
  //   text - text with \n characters
  //   *textHeight - returns text height including line breaking
  // returns maximum text line width,
  const int16_t textSize(const char* text, int16_t* textHeight);

} font_t;

