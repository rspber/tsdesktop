/*
  TouchScreenDesktop

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

/*
----------------------------------------------------------------
                        ValueButton Class
----------------------------------------------------------------
*/

#include "TSDesktop.h"
#include <stdio.h>

void ValueButton::setValue(const int aValue)
{
  char tmp[80];
  snprintf(tmp, sizeof(tmp), valueFormat, aValue);
  setText(tmp);
}

void ValueButton::setProp(const char* aName, const int aValue)
{
  char tmp[80];
  snprintf(tmp, sizeof(tmp), "%s: %d", aName, aValue);
  setText(tmp);
}

void ValueButton::setProp(const char* aName, const char* aValue)
{
  char tmp[80];
  snprintf(tmp, sizeof(tmp), "%s: %s", aName, aValue);
  setText(tmp);
}

