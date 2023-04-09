#pragma once

#include "utils.h"
#include "appstate.h"
#include "statemanager.h"

void initPausePage() { smLastProc(); }

void drawPausePage() { smLastProc(); }

void uiPausePageGetKeyboard(int key, int event) {
  controlKeyboard(key, event);
  uiGetKeyboard(key, event);
}

void uiPausePageGetChar(int ch) { uiGetChar(ch); }

void uiPausePageGetMouse(int x, int y, int button, int event) {
  uiGetMouse(x, y, button, event);
}

AppState PausePage = {idPausePage,
                      initPausePage,
                      drawPausePage,
                      NULL,
                      uiPausePageGetKeyboard,
                      uiPausePageGetChar,
                      uiPausePageGetMouse};

void clearPause() {
  if (smStateTop()->uid == idPausePage) {
    smPopState();
  }
  setPauseBuffer();
}

void makePause(double seconds) {
  smPushState(&PausePage);
  Pause(seconds);
  clearPause();
}