#pragma once

#include "appstate.h"

void initPausePage();

void drawPausePage() ;

void uiPausePageGetKeyboard(int key, int event);

void uiPausePageGetChar(int ch);

void uiPausePageGetMouse(int x, int y, int button, int event);

extern AppState PausePage;

void clearPause();

void makePause(double seconds);