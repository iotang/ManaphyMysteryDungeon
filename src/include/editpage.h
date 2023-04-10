#pragma once

#include "utils.h"
#include "appstate.h"

extern char editDungeonFileName[MaxFileNameLength + 1];

typedef enum EditMode {
  Targeted,
  SetLandEvent,
  Flip,
  SetPlain,
  SetBlock,
  PlaceStart,
  PlaceEnd
} EditMode;

int saveDungeonEditPage();

void randomizeEditDungeon();

void initEditPage();

void editGetSolution();

void editGetSolutionWithLimit();

void drawEditPage();

void stopEditPage();

void uiEditPageGetKeyboard(int key, int event);

void uiEditPageGetMouse(int x, int y, int button, int event);

void uiEditPageGetChar(int ch);

extern AppState EditPage;

void gotoEditPage();

void initSaveAsPage();

void drawSaveAsPage();

void uiSaveAsPageGetKeyboard(int key, int event);

void uiSaveAsPageGetChar(int ch);

void uiSaveAsPageGetMouse(int x, int y, int button, int event);

extern AppState SaveAsPage;

void gotoSaveAsPage();