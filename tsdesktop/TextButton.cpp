/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

/*
----------------------------------------------------------------
                        TextButton Class
----------------------------------------------------------------
*/

#include "TSDesktop.h"

void TextButton::setFont(const GFXfont* aFont)
{
  if (font.gfxFont != aFont) {
    hide();
    font.gfxFont = (GFXfont*)aFont;
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

void TextButton::setTextColor(const uint16_t aTextColor)
{
  textColor = aTextColor;
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
  return font.textSize(getText(), &h);
}

const int16_t TextButton::getTextHeight()
{
  int16_t h;
  font.textSize(getText(), &h);
  return h;
}

void TextButton::setText(const char* aText)
{
  const char* t = getText();
  if (strcmp(t, aText) != 0) {
    int16_t oldh;
    int16_t oldw = font.textSize(t, &oldh);
    int16_t newh;
    int16_t neww = font.textSize(aText, &newh);
    if (orgWidth >= 0 || (oldw == neww && oldh == newh)) {   // no need to resize button
      if (wasDrawn) {
        hideText();
        innerSetText(aText);
        drawText();
      }
      else {
        innerSetText(aText);
      }
    }
    else {
      hide();
      innerSetText(aText);
      setChanged();
    }
  }
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
  int16_t textWidth = getTextWidth();
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

  int16_t textHeight = getTextHeight();
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

void TextButton::drawText(const uint16_t aTextColor)
{
  if (getAbsVisible()) {
    if (screenEnabled) {
      int16_t absLeft = getAbsLeft(getTextMarginLeft() + textLeft);
      int16_t absTop = getAbsTop(getTextMarginTop() + textTop);
      cursor_t cursor{ absLeft, absTop };
      const char* p = getText();
      int16_t h;
      int16_t w = font.textSize(p, &h);
      const char* p0 = p;
      char c = 1;
      while (c) {
        c = *p++;
        if (c == 0 || c == '\n') {
          int8_t h0;
          int16_t w0;
          switch (textAlign) {
          case TEXT_ALIGN_RIGHT:
            w0 = font.textLineSize(p0, &h0);
            cursor.x += w - w0;
            break;
          case TEXT_ALIGN_CENTER:
            w0 = font.textLineSize(p0, &h0);
            cursor.x += (w - w0) / 2;
            break;
          default:;
          }
          display.drawTextLine(&cursor, &font, p0, aTextColor);
          p0 = p;
          cursor.x = absLeft;
          cursor.y += font.textLineHeight();
        }
      }
    }
  }
}

void TextButton::drawText()
{
  drawText(textColor);
}

void TextButton::hideText()
{
  drawText(getBackgroundColor());
}

void TextButton::draw(const bool redraw)
{
  Button::draw(redraw);
  drawText();
}

void TextButton::clickEffect(const int16_t posX, const int16_t posY)
{
  if (clickEffect_ == 1) {
    drawBorder(clickHighlightColor);
    drawText(clickHighlightColor);
    sleep_ms(clickHighlightDelay);
    drawBorder();
    drawText();
  }
}

