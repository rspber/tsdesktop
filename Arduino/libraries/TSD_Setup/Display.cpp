/*
  Graphics display adapter for TSDesktop
       implemented on Adafruit_ILI9341

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

/*
  added support for 2.4" TFT SPI 240X320 v1.3
*/

#include "Display.h"
#include "Setup.h"

#define BGR 1

//    # Command constants from ILI9341 datasheet
//#define ILI9341_NOP         0x00    // No-op register
#define ILI9341_SWRESET     0x01    // Software reset register
//#define ILI9341_RDDIDINF    0x04    // Read display identification information, returns non standard SPI 3 bytes
//#define ILI9341_RDDSTAT     0x09    // Read display status, returns non standard SPI 4 bytes
//#define ILI9341_RDDPWMODE   0x0A    // Read display power mode
//#define ILI9341_RDMADCTL    0x0B    // Read display MADCTL
//#define ILI9341_RDPIXFMT    0x0C    // Read display pixel format
//#define ILI9341_RDIMGFMT    0x0D    // Read display image format
//#define ILI9341_RDSELFDIAG  0x0F    // Read display self-diagnostic
//#define ILI9341_SLPIN       0x10    // Enter sleep mode
#define ILI9341_SLPOUT      0x11    // Exit sleep mode
//#define ILI9341_PTLON       0x12    // Partial mode on
//#define ILI9341_NORON       0x13    // Normal display mode on
#define ILI9341_INVOFF      0x20    // Display inversion off
#define ILI9341_INVON       0x21    // Display inversion on
#define ILI9341_GAMMASET    0x26    // Gamma set
#define ILI9341_DISPOFF     0x28    // Display off
#define ILI9341_DISPON      0x29    // Display on
#define ILI9341_CASET       0x2A    // Column address set
#define ILI9341_PASET       0x2B    // Page address set
#define ILI9341_RAMWR       0x2C    // Memory write
//#define ILI9341_RAMRD       0x2E    // Memory read
//#define ILI9341_PTLAR       0x30    // Partial area
#define ILI9341_VSCRDEF     0x33    // Vertical scrolling definition
#define ILI9341_MADCTL      0x36    // Memory access control
#define ILI9341_VSCRSADD    0x37    // Vertical scrolling start address
#define ILI9341_PIXFMT      0x3A    // COLMOD: Pixel format set
//#define ILI9341_WRITE_DISPLAY_BRIGHTNESS 0x51    // Brightness hardware dependent!
//#define ILI9341_READ_DISPLAY_BRIGHTNESS  0x52
//#define ILI9341_WRITE_CTRL_DISPLAY       0x53
//#define ILI9341_READ_CTRL_DISPLAY        0x54
//#define ILI9341_WRITE_CABC  0x55    // Write Content Adaptive Brightness Control
//#define ILI9341_READ_CABC   0x56    // Read Content Adaptive Brightness Control
//#define ILI9341_WRITE_CABC_MINIMUM       0x5E    // Write CABC Minimum Brightness
//#define ILI9341_READ_CABC_MINIMUM        0x5F    // Read CABC Minimum Brightness
#define ILI9341_FRMCTR1     0xB1    // Frame rate control (In normal mode/full colors)
//#define ILI9341_FRMCTR2     0xB2    // Frame rate control (In idle mode/8 colors)
//#define ILI9341_FRMCTR3     0xB3    // Frame rate control (In partial mode/full colors)
//#define ILI9341_INVCTR      0xB4    // Display inversion control
//#define ILI9341_BLPORCTL    0xB5    // Blanking Porch Controll : 0..255, from v.1.02  : 0..127
#define ILI9341_DFUNCTR     0xB6    // Display function control
//#define ILI9341_ENTMODSET   0xB7    // Entry Mode Set
#define ILI9341_PWCTR1      0xC0    // Power control 1
#define ILI9341_MADCTL13    0xC0    // v1.3 configuration xCRICxCC
//  I - inverse colors,
//  R - 0:RGB, 1:BGR,
//  C__C_CC - rotation + reverse mode
//  CxxCxCC             | CxxCxCC
//  0..0.00    0  <--   | 1..0.00    0  -->
//  0..0.01    0  -->   | 1..0.01    0  <--
//  0..0.10   90  -->   | 1..0.10   90  <--
//  0..0.11   90  <--   | 1..0.11   90  -->
//  0..1.00  180  -->   | 1..1.00  180  <--
//  0..1.01  180  <--   | 1..1.01  180  -->
//  0..1.10  270  <--   | 1..1.10  270  -->
//  0..1.11  270  -->   | 1..1.11  270  <--
#define ILI9341_PWCTR2      0xC1    // Power control 2
//#define ILI9341_PWCTR3      0xC2    // Power Control 3
//#define ILI9341_PWCTR4      0xC3    // Power Control 4
//#define ILI9341_PWCTR5      0xC4    // Power Control 5
#define ILI9341_VCOMCTR1      0xC5    // Set the VCOM(H/L) voltage
#define ILI9341_VCOMCTR2      0xC7    // Set the VCOM offset voltage
//#define ILI9341_RDID1       0xDA    // Read ID 1
//#define ILI9341_RDID2       0xDB    // Read ID 2
//#define ILI9341_RDID3       0xDC    // Read ID 3
//#define ILI9341_RDID4       0xD3    // Read ID 4
#define ILI9341_GMCTRP1     0xE0    // Positive gamma correction
#define ILI9341_GMCTRN1     0xE1    // Negative gamma correction
#define ILI9341_EF          0xEF    // unknown
#define ILI9341_PUMPRC      0xF7    // Pump ratio control
//#define ILI9341_PWCTR6      0xFC

