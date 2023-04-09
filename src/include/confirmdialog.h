#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "appstate.h"
#include "statemanager.h"
#include "utils.h"

int confirmArgc;
char *confirmArgv[99];
voidFn confirmed;

void setConfirmDialog1(voidFn value, char *argv0) {
  confirmed = value;
  confirmArgc = 1;
  confirmArgv[0] = argv0;
}

void setConfirmDialog2(voidFn value, char *argv0, char *argv1) {
  confirmed = value;
  confirmArgc = 2;
  confirmArgv[0] = argv0;
  confirmArgv[1] = argv1;
}
void setConfirmDialog3(voidFn value, char *argv0, char *argv1, char *argv2) {
  confirmed = value;
  confirmArgc = 3;
  confirmArgv[0] = argv0;
  confirmArgv[1] = argv1;
  confirmArgv[2] = argv2;
}
void setConfirmDialog4(voidFn value, char *argv0, char *argv1, char *argv2,
                       char *argv3) {
  confirmed = value;
  confirmArgc = 4;
  confirmArgv[0] = argv0;
  confirmArgv[1] = argv1;
  confirmArgv[2] = argv2;
  confirmArgv[3] = argv3;
}

void drawConfirmDialog() {
  smLastProc();

  double xlen = WindowWidthInch / 3;
  double ylen = WindowHeightInch / 3;
  double xstart = (WindowWidthInch - xlen) / 2;
  double ystart = (WindowHeightInch - ylen) / 2;
  SetPenColor("White");
  drawRectangle(xstart, ystart, xlen, ylen, 1);
  SetPenColor("Red");
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
             0.15 * WindowWidthInch / 3, h, "No", idConfirmDialog)) {
    smPopState();
  }

  if (button(GenUIID(0), 1.7 * WindowWidthInch / 3, 1.2 * WindowHeightInch / 3,
             0.15 * WindowWidthInch / 3, h, "Yes", idConfirmDialog)) {
    smPopState();
    if (confirmed != NULL)
      confirmed();
  }
}

void uiConfirmDialogGetMouse(int x, int y, int button, int event) {
  if (smStateTop()->uid == idConfirmDialog)
    uiGetMouse(x, y, button, event);
}

AppState ConfirmDialog = {
    idConfirmDialog,        NULL, drawConfirmDialog, NULL, NULL, NULL,
    uiConfirmDialogGetMouse};

void gotoConfirmDialog() { smPushState(&ConfirmDialog); }