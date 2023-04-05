#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "appstate.h"
#include "statemanager.h"
#include "utils.h"

int confirmArgc;
char *confirmArgv[99];
int *confirmed;

void setConfirmDialog1(int *value, char *argv0) {
  confirmed = value;
  *confirmed = -1;
  confirmArgc = 1;
  confirmArgv[0] = argv0;
}

void setConfirmDialog2(int *value, char *argv0, char *argv1) {
  confirmed = value;
  *confirmed = -1;
  confirmArgc = 2;
  confirmArgv[0] = argv0;
  confirmArgv[1] = argv1;
}
void setConfirmDialog3(int *value, char *argv0, char *argv1, char *argv2) {
  confirmed = value;
  *confirmed = -1;
  confirmArgc = 3;
  confirmArgv[0] = argv0;
  confirmArgv[1] = argv1;
  confirmArgv[2] = argv2;
}
void setConfirmDialog4(int *value, char *argv0, char *argv1, char *argv2,
                       char *argv3) {
  confirmed = value;
  *confirmed = -1;
  confirmArgc = 4;
  confirmArgv[0] = argv0;
  confirmArgv[1] = argv1;
  confirmArgv[2] = argv2;
  confirmArgv[3] = argv3;
}

void drawConfirmDialog() {
  SetPenColor("Red");
  double xlen = WindowWidthInch / 3;
  double ylen = WindowHeightInch / 3;
  double xstart = (WindowWidthInch - xlen) / 2;
  double ystart = (WindowHeightInch - ylen) / 2;
  drawRectangle(xstart, ystart, xlen, ylen, 0);

  double fontHeight = GetFontHeight();
  double h = fontHeight * 1.5;
  SetPointSize(16);
  SetPenColor("Red");
  for (int i = 0; i < confirmArgc; i++) {
    MovePen(xstart + xlen * 0.1, ystart + ylen * 0.8 - h * i);
    DrawTextString(confirmArgv[i]);
  }

  if (button(GenUIID(0), 1.4 * WindowWidthInch / 3, 1.2 * WindowHeightInch / 3,
             0.15 * WindowWidthInch / 3, h, "No")) {
    *confirmed = 0;
    smPopState();
  }

  if (button(GenUIID(0), 1.7 * WindowWidthInch / 3, 1.2 * WindowHeightInch / 3,
             0.15 * WindowWidthInch / 3, h, "Yes")) {
    *confirmed = 1;
    smPopState();
  }
}

AppState ConfirmDialog = {
    idConfirmDialog, NULL, drawConfirmDialog, NULL, NULL, NULL, uiGetMouse};

void gotoConfirmDialog() { smPushState(&ConfirmDialog); }