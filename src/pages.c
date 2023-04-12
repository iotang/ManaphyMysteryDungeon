#include "pages.h"

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "statemanager.h"
#include "utils.h"
#include "dungeon.h"
#include "drawdungeon.h"
#include "controller.h"
#include "imagesupport.h"

#include "alertdialog.h"
#include "confirmdialog.h"
#include "getfilenamedialog.h"
#include "editpage.h"
#include "explorer.h"
#include "simpage.h"
#include "aboutpage.h"
#include "helppage.h"

#include "globalvalue.h"

void callingExitWarning(voidFn nex) {
  if (modifiedSinceLastSave) {
    setConfirmDialog3(nex, "Warning", "Are you sure to exit?",
                      "All unsaved changes will be lost!");
    gotoConfirmDialog();
  } else {
    if (nex != NULL)
      nex();
  }
}

void smPopStateUntilMainMenu() { smPopStateUntil(idMainMenu); }

void drawToolsBar() {
  if (smStateTop()->uid == idPausePage)
    return;

  SetPointSize(16);
  double x = 0, y = WindowHeightInch;
  double h = MenuHeight;

  if (smStateTop()->uid == idEditPage || smStateTop()->uid == idMainMenu) {
    static char *menuListEdit[] = {
        "Edit",       "New | Ctrl-N", "Open | Ctrl-O", "Save | Ctrl-S",
        "Save as...", "Randomize",    "Exit"};

    double w = TextStringWidth(menuListEdit[0]) * 1.5;
    double wlist = TextStringWidth(menuListEdit[3]) * 1.2;
    int selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListEdit,
                             sizeof(menuListEdit) / sizeof(menuListEdit[0]) -
                                 (isDungeonOpened ? 0 : 4));

    if (selection == 1) {
      callingExitWarning(gotoNewPage);
    } else if (selection == 2) {
      callingExitWarning(gotoOpenPage);
    } else if (selection == 3) {
      if (saveDungeonEditPage() < 0) {
        setAlertDialog2("Error!", "Save failed");
        gotoAlertDialog();
      }
    } else if (selection == 4) {
      gotoSaveAsPage();
    } else if (selection == 5) {
      randomizeEditDungeon();
      modifiedSinceLastSave = 1;
    } else if (selection == 6) {
      callingExitWarning(smPopStateUntilMainMenu);
    }
    x += w;
  } else if (smStateTop()->uid == idExplorer ||
             smStateTop()->uid == idSimPage) {
    static char *menuListRunningEdit[] = {"Edit", "Quit Running"};

    double w = TextStringWidth(menuListRunningEdit[0]) * 1.5;
    double wlist = TextStringWidth(menuListRunningEdit[1]) * 1.2;
    int selection =
        menuList(GenUIID(0), x, y - h, w, wlist, h, menuListRunningEdit,
                 sizeof(menuListRunningEdit) / sizeof(menuListRunningEdit[0]));
    if (selection == 1) {
      smPopState();
    }
    x += w;
  }

  if (isDungeonOpened) {
    if (smStateTop()->uid == idEditPage) {
      static char *menuListRun[] = {"Run", "Explore | Ctrl-H", "Auto Run",
                                    "Limited Solution | Ctrl-Q",
                                    "Find Solution | Ctrl-F"};
      double w = TextStringWidth(menuListRun[0]) * 1.5;
      double wlist = TextStringWidth(menuListRun[3]) + 0.2;

      int selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListRun,
                               sizeof(menuListRun) / sizeof(menuListRun[0]));

      if (selection == 1) {
        if (saveDungeonEditPage() < 0) {
          setAlertDialog2("Error!", "Save failed");
          gotoAlertDialog();
        } else {
          smPopStateUntil(idEditPage);
          gotoExplorer();
        }
      } else if (selection == 2) {
        if (saveDungeonEditPage() < 0) {
          setAlertDialog2("Error!", "Save failed");
          gotoAlertDialog();
        } else {
          smPopStateUntil(idEditPage);
          gotoSimPage();
        }
      } else if (selection == 3) {
        editGetSolutionWithLimit();
      } else if (selection == 4) {
        editGetSolution();
      }

      x += w;
    } else if (smStateTop()->uid == idSimPage) {
      static char *menuListRun[] = {"Run", "Explore | Ctrl-H", "Auto Run"};
      double w = TextStringWidth(menuListRun[0]) * 1.5;
      double wlist = TextStringWidth(menuListRun[1]) + 0.2;

      int selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListRun,
                               sizeof(menuListRun) / sizeof(menuListRun[0]));

      if (selection == 1) {
        if (saveDungeonEditPage() < 0) {
          setAlertDialog2("Error!", "Save failed");
          gotoAlertDialog();
        } else {
          smPopStateUntil(idEditPage);
          gotoExplorer();
        }
      } else if (selection == 2) {
        if (saveDungeonEditPage() < 0) {
          setAlertDialog2("Error!", "Save failed");
          gotoAlertDialog();
        } else {
          smPopStateUntil(idEditPage);
          gotoSimPage();
        }
      }

      x += w;
    } else if (smStateTop()->uid == idExplorer) {
      static char *menuListRun[] = {"Run", "Explore | Ctrl-H", "Auto Run",
                                    "Cheat | Ctrl-P"};
      double w = TextStringWidth(menuListRun[0]) * 1.5;
      double wlist = TextStringWidth(menuListRun[1]) + 0.2;

      int selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListRun,
                               sizeof(menuListRun) / sizeof(menuListRun[0]));

      if (selection == 1) {
        if (saveDungeonEditPage() < 0) {
          setAlertDialog2("Error!", "Save failed");
          gotoAlertDialog();
        } else {
          smPopStateUntil(idEditPage);
          gotoExplorer();
        }
      } else if (selection == 2) {
        if (saveDungeonEditPage() < 0) {
          setAlertDialog2("Error!", "Save failed");
          gotoAlertDialog();
        } else {
          smPopStateUntil(idEditPage);
          gotoSimPage();
        }
      } else if (selection == 3) {
        giveCheat();
      }

      x += w;
    }
  }

  if (1) {
    static char *menuListAbout[] = {"More", "Help", "About"};
    double w = TextStringWidth(menuListAbout[0]) * 1.5;
    double wlist = TextStringWidth(menuListAbout[2]) + 0.2;

    int selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListAbout,
                             sizeof(menuListAbout) / sizeof(menuListAbout[0]));
    if (selection == 1) {
      gotoHelpPage();
    } else if (selection == 2) {
      gotoAboutPage();
    }
    x += w;
  }
}

