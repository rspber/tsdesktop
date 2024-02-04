/*
  HX8357B driver
*/

#pragma once

#include <TFT_ILI9xxx.h>

#define HX8357_PANELDRV   0xC0    // W/R (7) Panel Driving Setting
#define HX8357_FRAINCTL   0xC5    // W/R (1) Frame rate and Inversion Control
#define HX8357_GAMMASET   0xC8    // W/R (12) Gamma Setting
#define HX8357_POWERSET   0xD0    // W/R (3) Power Setting
#define HX8357_VCOMCTL    0xD1    // W/R (3) VCOM Control
#define HX8357_POWNORMD   0xD2    // W/R (2) Power Setting for Normal Mode

class TFT_HX8357B : public TFT_ILI9xxx {
public:
  TFT_HX8357B() : TFT_ILI9xxx() {}

protected:
  void init();

};
