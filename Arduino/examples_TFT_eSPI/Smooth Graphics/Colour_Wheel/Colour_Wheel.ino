// Arc drawing example - draw a colour wheel

#include <TFT_eSPI.h>       // Include the graphics library
TFT_eSPI tft = TFT_eSPI();  // Create object "tft"

uint32_t colors[12];

// -------------------------------------------------------------------------
// Setup
// -------------------------------------------------------------------------
void setup(void) {
  Serial.begin(115200);
  tft.init();
  tft.fillScreen(BLACK);

  // Create the outer ring colours
  for (uint8_t c = 0; c < 2; c++) {
    colors[c + 10] = tft.alphaBlend(128 + c * 127, RED,     MAGENTA);
    colors[c +  8] = tft.alphaBlend(128 + c * 127, MAGENTA, BLUE);
    colors[c +  6] = tft.alphaBlend(128 + c * 127, BLUE,    GREEN);
    colors[c +  4] = tft.alphaBlend(128 + c * 127, GREEN,   YELLOW);
    colors[c +  2] = tft.alphaBlend(128 + c * 127, YELLOW,  ORANGE);
    colors[c +  0] = tft.alphaBlend(128 + c * 127, ORANGE,  RED);
  }
}

// -------------------------------------------------------------------------
// Main loop
// -------------------------------------------------------------------------
void loop() {
  uint16_t rDelta = (tft.width() - 1) / 10;
  uint16_t x = tft.width() / 2;
  uint16_t y = tft.height() / 2;
  bool smooth = true;

  // Draw rings as a series of arcs, increasingly blend colour with white towards middle
  for (uint16_t i = 5; i > 0; i--) {
    for (uint16_t angle = 0; angle <= 330; angle += 30) {
      uint16_t radius = i * rDelta;
      uint16_t wheelColor = tft.alphaBlend((i * 255.0)/5.0, colors[angle / 30], WHITE);
      tft.drawArc(x, y, radius, radius - rDelta, angle, angle + 30, wheelColor, BLACK, smooth);
    }
    smooth = false;  // Only outer ring is smooth
  }

  while (1) delay(100);
}
