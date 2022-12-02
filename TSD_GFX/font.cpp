/*
  Font container

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "font.h"

void font_t::setFont(const GFXfont** gfxFont)
{
  gfx_fonts = gfxFont;
  v_st_1 = 0;
}

const GFXfont** font_t::getFont()
{
  return gfx_fonts;
}

void font_t::initialize()
{
  v_n = 0;
  v_n0 = 0;
  int16_t u = 0;
  uchar c1 = 0, c2 = 0, c3 = 0, c = 0;
  const GFXfont* f;
  bool ok = true;
  while ((f = gfx_fonts[u++])) {
    if (f->ch1 < c1) {
      ok = false;
      break;
    }
    if (f->ch1 > c1) {
      c1 = f->ch1;
      c2 = f->ch2;
      c3 = f->ch3;
      c = f->first;
    }
    else { // ==
      if (f->ch2 < c2) {
        ok = false;
        break;
      }
      if (f->ch2 > c2) {
        c2 = f->ch2;
        c3 = f->ch3;
        c = f->first;
      }
      else { // ==
        if (f->ch3 < c3) {
          ok = false;
          break;
        }
        if (f->ch3 > c3) {
          c3 = f->ch3;
          c = f->first;
        }
        else { // ==
          if (f->first < c) {
            ok = false;
            break;
          }
          c = f->first;
        }
      }
    }
    ++v_n;
    if (f->ch1 == 0) {
      ++v_n0;;
    }
  }
  if (ok) {
    v_st_1 = 0xfd;  // bin
    v_st_2 = 0xfe;  // bin
  }
  else {
    v_st_1 = 0xfe;  // seq
    v_st_2 = 0xfd;  // seq
  }
}

const GFXglyph* get_glyph(const GFXfont* f, const uint8_t code)
{
  const uint8_t* q = f->glyphs;
  int16_t i = f->last + 1 - f->first;
  while (--i >= 0) {
    if (((GFXglyph *)q)->code == code) {
      return (const GFXglyph *)q;
    }
    q += sizeof(GFXglyph) + (((GFXglyph *)q)->len0 | (((GFXglyph *)q)->len1 << 8));
  }
  return 0;
}

uchar* font_t::bin_search_glyph(GFXfont** g, uchar *p)
{
  int16_t btm = v_n0;
  int16_t top = v_n - 1;
  while (btm <= top) {
    int16_t mid = (top + btm) / 2;
    const GFXfont* f = gfx_fonts[mid];
    int8_t i = 0;
    if (f->ch1 > p[0]) {
      top = mid - 1;
      continue;
    }
    if (f->ch1 < p[0]) {
      btm = mid + 1;
      continue;
    }
    i = 1;
    if (f->ch2 > 0) {
      if (f->ch2 > p[1]) {
        top = mid - 1;
        continue;
      }
      if (f->ch2 < p[1]) {
        btm = mid + 1;
        continue;
      }
      i = 2;
      if (f->ch3 > 0) {
        if (f->ch3 > p[2]) {
          top = mid - 1;
          continue;
        }
        if (f->ch3 < p[2]) {
          btm = mid + 1;
          continue;
        }
        i = 3;
      }
    }
    uchar c = p[i];
    if (f->first > c) {
      top = mid - 1;
      continue;
    }
    if (f->last < c) {
      btm = mid + 1;
      continue;
    }
    *g = (GFXfont *)f;
    return p + i;
  }
  btm = 0;
  top = v_n0 - 1;
  while (btm <= top) {
    int16_t mid = (top + btm) / 2;
    const GFXfont* f = gfx_fonts[mid];
    uchar c = *p;
    if (f->first > c) {
      top = mid - 1;
      continue;
    }
    if (f->last < c) {
      btm = mid + 1;
      continue;
    }
    *g = (GFXfont *)f;
    return p;
  }
  return 0;
}

uchar* font_t::seq_scan_glyph(GFXfont** g, uchar* p)
{
  int16_t u = 0;
  int8_t i = 0;
  const GFXfont* f;
  while ((f = gfx_fonts[u++])) {
    i = 0;
    if (f->ch1 > 0) {
      if (p[0] == f->ch1) {
        i = 1;  // utf-8 >= 2 byte
        if (f->ch2 > 0) {
          if (p[1] == f->ch2) {
            i = 2;  // utf-8 >= 3 byte
            if (f->ch3 > 0) {
              if (p[2] == f->ch3) {
                i = 3;  // utf-8 4 byte
              }
              else {
                continue;
              }
            }
          }
          else {
            continue;
          }
        }
      }
      else {
        continue;
      }
    }
    uchar c = p[i];
    if (c >= f->first && c <= f->last) {
      *g = (GFXfont *)f;
      return p + i;
    }
  }
  return 0;
}

const GFXglyph* font_t::getCharGlyph(GFXfont** g, char** c)
{
  if (gfx_fonts) {

    if (!(v_st_1 == 0xfd && v_st_2 == 0xfe) && !(v_st_1 == 0xfe && v_st_2 == 0xfd)) {
      initialize();
    }

    uchar* p;
    GFXfont* f;
    if (v_st_1 == 0xfd && v_st_2 == 0xfe) {
      p = bin_search_glyph(&f, (uchar *)*c);
    }
    else {
      p = seq_scan_glyph(&f, (uchar *)*c);
    }
    if (p) {
      const GFXglyph* q = get_glyph(f, *p++);
      if (q) {
        if (c) {
          *c = (char *)p;
        }
        if (g) {
          *g = (GFXfont *)f;
        }
        return q;
      }
    }
  }
  if (g) {
    *g = 0;
  }
  return 0;
} 

const int16_t font_t::defaultCharSize(int16_t* letterHeight)
{
  int16_t w, h;
  if (gfx_fonts) {
    int16_t wx = 0;
    int16_t m = 0;
#define MAX 10
    const GFXfont* f;
    int16_t u = 0;
    int16_t yAdvance = 0;
    while ((f = gfx_fonts[u++])) {
      yAdvance = f->yAdvance;
      const uint8_t* q = f->glyphs;
      q += sizeof(GFXglyph) + (((GFXglyph *)q)->len0 | (((GFXglyph *)q)->len1 << 8));
      wx += ((GFXglyph *)q)->xAdvance;
      if (++m > MAX) {
        break;
      }
    }
    if (m > 0) {
      w = wx / m;
      h = yAdvHeight(yAdvance);
      *letterHeight = h;
      return w * fontSizeX;
    }
  }
  w = 6;
  h = 8 * fontSizeY;
  *letterHeight = h;
  return w * fontSizeX;
}

void font_t::cursorAdjust(GFXfont* gfxFont, int16_t* x, int16_t* y)
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
  
const int16_t font_t::charSize(char** c, int16_t* letterHeight)  // utf-8
{
  int16_t w, h;
  GFXfont* f;
  const GFXglyph* glyph = getCharGlyph(&f, c); 
  if (!glyph) { // Default font
    w = 6;
    h = 8 * fontSizeY;
    *c += 1;
  }
  else {
    w = glyph->xAdvance;
    h = yAdvHeight(f->yAdvance);
  }
  *letterHeight = h;
  return w * fontSizeX;
}

const int16_t font_t::textLineSize(const char* text, int16_t* textHeight)
{
  int16_t w = 0;
  int16_t h = 0;
  int16_t lh;

  char* p = (char *)text;
  char c;
  while ((c = *p) && c != '\r' && c != '\n') {
    w += charSize(&p, &lh);
    if (lh > h) {
      h = lh;
    }
  }
  if (h == 0) {
    defaultCharSize(&h);
  }
  *textHeight = h;
  return w;
}

const int16_t font_t::textSize(const char* text, int16_t* textHeight)
{
  char* p = (char *)text;
  int16_t w = 0;
  int16_t h = 0;
  int16_t lw = 0;
  int16_t lh = 0;
  char c = 1;
  while (c) {
    c = *p;
    if (c != '\r') {
      if (c == 0 || c == '\n') {
        if (lw > w) {
          w = lw;
        }
        lw = 0;
        if (lh == 0) {
          defaultCharSize(&lh);
        }
        h += lh;
        lh = 0;
        ++p;
      }
      else {
        int16_t lh1;
        lw += charSize(&p, &lh1);
        if (lh1 > lh) {
          lh = lh1;
        }
      }
    }
    else {
      ++p;
    }
  }
  if (h == 0) {
    defaultCharSize(&lh);
    h = lh;
  }
  *textHeight = h;
  return w;
}

const int16_t font_t::textLineWidth(int16_t len)
{
  int16_t h;
  return defaultCharSize(&h) * len;
}

const int16_t font_t::textLineWidth(const char* text)
{
  int16_t h;
  return textLineSize(text, &h);
}

const int16_t font_t::textLineHeight()
{
  int16_t h;
  defaultCharSize(&h);
  return h;
}
