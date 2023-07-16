/*
  TouchScreenDesktop

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#include "Editable.h"

void Editable::innerSetText(const void* aText, const bool aUnicode, const bool temp)
{
  int oldLen = textLength(textp, unicode);
  int newLen = textLength(aText, aUnicode);
  TextButton::innerSetText(aText, aUnicode, temp);
  if (textCursor == oldLen || textCursor > newLen) {
    textCursor = newLen;
  }
}

void Editable::setCursorAllow(const bool aCursorAllow)
{
  if (cursorAllow != aCursorAllow) {
    hideCursor();
    cursorAllow = aCursorAllow;
    drawCursor();
  }
}

void Editable::setCursorInsMode(const bool aCursorInsMode)
{
  if (cursorInsMode != aCursorInsMode) {
    hideCursor();
    cursorInsMode = aCursorInsMode;
    drawCursor();
  }
}

void Editable::setCursor(const int aCursor)
{
  int len = textLength(textp, unicode);
  int nc = aCursor < 0 ? 0 : aCursor < len ? aCursor : len;
  if (textCursor != nc) {
    hideCursor();
    textCursor = nc;
    drawCursor();
  }
}

void Editable::setCursorToEnd()
{
  setCursor(0x7fffffff);
}

void Editable::moveCursor(const int8_t aDirection)
{
  setCursor(textCursor + aDirection);
}

void Editable::setCursorColor(const rgb_t aCursorColor)
{
  if (cursorColor != aCursorColor) {
    hideCursor();
    cursorColor = aCursorColor;
    drawCursor();
  }
}

void Editable::drawCursor(const rgb_t aCursorColor)
{
  if (cursorAllow) {
    int16_t absLeft = getAbsInnerLeft(getTextMarginLeft() + getTextLeft());
    int16_t absTop = getAbsInnerTop(getTextMarginTop() + getTextTop());
    int16_t cpos = font.textApproxLineWidth(textp, unicode, textCursor);
    int16_t cursor_width = (cursorInsMode || textCursor >= (int)textLength(textp, unicode)) ? 1 : font.textApproxLineWidth(textp, unicode, 1) + 2;
    int16_t cursor_height = getTextHeight() + 2;
    clip_t clip;
    getOuterClip(clip);
    screen()->drawRect(clip, absLeft + (cpos ? cpos + 1 : -1), absTop - 1, cursor_width, cursor_height, aCursorColor);
  }
}

void Editable::drawCursor()
{
  drawCursor(cursorColor);
}

void Editable::hideCursor()
{
  drawCursor(getBackgroundColor());
}

void Editable::hideText()
{
  TextButton::hideText();
  hideCursor();
}

void Editable::drawText()
{
  TextButton::drawText();
  drawCursor();
}

void Editable::clickEffect(const int16_t posX, const int16_t posY)
{
  if (cursorAllow) {
    setCursor((posX - getLeftMargin() - getTextLeft() - getTextMarginLeft()) / font.textApproxLineWidth(textp, unicode, 1));
  }
  else {
    TextButton::clickEffect(posX, posY);
  }
}

void Editable::cmdInsChar_(const uint16_t aChar)
{
  int len = textLength(textp, unicode);
  const void* p = reallocTextTo(len + 1);
  int i = textCursor;
  if (i < 0) {
    i = 0;
  }
  if (cursorInsMode) {
    //      hideText();
    hide();
    if (unicode) {
      for (int j = len; j >= i; --j) {
        ((uint16_t*)p)[j + 1] = ((uint16_t*)p)[j];
      }
    }
    else {
      for (int j = len; j >= i; --j) {
        ((char*)p)[j + 1] = ((char*)p)[j];
      }
    }
    ++len;
  }
  else {
    if (i >= len) {
      return;
    }
    //      hideText();
    hide();
  }
  if (unicode) {
    ((uint16_t*)p)[i] = aChar;
  }
  else {
    ((char*)p)[i] = aChar;
  }
  textCursor = textCursor < len ? textCursor + 1 : len;
  setChanged();
}

void Editable::cmdDelBS(const bool BS)
{
  int len = textLength(textp, unicode);
  if (len > 0) {
    const void* p = reallocTextTo(len); // if textp is static convert it to allocated
    int i = textCursor;
    if (BS) {
      --i;
    }
    if (i >= 0 && i < len) {
      //      hideText();
      hide();
      if (unicode) {
        for (; i < len; ++i) {
          ((uint16_t*)p)[i] = ((uint16_t*)p)[i + 1];
        }
      }
      else {
        for (; i < len; ++i) {
          ((char*)p)[i] = ((char*)p)[i + 1];
        }
      }
      --len;
      if (BS) {
        textCursor = textCursor > 0 ? textCursor - 1 : 0;
      }
      setChanged();
    }
  }
}
