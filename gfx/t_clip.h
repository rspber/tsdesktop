
#pragma once

#include <inttypes.h>

struct tsd_clip {
  int32_t x1, y1, x2, y2;
  int32_t width();
  int32_t height();
};

typedef tsd_clip clip_t;
