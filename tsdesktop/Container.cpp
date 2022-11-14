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
  if (orgLeft < 0 ) {
    int16_t i = aLeft > 0 ? aLeft : 0;
    if (i != updLeft) {
      hide();
      updLeft = i;
    }
  }
}

void Container::setUpdTop(const int16_t aTop)
{
  if (orgTop < 0 ) {
    int16_t i = aTop > 0 ? aTop : 0;
    if (i != updTop) {
      hide();
      updTop = i;
    }
  }
}

void Container::setUpdPos(const int16_t aLeft, const int16_t aTop)
{
  setUpdLeft(aLeft);
  setUpdTop(aTop);
}

bool Container::setUpdWidth(const int16_t aWidth)
{
  if (orgWidth < 0 ) {
    int16_t w = aWidth > 0 ? aWidth : 0;
    if (w != updWidth) {
      if (deep == 0 ) {
        if (orgWidth == ALIGN_COMPACT) {
          if (w > display.width()) {
            w = display.width();
          }
        }
        else {
          return false;
        }
      }
      hide();
      updWidth = w;
      return true;
    }
  }
  return false;
}

bool Container::setUpdHeight(const int16_t aHeight)
{
  if (orgHeight < 0 ) {
    int16_t h = aHeight > 0 ? aHeight : 0;
    if (h != updHeight) {
      if (deep == 0 ) {
        if (orgHeight == ALIGN_COMPACT) {
          if (h > display.height()) {
            h = display.height();
          }
        }
        else {
          return false;
        }
      }
      hide();
      updHeight = h;
      return true;
    }
  }
  return false;
}

const int16_t Container::getAbsOuterLeft()
{
  return updLeft + (parent ? parent->getAbsInnerLeft(0) : 0);
}

const int16_t Container::getAbsInnerLeft(const int16_t pos)
{
  return getAbsOuterLeft() + marginLeft - offsetLeft + pos;
}

const int16_t Container::getAbsOuterTop()
{
  return updTop + (parent ? parent->getAbsInnerTop(0) : 0);
}

const int16_t Container::getAbsInnerTop(const int16_t pos)
{
  return getAbsOuterTop() + marginTop - offsetTop + pos;
}

const int16_t Container::getClipLeft(int16_t l)
{
  return (parent ? parent->getClipLeft(updLeft) : 0) + ((l -= offsetLeft) > 0 ? l : 0) + marginLeft;
}

const int16_t Container::getClipTop(int16_t t)
{
  return (parent ? parent->getClipTop(updTop) : 0) + ((t -= offsetTop) > 0 ? t : 0) + marginTop ;
}

const int16_t Container::getClipRight(int16_t r, int16_t m2)
{
  int16_t m1 = marginLeft + marginRight;
  int16_t i = updWidth - m1 - m2 - offsetLeft;
  if( r > i ) {
     r = i;
  }
  return updLeft + marginLeft + (parent ? parent->getClipRight(r, updLeft + m1 + m2) : r);
}

const int16_t Container::getClipBottom(int16_t b, int16_t m2)
{
  int16_t m1 = marginTop + marginBottom;
  int16_t i = updHeight - m1 - m2 - offsetTop;
  if( b > i ) {
    b = i;
  }
  return updTop + marginTop + (parent ? parent->getClipBottom(b, updTop + m1 + m2) : b);
}

clip_t* Container::getInnerClip(clip_t& clip)
{
  clip.x1 = getClipLeft(0);
  clip.y1 = getClipTop(0);
  int i = getClipRight(0x7fff, 0);
  clip.x2 = i >= 0 ? i : 0;
      i = getClipBottom(0x7fff, 0);
  clip.y2 = i >= 0 ? i : 0;
  return &clip;
}

clip_t* Container::getClip(clip_t& clip)
{
  if (parent) {
    return parent->getInnerClip(clip);
  }
  else {
    clip.x1 = 0;
    clip.y1 = 0;
    clip.x2 = updWidth;
    clip.y2 = updHeight;
    return &clip;
  }
}

clip_t* Container::getOuterClip(clip_t& clip)
{
  getInnerClip(clip);
  clip.x1 -= marginLeft;
  clip.y1 -= marginTop;
  clip.x2 += marginRight;
  clip.y2 += marginBottom;
  pageWidth = clip.x2 - clip.x1;
  pageHeight = clip.y2 - clip.y1;
  return &clip;
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

const rgb_t Container::getBackgroundColor()
{
  if (parent) {
    return parent->getBackgroundColor();
  }
  return NO_BACKGROUND_COLOR;
}

const int16_t Container::covers(const int16_t posX, const int16_t posY)
{
  if (getAbsVisible()) {
    int16_t w = pageWidth > 0 ? pageWidth : updWidth;
    int16_t h = pageHeight > 0 ? pageHeight : updHeight;
    if (posX >= updLeft - borderSize && posX < updLeft + w + borderSize &&
        posY >= updTop - borderSize && posY < updTop + h + borderSize)
    {
      return updLeft + w + borderSize - posX;
    }
  }
  return 0;
}

Container* Container::pressed(const int16_t xScreen, const int16_t yScreen)
{
  if (!disabled && getAbsVisible()) {
    int16_t absLeft = getAbsOuterLeft();
    int16_t absTop = getAbsOuterTop();
    int16_t absRight = getClipRight(0x7fff, 0) + marginRight;
    int16_t absBottom = getClipBottom(0x7fff, 0) + marginBottom;
    bool pressed = (xScreen >= absLeft && xScreen < absRight) && (yScreen >= absTop && yScreen < absBottom);
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
