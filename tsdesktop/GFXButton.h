/*
  GFXButton

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include "GFXObject.h"
#include "BufferedDisplay.h"

/// @GFXButton

class GFXButton : public Button {
public:
  GFXButton();
  virtual ~GFXButton();

  GFXButton(
    const int16_t aLeft,
    const int16_t aTop,
    const int16_t aWidth,
    const int16_t aHeight,
    rgb_t aBackgroundColor = BLACK,
    rgb_t aBorderColor = WHITE);

  void setBuffered(const bool aBuffered);
  bool getBuffered() { return buffered; }

  GFXObject* remove(GFXObject* item);
  GFXObject* remove(int16_t index);

  int16_t index(GFXObject* item);

  void add(GFXObject* item);
  void add(GFXObject& item);

  void animate(GFXObject& item, const int16_t dx, int16_t dy);

  void innerDraw(const bool redraw) override;

  void drawAll(bool redraw = true);

  BufferedDisplay* bf();
  
  TSD_SCREEN* screen() override;

private:
  GFXObject** list = NULL;
  int16_t len = 0, count = 0;
  bool buffered = false;
  BufferedDisplay* bf_ = 0;
};
