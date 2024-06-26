#include "explorer.h"

#include <math.h>
#include <time.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "statemanager.h"
#include "pokemon.h"
#include "dungeon.h"
#include "drawdungeon.h"
#include "dungeonprocess.h"
#include "controller.h"
#include "itembag.h"
#include "attempt.h"
#include "enemylist.h"

#include "pages.h"
#include "editpage.h"
#include "alertdialog.h"
#include "getfilenamedialog.h"
#include "messagedialog.h"
#include "statusbar.h"
#include "drawitembag.h"
#include "pausepage.h"
#include "helplist.h"

#include "globalvalue.h"

int expHasReadDungeon; // 是否已经读取了迷宫。
Dungeon expDungeon; // 游玩页面的迷宫，和之前的 editDungeon 独立。
char expDungeonFileName[MaxFileNameLength + 1]; // 游玩页面的迷宫名字。

Pokemon manaphy;        // 玛纳霏。
ItemBag manaphyItemBag; // 玛纳霏的道具背包。

EnemyList enemyList;  // 敌人列表。
int isEnemyMove;      // 是否处于敌人的回合。
int spawnEnemyCount;  // 生成敌人的数量。
int isAutoSpawnEnemy; // 是否随回合自动生成敌人。

void checkManaphyHealth() {
  while (updatePokemonStat(&manaphy))
    ;
  if (manaphy.hp <= 0) {
    clearMessage();
    static char _failed[200];
    sprintf(_failed, "Oh no! %s is fainted!", manaphy.name);
    setMessage(_failed);
    cancelTimer(MessageExpire);
    isDungeonGameOver = 1;
  }
}

int manhattanDistance(int x1, int y1, int x2, int y2) {
  return abs(x1 - x2) + abs(y1 - y2);
}

int enemyCanStepIn(int dx, int dy) {
  return isInDungeon(&expDungeon, dx, dy) && expDungeon.mp[dx][dy] != Block &&
         expDungeon.event[dx][dy].type != Lock &&
         (dx != manaphy.x || dy != manaphy.y) &&
         !isOnEnemyList(&enemyList, dx, dy); // 不是墙，没上锁，并且没人在上面
}

int spawnSingleEnemy(int distance) {
  for (int i = 0; i < 10; i++) { // 尝试十次
    int x = RandomInteger(0, expDungeon.width);
    int y = RandomInteger(0, expDungeon.height);
    if (expDungeon.mp[x][y] == Block || expDungeon.mp[x][y] == Start ||
        expDungeon.event[x][y].type == Lock)
      continue;
    if (isOnEnemyList(&enemyList, x, y)) // 相同位置生成过
      continue;
    if (manhattanDistance(x, y, manaphy.x, manaphy.y) <= 11) // 距离玛纳霏太近
      continue;
    Pokemon enemy;
    spawnPokemon(&enemy, Enemy, NRemoraid, RandomChance(0.5));
    enemy.x = x;
    enemy.y = y;
    enemy.exp =
        (manaphy.lv - RandomInteger(2, 4)) * 100.00; // 根据玛纳霏等级生成
    if (enemy.exp < 0)
      enemy.exp = 0;
    while (updatePokemonStat(&enemy))
      ;
    enemy.direction = RandomInteger(0, 3);
    Item item;
    item.type = RandomInteger(0, 1) * IOranBerry; // 随机带一个橙橙果
    emplaceEnemyListWithItem(&enemyList, enemy, item);
    return 1;
  }
  return 0;
}

