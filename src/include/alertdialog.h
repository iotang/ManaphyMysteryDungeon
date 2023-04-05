#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "appstate.h"
#include "statemanager.h"
#include "utils.h"

int alertArgc;
char *alertArgv[99];

void setAlertDialog1(char *argv0) {
  alertArgc = 1;
  alertArgv[0] = argv0;
}

void setAlertDialog2(char *argv0, char *argv1) {
  alertArgc = 2;
  alertArgv[0] = argv0;
  alertArgv[1] = argv1;
}
void setAlertDialog3(char *argv0, char *argv1, char *argv2) {
  alertArgc = 3;
  alertArgv[0] = argv0;
  alertArgv[1] = argv1;
  alertArgv[2] = argv2;
}
void setAlertDialog4(char *argv0, char *argv1, char *argv2, char *argv3) {
  alertArgc = 4;
  alertArgv[0] = argv0;
  alertArgv[1] = argv1;
  alertArgv[2] = argv2;
  alertArgv[3] = argv3;
}

void drawAlertDialog() {
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
  for (int i = 0; i < alertArgc; i++) {
    MovePen(xstart + xlen * 0.1, ystart + ylen * 0.8 - h * i);
    DrawTextString(alertArgv[i]);
  }

  if (button(GenUIID(0), 1.7 * WindowWidthInch / 3, 1.2 * WindowHeightInch / 3,
             0.15 * WindowWidthInch / 3, h, "OK")) {
    smPopState();
  }
}

AppState AlertDialog = {idAlertDialog, NULL, drawAlertDialog, NULL,
                        NULL,          NULL, uiGetMouse};

void gotoAlertDialog() { smPushState(&AlertDialog); }