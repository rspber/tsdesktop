/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

/*
----------------------------------------------------------------
                        CheckBox Class
----------------------------------------------------------------
*/

#include "TSDesktop.h"

void CheckBox::drawBox(const rgb_t aBoxColor)
{
  clip_t clip;
  display.drawRect(getClip(clip), getAbsInnerLeft(getDecorPosLeft()), getAbsInnerTop(getDecorPosTop()), getDecorWidth(), getDecorHeight(), aBoxColor);
}

void CheckBox::drawBox()
{
  drawBox(checked ? activeColor : inActiveColor);
}

void CheckBox::drawCheck(const rgb_t aCheckColor)
{
  int16_t x = getAbsInnerLeft(getDecorPosLeft());
  int16_t y = getAbsInnerTop(getDecorPosTop());
  clip_t clip;
  display.drawLine(getClip(clip), x + 1, y + 1, x + getDecorWidth() - 2, y + getDecorHeight() - 2, aCheckColor);
  display.drawLine(&clip, x + 1, y + getDecorHeight() - 2, x + getDecorWidth() - 2, y + 1, aCheckColor);
}

void CheckBox::drawCheck()
{
  drawCheck(checked ? activeColor : getBackgroundColor());
}

void CheckBox::drawDecor()
{
  drawBox();
  drawCheck();
}

void CheckBox::hideDecor()
{
  drawCheck(getBackgroundColor());
  drawBox();
}

