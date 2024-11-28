
#include "t_cmap.h"

void tft_cmap::rememberColor(const rgb_t color, const int idx)
{
  if( rLen < len ) {
    cMap[len + rLen++] = color;
    cMap[len + rLen++] = idx;
  }
}

inline int tft_cmap::getMapColorIdx_(const rgb_t color, const uint32_t mask)
{
  for( int i = len; --i >= 0; ) {
    if( (color & mask) == (cMap[i] & mask) ) {
      rememberColor(color, i);
      return i;
    }
  }
  return -1;
}

int tft_cmap::getMapColorIdx(const rgb_t color)
{
  for( int i = rLen; --i >= 0; ) {
    if( (cMap[len + i*2] & 0x00FFFFFF) == (color & 0x00FFFFFF) ) {
      return cMap[len + i*2 + 1];
    }
  }
  int i;
  i = getMapColorIdx_(color, 0x00FFFFFF);  if (i >= 0) return i;
  i = getMapColorIdx_(color, 0x00FEFEFE);  if (i >= 0) return i;
  i = getMapColorIdx_(color, 0x00FCFCFC);  if (i >= 0) return i;
  i = getMapColorIdx_(color, 0x00F8F8F8);  if (i >= 0) return i;
  i = getMapColorIdx_(color, 0x00F0F0F0);  if (i >= 0) return i;
  i = getMapColorIdx_(color, 0x00E0E0E0);  if (i >= 0) return i;
  i = getMapColorIdx_(color, 0x00C0C0C0);  if (i >= 0) return i;
  i = getMapColorIdx_(color, 0x00808080);  if (i >= 0) return i;
  return -1;
}

rgb_t tft_cmap::getMapColor(const rgb_t color)
{
  int i = getMapColorIdx(color);
  return i >= 0 ? cMap[i] : BLACK;
}
