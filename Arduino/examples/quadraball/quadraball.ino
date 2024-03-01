/*
  quadraball on TSDesktop

  buffered display example

  Copyright (c) 2024, rspber (https://github.com/rspber)

*/

#include <Display.h>
#include <BufferedDisplay.h>

Display display;
bool screenEnabled = false;

#define ROTATION ROTATION_HLR

// ----------------------------------------------------------------

BufferedDisplay window(0, 0, 100, 100, BLUE);

// ----------------------------------------------------------------

uint8_t LED_PIN = DEFAULT_LED_PIN;
int pin13state = LOW;

void setup()   {

  display.begin();
  display.setRotation(ROTATION);
  display.fillScreen(TEAL);

  pinMode(LED_PIN, OUTPUT);

  // BufferedDisplay is like a window on full sizes screen
  // drawing in window refers to absolute positions of full sizes screen
  // what remains on the window after this drawing depends on its position during this drawing

  window.setPos(0, 0);
  // below values refer to 0,0 window position
  window.drawRect(20, 20, 50, 50, CYAN);
  window.drawText(25, 25, "Lime", 1, LIME, BLUE);
  window.drawRect(50, 50, 40, 40, RED);
  window.drawText(55, 55, "Red", 1, RED, BLUE);

  // this effect is described in tsdeeSPI README.md
  window.drawRect(0, 0, window.width(), window.height(), TEAL);

  // in case to redraw window, set it's positions to 0, 0,
  // do drawings on it in relative to window 0,0 position values,
  // and place it where it should be finally
}

// ----------------------------------------------------------------

int x = 0;
int y = 0;
int dx = 1;
int dy = 1;

void loop() {

  if ( (x & 0x1f) == 0x0f) {
    if (dx > 0) {
      window.rotateRight(0, 0, window.width());
    }
    else {
      window.rotateLeft(0, 0, window.width());
    }
  }

  if ( (x & 0x1f) == 0x1f) {
    if (dx > 0) {
      window.flipHorizontal(0, 0, window.width(), window.height());
    }
    else {
      window.flipVertical(0, 0, window.width(), window.height());
    }
  }

  window.setPos(x, y);

  window.push(&display);

//  delay(50);

  x += dx;
  y += dy;
  
  if( x < -40) {
    dx = 1;   
  }

  if( x > 300) {
    dx = -1;   
  }

  if( y < -40) {
    dy = 1;   
  }

  if( y > 200) {
    dy = -1;   
  }

  pin13state = 1 - pin13state;
  digitalWrite(LED_PIN, pin13state);
}

// ----------------------------------------------------------------
