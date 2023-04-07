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
#include "solvemodel.h"
#include "controller.h"

#include "alertdialog.h"
#include "getfilenamedialog.h"
#include "hintvalue.h"

#include "globalvalue.h"

Dungeon simDungeon;
DungeonSolution simHistory;
char simDungeonFileName[MaxFileNameLength + 1];

double simCellSize;
Pokemon simCursor, cresselia;
double simMouseX, simMouseY;

int isMouseDownSimPage;
int isJumpedSimPage;

int isAutoSimulating;

void checkCresseliaHealth() {
  while (updatePokemonStat(&cresselia))
    ;
  if (cresselia.hp <= 0) {
    clearHint();
    static char _failed[200];
    sprintf(_failed, "Oh no! %s is fainted!", cresselia.name);
    setHint(_failed);
    isDungeonSimTerminated = 1;
  }
}

void playerMoveSimPage(int event) {
  if (event == MoveRight) {
    if (simCursor.x + 1 < simDungeon.width) {
      simCursor.x++;
    }
  }
  if (event == MoveUp) {
    if (simCursor.y + 1 < simDungeon.height) {
      simCursor.y++;
    }
  }
  if (event == MoveLeft) {
    if (simCursor.x > 0) {
      simCursor.x--;
    }
  }
  if (event == MoveDown) {
    if (simCursor.y > 0) {
      simCursor.y--;
    }
  }
}

void initSimPage() {
  simCellSize = 1;
  simDungeon = currentDungeon;
  clearDungeonSolution(&simHistory);

  simCursor.x = simCursor.y = 0;
  for (int x = 0; x < simDungeon.width; x++) {
    for (int y = 0; y < simDungeon.height; y++) {
      if (simDungeon.mp[x][y] == Start) {
        simCursor.x = x;
        simCursor.y = y;
      }
    }
  }

  simDungeon.mp[simCursor.x][simCursor.y] = Start;
  spawnPokemon(&cresselia, Player, NCresselia);
  cresselia.x = simCursor.x;
  cresselia.y = simCursor.y;
  cresselia.exp = 4900;
  while (updatePokemonStat(&cresselia))
    ;

  isAutoSimulating = 0;
  isMouseDownSimPage = 0;
  isJumpedSimPage = 0;
  bindPlayerMove(playerMoveSimPage);
}

void drawSimPage() {
  if (simCursor.x < 0)
    simCursor.x = 0;
  if (simCursor.x >= simDungeon.width)
    simCursor.x = simDungeon.width - 1;

  if (simCursor.y < 0)
    simCursor.y = 0;
  if (simCursor.y >= simDungeon.height)
    simCursor.y = simDungeon.height - 1;

  drawDungeon(&simDungeon, simCursor.x, simCursor.y, simCellSize, 1,
              &simHistory, 1);
  drawDungeonHighlightCell(&simDungeon, simCursor.x, simCursor.y, simCellSize,
                           simMouseX, simMouseY);

  // title

  SetPenColor("White");
  drawBox(Window43Left, 0, Window43Right - Window43Left,
          WindowHeightInch * 0.03, 1, expDungeonFileName, 'L', "Black");

  // status bar

  SetPenColor("Yellow");
  drawRectangle(0, 0, Window43Left, WindowHeightInch, 1);

  drawStatusBar(&cresselia, 0, WindowHeightInch * 0.04);

  // tools bar

  SetPenColor("Yellow");
  drawRectangle(Window43Right, 0, Window43Gap, WindowHeightInch, 1);
  drawMoveList(&cresselia, Window43Right, 0);

  // hint dialog

  drawHintDialog();

  drawToolsBar();
}

AppState SimPage = {idSimPage,     initSimPage, drawSimPage, NULL,
                    uiGetKeyboard, uiGetChar,   uiGetMouse};

void gotoSimPage() { smPushState(&SimPage); }