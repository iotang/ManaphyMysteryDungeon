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

#define MaxFileNameLength (20)

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

  if (selection == 1) {
    setAlertDialog1("Undone");
    gotoAlertDialog();
  }

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
char editDungeonFileName[MaxFileNameLength + 1];

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
      strcpy(editDungeonFileName, dialogFileName);
      gotoEditPage();
    }
  }
}

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

double cellSize;
Dungeon editDungeon;
Pokemon manaphy;
double mousex, mousey;

typedef enum EditMode {
  Flip,
  SetPlain,
  SetBlock,
  Targeted,
  PlaceStart,
  PlaceEnd
} EditMode;

EditMode editMode;

int isMouseDownEditPage;
int isJumpedEditPage;

void playerMoveEditPage(int event) {
  if (event == MoveRight) {
    if (manaphy.x + 1 < editDungeon.width) {
      manaphy.x++;
    }
  }
  if (event == MoveUp) {
    if (manaphy.y + 1 < editDungeon.height) {
      manaphy.y++;
    }
  }
  if (event == MoveLeft) {
    if (manaphy.x > 0) {
      manaphy.x--;
    }
  }
  if (event == MoveDown) {
    if (manaphy.y > 0) {
      manaphy.y--;
    }
  }
}

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
  isMouseDownEditPage = 0;
  isJumpedEditPage = 0;
  bindPlayerMove(playerMoveEditPage);
}

void drawEditPage() {
  drawDungeon(&editDungeon, manaphy.x, manaphy.y, cellSize);
  drawDungeonCell(&editDungeon, manaphy.x, manaphy.y, cellSize, mousex, mousey);

  // title

  SetPenColor("Cyan");
  drawRectangle(0, 0, Window43Left, WindowHeightInch, 1);

  SetPenColor("White");
  drawBox(WindowWidthInch * 0.01, WindowHeightInch * 0.85,
          Window43Left - WindowWidthInch * 0.02, WindowHeightInch * 0.05, 1,
          editDungeonFileName, 'C', "Black");

  // width height adj

  // edit mode

  SetPenColor("Cyan");
  drawRectangle(Window43Right, 0, Window43Gap, WindowHeightInch, 1);

  setButtonColors(editMode == Targeted ? "Yellow" : "White", "Blue", "Blue",
                  "White", 1);
  if (button(GenUIID(0), Window43Right + Window43Gap * 0.1,
             WindowHeightInch * 0.9, Window43Gap * 0.8, WindowHeightInch * 0.04,
             "Event")) {
    editMode = Targeted;
  }

  setButtonColors(editMode == Flip ? "Yellow" : "White", "Blue", "Blue",
                  "White", 1);
  if (button(GenUIID(0), Window43Right + Window43Gap * 0.1,
             WindowHeightInch * 0.85, Window43Gap * 0.8,
             WindowHeightInch * 0.04, "Flip")) {
    editMode = Flip;
  }

  setButtonColors(editMode == SetPlain ? "Yellow" : "White", "Blue", "Blue",
                  "White", 1);
  if (button(GenUIID(0), Window43Right + Window43Gap * 0.1,
             WindowHeightInch * 0.8, Window43Gap * 0.8, WindowHeightInch * 0.04,
             "Plain")) {
    editMode = SetPlain;
  }

  setButtonColors(editMode == SetBlock ? "Yellow" : "White", "Blue", "Blue",
                  "White", 1);
  if (button(GenUIID(0), Window43Right + Window43Gap * 0.1,
             WindowHeightInch * 0.75, Window43Gap * 0.8,
             WindowHeightInch * 0.04, "Block")) {
    editMode = SetBlock;
  }

  setButtonColors(editMode == PlaceStart ? "Yellow" : "White", "Blue", "Blue",
                  "White", 1);
  if (button(GenUIID(0), Window43Right + Window43Gap * 0.1,
             WindowHeightInch * 0.7, Window43Gap * 0.8, WindowHeightInch * 0.04,
             "Starts at")) {
    editMode = PlaceStart;
  }

  setButtonColors(editMode == PlaceEnd ? "Cyan" : "White", "Blue", "Blue",
                  "White", 1);
  if (button(GenUIID(0), Window43Right + Window43Gap * 0.1,
             WindowHeightInch * 0.65, Window43Gap * 0.8,
             WindowHeightInch * 0.04, "Ends at")) {
    editMode = PlaceEnd;
  }

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

  int mx, my;
  getCellLocation(&editDungeon, manaphy.x, manaphy.y, cellSize, mousex, mousey,
                  &mx, &my);
  if (event == MOUSEMOVE) {
    if (isMouseDownEditPage && mx >= 0 && my >= 0) {
      if (editMode == SetPlain && editDungeon.mp[mx][my] == Block) {
        editDungeon.mp[mx][my] = Plain;
      } else if (editMode == SetBlock && editDungeon.mp[mx][my] == Plain) {
        editDungeon.mp[mx][my] = Block;
      }
    }
  } else if (event == BUTTON_DOWN && button == LEFT_BUTTON) {
    isMouseDownEditPage = 1;
    if (mx >= 0 && my >= 0) {
      if (editMode == Flip && (editDungeon.mp[mx][my] == Plain ||
                               editDungeon.mp[mx][my] == Block)) {
        editDungeon.mp[mx][my] =
            editDungeon.mp[mx][my] == Plain ? Block : Plain;
      } else if (editMode == SetPlain && editDungeon.mp[mx][my] == Block) {
        editDungeon.mp[mx][my] = Plain;
      } else if (editMode == SetBlock && editDungeon.mp[mx][my] == Plain) {
        editDungeon.mp[mx][my] = Block;
      } else if (editMode == PlaceStart) {
        setDungeonStart(&editDungeon, mx, my);
      } else if (editMode == PlaceEnd) {
        setDungeonEnd(&editDungeon, mx, my);
      }
    }
  } else if (event == BUTTON_DOWN && button == RIGHT_BUTTON) {
    if (!isJumpedEditPage && mx >= 0 && my >= 0) {
      manaphy.x = mx;
      manaphy.y = my;
      isJumpedEditPage = 1;
    }
  } else if (event == BUTTON_UP && button == LEFT_BUTTON) {
    isMouseDownEditPage = 0;
  } else if (event == BUTTON_UP && button == RIGHT_BUTTON) {
    isJumpedEditPage = 0;
  } else if (event == ROLL_DOWN) {
    if (cellSize > 0.50) {
      cellSize -= 0.10;
    }
  } else if (event == ROLL_UP) {
    if (cellSize < 3.00) {
      cellSize += 0.10;
    }
  }

  uiGetMouse(x, y, button, event);
}

typedef enum { idMainMenu, idAlertDialog, idOpenPage, idEditPage } AppStateID;

AppState MainMenu = {idMainMenu, NULL, drawMainMenu, NULL,
                     NULL,       NULL, uiGetMouse};

AppState AlertDialog = {idAlertDialog, NULL, drawAlertDialog, NULL,
                        NULL,          NULL, uiGetMouse};

AppState OpenPage = {idOpenPage,    initOpenPage, drawOpenPage, NULL,
                     uiGetKeyboard, uiGetChar,    uiGetMouse};

AppState EditPage = {idEditPage,        initEditPage,          drawEditPage,
                     stopEditPage,      uiEditPageGetKeyboard, uiGetChar,
                     uiEditPageGetMouse};

void gotoAlertDialog() { smPushState(&AlertDialog); }

void gotoOpenPage() { smPushState(&OpenPage); }

void gotoEditPage() { smPushState(&EditPage); }