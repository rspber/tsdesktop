/*
  Canvas

  Copyright (c) 2024, rspber (https://github.com/rspber)

*/

#pragma once

#include "GfxObject.h"
#include "BufferedDisplay.h"

/// @Canvas

class Canvas : public GfxObject {
public:
  Canvas(const int16_t ax1, const int16_t ay1, rgb_t acolor = BLACK) : GfxObject(ax1, ay1, acolor) {}
  virtual ~Canvas();

  void setBuffered(const bool aBuffered);
  bool getBuffered() { return buffered; }

  GfxObject* remove(GfxObject* item);
  GfxObject* remove(int16_t index);

  int16_t index(GfxObject* item);

  void add(GfxObject* item);
  void add(GfxObject& item);

  void animate(GfxObject& item, const int16_t dx, int16_t dy);

  // absolute location of canvas in screen()
  virtual void getOuterClip(clip_t& clip) = 0;

  void dodraw(clip_t& clip, int16_t left, int16_t top) override;

  void innerDraw(bool redraw = false);

  BufferedDisplay* bf();

  virtual TSD_SCREEN* superScreen();
  TSD_SCREEN* screen() override;

private:
  GfxObject** list = NULL;
  int16_t len = 0, count = 0;
  bool buffered = false;
  BufferedDisplay* bf_ = 0;
};
