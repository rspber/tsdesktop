/*
  spider on TSDesktop

  buffered display example

  Copyright (c) 2024, rspber (https://github.com/rspber)

*/

#include <Display.h>
#include <BufferedDisplay.h>

Display display;
bool screenEnabled = false;

#define ROTATION ROTATION_HLR

// ----------------------------------------------------------------

BufferedDisplay window(0, 0, 43, 43, BLUE);

// ----------------------------------------------------------------

uint8_t LED_PIN = DEFAULT_LED_PIN;
int pin13state = LOW;

void drawSpider(BufferedDisplay& p, const rgb_t color) {
//  p.drawRect(5, 5, 35, 35, SILVER);
  p.fillEllipse(20, 20, 7, 9, color);
  p.fillEllipse(14, 14, 2, 4, BLACK);
  p.fillEllipse(26, 14, 2, 4, BLACK);
  p.fillEllipse(14, 28, 2, 4, BLACK);
  p.fillEllipse(26, 28, 2, 4, BLACK);
  p.fillEllipse(20, 20, 2, 4, BLACK);
}

void drawSpidersWeb(TSD_SCREEN& p, const rgb_t color) {

  int16_t w = display.width();
  int16_t h = display.height();

  p.drawLine(   0,    0,       w,       h, color, 1);
  p.drawLine(  80,    0,  w - 80,       h, color, 1);
  p.drawLine( 160,    0, w - 160,       h, color, 1);
  p.drawLine( 240,    0, w - 240,       h, color, 1);
  p.drawLine(   w,    0,       0,       h, color, 1);
  p.drawLine(   0,   80,       w,  h - 80, color, 1);
  p.drawLine(   0,  160,       w, h - 160, color, 1);
}

void setup()   {

  display.begin();
  display.setRotation(ROTATION);
  display.fillScreen(TEAL);

  pinMode(LED_PIN, OUTPUT);

  drawSpidersWeb(display, RED);
}

// ----------------------------------------------------------------

int x = 0;
int y = 0;
int dx = 1;
int dy = 1;

void loop() {

  window.setPos(x, y);

  window.clear(TEAL);
  drawSpidersWeb(window, RED);

  window.setPos(0, 0);
  drawSpider(window, BROWN);
  window.setPos(x, y);

  window.push(&display);

//  delay(5);

  x += dx;
  y += dy;
  
  if( x < -40) {
    dx = 1;   
  }

  if( x > 310) {
    dx = -1;   
  }

  if( y < -40) {
    dy = 1;   
  }

  if( y > 230) {
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
