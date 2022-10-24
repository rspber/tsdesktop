/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

/*
----------------------------------------------------------------
                        Container Class
----------------------------------------------------------------
*/

#include "TSDesktop.h"

void Container::setParent(const Container* aParent)
{
  if (aParent != parent) {
    hide();
    parent = (Container*)aParent;
    setChanged();
  }
}

void Container::setVisible(const bool aVisible)
{
  if (visible != aVisible) {
    hide();
    visible = aVisible;
    setChanged();
  }
}

void Container::setHidden(const bool aHidden)
{
  if (hidden != aHidden) {
    hide();
    hidden = aHidden;
    setChanged();
  }
}

void Container::setMargin(const uint8_t aLeft, const uint8_t aTop, const uint8_t aRight, const uint8_t aBottom)
{
  if (marginLeft != aLeft || marginTop != aTop || marginRight != aRight || marginBottom != aBottom) {
    hide();
    marginLeft = aLeft;
    marginTop = aTop;
    marginRight = aRight;
    marginBottom = aBottom;
    setChanged();
  }
}

void Container::setLeftMargin(const uint8_t aLeftMargin)
{
  if (marginLeft != aLeftMargin) {
    hide();
    marginLeft = aLeftMargin;
    setChanged();
  }
}

void Container::setTopMargin(const uint8_t aTopMargin)
{
  if (marginTop != aTopMargin) {
    hide();
    marginTop = aTopMargin;
    setChanged();
  }
}

void Container::setRightMargin(const uint8_t aRightMargin)
{
  if (marginRight != aRightMargin) {
    hide();
    marginRight = aRightMargin;
    setChanged();
  }
}

void Container::setBottomMargin(const uint8_t aBottomMargin)
{
  if (marginBottom != aBottomMargin) {
    hide();
    marginBottom = aBottomMargin;
    setChanged();
  }
}

void Container::setLeft(const int16_t aLeft)
{
  if (orgLeft != aLeft) {
    hide();
    orgLeft = aLeft;
    if (aLeft >= 0) {
      updLeft = aLeft;
    }
    setChanged();
  }
}

void Container::setTop(const int16_t aTop)
{
  if (orgTop != aTop) {
    hide();
    orgTop = aTop;
    if (aTop >= 0) {
      updTop = aTop;
    }
    setChanged();
  }
}

void Container::setWidth(const int16_t aWidth)
{
  if (orgWidth != aWidth) {
    hide();
    orgWidth = aWidth;
    if (aWidth >= 0) {
      updWidth = aWidth;
    }
    setChanged();
  }
}
void Container::setHeight(const int16_t aHeight)
{
  if (orgHeight != aHeight) {
    hide();
    orgHeight = aHeight;
    if (aHeight >= 0) {
      updHeight = aHeight;
    }
    setChanged();
  }
}

void Container::setAlignLeft(const bool aAlignLeft)
{
  if (aAlignLeft != alignLeft || alignCenterHoriz) {
    hide();
    alignLeft = aAlignLeft;
    alignCenterHoriz = false;
    setChanged();
  }
}

void Container::setAlignTop(const bool aAlignTop)
{
  if (aAlignTop != alignTop || alignCenterVert) {
    hide();
    alignTop = aAlignTop;
    alignCenterVert = false;
    setChanged();
  }
}

void Container::setAlignRight(const bool aAlignRight)
{
  if (aAlignRight != alignRight || alignCenterHoriz) {
    hide();
    alignRight = aAlignRight;
    alignCenterHoriz = false;
    setChanged();
  }
}

void Container::setAlignBottom(const bool aAlignBottom)
{
  if (aAlignBottom != alignBottom || alignCenterVert) {
    hide();
    alignBottom = aAlignBottom;
    alignCenterVert = false;
    setChanged();
  }
}

void Container::setAlignCenter(const bool aAlignCenter)
{
  if (aAlignCenter != alignCenterHoriz || aAlignCenter != alignCenterVert) {
    hide();
    alignCenterHoriz = aAlignCenter;
    alignCenterVert = aAlignCenter;
    setChanged();
  }
}

void Container::setAlignCenterHoriz(const bool aAlignCenterHoriz)
{
  if (aAlignCenterHoriz != alignCenterHoriz) {
    hide();
    alignCenterHoriz = aAlignCenterHoriz;
    setChanged();
  }
}

