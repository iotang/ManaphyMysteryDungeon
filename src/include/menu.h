#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "appstate.h"
#include "utils.h"
#include "dungeon.h"

#define MaxFileNameLength (64)

void gotoNewPage();

void drawToolsBar() {
  static char *menuListEdit[] = {"Edit", "New", "Open", "Save | Ctrl-S",
                                 "Save as..."};

  double fontHeight = GetFontHeight();
  double x = 0, y = WindowHeightInch;
  double h = fontHeight * 1.5;
  double w = TextStringWidth(menuListEdit[0]) * 1.5;
  double wlist = TextStringWidth(menuListEdit[3]) * 1.2;
  int selection;

  selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListEdit,
                       sizeof(menuListEdit) / sizeof(menuListEdit[0]));

  if (selection == 1)
    gotoNewPage();

  static char *menuListRun[] = {"Run", "Start New Run", "Find Solution"};
  x += w;
  w = TextStringWidth(menuListRun[0]) * 1.5;
  wlist = TextStringWidth(menuListRun[1]) + 0.2;

  selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListRun,
                       sizeof(menuListRun) / sizeof(menuListRun[0]));

  static char *menuListAbout[] = {"About", "Help", "About"};
  x += w;
  w = TextStringWidth(menuListAbout[0]) * 1.5;
  wlist = TextStringWidth(menuListAbout[2]) + 0.2;

  selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListAbout,
                       sizeof(menuListAbout) / sizeof(menuListAbout[0]));
}

void drawMainMenu() {
  drawToolsBar();

  static char *title = "Manaphy's Mystery Dungeon";
  MovePen((WindowWidthInch - TextStringWidth(title)) / 2, WindowHeightInch / 2);
  SetPenColor("Blue");
  DrawTextString(title);
}

char dialogFileName[MaxFileNameLength + 1];

void initGetFileNameDialog(char *name) {
  if (strlen(name) > MaxFileNameLength) {
    name[MaxFileNameLength + 1] = 0;
  }
  strcpy(dialogFileName, name);
}

int drawGetFileNameDialog() {
  SetPenColor("Blue");
  SetPointSize(8);
  double xlen = WindowWidthInch / 3;
  double ylen = WindowHeightInch / 3;
  double xstart = (WindowWidthInch - xlen) / 2;
  double ystart = (WindowHeightInch - ylen) / 2;
  drawRectangle(xstart, ystart, xlen, ylen, 0);

  SetPenColor("Blue");
  MovePen(xstart + xlen * 0.1, ystart + ylen * 0.8);
  DrawTextString("Input File Name:");

  SetPenColor("Blue");
  double fontHeight = GetFontHeight();
  double h = fontHeight * 1.5;
  drawRectangle(1.1 * WindowWidthInch / 3, WindowHeightInch / 2, 0.8 * xlen, h,
                0);

  return textbox(GenUIID(0), 1.1 * WindowWidthInch / 3, WindowHeightInch / 2,
                 0.8 * xlen, h, dialogFileName, MaxFileNameLength);
}

extern Dungeon currentDungeon;

void initNewPage() { initGetFileNameDialog("dungeon.dun"); }

void drawNewPage() {
  drawGetFileNameDialog();

  double fontHeight = GetFontHeight();
  double h = fontHeight * 1.5;
  if (button(GenUIID(0), 1.5 * WindowWidthInch / 3, 1.2 * WindowHeightInch / 3,
             0.15 * WindowWidthInch / 3, h, "Cancel")) {
    smPopState();
  }

  if (button(GenUIID(1), 1.7 * WindowWidthInch / 3, 1.2 * WindowHeightInch / 3,
             0.15 * WindowWidthInch / 3, h, "Confirm")) {
    smPopState();
  }
}

void drawEditPage() {}

typedef enum { idMainMenu, idNewPage } AppStateID;

AppState MainMenu = {idMainMenu, NULL, drawMainMenu, NULL,
                     NULL,       NULL, uiGetMouse};

AppState NewPage = {idNewPage,     initNewPage, drawNewPage, NULL,
                    uiGetKeyboard, uiGetChar,   uiGetMouse};

void gotoNewPage() { smPushState(&NewPage); }