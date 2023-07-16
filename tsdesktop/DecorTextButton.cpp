/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

/*
----------------------------------------------------------------
                    DecoratedTextButton Class
----------------------------------------------------------------
*/

#include "TSDesktop.h"

void DecorTextButton::setDecorPos(const DECOR_POS aDecorPosMode)
{
  switch (aDecorPosMode) {
  case DECOR_POS_LEFT:
    decorMarginTop = 0;
    decorMarginBottom = 0;
    decorMarginLeft = 2;
    decorMarginRight = 10;
    break;
  case DECOR_POS_RIGHT:
    decorMarginTop = 0;
    decorMarginBottom = 0;
    decorMarginLeft = 10;
    decorMarginRight = 2;
    break;
  default:
    decorMarginLeft = 2;
    decorMarginRight = 2;
    break;
  }
  hide();
  decorPosMode = aDecorPosMode;
  setChanged();
}

void DecorTextButton::setDecorAlign(const DECOR_ALIGN aDecorAlign)
{
  switch (aDecorAlign) {
  case DECOR_ALIGN_TOP:
    decorMarginLeft = 0;
    decorMarginRight = 0;
    decorMarginTop = 1;
    decorMarginBottom = 4;
    break;
  case DECOR_ALIGN_BOTTOM:
    decorMarginLeft = 0;
    decorMarginRight = 0;
    decorMarginTop = 4;
    decorMarginBottom = 1;
    break;
  case DECOR_ALIGN_NONE:
    decorAlign = aDecorAlign;
    setDecorPos(decorPosMode);
    return;
  }
  hide();
  decorAlign = aDecorAlign;
  setChanged();
}

void DecorTextButton::setDecorMarginLeft(const uint8_t aDecorMarginLeft)
{
  if (aDecorMarginLeft != decorMarginLeft) {
    hide();
    decorMarginLeft = aDecorMarginLeft;
    setChanged();
  }
}

void DecorTextButton::setDecorMarginRight(const uint8_t aDecorMarginRight)
{
  if (aDecorMarginRight != decorMarginRight) {
    hide();
    decorMarginRight = aDecorMarginRight;
    setChanged();
  }
}

void DecorTextButton::setDecorMarginTop(const uint8_t aDecorMarginTop)
{
  if (aDecorMarginTop != decorMarginTop) {
    hide();
    decorMarginTop = aDecorMarginTop;
    setChanged();
  }
}

void DecorTextButton::setDecorMarginBottom(const uint8_t aDecorMarginBottom)
{
  if (aDecorMarginBottom != decorMarginBottom) {
    hide();
    decorMarginBottom = aDecorMarginBottom;
    setChanged();
  }
}

const uint8_t DecorTextButton::getTextMarginLeft() // TODO
{
  switch (decorAlign) {
  case DECOR_ALIGN_NONE:
    switch (decorPosMode) {
    case DECOR_POS_LEFT:
      return decorMarginLeft + getDecorWidth() + decorMarginRight;
    default:
      return 0;
    }
  default:
    return 0;
  }
}

const uint8_t DecorTextButton::getTextMarginRight() // TODO
{
  switch (decorAlign) {
  case DECOR_ALIGN_NONE:
    switch (decorPosMode) {
    case DECOR_POS_RIGHT:
      return decorMarginLeft + getDecorWidth() + decorMarginRight;
    default:
      return 0;
    }
  default:
    return 0;
  }
}

const uint8_t DecorTextButton::getTextMarginTop() // TODO
{
  switch (decorAlign) {
  case DECOR_ALIGN_TOP:
    return decorMarginTop + getTextHeight() + decorMarginBottom;
  default:
    return 0;
  }
}

const uint8_t DecorTextButton::getTextMarginBottom() // TODO
{
  switch (decorAlign) {
  case DECOR_ALIGN_BOTTOM:
    return decorMarginTop + getTextHeight() + decorMarginBottom;
  default:
    return 0;
  }
}

const int16_t DecorTextButton::getDecorPosLeft()
{
  int16_t w = 0;
  switch (decorPosMode) {
  case DECOR_POS_RIGHT:
    w = getTextWidth();
    break;
  case DECOR_POS_CENTER:
    w = (getTextWidth() - decorMarginLeft - decorMarginRight - getDecorWidth()) / 2;
    break;
  default:;
  }
  return decorMarginLeft + w;
}

const int16_t DecorTextButton::getDecorPosTop()
{
  int16_t h = 0;
  switch (decorAlign) {
  case DECOR_ALIGN_NONE:
  case DECOR_ALIGN_TOP:
    h = 0;
    break;
  case DECOR_ALIGN_BOTTOM:
    h = getTextHeight();
    break;
  default:;
  }
  return decorMarginTop + h;
}

void DecorTextButton::setDecorActiveColor(const rgb_t aActiveColor)
{
  activeColor = aActiveColor;
  if (wasDrawn) {
    drawDecor();
  }
}

void DecorTextButton::setDecorInActiveColor(const rgb_t aInActiveColor)
{
  inActiveColor = aInActiveColor;
  if (wasDrawn) {
    drawDecor();
  }
}

void DecorTextButton::innerDraw(const bool redraw)
{
  TextButton::innerDraw(redraw);
  drawDecor();
}

void DecorTextButton::clickEffect(const int16_t posX, const int16_t posY)
{
  toggle();
}
