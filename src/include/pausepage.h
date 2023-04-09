#pragma once

#include "utils.h"
#include "appstate.h"
#include "statemanager.h"

void initPausePage() { smLastProc(); }

void drawPausePage() { smLastProc(); }

AppState PausePage = {idPausePage, initPausePage, drawPausePage, NULL,
                      NULL,        NULL,          NULL};

void clearPause() {
  if (smStateTop()->uid == idPausePage)
    smPopState();
}

void makePause(double seconds) {
  smPushState(&PausePage);
  Pause(seconds);
  clearPause();
}