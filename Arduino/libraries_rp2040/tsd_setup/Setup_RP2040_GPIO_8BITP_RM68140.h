/*
  User setup for RP2040 GPIO 8BIT parallel RM68140
*/

// -------------------------------TFT driver ----------------------------------

//  #define GC9A01            // -- not tested
      // TFT_REV MAD_MX or 0

//  #define HX8357B           // -- not tested
//  #define HX8357C           // -- not tested
//  #define HX8357D           // -- not tested
      // TFT_REV MAD_MY or 0

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
  #define RM68140           // -- ??? 3.5" TFT LCD for arduino uno (red)

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
//  #define TFT_PIO_16BITP_WRITE    // TODO
//  #define TFT_PIO_8BITP_WRITE
  #define TFT_GPIO_8BITP_WRITE
//  #define TFT_PIO_SPI_WRITE
//  #define TFT_SPI_WRITE
//  #define TFT_NO_WRITE

// select one of
  #define TFT_GPIO_8BITP_READ          // can be mixed with all 8bitp
//  #define TFT_PIO_SPI_READ             cannot be mixed with 8bitp
//  #define TFT_SPI_READ                 cannot be mixed with 8bitp
//  #define TFT_NO_READ

// Display screen size
  #define TFT_WIDTH  320
  #define TFT_HEIGHT 480

  #define TFT_8BITP_CS    1    //  1  LCD_CS - Chip select,  if no READ and TOUCH comment it and connent LCD_CS to 0V
  #define TFT_8BITP_DC   26    // A0  LCD_RS - Command / Data
  #define TFT_8BITP_WR   27    // A1  LCD_WR - Write
  #define TFT_8BITP_RD    0    //  0  LCD_RD - Read,  if no READ and TOUCH comment it and connent LCD_RD to 3.3V
  #define TFT_8BITP_RST   2    //  2  Reset pin

// in simplest implementation of GPIO these must be sequentially increasing
  #define TFT_8BITP_D0    8
  #define TFT_8BITP_D1    9
//  #define TFT_8BITP_D2   10
//  #define TFT_8BITP_D3   11
//  #define TFT_8BITP_D4   12
//  #define TFT_8BITP_D5   13
  #define TFT_8BITP_D6   14
  #define TFT_8BITP_D7   15

    #define TFT_REV 0

// ---------------------------- Touch Screen ----------------------------------

// select one of or none
//  #define TOUCH_SPI                    cannot be mixed with 8bitp
  #define TOUCH_RESISTIVE

// Resistive Touch
  #define TOUCH_Y_PLUS   TFT_8BITP_WR  // Y+ pin. Must be an analog pin
  #define TOUCH_X_PLUS   TFT_8BITP_D6  // X+ pin. Can be a digital pin
  #define TOUCH_Y_MINUS  TFT_8BITP_D7  // Y- pin. Can be a digital pin
  #define TOUCH_X_MINUS  TFT_8BITP_DC  // X- pin. Must be an analog pin
  #define TOUCH_RX       300           // The resistance in ohms between X+ and X- to calibrate pressure

  #define TOUCH_REV MAD_YX
  #define TOUCH_ROTATION touch_9xxx

// TouchScreen edges of the display in range of 0..4095
  #define TS_LEFT 650
  #define TS_TOP 450
  #define TS_RIGHT 350
  #define TS_BOTTOM 250

// -------------------------------I2C params ----------------------------------

  #define I2C0_SDA_PIN 4
  #define I2C0_SCL_PIN 5

  #define I2C1_SDA_PIN 6
  #define I2C1_SCL_PIN 7
