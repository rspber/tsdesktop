
#pragma once

#include <inttypes.h>

struct tsd_clip {
  int16_t x1, y1, x2, y2;
  const int16_t width();
  const int16_t height();
};

typedef tsd_clip clip_t;
