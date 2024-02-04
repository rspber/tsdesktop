/*
  TouchScreenDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

/*
----------------------------------------------------------------
                        TextButton Class
----------------------------------------------------------------
*/

#include "TSDesktop.h"
#include <string.h>
#include <stdlib.h>
#include <Arduino.h>

static const uint8_t EMPTY[] {0,0,0,0};

void TextButton::setFont(const GFXfont** aFont)
{
  if (font.getFont() != aFont) {
    hide();
    font.setFont(aFont);
    setChanged();
  }
}

void TextButton::setTextCoord(const int16_t aLeft, const int16_t aTop)
{
  if (textLeft != aLeft || textTop != aTop) {
    hide();
    textLeft = aLeft;
    textTop = aTop;
    setChanged();
  }
}

void TextButton::setFontSize(const int8_t aFontSizeX, const int8_t aFontSizeY)
{
  if (font.fontSizeX != aFontSizeX || font.fontSizeY != aFontSizeY) {
    hide();
    font.fontSizeX = aFontSizeX;
    font.fontSizeY = aFontSizeY;
    setChanged();
  }
}

void TextButton::setFontSize(const int8_t aFontSize)
{
  setFontSize(aFontSize, aFontSize);
}

void TextButton::setTextColor(const rgb_t aTextColorh, const rgb_t aTextColorl)
{
  textColorh = aTextColorh;
  textColorl = aTextColorl;
  if (wasDrawn) {
    drawText();
  }
}

void TextButton::setTextAlign(TEXT_ALIGN aTextAlign)
{
  if (aTextAlign != textAlign) {
    hide();
    textAlign = aTextAlign;
    setChanged();
  }
}

const int16_t TextButton::getTextWidth()
{
  int16_t h;
  return font.textSize(textp, unicode, &h);
}

const int16_t TextButton::getTextHeight()
{
  int16_t h;
  font.textSize(textp, unicode, &h);
  return h;
}

void TextButton::setTempText(const void* aText, const bool aUnicode, const bool temp)
{
  if (unicode != aUnicode || textCmp(textp, aText, unicode) != 0) {
    bool ok = false;
    if (unicode != aUnicode) {
      ok = true;
    }
    else {
      int16_t oldh;
      int16_t oldw = font.textSize(textp, unicode, &oldh);
      int16_t newh;
      int16_t neww = font.textSize(aText, aUnicode, &newh);
      ok = orgWidth >= 0 || (oldw == neww && oldh == newh);
    }
    if (ok) {   // no need to resize button
      if (wasDrawn) {
        hideText();
        innerSetText(aText, aUnicode, temp);
        drawText();
      }
      else {
        innerSetText(aText, aUnicode, temp);
      }
    }
    else {
      hide();
      innerSetText(aText, aUnicode, temp);
      setChanged();
    }
  }
}

const char* TextButton::textToUtf8()
{
  if (unicode) {
    int len = textLength(textp, true);
    if (len) {
      len = (len + 1) * 3;   // to be enough
      allocated = len;
      void* p = malloc(len);
      unicodeToUtf8((char*)p, len, (const uint16_t*)textp);
      free((void*)textp);
      textp = p;
    }
    else {
      textp = &EMPTY;
    }
    unicode = false;
  }
  return (const char*)textp;
}

const uint16_t* TextButton::textToUnicode()
{
  if (!unicode) {
    int len = utf8CharLen((const char*)textp);
    if (len) {
      len += 2;
      allocated = len;
      void* p = malloc(len<<1);
      utf8ToUnicode((uint16_t*)p, len, (const char*)textp);
      free((void*)textp);
      textp = p;
    }
    else {
      textp = &EMPTY;
    }
    unicode = true;
  }
  return (const uint16_t*)textp;
}

void TextButton::innerSetText(const void* aText, const bool aUnicode, const bool temp)
{
  bool empty = !(aText && aUnicode ? *(uint16_t*)aText : *(char*)aText);
  if (unicode != aUnicode || empty || !temp ) {
    if (allocated) {
      free((void*)textp);
      allocated = 0;
    }
    unicode = aUnicode;
  }
  if (empty) {
    textp = &EMPTY;
  }
  else {
    if (temp) {
      int len = textLength(aText, aUnicode) + 1;
      if (allocated == 0) {
        textp = malloc(len<<unicode);
        allocated = len;
      }
      else {
        if (len > allocated) {
          textp = realloc((void*)textp, len<<unicode);
          allocated = len;
        }
      }
      memcpy((void*)textp, aText, len<<unicode);
    }
    else {
      textp = aText;
    }
  }
}

const void* TextButton::reallocTextTo(const int size)
{
  if (allocated < size) {
    if (allocated > 0) {
      allocated = size + 20;
      textp = realloc((void*)textp, allocated<<unicode);
    }
    else {
      int len = textLength(textp, unicode) + 1;
      allocated = size + 20;
      if (allocated < len) {
        allocated = len;
      }
      void* p = malloc(allocated<<unicode);
      memcpy((void*)p, textp, len<<unicode);
      textp = p;
    }
  }
  return textp;
}

