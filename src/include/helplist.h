#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#define MaxHelpListLength (20)

typedef struct HelpEntry {
  char *fun;
  char *key;
} HelpEntry;

HelpEntry helpList[MaxHelpListLength];
int helpListLength;

void clearHelpList() { helpListLength = 0; }

void addHelpEntry(char *fun, char *key) {
  helpList[helpListLength].fun = fun;
  helpList[helpListLength].key = key;
  helpListLength++;
}

void drawHelpList(double basex, double topy) {
  int _pointSize = GetPointSize();
  SetPointSize(8);
  double fontHeight = GetFontHeight();
  double h = fontHeight * 1.5;
  for (int i = 0; i < helpListLength; i++) {
    double baseHeight = topy - (i + 1) * h;
    drawBoxWithoutBorder(basex + Window43Gap * 0.05, baseHeight,
                         Window43Gap * 0.90, h, 0, helpList[i].fun, 'L',
                         "Black");
    drawBoxWithoutBorder(basex + Window43Gap * 0.05, baseHeight,
                         Window43Gap * 0.90, h, 0, helpList[i].key, 'R',
                         "Black");
  }
  SetPointSize(_pointSize);
}