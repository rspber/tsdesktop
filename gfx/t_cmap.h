
#pragma once

#include "rgb.h"

struct tft_cmap {
  rgb_t* cMap;
  int len;
  int rLen;

  void rememberColor(const rgb_t color, const int idx);
  bool eqColorsMask(rgb_t c1, rgb_t c2, const uint32_t mask);
  inline int getMapColorIdx_(const rgb_t color, const uint32_t mask);
  int getMapColorIdx(const rgb_t color);
  rgb_t getMapColor(const rgb_t color);

};

typedef tft_cmap cmap_t;

inline bool eqColorsFC(rgb_t c1, rgb_t c2)
{
  return (c1 & 0x00FCFCFC) == (c2 & 0x00FCFCFC);
}
