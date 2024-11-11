
#include "TSD_CHAR.h"
#include "glcdfont.c.h"

namespace {

  bool ifdrawbgif(rgb_t color, rgb_t bg)
  {
  #ifdef OVERLAID
    // see TSD_ILI9341.cpp for explanation
    if ((color & 0xFF000000) != 0xFF000000) {
      over_t* t = (over_t*)color;
      return t->color != bg;
    }
    else
  #endif
    {
      return color != bg;
    }
  }

  rgb_t alphaBlend(uint8_t alpha, rgb_t ch, rgb_t cl)
  {
    // For speed use fixed point maths and rounding to permit a power of 2 division
    uint16_t rh = ((ch >> 15) & 0x1FE) + 1;
    uint16_t gh = ((ch >>  7) & 0x1FE) + 1;
    uint16_t bh = ((ch <<  1) & 0x1FE) + 1;

    uint16_t rl = ((cl >> 15) & 0x1FE) + 1;
    uint16_t gl = ((cl >>  7) & 0x1FE) + 1;
    uint16_t bl = ((cl <<  1) & 0x1FE) + 1;

    // Shift right 1 to drop rounding bit and shift right 8 to divide by 256
    uint8_t r = (rh * alpha + rl * (255 - alpha)) >> 9;
    uint8_t g = (gh * alpha + gl * (255 - alpha)) >> 9;
    uint8_t b = (bh * alpha + bl * (255 - alpha)) >> 9;

    // Combine RGB colours into 24 bits
    return RGB(r,g,b);
  }

  rgb_t alphaBlend(uint8_t bpp, uint8_t bits, uint8_t b80, rgb_t colorh, rgb_t colorl)
  {
    uint8_t alpha = (bits & b80) + (0x100 - b80 - 1);
    if (bpp == 0 || colorh == colorl || alpha == 255) {
      return colorh;
    }
    return alphaBlend(alpha, colorh, colorl);
  }
};

// utf-8

