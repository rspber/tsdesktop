/*
  TouchScreenDesktop

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

/*
----------------------------------------------------------------
                        RadioButton Class
----------------------------------------------------------------
*/

#include "TSDesktop.h"

void RadioButton::setDotRadius(const int8_t aDotRadius)
{
  if (aDotRadius != dotRadius) {
    hide();
    dotRadius = aDotRadius;
    setChanged();
  }
}

void RadioButton::setCircleRadius(const int8_t aCircleRadius)
{
  if (aCircleRadius != circleRadius) {
    hide();
    circleRadius = aCircleRadius;
    setChanged();
  }
}

const int16_t RadioButton::getCenterX()
{
  switch (decorPosMode) {
  case DECOR_POS_LEFT:
    return decorMarginLeft + getCircleRadius();
  case DECOR_POS_RIGHT:
    return getTextWidth() + decorMarginLeft + getCircleRadius();
  case DECOR_POS_CENTER:
    return (getTextWidth() - decorMarginLeft - decorMarginRight) / 2;
  default:
    return 0;
  }
}

const int16_t RadioButton::getCenterY()
{
  int16_t h = 0;
  switch (decorAlign) {
  case DECOR_ALIGN_NONE:
  case DECOR_ALIGN_TOP:
    h = getTextHeight() / 2;
    break;
  case DECOR_ALIGN_BOTTOM:
    h = getTextHeight() + getTextHeight() / 2;
    break;
  }
  return decorMarginTop + h;
}

void RadioButton::drawCircle(const rgb_t aCircleColor)
{
  clip_t clip;
  getOuterClip(clip);
  screen()->drawCircle(clip, getAbsInnerLeft(getCenterX()), getAbsInnerTop(getCenterY()), getCircleRadius(), aCircleColor);
}

void RadioButton::drawCircle()
{
  drawCircle(state ? activeColor : inActiveColor);
}

void RadioButton::drawDot(const rgb_t aDotColor)
{
  clip_t clip;
  getOuterClip(clip);
  screen()->fillCircle(clip, getAbsInnerLeft(getCenterX()), getAbsInnerTop(getCenterY()), getDotRadius(), aDotColor);
}

void RadioButton::drawDot()
{
  drawDot(state ? activeColor : getBackgroundColor());
}

void RadioButton::drawDecor()
{
  drawCircle();
  drawDot();
}

void RadioButton::hideDecor()
{
  drawDot(getBackgroundColor());
}

const bool RadioButton::toggle()
{
  Container* parent = getParent();
  FieldSet* fset = NULL;
  while (parent) {
    if (parent->getType().type == TYPE_FIELDSET.type) {
      fset = static_cast<FieldSet*>(parent);
      if (!fset->markedSubFolder()) {
        break;
      }
      fset = NULL;
    }
    parent = parent->getParent();
  }
  if (fset) {
    fset->setActiveRadio(this);
  }
  else {
    setState(!state);
  }
  return state;
}
