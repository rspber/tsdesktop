/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

#pragma once

#include "TSDesktop.h"

/// @Editable

class Editable : public TextButton {
public:
   Editable(const int aBufSize)
      : TextButton("", -1, -1, ALIGN_CLIENT, ALIGN_COMPACT)
   {
      bufSize = aBufSize;
      bufp = new char[aBufSize];
      bufp[0] = '\0';

      setEnabled();
   }

   virtual const char* getText() { return bufp; }

   void setCursorAllow(const bool aCursorAllow);
   void setCursor(const int aCursor);
   void setCursorToEnd();
   void moveCursor(const int8_t aDirection);
   void setCursorColor(const uint16_t aCursorColor);
   void setCursorInsMode(const bool aCursorInsMode);
   void swapCursorInsMode() { setCursorInsMode(!cursorInsMode); }

   const bool getCursorAllow() { return cursorAllow; }
   const int getCursor() { return textCursor; }
   const uint16_t getCursorColor() { return cursorColor; }
   const bool getCursorInsMode() { return cursorInsMode; }

   void cmdInsChar(const char aChar);
   void cmdDelBS(const bool BS);
   void cmdBS() { cmdDelBS(true); }
   void cmdDel() { cmdDelBS(false); }

protected:
   virtual void innerSetText(const char* aText);

   virtual void hideText();
   virtual void drawText();

private:
   virtual void clickEffect(const int16_t posX, const int16_t posY);

   void drawCursor(const uint16_t aCursorColor);
   void drawCursor();

   void hideCursor();

private:
   int bufSize = 0;
   char* bufp;
   int textCursor = 0;
   bool cursorAllow = true;
   bool cursorInsMode = true;   // insert chars vs overwrite
   uint16_t cursorColor = RED;
};