void drawMainMenu() {
  SetPointSize(64);
  SetPenColor("Blue");
  MovePen((WindowWidthInch - TextStringWidth(ProjectName)) / 2,
          WindowHeightInch / 2);
  DrawTextString(ProjectName);

  HBITMAP bitmap = readBmpImage("assets/sprites/0490D.bmp");
  drawBmp(bitmap, WindowWidthInch / 2, WindowHeightInch / 2, 2, 2, SRCAND);

  drawToolsBar();
}

int isOpenPageFileExist;

void initOpenPage() {
  initGetFileNameDialog("dungeon.dun");
  isOpenPageFileExist = 1;
}

void drawOpenPage() {
  smLastProc();
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
             0.15 * WindowWidthInch / 3, h, "Cancel", idOpenPage)) {
    smPopState();
  }

  if (button(GenUIID(1), 1.7 * WindowWidthInch / 3, 1.2 * WindowHeightInch / 3,
             0.15 * WindowWidthInch / 3, h, "Confirm", idOpenPage)) {
    FILE *file = fopen(dialogFileName, "r");
    if (file == NULL) {
      isOpenPageFileExist = 0;
    } else {
      smPopState();
      while (!smIsStateEmpty() && smStateTop()->uid == idEditPage)
        smPopState();
      loadDungeon(&currentDungeon, file);
      sortDungeon(&currentDungeon);
      strcpy(editDungeonFileName, dialogFileName);
      modifiedSinceLastSave = 0;
      gotoEditPage();
    }
  }
}

void initNewPage() { initGetFileNameDialog("dungeon.dun"); }

void drawNewPage() {
  smLastProc();
  drawGetFileNameDialog();

  double fontHeight = GetFontHeight();
  double h = fontHeight * 1.5;

  if (button(GenUIID(0), 1.5 * WindowWidthInch / 3, 1.2 * WindowHeightInch / 3,
             0.15 * WindowWidthInch / 3, h, "Cancel", idNewPage)) {
    smPopState();
  }

  if (button(GenUIID(1), 1.7 * WindowWidthInch / 3, 1.2 * WindowHeightInch / 3,
             0.15 * WindowWidthInch / 3, h, "Confirm", idNewPage)) {
    FILE *file = fopen(dialogFileName, "r");
    if (file == NULL) {
      setDefaultDungeon(&currentDungeon);
      modifiedSinceLastSave = 1;
    } else {
      loadDungeon(&currentDungeon, file);
      sortDungeon(&currentDungeon);
      modifiedSinceLastSave = 0;
    }
    smPopState();
    while (!smIsStateEmpty() && smStateTop()->uid == idEditPage)
      smPopState();
    strcpy(editDungeonFileName, dialogFileName);
    gotoEditPage();
  }
}

void uiMainMenuGetKeyboard(int key, int event) {
  if (smStateTop()->uid == idMainMenu)
    uiGetKeyboard(key, event);
}

void uiMainMenuGetChar(int ch) {
  if (smStateTop()->uid == idMainMenu)
    uiGetChar(ch);
}

void uiMainMenuGetMouse(int x, int y, int button, int event) {
  if (smStateTop()->uid == idMainMenu)
    uiGetMouse(x, y, button, event);
}

void uiNewPageGetKeyboard(int key, int event) {
  if (smStateTop()->uid == idNewPage)
    uiGetKeyboard(key, event);
}

void uiNewPageGetChar(int ch) {
  if (smStateTop()->uid == idNewPage)
    uiGetChar(ch);
}

void uiNewPageGetMouse(int x, int y, int button, int event) {
  if (smStateTop()->uid == idNewPage)
    uiGetMouse(x, y, button, event);
}

void uiOpenPageGetKeyboard(int key, int event) {
  if (smStateTop()->uid == idOpenPage)
    uiGetKeyboard(key, event);
}

void uiOpenPageGetChar(int ch) {
  if (smStateTop()->uid == idOpenPage)
    uiGetChar(ch);
}

void uiOpenPageGetMouse(int x, int y, int button, int event) {
  if (smStateTop()->uid == idOpenPage)
    uiGetMouse(x, y, button, event);
}

AppState MainMenu = {idMainMenu,
                     NULL,
                     drawMainMenu,
                     NULL,
                     uiMainMenuGetKeyboard,
                     uiMainMenuGetChar,
                     uiMainMenuGetMouse};

AppState NewPage = {
    idNewPage,        initNewPage,      drawNewPage, NULL, uiNewPageGetKeyboard,
    uiNewPageGetChar, uiNewPageGetMouse};

AppState OpenPage = {idOpenPage,
                     initOpenPage,
                     drawOpenPage,
                     NULL,
                     uiOpenPageGetKeyboard,
                     uiOpenPageGetChar,
                     uiOpenPageGetMouse};

void gotoNewPage() { smPushState(&NewPage); }

void gotoOpenPage() { smPushState(&OpenPage); }