void enemyRound() {
  isEnemyMove = 1;
  for (size_t i = 0; i < enemyList.count; i++) {
    Pokemon *who = &enemyList.enemy[i];
    Item *item = &enemyList.item[i];

    int distan = manhattanDistance(who->x, who->y, manaphy.x, manaphy.y);

    int madeDecision = 0;

    if (distan <= 1) { // 准备攻击
      if (who->x == manaphy.x - 1) {
        who->direction = RIGHT;
      } else if (who->x == manaphy.x + 1) {
        who->direction = LEFT;
      } else if (who->y == manaphy.y - 1) {
        who->direction = UP;
      } else if (who->y == manaphy.y + 1) {
        who->direction = DOWN;
      }

      int able[MaxMoveCount], ableCount = 0;
      for (int i = 0; i < who->moveCount; i++) {
        if (movedex[who->move[i]].pp < 0 || who->pp[i] > 0) {
          able[ableCount++] = i; // 能用的招式
        }
      }
      if (ableCount > 0) {
        if (who->direction == RIGHT) {
          manaphy.direction = LEFT;
        } else if (who->direction == LEFT) {
          manaphy.direction = RIGHT;
        } else if (who->direction == UP) {
          manaphy.direction = DOWN;
        } else if (who->direction == DOWN) {
          manaphy.direction = UP;
        } // 玛纳霏面对敌方

        int index = able[RandomInteger(0, ableCount - 1)]; // 选一个招式
        int move = who->move[index];
        static char _useMove[99];
        sprintf(_useMove, "%s uses %s!", who->name, movedex[move].name);
        emplaceMessage(_useMove);
        who->pp[index]--;
        makePause(0.5);

        int dmg =
            calcDamage(who->lv, movedex[move].effect, who->atk, manaphy.def) *
            RandomReal(0.85, 1);
        if (dmg < 1)
          dmg = 1;
        sprintf(_useMove, "%s takes %d damage!", manaphy.name, dmg);
        emplaceMessage(_useMove);
        manaphy.hp -= dmg;
        makePokemonStatBound(&manaphy);
        makePause(0.5);
        checkManaphyHealth();
        if (isDungeonGameOver)
          return;
        madeDecision = 1;
      }
    }

    if (isDungeonGameOver)
      return;

    if (!madeDecision) {
      if (distan > 10) { // 无法索敌，朝当前朝向走
        int dx = who->x + go[who->direction][0],
            dy = who->y + go[who->direction][1];
        if (enemyCanStepIn(dx, dy)) {
          who->x = dx;
          who->y = dy;
          madeDecision = 1;
        }
      }

      if (!madeDecision) { // 没走，换个方向
        Direction ret = who->direction;
        lint minDistan = linf - 1;
        for (int j = 0; j < 4; j++) {
          int dx = who->x + go[j][0], dy = who->y + go[j][1];
          if (isInDungeon(&expDungeon, dx, dy)) {
            if (expDungeon.mp[dx][dy] == Block)
              continue;
            lint distan = getDungeonDistance(&expDungeon, dx, dy, 0, manaphy.x,
                                             manaphy.y, DefaultHPPenalty, 0);
            if (distan < minDistan ||
                (distan == minDistan && RandomChance(0.5))) {
              ret = j; // 自瞄
              minDistan = distan;
            }
          }
        }
        who->direction = ret;
        int dx = who->x + go[ret][0], dy = who->y + go[ret][1];
        if (enemyCanStepIn(dx, dy)) {
          who->x = dx;
          who->y = dy;
        }
        madeDecision = 1;
      }
    }

    if (isDungeonGameOver)
      return;

    pokemonEnemyStepOn(&expDungeon, who, item);
    if (who->hp <= 0) {
      static char _failed[200];
      sprintf(_failed, "%s is fainted.", who->name);
      setMessage(_failed);
      who->x = who->y = -1;
    }

    if (isDungeonGameOver)
      return;
  }

  for (int i = 0; i < enemyList.count; i++) {
    if (enemyList.enemy[i].hp <= 0) { // 被打倒的敌人
      removeAtEnemyList(&enemyList, i);
      i--;
    }
  }

  if (isAutoSpawnEnemy && RandomChance(1.00 / (8 + 0.5 * enemyList.count))) {
    spawnSingleEnemy(11); // 随机生成敌人
  }

  isEnemyMove = 0;
}

int emptyBellyMessageCount; // 饱腹度空时提示语的播报顺序。

