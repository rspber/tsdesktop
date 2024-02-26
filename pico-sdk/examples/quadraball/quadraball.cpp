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

  // buffered screen is like a window on parent screen
  // drawing in window refers to absolute positions of parent screen
  // below values refer to 0,0 window position
  window.drawRect(20, 20, 50, 50, CYAN);
  window.drawRect(50, 50, 40, 40, RED);

  // this effect is described in tsdeeSPI as Yeti technique
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

  window.push(&display);

  window.setPos(x, y);

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

int main() {

  stdio_init_all();

  setup();

  while (true) {
    loop();
  }
}

// ----------------------------------------------------------------
