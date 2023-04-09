#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "helplist.h"
#include "appstate.h"
#include "statemanager.h"
#include "utils.h"
#include "pokemon.h"
#include "dungeon.h"
#include "drawdungeon.h"
#include "solvemodel.h"
#include "controller.h"

#include "alertdialog.h"
#include "getfilenamedialog.h"

#include "globalvalue.h"

int editHasReadDungeon;
Dungeon editDungeon;
char editDungeonFileName[MaxFileNameLength + 1];

int editHasSolution;
DungeonSolution editDungeonSolution;
lint editDungeonSolutionLimit;
lint editDungeonSolutionHPPenalty;

double editCellSize;
Pokemon editCamera, editCursor;
double editMouseX, editMouseY;
LandEvent editLandEvent;
Item editLandItem;

int editEventOverrideItem;
int editEventOverrideLandEvent;

void decEditCellSize() {
  if (editCellSize > 0.15) {
    if (editCellSize > 1.00)
      editCellSize -= 0.10;
    else
      editCellSize -= 0.05;
  }
}

void incEditCellSize() {
  if (editCellSize < 2.00) {
    if (editCellSize < 1.00)
      editCellSize += 0.05;
    else
      editCellSize += 0.10;
  }
}

typedef enum EditMode {
  Targeted,
  SetLandEvent,
  Flip,
  SetPlain,
  SetBlock,
  PlaceStart,
  PlaceEnd
} EditMode;

EditMode editMode;

int isMouseDownEditPage;
int isJumpedEditPage;

