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

void drawSpider(TSD_SCREEN& w) {
//  w.drawRect(5, 5, 35, 35, SILVER);
  w.fillEllipse(20, 20, 7, 9, BROWN);
  w.fillEllipse(14, 14, 2, 4, BLACK);
  w.fillEllipse(26, 14, 2, 4, BLACK);
  w.fillEllipse(14, 28, 2, 4, BLACK);
  w.fillEllipse(26, 28, 2, 4, BLACK);
  w.fillEllipse(20, 20, 2, 4, BLACK);
}

void drawSpidersWeb(TSD_SCREEN& w) {
  w.drawLine(0, 0, display.width(), display.height(), RED, 1);
  w.drawLine(80, 0, display.width() - 80, display.height(), RED, 1);
  w.drawLine(160, 0, display.width() - 160, display.height(), RED, 1);
  w.drawLine(240, 0, display.width() - 240, display.height(), RED, 1);
  w.drawLine(display.width(), 0, 0, display.height(), RED, 1);
  w.drawLine(0, 80, display.width(), display.height() - 80, RED, 1);
  w.drawLine(0, 160, display.width(), display.height() - 160, RED, 1);
}

void setup()   {

  display.begin();
  display.setRotation(ROTATION);
  display.fillScreen(TEAL);

  pinMode(LED_PIN, OUTPUT);

  drawSpidersWeb(display);
}

// ----------------------------------------------------------------

int x = 0;
int y = 0;
int dx = 1;
int dy = 1;

void loop() {

  window.setPos(x, y);

  window.clear(TEAL);
  drawSpidersWeb(window);

  window.setPos(0, 0);
  drawSpider(window);
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