void decreaseManaphyBelly(double val) {
  manaphy.belly -= val;
  if (manaphy.belly <= 0) {
    manaphy.belly = 0;
    manaphy.hp--;
    if (emptyBellyMessageCount == 0) {
      emplaceMessage("Oh, no! Your belly is empty!");
    } else if (emptyBellyMessageCount == 1) {
      emplaceMessage("Hurry up! You must have something to eat!");
    } else if (emptyBellyMessageCount == 2) {
      emplaceMessage("Otherwise you will fall soon!");
    }
    emptyBellyMessageCount++;
  } else {
    emptyBellyMessageCount = 0;
  }
  makePokemonStatBound(&manaphy);
}

int manaphyMove(int att) {
  if (isDungeonGameOver)
    return 0;
  if (isEnemyMove)
    return 0;

  if (isFaceAttempt(att)) { // 朝向
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

  if (isMoveAttempt(att)) { // 移动
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
        emplaceMessage("You don't have the right key to open it.");
      } else {
        successMove = 1;
      }
    } else if (expDungeon.mp[dx][dy] == Block) {
      // emplaceMessage("You cannot move into a block.");
    } else if (isOnEnemyList(&enemyList, dx, dy)) {
      emplaceMessage("Use moves to take the enemies down.");
    } else {
      successMove = 1;
    }

    if (successMove) {
      manaphy.x = dx;
      manaphy.y = dy;
    }

    return successMove;
  }

  if (isUseItemAttempt(att)) { // 使用道具
    int index = argUseItemAttempt(att);
    int type = manaphyItemBag.items[index].type;
    int arg = manaphyItemBag.items[index].arg;

    static char _useItem[99];

    if (type == IKey) {
      sprintf(_useItem, "Walk into the lock to use the key.");
      emplaceMessage(_useItem);
      return 0;
    } else if (type == ITM) {
      for (int i = 1; i < manaphy.moveCount; i++) {
        if (manaphy.move[i] == arg) {
          sprintf(_useItem, "%s's PP is maxed out!", movedex[arg].name);
          emplaceMessage(_useItem);
          manaphy.pp[i] = movedex[arg].pp;
          removeOutItemBag(&manaphyItemBag, index);
          return 1;
        }
      }

      if (manaphy.moveCount < MaxMoveCount) {
        sprintf(_useItem, "%s has learned %s!", manaphy.name,
                movedex[arg].name);
        emplaceMessage(_useItem);
        manaphy.move[manaphy.moveCount] = arg;
        manaphy.pp[manaphy.moveCount] = movedex[arg].pp;
        manaphy.moveCount++;
        removeOutItemBag(&manaphyItemBag, index);
        return 1;
      } else {
        sprintf(_useItem,
                "%s cannot learn more moves. Unlearn some moves first.",
                manaphy.name);
        emplaceMessage(_useItem);
        return 0;
      }
    } else {
      sprintf(_useItem, "%s uses %s.", manaphy.name, itemsData[type].name);
      emplaceMessage(_useItem);

      makePause(0.5);

      if (itemsData[type].dbelly != 0) {
        if (itemsData[type].dbelly > 49.5) {
          sprintf(_useItem, "%s's belly is filled!", manaphy.name);
          emplaceMessage(_useItem);
        } else if (itemsData[type].dbelly > 9.5) {
          sprintf(_useItem, "%s's belly is slightly filled!", manaphy.name);
          emplaceMessage(_useItem);
        } else if (itemsData[type].dbelly < 0) {
          sprintf(_useItem, "%s's belly dropped!", manaphy.name);
          emplaceMessage(_useItem);
        }
        manaphy.belly += itemsData[type].dbelly;
        makePause(0.2);
      }

      if (itemsData[type].dmaxhp) {
        sprintf(_useItem, "%s's max HP %s by %d!", manaphy.name,
                itemsData[type].dmaxhp > 0 ? "raised" : "dropped",
                abs(itemsData[type].dmaxhp));
        emplaceMessage(_useItem);
        manaphy.maxhp += itemsData[type].dmaxhp;
        makePause(0.2);
      }

      makePokemonStatBound(&manaphy);

      if (itemsData[type].dhp) {
        if (itemsData[type].dhp > 0) {
          int recover = manaphy.maxhp - manaphy.hp;
          if (recover > itemsData[type].dhp)
            recover = itemsData[type].dhp;
          if (recover > 0) {
            sprintf(_useItem, "%s recovers %d HP!", manaphy.name, recover);
          } else {
            sprintf(_useItem, "%s's HP doesn't changed.", manaphy.name);
          }
        } else {
          sprintf(_useItem, "%s receives %d damage!", manaphy.name,
                  -itemsData[type].dhp);
        }
        emplaceMessage(_useItem);
        manaphy.hp += itemsData[type].dhp;
        makePause(0.2);
      }

      if (itemsData[type].datk) {
        sprintf(_useItem, "%s's attack %s by %d!", manaphy.name,
                itemsData[type].datk > 0 ? "raised" : "dropped",
                abs(itemsData[type].datk));
        emplaceMessage(_useItem);
        manaphy.atk += itemsData[type].datk;
        makePause(0.2);
      }

      if (itemsData[type].ddef) {
        sprintf(_useItem, "%s's defense %s by %d!", manaphy.name,
                itemsData[type].ddef > 0 ? "raised" : "dropped",
                abs(itemsData[type].ddef));
        emplaceMessage(_useItem);
        manaphy.def += itemsData[type].ddef;
        makePause(0.2);
      }

      if (itemsData[type].dexp != 0) {
        sprintf(_useItem, "%s's %s %.0lf EXP!", manaphy.name,
                itemsData[type].dexp > 0 ? "gains" : "loses",
                fabs(itemsData[type].dexp));
        emplaceMessage(_useItem);
        manaphy.exp += itemsData[type].dexp;
        makePause(0.2);
      }

      while (updatePokemonStat(&manaphy)) {
        sprintf(_useItem, "%s raises to level %d!", manaphy.name, manaphy.lv);
        emplaceMessage(_useItem);
      }

      makePause(0.3);

      removeOutItemBag(&manaphyItemBag, index);
      return 1;
    }
  }

  if (isUseMoveAttempt(att)) { // 使用招式
    int index = argUseMoveAttempt(att);
    if (manaphy.moveCount <= index)
      return 0;

    int move = manaphy.move[index];

    static char _useMove[99];

    if (movedex[move].pp > 0 && manaphy.pp[index] <= 0) {
      sprintf(_useMove, "%s is too exhausted to use %s.", manaphy.name,
              movedex[move].name);
      emplaceMessage(_useMove);
      return 0;
    }

    sprintf(_useMove, "%s uses %s!", manaphy.name, movedex[move].name);
    emplaceMessage(_useMove);
    manaphy.pp[index]--;
    makePause(0.5);

    int dx = manaphy.x + go[manaphy.direction][0];
    int dy = manaphy.y + go[manaphy.direction][1];
    int who = locationEnemyList(&enemyList, dx, dy);
    if (who < 0) {
      sprintf(_useMove, "But %s didn't hit anyone.",
              manaphy.gender ? "he" : "she");
      emplaceMessage(_useMove);
      return 1;
    }

    int dmg = calcDamage(manaphy.lv, movedex[move].effect, manaphy.atk,
                         enemyList.enemy[who].def) *
              RandomReal(0.85, 1);
    if (dmg < 1)
      dmg = 1;
    sprintf(_useMove, "%s takes %d damage!", enemyList.enemy[who].name, dmg);
    emplaceMessage(_useMove);
    enemyList.enemy[who].hp -= dmg;
    makePokemonStatBound(&enemyList.enemy[who]);
    makePause(0.5);

    if (enemyList.enemy[who].hp <= 0) {
      sprintf(_useMove, "%s is fainted!", enemyList.enemy[who].name);
      emplaceMessage(_useMove);
      double exp = calcExp(manaphy.lv, enemyList.enemy[who].lv);
      Item getItem = enemyList.item[who];
      removeAtEnemyList(&enemyList, who);
      makePause(0.2);

      sprintf(_useMove, "%s gets %.2lf EXP.", manaphy.name, exp);
      emplaceMessage(_useMove);
      manaphy.exp += exp;
      makePause(0.2);

      while (updatePokemonStat(&manaphy)) {
        sprintf(_useMove, "%s raises to level %d!", manaphy.name, manaphy.lv);
        emplaceMessage(_useMove);
      }

      if (getItem.type != 0) {
        sprintf(_useMove, "A %s drops down, and %s puts it into the bag.",
                itemsData[getItem.type].name, manaphy.name);
        emplaceMessage(_useMove);
        addIntoItemBag(&manaphyItemBag, getItem);
        makePause(0.2);
      }
    }

    return 1;
  }

  if (isRemoveItemAttempt(att)) { // 丢掉道具
    int index = argRemoveItemAttempt(att);
    int type = manaphyItemBag.items[index].type;
    int arg = manaphyItemBag.items[index].arg;

    static char _removeItem[99];
    sprintf(_removeItem, "%s drops the %s on the ground.", manaphy.name,
            itemsData[type].name);
    emplaceMessage(_removeItem);

    expDungeon.item[manaphy.x][manaphy.y].type = type;
    expDungeon.item[manaphy.x][manaphy.y].arg = arg;
    removeOutItemBag(&manaphyItemBag, index);

    return 0;
  }

  if (isRemoveMoveAttempt(att)) { // 遗忘招式
    int index = argRemoveMoveAttempt(att);
    if (index >= manaphy.moveCount)
      return 0;

    int arg = manaphy.move[index];

    static char _removeMove[99];
    if (arg == MTackle) {
      sprintf(_removeMove, "You cannot unlearn %s.", movedex[arg].name);
      emplaceMessage(_removeMove);
      return 0;
    } else {
      sprintf(_removeMove, "%s has unlearned %s!", manaphy.name,
              movedex[arg].name);
      emplaceMessage(_removeMove);
      for (int i = index; i < manaphy.moveCount; i++) {
        manaphy.move[i] = manaphy.move[i + 1];
        manaphy.pp[i] = manaphy.pp[i + 1];
      }
      manaphy.moveCount--;
      return 0;
    }
  }

  return 0;
}

