/*
  SSD1963_480 driver 

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <TFT_SSD196x.h>

class TFT_DRIVER : public TFT_SSD196x {
public:
  TFT_DRIVER(const int16_t w, const int16_t h) : TFT_SSD196x(w, h) {}

  void init()
  {
    sendCmdData(0xE2, (uint8_t*)"\x23\x02\x54", 3);
    sendCmdByte(0xE0, 0x01);   //PLL multiplier, set PLL clock to 120M
        // 1: 0x23 N=0x36 for 6.5M, 0x23 for 10M crystal

    delay(10);
    sendCmdByte(0xE0, 0x03);
    delay(10);

    sendCmd(0x01);   // software reset

    delay(100);

    //PLL setting for PCLK, depends on resolution
    sendCmdData(0xE6, (uint8_t*)"\x01\x1F\xFF", 3);
    sendCmdData(0xB0, (uint8_t*)"\x20\x00\x01\xDF\x01\x0F\x00", 7);   //LCD SPECIFICATION
        // 3: 0x01 Set HDP 479
        // 5: 0x01 Set VDP 271

    sendCmdData(0xB4, (uint8_t*)"\x02\x13\x00\x08\x2B\x00\x02\x00",  8);     //HSYNC
        // 1: 0x02 Set HT  531
        // 3: 0x00 Set HPS 8
        // 5: 0x2B Set HPW 43
        // 6: 0x00 Set LPS 2

    sendCmdData(0xB6, (uint8_t*)"\x01\x20\x00\x04\x0c\x00\x02", 7);     //VSYNC
        // 1: 0x01 Set VT  288
        // 3: 0x00 Set VPS 4
        // 5: 0x0c Set VPW 12
        // 6: 0x00 Set FPS 2

    sendCmdByte(0xBA, 0x0F);      //GPIO[3:0] out 1

    sendCmdData(0xB8, (uint8_t*)"\x07\x01", 2);
        // 1: 0x07 GPIO3=input, GPIO[2:0]=output
        // 2: 0x01 GPIO0 normal

    sendCmdByte(0x36, 0x21 | (BGR << 3));

    sendCmdByte(0xF0, 0x00);     //pixel data interface
        // 1: 0x00 8 bit bus

    delay(1);

    sendCmdData(0xB8, (uint8_t*)"\x0f\x01", 2);
        // 1: 0x0f GPIO is controlled by host GPIO[3:0]=output   GPIO[0]=1  LCD ON  GPIO[0]=1  LCD OFF
        // 2: 0x01 GPIO0 normal

    sendCmdByte(0xBA, 0x01);      //GPIO[0] out 1 --- LCD display on/off control PIN

    sendCmd(0x29);   //display on

    //set PWM for B/L
    sendCmdData(0xBE, (uint8_t*)"\x06\xF0\x01\xF0\x00\x00", 6);
    sendCmdByte(0xD0, 0x0D);  
  }

};