void TextButton::updCompactWidth(const bool recalc)
{
  setUpdWidth(marginLeft + getTextMarginLeft() + textLeft + getTextWidth() + getTextMarginRight() + marginRight);
}

void TextButton::updCompactHeight(const bool recalc)
{
  setUpdHeight(marginTop + getTextMarginTop() + textTop + getTextHeight() + getTextMarginBottom() + marginBottom);
}

void TextButton::textUpdateCoord()
{
  textWidth = font.textSize(textp, unicode, &textHeight);
  int16_t w = updWidth - marginLeft - marginRight - getTextMarginLeft() - getTextMarginRight();
  if (alignCenterHoriz) {
    textLeft = w > textWidth ? (w - textWidth) / 2 : 0;
  }
  else {
    if (textLeft + textWidth > w) {
      int16_t i = w - textWidth;
      if (i < 0) {
        i = 0;
      }
      textLeft = i;
    }
    if (alignLeft) {
      textLeft = 0;
    }
    else {
      if (alignRight) {
        if (textWidth < w) {
          textLeft = w - textWidth;
        }
      }
    }
  }

  int16_t h = updHeight - marginTop - marginBottom - getTextMarginTop() - getTextMarginBottom();
  if (alignCenterVert) {
    textTop = h > textHeight ? (h - textHeight) / 2 : 0;
  }
  else {
    if (textTop + textHeight > h) {
      int16_t i = h - textHeight;
      if (i < 0) {
        i = 0;
      }
      textTop = i;
    }
    if (alignTop) {
      textTop = 0;
    }
    else {
      if (alignBottom) {
        if (textHeight < h) {
          textTop = h - textHeight;
        }
      }
    }
  }
}

void TextButton::updateCoord(const bool recalc)
{
  if (!updated || recalc) {
    updated = true;
    textUpdateCoord();
  }
}

void TextButton::drawBackground()
{
  rgb_t bg = getBackgroundColor();
  if (bg != NO_BACKGROUND_COLOR) {
    int16_t x = getAbsOuterLeft();
    int16_t y = getAbsOuterTop();
    clip_t clip;
    getOuterClip(clip);
    bool lines = (textChr(textp, unicode, '\n'));
    if (radius || lines) {
      screen()->fillRoundRect(clip, x, y, updWidth, updHeight, radius, bg);
    }
    else {
      screen()->fillRect(clip, x, y, updWidth, marginTop + textTop, bg);
      int16_t toph = marginTop + textTop + textHeight;
      screen()->fillRect(clip, x, y + toph, updWidth, updHeight - toph, bg);
      screen()->fillRect(clip, x, y + marginTop + textTop, marginLeft + textLeft, textHeight, bg);
      int16_t lftw = marginLeft + textLeft + textWidth;
      screen()->fillRect(clip, x + lftw, y + marginTop + textTop, updWidth - lftw, textHeight, bg);
    }
  }
  else {
    Button::drawBackground();
  }
}

void TextButton::drawText(const rgb_t aTextColorh, const rgb_t aTextColorl)
{
  rgb_t bg = getBackgroundColor();
  int16_t x = getAbsInnerLeft(getTextMarginLeft() + textLeft);
  int16_t y = getAbsInnerTop(getTextMarginTop() + textTop);
  cursor_t cursor{ x, y };
  clip_t clip;
  getOuterClip(clip);
  const void* p = textp;
  int16_t h;
  int16_t w = font.textSize(p, unicode, &h);
  const void* p0 = p;
  uint16_t uc = 1;
  while (uc) {
    uc = unicode ? *(uint16_t*)p : *(char*)p;
    p = (void*)((uint8_t*)p + (1<<unicode));
    if (uc == 0 || uc == '\n') {
      int16_t nofchars;
      int16_t h0;
      int16_t w0;
      switch (textAlign) {
      case TEXT_ALIGN_RIGHT:
        w0 = font.textLineSize(p0, unicode, &h0, &nofchars);
        cursor.x += w - w0;
        break;
      case TEXT_ALIGN_CENTER:
        w0 = font.textLineSize(p0, unicode, &h0, &nofchars);
        cursor.x += (w - w0) / 2;
        break;
      default:;
      }
      if (unicode) {
        screen()->drawTextLine(clip, cursor, font, (const uint16_t*)p0, aTextColorh, bg == NO_BACKGROUND_COLOR ? aTextColorh : bg, aTextColorl, 0);
      }
      else {
        screen()->drawTextLine(clip, cursor, font, (const char*)p0, aTextColorh, bg == NO_BACKGROUND_COLOR ? aTextColorh : bg, aTextColorl, 0);
      }
      p0 = p;
      cursor.x = x;
      cursor.y += font.textLineHeight();
    }
  }
}

void TextButton::drawText()
{
  drawText(textColorh, textColorl);
}

void TextButton::hideText()
{
  drawText(getBackgroundColor(), BLACK);
}

void TextButton::innerDraw(const bool redraw)
{
  drawText();
}

void TextButton::clickEffect(const int16_t posX, const int16_t posY)
{
  if (clickEffect_ == 1) {
    drawBorder(clickHighlightColor);
    drawText(clickHighlightColor, clickHighlightColor);
    delay(clickHighlightDelay);
    drawBorder();
    drawText();
  }
}

