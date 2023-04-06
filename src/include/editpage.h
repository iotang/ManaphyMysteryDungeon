#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "appstate.h"
#include "statemanager.h"
#include "utils.h"
#include "pokemon.h"
#include "dungeon.h"
#include "drawdungeon.h"
#include "controller.h"

#include "alertdialog.h"
#include "getfilenamedialog.h"

#include "globalvalue.h"

int editHasReadDungeon;
Dungeon editDungeon;
char editDungeonFileName[MaxFileNameLength + 1];

double editCellSize;
Pokemon editCursor;
double editMouseX, editMouseY;

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
    if (editCursor.x + 1 < editDungeon.width) {
      editCursor.x++;
    }
  }
  if (event == MoveUp) {
    if (editCursor.y + 1 < editDungeon.height) {
      editCursor.y++;
    }
  }
  if (event == MoveLeft) {
    if (editCursor.x > 0) {
      editCursor.x--;
    }
  }
  if (event == MoveDown) {
    if (editCursor.y > 0) {
      editCursor.y--;
    }
  }
}

int saveDungeonEditPage() {
  FILE *file = fopen(editDungeonFileName, "w");
  if (file == NULL) {
    return -1;
  }

  currentDungeon = editDungeon;
  saveDungeon(&currentDungeon, file);
  modifiedSinceLastSave = 0;
}

