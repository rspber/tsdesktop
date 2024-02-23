/*
  ILI9486 defines
*/

#pragma once

#define ILI9486_RDDIDINF    0x04    // Read display identification information, returns non standard SPI 3 bytes
#define ILI9486_RDNERRDSI   0x05    // Read the Number of the Errors on DSI
#define ILI9486_RDDSTAT     0x09    // Read display status, returns non standard SPI 4 bytes
#define ILI9486_RDDPWMODE   0x0A    // Read display power mode
#define ILI9486_RDMADCTL    0x0B    // Read display MADCTL
#define ILI9486_RDPIXFMT    0x0C    // Read display pixel format
#define ILI9486_RDIMGFMT    0x0D    // Read display image format
#define ILI9486_RDSIGMOD    0x0E    // Read display signal mode
#define ILI9486_RDSELFDIAG  0x0F    // Read display self-diagnostic
#define ILI9486_RDFSTCHK    0xAA    // Read First Checksum
#define ILI9486_RDCONCHK    0xAF    // Read Continue Checksum
#define ILI9486_INTFMDCTL   0xB0    // Interface Mode Control
#define ILI9486_FRMCTR1     0xB1    // Frame rate control (In normal mode/full colors)
#define ILI9486_FRMCTR2     0xB2    // Frame rate control (In idle mode/8 colors)
#define ILI9486_FRMCTR3     0xB3    // Frame rate control (In partial mode/full colors)
#define ILI9486_INVCTR      0xB4    // Display inversion control
#define ILI9486_BLPORCTL    0xB5    // Blanking Porch Controll
#define ILI9486_DFUNCTR     0xB6    // Display function control
#define ILI9486_ENTMODSET   0xB7    // Entry Mode Set
#define ILI9486_PWCTR1      0xC0    // Power control 1
#define ILI9486_PWCTR2      0xC1    // Power control 2
#define ILI9486_PWCTR3      0xC2    // Power Control 3
#define ILI9486_PWCTR4      0xC3    // Power Control 4
#define ILI9486_PWCTR5      0xC4    // Power Control 5
#define ILI9486_VCOMCTR1    0xC5    // Set the VCOM(H/L) voltage
#define ILI9486_CABCCTL1    0xC6    // CABC Control 1
#define ILI9486_CABCCTL2    0xC8    // CABC Control 2
#define ILI9486_CABCCTL3    0xC9    // CABC Control 3
#define ILI9486_CABCCTL4    0xCA    // CABC Control 4
#define ILI9486_CABCCTL5    0xCB    // CABC Control 5
#define ILI9486_CABCCTL6    0xCC    // CABC Control 6
#define ILI9486_CABCCTL7    0xCD    // CABC Control 7
#define ILI9486_CABCCTL8    0xCE    // CABC Control 8
#define ILI9486_CABCCTL9    0xCF    // CABC Control 9
#define ILI9486_NVMEMWR     0xD0    // NV Memory write
#define ILI9486_NVMPRKEY    0xD1    // NV Memory protection key
#define ILI9486_RDNVMST     0xD2    // NV Memory status read
#define ILI9486_RDID1       0xDA    // Read ID 1
#define ILI9486_RDID2       0xDB    // Read ID 2
#define ILI9486_RDID3       0xDC    // Read ID 3
#define ILI9486_RDID4       0xD3    // Read ID 4
#define ILI9486_RD_0xDD     0xDD    // undocumented
#define ILI9486_RD_0xDE     0xDE    // undocumented
#define ILI9486_RD_0xDF     0xDF    // undocumented
#define ILI9486_PVGAMMA     0xE0    // Positive gamma correction
#define ILI9486_NVGAMMA     0xE1    // Negative gamma correction
#define ILI9486_DIGGAMC1    0xE2    // Digital gamma control 1
#define ILI9486_DIGGAMC2    0xE3    // Digital gamma control 2
#define ILI9486_RDSPISET    0xFB    // SPI read command setting
