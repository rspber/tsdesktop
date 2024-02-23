/*
  ILI9341 Diagnostics

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include <Display.h>
#include <drivers/TFT_ILI9341.h>

#define ROTATION ROTATION_HLR

Display display;
bool screenEnabled = false;

uint8_t LED_PIN = DEFAULT_LED_PIN;
int pin13state = LOW;

int y = 0;
int dy = 10;

char patt16[] = "%02x : %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x";

void diag_reg(const uint8_t reg, const int8_t len)
{
  uint8_t b[20];
  display.readRegister(b, reg, len);

  patt16[6 + len * 5] = '\0';
  char tx[80];
  snprintf(tx, 80, patt16, b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8], b[9], b[10], b[11], b[12], b[13], b[14], b[15], b[16]);
  patt16[6 + len * 5] = ' ';
  display.drawText(0, y, tx);
  y += dy;
}

void setup()   {

  display.begin();
  display.setRotation(ROTATION, TFT_REV);
  display.clearDisplay();

  pinMode(LED_PIN, OUTPUT);

//  screenEnabled = true;   // it's for tsdesktop only
}

// ----------------------------------------------------------------

void loop()
{
//  display.clearDisplay();
  y = 0;

  diag_reg(0, 2);     // read device id ???
  diag_reg(ILI9341_RDDIDINF, 4);     // 0x04
  diag_reg(ILI9341_RDDSTAT, 5);      // 0x09
  diag_reg(ILI9341_RDDPWMODE, 1);    // 0x0a
  diag_reg(ILI9341_RDMADCTL, 1);     // 0x0b
  diag_reg(ILI9341_RDPIXFMT, 1);     // 0x0c
  diag_reg(ILI9341_RDIMGFMT, 1);     // 0x0d
  diag_reg(ILI9341_RDSELFDIAG, 1);   // 0x0f
//  diag_reg(ILI9341_DFUNCTR, 6);      // 0xb6
  diag_reg(0xD2, 4);                 // NVM Status
  diag_reg(ILI9341_RDID4, 4);        // 0xd3
  diag_reg(ILI9341_RDID1, 1);        // 0xda
  diag_reg(ILI9341_RDID2, 1);        // 0xdb
  diag_reg(ILI9341_RDID3, 1);        // 0xdc
  diag_reg(ILI9341_RD_0xDD, 1);      // 0xdd
  diag_reg(ILI9341_RD_0xDE, 1);      // 0xde
  diag_reg(ILI9341_RD_0xDF, 1);      // 0xdf
//  diag_reg(0xD0, 4);
//  diag_reg(0xD1, 16);
//  diag_reg(0xD4, 16);
//  diag_reg(0xD5, 16);
//  diag_reg(0xD6, 16);
//  diag_reg(0xD7, 16);
//  diag_reg(0xD8, 16);
  diag_reg(ILI9341_INTFCCTR, 4);     // 0xf6

// if after reading register there is a problem (dislocation) with print it, it means, that not all register values were read

  delay(50);

  pin13state = 1 - pin13state;
  digitalWrite(LED_PIN, pin13state);
}

// ----------------------------------------------------------------
