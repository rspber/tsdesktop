
#pragma once

#include <rgb.h>

#define GNT_LR 1 // horizontal gradient left to right
#define GNT_TB 2 // vertical gradient top to bottom
#define GNT_RL 3 // horizontal gradient right to left
#define GNT_BT 4 // vertical gradient bottom to top

typedef struct {
  uint8_t deg;  // 1,3 - horizontal, 2,4 -vertical
  rgb_t color1;
  int8_t percent; // 0 .. 50 .. 100
  rgb_t color2;
//  int8_t gradient = 0;
} gradient_t;

