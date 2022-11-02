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

void Button::setBackgroundColor(const rgb_t aBackgroundColor)
{
  hide();
  backgroundColor = aBackgroundColor;
  setChanged();
}

void Button::setBackground(const rgb_t aBackgroundColor)
{
  hide();
  backgroundColor = aBackgroundColor;
  setChanged();
}

void Button::setBorderColor(const rgb_t aBorderColor)
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

void Button::drawBorder(const rgb_t aBorderColor)
{
  if (screenEnabled && aBorderColor != NO_BACKGROUND_COLOR) {
    int16_t bs = borderSize;
    int16_t l = getAbsLeft();
    int16_t t = getAbsTop();
    int16_t w = updWidth;
    int16_t h = updHeight;
    clip_t clip;
    getClip(clip);
    if( l + w > clip.x2 ) {
      w = clip.x2 - l;
    }
    if( t + h > clip.y2 ) {
      h = clip.y2 - t;
    }
    while (--bs >= 0) {
      --l;
      --t;
      w += 2;
      h += 2;
      if (clip.x1 > 0) {
        --clip.x1;
      }
      if (clip.y1 > 0) {
        --clip.y1;
      }
      if( clip.x2 < display.width() ) {
        ++clip.x2;
      }
      if( clip.y2 < display.height() ) {
        ++ clip.y2;
      }
      display.drawRoundRect(&clip, l, t, w, h, radius, aBorderColor);
    }
  }
}

void Button::drawBorder()
{
  drawBorder(borderColor);
}

void Button::drawBackground(const rgb_t aBackgroundColor)
{
  if (screenEnabled && aBackgroundColor != NO_BACKGROUND_COLOR) {
    clip_t clip;
    display.fillRoundRect(getClip(clip), getAbsLeft(), getAbsTop(), updWidth, updHeight, radius, aBackgroundColor);
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

bool diffColor(const rgb_t oldColor, const rgb_t newColor)
{
  return oldColor != newColor && newColor != NO_BACKGROUND_COLOR;
}

void Button::hide()
{
  if (wasDrawn) {
    const rgb_t hideColor = parent ? parent->getBackgroundColor() : BLACK;
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

