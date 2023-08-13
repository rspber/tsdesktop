/*
  User setup for RP2040 SPI ST7796 with touch
*/

// -------------------------------TFT driver ----------------------------------

//  #define GC9A01            // -- not tested

//  #define HX8357B           // -- not tested
//  #define HX8357C           // -- not tested
//  #define HX8357D           // -- not tested

//  #define ILI9341       // 2.4" TFT SPI 240x320 no version - tested

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

  #define ST7796            // 4" TFT SPI 480x320 v1.0 - tested

// -------------------------------TFT params ----------------------------------

  #define COLOR_565     // comment it for 666 colors

// don't enable OVERLAID if you are not shure that absolutelly
// all colors comes from RGB macro, even 0 is not a BLACK
  #define OVERLAID      // enable overlaid in writeColor and writeChar

// select one of
//  #define TFT_PIO_16BITP_WRITE    // TODO
//  #define TFT_PIO_8BITP_WRITE
//  #define TFT_GPIO_8BITP_WRITE
//  #define TFT_PIO_SPI_WRITE
  #define TFT_SPI_WRITE
//  #define TFT_NO_WRITE

// select one of
//  #define TFT_GPIO_8BITP_READ        cannot be mixed with spi
//  #define TFT_PIO_SPI_READ
  #define TFT_SPI_READ
//  #define TFT_NO_READ

// Display screen size
  #define TFT_WIDTH  320
  #define TFT_HEIGHT 480

// SPI0
  #define TFT_SPI_MISO   16  // RX - default
  #define TFT_SPI_CS     17    // Chip select control pin
  #define TFT_SPI_CLK    18  // SCK - default
  #define TFT_SPI_MOSI   19  // TX - default
  #define TFT_SPI_RST    20    // Reset pin
  #define TFT_SPI_DC     21    // Data Command control pin
//  #define TFT_SPI_BL       // LED back-light

  #define TFT_SPI_SETUP_SPEED     2 * 1000 * 1000          // 2 MHz
  #define TFT_SPI_WRITE_SPEED    60 * 1000 * 1000          // 60 MHz
  #define TFT_SPI_READ_SPEED      2 * 1000 * 1000          //  2 MHz

  #define TFT_REV 0

// ---------------------------- Touch Screen ----------------------------------

// select one of or none
  #define TOUCH_SPI
  #define TOUCH_SPI_SPEED         2 * 1000 * 1000          //  2 MHz

// don't comment it if T.CS is connected to this pin
  #define TOUCH_SPI_CS   22     // Chip select pin (T_CS) of touch screen

  #define TOUCH_REV 0
  #define TOUCH_ROTATION touch_7796

// TouchScreen edges of the display in range of 0..4095
  #define TS_LEFT 0
  #define TS_TOP 200
  #define TS_RIGHT 150
  #define TS_BOTTOM 100

// -------------------------------I2C params ----------------------------------

  #define I2C0_SDA_PIN 4
  #define I2C0_SCL_PIN 5

  #define I2C1_SDA_PIN 6
  #define I2C1_SCL_PIN 7
