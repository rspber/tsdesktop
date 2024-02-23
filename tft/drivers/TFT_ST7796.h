/*
  ST7796 defines
*/

#pragma once

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
