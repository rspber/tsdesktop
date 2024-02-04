/*
  Menu 3 on TSDesktop

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

FieldSet menu(VERTICAL, 3, 7, 7, TBT, 50, ORANGE);

TextButton gap01;

Container* T2T[]{
  &gap01,
  &menu
};

FieldSet gmenu(VERTICAL, 3, 7, 7, T2T, 2, GREEN);

Container* TMT[]{
  &board,
  &gmenu
};

FieldSet div01(HORIZONTAL, 4, 0, 0, TMT, 2, AQUA);

Container* TDIV1T[]{
  &div01
};

FieldSet desktop(-1, -1, ALIGN_CLIENT, ALIGN_CLIENT, TDIV1T, 1, BLACK);

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

  tx_btn(&tx01,  1, " 01. menu item");
  tx_btn(&tx02,  2, " 02. menu item");
  tx_btn(&tx03,  3, " 03. menu item");
  tx_btn(&tx04,  4, " 04. menu item");
  tx_btn(&tx05,  5, " 05. menu item");
  tx_btn(&tx06,  6, " 06. menu item");
  tx_btn(&tx07,  7, " 07. menu item");
  tx_btn(&tx08,  8, " 08. menu item");
  tx_btn(&tx09,  9, " 09. menu item");
  tx_btn(&tx10, 10, " 10. menu item");
  tx_btn(&tx11, 11, " 11. menu item");
  tx_btn(&tx12, 12, " 12. menu item");
  tx_btn(&tx13, 13, " 13. menu item");
  tx_btn(&tx14, 14, " 14. menu item");
  tx_btn(&tx15, 15, " 15. menu item");
  tx_btn(&tx16, 16, " 16. menu item");
  tx_btn(&tx17, 17, " 17. menu item");
  tx_btn(&tx18, 18, " 18. menu item");
  tx_btn(&tx19, 19, " 19. menu item");
  tx_btn(&tx20, 20, " 20. menu item");
  tx_btn(&tx21, 21, " 21. menu item");
  tx_btn(&tx22, 22, " 22. menu item");
  tx_btn(&tx23, 23, " 23. menu item");
  tx_btn(&tx24, 24, " 24. menu item");
  tx_btn(&tx25, 25, " 25. menu item");
  tx_btn(&tx26, 26, " 26. menu item");
  tx_btn(&tx27, 27, " 27. menu item");
  tx_btn(&tx28, 28, " 28. menu item");
  tx_btn(&tx29, 29, " 29. menu item");
  tx_btn(&tx30, 30, " 30. menu item");
  tx_btn(&tx31, 31, " 31. menu item");
  tx_btn(&tx32, 32, " 32. menu item");
  tx_btn(&tx33, 33, " 33. menu item");
  tx_btn(&tx34, 34, " 34. menu item");
  tx_btn(&tx35, 35, " 35. menu item");
  tx_btn(&tx36, 36, " 36. menu item");
  tx_btn(&tx37, 37, " 37. menu item");
  tx_btn(&tx38, 38, " 38. menu item");
  tx_btn(&tx39, 39, " 39. menu item");
  tx_btn(&tx40, 40, " 40. menu item");
  tx_btn(&tx41, 41, " 41. menu item");
  tx_btn(&tx42, 42, " 42. menu item");
  tx_btn(&tx43, 43, " 43. menu item");
  tx_btn(&tx44, 44, " 44. menu item");
  tx_btn(&tx45, 45, " 45. menu item");
  tx_btn(&tx46, 46, " 46. menu item");
  tx_btn(&tx47, 47, " 47. menu item");
  tx_btn(&tx48, 48, " 48. menu item");
  tx_btn(&tx49, 49, " 49. menu item");
  tx_btn(&tx50, 50, " 50. menu item");

  sel01.setAlignClientHoriz();
  sel01.setFontSize(2);

  board.setWidth(100);
  board.setAlignClientVert();
  board.setAlignTop(true);
  board.setTransparent(true);

  gap01.setStaticText("Menu");
  gmenu.setAlignClientVert();
  gmenu.setTransparent(true);

  gmenu.setHeight(150);

  desktop.setMargin(5);
  desktop.setOrientation(HORIZONTAL, 10);

//  menu.disableScroll(SCROLL_PAGE | SCROLL_HOME);
  menu.disableScroll(SCROLL_HOME);

  div01.setTransparent(true);

  desktop.setTransparent(true);

  gradient_t g1{GNT_LR, RED, 100, BLUE};
  display.fillRectGradient(0, 0, 100, 220, g1);
  gradient_t g2{GNT_TB, CYAN, 30, BLACK};
  display.fillRectGradient(110, 0, 300, 100, g2);
  display.drawLine(0, 0, 500, 500, WHITE);
  display.drawLine(100, 0, 300, 400, WHITE);
  display.drawLine(200, 0, -100, 300, GRAY, 8);
  display.drawLine(300, 0, 100, 400, WHITE);

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

// ----------------------------------------------------------------
