/*
  idyll on TSDesktop

  buffered display example

  Copyright (c) 2024, rspber (https://github.com/rspber)

*/

#include <Display.h>
#include <BufferedDisplay.h>

Display display;
bool screenEnabled = false;

#define ROTATION ROTATION_HLR

gradient_t g2{GNT_LR, GREEN, 20, CYAN};

// ----------------------------------------------------------------

BufferedDisplay window(0, 0, 80, 80, BLUE);

// ----------------------------------------------------------------

uint8_t LED_PIN = DEFAULT_LED_PIN;
int pin13state = LOW;

void setup()   {

  display.begin();
  display.setRotation(ROTATION);
  display.fillScreen(TEAL);

  pinMode(LED_PIN, OUTPUT);

//  display.fillRectGradient(0, 0, 300, 200, g2);
}

// ----------------------------------------------------------------

int x =  0;
int y =  0;
int dx = 1;
int dy = 1;

void loop() {

  window.setPos(x, y);

  window.clear(TEAL);
  window.fillRectGradient(0, 0, 300, 200, g2);
  window.drawLine(0, 0, 300, 200, YELLOW, 1);

  window.setPos(0, 0);
  window.drawRect(10, 10, window.width() - 20, window.height() - 20, YELLOW);
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

int main() {

  stdio_init_all();

  setup();

  while (true) {
    loop();
  }
}

// ----------------------------------------------------------------