void playerMoveEditPage(int event) {
  if (event == MoveRight) {
    if (editCamera.x + 1 < editDungeon.width) {
      editCamera.x++;
    }
  }
  if (event == MoveUp) {
    if (editCamera.y + 1 < editDungeon.height) {
      editCamera.y++;
    }
  }
  if (event == MoveLeft) {
    if (editCamera.x > 0) {
      editCamera.x--;
    }
  }
  if (event == MoveDown) {
    if (editCamera.y > 0) {
      editCamera.y--;
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
  if (modifiedSinceLastSave)
    editHasSolution = 0;
  modifiedSinceLastSave = 0;
  return 1;
}

void initEditPage() {
  if (!editHasReadDungeon) {
    editCellSize = 1;
    editDungeon = currentDungeon;
    editHasReadDungeon = 1;

    editCamera.x = editCamera.y = 0;
    for (int x = 0; x < editDungeon.width; x++) {
      for (int y = 0; y < editDungeon.height; y++) {
        if (editDungeon.mp[x][y] == Start) {
          editCamera.x = x;
          editCamera.y = y;
        }
      }
    }

    editDungeon.mp[editCamera.x][editCamera.y] = Start;
    editCursor.x = editCamera.x;
    editCursor.y = editCamera.y;

    editLandItem.type = INone;
    editLandEvent.type = None;
    editLandEvent.arg = 0;

    editEventOverrideItem = 1;
    editEventOverrideLandEvent = 1;
    editDungeonSolutionLimit = 1;
    editDungeonSolutionHPPenalty = 0;
  }

  clearHelpList();
  addHelpEntry("Move Camera:", "");
  addHelpEntry("", "Arrow or WASD");
  addHelpEntry("Select Cell:", "Left Click");
  addHelpEntry("Jump to Cell:", "Right Click");
  addHelpEntry("Zoom:", "Mouse Wheel");
  clearHint();

  editMode = Flip;
  isDungeonOpened = 1;
  isMouseDownEditPage = 0;
  isJumpedEditPage = 0;
  bindPlayerMove(playerMoveEditPage);
}

void editGetSolution() {
  if (saveDungeonEditPage() < 0) {
    setAlertDialog2("Error!", "Save failed");
    gotoAlertDialog();
    return;
  }

  int ret = getDungeonSolution(&editDungeon, &editDungeonSolution);
  if (ret < 0) {
    setAlertDialog2("Error!", "Dungeon is invalid");
    gotoAlertDialog();
  } else if (ret == 0) {
    editHasSolution = 1;
    setAlertDialog2("Warning", "Destination is unreachable");
    gotoAlertDialog();
  } else {
    editHasSolution = 1;
  }
}

void editGetSolutionWithLimit() {
  if (saveDungeonEditPage() < 0) {
    setAlertDialog2("Error!", "Save failed");
    gotoAlertDialog();
    return;
  }

  int ret = getDungeonSolutionWithLimit(&editDungeon, &editDungeonSolution,
                                        editDungeonSolutionLimit,
                                        editDungeonSolutionHPPenalty);
  if (ret < 0) {
    setAlertDialog2("Error!", "Dungeon is invalid");
    gotoAlertDialog();
  }
  editHasSolution = 1;
}

void drawEditPage() {
  sortDungeon(&editDungeon);

  if (editCamera.x < 0)
    editCamera.x = 0;
  if (editCamera.x >= editDungeon.width)
    editCamera.x = editDungeon.width - 1;

  if (editCamera.y < 0)
    editCamera.y = 0;
  if (editCamera.y >= editDungeon.height)
    editCamera.y = editDungeon.height - 1;

  int mx, my;
  if (notInAllMenu(editMouseX, editMouseY)) {
    getCellLocation(&editDungeon, editCamera.x, editCamera.y, editCellSize,
                    editMouseX, editMouseY, &mx, &my);
  }
  drawDungeon(&editDungeon, editCamera.x, editCamera.y, editCellSize, 1,
              &editDungeonSolution, editHasSolution && !modifiedSinceLastSave);
  if (notInAllMenu(editMouseX, editMouseY)) {
    drawDungeonHighlightCellAt(&editDungeon, editCamera.x, editCamera.y,
                               editCellSize, mx, my, editCellSize, 0, "Magenta",
                               0, 0);
  }
  if (editMode == Targeted) {
    drawDungeonHighlightCellAt(&editDungeon, editCamera.x, editCamera.y,
                               editCellSize, editCursor.x, editCursor.y,
                               editCellSize * 0.2, 1, "Orange", 0.3, -0.3);
  }

  // title

  SetPenColor(modifiedSinceLastSave ? "Yellow" : "White");
  drawBox(Window43Left, WindowHeightInch - MenuHeight, Window43Width,
          MenuHeight, 1, editDungeonFileName, 'L', "Black");

  SetPenColor("Cyan");
  drawRectangle(0, 0, Window43Left, WindowHeightInch, 1);

  drawHelpList(0, WindowHeightInch * 0.9);

  char __buf[99];

  sprintf(__buf, "%lld", editDungeonSolutionLimit);
  drawBoxWithoutBorder(Window43Gap * 0.04, WindowHeightInch * 0.70,
                       Window43Gap * 0.92, WindowHeightInch * 0.03, 1,
                       "Solution Step", 'C', "Black");
  SetPenColor("White");
  drawBox(Window43Gap * 0.04, WindowHeightInch * 0.66, Window43Gap * 0.92,
          WindowHeightInch * 0.03, 1, __buf, 'C', "Black");
  setButtonColors("White", "Blue", "Blue", "White", 1);
  if (button(GenUIID(0), Window43Gap * 0.64, WindowHeightInch * 0.62,
             Window43Gap * 0.1, WindowHeightInch * 0.03, "+", idEditPage)) {
    editDungeonSolutionLimit++;
  }
  if (button(GenUIID(0), Window43Gap * 0.78, WindowHeightInch * 0.62,
             Window43Gap * 0.18, WindowHeightInch * 0.03, "+10", idEditPage)) {
    editDungeonSolutionLimit += 10;
  }
  if (button(GenUIID(0), Window43Gap * 0.26, WindowHeightInch * 0.62,
             Window43Gap * 0.1, WindowHeightInch * 0.03, "-", idEditPage)) {
    editDungeonSolutionLimit--;
    if (editDungeonSolutionLimit < 1)
      editDungeonSolutionLimit = 1;
  }
  if (button(GenUIID(0), Window43Gap * 0.04, WindowHeightInch * 0.62,
             Window43Gap * 0.18, WindowHeightInch * 0.03, "-10", idEditPage)) {
    editDungeonSolutionLimit -= 10;
    if (editDungeonSolutionLimit < 1)
      editDungeonSolutionLimit = 1;
  }
  if (button(GenUIID(0), Window43Gap * 0.505, WindowHeightInch * 0.58,
             Window43Gap * 0.195, WindowHeightInch * 0.03, "+100",
             idEditPage)) {
    editDungeonSolutionLimit += 100;
  }
  if (button(GenUIID(0), Window43Gap * 0.71, WindowHeightInch * 0.58,
             Window43Gap * 0.25, WindowHeightInch * 0.03, "+1000",
             idEditPage)) {
    editDungeonSolutionLimit += 1000;
  }
  if (button(GenUIID(0), Window43Gap * 0.30, WindowHeightInch * 0.58,
             Window43Gap * 0.195, WindowHeightInch * 0.03, "-100",
             idEditPage)) {
    editDungeonSolutionLimit -= 100;
    if (editDungeonSolutionLimit < 0)
      editDungeonSolutionLimit = 0;
  }
  if (button(GenUIID(0), Window43Gap * 0.04, WindowHeightInch * 0.58,
             Window43Gap * 0.25, WindowHeightInch * 0.03, "-1000",
             idEditPage)) {
    editDungeonSolutionLimit -= 1000;
    if (editDungeonSolutionLimit < 0)
      editDungeonSolutionLimit = 0;
  }

  sprintf(__buf, "%lld", editDungeonSolutionHPPenalty);
  drawBoxWithoutBorder(Window43Gap * 0.04, WindowHeightInch * 0.54,
                       Window43Gap * 0.92, WindowHeightInch * 0.03, 1,
                       "HP Penalty", 'C', "Black");
  SetPenColor("White");
  drawBox(Window43Gap * 0.04, WindowHeightInch * 0.50, Window43Gap * 0.92,
          WindowHeightInch * 0.03, 1, __buf, 'C', "Black");
  setButtonColors("White", "Blue", "Blue", "White", 1);
  if (button(GenUIID(0), Window43Gap * 0.505, WindowHeightInch * 0.46,
             Window43Gap * 0.195, WindowHeightInch * 0.03, "+10", idEditPage)) {
    editDungeonSolutionHPPenalty += 10;
  }
  if (button(GenUIID(0), Window43Gap * 0.71, WindowHeightInch * 0.46,
             Window43Gap * 0.25, WindowHeightInch * 0.03, "+100", idEditPage)) {
    editDungeonSolutionHPPenalty += 100;
  }
  if (button(GenUIID(0), Window43Gap * 0.30, WindowHeightInch * 0.46,
             Window43Gap * 0.195, WindowHeightInch * 0.03, "-10", idEditPage)) {
    editDungeonSolutionHPPenalty -= 10;
    if (editDungeonSolutionHPPenalty < 0)
      editDungeonSolutionHPPenalty = 0;
  }
  if (button(GenUIID(0), Window43Gap * 0.04, WindowHeightInch * 0.46,
             Window43Gap * 0.25, WindowHeightInch * 0.03, "-100", idEditPage)) {
    editDungeonSolutionHPPenalty -= 100;
    if (editDungeonSolutionHPPenalty < 0)
      editDungeonSolutionHPPenalty = 0;
  }

  if (button(GenUIID(0), Window43Gap * 0.04, WindowHeightInch * 0.40,
             Window43Gap * 0.92, WindowHeightInch * 0.03, "Limited Solution",
             idEditPage)) {
    editGetSolutionWithLimit();
  }
  if (button(GenUIID(0), Window43Gap * 0.04, WindowHeightInch * 0.34,
             Window43Gap * 0.92, WindowHeightInch * 0.03, "All Solution",
             idEditPage)) {
    editGetSolution();
  }

  // width height adj

  sprintf(__buf, "%d", editDungeon.width);
  drawBoxWithoutBorder(Window43Gap * 0.4, WindowHeightInch * 0.23,
                       Window43Gap * 0.2, WindowHeightInch * 0.03, 1, "Width",
                       'C', "Black");
  SetPenColor("White");
  drawBox(Window43Gap * 0.4, WindowHeightInch * 0.19, Window43Gap * 0.2,
          WindowHeightInch * 0.03, 1, __buf, 'C', "Black");
  setButtonColors("White", "Blue", "Blue", "White", 1);
  if (button(GenUIID(0), Window43Gap * 0.64, WindowHeightInch * 0.19,
             Window43Gap * 0.1, WindowHeightInch * 0.03, "+", idEditPage)) {
    setDungeonSize(&editDungeon, editDungeon.width + 1, editDungeon.height);
    modifiedSinceLastSave = 1;
  }
  if (button(GenUIID(0), Window43Gap * 0.78, WindowHeightInch * 0.19,
             Window43Gap * 0.18, WindowHeightInch * 0.03, "+10", idEditPage)) {
    setDungeonSize(&editDungeon, editDungeon.width + 10, editDungeon.height);
    modifiedSinceLastSave = 1;
  }
  if (button(GenUIID(0), Window43Gap * 0.26, WindowHeightInch * 0.19,
             Window43Gap * 0.1, WindowHeightInch * 0.03, "-", idEditPage)) {
    setDungeonSize(&editDungeon, editDungeon.width - 1, editDungeon.height);
    modifiedSinceLastSave = 1;
  }
  if (button(GenUIID(0), Window43Gap * 0.04, WindowHeightInch * 0.19,
             Window43Gap * 0.18, WindowHeightInch * 0.03, "-10", idEditPage)) {
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
             Window43Gap * 0.1, WindowHeightInch * 0.03, "+", idEditPage)) {
    setDungeonSize(&editDungeon, editDungeon.width, editDungeon.height + 1);
    modifiedSinceLastSave = 1;
  }
  if (button(GenUIID(0), Window43Gap * 0.78, WindowHeightInch * 0.1,
             Window43Gap * 0.18, WindowHeightInch * 0.03, "+10", idEditPage)) {
    setDungeonSize(&editDungeon, editDungeon.width, editDungeon.height + 10);
    modifiedSinceLastSave = 1;
  }
  if (button(GenUIID(0), Window43Gap * 0.26, WindowHeightInch * 0.1,
             Window43Gap * 0.1, WindowHeightInch * 0.03, "-", idEditPage)) {
    setDungeonSize(&editDungeon, editDungeon.width, editDungeon.height - 1);
    modifiedSinceLastSave = 1;
  }
  if (button(GenUIID(0), Window43Gap * 0.04, WindowHeightInch * 0.1,
             Window43Gap * 0.18, WindowHeightInch * 0.03, "-10", idEditPage)) {
    setDungeonSize(&editDungeon, editDungeon.width, editDungeon.height - 10);
    modifiedSinceLastSave = 1;
  }

  sprintf(__buf, "%.2lf", editCellSize);
  drawBoxWithoutBorder(Window43Gap * 0.4, WindowHeightInch * 0.06,
                       Window43Gap * 0.2, WindowHeightInch * 0.03, 1, "Zoom",
                       'C', "Black");
  SetPenColor("White");
  drawBox(Window43Gap * 0.25, WindowHeightInch * 0.02, Window43Gap * 0.5,
          WindowHeightInch * 0.03, 1, __buf, 'C', "Black");
  setButtonColors("White", "Blue", "Blue", "White", 1);
  if (button(GenUIID(0), Window43Gap * 0.78, WindowHeightInch * 0.02,
             Window43Gap * 0.18, WindowHeightInch * 0.03, "+", idEditPage)) {
    incEditCellSize();
  }
  if (button(GenUIID(0), Window43Gap * 0.04, WindowHeightInch * 0.02,
             Window43Gap * 0.18, WindowHeightInch * 0.03, "-", idEditPage)) {
    decEditCellSize();
  }

  // edit mode

  SetPenColor("Cyan");
  drawRectangle(Window43Right, 0, Window43Gap, WindowHeightInch, 1);

  setButtonColors(editMode == Targeted ? "Yellow" : "White", "Blue", "Blue",
                  "White", 1);
  if (button(GenUIID(0), Window43Right + Window43Gap * 0.1,
             WindowHeightInch * 0.90, Window43Gap * 0.8,
             WindowHeightInch * 0.04, "Select", idEditPage)) {
    editMode = Targeted;
  }

  setButtonColors(editMode == SetLandEvent ? "Yellow" : "White", "Blue", "Blue",
                  "White", 1);
  if (button(GenUIID(0), Window43Right + Window43Gap * 0.1,
             WindowHeightInch * 0.85, Window43Gap * 0.8,
             WindowHeightInch * 0.04, "Event", idEditPage)) {
    editMode = SetLandEvent;
  }

  setButtonColors(editMode == Flip ? "Yellow" : "White", "Blue", "Blue",
                  "White", 1);
  if (button(GenUIID(0), Window43Right + Window43Gap * 0.1,
             WindowHeightInch * 0.8, Window43Gap * 0.8, WindowHeightInch * 0.04,
             "Flip", idEditPage)) {
    editMode = Flip;
  }

  setButtonColors(editMode == SetPlain ? "Yellow" : "White", "Blue", "Blue",
                  "White", 1);
  if (button(GenUIID(0), Window43Right + Window43Gap * 0.1,
             WindowHeightInch * 0.75, Window43Gap * 0.8,
             WindowHeightInch * 0.04, "Plain", idEditPage)) {
    editMode = SetPlain;
  }

  setButtonColors(editMode == SetBlock ? "Yellow" : "White", "Blue", "Blue",
                  "White", 1);
  if (button(GenUIID(0), Window43Right + Window43Gap * 0.1,
             WindowHeightInch * 0.7, Window43Gap * 0.8, WindowHeightInch * 0.04,
             "Block", idEditPage)) {
    editMode = SetBlock;
  }

  setButtonColors(editMode == PlaceStart ? "Yellow" : "White", "Blue", "Blue",
                  "White", 1);
  if (button(GenUIID(0), Window43Right + Window43Gap * 0.1,
             WindowHeightInch * 0.65, Window43Gap * 0.8,
             WindowHeightInch * 0.04, "Starts at", idEditPage)) {
    editMode = PlaceStart;
  }

  setButtonColors(editMode == PlaceEnd ? "Cyan" : "White", "Blue", "Blue",
                  "White", 1);
  if (button(GenUIID(0), Window43Right + Window43Gap * 0.1,
             WindowHeightInch * 0.6, Window43Gap * 0.8, WindowHeightInch * 0.04,
             "Ends at", idEditPage)) {
    editMode = PlaceEnd;
  }

  // event and item

  if (editMode == Targeted) {
    if (drawDungeonEventEdit(
            &editDungeon.event[editCursor.x][editCursor.y],
            &editDungeon.item[editCursor.x][editCursor.y], Window43Right, 0,
            "Orange",
            editDungeon.mp[editCursor.x][editCursor.y] != Start &&
                editDungeon.mp[editCursor.x][editCursor.y] != End,
            idEditPage)) {
      modifiedSinceLastSave = 1;
    }
  } else if (editMode == SetLandEvent) {
    drawDungeonEventEditOverride(
        &editLandEvent, &editLandItem, Window43Right, 0, "Magenta", 1,
        idEditPage, 1, &editEventOverrideItem, &editEventOverrideLandEvent);
  } else {
    drawDungeonEventEdit(&editDungeon.event[mx][my], &editDungeon.item[mx][my],
                         Window43Right, 0, "White", 0, idEditPage);
  }

  drawToolsBar();
}

void stopEditPage() { isDungeonOpened = 0; }

void uiEditPageGetKeyboard(int key, int event) {
  if (smStateTop()->uid != idEditPage)
    return;

  controlKeyboard(key, event);
  uiGetKeyboard(key, event);
}

void uiEditPageGetMouse(int x, int y, int button, int event) {
  if (smStateTop()->uid != idEditPage)
    return;

  if (event != ROLL_DOWN && event != ROLL_UP) {
    editMouseX = ScaleXInches(x);
    editMouseY = ScaleYInches(y);
  }

  if (notInAllMenu(editMouseX, editMouseY)) {
    int mx, my;
    getCellLocation(&editDungeon, editCamera.x, editCamera.y, editCellSize,
                    editMouseX, editMouseY, &mx, &my);
    if (event == MOUSEMOVE) {
      if (isMouseDownEditPage && mx >= 0 && my >= 0) {
        if (editMode == SetPlain && editDungeon.mp[mx][my] == Block) {
          editDungeon.mp[mx][my] = Plain;
          modifiedSinceLastSave = 1;
        } else if (editMode == SetBlock && editDungeon.mp[mx][my] == Plain) {
          editDungeon.mp[mx][my] = Block;
          modifiedSinceLastSave = 1;
        } else if (editMode == SetLandEvent) {
          if (editEventOverrideItem)
            editDungeon.item[mx][my] = editLandItem;
          if (editEventOverrideLandEvent)
            editDungeon.event[mx][my] = editLandEvent;
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
        } else if (editMode == Targeted) {
          editCursor.x = mx;
          editCursor.y = my;
        } else if (editMode == SetLandEvent &&
                   editDungeon.mp[mx][my] != Start &&
                   editDungeon.mp[mx][my] != End) {
          if (editEventOverrideItem)
            editDungeon.item[mx][my] = editLandItem;
          if (editEventOverrideLandEvent)
            editDungeon.event[mx][my] = editLandEvent;
          modifiedSinceLastSave = 1;
        }
      }
    } else if (event == BUTTON_DOWN && button == RIGHT_BUTTON) {
      if (!isJumpedEditPage && mx >= 0 && my >= 0) {
        editCamera.x = mx;
        editCamera.y = my;
        isJumpedEditPage = 1;
      }
    } else if (event == BUTTON_UP && button == LEFT_BUTTON) {
      isMouseDownEditPage = 0;
    } else if (event == BUTTON_UP && button == RIGHT_BUTTON) {
      isJumpedEditPage = 0;
    } else if (event == ROLL_DOWN) {
      decEditCellSize();
    } else if (event == ROLL_UP) {
      incEditCellSize();
    }
  }

  uiGetMouse(x, y, button, event);
}

void uiEditPageGetChar(int ch) {
  if (smStateTop()->uid == idEditPage)
    uiGetChar(ch);
}

AppState EditPage = {
    idEditPage,        initEditPage,          drawEditPage,
    stopEditPage,      uiEditPageGetKeyboard, uiEditPageGetChar,
    uiEditPageGetMouse};

void gotoEditPage() {
  editHasReadDungeon = 0;
  editHasSolution = 0;
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
             0.15 * WindowWidthInch / 3, h, "Cancel", idSaveAsPage)) {
    smPopState();
  }

  if (button(GenUIID(1), 1.7 * WindowWidthInch / 3, 1.2 * WindowHeightInch / 3,
             0.15 * WindowWidthInch / 3, h, "Confirm", idSaveAsPage)) {
    FILE *file = fopen(dialogFileName, "w");
    if (file == NULL) {
      isSaveAsPageFileExist = 0;
    } else {
      smPopState();
      currentDungeon = editDungeon;
      saveDungeon(&currentDungeon, file);
      strcpy(editDungeonFileName, dialogFileName);
      if (modifiedSinceLastSave)
        editHasSolution = 0;
      modifiedSinceLastSave = 0;
    }
  }
}

void uiSaveAsPageGetKeyboard(int key, int event) {
  if (smStateTop()->uid == idSaveAsPage)
    uiGetKeyboard(key, event);
}

void uiSaveAsPageGetChar(int ch) {
  if (smStateTop()->uid == idSaveAsPage)
    uiGetChar(ch);
}

void uiSaveAsPageGetMouse(int x, int y, int button, int event) {
  if (smStateTop()->uid == idSaveAsPage)
    uiGetMouse(x, y, button, event);
}

AppState SaveAsPage = {idSaveAsPage,
                       initSaveAsPage,
                       drawSaveAsPage,
                       NULL,
                       uiSaveAsPageGetKeyboard,
                       uiSaveAsPageGetChar,
                       uiSaveAsPageGetMouse};

void gotoSaveAsPage() { smPushState(&SaveAsPage); }