void initEditPage() {
  editCellSize = 1;
  if (!editHasReadDungeon) {
    editDungeon = currentDungeon;
    editHasReadDungeon = 1;
  }
  editCursor.x = editCursor.y = 0;
  int gotEnd = 0;
  for (int x = 0; x < editDungeon.width; x++) {
    for (int y = 0; y < editDungeon.height; y++) {
      if (editDungeon.mp[x][y] == Start) {
        editCursor.x = x;
        editCursor.y = y;
      }
      if (editDungeon.mp[x][y] == End) {
        gotEnd = 1;
      }
    }
  }

  editDungeon.mp[editCursor.x][editCursor.y] = Start;
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
  if (editCursor.x < 0)
    editCursor.x = 0;
  if (editCursor.x >= editDungeon.width)
    editCursor.x = editDungeon.width - 1;

  if (editCursor.y < 0)
    editCursor.y = 0;
  if (editCursor.y >= editDungeon.height)
    editCursor.y = editDungeon.height - 1;

  drawDungeon(&editDungeon, editCursor.x, editCursor.y, editCellSize, 1);
  drawDungeonCell(&editDungeon, editCursor.x, editCursor.y, editCellSize,
                  editMouseX, editMouseY);

  // title
  /*
  SetPenColor("White");
  drawBox(Window43Left, WindowHeightInch * 0.97, Window43Right - Window43Left,
          WindowHeightInch * 0.03, 1, NULL, 'L', "Black");
  */

  SetPenColor(modifiedSinceLastSave ? "Yellow" : "White");
  drawBox(Window43Left, 0, Window43Right - Window43Left,
          WindowHeightInch * 0.03, 1, editDungeonFileName, 'L', "Black");

  SetPenColor("Cyan");
  drawRectangle(0, 0, Window43Left, WindowHeightInch, 1);

  // width height adj

  char __buf[99];
  sprintf(__buf, "%d", editDungeon.width);
  drawBoxWithoutBorder(Window43Gap * 0.4, WindowHeightInch * 0.23,
                       Window43Gap * 0.2, WindowHeightInch * 0.03, 1, "Width",
                       'C', "Black");
  SetPenColor("White");
  drawBox(Window43Gap * 0.4, WindowHeightInch * 0.19, Window43Gap * 0.2,
          WindowHeightInch * 0.03, 1, __buf, 'C', "Black");
  setButtonColors("White", "Blue", "Blue", "White", 1);
  if (button(GenUIID(0), Window43Gap * 0.64, WindowHeightInch * 0.19,
             Window43Gap * 0.1, WindowHeightInch * 0.03, "+")) {
    setDungeonSize(&editDungeon, editDungeon.width + 1, editDungeon.height);
    modifiedSinceLastSave = 1;
  }
  if (button(GenUIID(0), Window43Gap * 0.78, WindowHeightInch * 0.19,
             Window43Gap * 0.18, WindowHeightInch * 0.03, "+10")) {
    setDungeonSize(&editDungeon, editDungeon.width + 10, editDungeon.height);
    modifiedSinceLastSave = 1;
  }
  if (button(GenUIID(0), Window43Gap * 0.26, WindowHeightInch * 0.19,
             Window43Gap * 0.1, WindowHeightInch * 0.03, "-")) {
    setDungeonSize(&editDungeon, editDungeon.width - 1, editDungeon.height);
    modifiedSinceLastSave = 1;
  }
  if (button(GenUIID(0), Window43Gap * 0.04, WindowHeightInch * 0.19,
             Window43Gap * 0.18, WindowHeightInch * 0.03, "-10")) {
    setDungeonSize(&editDungeon, editDungeon.width - 10, editDungeon.height);
    modifiedSinceLastSave = 1;
  }

  sprintf(__buf, "%d", editDungeon.height);
  drawBoxWithoutBorder(Window43Gap * 0.4, WindowHeightInch * 0.14,
                       Window43Gap * 0.2, WindowHeightInch * 0.03, 1, "Height",
                       'C', "Black");
  SetPenColor("White");
  drawBox(Window43Gap * 0.4, WindowHeightInch * 0.1, Window43Gap * 0.2,
          WindowHeightInch * 0.03, 1, __buf, 'C', "Black");
  setButtonColors("White", "Blue", "Blue", "White", 1);
  if (button(GenUIID(0), Window43Gap * 0.64, WindowHeightInch * 0.1,
             Window43Gap * 0.1, WindowHeightInch * 0.03, "+")) {
    setDungeonSize(&editDungeon, editDungeon.width, editDungeon.height + 1);
    modifiedSinceLastSave = 1;
  }
  if (button(GenUIID(0), Window43Gap * 0.78, WindowHeightInch * 0.1,
             Window43Gap * 0.18, WindowHeightInch * 0.03, "+10")) {
    setDungeonSize(&editDungeon, editDungeon.width, editDungeon.height + 10);
    modifiedSinceLastSave = 1;
  }
  if (button(GenUIID(0), Window43Gap * 0.26, WindowHeightInch * 0.1,
             Window43Gap * 0.1, WindowHeightInch * 0.03, "-")) {
    setDungeonSize(&editDungeon, editDungeon.width, editDungeon.height - 1);
    modifiedSinceLastSave = 1;
  }
  if (button(GenUIID(0), Window43Gap * 0.04, WindowHeightInch * 0.1,
             Window43Gap * 0.18, WindowHeightInch * 0.03, "-10")) {
    setDungeonSize(&editDungeon, editDungeon.width, editDungeon.height - 10);
    modifiedSinceLastSave = 1;
  }

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
  if (event != ROLL_DOWN && event != ROLL_UP) {
    editMouseX = ScaleXInches(x);
    editMouseY = ScaleYInches(y);
  }

  int mx, my;
  getCellLocation(&editDungeon, editCursor.x, editCursor.y, editCellSize,
                  editMouseX, editMouseY, &mx, &my);
  if (event == MOUSEMOVE) {
    if (isMouseDownEditPage && mx >= 0 && my >= 0) {
      if (editMode == SetPlain && editDungeon.mp[mx][my] == Block) {
        editDungeon.mp[mx][my] = Plain;
        modifiedSinceLastSave = 1;
      } else if (editMode == SetBlock && editDungeon.mp[mx][my] == Plain) {
        editDungeon.mp[mx][my] = Block;
        modifiedSinceLastSave = 1;
      }
    }
  } else if (event == BUTTON_DOWN && button == LEFT_BUTTON) {
    isMouseDownEditPage = 1;
    if (mx >= 0 && my >= 0) {
      if (editMode == Flip && (editDungeon.mp[mx][my] == Plain ||
                               editDungeon.mp[mx][my] == Block)) {
        editDungeon.mp[mx][my] =
            editDungeon.mp[mx][my] == Plain ? Block : Plain;
        modifiedSinceLastSave = 1;
      } else if (editMode == SetPlain && editDungeon.mp[mx][my] == Block) {
        editDungeon.mp[mx][my] = Plain;
        modifiedSinceLastSave = 1;
      } else if (editMode == SetBlock && editDungeon.mp[mx][my] == Plain) {
        editDungeon.mp[mx][my] = Block;
        modifiedSinceLastSave = 1;
      } else if (editMode == PlaceStart) {
        setDungeonStart(&editDungeon, mx, my);
        modifiedSinceLastSave = 1;
      } else if (editMode == PlaceEnd) {
        setDungeonEnd(&editDungeon, mx, my);
        modifiedSinceLastSave = 1;
      }
    }
  } else if (event == BUTTON_DOWN && button == RIGHT_BUTTON) {
    if (!isJumpedEditPage && mx >= 0 && my >= 0) {
      editCursor.x = mx;
      editCursor.y = my;
      isJumpedEditPage = 1;
    }
  } else if (event == BUTTON_UP && button == LEFT_BUTTON) {
    isMouseDownEditPage = 0;
  } else if (event == BUTTON_UP && button == RIGHT_BUTTON) {
    isJumpedEditPage = 0;
  } else if (event == ROLL_DOWN) {
    if (editCellSize > 0.30) {
      editCellSize -= 0.10;
    }
  } else if (event == ROLL_UP) {
    if (editCellSize < 3.00) {
      editCellSize += 0.10;
    }
  }

  uiGetMouse(x, y, button, event);
}

AppState EditPage = {idEditPage,        initEditPage,          drawEditPage,
                     stopEditPage,      uiEditPageGetKeyboard, uiGetChar,
                     uiEditPageGetMouse};

void gotoEditPage() {
  editHasReadDungeon = 0;
  smPushState(&EditPage);
}

int isSaveAsPageFileExist;

void initSaveAsPage() {
  initGetFileNameDialog(editDungeonFileName);
  isSaveAsPageFileExist = 1;
}

void drawSaveAsPage() {
  drawGetFileNameDialog();

  double fontHeight = GetFontHeight();
  double h = fontHeight * 1.5;

  if (!isSaveAsPageFileExist) {
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
    FILE *file = fopen(dialogFileName, "w");
    if (file == NULL) {
      isSaveAsPageFileExist = 0;
    } else {
      smPopState();
      currentDungeon = editDungeon;
      saveDungeon(&currentDungeon, file);
      strcpy(editDungeonFileName, dialogFileName);
      modifiedSinceLastSave = 0;
    }
  }
}

AppState SaveAsPage = {idSaveAsPage,  initSaveAsPage, drawSaveAsPage, NULL,
                       uiGetKeyboard, uiGetChar,      uiGetMouse};

void gotoSaveAsPage() { smPushState(&SaveAsPage); }