void manaphyRound(int att) { // 玛纳霏回合
  if (!manaphyMove(att))
    return;

  pokemonStepOn(&expDungeon, &manaphy, &manaphyItemBag);
  decreaseManaphyBelly(0.1);

  checkManaphyHealth();
  if (expDungeon.mp[manaphy.x][manaphy.y] == End) {
    clearMessage();
    setMessage("You have successfully conquered this dungeon!");
    cancelTimer(MessageExpire);
    isDungeonGameOver = 1;
  }

  enemyRound();
}

void manaphyMoveAttempt(int event) { // 尝试解码玛纳霏的行动
  if (isDungeonGameOver)
    return;
  if (isEnemyMove)
    return;

  int att = 0;

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
             event == MoveDown || event == MoveNoDirection) {
    if (event == MoveRight) {
      att = makeMoveAttempt(RIGHT);
    } else if (event == MoveUp) {
      att = makeMoveAttempt(UP);
    } else if (event == MoveLeft) {
      att = makeMoveAttempt(LEFT);
    } else if (event == MoveDown) {
      att = makeMoveAttempt(DOWN);
    } else if (event == MoveNoDirection) {
      att = makeMoveAttempt(NODIRECTION);
    }
  } else if (event == UseMove1 || event == UseMove2 || event == UseMove3 ||
             event == UseMove4 || event == UseMove5) {
    if (event == UseMove1) {
      att = makeUseMoveAttempt(0);
    } else if (event == UseMove2) {
      att = makeUseMoveAttempt(1);
    } else if (event == UseMove3) {
      att = makeUseMoveAttempt(2);
    } else if (event == UseMove4) {
      att = makeUseMoveAttempt(3);
    } else if (event == UseMove5) {
      att = makeUseMoveAttempt(4);
    }
  }

  return manaphyRound(att);
}

