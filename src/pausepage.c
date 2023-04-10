#include "pausepage.h"

#include <windows.h>
#include <time.h>

#include "utils.h"
#include "controller.h"
#include "statemanager.h"

AppState PausePage = {idPausePage, NULL, NULL, NULL, NULL, NULL, NULL};

int isPausing() {
  return !smIsStateEmpty() && smStateTop()->uid == idPausePage;
}

void makePause(double seconds) {
  double pausePageTimeFinish = (double)clock() / CLK_TCK + seconds;
  smBarePushState(&PausePage);
  smLastProc();
  UpdateDisplay();
  while (((double)clock() / CLK_TCK) < pausePageTimeFinish) {
    smLastProc();
    UpdateDisplay();
    Sleep(10);
  }
  smBarePopState();
  setPauseBuffer();
}