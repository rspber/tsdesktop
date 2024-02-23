/*
  ILI9341 defines
*/

#pragma once

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
