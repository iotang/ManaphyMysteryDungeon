#pragma once

#include "appstate.h"

#define BaseSimulateSpeed (20000.00)

void initSimPage();

void startAutoSimulating();

void endAutoSimulating();

void setSimulateSpeed(int speed);

void drawSimPage();

void uiSimPageGetMouse(int x, int y, int button, int event);

void uiSimPageGetKeyboard(int key, int event);

void uiSimPageGetChar(int ch);

extern AppState SimPage;

void gotoSimPage();