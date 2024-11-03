/*
  Setup for TSDesktop
*/

#pragma once

// --------------------------- Compatibilities -------------------------------

#define PinMode uint8_t

// ---------------- Constans to use in REV and ROTATION ----------------------

#define TFT_MAD_MY  0x80  // 00 top to botom, 80 bottom to top
#define TFT_MAD_MX  0x40  // 00 left to right, 40 right to left
#define TFT_MAD_YX  0x20  // it means that X and Y are exchanged, wrongly called MV
//#define TFT_MAD_MV  0x10  // vertical refresh direction, wrongly called ML
//#define TFT_MAD_RGB 0x00
//#define TFT_MAD_BGR 0x08
//#define TFT_MAD_MH  0x04 // horizontal refresh direction,
#define TFT_MAD_SS  0x02 // horizontal flip
#define TFT_MAD_GS  0x01 // vertical flip

// ------------------- Constants to use in examples --------------------------

#define ROTATION_VTB 2    // vertical top to bottom
#define ROTATION_VBT 0    // vertical bottom to top
#define ROTATION_HLR 1    // horizontal left to right
#define ROTATION_HRL 3    // horizontal right to left

#define DEFAULT_LED_PIN   2

// --------------------------- User Setups -----------------------------------

#define USE_DMA

//  #include <Setup_ESP32_GPIO_8BITP_ILI9488.h>
//  #include <Setup_ESP32_GPIO_8BITP_ILI9488_1.h>
//  #include <Setup_ESP32_GPIO_8BITP_RM68140.h>

  #include <Setup_ESP32_SPI_ILI9341.h>
//  #include <Setup_ESP32_SPI_ST7789.h>
//  #include <Setup_ESP32_SPI_ST7796.h>
