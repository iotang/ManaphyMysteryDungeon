#pragma once

#include "appstate.h"

#define BaseSimulateSpeed (20000.00) // 用于计算克雷色利亚自动演示速度的基准值。

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