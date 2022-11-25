/*
  Font container

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "font.h"

void font_t::cursorAdjust(int16_t* x, int16_t* y)
{
  if (!gfxFont) {  // Default font
    ++*x;
    *y += (fontSizeY + 1) >> 1;
  }
  else {
    //    *x;
    // some magic code
    uint16_t adv = gfxFont->yAdvance;
    if (adv == 18 || adv == 24) {
      adv++;
    }
    else {
      if (adv == 42) {
        adv += 2;
      }
    }
    *y += adv * 3 * fontSizeY / 5;
  }
}

const int16_t font_t::yAdvHeight(const int16_t yAdvance)
{
  // some magic code
  return yAdvance * 2 * fontSizeY / 3; 
}  
  
const int16_t font_t::charSize(const char c, int16_t* letterHeight)
{
  int16_t w, h;
  if (!gfxFont) {  // Default font
    w = 6;
    h = 8 * fontSizeY;
  }
  else {
    uint8_t first = gfxFont->first, last = gfxFont->last;
    if (c >= first && c <= last) {
      GFXglyph* glyph = &gfxFont->glyph[c - first];
      w = glyph->xAdvance;
    }
    else {
      w = 0;
    }
    h = yAdvHeight(gfxFont->yAdvance);
  }
  *letterHeight = h;
  return w * fontSizeX;
}

const int16_t font_t::textLineSize(const char* text, int16_t* textHeight)
{
  int16_t w = 0;
  int16_t h = 0;
  int16_t lh;

  const char* p = text;
  char c;
  while ((c = *p++) && c != '\r' && c != '\n') {
    w += charSize(c, &lh);
    if (lh > h) {
      h = lh;
    }
  }
  if (h == 0) {
    charSize('A', &h);
  }
  *textHeight = h;
  return w;
}

const int16_t font_t::textSize(const char* text, int16_t* textHeight)
{
  const char* p = text;
  int16_t w = 0;
  int16_t h = 0;
  int16_t lw = 0;
  int16_t lh = 0;
  char c = 1;
  while (c) {
    c = *p++;
    if (c != '\r') {
      if (c == 0 || c == '\n') {
        if (lw > w) {
          w = lw;
        }
        lw = 0;
        if (lh == 0) {
          charSize('A', &lh);
        }
        h += lh;
        lh = 0;
      }
      else {
        int16_t lh1;
        lw += charSize(c, &lh1);
        if (lh1 > lh) {
          lh = lh1;
        }
      }
    }
  }
  if (h == 0) {
    charSize('A', &lh);
    h = lh;
  }
  *textHeight = h;
  return w;
}

const int16_t font_t::textLineWidth(int16_t len)
{
  int16_t h;
  return charSize('A', &h) * len;
}

const int16_t font_t::textLineWidth(const char* text)
{
  int16_t h;
  return textLineSize(text, &h);
}

const int16_t font_t::textLineHeight()
{
  int16_t h;
  charSize('A', &h);
  return h;
}