void giveCheat() {
  if (isDungeonGameOver)
    return;
  if (isEnemyMove)
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
                                       DefaultHPPenalty, 1);
      if (distan < minDistan || (distan == minDistan && RandomChance(0.5))) {
        ret = i;
        minDistan = distan;
      }
    }
  }

  static char _cheat[99];
  if (minDistan < linf - 1) {
    sprintf(_cheat, "Walk %s may good for you.",
            ret == RIGHT
                ? "right"
                : (ret == UP ? "up" : (ret == LEFT ? "left" : "down")));
  } else {
    int gotKey = 0;
    for (int i = 0; i < enemyList.count; i++) {
      if (enemyList.item[i].type == IKey) {
        gotKey = 1; // 有人抢了钥匙导致无解
        tx = enemyList.enemy[i].x;
        ty = enemyList.enemy[i].y;
        break;
      }
    }

    if (!gotKey) {
      emplaceMessage("Something went wrong in this dungeon.");
      emplaceMessage("Is it really solvable?");
      return;
    }

    for (int i = 0; i < 4; i++) {
      int dx = manaphy.x + go[i][0], dy = manaphy.y + go[i][1];
      if (dx >= 0 && dx < expDungeon.width && dy >= 0 &&
          dy < expDungeon.height) {
        if (expDungeon.mp[dx][dy] == Block)
          continue;
        lint distan = getDungeonDistance(&expDungeon, dx, dy, key, tx, ty,
                                         DefaultHPPenalty, 1);
        if (distan < minDistan || (distan == minDistan && RandomChance(0.5))) {
          ret = i;
          minDistan = distan;
        }
      }
    }
    sprintf(_cheat, "Someone has stolen your key! Walk %s to find it!",
            ret == RIGHT
                ? "right"
                : (ret == UP ? "up" : (ret == LEFT ? "left" : "down")));
  }
  emplaceMessage(_cheat);
}

