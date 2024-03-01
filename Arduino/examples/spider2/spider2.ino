/*
  spider 2 on TSDesktop

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

void drawSpider(BufferedDisplay& w) {
//  w.drawRect(5, 5, 35, 35, SILVER);
  w.fillEllipse(20, 20, 7, 9, BROWN);
  w.fillEllipse(14, 14, 2, 4, BLACK);
  w.fillEllipse(26, 14, 2, 4, BLACK);
  w.fillEllipse(14, 28, 2, 4, BLACK);
  w.fillEllipse(26, 28, 2, 4, BLACK);
  w.fillEllipse(20, 20, 2, 4, BLACK);
}

rgb_t WEB_1 = RGB(0xc8, 0xcc, 0xc8);
rgb_t WEB_2 = RGB(0xc0, 0xcc, 0xc8);
rgb_t WEB_3 = RGB(0xc8, 0xcc, 0xc0);
rgb_t WEB_4 = RGB(0xc8, 0xc8, 0xc8);
rgb_t WEB_5 = RGB(0xc0, 0xc8, 0xc8);
rgb_t WEB_6 = RGB(0xc8, 0xc8, 0xc0);
rgb_t WEB_7 = RGB(0xc8, 0xc4, 0xc8);


void drawSegment(TSD_SCREEN& p, int16_t w2, int16_t w3, int16_t w4, int16_t h1, int16_t h2, int16_t h3, int16_t h4)
{
  int16_t h = display.height();
 
  p.drawLine( 160,    h1,   w2,    h2, LIGHT_GRAY, 1);
  p.drawLine(  w2,    h2,   w3,    h3, LIGHT_GRAY, 1);
  p.drawLine(  w3,    h3,   w4,    h4, LIGHT_GRAY, 1);
  p.drawLine(  w4,    h4,   w4,  h-h4, LIGHT_GRAY, 1);
  p.drawLine(  w4,  h-h4,   w3,  h-h3, LIGHT_GRAY, 1);
  p.drawLine(  w3,  h-h3,   w2,  h-h2, LIGHT_GRAY, 1);
  p.drawLine(  w2,  h-h2,  160,  h-h1, LIGHT_GRAY, 1);
}

void drawSpidersWeb(TSD_SCREEN& p) {

  int16_t w = display.width();
  int16_t h = display.height();

  p.drawLine(0, 0, w, h, WEB_1, 1);
  p.drawLine(80, 0, w - 80, h, WEB_2, 1);
  p.drawLine(160, 0, w - 160, h, WEB_3, 1);
  p.drawLine(240, 0, w - 240, h, WEB_4, 1);
  p.drawLine(w, 0, 0, h, WEB_5, 1);
  p.drawLine(0, 80, w, h - 80, WEB_6, 1);
  p.drawLine(0, 160, w, h - 160, WEB_7, 1);

  // some primitive method drawing spiderweb

  drawSegment(p,    95,    65,    50,  7, 25, 55, 90); 
  drawSegment(p,  w-95,  w-65,  w-50,  7, 25, 55, 90); 

  drawSegment(p,   105,    85,    70, 30, 45, 65, 95); 
  drawSegment(p, w-105,  w-85,  w-70, 30, 45, 65, 95); 

  drawSegment(p,   125,   100,    90, 50, 60, 75, 100); 
  drawSegment(p, w-125, w-100,  w-90, 50, 60, 75, 100); 

  drawSegment(p,   145,   120,   110, 70, 75, 90, 110); 
  drawSegment(p, w-145, w-120, w-110, 70, 75, 90, 110); 
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
