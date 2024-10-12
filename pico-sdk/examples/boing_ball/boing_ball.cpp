// 'Boing' ball demo

// TFT_eSPI -------------------------------------------
//                       *no DMA*    DMA
// STM32F767 55MHz SPI       170     227 fps
// STM32F446 55MHz SPI       110     187 fps
// STM32F401 55MHz SPI        56     120 fps

// STM32F767 27MHz SPI        99     120 fps
// STM32F446 27MHz SPI        73      97 fps
// STM32F401 27MHz SPI        51      90 fps

// Blue Pill - 36MHz SPI      36      67 fps
// Blue Pill - overcl. 128MHz 64     116 fps

// ESP32 - 8 bit parallel    110 fps
// ESP32 - 40MHz SPI          93     112 fps

// tsdesktop -------------------------------------------

// ESP32 - 40MHz SPI          53     103 fps   WROOM-32

// RP2040  62.5MHz PIO SPI    69     129 fps   125MHz/2

// RP2040 60MHz SPI           54     96 fps     = 40MHz
// RP2040 40MHz SPI           54     96 fps
// RP2040 20MHz SPI           33     48 fps

// -----------------------------------------------------

#define SCREENWIDTH 320
#define SCREENHEIGHT 240

#include <Display.h> // Hardware-specific library

Display display;       // Invoke custom library

#include "graphic.h"

#define TFT_BGCOLOR    0xAD75
#define BGCOLOR        RGB(0xA8, 0xAC, 0xA8)
#define TFT_GRIDCOLOR  0xA815
#define GRIDCOLOR      RGB(0xA8, 0x00, 0xA8)
#define TFT_BGSHADOW   0x5285
#define TFT_GRIDSHADOW 0x600C
#define TFT_VIOLET     0xF81C
#define TFT_YELLOW     0xcfcc

#define YBOTTOM  123  // Ball Y coordinate at bottom
#define YBOUNCE -3.5  // Upward velocity on ball bounce

// Ball coordinates are stored floating-point because screen refresh
// is so quick, whole-pixel movements are just too fast!
float ballx     = 20.0, bally     = YBOTTOM, // Current ball position
      ballvx    =  0.8, ballvy    = YBOUNCE, // Ball velocity
      ballframe = 3;                         // Ball animation frame #
int   balloldx  = ballx, balloldy = bally;   // Prior ball position

// Working buffer for ball rendering...2 scan lines that alternate,
// one is rendered while the other is transferred via DMA.
uint16_t renderbuf[2][SCREENWIDTH];

uint16_t palette[16]; // Color table for ball rotation effect

uint32_t startTime, frame = 0; // For frames-per-second estimate

void setup() {
  Serial.begin(115200);
//  while(!Serial);

  display.begin();
  display.setRotation(3); // Landscape orientation, USB at bottom right
//  display.setSwapBytes(false);
  // Draw initial frame buffer contents:
  //display.setBitmapColor(GRIDCOLOR, BGCOLOR);
  display.fillScreen(BGCOLOR);

  display.initDMA();

  display.drawBitmap(0, 0, (const uint8_t *)background, SCREENWIDTH, SCREENHEIGHT, GRIDCOLOR);

  startTime = millis();
}

