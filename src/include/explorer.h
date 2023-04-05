#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "appstate.h"
#include "statemanager.h"
#include "utils.h"
#include "pokemon.h"
#include "dungeon.h"
#include "drawdungeon.h"
#include "controller.h"

#include "alertdialog.h"
#include "getfilenamedialog.h"

#include "globalvalue.h"

Dungeon expDungeon;
char expDungeonFileName[MaxFileNameLength + 1];

#define runCellSize (1.00)
Pokemon manaphy;

void initExplorer() {
  expDungeon = currentDungeon;
  spawnPokemon(&manaphy, Player, NManaphy);
  manaphy.exp = 500;
  updatePokemonStat(&manaphy);

  int gotEnd = 0;
  for (int x = 0; x < expDungeon.width; x++) {
    for (int y = 0; y < expDungeon.height; y++) {
      if (expDungeon.mp[x][y] == Start) {
        manaphy.x = x;
        manaphy.y = y;
      }
      if (expDungeon.mp[x][y] == End) {
        gotEnd = 1;
      }
    }
  }

  if (manaphy.x < 0 || manaphy.y < 0) {
    setAlertDialog2("Error", "No entry in the dungeon");
    gotoAlertDialog();
    smPopState();
  }

  if (!gotEnd) {
    setAlertDialog2("Error", "No destination in the dungeon");
    gotoAlertDialog();
    smPopState();
  }
}

void drawExplorer() {
  drawDungeon(&editDungeon, manaphy.x, manaphy.y, editCellSize, 0);

  // title

  SetPenColor("White");
  drawBox(Window43Left, 0, Window43Right - Window43Left,
          WindowHeightInch * 0.03, 1, editDungeonFileName, 'L', "Black");

  SetPenColor("Light Gray");
  drawRectangle(0, 0, Window43Left, WindowHeightInch, 1);

  SetPenColor("Light Gray");
  drawRectangle(Window43Right, 0, Window43Gap, WindowHeightInch, 1);
}

AppState Explorer = {idExplorer,    initExplorer, drawExplorer, NULL,
                     uiGetKeyboard, uiGetChar,    uiGetMouse};

void gotoExplorer() { smPushState(&Explorer); }