#pragma once

#include "appstate.h"

#define runCellSize (1.00) // 游玩时的视角大小。这个值在游玩的时候是锁定的。

void giveCheat();

void initExplorer();

void drawExplorer();

void stopExplorer();

void uiExplorerGetKeyboard(int key, int event);

void uiExplorerGetChar(int ch);

void uiExplorerGetMouse(int x, int y, int button, int event);

extern AppState Explorer;

void gotoExplorer();