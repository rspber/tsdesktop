/*
  Menu 2 on TSDesktop

  Copyright (c) 2022-2024, rspber (https://github.com/rspber)

*/

#include <Media.h>
#include <TSDesktop.h>
#include <Fonts/TSD_FreeSans_9pt.h>

#define ROTATION ROTATION_HLR

Display display;
bool screenEnabled = false;

Touch touch;

TextButton sel01;

Container* SELT[]{
  &sel01
};

FieldSet board(VERTICAL, 3, 7, 7, SELT, 1, BLUE);

TextButton tx01;
TextButton tx02;
TextButton tx03;
TextButton tx04;
TextButton tx05;
TextButton tx06;
TextButton tx07;
TextButton tx08;
TextButton tx09;
TextButton tx10;
TextButton tx11;
TextButton tx12;
TextButton tx13;
TextButton tx14;
TextButton tx15;
TextButton tx16;
TextButton tx17;
TextButton tx18;
TextButton tx19;
TextButton tx20;
TextButton tx21;
TextButton tx22;
TextButton tx23;
TextButton tx24;
TextButton tx25;
TextButton tx26;
TextButton tx27;
TextButton tx28;
TextButton tx29;
TextButton tx30;
TextButton tx31;
TextButton tx32;
TextButton tx33;
TextButton tx34;
TextButton tx35;
TextButton tx36;
TextButton tx37;
TextButton tx38;
TextButton tx39;
TextButton tx40;
TextButton tx41;
TextButton tx42;
TextButton tx43;
TextButton tx44;
TextButton tx45;
TextButton tx46;
TextButton tx47;
TextButton tx48;
TextButton tx49;
TextButton tx50;

Container* TBT[]{
  &tx01, &tx02, &tx03, &tx04, &tx05, &tx06, &tx07, &tx08, &tx09, &tx10,
  &tx11, &tx12, &tx13, &tx14, &tx15, &tx16, &tx17, &tx18, &tx19, &tx20,
  &tx21, &tx22, &tx23, &tx24, &tx25, &tx26, &tx27, &tx28, &tx29, &tx30,
  &tx31, &tx32, &tx33, &tx34, &tx35, &tx36, &tx37, &tx38, &tx39, &tx40,
  &tx41, &tx42, &tx43, &tx44, &tx45, &tx46, &tx47, &tx48, &tx49, &tx50
};

FieldSet menu(VERTICAL, 3, 7, 7, TBT, 50, BLUE);

Container* TMT[]{
  &board,
  &menu
};

FieldSet desktop(0, 0, ALIGN_CLIENT, ALIGN_CLIENT, TMT, 2, AQUA);

void tx_click(Container* c)
{
  sel01.setText(static_cast<TextButton*>(c)->getUtf8Text());
  desktop.draw();
}

void tx_btn(TextButton* b, const int id, const char* text)
{
  b->setId(id);
  b->setStaticText(text);
  b->setTextColor(GRAY);
  b->setFont(FreeSans_9pt);
  b->setFontSize(1);
  b->setBorderColor(GRAY);
  b->setEnabled();
  b->setOnClick(tx_click);
}

uint8_t LED_PIN = DEFAULT_LED_PIN;
int pin13state = LOW;

void setup()   {

  media_begin(ROTATION);

  pinMode(LED_PIN, OUTPUT);

  tx_btn(&tx01,  1, " 01. item  ");
  tx_btn(&tx02,  2, " 02. item");
  tx_btn(&tx03,  3, " 03. item");
  tx_btn(&tx04,  4, " 04. item");
  tx_btn(&tx05,  5, " 05. item");
  tx_btn(&tx06,  6, " 06. item");
  tx_btn(&tx07,  7, " 07. item");
  tx_btn(&tx08,  8, " 08. item");
  tx_btn(&tx09,  9, " 09. item");
  tx_btn(&tx10, 10, " 10. item");
  tx_btn(&tx11, 11, " 11. item");
  tx_btn(&tx12, 12, " 12. item");
  tx_btn(&tx13, 13, " 13. item");
  tx_btn(&tx14, 14, " 14. item");
  tx_btn(&tx15, 15, " 15. item");
  tx_btn(&tx16, 16, " 16. item");
  tx_btn(&tx17, 17, " 17. item");
  tx_btn(&tx18, 18, " 18. item");
  tx_btn(&tx19, 19, " 19. item");
  tx_btn(&tx20, 20, " 20. item");
  tx_btn(&tx21, 21, " 21. item");
  tx_btn(&tx22, 22, " 22. item");
  tx_btn(&tx23, 23, " 23. item");
  tx_btn(&tx24, 24, " 24. item");
  tx_btn(&tx25, 25, " 25. item");
  tx_btn(&tx26, 26, " 26. item");
  tx_btn(&tx27, 27, " 27. item");
  tx_btn(&tx28, 28, " 28. item");
  tx_btn(&tx29, 29, " 29. item");
  tx_btn(&tx30, 30, " 30. item");
  tx_btn(&tx31, 31, " 31. item");
  tx_btn(&tx32, 32, " 32. item");
  tx_btn(&tx33, 33, " 33. item");
  tx_btn(&tx34, 34, " 34. item");
  tx_btn(&tx35, 35, " 35. item");
  tx_btn(&tx36, 36, " 36. item");
  tx_btn(&tx37, 37, " 37. item");
  tx_btn(&tx38, 38, " 38. item");
  tx_btn(&tx39, 39, " 39. item");
  tx_btn(&tx40, 40, " 40. item");
  tx_btn(&tx41, 41, " 41. item");
  tx_btn(&tx42, 42, " 42. item");
  tx_btn(&tx43, 43, " 43. item");
  tx_btn(&tx44, 44, " 44. item");
  tx_btn(&tx45, 45, " 45. item");
  tx_btn(&tx46, 46, " 46. item");
  tx_btn(&tx47, 47, " 47. item");
  tx_btn(&tx48, 48, " 48. item");
  tx_btn(&tx49, 49, " 49. item");
  tx_btn(&tx50, 50, " 50. item");

  sel01.setAlignClientHoriz();
  sel01.setFontSize(2);

  board.setAlignClient();
  board.setAlignTop(true);

  menu.setAlignLeft(true);

  desktop.setMargin(10);
  desktop.setOrientation(HORIZONTAL, 10);

  screenEnabled = true;
  desktop.draw();
}

// ----------------------------------------------------------------

void loop()
{
  point_t p;
  if (touch.getTouch(&p)) {
    /*Container* b =*/ desktop.pressed(p.x, p.y);
  }

  sleep_ms(50);

  pin13state = 1 - pin13state;
  digitalWrite(LED_PIN, pin13state);
}

int main() {

  stdio_init_all();

  setup();

  while (true) {
    loop();
  }
}
// ----------------------------------------------------------------
