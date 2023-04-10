#pragma once

#include "appstate.h"

void drawAboutPage();

void uiAboutPageGetKeyboard(int key, int event);

void uiAboutPageGetChar(int ch);

void uiAboutPageGetMouse(int x, int y, int button, int event);

extern AppState AboutPage;

void gotoAboutPage();