void spawnEnemy() {
  for (int i = 0; i < spawnEnemyCount; i++) {
    spawnSingleEnemy(1);
  }
}

void initExplorer() {
  if (!expHasReadDungeon) {
    expDungeon = currentDungeon;
    strcpy(expDungeonFileName, editDungeonFileName);

    spawnPokemon(&manaphy, Player, NManaphy, Male);
    manaphy.exp = 400; // 5 级
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

    clearEnemyList(&enemyList);

    isDungeonGameOver = 0;
    emptyBellyMessageCount = 0;
    spawnEnemyCount = 5;
    isAutoSpawnEnemy = 0;
    isEnemyMove = 0;
    clearMessage();

    expHasReadDungeon = 1;
  }
  clearHelpList();
  addHelpEntry("Move:", "Arrow or WASD");
  addHelpEntry("Rest:", "Space");
  addHelpEntry("Change Direction:", "");
  addHelpEntry("", "Shift-Arrow or Shift-WASD");
  addHelpEntry("Use Items:", "Click on list");
  addHelpEntry("Use Moves:", "Number 1-5");
  addHelpEntry("Drop Items:", "");
  addHelpEntry("", "Ctrl and Right-click on list");
  addHelpEntry("Unlearn Moves:", "");
  addHelpEntry("", "Ctrl and Right-click on list");
  bindPlayerMove(manaphyMoveAttempt);
}

