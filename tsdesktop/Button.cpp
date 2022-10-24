/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

/*
----------------------------------------------------------------
            Button Class
----------------------------------------------------------------
*/

#include "TSDesktop.h"

void Button::setRadius(const int8_t aRadius)
{
  hide();
  radius = aRadius;
}

void Button::setTransparent(const bool aTransparent)
{
  hide();
  transparent = aTransparent;
  setChanged();
}

void Button::setBackgroundColor(const uint16_t aBackgroundColor)
{
  hide();
  backgroundColor = aBackgroundColor;
  setChanged();
}

void Button::setBackground(const uint16_t aBackgroundColor)
{
  hide();
  backgroundColor = aBackgroundColor;
  setChanged();
}

void Button::setBorderColor(const uint16_t aBorderColor)
{
  if (aBorderColor != borderColor) {
    borderColor = aBorderColor;
    if (wasDrawn) {
      if (getAbsVisible()) {
        drawBorder();
      }
    }
  }
}

void Button::drawBorder(const uint16_t aBorderColor)
{
  if (screenEnabled && aBorderColor != NO_BACKGROUND_COLOR) {
    int16_t bs = borderSize;
    int16_t l = getAbsLeft();
    int16_t t = getAbsTop();
    int16_t w = updWidth;
    int16_t h = updHeight;
    while (--bs >= 0) {
      --l;
      --t;
      w += 2;
      h += 2;
      display.drawRoundRect(l, t, w, h, radius, aBorderColor);
    }
  }
}

void Button::drawBorder()
{
  drawBorder(borderColor);
}

void Button::drawBackground(const uint16_t aBackgroundColor)
{
  if (screenEnabled && aBackgroundColor != NO_BACKGROUND_COLOR) {
    display.fillRoundRect(getAbsLeft(), getAbsTop(), updWidth, updHeight, radius, aBackgroundColor);
  }
}

void Button::drawBackground()
{
  drawBackground(getBackgroundColor());
}

void Button::draw(const bool redraw)
{
  updateCoord(redraw);
  if (!wasDrawn || redraw) {
    if (getAbsVisible()) {
      drawBackground();
      drawBorder();
      wasDrawn = true;
    }
  }
}

bool diffColor(const uint16_t oldColor, const uint16_t newColor)
{
  return oldColor != newColor && newColor != NO_BACKGROUND_COLOR;
}

void Button::hide()
{
  if (wasDrawn) {
    const uint16_t hideColor = parent ? parent->getBackgroundColor() : BLACK;
    if (diffColor(getBorderColor(), hideColor)) {
      drawBorder(hideColor);
    }
    if (diffColor(getBackgroundColor(), hideColor)) {
      drawBackground(hideColor);
    }
  }
  Container::hide();
}

void Button::clickEffect(const int16_t posX, const int16_t posY)
{
  if (clickEffect_ == 1) {
    drawBorder(clickHighlightColor);
    sleep_ms(clickHighlightDelay);
    drawBorder();
  }
}

