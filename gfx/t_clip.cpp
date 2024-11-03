
#include "t_clip.h"

const int16_t tsd_clip::width()
{
  return x2 > x1 ? x2 - x1 : 0;
}

const int16_t tsd_clip::height()
{
  return y2 > y1 ? y2 - y1 : 0;
}
