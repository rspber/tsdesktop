/*
  SSD1963_800BD driver

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <TFT_SSD196x.h>

class TFT_DRIVER : public TFT_SSD196x {
public:
  TFT_DRIVER(const int16_t w, const int16_t h) : TFT_SSD196x(w, h) {}

  void init()
  {
    sendCmdData(0xE2, (uint8_t*)"\x23\x02\x54", 3);     //PLL multiplier, set PLL clock to 120M
      // 0x23 N=0x36 for 6.5M, 0x23 for 10M crystal

    // PLL enable
    sendCmdByte(0xE0, 0x01);
    delay(10);

    sendCmdByte(0xE0, 0x03);
    delay(10);

    sendCmd(0x01);   // software reset

    delay(100);

    //PLL setting for PCLK, depends on resolution
    sendCmdData(0xE6, (uint8_t*)"\x03\x33\x33", 3);

    {
      uint8_t dd[] {0x20, 0x00, 799 >> 8, 799 & 0xFF, 479 >> 8, 479 & 0xFF, 0x00};
      sendCmdData(0xB0, dd, 7);     //LCD SPECIFICATION
      // 0x20
      // 0x00
      // (799 >> 8) Set HDP 799
      // (799 & 0xFF)
      // (479 >> 8) Set VDP 479
      // (479 & 0xFF)
      // 0x00
    }

    sendCmdData(0xB4, (uint8_t*)"\x04\x1F\x00\xD2\x00\x00\x00\x00", 8);     //HSYNC
      // 0x04 Set HT
      // 0x1F
      // 0x00 Set HPS
      // 0xD2
      // 0x00 Set HPW
      // 0x00 Set LPS
      // 0x00

    sendCmdData(0xB6, (uint8_t*)"\x02\x0C\x00\x22\x00\x00\x00", 7);     //VSYNC
      // 0x02 Set VT
      // 0x0C
      // 0x00 Set VPS
      // 0x22
      // 0x00 Set VPW
      // 0x00 Set FPS
      // 0x00

    sendCmdData(0xB8, (uint8_t*)"\x0F\x01", 2);
      // 0x0F GPIO3=input, GPIO[2:0]=output
      // 0x01 GPIO0 normal

    sendCmdByte(0xBA, 0x01);
      // 0x01 GPIO[3:0] out 1 --- LCD display on/off control PIN

    sendCmdByte(0x36, 0x21 | (BGR << 3));

    sendCmdByte(0xF0, 0x00);        //pixel data interface
      // 0x00     //000 = 8 bit bus, 011 = 16 bit, 110 = 9 bit

    sendCmdData(0xBC, (uint8_t*)"\x40\x80\x40\x01", 4);
      // 0x40 contrast value
      // 0x80 brightness value
      // 0x40 saturation value
      // 0x01 Post Processor Enable

    delay(10);

    sendCmd(0x29);   //display on

    //set PWM for B/L
    sendCmdData(0xBE, (uint8_t*)"\x06\x80\x01\xF0\x00\x00", 6);
    sendCmdByte(0xD0, 0x0D);
  }

};