void Container::setAlignCenterVert(const bool aAlignCenterVert)
{
  if (aAlignCenterVert != alignCenterVert) {
    hide();
    alignCenterVert = aAlignCenterVert;
    setChanged();
  }
}

void Container::setAlignClient()
{
  if (orgWidth != ALIGN_CLIENT || orgHeight != ALIGN_CLIENT) {
    hide();
    orgWidth = ALIGN_CLIENT;
    orgHeight = ALIGN_CLIENT;
    setChanged();
  }
}

void Container::setAlignClientHoriz()
{
  if (orgWidth != ALIGN_CLIENT) {
    hide();
    orgWidth = ALIGN_CLIENT;
    setChanged();
  }
}

void Container::setAlignClientVert()
{
  if (orgHeight != ALIGN_CLIENT) {
    hide();
    orgHeight = ALIGN_CLIENT;
    setChanged();
  }
}

void Container::setAlignCompact()
{
  if (orgWidth != ALIGN_COMPACT || orgHeight != ALIGN_COMPACT) {
    hide();
    orgWidth = ALIGN_COMPACT;
    orgHeight = ALIGN_COMPACT;
    setChanged();
  }
}

void Container::setAlignCompactHoriz()
{
  if (orgWidth != ALIGN_COMPACT) {
    hide();
    orgWidth = ALIGN_COMPACT;
    setChanged();
  }
}

void Container::setAlignCompactVert()
{
  if (orgHeight != ALIGN_COMPACT) {
    hide();
    orgHeight = ALIGN_COMPACT;
    setChanged();
  }
}

void Container::setUpdLeft(const int16_t aLeft)
{
  int16_t i = aLeft > 0 ? aLeft : 0;
  if (i != updLeft) {
    hide();
    updLeft = i;
  }
}

void Container::setUpdTop(const int16_t aTop)
{
  int16_t i = aTop > 0 ? aTop : 0;
  if (i != updTop) {
    hide();
    updTop = i;
  }
}

void Container::setUpdPos(const int16_t aLeft, const int16_t aTop)
{
  setUpdLeft(aLeft);
  setUpdTop(aTop);
}

bool Container::setUpdWidth(const int16_t aWidth)
{
  int16_t i = aWidth > 0 ? aWidth : 0;
  if (i != updWidth) {
    hide();
    updWidth = i;
    return true;
  }
  return false;
}

bool Container::setUpdHeight(const int16_t aHeight)
{
  int16_t i = aHeight > 0 ? aHeight : 0;
  if (i != updHeight) {
    hide();
    updHeight = i;
    return true;
  }
  return false;
}

const int16_t Container::getAbsLeft()
{
  return (parent ? parent->getAbsLeft(0) : 0) + updLeft;
}

const int16_t Container::getAbsLeft(const int16_t pos)
{
  return getAbsLeft() + marginLeft + pos;
}

const int16_t Container::getAbsTop()
{
  return (parent ? parent->getAbsTop(0) : 0) + updTop;
}

const int16_t Container::getAbsTop(const int16_t pos)
{
  return getAbsTop() + marginTop + pos;
}

const bool Container::getAbsVisible()
{
  if (visible && !hidden) {
    return parent ? parent->getAbsVisible() : true;
  }
  else {
    return false;
  }
}


void Container::setChanged()
{
  updated = false;
  if (parent) {
    parent->setChanged();
  }
}

const uint16_t Container::getBackgroundColor()
{
  if (type > 0) {   // Button
    Button* my = static_cast<Button*>(this);
    if (!my->transparent && my->backgroundColor != NO_BACKGROUND_COLOR) {
      return my->backgroundColor;
    }
  }
  if (parent) {
    return parent->getBackgroundColor();
  }
  return NO_BACKGROUND_COLOR;
}

Container* Container::pressed(const int16_t xScreen, const int16_t yScreen)
{
  if (!disabled && getAbsVisible()) {
    int16_t absLeft = getAbsLeft();
    int16_t absTop = getAbsTop();
    bool pressed = (xScreen > absLeft && xScreen < absLeft + updWidth) && (yScreen > absTop && yScreen < absTop + updHeight);
    if (pressed) {
      int16_t posX = xScreen - absLeft;
      int16_t posY = yScreen - absTop;

      clickEffect(posX, posY);

      if (funcOnClick) {
        funcOnClick(this);
      }
    }
    return pressed ? this : 0;
  }
  else {
    return 0;
  }
}

