/*
  ST7789 defines
*/

#pragma once

#define ST7789_RDDID      0x04
#define ST7789_RDDST      0x09
#define ST7789_RDDPM      0x0A      // Read display power mode
#define ST7789_RDD_MADCTL 0x0B      // Read display MADCTL
#define ST7789_RDD_COLMOD 0x0C      // Read display pixel format
#define ST7789_RDDIM      0x0D      // Read display image mode
#define ST7789_RDDSM      0x0E      // Read display signal mode
#define ST7789_RDDSR      0x0F      // Read display self-diagnostic result (ST7789V)
#define ST7789_RAMCTRL    0xB0      // RAM control
#define ST7789_RGBCTRL    0xB1      // RGB control
#define ST7789_PORCTRL    0xB2      // Porch control
#define ST7789_FRCTRL1    0xB3      // Frame rate control
#define ST7789_PARCTRL    0xB5      // Partial mode control
#define ST7789_GCTRL      0xB7      // Gate control
#define ST7789_GTADJ      0xB8      // Gate on timing adjustment
#define ST7789_DGMEN      0xBA      // Digital gamma enable
#define ST7789_VCOMS      0xBB      // VCOMS setting
#define ST7789_LCMCTRL    0xC0      // LCM control
#define ST7789_IDSET      0xC1      // ID setting
#define ST7789_VDVVRHEN   0xC2      // VDV and VRH command enable
#define ST7789_VRHS       0xC3      // VRH set
#define ST7789_VDVSET     0xC4      // VDV setting
#define ST7789_VCMOFSET   0xC5      // VCOMS offset set
#define ST7789_FRCTR2     0xC6      // FR Control 2
#define ST7789_CABCCTRL   0xC7      // CABC control
#define ST7789_REGSEL1    0xC8      // Register value section 1
#define ST7789_REGSEL2    0xCA      // Register value section 2
#define ST7789_PWMFRSEL   0xCC      // PWM frequency selection
#define ST7789_PWCTRL1    0xD0      // Power control 1
#define ST7789_VAPVANEN   0xD2      // Enable VAP/VAN signal output
#define ST7789_CMD2EN     0xDF      // Command 2 enable
#define ST7789_PVGAMCTRL  0xE0      // Positive voltage gamma control
#define ST7789_NVGAMCTRL  0xE1      // Negative voltage gamma control
#define ST7789_DGMLUTR    0xE2      // Digital gamma look-up table for red
#define ST7789_DGMLUTB    0xE3      // Digital gamma look-up table for blue
#define ST7789_GATECTRL   0xE4      // Gate control
#define ST7789_SPI2EN     0xE7      // SPI2 enable
#define ST7789_PWCTRL2    0xE8      // Power control 2
#define ST7789_EQCTRL     0xE9      // Equalize time control
#define ST7789_PROMCTRL   0xEC      // Program control
#define ST7789_PROMEN     0xFA      // Program mode enable
#define ST7789_NVMSET     0xFC      // NVM setting
#define ST7789_PROMACT    0xFE      // Program action
