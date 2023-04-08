#pragma once

#include <time.h>

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
#include "itembag.h"

#include "alertdialog.h"
#include "getfilenamedialog.h"
#include "hintvalue.h"
#include "statusbar.h"
#include "drawitembag.h"

#include "globalvalue.h"

Dungeon expDungeon;
char expDungeonFileName[MaxFileNameLength + 1];
int expHasReadDungeon;

#define runCellSize (1.00)
Pokemon manaphy;
ItemBag manaphyItemBag;

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
        manaphy.belly -= 2.8;
        if (manaphy.belly <= 0) {
          manaphy.belly = 0;
          manaphy.hp--;
        }
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
    spawnPokemon(&manaphy, Player, NManaphy, Male);
    manaphy.exp = 400;
    while (updatePokemonStat(&manaphy))
      ;
    clearItemBag(&manaphyItemBag);
    for (int i = 0; i < 3; i++) {
      Item item;
      item.type = IApple;
      addIntoItemBag(&manaphyItemBag, item);
      item.type = IGummi;
      addIntoItemBag(&manaphyItemBag, item);
      item.type = IOranBerry;
      addIntoItemBag(&manaphyItemBag, item);
      item.type = ISitrusBerry;
      addIntoItemBag(&manaphyItemBag, item);
      item.type = IElixir;
      addIntoItemBag(&manaphyItemBag, item);
      item.type = IKey;
      item.arg = i + 1;
      addIntoItemBag(&manaphyItemBag, item);
      item.type = ITM;
      item.arg = i + 1;
      addIntoItemBag(&manaphyItemBag, item);
    }

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
  drawBox(Window43Left, WindowHeightInch * 0.97, Window43Width,
          WindowHeightInch * 0.03, 1, expDungeonFileName, 'L', "Black");

  int colorType = (clock() >> 8) & 1;
  int hpLow = manaphy.hp * 5 <= manaphy.maxhp;
  int emptyBelly = manaphy.belly <= 0;
  if (hpLow) {
    SetPenColor(colorType ? "Red" : "Light Pink");
    drawBox(Window43Left, 0, Window43Width, WindowHeightInch * 0.03, 1,
            "Danger! HP Low!", 'L', colorType ? "White" : "Black");
  } else if (emptyBelly) {
    SetPenColor(colorType ? "Yellow" : "Light Pink");
    drawBox(Window43Left, 0, Window43Width, WindowHeightInch * 0.03, 1,
            "Danger! Belly is Empty!", 'L', "Black");
  }

  // status bar
  /*
    if (hpLow) {
      SetPenColor(colorType ? "Red" : "Light Pink");
    } else if (emptyBelly) {
      SetPenColor(colorType ? "Yellow" : "Light Pink");
    } else {
      SetPenColor("Light Pink");
    }
  */
  SetPenColor("Light Pink");
  drawRectangle(0, 0, Window43Left, WindowHeightInch, 1);

  drawHelpList(0, WindowHeightInch * 0.9);

  drawStatusBar(&manaphy, 0, WindowHeightInch * 0.01);

  // tools bar
  SetPenColor("Light Pink");
  drawRectangle(Window43Right, 0, Window43Gap, WindowHeightInch, 1);

  drawItemBag(&manaphyItemBag, Window43Right, WindowHeightInch * 0.5);
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