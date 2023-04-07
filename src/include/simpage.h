#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "random.h"
#include "appstate.h"
#include "statemanager.h"
#include "utils.h"
#include "pokemon.h"
#include "dungeon.h"
#include "drawdungeon.h"
#include "solvemodel.h"
#include "controller.h"
#include "helplist.h"

#include "alertdialog.h"
#include "getfilenamedialog.h"
#include "hintvalue.h"

#include "globalvalue.h"

Dungeon simDungeon;
DungeonSolution simHistory;
char simDungeonFileName[MaxFileNameLength + 1];
int simHasReadDungeon;

double simCellSize;
Pokemon simCursor, cresselia;
double simMouseX, simMouseY;

int isMouseDownSimPage;
int isJumpedSimPage;

int simulateSpeed;
int isAutoSimulating;
int isCursorFollowCresselia;

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

Direction cresseliaAttempt() {
  int tx, ty;
  getDungeonEnd(&simDungeon, &tx, &ty);
  if (cresselia.x == tx && cresselia.y == ty)
    return NODIRECTION;

  Direction ret = ERRORDIRECTION;
  int minDistan = inf - 1;
  for (int i = 0; i < 4; i++) {
    int dx = cresselia.x + go[i][0], dy = cresselia.y + go[i][1];
    if (dx >= 0 && dx < simDungeon.width && dy >= 0 && dy < simDungeon.height) {
      if (simDungeon.mp[dx][dy] == Block)
        continue;
      int distan = getDungeonDistance(&simDungeon, dx, dy, tx, ty);
      if (distan < minDistan || (distan == minDistan && RandomChance(0.5))) {
        ret = i;
        minDistan = distan;
      }
    }
  }

  return ret;
}

void cresseliaMove() {
  Direction dir = cresseliaAttempt();
  if (dir != NODIRECTION && dir != ERRORDIRECTION) {
    int x = cresselia.x + go[dir][0], y = cresselia.y + go[dir][1];
    simHistory.mp[cresselia.x][cresselia.y] = 1;
    RouteNode *node = newRouteNode(x, y);
    if (simHistory.route == NULL)
      simHistory.route = newRouteNode(cresselia.x, cresselia.y);
    RouteNode *now = simHistory.route;
    while (now->nex != NULL) {
      now = now->nex;
    }
    now->nex = node;
    cresselia.x = x;
    cresselia.y = y;
    cresselia.direction = dir;
  } else if (dir == ERRORDIRECTION) {
    clearHint();
    static char _failed[200];
    sprintf(_failed, "%s: I cannot find a way to get out!", cresselia.name);
    setHint(_failed);
  }

  checkCresseliaHealth();
  if (simDungeon.mp[cresselia.x][cresselia.y] == End) {
    clearHint();
    static char _success[200];
    sprintf(_success, "%s has successfully conquered this dungeon!",
            cresselia.name);
    setHint(_success);
    cancelTimer(HintExpire);
    endAutoSimulating();
    isDungeonSimTerminated = 1;
  }
}

void initSimPage() {
  if (!simHasReadDungeon) {
    simCellSize = 1;
    simDungeon = currentDungeon;
    strcpy(simDungeonFileName, editDungeonFileName);
    clearDungeonSolution(&simHistory);
    simHistory.routeValid = 1;

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
    simulateSpeed = 100;
    isCursorFollowCresselia = 0;
  }
  clearHelpList();
  addHelpEntry("Move Camera:", "");
  addHelpEntry("", "Arrow or WASD");
  addHelpEntry("Jump to Cell:", "Right Click");
  addHelpEntry("Zoom:", "Mouse Wheel");
  clearHint();

  isMouseDownSimPage = 0;
  isJumpedSimPage = 0;
  bindPlayerMove(playerMoveSimPage);
  bindAutoMove(cresseliaMove);
}

void startAutoSimulating() {
  startTimer(AutoRun, 20000.00 / simulateSpeed);
  isAutoSimulating = 1;
}

void endAutoSimulating() {
  cancelTimer(AutoRun);
  isAutoSimulating = 0;
}

void setSimulateSpeed(int speed) {
  if (speed != simulateSpeed) {
    if (isAutoSimulating) {
      cancelTimer(AutoRun);
      startTimer(AutoRun, 20000.00 / speed);
    }
    simulateSpeed = speed;
  }
}

