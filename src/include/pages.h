#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "appstate.h"
#include "statemanager.h"
#include "utils.h"
#include "dungeon.h"
#include "drawdungeon.h"
#include "control.h"

#define MaxFileNameLength (64)

void gotoOpenPage();
void gotoEditPage();

int isDungeonOpened = 0;

void drawToolsBar() {
  SetPointSize(16);
  static char *menuListEdit[] = {"Edit", "New", "Open", "Save | Ctrl-S",
                                 "Save as..."};

  double fontHeight = GetFontHeight();
  double x = 0, y = WindowHeightInch;
  double h = fontHeight * 1.5;
  double w = TextStringWidth(menuListEdit[0]) * 1.5;
  double wlist = TextStringWidth(menuListEdit[3]) * 1.2;
  int selection;

  selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListEdit,
                       sizeof(menuListEdit) / sizeof(menuListEdit[0]) -
                           (isDungeonOpened ? 0 : 2));

  if (selection == 2)
    gotoOpenPage();

  if (isDungeonOpened) {

    static char *menuListRun[] = {"Run", "Start New Run", "Find Solution"};
    x += w;
    w = TextStringWidth(menuListRun[0]) * 1.5;
    wlist = TextStringWidth(menuListRun[1]) + 0.2;

    selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListRun,
                         sizeof(menuListRun) / sizeof(menuListRun[0]));
  }

  static char *menuListAbout[] = {"About", "Help", "About"};
  x += w;
  w = TextStringWidth(menuListAbout[0]) * 1.5;
  wlist = TextStringWidth(menuListAbout[2]) + 0.2;

  selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListAbout,
                       sizeof(menuListAbout) / sizeof(menuListAbout[0]));
}

void drawMainMenu() {
  static char *title = "Manaphy's Mystery Dungeon";
  SetPointSize(64);
  SetPenColor("Blue");
  MovePen((WindowWidthInch - TextStringWidth(title)) / 2, WindowHeightInch / 2);
  DrawTextString(title);

  drawToolsBar();
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

  SetPointSize(16);
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

int isOpenPageFileExist;

void initOpenPage() {
  initGetFileNameDialog("dungeon.dun");
  isOpenPageFileExist = 1;
}

void drawOpenPage() {
  drawGetFileNameDialog();

  double fontHeight = GetFontHeight();
  double h = fontHeight * 1.5;

  if (!isOpenPageFileExist) {
    static char fileNotExistAlert[] = "Cannot open file";
    double length = TextStringWidth(fileNotExistAlert) * 1.2;
    drawBox(WindowWidthInch / 2 - length / 2, 1.4 * WindowHeightInch / 3,
            length, h, 0, fileNotExistAlert, 'C', "Red");
  }
  if (button(GenUIID(0), 1.5 * WindowWidthInch / 3, 1.2 * WindowHeightInch / 3,
             0.15 * WindowWidthInch / 3, h, "Cancel")) {
    smPopState();
  }

  if (button(GenUIID(1), 1.7 * WindowWidthInch / 3, 1.2 * WindowHeightInch / 3,
             0.15 * WindowWidthInch / 3, h, "Confirm")) {
    FILE *file = fopen(dialogFileName, "r");
    if (file == NULL)
      isOpenPageFileExist = 0;
    else {
      smPopState();
      loadDungeon(&currentDungeon, file);
      gotoEditPage();
    }
  }
}

double cellSize;
Dungeon editDungeon;
Pokemon manaphy;
double mousex, mousey;

typedef enum EditMode { Flip, SetPlain, SetBlock, Targeted } EditMode;

EditMode editMode;

void initEditPage() {
  cellSize = 1;
  editDungeon = currentDungeon;
  manaphy.x = manaphy.y = 0;
  int gotEnd = 0;
  for (int x = 0; x < editDungeon.width; x++) {
    for (int y = 0; y < editDungeon.height; y++) {
      if (editDungeon.mp[x][y] == Start) {
        manaphy.x = x;
        manaphy.y = y;
      }
      if (editDungeon.mp[x][y] == End) {
        gotEnd = 1;
      }
    }
  }

  editDungeon.mp[manaphy.x][manaphy.y] = Start;
  if (!gotEnd) {
    editDungeon.mp[editDungeon.width - 1][editDungeon.height - 1] = End;
  }

  editMode = Flip;
  isDungeonOpened = 1;
  bindingControl(&manaphy, &editDungeon);
}

void drawEditPage() {
  drawDungeon(&editDungeon, manaphy.x, manaphy.y, cellSize);
  drawDungeonCell(&editDungeon, manaphy.x, manaphy.y, cellSize, mousex, mousey);

  drawToolsBar();
}

void stopEditPage() { isDungeonOpened = 0; }

void uiEditPageGetKeyboard(int key, int event) {
  controlKeyboard(key, event);
  uiGetKeyboard(key, event);
}

void uiEditPageGetMouse(int x, int y, int button, int event) {
  mousex = ScaleXInches(x);
  mousey = ScaleYInches(y);

  if (event == BUTTON_DOWN) {
    int mx, my;
    getCellLocation(&editDungeon, manaphy.x, manaphy.y, cellSize, mousex,
                    mousey, &mx, &my);
    if (mx >= 0 && my >= 0) {
      if (editMode == Flip && (editDungeon.mp[mx][my] == Plain ||
                               editDungeon.mp[mx][my] == Block)) {
        editDungeon.mp[mx][my] =
            editDungeon.mp[mx][my] == Plain ? Block : Plain;
      }
    }
  }

  uiGetMouse(x, y, button, event);
}

typedef enum { idMainMenu, idOpenPage, idEditPage } AppStateID;

AppState MainMenu = {idMainMenu, NULL, drawMainMenu, NULL,
                     NULL,       NULL, uiGetMouse};

AppState OpenPage = {idOpenPage,    initOpenPage, drawOpenPage, NULL,
                     uiGetKeyboard, uiGetChar,    uiGetMouse};

AppState EditPage = {idEditPage,        initEditPage,          drawEditPage,
                     stopEditPage,      uiEditPageGetKeyboard, uiGetChar,
                     uiEditPageGetMouse};

void gotoOpenPage() { smPushState(&OpenPage); }

void gotoEditPage() { smPushState(&EditPage); }