/*
  User setup for STM32 SPI ILI9341 with touch
*/

// -------------------------------TFT driver ----------------------------------

//  #define GC9A01            // -- not tested

//  #define HX8357B           // -- not tested
//  #define HX8357C           // -- not tested
//  #define HX8357D           // -- not tested

  #define ILI9341       // 2.4" TFT SPI 240x320 no version - tested

//  #define ILI9481_1         // -- not tested
//  #define ILI9481_AUO317    // -- not tested
//  #define ILI9481_CMO35     // -- not tested
//  #define ILI9481_CPT29     // -- not tested
//  #define ILI9481_PVI35     // -- not tested
//  #define ILI9481_RGB       // -- not tested
//  #define ILI9481_YA1       // -- not tested
//  #define ILI9481_YA2       // -- not tested

//  #define ILI9486           // -- not tested
//  #define ILI9488           // 3.5" TFT 480x320 - tested

//  #define R61581            // -- not tested
//  #define RM68140           // -- not tested

//  #define SSD1963_480       // -- not tested
//  #define SSD1963_800       // -- not tested
//  #define SSD1963_800ALT    // -- not tested
//  #define SSD1963_800BD     // -- not tested

//  #define ST7789        // 2.4" TFT SPI 240x320 v1.0, v1.1, v1.2, (v1.3 - tested)

//  #define ST7796            // 4" TFT SPI 480x320 v1.0 - tested

// -------------------------------TFT params ----------------------------------

  #define COLOR_565     // comment it for 666 colors

// don't enable OVERLAID if you are not shure that absolutelly
// all colors comes from RGB macro, even 0 is not a BLACK
//  #define OVERLAID      // enable overlaid in writeColor and writeChar

// select one of
//  #define TFT_GPIO_8BITP_WRITE
  #define TFT_SPI_WRITE
//  #define TFT_NO_WRITE

// select one of
//  #define TFT_GPIO_8BITP_READ        cannot be mixed with spi
  #define TFT_SPI_READ
//  #define TFT_NO_READ

// Display screen size
  #define TFT_WIDTH  240
  #define TFT_HEIGHT 320

  // Arduino IDE - WeAct Studio
  // Generic STM32F4 Series / F405RGTx  = STM32F405 RGT6
  // Generic STM32F4 Series / F412RETx  = STM32F412 RET6
  // Nucleo 64         / Nucleo F446RE  = STM32F446 RET6

  //      F405RGTx     F412RETx     F446RETx
  // ---------------------------------------
  //  D0       PA0          PA0          PA3
  //  D1       PA1          PA1          PA2
  //  D2       PA2          PA2         PA10
  //  D3       PA3          PA3          PB3
  //  D4       PA4          PA4          PB5
  //  D5       PA5          PA5          PB4
  //  D6       PA6          PA6         PB10
  //  D7       PA7          PA7          PA8
  //  D8       PA8          PA8          PA9
  //  D9       PA9          PA9          PC7
  //  D10     PA10         PA10          PB6
  //  D11     PA11         PA11          PA7
  //  D12     PA12         PA12          PA6
  //  D13        -            -          PA5
  //  D14        -            -          PB9
  //  D15     PA15         PA15          PB8

// SPI 1
  #define TFT_SPI_CLK    PA5    //
  #define TFT_SPI_MISO   PA6    //
  #define TFT_SPI_MOSI   PA7    //

  #define TFT_SPI_CS     PB7    //       SS - chip select control pin
  #define TFT_SPI_DC     PB10   //       Data Command control pin
  #define TFT_SPI_RST    PA8    //       Reset pin
//  #define TFT_SPI_BL       // LED back-light

  #define TFT_SPI_SETUP_SPEED     2 * 1000 * 1000          //  2 MHz
  #define TFT_SPI_WRITE_SPEED    53 * 1000 * 1000          // 53 MHz
  #define TFT_SPI_READ_SPEED      5 * 1000 * 1000          //  5 MHz

  #define TFT_REV 0

// ---------------------------- Touch Screen ----------------------------------

// select one of or none
  #define TOUCH_SPI
  #define TOUCH_SPI_SPEED         2 * 1000 * 1000          //  2 MHz

// don't comment it if T.CS is connected to this pin
  #define TOUCH_SPI_CS  PB6

  #define TOUCH_REV 0
  #define TOUCH_ROTATION touch_9xxx

// TouchScreen edges of the display in range of 0..4095
  #define TS_LEFT 150
  #define TS_TOP 400
  #define TS_RIGHT 150
  #define TS_BOTTOM 150

// -------------------------------I2C params ----------------------------------

  #define I2C0_SDA_PIN PB9
  #define I2C0_SCL_PIN PB8

  #define I2C1_SDA_PIN -1
  #define I2C1_SCL_PIN -1
