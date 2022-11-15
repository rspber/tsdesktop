/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

/*
----------------------------------------------------------------
                        FieldSet Class
----------------------------------------------------------------
*/

#include "TSDesktop.h"

void FieldSet::setOrientation(const orient_t aOrientation, const int8_t aDistance)
{
  hide();
  orientation = aOrientation;
  distance = aDistance >= 0 ? aDistance : 0;
  setChanged();
}


// updateCoordHorizontal

void FieldSet::updCompactWidth(const bool recalc)
{
  updateCoord(recalc);
  int16_t maxw = 0;
  for (int16_t i = len; --i >= 0; ) {
    Container* b = children[i];
    if (!b->getHidden()) {
      b->updateCoord(true);
      int16_t w = b->getUpdLeft() + b->getUpdWidth();
      if (w > maxw) {
        maxw = w;
      }
    }
  }
  setUpdWidth(marginLeft + maxw + marginRight);
}

const int16_t FieldSet::updCoordHorizGetStretch()
{
  int16_t iw = 0;
  int16_t nclnts = 0;
  bool nxt = false;
  for (int16_t i = len; --i >= 0; ) {
    Container* b = children[i];
    if (!b->getHidden()) {
      if (b->getOrgHeight() == ALIGN_CLIENT) {
        b->setUpdHeight(updHeight - marginTop - marginBottom);
      }
      int16_t z = b->getOrgWidth();
      switch (z) {
      case ALIGN_CLIENT:
        nclnts++;
        break;
      case ALIGN_COMPACT:
        b->updCompactWidth(true);
        // keep going
      default:
        iw += b->getUpdWidth();
      }
      if (nxt) {
        iw += distance;
      }
      nxt = true;
    }
  }
  int16_t w = updWidth - marginLeft - marginRight;
  if (w > iw) {
    if (nclnts > 0) {
      iw = (w - iw) / nclnts;
      for (int16_t i = len; --i >= 0; ) {
        Container* b = children[i];
        if (!b->getHidden()) {
          int16_t z = b->getOrgWidth();
          if (z == ALIGN_CLIENT) {
            b->setUpdWidth(iw);
            b->updateCoord(true);
          }
        }
      }
      return 0;
    }
    else {
      return (w - iw) / (len + 1);
    }
  }
  return 0;
}

void FieldSet::updCoordHorizUpdPos(Container* b, const int16_t x)
{
  int16_t h = updHeight - marginTop - marginBottom;
  int16_t z = b->getOrgHeight();
  if (z == ALIGN_CLIENT) {
    b->setUpdHeight(h);
    b->updateCoord(true);
  }
  int16_t ih = b->getUpdHeight();
  int16_t y;
  if (alignBottom) {
    y = h - ih;
  }
  else {
    if (alignCenterVert) {
      y = (h - ih) / 2;
    }
    else {  // top is default
      y = 0;
    }
  }
  b->setUpdPos(x, y);
}

void FieldSet::updateCoordHorizontal()
{
  int16_t stretch = updCoordHorizGetStretch();

  if (!alignCenterHoriz) {
    stretch = 0;
  }

  int16_t h = updHeight - marginTop - marginBottom;
  for (int16_t i = len; --i >= 0; ) {
    Container* b = children[i];
    if (!b->getHidden()) {
      int16_t z = b->getOrgHeight();
      if (z == ALIGN_COMPACT) {
        b->updCompactHeight(true);
        int16_t ih = b->getUpdHeight();
        if (ih > h) {
          h = ih;
        }
      }
    }
  }
  setUpdHeight(marginTop + h + marginBottom);

  if (alignRight) {
    int16_t w = updWidth - marginLeft - marginRight;
    int16_t x = w - stretch;
    for (int16_t i = len; --i >= 0; ) {
      Container* b = children[i];
      if (!b->getHidden()) {
        x -= b->getUpdWidth();
        updCoordHorizUpdPos(b, x);
        x -= distance + stretch;
      }
    }
  }
  else {
    //      if( getAlignLeft() ) {  // left is default
    int16_t x = stretch;
    for (int16_t i = 0; i < len; i++) {
      Container* b = children[i];
      if (!b->getHidden()) {
        updCoordHorizUpdPos(b, x);
        x += b->getUpdWidth() + distance + stretch;
      }
    }
    //      }
  }
}

// updateCoordVertical

