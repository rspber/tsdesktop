/*
  Font container

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#pragma once

#include <stdint.h>
#include <gfxfont.h>

#ifndef uchar
typedef unsigned char uchar;
#endif

struct t_font_t {
  const GFXfont** gfx_fonts;  // private
  int8_t fontSizeX, fontSizeY;

  void setFont(const GFXfont** gfxFont);
  const GFXfont** getFont();

  // some magic code
  void cursorAdjust(GFXfont* gfxFont, int16_t* x, int16_t* y);

  const GFXglyph* getCharGlyph(GFXfont** g, char** c);

  const int16_t yAdvHeight(const int16_t yAdvance);

  // character size
  // parameters:
  //   c - character
  //   *letterHeight - returns character height
  // returns character width,
  const int16_t charSize(char** c, int16_t* letterHeight);  // utf-8
  const int16_t charSize(const void** c, const bool unicode, int16_t* letterHeight);

  const int16_t defaultCharSize(int16_t* letterHeight);

  // text line size to \n or eol
  // parameters:
  //   text - begin of text line in text
  //   *textHeight - returns text line height
  // returns text line width,
  const int16_t textLineSize(const void* textp, const bool unicode, int16_t* textHeight, int16_t* nOfChars);

  // approximately len chars line size
  // parameters:
  //   len - number of chars
  // returns text line width,
  const int16_t textApproxLineWidth(const void* textp, const bool unicode, const int16_t len);

  // text line width to \n or eol
  // parameters:
  //   text - begin of text line in text
  // returns text line width,
  const int16_t textLineWidth(const void* textp, const bool unicode);

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
  const int16_t textSize(const void* textp, const bool unicode, int16_t* textHeight);

// private
  void initialize();
  uchar* seq_scan_glyph(GFXfont** g, uchar* p);
  uchar* bin_search_glyph(GFXfont** g, uchar* p);
  int16_t v_n;     // number of all entries
  int8_t  v_n0;    // number of ASCII entries
  uint8_t v_st_1;  // fd - bin,  fe - seq
  uint8_t v_st_2;  // fe - bin,  fd - seq
};

typedef t_font_t font_t;

int textLength(const void* textp, const bool unicode);
const void* textChr(const void* textp, const bool unicode, const char c);
int textCmp(const void* t1, const void* t2, const bool unicode);

const int toUtf8(char* buf, const uint16_t ucode);
const int utf8CharLen(const char* utf8);
const int unicodeToUtf8(char* utf8, const int size, const uint16_t* unicode);

const uint16_t toUnicode(char** c);
const int utf8ToUnicode(uint16_t* unicode, const int size, const char* utf8);
