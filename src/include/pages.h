#pragma once

#include "appstate.h"

void callingExitWarning(voidFn nex);

void smPopStateUntilMainMenu();

void drawToolsBar();

void drawMainMenu();

void initOpenPage();

void drawOpenPage();

void initNewPage();

void drawNewPage();

void uiMainMenuGetKeyboard(int key, int event);

void uiMainMenuGetChar(int ch);

void uiMainMenuGetMouse(int x, int y, int button, int event);

void uiNewPageGetKeyboard(int key, int event);

void uiNewPageGetChar(int ch);

void uiNewPageGetMouse(int x, int y, int button, int event);

void uiOpenPageGetKeyboard(int key, int event);

void uiOpenPageGetChar(int ch);

void uiOpenPageGetMouse(int x, int y, int button, int event);

extern AppState MainMenu;

extern AppState NewPage;

extern AppState OpenPage;

void gotoNewPage();

void gotoOpenPage();