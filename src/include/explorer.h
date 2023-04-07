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
#include "hintvalue.h"
#include "statusbar.h"

#include "globalvalue.h"

Dungeon expDungeon;
char expDungeonFileName[MaxFileNameLength + 1];
int expHasReadDungeon;

#define runCellSize (1.00)
Pokemon manaphy;

void checkManaphyHealth() {
  while (updatePokemonStat(&manaphy))
    ;
  if (manaphy.hp <= 0) {
    clearHint();
    static char _failed[200];
    sprintf(_failed, "Oh no! %s is fainted!", manaphy.name);
    setHint(_failed);
    cancelTimer(HintExpire);
    isDungeonGameOver = 1;
  }
}

void manaphyMoveAttempt(int event) {
  if (isDungeonGameOver)
    return;

  if (event == FaceRight || event == FaceUp || event == FaceLeft ||
      event == FaceDown) {
    if (event == FaceRight) {
      manaphy.direction = RIGHT;
    }
    if (event == FaceUp) {
      manaphy.direction = UP;
    }
    if (event == FaceLeft) {
      manaphy.direction = LEFT;
    }
    if (event == FaceDown) {
      manaphy.direction = DOWN;
    }
  } else if (event == MoveRight || event == MoveUp || event == MoveLeft ||
             event == MoveDown) {
    int dx = manaphy.x, dy = manaphy.y;
    if (event == MoveRight) {
      manaphy.direction = RIGHT;
      dx++;
    }
    if (event == MoveUp) {
      manaphy.direction = UP;
      dy++;
    }
    if (event == MoveLeft) {
      manaphy.direction = LEFT;
      dx--;
    }
    if (event == MoveDown) {
      manaphy.direction = DOWN;
      dy--;
    }

    if (dx >= 0 && dx < expDungeon.width && dy >= 0 && dy < expDungeon.height) {
      if (expDungeon.mp[dx][dy] == Block) {
        setHint("You cannot move into a block.");
      } else {
        clearHint();
        manaphy.x = dx;
        manaphy.y = dy;
        manaphy.hp--;
        manaphy.exp += 33;
      }

      checkManaphyHealth();
      if (expDungeon.mp[dx][dy] == End) {
        clearHint();
        setHint("You have successfully conquered this dungeon!");
        cancelTimer(HintExpire);
        isDungeonGameOver = 1;
      }
    }
  }

  checkManaphyHealth();
}

void initExplorer() {
  if (!expHasReadDungeon) {
    expDungeon = currentDungeon;
    expHasReadDungeon = 1;
    strcpy(expDungeonFileName, editDungeonFileName);
    spawnPokemon(&manaphy, Player, NManaphy);
    manaphy.exp = 400;
    while (updatePokemonStat(&manaphy))
      ;

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

    isDungeonGameOver = 0;
  }
  clearHelpList();
  addHelpEntry("Move:", "Arrow or WASD");
  addHelpEntry("Change Direction:", "");
  addHelpEntry("", "Shift-Arrow or Shift-WASD");
  clearHint();
  bindPlayerMove(manaphyMoveAttempt);
}

void drawExplorer() {
  drawDungeon(&expDungeon, manaphy.x, manaphy.y, runCellSize, 0, NULL, 0);
  drawDungeonPokemon(&expDungeon, manaphy.x, manaphy.y, runCellSize, &manaphy);

  // title

  SetPenColor("White");
  drawBox(Window43Left, 0, Window43Right - Window43Left,
          WindowHeightInch * 0.03, 1, expDungeonFileName, 'L', "Black");

  // status bar
  int _pointSize;

  SetPenColor("Light Pink");
  drawRectangle(0, 0, Window43Left, WindowHeightInch, 1);

  drawHelpList(0, WindowHeightInch * 0.9);

  drawStatusBar(&manaphy, 0, WindowHeightInch * 0.01);

  // tools bar

  SetPenColor("Light Pink");
  drawRectangle(Window43Right, 0, Window43Gap, WindowHeightInch, 1);

  drawMoveList(&manaphy, Window43Right, 0);

  // hint dialog

  drawHintDialog();

  drawToolsBar();
}

void stopExplorer() { clearHint(); }

void uiExplorerGetKeyboard(int key, int event) {
  controlKeyboard(key, event);
  uiGetKeyboard(key, event);
}

AppState Explorer = {idExplorer,   initExplorer,          drawExplorer,
                     stopExplorer, uiExplorerGetKeyboard, uiGetChar,
                     uiGetMouse};

void gotoExplorer() {
  expHasReadDungeon = 0;
  smPushState(&Explorer);
}