/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "Editable.h"

void Editable::innerSetText(const char* aText)
{
  int oldLen = strlen(bufp);
  int newLen = strlen(aText);
  strncpy(bufp, aText, bufSize);
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
  int len = strlen(getText());
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
    int16_t cpos = font.textLineWidth(textCursor);
    int16_t cursor_width = (cursorInsMode || textCursor >= (int)strlen(getText())) ? 1 : font.textLineWidth(1) + 2;
    int16_t cursor_height = getTextHeight() + 2;
    clip_t clip;
    display.drawRect(getClip(clip), absLeft + (cpos ? cpos + 1 : -1), absTop - 1, cursor_width, cursor_height, aCursorColor);
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
  if (getAbsVisible()) {
    hideCursor();
  }
}

void Editable::drawText()
{
  TextButton::drawText();
  if (getAbsVisible()) {
    drawCursor();
  }
}

void Editable::clickEffect(const int16_t posX, const int16_t posY)
{
  if (cursorAllow) {
    setCursor((posX - getLeftMargin() - getTextLeft() - getTextMarginLeft()) / font.textLineWidth(1));
  }
  else {
    TextButton::clickEffect(posX, posY);
  }
}

void Editable::cmdInsChar(const char aChar)
{
  int len = strlen(bufp);
  if (len + 1 < bufSize) {
    int i = textCursor;
    if (i < 0) {
      i = 0;
    }
    if (cursorInsMode) {
      //      hideText();
      hide();
      for (int j = len; j >= i; --j) {
        bufp[j + 1] = bufp[j];
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
    bufp[i] = aChar;
    textCursor = textCursor < len ? textCursor + 1 : len;
    setChanged();
  }
}

void Editable::cmdDelBS(const bool BS)
{
  int len = strlen(bufp);
  if (len > 0) {
    int i = textCursor;
    if (BS) {
      --i;
    }
    if (i >= 0 && i < len) {
      //      hideText();
      hide();
      for (; i < len; ++i) {
        bufp[i] = bufp[i + 1];
      }
      --len;
      if (BS) {
        textCursor = textCursor > 0 ? textCursor - 1 : 0;
      }
      setChanged();
    }
  }
}
