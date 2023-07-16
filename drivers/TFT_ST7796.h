/*
  ST7796 driver

  Copyright (c) 2023, rspber (https://github.com/rspber)

*/

#pragma once

#include <TFT_ILI9xxx.h>

#define ST7796_RDDIDINF    0x04    // Read display identification information, returns non standard SPI 3 bytes
#define ST7796_RDNERRDSI   0x05    // Read the Number of the Errors on DSI
#define ST7796_RDDSTAT     0x09    // Read display status, returns non standard SPI 4 bytes
#define ST7796_RDDPWMODE   0x0A    // Read display power mode
#define ST7796_RDMADCTL    0x0B    // Read display MADCTL
#define ST7796_RDPIXFMT    0x0C    // Read display pixel format
#define ST7796_RDIMGFMT    0x0D    // Read display image format
#define ST7796_RDSIGMOD    0x0E    // Read display signal mode 
#define ST7796_RDSELFDIAG  0x0F    // Read display self-diagnostic
#define ST7796_RDFSTCHK    0xAA    // Read First Checksum
#define ST7796_RDCONCHK    0xAF    // Read Continue Checksum
#define ST7796_INTFMDCTL   0xB0    // Interface Mode Control
#define ST7796_FRMCTR1     0xB1    // Frame rate control (In normal mode/full colors)
#define ST7796_FRMCTR2     0xB2    // Frame rate control (In idle mode/8 colors)
#define ST7796_FRMCTR3     0xB3    // Frame rate control (In partial mode/full colors)
#define ST7796_INVCTR      0xB4    // Display inversion control
#define ST7796_BLPORCTL    0xB5    // Blanking Porch Controll
#define ST7796_DFUNCTR     0xB6    // Display function control
#define ST7796_ENTMODSET   0xB7    // Entry Mode Set
#define ST7796_PWCTR1      0xC0    // Power control 1
#define ST7796_PWCTR2      0xC1    // Power control 2
#define ST7796_PWCTR3      0xC2    // Power Control 3
#define ST7796_PWCTR4      0xC3    // Power Control 4
#define ST7796_PWCTR5      0xC4    // Power Control 5
#define ST7796_VCOMCTR1    0xC5    // Set the VCOM(H/L) voltage
#define ST7796_VCOMCTR2    0xC6    // Set the VCOM offset voltage     // ??? it always was at 0xC7
#define ST7796_NVMEMWR     0xD0    // NV Memory write
#define ST7796_NVMPRKEY    0xD1    // NV Memory protection key
#define ST7796_RDNVMST     0xD2    // NV Memory status read
#define ST7796_RDID1       0xDA    // Read ID 1
#define ST7796_RDID2       0xDB    // Read ID 2
#define ST7796_RDID3       0xDC    // Read ID 3
#define ST7796_RDID4       0xD3    // Read ID 4
#define ST7796_PVGAMMA     0xE0    // Positive gamma correction
#define ST7796_NVGAMMA     0xE1    // Negative gamma correction
#define ST7796_DIGGAMC1    0xE2    // Digital gamma control 1
#define ST7796_DIGGAMC2    0xE3    // Digital gamma control 2
#define ST7796_DIGOUTCA    0xE8    // Digital Output CTRL Adjust
#define ST7796_CMDSETCTL   0xF0    // Command Set Control
#define ST7796_RDSPISET    0xFB    // SPI read command setting

class TFT_ST7796 : public TFT_ILI9xxx {
public:
  TFT_ST7796() : TFT_ILI9xxx() {}

protected:
  void init()
  {
    delay(120);
    sendCmd(TFT_SWRESET);
    delay(120);
    sendCmd(TFT_SLPOUT);
    delay(120);
    sendCmdByte(ST7796_CMDSETCTL, 0xC3); // Enable extension command 2 partI
    sendCmdByte(ST7796_CMDSETCTL, 0x96); // Enable extension command 2 partII
    sendCmdByte(TFT_MADCTL, 0x48);
    sendCmdByte(TFT_PIXFMT, TFT_PIXFMT_VALUE);
    sendCmdByte(ST7796_INVCTR, 0x01); // 1-dot inversion
    sendCmdData(ST7796_DFUNCTR, (uint8_t*)"\x80\x02\x3B", 3);
    // 0x80 Bypass
    // 0x02 Source Output Scan from S1 to S960, Gate Output scan from G1 to G480, scan cycle=2
    // 0x3B LCD Drive Line=8*(59+1)
    sendCmdData(ST7796_DIGOUTCA, (uint8_t*)"\x40\x8A\x00\x00\x29\x19\xA5\x33", 8);
    // 0x40
    // 0x8A
    // 0x00
    // 0x00
    // 0x29 Source eqaulizing period time= 22.5 us
    // 0x19 Timing for "Gate start"=25 (Tclk)
    // 0xA5 Timing for "Gate End"=37 (Tclk), Gate driver EQ function ON
    // 0x33
    sendCmdByte(ST7796_PWCTR2, 0x06); // VAP(GVDD)=3.85+( vcom+vcom offset), VAN(GVCL)=-3.85+( vcom+vcom offset)
    sendCmdByte(ST7796_PWCTR3, 0xA7); // Source driving current level=low, Gamma driving current level=High
    sendCmdByte(ST7796_VCOMCTR1, 0x18); // VCOM=0.9
    delay(120);
    sendCmdData(ST7796_PVGAMMA, (uint8_t*)"\xF0\x09\x0b\x06\x04\x15\x2F\x54\x42\x3C\x17\x14\x18\x1B", 14);
    sendCmdData(ST7796_NVGAMMA, (uint8_t*)"\xE0\x09\x0B\x06\x04\x03\x2B\x43\x42\x3B\x16\x14\x17\x1B", 14);
    delay(120);
    sendCmdByte(ST7796_CMDSETCTL, 0x3C); // Disable extension command 2 partI
    sendCmdByte(ST7796_CMDSETCTL, 0x69); // Disable extension command 2 partII
    delay(120);
    sendCmd(TFT_DISPON);
  }

};
