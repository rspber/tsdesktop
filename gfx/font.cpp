/*
  Font container

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include "font.h"
#include <string.h>

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

const int16_t font_t::charSize(const void** c, const bool unicode, int16_t* letterHeight)
{
  if (unicode) {
    char buf[8];
    toUtf8(buf, **(uint16_t**)c);
    *(uint16_t**)c += 1;
    char* pb = buf;
    return charSize(&pb, letterHeight);
  }
  else {
    return charSize((char**)c, letterHeight);
  }
}

const int16_t font_t::textLineSize(const void* textp, const bool unicode, int16_t* textHeight, int16_t* nOfChars)
{
  int16_t w = 0;
  int16_t h = 0;
  *nOfChars = 0;

  if (textp) {
    const void* p = textp;
    int16_t lh;
    uint16_t uc;
    while ((uc = unicode ? *(uint16_t*)p : *(char *)p) && uc != '\0' && uc != '\r' && uc != '\n') {
      w += charSize(&p, unicode, &lh);
      if (lh > h) {
        h = lh;
      }
      ++*nOfChars;
    }
  }
  if (h == 0) {
    defaultCharSize(&h);
  }
  *textHeight = h;
  return w;
}

const int16_t font_t::textSize(const void* textp, const bool unicode, int16_t* textHeight)
{
  int16_t w = 0;
  int16_t h = 0;
  if (textp) {
    int16_t lw = 0;
    int16_t lh = 0;
    const void* p = textp;
    uint16_t uc = 1;
    while (uc) {
      uc = unicode ? *(uint16_t*)p : *(char*)p;
      if (uc != '\r') {
        if (uc == 0 || uc == '\n') {
          if (lw > w) {
            w = lw;
          }
          lw = 0;
          if (lh == 0) {
            defaultCharSize(&lh);
          }
          h += lh;
          lh = 0;
          p = (void*)((uint8_t*)p + (1<<unicode));
        }
        else {
          int16_t lh1;
          lw += charSize(&p, unicode, &lh1);
          if (lh1 > lh) {
            lh = lh1;
          }
        }
      }
      else {
        p = (void*)((uint8_t*)p + (1<<unicode));
      }
    }
  }
  if (h == 0) {
    defaultCharSize(&h);
  }
  *textHeight = h;
  return w;
}

const int16_t font_t::textApproxLineWidth(const void* textp, const bool unicode, int16_t len)
{
  int16_t h;
  int16_t nofch = 0;
  int16_t w = textLineSize(textp, unicode, &h, &nofch);
  return w * len / (nofch > 0 ? nofch : 1);
}

const int16_t font_t::textLineWidth(const void* textp, const bool unicode)
{
  int16_t h;
  int16_t nofch;
  return textLineSize(textp, unicode, &h, &nofch);
}

const int16_t font_t::textLineHeight()
{
  int16_t h;
  defaultCharSize(&h);
  return h;
}

// bi strings

int textLength(const void* textp, const bool unicode)
{
  if (textp) {
    if (unicode) {
      const uint16_t* p = (const uint16_t*)textp;
      while (*p) {++p;}
      return p - (const uint16_t*)textp;
    }
    else {
      return strlen((char*)textp);
    }
  }
  return 0;
}

const void* textChr(const void* textp, const bool unicode, const char c)
{
  if (textp) {
    if (unicode) {
      const uint16_t* p = (const uint16_t*)textp;
      uint16_t u;
      while ((u = *p)) {
        if (u != c) {
          return p;
        }
        ++p;
      }
      return 0;
    }
    else {
      return (const void*)strchr((const char*)textp, c);
    }
  }
  return 0;
}

int textCmp(const void* t1, const void* t2, const bool unicode)
{
  if (t1 && t2) {
    if (unicode) {
      uint16_t* p1 = (uint16_t*)t1;
      uint16_t* p2 = (uint16_t*)t2;
      while (true) {
        if (*p1) {
          if (*p2) {
            if (*p1 > *p2) {
              return 1;
            }
            if (*p1 < *p2) {
              return -1;
            }
            ++p1, ++p2;
          }
          else {
            return 1;
          }
        }
        else {
          return *p2 ? -1 : 0;
        }
      }
    }
    else {
      return strcmp((const char*)t1, (const char*)t2);
    }
  }
  return -1;
}

// unicode -> utf-8

const int toUtf8(char* buf, const uint16_t ucode)
{
  if (ucode & 0xF800) {
    buf[0] = 0xE0 | ((ucode >> 12) & 0x0F);
    buf[1] = 0x80 | ((ucode >> 6) & 0x3F);
    buf[2] = 0x80 | (ucode & 0x3F);
    buf[3] = 0;
    return 3;
  }
  else
  if (ucode & 0x0700) {
    buf[0] = 0xC0 | ((ucode >> 6) & 0x1F);
    buf[1] = 0x80 | (ucode & 0x3F);
    buf[2] = 0;
    return 2;
  }
  else {
    buf[0] = (ucode & 0xff);
    buf[1] = 0;
    return 1;
  }
}

const int unicodeToUtf8(char* utf8, const int size, const uint16_t* unicode)
{
  if (unicode) {
    const uint16_t* w = unicode;
    char* p = utf8;
    while (*w && p - utf8 < size - 4) {
      p += toUtf8(p, *w++);
    }
    *p = 0;
    return p - utf8;
  }
  *utf8 = 0;
  return 0;
}

const uint16_t toUnicode(char** c)
{
  uchar c1 = **c;
  if (c1 >= 0xE0) {
    uchar* p = (uchar*)*c;
    uint16_t w = *p++ & 0x0F;
    if (*p >= 0x80 && *p <= 0xBF) {
      w = (w << 6) | (*p++ & 0x3F);
      if (*p >= 0x80 && *p <= 0xBF) {
        w = (w << 6) | (*p++ & 0x3F);
        *c = (char*)p;
        return w;
      }
    }
  }
  else {
    if (c1 >= 0xC0) {
      uchar* p = (uchar*)*c;
      uint16_t w = *p++ & 0x1F;
      if (*p >= 0x80 && *p <= 0xBF) {
        w = (w << 6) | (*p++ & 0x3F);
        *c = (char*)p;
        return w;
      }
    }
  }
  *c += 1;
  return c1;
}

const int utf8CharLen(const char* utf8)
{
  if (utf8) {
    char* p = (char*)utf8;
    int n = 0;
    while (*p) {
      toUnicode(&p);
      ++n;
    }
    return n;
  }
  return 0;
}

const int utf8ToUnicode(uint16_t* unicode, const int size, const char* utf8)
{
  if (utf8) {
    char* p = (char*)utf8;
    int j = 0;
    uint16_t* w = unicode;
    while (*p && j < size - 1) {
      *w++ = toUnicode(&p);
      ++j;
    }
    *w = 0;
    return j;
  }
  *unicode = 0;
  return 0;
}
