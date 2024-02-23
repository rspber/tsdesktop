/*
  ILI9481 defines
*/

#pragma once

#define ILI9481_RDPOWERMD  0x0A    //  R  (1) Get power mode
#define ILI9481_RDADDRMD   0x0B    //  R  (1) Get address mode
#define ILI9481_RDPIXFMT   0x0C    //  R  (1) Get pixel format
#define ILI9481_RDDISPMD   0x0D    //  R  (1) Get display mode
#define ILI9481_RDSIGMD    0x0E    //  R  (1) Get signal mode
#define ILI9481_RDDIAG     0x0F    //  R  (1) Get self-diagnostic result
#define ILI9481_DDBRDST    0xA1    // W/  (1) Read DDB start
#define ILI9481_DDBRDCNT   0xA8    // W/  (1) Read DDB continue
#define ILI9481_CMDACCPRO  0xB0    // W/R (1) Command Access Protect
#define ILI9481_LPOWMDCTL  0xB1    // W/R (1) Low Power Mode Control
#define ILI9481_FMEMACIN   0xB3    // W/R (5) Frame Memory Access and Interface setting
#define ILI9481_DMFMWSET   0xB4    // W/R (1) Display Mode and Frame Memory Write Mode setting
#define ILI9481_DEVCDRD    0xBF    //  R  (4) Device code Read
#define ILI9481_PANELDRV   0xC0    // W/R (7) Panel Driving Setting
#define ILI9481_DTSNORMD   0xC1    // W/R (3) Display Timing Setting for Normal Mode
#define ILI9481_DTSPARMD   0xC2    // W/R (3) Display Timing Setting for Partial Mode
#define ILI9481_DTSIDLMD   0xC3    // W/R (3) Display Timing Setting for Idle Mode
#define ILI9481_FRAINCTL   0xC5    // W/R (1) Frame rate and Inversion Control
#define ILI9481_INTRFCTL   0xC6    // W/R (1) Interface Control
#define ILI9481_GAMMASET   0xC8    // W/R (12) Gamma Setting
#define ILI9481_POWERSET   0xD0    // W/R (3) Power Setting
#define ILI9481_VCOMCTL    0xD1    // W/R (3) VCOM Control
#define ILI9481_POWNORMD   0xD2    // W/R (2) Power Setting for Normal Mode
#define ILI9481_POWPARMD   0xD3    // W/R (2) Power Setting for Partial Mode
#define ILI9481_POWIDLMD   0xD4    // W/R (2) Power Setting for Idle Mode
#define ILI9481_NVMEMWR    0xE0    // W/R (1) NV Memory Write
#define ILI9481_NVMEMCTL   0xE1    // W/R (1) NV Memory Control
#define ILI9481_NVMEMST    0xE2    // W/R (3) NV Memory Status
#define ILI9481_NVMEMPRO   0xE3    // W/R (2) NV Memory Protection
//      others          0xB0..0xFF // W/R (Variable) LSI Test Registers
