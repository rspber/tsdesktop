/*
  GfxButton

  Copyright (c) 2023-2024, rspber (https://github.com/rspber)

*/

#pragma once

#include "TSDesktop.h"
#include "Canvas.h"

class GFXButtonCanvas : public Canvas {

  friend class GfxButton;

public:
  GFXButtonCanvas() : Canvas(0, 0, BLACK) {}

  void getInnerClip(clip_t& clip) override { gfxbtn->getInnerClip(clip); }

  rgb_t getBackgroundColor() override { return gfxbtn->getBackgroundColor(); }

  TSD_SCREEN* superScreen() override { return gfxbtn->screen(); }

  // irrelevant in this object
  int16_t lW() override  { return 0; }
  int16_t rW() override  { return 0; }
  int16_t tH() override  { return 0; }
  int16_t bH() override  { return 0; }

private:
  Button* gfxbtn;   // set by GfxButton constructor
};


/// @GfxButton

class GfxButton : public Button {
public:
  GfxButton() : GfxButton(-1, -1, ALIGN_CLIENT, ALIGN_CLIENT) {}

  GfxButton(
    const int16_t aLeft,
    const int16_t aTop,
    const int16_t aWidth,
    const int16_t aHeight,
    rgb_t aBackgroundColor = BLACK,
    rgb_t aBorderColor = WHITE)
  : Button(TYPE_GFXBUTTON, aLeft, aTop, aWidth, aHeight, aBackgroundColor, aBorderColor)
  {
    canvas.gfxbtn = this;
    canvas.setX1(0);  // possibly some displacement in the button area
    canvas.setY1(0);  // possibly some displacement in the button area
  }

  Canvas* getCanvas() { return &canvas; }

  void innerDraw(const bool redraw) override
  {
    canvas.innerDraw(redraw);
  }

private:
  GFXButtonCanvas canvas;
};
