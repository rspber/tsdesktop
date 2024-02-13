/*
  TouchScreenDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

/*
----------------------------------------------------------------
                        FieldSet Class
----------------------------------------------------------------
*/

#include "TSDesktop.h"
#include <stdlib.h>

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
    //      if (getAlignLeft()) {  // left is default
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
    //    if (getAlignTop()) { // top is default
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
    int16_t maxw = screen()->width() - updLeft;
    switch (getOrgWidth()) {
      case ALIGN_CLIENT:
        if (updWidth != maxw) {
          hide();
          updWidth = maxw;
          setChanged();
        }
        break;
      case ALIGN_COMPACT: // has no effect as desktop
          // it should be placed on transparent client align desktop
        break;
      default:
        if (updWidth > maxw) {
          hide();
          updWidth = maxw;
          setChanged();
        }
    }
    int16_t maxh = screen()->height() - updTop;
    switch (getOrgHeight()) {
      case ALIGN_CLIENT:
        if (updHeight != maxh) {
          hide();
          updHeight = maxh;
          setChanged();
        }
        break;
      case ALIGN_COMPACT: // has no effect as desktop
          // it should be placed on transparent client align desktop
        break;
      default:
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

const bool FieldSet::isBorderRadius()
{
  if (Button::isBorderRadius()) {
    return true;
  }
  for (int16_t i = len; --i >= 0; ) {
    Container* b = children[i];
    if (b->getAbsVisible()) {
      if (b->isBorderRadius()) {
        int16_t le = marginLeft + b->getUpdLeft();
        int16_t tp = marginTop + b->getUpdTop();
        int16_t wh = b->getUpdWidth();
        int16_t ht = b->getUpdHeight();
        uint8_t bs = b->getBorderSize();
        if (
          le - bs < 0 ||
          tp - bs < 0 ||
          le + wh + bs >= updWidth ||
          tp + ht + bs >= updHeight)
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
  rgb_t bg = getBackgroundColor();
  if (bg == NO_BACKGROUND_COLOR) {
    backgroundDrawn = true;
  }
  if (isBorderRadius()) {
    backgroundDrawn = true;
  }
  if (backgroundDrawn) {
    Button::drawBackground();
  }
}

const int16_t innerCovers(clip_t* p, int16_t n, const int16_t posX, const int16_t posY)
{
  while (--n >= 0) {
    if (posX >= p->x1 && posX < p->x2 && posY >= p->y1 && posY < p->y2) {
      return p->x2 - posX;
    }
    ++p;
  }
  return 0;
}

void FieldSet::drawVisibleBackground()
{
  rgb_t bg = getBackgroundColor();
  if (bg == NO_BACKGROUND_COLOR) {
    return;
  }
  clip_t clip;
  getOuterClip(clip);
  int16_t x = clip.x1;
  int16_t y = clip.y1;
  int16_t w = clip.width();
  int16_t h = clip.height();

  clip_t tmp[20];

  int16_t n = 0;
  for (int16_t i = len; --i >= 0; ) {
    Container* b = children[i];
    n += b->getAbsVisible();
  }
  clip_t* cps = n <= 20 ? tmp : (clip_t*)malloc(n * sizeof(clip_t));
  int16_t mini = 0x7fff, minj = 0x7fff, maxi = 0, maxj = 0;
  {
    clip_t* p = cps;
    for (int16_t i = 0; i < len; ++i ) {
      Container* b = children[i];
      if (b->getAbsVisible()) {
        int16_t x1 = b->getUpdLeft() + marginLeft - offsetLeft;
        int16_t x2 = x1 + b->getUpdWidth();
        int16_t y1 = b->getUpdTop() + marginTop - offsetTop;
        int16_t y2 = y1 + b->getUpdHeight();
        {
          clip_t cp;
          b->getOuterClip(cp);
          if (x2 > cp.x2 - x) {
            x2 = cp.x2 - x;
          }
          if (y2 > cp.y2 - y) {
            y2 = cp.y2 - y;
          }
        }
        {
          uint8_t bs = b->getBorderSize();
          x1 += -bs;
          x2 += +bs;
          y1 += -bs;
          y2 += +bs;
        }
        if (x1 < mini) {
          mini = x1;
        }
        if (x2 > maxi) {
          maxi = x2;
        }
        if (y1 < minj) {
          minj = y1;
        }
        if (y2 > maxj) {
          maxj = y2;
        }
        p->x1 = x1;
        p->x2 = x2;
        p->y1 = y1;
        p->y2 = y2;
        ++p;
      }
    }
  }

  if (mini < 0) {
    mini = 0;
  }
  if (minj < 0) {
    minj = 0;
  }

  TSD_SCREEN* scr = screen();
  scr->startWrite();

  if (n > 1) {
    clip_t* p = cps;
    int16_t x1 = p->x1, x2 = p->x2, y1 = p->y1, y2 = p->y2;
    bool dnok = true, rgok = true;
    for (int16_t i = 1; i < n; ++i ) {
      ++p;
      if (dnok && p->y1 >= y2 && p->x1 == x1 && p->x2 == x2) {
        y2 = p->y2;
      }
      else {
        dnok = false;
      }
      if (rgok && p->x1 >= x2 && p->y1 == y1 && p->y2 == y2) {
        x2 = p->x2;
      }
      else {
        rgok = false;
      }
    }

    if (dnok) {
      clip_t* p = cps;
      for (int16_t i = 1; i < n; ++i ) {
        y1 = p->y2;
        if (y1 >= maxj) {
          break;
        }
        ++p;
        y2 = p->y1;
        if (y2 > maxj) {
          y2 = maxj;
        }
        if (y2 > minj) {
          if (y1 < minj) {
            y1 = minj;
          }
          scr->fillRectHelper(clip, x + x1, y + y1, x2 - x1, y2 - y1, bg);
        }
      }
    }
    else
    if (rgok) {
      clip_t* p = cps;
      for (int16_t i = 1; i < n; ++i ) {
        x1 = p->x2;
        if (x1 >= maxi) {
          break;
        }
        ++p;
        x2 = p->x1;
        if (x2 > maxi) {
          x2 = maxi;
        }
        if (x2 > mini) {
          if (x1 < mini) {
            x1 = mini;
          }
          scr->fillRectHelper(clip, x + x1, y + y1, x2 - x1, y2 - y1, bg);
        }
      }
    }
    else {
      int16_t j = minj;
      while (j < maxj) {
        int16_t i0 = mini;
        int16_t i = i0;
        while (i < maxi) {
          int16_t t = innerCovers(cps, n, i, j);
          if (t) {
            if (i > i0) {
              scr->drawFastHLine(clip, x + i0, y + j, i - i0, bg);
            }
            i += t;
            i0 = i;
            continue;
          }
          ++i;
        }
        if (i > i0) {
          scr->drawFastHLine(clip, x + i0, y + j, i - i0, bg);
        }
        ++j;
      }
    }
  }
  
  if (mini > 0) {
    scr->fillRectHelper(clip, x, y, mini, h, bg);
  }
  if (minj > 0) {
    scr->fillRectHelper(clip, x + mini, y, maxi - mini, minj, bg);
  }
  if (maxj < h) {
    scr->fillRectHelper(clip, x + mini, y + maxj, maxi - mini, h - maxj, bg);
  }
  if (maxi < w) {
    scr->fillRectHelper(clip, x + maxi, y, w - maxi, h, bg);
  }

  scr->endWrite();

  if (n > 20) {
    free(cps);
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
