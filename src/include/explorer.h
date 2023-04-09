#pragma once

#include <math.h>
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
#include "dungeonprocess.h"
#include "controller.h"
#include "itembag.h"
#include "attempt.h"

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

int emptyBellyHintCount;

void enemyRound() {}

int manaphyMove(int att) {
  if (isDungeonGameOver)
    return 0;

  if (isFaceAttempt(att)) {
    Direction dir = argFaceAttempt(att);
    if (dir == RIGHT) {
      manaphy.direction = RIGHT;
    } else if (dir == UP) {
      manaphy.direction = UP;
    } else if (dir == LEFT) {
      manaphy.direction = LEFT;
    } else if (dir == DOWN) {
      manaphy.direction = DOWN;
    }
    return 0;
  }

  if (isMoveAttempt(att)) {
    Direction dir = argMoveAttempt(att);
    int dx = manaphy.x, dy = manaphy.y;
    if (dir == RIGHT) {
      dx++;
      manaphy.direction = RIGHT;
    } else if (dir == UP) {
      dy++;
      manaphy.direction = UP;
    } else if (dir == LEFT) {
      dx--;
      manaphy.direction = LEFT;
    } else if (dir == DOWN) {
      dy--;
      manaphy.direction = DOWN;
    }
    if (!isInDungeon(&expDungeon, dx, dy))
      return 0;

    int successMove = 0;

    if (expDungeon.event[dx][dy].type == Lock) {
      int key = getKeyInItemBag(&manaphyItemBag);
      if (!((key >> (expDungeon.event[dx][dy].arg - 1)) & 1)) {
        emplaceHint("You don't have the right key to open it.");
      } else {
        successMove = 1;
      }
    } else if (expDungeon.mp[dx][dy] == Block) {
      emplaceHint("You cannot move into a block.");
    } else {
      successMove = 1;
    }

    if (successMove) {
      manaphy.x = dx;
      manaphy.y = dy;
      manaphy.belly -= 2.8;
      if (manaphy.belly <= 0) {
        manaphy.belly = 0;
        manaphy.hp--;
        if (emptyBellyHintCount == 0) {
          emplaceHint("Oh, no! Your belly is empty!");
        } else if (emptyBellyHintCount == 1) {
          emplaceHint("Hurry up! You must have something to eat!");
        } else if (emptyBellyHintCount == 2) {
          emplaceHint("Otherwise you will fall soon!");
        }
        emptyBellyHintCount++;
      } else
        emptyBellyHintCount = 0;
    }

    return successMove;
  }

  if (isUseItemAttempt(att)) {
    int index = argUseItemAttempt(att);
    int type = manaphyItemBag.items[index].type;
    int arg = manaphyItemBag.items[index].arg;

    printf("%d %d\n", type, arg);
    static char _useItem[99];

    if (type == IKey) {
      sprintf(_useItem, "Walk into the lock to use the key.");
      emplaceHint(_useItem);
      return 0;
    } else if (type == ITM) {
      return 0;
    } else {
      sprintf(_useItem, "%s uses %s.", manaphy.name, itemsData[type].name);
      emplaceHint(_useItem);

      if (itemsData[type].dbelly != 0) {
        if (itemsData[type].dbelly > 49.5) {
          sprintf(_useItem, "%s's belly is filled!", manaphy.name);
          emplaceHint(_useItem);
        } else if (itemsData[type].dbelly > 9.5) {
          sprintf(_useItem, "%s's belly is slightly filled!", manaphy.name);
          emplaceHint(_useItem);
        } else if (itemsData[type].dbelly < 0) {
          sprintf(_useItem, "%s's belly dropped!", manaphy.name);
          emplaceHint(_useItem);
        }
        manaphy.belly += itemsData[type].dbelly;
      }

      if (itemsData[type].dmaxhp) {
        sprintf(_useItem, "%s's max HP %s by %d!", manaphy.name,
                itemsData[type].dmaxhp > 0 ? "raised" : "dropped",
                abs(itemsData[type].dmaxhp));
        emplaceHint(_useItem);
        manaphy.maxhp += itemsData[type].dmaxhp;
      }

      makePokemonStatBound(&manaphy);

      if (itemsData[type].dhp) {
        if (itemsData[type].dhp > 0) {
          int recover = min(manaphy.maxhp - manaphy.hp, itemsData[type].dhp);
          if (recover > 0) {
            sprintf(_useItem, "%s recovers %d HP!", manaphy.name, recover);
          } else {
            sprintf(_useItem, "%s's HP doesn't changed.", manaphy.name);
          }
        } else {
          sprintf(_useItem, "%s receives %d damage!", manaphy.name,
                  -itemsData[type].dhp);
        }
        emplaceHint(_useItem);
        manaphy.hp += itemsData[type].dhp;
      }

      if (itemsData[type].datk) {
        sprintf(_useItem, "%s's attack %s by %d!", manaphy.name,
                itemsData[type].datk > 0 ? "raised" : "dropped",
                abs(itemsData[type].datk));
        emplaceHint(_useItem);
        manaphy.atk += itemsData[type].datk;
      }

      if (itemsData[type].ddef) {
        sprintf(_useItem, "%s's defense %s by %d!", manaphy.name,
                itemsData[type].ddef > 0 ? "raised" : "dropped",
                abs(itemsData[type].ddef));
        emplaceHint(_useItem);
        manaphy.def += itemsData[type].ddef;
      }

      if (itemsData[type].dexp != 0) {
        sprintf(_useItem, "%s's %s %.0lf EXP!", manaphy.name,
                itemsData[type].dexp > 0 ? "gains" : "loses",
                fabs(itemsData[type].dexp));
        emplaceHint(_useItem);
        manaphy.exp += itemsData[type].dexp;
      }

      while (updatePokemonStat(&manaphy)) {
        sprintf(_useItem, "%s raises to level %d!", manaphy.name, manaphy.lv);
        setHint(_useItem);
      }

      removeOutItemBag(&manaphyItemBag, index);
      return 1;
    }
  }

  return 0;
}

