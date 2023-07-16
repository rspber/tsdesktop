/*
  ILI9488 driver

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <TFT_ILI9xxx.h>

#define ILI9488_RDDIDINF    0x04    // Read display identification information, returns non standard SPI 3 bytes
#define ILI9488_RDNERRDSI   0x05    // Read the Number of the Errors on DSI
#define ILI9488_RDDSTAT     0x09    // Read display status, returns non standard SPI 4 bytes
#define ILI9488_RDDPWMODE   0x0A    // Read display power mode
#define ILI9488_RDMADCTL    0x0B    // Read display MADCTL
#define ILI9488_RDPIXFMT    0x0C    // Read display pixel format
#define ILI9488_RDIMGFMT    0x0D    // Read display image format
#define ILI9488_RDSIGMOD    0x0E    // Read display signal mode 
#define ILI9488_RDSELFDIAG  0x0F    // Read display self-diagnostic
#define ILI9488_ALLPIXOFF   0x22    // All pixel off
#define ILI9488_ALLPIXON    0x23    // All pixel off
#define ILI9488_RDAUTBRNS   0x68    // Read automatic brightness
#define ILI9488_INTFMDCTL   0xB0    // Interface Mode Control
#define ILI9488_FRMCTR1     0xB1    // Frame rate control (In normal mode/full colors)
#define ILI9488_FRMCTR2     0xB2    // Frame rate control (In idle mode/8 colors)
#define ILI9488_FRMCTR3     0xB3    // Frame rate control (In partial mode/full colors)
#define ILI9488_INVCTR      0xB4    // Display inversion control
#define ILI9488_BLPORCTL    0xB5    // Blanking Porch Controll
#define ILI9488_DFUNCTR     0xB6    // Display function control
#define ILI9488_ENTMODSET   0xB7    // Entry Mode Set
#define ILI9488_COLENHC1    0xB9    // Color Enhancement control 1
#define ILI9488_COLENHC2    0xBA    // Color Enhancement control 2
#define ILI9488_HSLANCTL    0xBE    // HS Lanes control
#define ILI9488_PWCTR1      0xC0    // Power control 1
#define ILI9488_PWCTR2      0xC1    // Power control 2
#define ILI9488_PWCTR3      0xC2    // Power Control 3
#define ILI9488_PWCTR4      0xC3    // Power Control 4
#define ILI9488_PWCTR5      0xC4    // Power Control 5
#define ILI9488_VCOMCTR1    0xC5    // Set the VCOM(H/L) voltage
#define ILI9488_CABCCTL1    0xC6    // CABC Control 1
#define ILI9488_CABCCTL2    0xC8    // CABC Control 2
#define ILI9488_CABCCTL3    0xC9    // CABC Control 3
#define ILI9488_CABCCTL4    0xCA    // CABC Control 4
#define ILI9488_CABCCTL5    0xCB    // CABC Control 5
#define ILI9488_CABCCTL6    0xCC    // CABC Control 6
#define ILI9488_CABCCTL7    0xCD    // CABC Control 7
#define ILI9488_CABCCTL8    0xCE    // CABC Control 8
#define ILI9488_CABCCTL9    0xCF    // CABC Control 9
#define ILI9488_NVMEMWR     0xD0    // NV Memory write
#define ILI9488_NVMPRKEY    0xD1    // NV Memory protection key
#define ILI9488_RDNVMST     0xD2    // NV Memory status read
#define ILI9488_ADJCTL1     0xD7    // Adjust Control 1
#define ILI9488_RDID4CHK    0xD8    // Read ID4 Check
#define ILI9488_RDID1       0xDA    // Read ID 1
#define ILI9488_RDID2       0xDB    // Read ID 2
#define ILI9488_RDID3       0xDC    // Read ID 3
#define ILI9488_RDID4       0xD3    // Read ID 4
#define ILI9488_RD_0xDD     0xDD    // undocumented
#define ILI9488_RD_0xDE     0xDE    // undocumented
#define ILI9488_RD_0xDF     0xDF    // undocumented
#define ILI9488_PVGAMMA     0xE0    // Positive gamma correction
#define ILI9488_NVGAMMA     0xE1    // Negative gamma correction
#define ILI9488_DIGGAMC1    0xE2    // Digital gamma control 1
#define ILI9488_DIGGAMC2    0xE3    // Digital gamma control 2
#define ILI9488_SETIMAGE    0xE9    // Set image function
#define ILI9488_ADJCTL2     0xF2    // Adjust Control 2
#define ILI9488_ADJCTL3     0xF7    // Adjust Control 3
#define ILI9488_ADJCTL4     0xF8    // Adjust Control 4
#define ILI9488_ADJCTL5     0xF9    // Adjust Control 5
#define ILI9488_ADJCTL6     0xFC    // Adjust Control 6
#define ILI9488_ADJCTL7     0xFF    // Adjust Control 7

class TFT_ILI9488 : public TFT_ILI9xxx {
public:
  TFT_ILI9488() : TFT_ILI9xxx() {}

protected:
  void init()
  {
    sendCmdData(ILI9488_PVGAMMA, (uint8_t*)"\x00\x03\x09\x08\x16\x0A\x3F\x78\x4C\x09\x0A\x08\x16\x1A\x0F", 15);
    sendCmdData(ILI9488_NVGAMMA, (uint8_t*)"\x00\x16\x19\x03\x0F\x05\x32\x45\x46\x04\x0E\x0D\x35\x37\x0F", 15);
    sendCmdData(ILI9488_PWCTR1, (uint8_t*)"\x17\x15", 2);
      // 0x17 Vreg1out
      // 0x15 Verg2out
    sendCmdByte(ILI9488_PWCTR2, 0x41);  // VGH,VGL
    sendCmdData(ILI9488_VCOMCTR1, (uint8_t*)"\x00\x12\x80", 3);
      // 0x00
      // 0x12 Vcom
      // 0x80
    sendCmdByte(TFT_MADCTL, 0x48);

    // NOTE: For 16bit parallel transfer, IM jumpers need to be set
    // as 010.
    sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
    sendCmdByte(ILI9488_INTFMDCTL, 0x00);
    sendCmdByte(ILI9488_FRMCTR1, 0xA0);     // 60Hz
    sendCmdByte(ILI9488_INVCTR, 0x02);     // 2-dot
    sendCmdData(ILI9488_DFUNCTR, (uint8_t*)"\x02\x02\x3B", 3);
      // 0x02 MCU
      // 0x02 Source,Gate scan direction
      // 0x3B
    sendCmdByte(ILI9488_ENTMODSET, 0xC6);
    sendCmdData(ILI9488_ADJCTL3, (uint8_t*)"\xA9\x51\x2C\x82", 4);
      // 0x82 D7 stream, loose
    sendCmd(TFT_SLPOUT);
    delay(120);
    sendCmd(TFT_DISPON);
    delay(25);
  }

};
