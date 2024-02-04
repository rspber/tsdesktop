/*
  Touch Screen for TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#pragma once

#include <Setup.h>
#include <inttypes.h>

typedef struct {
  int16_t x, y;
} point_t;

#if defined(TOUCH_SPI) || defined(TOUCH_PIO_SPI)
  #include <TSD_XPT2046.h>
  #define TOUCH_DRIVER TSD_XPT2046
#elif defined(TOUCH_RESISTIVE)
  #include <TSD_SPFD5408.h>
  #define TOUCH_DRIVER TSD_SPFD5408
#endif 

#if defined(TOUCH_DRIVER)

  class Touch : public TOUCH_DRIVER {
  public:
    Touch() : TOUCH_DRIVER() {}
    void setRotation(const uint8_t aRotation, const uint8_t aReverseMode = 0)
    {
      rotation = aRotation;
      reverse = aReverseMode;
    }
    bool getTouch(point_t* p) { return WhereAreYou(p); }
  protected:
    bool WhereAreYou(point_t* p);
  private:
    uint8_t rotation;
    uint8_t reverse;
  };

#else

  class Touch {
  public:
    Touch() {}
    void begin() {
#ifdef TOUCH_SPI_CS
      if (TOUCH_SPI_CS >= 0) {
        pinMode(TOUCH_SPI_CS, OUTPUT);
        digitalWrite(TOUCH_SPI_CS, HIGH);
      }
#endif
    }
    void setRotation(const uint8_t aRotation, const uint8_t aReverseMode = 0) {}
    bool getTouch(point_t* p) { return false; }
  };

#endif
