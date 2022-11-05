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
  return (parent ? parent->getAbsInnerLeft(0) : 0) + updLeft;
}

const int16_t Container::getAbsInnerLeft(const int16_t pos)
{
  return getAbsOuterLeft() + marginLeft - offsetLeft + pos;
}

const int16_t Container::getAbsOuterTop()
{
  return (parent ? parent->getAbsInnerTop(0) : 0) + updTop;
}

const int16_t Container::getAbsInnerTop(const int16_t pos)
{
  return getAbsOuterTop() + marginTop - offsetTop + pos;
}

const int16_t Container::getAbsInnerRight(int16_t r, int16_t m2)
{
  int16_t m1 = marginLeft + marginRight;
  int16_t i = updWidth - m1 - m2 - offsetLeft;
  if( r > i ) {
     r = i;
  }
  return updLeft + marginLeft + (parent ? parent->getAbsInnerRight(r, m1) : r);
}

const int16_t Container::getAbsInnerBottom(int16_t b, int16_t m2)
{
  int16_t m1 = marginTop + marginBottom;
  int16_t i = updHeight - m1 - m2 - offsetTop;
  if( b > i ) {
    b = i;
  }
  return updTop + marginTop + (parent ? parent->getAbsInnerBottom(b, m1) : b);
}

clip_t* Container::getInnerClip(clip_t& clip)
{
  clip.x1 = getAbsInnerLeft(offsetLeft);
  clip.y1 = getAbsInnerTop(offsetTop);
  int i = getAbsInnerRight(0x7fff, 0);
  clip.x2 = i >= 0 ? i : 0;
      i = getAbsInnerBottom(0x7fff, 0);
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
    int16_t absLeft = getAbsOuterLeft();
    int16_t absTop = getAbsOuterTop();
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

const int16_t doScroll(const uint8_t tp, const bool up, int16_t offs, const int16_t size, const int16_t page, int8_t* percent)
{
  switch (tp) {
    case SCROLL_BTN_STEP:
      if (up) {
        offs -= 16;
        if (offs < 0) {
          offs = 0;
        }
      }
      else {
        offs += 16;
      }
      break;
    case SCROLL_BTN_PAGE:
      if (up) {
        offs -= page;
        if (offs < 0) {
          offs = 0;
        }
      }
      else {
        offs += page;
      }
      break;
    case SCROLL_BTN_HOME:
      offs = up ? 0 : 0x7fff;
      break;
  }
  int maxoffs = size - page;
  if (offs > maxoffs) {
    offs = maxoffs;
  }
  if (offs < 0) {
    offs = 0;
  }
  *percent = offs * 100 / maxoffs;
  return offs;
}

const bool Container::horizScroll(const uint8_t tp, const bool up, int8_t* percent)
{
  clip_t clip;
  getClip(clip);
  int16_t offs = doScroll(tp, up, offsetLeft, updWidth, clip.x2 - clip.x1, percent);
  if (offs != offsetLeft) {
    offsetLeft = offs;
    setChanged();
    return true;
  }
  return false;
}

const bool Container::vertScroll(const uint8_t tp, const bool up, int8_t* percent)
{
  clip_t clip;
  getClip(clip);
  int16_t offs = doScroll(tp, up, offsetTop, updHeight, clip.y2 - clip.y1, percent);
  if (offs != offsetTop) {
    offsetTop = offs;
    setChanged();
    return true;
  }
  return false;
}