void manaphyRound(int att) {
  if (!manaphyMove(att))
    return;

  pokemonStepOn(&expDungeon, &manaphy, &manaphyItemBag);

  checkManaphyHealth();
  if (expDungeon.mp[manaphy.x][manaphy.y] == End) {
    clearHint();
    setHint("You have successfully conquered this dungeon!");
    cancelTimer(HintExpire);
    isDungeonGameOver = 1;
  }

  enemyRound();
}

void manaphyMoveAttempt(int event) {
  if (isDungeonGameOver)
    return;

  int att = makeFaceAttempt(DOWN);

  if (event == FaceRight || event == FaceUp || event == FaceLeft ||
      event == FaceDown) {
    if (event == FaceRight) {
      att = makeFaceAttempt(RIGHT);
    } else if (event == FaceUp) {
      att = makeFaceAttempt(UP);
    } else if (event == FaceLeft) {
      att = makeFaceAttempt(LEFT);
    } else if (event == FaceDown) {
      att = makeFaceAttempt(DOWN);
    }
  } else if (event == MoveRight || event == MoveUp || event == MoveLeft ||
             event == MoveDown) {
    if (event == MoveRight) {
      att = makeMoveAttempt(RIGHT);
    } else if (event == MoveUp) {
      att = makeMoveAttempt(UP);
    } else if (event == MoveLeft) {
      att = makeMoveAttempt(LEFT);
    } else if (event == MoveDown) {
      att = makeMoveAttempt(DOWN);
    }
  }

  return manaphyRound(att);
}

void giveCheat() {
  if (isDungeonGameOver)
    return;
  int tx, ty, key = getKeyInItemBag(&manaphyItemBag);
  getDungeonEnd(&expDungeon, &tx, &ty);
  Direction ret = ERRORDIRECTION;
  lint minDistan = linf - 1;
  for (int i = 0; i < 4; i++) {
    int dx = manaphy.x + go[i][0], dy = manaphy.y + go[i][1];
    if (dx >= 0 && dx < expDungeon.width && dy >= 0 && dy < expDungeon.height) {
      if (expDungeon.mp[dx][dy] == Block)
        continue;
      lint distan = getDungeonDistance(&expDungeon, dx, dy, key, tx, ty,
                                       DefaultHPPenalty);
      if (distan < minDistan || (distan == minDistan && RandomChance(0.5))) {
        ret = i;
        minDistan = distan;
      }
    }
  }

  static char _cheat[99];
  sprintf(_cheat, "Walk %s may good for you.",
          ret == RIGHT ? "right"
                       : (ret == UP ? "up" : (ret == LEFT ? "left" : "down")));
  emplaceHint(_cheat);
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
    emptyBellyHintCount = 0;
    clearHint();
  }
  clearHelpList();
  addHelpEntry("Move:", "Arrow or WASD");
  addHelpEntry("Change Direction:", "");
  addHelpEntry("", "Shift-Arrow or Shift-WASD");
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
    drawBox(Window43Left, 0, Window43Width, MenuHeight, 1, "Danger! HP Low!",
            'L', colorType ? "White" : "Black");
  } else if (emptyBelly) {
    SetPenColor(colorType ? "Yellow" : "Light Pink");
    drawBox(Window43Left, 0, Window43Width, MenuHeight, 1,
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

  drawStatusBar(&manaphy, 0, WindowHeightInch * 0.01, idExplorer);

  // tools bar
  SetPenColor("Light Pink");
  drawRectangle(Window43Right, 0, Window43Gap, WindowHeightInch, 1);

  int useItem = drawItemBag(&manaphyItemBag, Window43Right,
                            WindowHeightInch * 0.5, idExplorer);
  if (useItem >= 0) {
    manaphyMove(makeUseItemAttempt(useItem));
  }
  int useMove = drawMoveList(&manaphy, Window43Right, 0, idExplorer);
  if (useMove >= 0) {
    manaphyMove(makeUseMoveAttempt(useMove));
  }

  // hint dialog

  drawHintDialog();

  drawToolsBar();
}

void stopExplorer() { clearHint(); }

void uiExplorerGetKeyboard(int key, int event) {
  controlKeyboard(key, event);
  uiGetKeyboard(key, event);
}

void uiExplorerGetChar(int ch) {
  if (smStateTop()->uid == idExplorer)
    uiGetChar(ch);
}

void uiExplorerGetMouse(int x, int y, int button, int event) {
  if (smStateTop()->uid == idExplorer)
    uiGetMouse(x, y, button, event);
}

AppState Explorer = {
    idExplorer,        initExplorer,          drawExplorer,
    stopExplorer,      uiExplorerGetKeyboard, uiExplorerGetChar,
    uiExplorerGetMouse};

void gotoExplorer() {
  expHasReadDungeon = 0;
  smPushState(&Explorer);
}