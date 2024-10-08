/*
  User setup for PIC32 Uno32 SPI ILI9341 with touch
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

// SPI0
  #define TFT_SPI_CLK    13
  #define TFT_SPI_MISO   12
  #define TFT_SPI_MOSI   11
  #define TFT_SPI_CS     10    // SS - chip select control pin
  #define TFT_SPI_RST     9    // Reset pin
  #define TFT_SPI_DC      8    // Data Command control pin
//  #define TFT_SPI_BL       // LED back-light

  #define TFT_SPI_SETUP_SPEED     2 * 1000 * 1000          //  2 MHz
  #define TFT_SPI_WRITE_SPEED    40 * 1000 * 1000          // 40 MHz
  #define TFT_SPI_READ_SPEED     20 * 1000 * 1000          // 20 MHz

  #define TFT_REV 0

// ---------------------------- Touch Screen ----------------------------------

// select one of or none
  #define TOUCH_SPI
  #define TOUCH_SPI_SPEED         2 * 1000 * 1000          //  2 MHz

// don't comment it if T.CS is connected to this pin
  #define TOUCH_SPI_CS    7

  #define TOUCH_REV 0
  #define TOUCH_ROTATION touch_9xxx

// TouchScreen edges of the display in range of 0..4095
  #define TS_LEFT 150
  #define TS_TOP 400
  #define TS_RIGHT 150
  #define TS_BOTTOM 150

// -------------------------------I2C params ----------------------------------

  #define I2C0_SDA_PIN 36
  #define I2C0_SCL_PIN 37

//  #define I2C1_SDA_PIN ?
//  #define I2C1_SCL_PIN ?
