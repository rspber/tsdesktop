/*
  TouchScreenDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

/*
----------------------------------------------------------------
            Button Class
----------------------------------------------------------------
*/

#include "TSDesktop.h"
#include <Arduino.h>

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

void Button::setGradient(gradient_t* aGradient)
{
  hide();
  gradient = *aGradient;
  backgroundColor = NO_BACKGROUND_COLOR;
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
  if (aBorderColor != NO_BACKGROUND_COLOR) {
    int16_t bs = borderSize;
    int16_t l = getAbsOuterLeft();
    int16_t t = getAbsOuterTop();
    int16_t w = updWidth;
    int16_t h = updHeight;
    clip_t clip;
    getOuterClip(clip);
    if (l + w > clip.x2) {
      w = clip.x2 - l;
    }
    if (t + h > clip.y2) {
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
      if (clip.x2 < screen()->width()) {
        ++clip.x2;
      }
      if (clip.y2 < screen()->height()) {
        ++ clip.y2;
      }
      if (parent) {
        parent->screen()->drawRoundRect(clip, l, t, w, h, radius, aBorderColor);
      }
    }
  }
}

void Button::drawBorder()
{
  drawBorder(borderColor);
}

const rgb_t Button::getBackgroundColor()
{
  if (gradient.deg) {
    return NO_BACKGROUND_COLOR;
  }
  if (!transparent && backgroundColor != NO_BACKGROUND_COLOR) {
    return backgroundColor;
  }
  return Container::getBackgroundColor();
}


void Button::drawBackground(const rgb_t aBackgroundColor)
{
  if (aBackgroundColor != NO_BACKGROUND_COLOR) {
    int16_t x = getAbsOuterLeft();
    int16_t y = getAbsOuterTop();
    clip_t clip;
    getOuterClip(clip);
    if (radius) {
      screen()->fillRoundRect(clip, x, y, updWidth, updHeight, radius, aBackgroundColor);
    }
    else {
      screen()->fillRect(clip, x, y, updWidth, updHeight, aBackgroundColor);
    }
  }
}

void Button::drawBackground()
{
  if (gradient.deg) {
    int16_t x = getAbsOuterLeft();
    int16_t y = getAbsOuterTop();
    clip_t clip;
    getOuterClip(clip);
    screen()->fillRectGradient(clip, x, y, updWidth, updHeight, gradient);
  }
  else {
    drawBackground(getBackgroundColor());
  }
}

void Button::draw(const bool redraw)
{
  if (screenEnabled) {
    if (getAbsVisible()) {
      updateCoord(redraw);
      if (!wasDrawn || redraw) {
        drawBackground();
        drawBorder();
        innerDraw(true);
      }
      else {
        innerDraw(redraw);
      }
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
  if (screenEnabled) {
    if (getAbsVisible()) {
      if (wasDrawn) {
        const rgb_t hideColor = parent ? parent->getBackgroundColor() : BLACK;
        if (diffColor(getBorderColor(), hideColor)) {
          drawBorder(hideColor);
        }
        if (diffColor(getBackgroundColor(), hideColor)) {
          drawBackground(hideColor);
        }
      }
    }
  }
  Container::hide();
}

void Button::clickEffect(const int16_t posX, const int16_t posY)
{
  if (clickEffect_ == 1) {
    drawBorder(clickHighlightColor);
    delay(clickHighlightDelay);
    drawBorder();
  }
}

