/*
  SSD1963_800ALT driver

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <TFT_SSD196x.h>

class TFT_DRIVER : public TFT_SSD196x {
public:
  TFT_DRIVER(const int16_t w, const int16_t h) : TFT_SSD196x(w, h) {}

  void init()
  {
    sendCmdData(0xE2, (uint8_t*)"\x23\x02\x04", 3);     //PLL multiplier, set PLL clock to 120M
    // 0x23 N=0x36 for 6.5M, 0x23 for 10M crystal

    // PLL enable
    sendCmdByte(0xE0, 0x01);
    delay(10);

    sendCmdByte(0xE0, 0x03);
    delay(10);

    sendCmd(0x01);   // software reset

    delay(100);

    //PLL setting for PCLK, depends on resolution
    sendCmdData(0xE6, (uint8_t*)"\x04\x93\xE0", 3);
    sendCmdData(0xB0, (uint8_t*)"\x00\x00\x03\x1F\x01\xDF\x00", 7);     //LCD SPECIFICATION
      // 0x24
      // 0x00
      // 0x03 Set HDP 799
      // 0x1F
      // 0x01 Set VDP 479
      // 0xDF
      // 0x00

    sendCmdData(0xB4, (uint8_t*)"\x03\xA0\x00\x2E\x30\x00\x0F\x00", 8);     //HSYNC
      // 0x03 Set HT  928
      // 0xA0
      // 0x00 Set HPS 46
      // 0x2E
      // 0x30 Set HPW 48
      // 0x00 Set LPS 15
      // 0x0F
      // 0x00

    sendCmdData(0xB6, (uint8_t*)"\x02\x0D\x00\x10\x10\x00\x08", 7);     //VSYNC
      // 0x02 Set VT  525
      // 0x0D
      // 0x00 Set VPS 16
      // 0x10
      // 0x10 Set VPW 16
      // 0x00 Set FPS 8
      // 0x08

    sendCmdByte(0xBA, 0x05);
      // 0x05 GPIO[3:0] out 1

    sendCmdData(0xB8, (uint8_t*)"\x07\x01", 2);
      // 0x07 GPIO3=input, GPIO[2:0]=output
      // 0x01 GPIO0 normal

    sendCmdByte(0x36, 0x21 | (BGR << 3));

    sendCmdByte(0xF0, 0x00);        //pixel data interface
      // 0x00 8 bit bus

    delay(10);

    sendCmd(0x29);   //display on

    //set PWM for B/L
    sendCmdData(0xBE, (uint8_t*)"\x06\xF0\x01\xF0\x00\x00", 6);
    sendCmdByte(0xD0, 0x0D);
  }

};
