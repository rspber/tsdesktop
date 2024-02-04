/*
  TouchScreenDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

/*
----------------------------------------------------------------
                        CheckBox Class
----------------------------------------------------------------
*/

#include "TSDesktop.h"
#include <Arduino.h>

bool CheckBox::setChecked(const bool aChecked)
{
  checked = aChecked;
  drawDecor();
  delay(250);
  return checked;
}

void CheckBox::drawBox(const rgb_t aBoxColor)
{
  clip_t clip;
  getOuterClip(clip);
  screen()->drawRect(clip, getAbsInnerLeft(getDecorPosLeft()), getAbsInnerTop(getDecorPosTop()), getDecorWidth(), getDecorHeight(), aBoxColor);
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
  getOuterClip(clip);
  screen()->drawLine(clip, x + 1, y + 1, x + getDecorWidth() - 2, y + getDecorHeight() - 2, aCheckColor);
  screen()->drawLine(clip, x + 1, y + getDecorHeight() - 2, x + getDecorWidth() - 2, y + 1, aCheckColor);
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

