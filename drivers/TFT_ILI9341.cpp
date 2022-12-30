/*
  ILI9341 driver

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#include "ILI9341_Regs.h"
#include "TFT_ILI9341.h"

#include <Setup.h>

#define BGR 1

#define SETUP_SPEED 2 * 1000 * 1000   // 2 MHz

#define beginTransaction(Hz) spi->spiBegin(Hz)
#define endTransaction() spi->spiEnd()
#define sendCmd(cmd) spi->sendCmd(cmd)
#define sendData(data, size) spi->sendData(data, size)
#define sendCmdData(cmd, data, size) spi->sendCmdData(cmd, data, size)
#define sendCmdByte(cmd, data) spi->sendCmdByte(cmd, data)

void TFT_ILI9341::begin(TFT_SPI* aspi, const int16_t aRST)
{
  spi = aspi;
  spi->begin();

  RST = aRST;
  if (RST >= 0) {
    pinMode(RST, OUTPUT);
  }

  hardReset();

  beginTransaction(SETUP_SPEED);

  sendCmd(ILI9341_SWRESET); // Engage software reset
  sleep_ms(150);

#if ILI9341_VERSION < 3  // < v1.2
    sendCmdData(ILI9341_EF, (uint8_t*)"\x03\x80\x02", 3);              // unknown
    sendCmdData(ILI9341_PWCTRB, (uint8_t*)"\x00\xC1\x30", 3);      // retired in v.1.02
    sendCmdData(ILI9341_POSC, (uint8_t*)"\x64\x03\x12\x81", 4);          // retired in v.1.02
    sendCmdData(ILI9341_DTCA, (uint8_t*)"\x85\x00\x78", 3);          // retired in v.1.02
    sendCmdData(ILI9341_PWCTRA, (uint8_t*)"\x39\x2C\x00\x34\x02", 5);      // retired in v.1.02
#endif
  sendCmdByte(ILI9341_PUMPRC,   0x20);

#if ILI9341_VERSION < 3  // < v1.2
    sendCmdData(ILI9341_DTCB, (uint8_t*)"\x00\x00", 2);          // retired in v.1.02
#endif
#if ILI9341_VERSION < 3  // < v1.2
    sendCmdByte(ILI9341_PWCTR1, 0x23);             // Power control VRH[5:0]
#else
    sendCmdByte(ILI9341_MADCTL13, 0x21);           // xCRICxCC
#endif
  sendCmdByte(ILI9341_PWCTR2,   0x10);             // Power control SAP[2:0];BT[3:0]
  sendCmdData(ILI9341_VCOMCTR1, (uint8_t*)"\x31\x3c", 2);       // VCM control 1
  sendCmdByte(ILI9341_VCOMCTR2, 0xC0);             // VCM control 2
#if ILI9341_VERSION < 3  // < v1.2
    sendCmdByte(ILI9341_MADCTL,   0x48);             // Memory Access Control
#endif
  sendCmdByte(ILI9341_VSCRSADD, 0x00);             // Vertical scroll zero
#ifdef COLOR_565
  sendCmdByte(ILI9341_PIXFMT,   0x55);
#else
  sendCmdByte(ILI9341_PIXFMT,   0x66);
#endif
  sendCmdData(ILI9341_FRMCTR1, (uint8_t*)"\x00\x18", 2);
  sendCmdData(ILI9341_DFUNCTR, (uint8_t*)"\x08\x82\x27", 3);  // Display Function Control
  sendCmdByte(ILI9341_ENABLE3G, 0x00);             // 3Gamma Function Disable   // retired in v.1.02
  sendCmdByte(ILI9341_GAMMASET, 0x01);             // Gamma curve selected
#if ILI9341_VERSION < 3  // < v1.2
                                         //"\x0F\x31\x2B\x0C\x0E\x08\x4E\xF1\x37\x07\x10\x03\x0E\x09\x00" // original
    sendCmdData(ILI9341_GMCTRP1, (uint8_t*)"\x0F\x31\x2B\x0C\x0E\x08\x4E\x21\x26\x07\x10\x03\x0E\x09\x00", 15);  // positive gamma correction
                                         //"\x00\x0E\x14\x03\x11\x07\x31\xC1\x48\x08\x0F\x0C\x31\x36\x0F" // original
    sendCmdData(ILI9341_GMCTRN1, (uint8_t*)"\x00\x0E\x14\x03\x11\x07\x31\xC1\x48\x08\x0F\x0C\x31\x36\x0F", 15);  // negative gamma correction
#else
    sendCmdData(ILI9341_GMCTRP1, (uint8_t*)"\x0F\x31\x2B\x0C\x0E\x08\x30\xF1\x37\x07\x10\x03\x0E\x09\x00", 15);  // positive gamma correction v1.3
    sendCmdData(ILI9341_GMCTRN1, (uint8_t*)"\x00\x0E\x14\x03\x11\x07\x11\xC1\x48\x08\x0F\x0C\x31\x36\x0F", 15);  // negative gamma correction
#endif

  sendCmd(ILI9341_SLPOUT);   // Exit Sleep
  delay(150);

  sendCmd(ILI9341_DISPON);   // Display on
  delay(150);

  endTransaction();
}

/**************************************************************************/
/*!
   @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void TFT_ILI9341::setRotation(const int8_t rotation)
{
# if ILI9341_VERSION < 3 // < v1.2
  uint8_t m = 0;
  switch (rotation % 4) { // can't be higher than 3
  case 0:
    m = 0x40 | (BGR << 3);
    setSize(getWIDTH(), getHEIGHT());
    break;
  case 1:
    m = 0x20 | (BGR << 3);
    setSize(getHEIGHT(), getWIDTH());
    break;
  case 2:
    m = 0x80 | (BGR << 3);
    setSize(getWIDTH(), getHEIGHT());
    break;
  case 3:
    m = 0xe0 | (BGR << 3);
    setSize(getHEIGHT(), getWIDTH());
    break;
  }
  beginTransaction(SETUP_SPEED);
    sendCmdByte(ILI9341_MADCTL, m);
  endTransaction();
#else
  uint8_t g = 0; // v1.3
  switch (rotation % 4) { // can't be higher than 3
  case 0:
    g = 0x01 | (BGR << 5);
    setSize(getWIDTH(), getHEIGHT());
    break;
  case 1:
    g = 0x02 | (BGR << 5);
    setSize(getHEIGHT(), getWIDTH());
    break;
  case 2:
    g = 0x08 | (BGR << 5);
    setSize(getWIDTH(), getHEIGHT());
    break;
  case 3:
    g = 0x0b | (BGR << 5);
    setSize(getHEIGHT(), getWIDTH());
    break;
  }
  beginTransaction(SETUP_SPEED);
    sendCmdByte(ILI9341_MADCTL13, g);
  endTransaction();
#endif
}
