#include <windows.h>

#include "extgraph.h"
#include "graphics.h"
#include "imgui.h"
#include "mmsystem.h"
#include "utils.h"

#include "controller.h"
#include "statemanager.h"
#include "menu.h"

extern AppState MainMenu;

void Main() {
  SetWindowSize(WindowWidthInch, WindowHeightInch);

  // InitConsole();

  InitGraphics();
  InitGUI();
  SetWindowTitle("Manaphy's Mystery Dungeon");

  smPushState(&MainMenu);
  registerTimerEvent(render);

  startTimer(ScreenRend, FrameLength);
}