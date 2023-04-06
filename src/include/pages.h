#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "appstate.h"
#include "statemanager.h"
#include "utils.h"
#include "dungeon.h"
#include "drawdungeon.h"
#include "controller.h"

#include "alertdialog.h"
#include "confirmdialog.h"
#include "getfilenamedialog.h"
#include "editpage.h"
#include "explorer.h"

#include "globalvalue.h"

int confirmedForceExit;

void drawToolsBar() {
  if (confirmedForceExit == 1) {
    smPopStateUntil(idMainMenu);
    confirmedForceExit = 0;
    return;
  }

  SetPointSize(16);
  static char *menuListEdit[] = {
      "Edit",       "New | Ctrl-N", "Open | Ctrl-O", "Save | Ctrl-S",
      "Save as...", "Randomize",    "Exit"};

  double fontHeight = GetFontHeight();
  double x = 0, y = WindowHeightInch;
  double h = fontHeight * 1.5;
  double w = TextStringWidth(menuListEdit[0]) * 1.5;
  double wlist = TextStringWidth(menuListEdit[3]) * 1.2;
  int selection;

  if (!isDungeonRunning) {
    selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListEdit,
                         sizeof(menuListEdit) / sizeof(menuListEdit[0]) -
                             (isDungeonOpened ? 0 : 4));

    if (selection == 1) {
      gotoNewPage();
    } else if (selection == 2) {
      gotoOpenPage();
    } else if (selection == 3) {
      if (saveDungeonEditPage() < 0) {
        setAlertDialog2("Error!", "Save failed");
        gotoAlertDialog();
      }
    } else if (selection == 4) {
      gotoSaveAsPage();
    } else if (selection == 5) {
      randomizeDungeon(&editDungeon);
      modifiedSinceLastSave = 1;
    } else if (selection == 6) {
      if (modifiedSinceLastSave) {
        setConfirmDialog3(&confirmedForceExit, "Warning",
                          "Are you sure to exit?",
                          "All unsaved changes will be lost!");
        gotoConfirmDialog();
      } else {
        smPopStateUntil(idMainMenu);
      }
    }
  } else {
    static char *menuListRunningEdit[] = {"Edit", "Quit Running"};

    selection =
        menuList(GenUIID(0), x, y - h, w, wlist, h, menuListRunningEdit,
                 sizeof(menuListRunningEdit) / sizeof(menuListRunningEdit[0]));
    if (selection == 1) {
      smPopState();
    }
  }

  if (isDungeonOpened) {

    static char *menuListRun[] = {"Run", "New Run | Ctrl-H", "Find Solution"};
    x += w;
    w = TextStringWidth(menuListRun[0]) * 1.5;
    wlist = TextStringWidth(menuListRun[1]) + 0.2;

    selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListRun,
                         sizeof(menuListRun) / sizeof(menuListRun[0]));

    if (selection == 1) {
      if (saveDungeonEditPage() < 0) {
        setAlertDialog2("Error!", "Save failed");
        gotoAlertDialog();
      } else {
        smPopStateUntil(idEditPage);
        gotoExplorer();
      }
    }
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
    if (file == NULL) {
      isOpenPageFileExist = 0;
    } else {
      smPopState();
      loadDungeon(&currentDungeon, file);
      strcpy(editDungeonFileName, dialogFileName);
      modifiedSinceLastSave = 0;
      gotoEditPage();
    }
  }
}

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
    FILE *file = fopen(dialogFileName, "r");
    if (file == NULL) {
      setDefaultDungeon(&currentDungeon);
      modifiedSinceLastSave = 1;
    } else {
      loadDungeon(&currentDungeon, file);
      modifiedSinceLastSave = 0;
    }
    smPopState();
    strcpy(editDungeonFileName, dialogFileName);
    gotoEditPage();
  }
}

AppState MainMenu = {idMainMenu,    NULL,      drawMainMenu, NULL,
                     uiGetKeyboard, uiGetChar, uiGetMouse};

AppState NewPage = {idNewPage,     initNewPage, drawNewPage, NULL,
                    uiGetKeyboard, uiGetChar,   uiGetMouse};

AppState OpenPage = {idOpenPage,    initOpenPage, drawOpenPage, NULL,
                     uiGetKeyboard, uiGetChar,    uiGetMouse};

void gotoNewPage() { smPushState(&NewPage); }

void gotoOpenPage() { smPushState(&OpenPage); }