void FieldSet::updCompactHeight(const bool recalc)
{
  updateCoord(recalc);
  int16_t maxh = 0;
  for (int16_t i = len; --i >= 0; ) {
    Container* b = children[i];
    if (!b->getHidden()) {
      b->updateCoord(true);
      int16_t h = b->getUpdTop() + b->getUpdHeight();
      if (h > maxh) {
        maxh = h;
      }
    }
  }
  setUpdHeight(marginTop + maxh + marginBottom);
}

const int16_t FieldSet::updCoordVertGetStretch()
{
  int16_t ih = 0;
  int16_t nclnts = 0;
  bool nxt = false;
  for (int16_t i = len; --i >= 0; ) {
    Container* b = children[i];
    if (!b->getHidden()) {
      if (b->getOrgWidth() == ALIGN_CLIENT) {
        b->setUpdWidth(updWidth - marginLeft - marginRight);
      }
      int16_t z = b->getOrgHeight();
      switch (z) {
      case ALIGN_CLIENT:
        nclnts++;
        break;
      case ALIGN_COMPACT:
        b->updCompactHeight(true);
        // keep going
      default:
        ih += b->getUpdHeight();
      }
      if (nxt) {
        ih += distance;
      }
      nxt = true;
    }
  }
  int16_t h = updHeight - marginTop - marginBottom;
  if (h > ih) {
    if (nclnts > 0) {
      ih = (h - ih) / nclnts;
      for (int16_t i = len; --i >= 0; ) {
        Container* b = children[i];
        if (!b->getHidden()) {
          int16_t z = b->getOrgHeight();
          if (z == ALIGN_CLIENT) {
            b->setUpdHeight(ih);
            b->updateCoord(true);
          }
        }
      }
      return 0;
    }
    else {
      return (h - ih) / (len + 1);
    }
  }
  return 0;
}

void FieldSet::updCoordVertUpdPos(Container* b, const int16_t y)
{
  int16_t w = updWidth - marginLeft - marginRight;
  int16_t z = b->getOrgWidth();
  if (z == ALIGN_CLIENT) {
    b->setUpdWidth(w);
    b->updateCoord(true);
  }
  int16_t iw = b->getUpdWidth();
  int16_t x;
  if (alignRight) {
    x = w - iw;
  }
  else {
    if (alignCenterHoriz) {
      x = (w - iw) / 2;
    }
    else {  // left is default
      x = 0;
    }
  }
  b->setUpdPos(x, y);
}

void FieldSet::updateCoordVertical()
{
  int16_t stretch = updCoordVertGetStretch();

  if (!alignCenterVert) {
    stretch = 0;
  }

  int16_t w = updWidth - marginLeft - marginRight;
  for (int16_t i = len; --i >= 0; ) {
    Container* b = children[i];
    if (!b->getHidden()) {
      int16_t z = b->getOrgWidth();
      if (z == ALIGN_COMPACT) {
        b->updCompactWidth(true);
        int16_t iw = b->getUpdWidth();
        if (iw > w) {
          w = iw;
        }
      }
    }
  }
  setUpdWidth(marginLeft + w + marginRight);

  if (alignBottom) {
    int16_t h = updHeight - marginTop - marginBottom;
    int16_t y = h - stretch;
    for (int16_t i = len; --i >= 0; ) {
      Container* b = children[i];
      if (!b->getHidden()) {
        y -= b->getUpdHeight();
        updCoordVertUpdPos(b, y);
        y -= distance + stretch;
      }
    }
  }
  else {
    //    if( getAlignTop() ) { // top is default
    int16_t y = stretch;
    for (int16_t i = 0; i < len; i++) {
      Container* b = children[i];
      if (!b->getHidden()) {
        updCoordVertUpdPos(b, y);
        y += b->getUpdHeight() + distance + stretch;
      }
    }
    //    }
  }
}

// updateCoord

void FieldSet::updateCoord(const bool recalc)
{
  if (!getParent()) {
    switch (getOrgWidth()) {
      case ALIGN_CLIENT:
        if (updWidth != display.width()) {
          hide();
          updWidth = display.width();
          setChanged();
        }
        break;
      case ALIGN_COMPACT: // has no effect as desktop
          // it should be placed on transparent client align desktop
        break;
      default:
        int16_t maxw = display.width() - updLeft;
        if (updWidth > maxw) {
          hide();
          updWidth = maxw;
          setChanged();
        }
    }
    switch (getOrgHeight()) {
      case ALIGN_CLIENT:
        if (updHeight != display.height()) {
          hide();
          updHeight = display.height();
          setChanged();
        }
        break;
      case ALIGN_COMPACT: // has no effect as desktop
          // it should be placed on transparent client align desktop
        break;
      default:
        int16_t maxh = display.height() - updTop;
        if (updHeight > maxh) {
          hide();
          updHeight = maxh;
          setChanged();
        }
    }
  }
  if (!updated || recalc) {
    updated = true;
    if (orientation.vertical) {
      updateCoordVertical();
    }
    else {
      updateCoordHorizontal();
    }
  }
}

