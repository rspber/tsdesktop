/*
  ILI9341 driver

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <TFT_ILI9xxx.h>

#define ILI9341_RDDIDINF    0x04    // Read display identification information, returns non standard SPI 3 bytes
#define ILI9341_RDDSTAT     0x09    // Read display status, returns non standard SPI 4 bytes
#define ILI9341_RDDPWMODE   0x0A    // Read display power mode
#define ILI9341_RDMADCTL    0x0B    // Read display MADCTL
#define ILI9341_RDPIXFMT    0x0C    // Read display pixel format
#define ILI9341_RDIMGFMT    0x0D    // Read display image format
#define ILI9341_RDSELFDIAG  0x0F    // Read display self-diagnostic
#define ILI9341_GAMMASET    0x26    // Gamma set
#define ILI9341_FRMCTR1     0xB1    // Frame rate control (In normal mode/full colors)
#define ILI9341_FRMCTR2     0xB2    // Frame rate control (In idle mode/8 colors)
#define ILI9341_FRMCTR3     0xB3    // Frame rate control (In partial mode/full colors)
#define ILI9341_INVCTR      0xB4    // Display inversion control
#define ILI9341_BLPORCTL    0xB5    // Blanking Porch Controll : 0..255, from v.1.02  : 0..127
#define ILI9341_DFUNCTR     0xB6    // Display function control
#define ILI9341_ENTMODSET   0xB7    // Entry Mode Set
#define ILI9341_PWCTR1      0xC0    // Power control 1
#define ILI9341_PWCTR2      0xC1    // Power control 2
#define ILI9341_PWCTR3      0xC2    // Power Control 3
#define ILI9341_PWCTR4      0xC3    // Power Control 4
#define ILI9341_PWCTR5      0xC4    // Power Control 5
#define ILI9341_VCOMCTR1    0xC5    // Set the VCOM(H/L) voltage
#define ILI9341_VCOMCTR2    0xC7    // Set the VCOM offset voltage
#define ILI9341_RDID1       0xDA    // Read ID 1
#define ILI9341_RDID2       0xDB    // Read ID 2
#define ILI9341_RDID3       0xDC    // Read ID 3
#define ILI9341_RDID4       0xD3    // Read ID 4
#define ILI9341_RD_0xDD     0xDD    // undocumented
#define ILI9341_RD_0xDE     0xDE    // undocumented
#define ILI9341_RD_0xDF     0xDF    // undocumented
#define ILI9341_PVGAMMA     0xE0    // Positive gamma correction
#define ILI9341_NVGAMMA     0xE1    // Negative gamma correction
#define ILI9341_EF          0xEF    // undocumented
#define ILI9341_INTFCCTR    0xF6    // Interface control
#define ILI9341_PUMPRC      0xF7    // Pump ratio control
#define ILI9341_PWCTR6      0xFC

// retired in version v.1.02
#define ILI9341_PWCTRA      0xCB    // Power control A
#define ILI9341_PWCTRB      0xCF    // Power control B
#define ILI9341_DTCA        0xE8    // Driver timing control A
// E9
#define ILI9341_DTCB        0xEA    // Driver timing control B
#define ILI9341_POSC        0xED    // Power on sequence control
#define ILI9341_ENABLE3G    0xF2    // Enable 3 gamma control

class TFT_ILI9341 : public TFT_ILI9xxx {
public:
  TFT_ILI9341() : TFT_ILI9xxx() {}

protected:
  void init()
  {
    sendCmd(TFT_SWRESET); // Engage software reset
    delay(150);
    sendCmdData(ILI9341_EF, (uint8_t*)"\x03\x80\x02", 3);                // unknown
    sendCmdData(ILI9341_PWCTRB, (uint8_t*)"\x00\xC1\x30", 3);            // retired in v.1.02
    sendCmdData(ILI9341_POSC, (uint8_t*)"\x64\x03\x12\x81", 4);          // retired in v.1.02
    sendCmdData(ILI9341_DTCA, (uint8_t*)"\x85\x00\x78", 3);              // retired in v.1.02
    sendCmdData(ILI9341_PWCTRA, (uint8_t*)"\x39\x2C\x00\x34\x02", 5);    // retired in v.1.02
    sendCmdByte(ILI9341_PUMPRC,   0x20);
    sendCmdData(ILI9341_DTCB, (uint8_t*)"\x00\x00", 2);                  // retired in v.1.02
    sendCmdByte(ILI9341_PWCTR1, 0x23);
    sendCmdByte(ILI9341_PWCTR2,   0x10);
    sendCmdData(ILI9341_VCOMCTR1, (uint8_t*)"\x31\x3c", 2);
    sendCmdByte(ILI9341_VCOMCTR2, 0xC0);
    sendCmdByte(TFT_MADCTL,   0x48);
    sendCmdByte(TFT_VSCRSADD, 0x00);
    sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
    sendCmdData(ILI9341_FRMCTR1, (uint8_t*)"\x00\x18", 2);   // 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz
    sendCmdData(ILI9341_DFUNCTR, (uint8_t*)"\x08\x82\x27", 3);
    sendCmdByte(ILI9341_ENABLE3G, 0x00);                                 // retired in v.1.02
    sendCmdByte(ILI9341_GAMMASET, 0x01);
                                         //"\x0F\x31\x2B\x0C\x0E\x08\x4E\xF1\x37\x07\x10\x03\x0E\x09\x00" // original
    sendCmdData(ILI9341_PVGAMMA, (uint8_t*)"\x0F\x31\x2B\x0C\x0E\x08\x4E\x21\x26\x07\x10\x03\x0E\x09\x00", 15);
                                         //"\x00\x0E\x14\x03\x11\x07\x31\xC1\x48\x08\x0F\x0C\x31\x36\x0F" // original
    sendCmdData(ILI9341_NVGAMMA, (uint8_t*)"\x00\x0E\x14\x03\x11\x07\x31\xC1\x48\x08\x0F\x0C\x31\x36\x0F", 15);
    sendCmd(TFT_SLPOUT);
    delay(150);
    sendCmd(TFT_DISPON);
    delay(150);
  }

};