// retired in version v.1.02
#define ILI9341_PWCTRA      0xCB    // Power control A
#define ILI9341_PWCTRB      0xCF    // Power control B
#define ILI9341_DTCA        0xE8    // Driver timing control A
// E9
#define ILI9341_DTCB        0xEA    // Driver timing control B
#define ILI9341_POSC        0xED    // Power on sequence control
#define ILI9341_ENABLE3G    0xF2    // Enable 3 gamma control

void Display::sendCmd(const uint8_t cmd)
{
  sendCommand(cmd);
}

void Display::sendCmdData(const uint8_t cmd, const int16_t size, const uint8_t* data)
{
  sendCommand(cmd, data, size);
}

void Display::sendCmdData(const uint8_t cmd, const uint8_t data)
{
  sendCommand(cmd, &data, 1);
}

#if defined(ARDUINO_ARCH_ARC32) || defined(ARDUINO_MAXIM)
#define SPI_DEFAULT_FREQ 16000000
// Teensy 3.0, 3.1/3.2, 3.5, 3.6
#elif defined(__MK20DX128__) || defined(__MK20DX256__) ||                      \
    defined(__MK64FX512__) || defined(__MK66FX1M0__)
#define SPI_DEFAULT_FREQ 40000000
#elif defined(__AVR__) || defined(TEENSYDUINO)
#define SPI_DEFAULT_FREQ 8000000
#elif defined(ESP8266) || defined(ESP32)
#define SPI_DEFAULT_FREQ 40000000
#elif defined(RASPI)
#define SPI_DEFAULT_FREQ 80000000
#elif defined(ARDUINO_ARCH_STM32F1)
#define SPI_DEFAULT_FREQ 36000000
#else
#define SPI_DEFAULT_FREQ 24000000 ///< Default SPI data clock frequency
#endif

