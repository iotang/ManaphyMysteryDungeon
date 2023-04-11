#include <windows.h>

#include "extgraph.h"
#include "graphics.h"
#include "imgui.h"
#include "mmsystem.h"

#include "random.h"
#include "utils.h"
#include "controller.h"
#include "statemanager.h"
#include "pages.h"
#include "pokemon.h"
#include "items.h"
#include "landevents.h"
#include "drawdungeon.h"
#include "helppage.h"

extern AppState MainMenu;

void Main() {
  Randomize();

  SetWindowSize(WindowWidthInch, WindowHeightInch);

  initPokedex();
  initMovedex();
  initItems();
  initLandEvents();
  initDungeonSprites();
  readHelpPageSprites();

  InitConsole();

  InitGraphics();
  InitGUI();
  SetWindowTitle("Manaphy's Mystery Dungeon");

  smPushState(&MainMenu);
  registerTimerEvent(render);

  startTimer(ScreenRend, FrameLength);
}