// Draw a character
/**************************************************************************/
/*!
   @brief   Draw a single character
    @param    cursor top left corner x, y coordinate
    @param    font  gfxFont with fontSizeX and fontSizeY
    @param    c
    @param    color
*/
/**************************************************************************/
void TSD_CHAR::drawChar(clip_t& clip, cursor_t& cursor, tsd_font_t& font, char** c, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  startWrite();

  uint8_t size_x = font.fontSizeX;
  uint8_t size_y = font.fontSizeY;

  int16_t x = cursor.x;
  int16_t y = cursor.y;

  int32_t lw;
  int32_t lhz;
  TSD_GFXfont* gfxFont;
  const TSD_GFXglyph* glyph = font.getCharGlyph(&gfxFont, c);
  font.cursorAdjust(gfxFont, &x, &y);

  bool drawbg = ifdrawbgif(colorh, bg);

  if (!glyph) { // 'Classic' built-in font
    lw = 6;
    lhz = 8 * size_y;
    if (drawbg && x - cursor.x > 0) {
      fillRectHelper(clip, cursor.x, cursor.y, x - cursor.x, lhz, bg);
    }
    if (drawbg && y - cursor.y > 0) {
      fillRectHelper(clip, cursor.x, cursor.y, lw * size_x, y - cursor.y, bg);
    }
    for (int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
      uint8_t line = default_font[**c * 5 + i];
      for (int8_t j = 0; j < 8; j++, line >>= 1) {
        if (line & 1) {
          if (size_x == 1 && size_y == 1)
            drawPixel(clip, x + i, y + j, colorh);
          else
            fillRectHelper(clip, x + i * size_x, y + j * size_y, size_x, size_y, colorh);
        } else if (drawbg) {
          if (size_x == 1 && size_y == 1)
            drawPixel(clip, x + i, y + j, bg);
          else
            fillRectHelper(clip, x + i * size_x, y + j * size_y, size_x, size_y, bg);
        }
      }
    }
    if (drawbg) { // If opaque, draw vertical line for last column
      if (size_x == 1 && size_y == 1)
        drawFastVLine(clip, x + 5, y, 8, bg);
      else
        fillRectHelper(clip, x + 5 * size_x, y, size_x, 8 * size_y, bg);
    }
    *c += 1;
  }
  else { // Custom font
    lw = glyph->xAdvance;
    lhz = font.yAdvHeight(gfxFont->yAdvance);

    const uint8_t* bp = (const uint8_t*)(glyph + 1);
    uint16_t w = glyph->width, h = glyph->height;
    int16_t xo = glyph->xOffset,
       yo = 1 - glyph->yOffset; // yOffset is positive

    if (drawbg && x - cursor.x + xo * size_x > 0) {
      fillRectHelper(clip, cursor.x, cursor.y, x - cursor.x + xo * size_x, lhz, bg);
    }
    if (drawbg && y - cursor.y + yo * size_y > 0) {
      fillRectHelper(clip, cursor.x, cursor.y, lw * size_x, y - cursor.y + yo * size_y, bg);
    }

    uint8_t xx, yy, bits = 0, bit = 0;

    if (size_x > 1 || size_y > 1) {
      xo = (xo + 1) >> 1;
    }

    uint8_t bpp = glyph->bpp;
    uint8_t shu, b80;
    switch (bpp) {
      case 0: b80 = 0x80; shu = 1; break;
      case 1: b80 = 0xc0; shu = 2; break;
      case 2: b80 = 0xf0; shu = 4; break;
      case 3:
      default: b80 = 0xff; shu = 8;
    }
    for (yy = 0; yy < h; yy++) {
      for (xx = 0; xx < w; xx++) {
        if (!(bit & 7)) {
          bits = *bp++;
          bit = 0;
        }
        bit += shu;
        uint8_t b = bits & b80;
        rgb_t color = b ? alphaBlend(bpp, bits, b80, colorh, colorl) : bg;
        if (b || drawbg) {
          if (size_x == 1 && size_y == 1) {
            drawPixel(clip, x + xo + xx, y + yo + yy, color);
          }
          else {
            fillRectHelper(clip, x + (xo + xx) * size_x, y + (yo + yy) * size_y, size_x, size_y, color);
          }
        }
        bits <<= shu;
      }
      if (glyph->ver > 0) {  // not compressed
        bit = 0;  // go to next bitmap byte
      }
    }
    int32_t y2 = y + (yo + h) * size_y;
    int32_t h2 = cursor.y + lhz - y2;
    if (drawbg && h2 > 0) {
      fillRectHelper(clip, cursor.x, y2, lw * size_x, h2, bg);
    }
    int32_t w2 = lw - (xo + w);
    if (drawbg && w2 > 0) {
      fillRectHelper(clip, cursor.x + (xo + w) * size_x, cursor.y, w2 * size_x, lhz, bg);
    }
  } // End classic vs custom font
  if (drawbg && spacing > 0) {
    fillRectHelper(clip, cursor.x + lw * size_x, cursor.y, spacing * size_x, lhz, bg);
  }
  cursor.x += (lw + spacing) * size_x;

  endWrite();
}


// Draw text line
/**************************************************************************/
/*!
   @brief   Draw text line to the \r or \n character or to the end of the text
    @param    cursor top left corner x, y coordinate
    @param    font  gfxFont with fontSizeX and fontSizeY
    @param    text   text line to be drawn
    @param    color
  @param    spacing extra horizontal spacing for letters
*/
/**************************************************************************/
const char* TSD_CHAR::drawTextLine(clip_t& clip, cursor_t& cursor, tsd_font_t& font, const char* text, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  if (text) {
    char* p = (char *)text;
    char c;
    while ((c = *p) && c != '\r' && c != '\n') {
      drawChar(clip, cursor, font, &p, colorh, bg, colorl, spacing);
    }
    return (const char*) p;
  }
  return NULL;
}

// unicode

void TSD_CHAR::drawChar(clip_t& clip, cursor_t& cursor, tsd_font_t& font, const uint16_t uchar, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  char buf[8];
  toUtf8(buf, uchar);
  char* bp = buf;
  drawChar(clip, cursor, font, &bp, colorh, bg, colorl, spacing);
}

const uint16_t* TSD_CHAR::drawTextLine(clip_t& clip, cursor_t& cursor, tsd_font_t& font, const uint16_t* utext, rgb_t colorh, rgb_t bg, rgb_t colorl, const int8_t spacing)
{
  if (utext) {
    const uint16_t* p = utext;
    uint16_t u;
    while ((u = *p) && u != '\r' && u != '\n') {
      char buf[8];
      toUtf8(buf, u);
      char* bp = buf;
      drawChar(clip, cursor, font, &bp, colorh, bg, colorl, spacing);
      ++p;
    }
    return p;
  }
  return NULL;
}