void loop() {

  balloldx = (int16_t)ballx; // Save prior position
  balloldy = (int16_t)bally;
  ballx   += ballvx;         // Update position
  bally   += ballvy;
  ballvy  += 0.06;          // Update Y velocity
  if((ballx <= 15) || (ballx >= SCREENWIDTH - BALLWIDTH))
    ballvx *= -1;            // Left/right bounce
  if(bally >= YBOTTOM) {     // Hit ground?
    bally  = YBOTTOM;        // Clip and
    ballvy = YBOUNCE;        // bounce up
  }

  // Determine screen area to update.  This is the bounds of the ball's
  // prior and current positions, so the old ball is fully erased and new
  // ball is fully drawn.
  int16_t minx, miny, maxx, maxy, width, height;
  // Determine bounds of prior and new positions
  minx = ballx;
  if(balloldx < minx)                    minx = balloldx;
  miny = bally;
  if(balloldy < miny)                    miny = balloldy;
  maxx = ballx + BALLWIDTH  - 1;
  if((balloldx + BALLWIDTH  - 1) > maxx) maxx = balloldx + BALLWIDTH  - 1;
  maxy = bally + BALLHEIGHT - 1;
  if((balloldy + BALLHEIGHT - 1) > maxy) maxy = balloldy + BALLHEIGHT - 1;

  width  = maxx - minx + 1;
  height = maxy - miny + 1;

  // Ball animation frame # is incremented opposite the ball's X velocity
  ballframe -= ballvx * 0.5;
  if(ballframe < 0)        ballframe += 14; // Constrain from 0 to 13
  else if(ballframe >= 14) ballframe -= 14;

  // Set 7 palette entries to white, 7 to red, based on frame number.
  // This makes the ball spin
  for(uint8_t i=0; i<14; i++) {
    palette[i+2] = ((((int)ballframe + i) % 14) < 7) ? TFT_YELLOW : TFT_VIOLET;
    // Palette entries 0 and 1 aren't used (clear and shadow, respectively)
  }

  // Only the changed rectangle is drawn into the 'renderbuf' array...
  uint16_t c, *destPtr;
  int16_t  bx  = minx - (int)ballx, // X relative to ball bitmap (can be negative)
           by  = miny - (int)bally, // Y relative to ball bitmap (can be negative)
           bgx = minx,              // X relative to background bitmap (>= 0)
           bgy = miny,              // Y relative to background bitmap (>= 0)
           x, y, bx1, bgx1;         // Loop counters and working vars
  uint8_t  p;                       // 'packed' value of 2 ball pixels
  int8_t bufIdx = 0;

  // Start SPI transaction and drop TFT_CS - avoids transaction overhead in loop
  display.startUsingDMA();

  // Set window area to pour pixels into
  display.startWrite();
  display.writeAddrWindow(minx, miny, width, height);
  display.endWrite();

  display.startWrite();
  // Draw line by line loop
  for(y=0; y<height; y++) { // For each row...
    destPtr = &renderbuf[bufIdx][0];
    bx1  = bx;  // Need to keep the original bx and bgx values,
    bgx1 = bgx; // so copies of them are made here (and changed in loop below)
    for(x=0; x<width; x++) {
      if((bx1 >= 0) && (bx1 < BALLWIDTH) &&  // Is current pixel row/column
         (by  >= 0) && (by  < BALLHEIGHT)) { // inside the ball bitmap area?
        // Yes, do ball compositing math...
        p = ball[by][bx1 / 2];                // Get packed value (2 pixels)
        c = (bx1 & 1) ? (p & 0xF) : (p >> 4); // Unpack high or low nibble
        if(c == 0) { // Outside ball - just draw grid
          c = background[bgy][bgx1 / 8] & (0x80 >> (bgx1 & 7)) ? TFT_GRIDCOLOR : TFT_BGCOLOR;
        } else if(c > 1) { // In ball area...
          c = palette[c];
        } else { // In shadow area...
          c = background[bgy][bgx1 / 8] & (0x80 >> (bgx1 & 7)) ? TFT_GRIDSHADOW : TFT_BGSHADOW;
        }
      } else { // Outside ball bitmap, just draw background bitmap...
        c = background[bgy][bgx1 / 8] & (0x80 >> (bgx1 & 7)) ? TFT_GRIDCOLOR : TFT_BGCOLOR;
      }
      *destPtr++ = c<<8 | c>>8; // Store pixel colour
      bx1++;  // Increment bitmap position counters (X axis)
      bgx1++;
    }

    display.sendMDTBuffer16((const uint8_t*)&renderbuf[bufIdx][0], width); // Push line to screen

    // Push line to screen (swap bytes false for STM/ESP32)
    //tft.pushPixels(&renderbuf[bufIdx][0], width);

    bufIdx = 1 - bufIdx;
    by++; // Increment bitmap position counters (Y axis)
    bgy++;
  }
  //if (random(100) == 1) delay(2000);
  display.endWrite();
  display.endUsingDMA();
  //delay(5);
  // Show approximate frame rate
  if(!(++frame & 255)) { // Every 256 frames...
    uint32_t elapsed = (millis() - startTime) / 1000; // Seconds
    if(elapsed) {
      Serial.print(frame / elapsed);
      Serial.println(" fps");
    }
  }
}


// ----------------------------------------------------------------
int main() {

  stdio_init_all();

  setup();

  while (true) {
    loop();
  }
}
// ----------------------------------------------------------------
