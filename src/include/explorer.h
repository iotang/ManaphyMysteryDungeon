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

#include "globalvalue.h"

Dungeon expDungeon;
char expDungeonFileName[MaxFileNameLength + 1];

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
  expDungeon = currentDungeon;
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

  isDungeonRunning = 1;
  isDungeonGameOver = 0;
  clearHint();
  bindPlayerMove(manaphyMoveAttempt);
}

void drawExplorer() {
  drawDungeon(&editDungeon, manaphy.x, manaphy.y, runCellSize,
              manaphy.direction, 0);

  // title

  SetPenColor("White");
  drawBox(Window43Left, 0, Window43Right - Window43Left,
          WindowHeightInch * 0.03, 1, editDungeonFileName, 'L', "Black");

  // status bar
  int _pointSize;

  SetPenColor("Light Pink");
  drawRectangle(0, 0, Window43Left, WindowHeightInch, 1);

  SetPenColor("Black");

  double expRatio = 0.01 * manaphy.exp;
  _pointSize = GetPointSize();
  SetPointSize(16);
  drawBoxWithoutBorder(Window43Gap * 0.05, WindowHeightInch * 0.18,
                       Window43Gap * 0.90, WindowHeightInch * 0.03, 0,
                       manaphy.name, 'L', "Black");
  char lvTag[99];
  sprintf(lvTag, "%d", manaphy.lv);
  SetPenColor("Light Gray");
  drawRectangle(Window43Gap * 0.05, WindowHeightInch * 0.153,
                Window43Gap * 0.90, WindowHeightInch * 0.01, 1);
  SetPenColor("Cyan");
  if (expRatio > 0) {
    drawRectangle(Window43Gap * 0.05, WindowHeightInch * 0.153,
                  Window43Gap * 0.90 * expRatio, WindowHeightInch * 0.01, 1);
  }
  drawBoxWithoutBorder(Window43Gap * 0.05, WindowHeightInch * 0.15,
                       Window43Gap * 0.90, WindowHeightInch * 0.03, 0, "Lv.",
                       'L', "Black");
  drawBoxWithoutBorder(Window43Gap * 0.05, WindowHeightInch * 0.15,
                       Window43Gap * 0.90, WindowHeightInch * 0.03, 0, lvTag,
                       'R', "Black");
  SetPointSize(_pointSize);

  double hpRatio = 1.00 * manaphy.hp / manaphy.maxhp;
  if (hpRatio > 1)
    hpRatio = 1;
  if (hpRatio < 0)
    hpRatio = 0;

  SetPenColor("Light Gray");
  drawRectangle(Window43Gap * 0.05, WindowHeightInch * 0.1, Window43Gap * 0.90,
                WindowHeightInch * 0.05, 1);
  if (hpRatio > 0.50) {
    SetPenColor("Green");
  } else if (hpRatio > 0.20) {
    SetPenColor("Yellow");
  } else {
    SetPenColor("Red");
  }
  if (hpRatio > 0) {
    drawRectangle(Window43Gap * 0.05, WindowHeightInch * 0.1,
                  Window43Gap * 0.90 * hpRatio, WindowHeightInch * 0.05, 1);
  }

  SetPenColor("Black");
  char hpTag[99];
  _pointSize = GetPointSize();
  SetPointSize(32);
  sprintf(hpTag, "%d / %d", manaphy.hp, manaphy.maxhp);
  drawBoxWithoutBorder(Window43Gap * 0.05, WindowHeightInch * 0.103,
                       Window43Gap * 0.90, WindowHeightInch * 0.05, 0, hpTag,
                       'R', "Black");
  SetPointSize(_pointSize);

  _pointSize = GetPointSize();
  SetPointSize(16);
  char statTag[99];
  sprintf(statTag, "%d", manaphy.atk);
  SetPenColor("Light Gray");
  drawBoxWithoutBorder(Window43Gap * 0.05, WindowHeightInch * 0.07,
                       Window43Gap * 0.90, WindowHeightInch * 0.03, 0, "Attack",
                       'L', "Black");
  drawBoxWithoutBorder(Window43Gap * 0.05, WindowHeightInch * 0.07,
                       Window43Gap * 0.90, WindowHeightInch * 0.03, 0, statTag,
                       'R', "Black");
  sprintf(statTag, "%d", manaphy.def);
  SetPenColor("Light Gray");
  drawBoxWithoutBorder(Window43Gap * 0.05, WindowHeightInch * 0.04,
                       Window43Gap * 0.90, WindowHeightInch * 0.03, 0,
                       "Defense", 'L', "Black");
  drawBoxWithoutBorder(Window43Gap * 0.05, WindowHeightInch * 0.04,
                       Window43Gap * 0.90, WindowHeightInch * 0.03, 0, statTag,
                       'R', "Black");
  SetPointSize(_pointSize);

  // tools bar

  SetPenColor("Light Pink");
  drawRectangle(Window43Right, 0, Window43Gap, WindowHeightInch, 1);

  SetPenColor("White");
  drawRectangle(Window43Right + Window43Gap * 0.03, WindowHeightInch * 0.005,
                Window43Gap * 0.94, WindowHeightInch * 0.49, 1);

  for (int i = 0; i < manaphy.moveCount; i++) {
    double baseHeight = WindowHeightInch * (0.40 - i * 0.10 + 0.01);
    SetPenColor("White");

    setButtonColors("White", "Blue", "Cyan", "Blue", 1);
    button(GenUIID(i), Window43Right + Window43Gap * 0.05, baseHeight,
           Window43Gap * 0.9, WindowHeightInch * 0.08, NULL);

    SetPenColor("Black");
    _pointSize = GetPointSize();
    SetPointSize(18);
    drawBoxWithoutBorder(Window43Right + Window43Gap * 0.05,
                         baseHeight + WindowHeightInch * 0.03,
                         Window43Gap * 0.9, WindowHeightInch * 0.05, 0,
                         movedex[manaphy.move[i]].name, 'C', "Black");
    SetPointSize(_pointSize);
    _pointSize = GetPointSize();
    SetPointSize(8);
    char effectTag[99];
    sprintf(effectTag, "Eff. %d", movedex[manaphy.move[i]].effect);
    char ppTag[99];
    if (movedex[manaphy.move[i]].pp > 0) {
      sprintf(ppTag, "%d / %d", manaphy.pp[i], movedex[manaphy.move[i]].pp);
    } else {
      sprintf(ppTag, "Unlimited");
    }

    drawBoxWithoutBorder(Window43Right + Window43Gap * 0.05, baseHeight,
                         Window43Gap * 0.9, WindowHeightInch * 0.03, 0,
                         effectTag, 'L', "Black");
    drawBoxWithoutBorder(Window43Right + Window43Gap * 0.05, baseHeight,
                         Window43Gap * 0.9, WindowHeightInch * 0.03, 0, ppTag,
                         'R', "Black");
    SetPointSize(_pointSize);
  }

  // hint dialog

  if (!isHintEmpty()) {
    SetPenColor("White");
    drawRectangle(Window43Left + Window43Width * 0.1, WindowHeightInch * 0.05,
                  Window43Width * 0.8, WindowHeightInch * 0.2, 1);
    SetPenColor("Pink");
    drawRectangle(Window43Left + Window43Width * 0.1, WindowHeightInch * 0.05,
                  Window43Width * 0.8, WindowHeightInch * 0.2, 0);
    drawRectangle(Window43Left + Window43Width * 0.11, WindowHeightInch * 0.055,
                  Window43Width * 0.78, WindowHeightInch * 0.19, 0);
    _pointSize = GetPointSize();
    SetPointSize(24);
    drawBox(Window43Left + Window43Width * 0.11, WindowHeightInch * 0.055,
            Window43Width * 0.78, WindowHeightInch * 0.19, 0, hintString, 'L',
            "Black");
    SetPointSize(_pointSize);
  }

  drawToolsBar();
}

void stopExplorer() { isDungeonRunning = 0; }

void uiExplorerGetKeyboard(int key, int event) {
  controlKeyboard(key, event);
  uiGetKeyboard(key, event);
}

AppState Explorer = {idExplorer,   initExplorer,          drawExplorer,
                     stopExplorer, uiExplorerGetKeyboard, uiGetChar,
                     uiGetMouse};

void gotoExplorer() { smPushState(&Explorer); }