void drawSimPage() {
  if (isCursorFollowCresselia) {
    simCursor.x = cresselia.x;
    simCursor.y = cresselia.y;
  }

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
  drawDungeonPokemon(&simDungeon, simCursor.x, simCursor.y, simCellSize,
                     &cresselia);

  // title

  SetPenColor("White");
  drawBox(Window43Left, 0, Window43Right - Window43Left,
          WindowHeightInch * 0.03, 1, simDungeonFileName, 'L', "Black");

  // status bar

  SetPenColor("Yellow");
  drawRectangle(0, 0, Window43Left, WindowHeightInch, 1);

  drawHelpList(0, WindowHeightInch * 0.9);

  drawStatusBar(&cresselia, 0, WindowHeightInch * 0.01);

  char __buf[99];
  drawBoxWithoutBorder(Window43Gap * 0.04, WindowHeightInch * 0.42,
                       Window43Gap * 0.92, WindowHeightInch * 0.03, 0,
                       "Simulate Speed", 'L', "Black");

  sprintf(__buf, "%d%%", simulateSpeed);
  SetPenColor("White");
  drawBox(Window43Gap * 0.3, WindowHeightInch * 0.39, Window43Gap * 0.4,
          WindowHeightInch * 0.03, 1, __buf, 'C', "Black");
  setButtonColors("White", "Blue", "Blue", "White", 1);
  int nextSimulateSpeed = simulateSpeed;
  if (button(GenUIID(0), Window43Gap * 0.70, WindowHeightInch * 0.39,
             Window43Gap * 0.1, WindowHeightInch * 0.03, "+")) {
    nextSimulateSpeed += 10;
  }
  if (button(GenUIID(0), Window43Gap * 0.84, WindowHeightInch * 0.39,
             Window43Gap * 0.11, WindowHeightInch * 0.03, "++")) {
    nextSimulateSpeed += 100;
  }
  if (button(GenUIID(0), Window43Gap * 0.20, WindowHeightInch * 0.39,
             Window43Gap * 0.1, WindowHeightInch * 0.03, "-")) {
    nextSimulateSpeed -= 10;
  }
  if (button(GenUIID(0), Window43Gap * 0.05, WindowHeightInch * 0.39,
             Window43Gap * 0.11, WindowHeightInch * 0.03, "--")) {
    nextSimulateSpeed -= 100;
  }
  if (nextSimulateSpeed < 20)
    nextSimulateSpeed = 20;
  if (nextSimulateSpeed > 2000)
    nextSimulateSpeed = 2000;

  setSimulateSpeed(nextSimulateSpeed);

  setButtonColors(isAutoSimulating ? "Magenta" : "White", "Blue", "Blue",
                  "White", 1);
  if (button(GenUIID(0), Window43Gap * 0.05, WindowHeightInch * 0.35,
             Window43Gap * 0.9, WindowHeightInch * 0.03, "Auto Run")) {
    if (isAutoSimulating) {
      endAutoSimulating();
    } else {
      startAutoSimulating();
    }
  }

  setButtonColors("White", "Blue", "Blue", "White", 1);
  if (button(GenUIID(0), Window43Gap * 0.05, WindowHeightInch * 0.31,
             Window43Gap * 0.9, WindowHeightInch * 0.03, "Single Step")) {
    if (!isAutoSimulating) {
      cresseliaMove();
    }
  }

  setButtonColors(isCursorFollowCresselia ? "Magenta" : "White", "Blue", "Blue",
                  "White", 1);
  if (button(GenUIID(0), Window43Gap * 0.05, WindowHeightInch * 0.27,
             Window43Gap * 0.9, WindowHeightInch * 0.03, "Lock Camera")) {
    isCursorFollowCresselia = !isCursorFollowCresselia;
  }

  // tools bar

  SetPenColor("Yellow");
  drawRectangle(Window43Right, 0, Window43Gap, WindowHeightInch, 1);
  drawMoveList(&cresselia, Window43Right, 0);

  // hint dialog

  drawHintDialog();

  drawToolsBar();
}

void uiSimPageGetMouse(int x, int y, int button, int event) {
  if (event != ROLL_DOWN && event != ROLL_UP) {
    simMouseX = ScaleXInches(x);
    simMouseY = ScaleYInches(y);
  }

  int mx, my;
  getCellLocation(&simDungeon, simCursor.x, simCursor.y, simCellSize, simMouseX,
                  simMouseY, &mx, &my);
  if (event == MOUSEMOVE) {
  } else if (event == BUTTON_DOWN && button == LEFT_BUTTON) {
    isMouseDownSimPage = 1;
  } else if (event == BUTTON_DOWN && button == RIGHT_BUTTON) {
    if (!isJumpedSimPage && mx >= 0 && my >= 0) {
      simCursor.x = mx;
      simCursor.y = my;
      isJumpedSimPage = 1;
    }
  } else if (event == BUTTON_UP && button == LEFT_BUTTON) {
    isMouseDownSimPage = 0;
  } else if (event == BUTTON_UP && button == RIGHT_BUTTON) {
    isJumpedSimPage = 0;
  } else if (event == ROLL_DOWN) {
    if (simCellSize > 0.30) {
      simCellSize -= 0.10;
    }
  } else if (event == ROLL_UP) {
    if (simCellSize < 3.00) {
      simCellSize += 0.10;
    }
  }

  uiGetMouse(x, y, button, event);
}

void uiSimPageGetKeyboard(int key, int event) {
  controlKeyboard(key, event);
  uiGetKeyboard(key, event);
}

AppState SimPage = {
    idSimPage, initSimPage,      drawSimPage, NULL, uiSimPageGetKeyboard,
    uiGetChar, uiSimPageGetMouse};

void gotoSimPage() {
  simHasReadDungeon = 0;
  smPushState(&SimPage);
}