void FieldSet::setNotWasDrawn()
{
  Button::setNotWasDrawn();
  for (int16_t i = 0; i < len; i++) {
    Container* b = children[i];
    b->setNotWasDrawn();
  }
}

const bool FieldSet::isRadius()
{
  if (Button::isRadius()) {
    return true;
  }
  for (int16_t i = len; --i >= 0; ) {
    Container* b = children[i];
    if (b->getAbsVisible()) {
      if (b->isRadius()) {
        if (
          b->getUpdLeft() - b->getBorderSize() < 0 ||
          b->getUpdTop() - b->getBorderSize() < 0 ||
          b->getUpdLeft() + b->getUpdWidth() + b->getBorderSize() >= updWidth ||
          b->getUpdTop() + b->getUpdHeight() + b->getBorderSize() >= updHeight)
        {
          return true;
        }
      }
    }
  }
  return false;
}

void FieldSet::drawBackground()
{
  backgroundDrawn = false;
  if (radius > 0) {
    backgroundDrawn = true;
  }
  int32_t area = 0;
  for (int16_t i = len; --i >= 0; ) {
    Container* b = children[i];
    if (b->getAbsVisible()) {
      if (b->isRadius()) {
        backgroundDrawn = true;
        break;
      }
      int32_t a = b->getUpdWidth() * b->getUpdHeight();
      area += a;
    }
  }
  if (area * 2 < updWidth * updHeight) {
    backgroundDrawn = true;
  }
  if (backgroundDrawn) {
    Button::drawBackground();
  }
}

const int16_t FieldSet::innerCovers(const int16_t posX, const int16_t posY)
{
  for (int16_t i = len; --i >= 0; ) {
    Container* b = children[i];
    int16_t t = b->covers(posX, posY);
    if (t) {
      return t;
    }
  }
  return 0;
}

void FieldSet::drawVisibleBackground()
{
  rgb_t bg = getBackgroundColor();
  clip_t clip;
  getOuterClip(clip, true); // important setting pageWidth, pageHeight
  int16_t x = clip.x1;
  int16_t y = clip.y1;
  int16_t w = getPageWidth();
  int16_t iw = w < updWidth ? w - marginRight : w;
  int16_t h = getPageHeight();
  int16_t jh = h < updHeight ? h - marginBottom : h;

  for (int16_t j = 0; j < h; ++j ) {
    int16_t i0 = 0;
    int16_t i = 0;
    while (i < w) {
      if (i < iw && j < jh) {
        int16_t t = innerCovers(i - marginLeft + offsetLeft, j - marginTop + offsetTop);
        if (t) {
          if (i > i0) {
            display.drawFastHLine(&clip, x + i0, y, i - i0, bg);
          }
          i += t;
          i0 = i;
          continue;
        }
      }
      ++i;
    }
    if (i > i0) {
      display.drawFastHLine(&clip, x + i0, y, i - i0, bg);
    }
    ++y;
  }

}

void FieldSet::innerDraw(const bool redraw)
{
  for (int16_t i = 0; i < len; i++) {
    Container* b = children[i];
    b->draw(redraw);
  }
  if (!backgroundDrawn) {
    drawVisibleBackground();
  }
  drawScroller();
}

Container* FieldSet::pressed(const int16_t xScreen, const int16_t yScreen)
{
  if (scrollerPressed(xScreen, yScreen)) {
    return NULL;
  }
  for (int16_t i = len; --i >= 0; ) {
    Container* b = children[i];
    b = b->pressed(xScreen, yScreen);
    if (b) {
      return b;
    }
  }
  return Button::pressed(xScreen, yScreen);
}

void FieldSet::setActiveRadio(RadioButton* aActiveRadio)
{
  if (activeRadio != aActiveRadio) {
    if (activeRadio) {
      activeRadio->setState(false);
    }
    activeRadio = aActiveRadio;
    if (activeRadio) {
      activeRadio->setState(true);
    }
  }
}