void Display::begin(uint32_t freq)
{
  if (!freq)
    freq = SPI_DEFAULT_FREQ;

  initSPI(2 * 1000 * 1000);  // slow down

  if (_rst < 0) {                 // If no hardware reset pin...
    sendCommand(ILI9341_SWRESET); // Engage software reset
    delay(150);
  }

  if (ILI9341_VERSION < 3) {  // < v1.2
    static uint8_t ILI9341_EF_DATA[] = {0x03, 0x80, 0x02};
    sendCmdData(ILI9341_EF, 3, ILI9341_EF_DATA);              // unknown

    static uint8_t ILI9341_PWCTRB_DATA[] = {0x00, 0xC1, 0x30};
    sendCmdData(ILI9341_PWCTRB, 3, ILI9341_PWCTRB_DATA);      // retired in v.1.02

    static uint8_t ILI9341_POSC_DATA[] = {0x64, 0x03, 0x12, 0x81};
    sendCmdData(ILI9341_POSC, 4, ILI9341_POSC_DATA);          // retired in v.1.02

    static uint8_t ILI9341_DTCA_DATA[] = {0x85, 0x00, 0x78};
    sendCmdData(ILI9341_DTCA, 3, ILI9341_DTCA_DATA);          // retired in v.1.02

    static uint8_t ILI9341_PWCTRA_DATA[] = {0x39, 0x2C, 0x00, 0x34, 0x02};
    sendCmdData(ILI9341_PWCTRA, 5, ILI9341_PWCTRA_DATA);      // retired in v.1.02
  }

  sendCmdData(ILI9341_PUMPRC,   0x20);

  if (ILI9341_VERSION < 3) {  // < v1.2
    static uint8_t ILI9341_DTCB_DATA[] = {0x00, 0x00};
    sendCmdData(ILI9341_DTCB, 2, ILI9341_DTCB_DATA);          // retired in v.1.02
  }

  if (ILI9341_VERSION < 3) {  // < v1.2
    sendCmdData(ILI9341_PWCTR1, 0x23);             // Power control VRH[5:0]
  }
  else {
    sendCmdData(ILI9341_MADCTL13, 0x21);           // xCRICxCC
  }

  sendCmdData(ILI9341_PWCTR2,   0x10);             // Power control SAP[2:0];BT[3:0]

  static uint8_t ILI9341_VCOMCTR_DATA[] = {0x31, 0x3c};
  sendCmdData(ILI9341_VCOMCTR1, 2, ILI9341_VCOMCTR_DATA);       // VCM control 1

  sendCmdData(ILI9341_VCOMCTR2, 0xC0);             // VCM control 2

  if (ILI9341_VERSION < 3) {  // < v1.2
    sendCmdData(ILI9341_MADCTL,   0x48);             // Memory Access Control
  }

  sendCmdData(ILI9341_VSCRSADD, 0x00);             // Vertical scroll zero

  sendCmdData(ILI9341_PIXFMT,   0x55);

  static uint8_t ILI9341_FRMCTR1_DATA[] = {0x00, 0x18};
  sendCmdData(ILI9341_FRMCTR1, 2, ILI9341_FRMCTR1_DATA);

  static uint8_t ILI9341_DFUNCTR_DATA[] = {0x08, 0x82, 0x27};
  sendCmdData(ILI9341_DFUNCTR, 3, ILI9341_DFUNCTR_DATA);  // Display Function Control

  sendCmdData(ILI9341_ENABLE3G, 0x00);             // 3Gamma Function Disable   // retired in v.1.02

  sendCmdData(ILI9341_GAMMASET, 0x01);             // Gamma curve selected

  if (ILI9341_VERSION < 3) {  // < v1.2
    static uint8_t ILI9341_GMCTRP1_DATA_OLD[] = {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x26, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00};
    sendCmdData(ILI9341_GMCTRP1, 15, ILI9341_GMCTRP1_DATA_OLD);  // positive gamma correction
  }
  else {
    static uint8_t ILI9341_GMCTRP1_DATA_V13[] = {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x20, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00};
    sendCmdData(ILI9341_GMCTRP1, 15, ILI9341_GMCTRP1_DATA_V13);  // positive gamma correction v1.3
  }

  static uint8_t ILI9341_GMCTRN1_DATA[] = {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F};
  sendCmdData(ILI9341_GMCTRN1, 15, ILI9341_GMCTRN1_DATA);  // negative gamma correction

  sendCmd(ILI9341_SLPOUT);   // Exit Sleep
  delay(150);

  sendCmd(ILI9341_DISPON);   // Display on
  delay(150);

  setSPISpeed(freq);   // restore speed
}

/**************************************************************************/
/*!
  @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Display::setRotation(const int8_t rotation) {
  uint8_t m = 0;
  uint8_t g = 0; // v1.3
  switch (rotation % 4) { // can't be higher than 3
  case 0:
    m = 0x40 | (BGR << 3); // MX
    g = 0x01 | (BGR << 5);
    _width = WIDTH;
    _height = HEIGHT;
    break;
  case 1:
    m = 0x20 | (BGR << 3); // MV
    g = 0x02 | (BGR << 5);
    _width = HEIGHT;
    _height = WIDTH;
    break;
  case 2:
    m = 0x80 | (BGR << 3); // MY
    g = 0x08 | (BGR << 5);
    _width = WIDTH;
    _height = HEIGHT;
    break;
  case 3:
    m = 0xe0 | (BGR << 3); // MX | MY | MV
    g = 0x0b | (BGR << 5);
    _width = HEIGHT;
    _height = WIDTH;
    break;
  }
  if (ILI9341_VERSION < 3) { // < v1.2
    sendCmdData(ILI9341_MADCTL, m);
  }
  else {
    sendCmdData(ILI9341_MADCTL13, g);
  }
}

void Display::drawText(const int16_t aLeft, const int16_t aTop, const char* aText, const int8_t aFontSize, const rgb_t aTextColor)
{
  setCursor(aLeft, aTop);
  setTextSize(aFontSize);
  setTextColor(aTextColor);
  print(aText);
}

void Display::drawTextLine(cursor_t* cursor, font_t* font, const char* aText, const rgb_t aTextColor)
{
  GFXfont* oldfont = gfxFont;
  setFont(font->gfxFont);
  int16_t x = cursor->x, y = cursor->y;
  font->cursorAdjust(&x, &y);
  setCursor(x, y);
  setTextSize(font->fontSizeX, font->fontSizeY);
  setTextColor(aTextColor);
  print(aText);
  setFont(oldfont);
}

void Display::clearDisplay()
{
  fillScreen(BLACK);
}