void drawExplorer() {
  makePokemonStatBound(&manaphy);

  SetPenColor("Gray");
  drawRectangle(Window43Left, 0, Window43Width, WindowHeightInch, 1);

  drawDungeon(&expDungeon, manaphy.x, manaphy.y, runCellSize, 0, 0, NULL, 0);
  if (manaphy.hp > 0 || !isDungeonGameOver) {
    drawDungeonPokemon(&expDungeon, manaphy.x, manaphy.y, runCellSize,
                       &manaphy);
  }
  for (size_t i = 0; i < enemyList.count; i++) {
    drawDungeonPokemon(&expDungeon, manaphy.x, manaphy.y, runCellSize,
                       &enemyList.enemy[i]);
    if (enemyList.item[i].type == IKey) {
      drawDungeonHighlightCellAt(&expDungeon, manaphy.x, manaphy.y, runCellSize,
                                 enemyList.enemy[i].x, enemyList.enemy[i].y,
                                 runCellSize * 0.7, "Red", 0, 0);
    }
  }

  // title

  SetPenColor("White");
  drawBox(Window43Left, WindowHeightInch * 0.97, Window43Width,
          WindowHeightInch * 0.03, 1, expDungeonFileName, 'L', "Black");

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

  drawHelpList(0, WindowHeightInch * 0.97);

  char __buf[99];

  sprintf(__buf, "%d", spawnEnemyCount);
  drawBoxWithoutBorder(Window43Gap * 0.04, WindowHeightInch * 0.63,
                       Window43Gap * 0.92, WindowHeightInch * 0.03, 1,
                       "Spawn Enemy", 'C', "Black");
  SetPenColor("White");
  drawBox(Window43Gap * 0.04, WindowHeightInch * 0.59, Window43Gap * 0.92,
          WindowHeightInch * 0.03, 1, __buf, 'C', "Black");
  setButtonColors("White", "Blue", "Blue", "White", 1);
  if (button(GenUIID(0), Window43Gap * 0.505, WindowHeightInch * 0.55,
             Window43Gap * 0.195, WindowHeightInch * 0.03, "+", idExplorer)) {
    spawnEnemyCount++;
  }
  if (button(GenUIID(0), Window43Gap * 0.71, WindowHeightInch * 0.55,
             Window43Gap * 0.25, WindowHeightInch * 0.03, "+10", idExplorer)) {
    spawnEnemyCount += 10;
  }
  if (button(GenUIID(0), Window43Gap * 0.30, WindowHeightInch * 0.55,
             Window43Gap * 0.195, WindowHeightInch * 0.03, "-", idExplorer)) {
    spawnEnemyCount--;
    if (spawnEnemyCount < 1)
      spawnEnemyCount = 1;
  }
  if (button(GenUIID(0), Window43Gap * 0.04, WindowHeightInch * 0.55,
             Window43Gap * 0.25, WindowHeightInch * 0.03, "-10", idExplorer)) {
    spawnEnemyCount -= 10;
    if (spawnEnemyCount < 1)
      spawnEnemyCount = 1;
  }

  if (button(GenUIID(0), Window43Gap * 0.04, WindowHeightInch * 0.49,
             Window43Gap * 0.92, WindowHeightInch * 0.03, "Spawn",
             idExplorer)) {
    spawnEnemy();
  }
  if (isAutoSpawnEnemy) {
    setButtonColors("Blue", "White", "White", "Blue", 1);
  } else {
    setButtonColors("White", "Blue", "Blue", "White", 1);
  }
  if (button(GenUIID(0), Window43Gap * 0.04, WindowHeightInch * 0.43,
             Window43Gap * 0.92, WindowHeightInch * 0.03,
             isAutoSpawnEnemy ? "Auto Spawn On" : "Auto Spawn Off",
             idExplorer)) {
    isAutoSpawnEnemy ^= 1;
  }

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

  drawStatusBar(&manaphy, 0, WindowHeightInch * 0.01, 1, idExplorer);

  // tools bar
  SetPenColor("Light Pink");
  drawRectangle(Window43Right, 0, Window43Gap, WindowHeightInch, 1);

  int itemAtt = drawItemBag(&manaphyItemBag, Window43Right,
                            WindowHeightInch * 0.5, idExplorer);
  if (itemAtt != 0) {
    manaphyRound(itemAtt);
  }

  int moveAtt = drawMoveList(&manaphy, Window43Right, 0, idExplorer);
  if (moveAtt != 0) {
    manaphyRound(moveAtt);
  }

  // Message dialog

  drawMessageDialog();

  drawToolsBar();
}

void stopExplorer() { clearMessage(); }

void uiExplorerGetKeyboard(int key, int event) {
  controlKeyboard(key, event);
  if (smStateTop()->uid == idExplorer) {
    uiGetKeyboard(key, event);
  }
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