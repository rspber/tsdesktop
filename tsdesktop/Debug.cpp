/*
  Copyright (c) 2022, rspber (https://github.com/rspber)
*/

#include "Debug.h"

const char* TP[]{ "?", "B", "T", "C", "R", "F" };

#define DBN  1
#define BS   40
char DBTT[DBN][BS];
char* DBT[DBN];
int dbi = 0;

char* dcurr()
{
  if (dbi == 0) {
    for (int i = 0; i < DBN; ++i) {
      DBT[i] = DBTT[i];
    }
  }
  if (dbi < DBN) {
    return DBT[dbi++];
  }
  else {
    char* tmp = DBT[0];
    for (int i = 1; i < DBN; ++i) {
      DBT[i - 1] = DBT[i];
    }
    dbi = DBN - 1;
    DBT[dbi++] = tmp;
    return tmp;
  }
}

void debug(Container* c, const char* t)
{
  if (screenEnabled) {
    char* buf = dcurr();
    snprintf(buf, BS, "%s,%d,%d %3d,%3d,%3d,%3d %s", TP[c->getType()], c->getDeep(), c->getId(), c->getUpdLeft(), c->getUpdTop(), c->getUpdWidth(), c->getUpdHeight(), t);
    //        snprintf(buf, BS, "%s %d %3d,%3d %3d,%3d %s", TP[c->getType()], c->getDeep(), c->getOrgWidth(), c->getOrgHeight(), c->getUpdWidth(), c->getUpdHeight(), t);
    display.fillRect(0, 0, 240, DBN * 8, BLACK);
    for (int i = 0; i < DBN; ++i) {
      display.drawText(0, i * 8, DBT[i], 1, WHITE);
    }
    sleep_ms(20);
  }
}

void debug(Container* c, const char* t, const char* v)
{
  char buf[40];
  snprintf(buf, sizeof(buf), "%s: %s", t, v);
  debug(c, buf);
}

void debug(Container* c, const char* t, const int v)
{
  char buf[16];
  snprintf(buf, sizeof(buf), "%s: %d", t, v);
  debug(c, buf);
}

void debug(Container* c, const char* t, const bool v)
{
  char buf[16];
  snprintf(buf, sizeof(buf), "%s: %s", t, v ? "T" : "F");
  debug(c, buf);
}

void debug(Container* c, const char* t, const int v1, const int v2)
{
  char buf[16];
  snprintf(buf, sizeof(buf), "%s: %d, %d", t, v1, v2);
  debug(c, buf);
}
