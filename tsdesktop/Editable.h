/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include "TSDesktop.h"

/// @Editable

class Editable : public TextButton {
public:
   Editable()
      : TextButton("", -1, -1, ALIGN_CLIENT, ALIGN_COMPACT)
   {
      setEnabled();
   }

   void setCursorAllow(const bool aCursorAllow);
   void setCursor(const int aCursor);
   void setCursorToEnd();
   void moveCursor(const int8_t aDirection);
   void setCursorColor(const rgb_t aCursorColor);
   void setCursorInsMode(const bool aCursorInsMode);
   void swapCursorInsMode() { setCursorInsMode(!cursorInsMode); }

   const bool getCursorAllow() { return cursorAllow; }
   const int getCursor() { return textCursor; }
   const rgb_t getCursorColor() { return cursorColor; }
   const bool getCursorInsMode() { return cursorInsMode; }

   /**
    * cmdInsChar - insert unicode char
   */
   void cmdInsChar(const uint16_t aChar) {
     textToUnicode();
     cmdInsChar_(aChar);
   }

   /**
    * cmdInsChar - insert utf-8 char
   */
   void cmdInsChar(const char aChar) {
      cmdInsChar_(aChar);
   }

   void cmdDelBS(const bool BS);
   void cmdBS() { cmdDelBS(true); }
   void cmdDel() { cmdDelBS(false); }

protected:
   void hideText() override;
   void drawText() override;

private:
   void cmdInsChar_(const uint16_t aChar);
   void innerSetText(const void* aText, const bool aUnicode, const bool temp) override;

   void clickEffect(const int16_t posX, const int16_t posY) override;

   void drawCursor(const rgb_t aCursorColor);
   void drawCursor();

   void hideCursor();

private:
   int textCursor = 0;
   bool cursorAllow = true;
   bool cursorInsMode = true;   // insert chars vs overwrite
   rgb_t cursorColor = RED;
};

