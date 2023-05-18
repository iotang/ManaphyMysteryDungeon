#include "dungeonprocess.h"

#include "utils.h"
#include "items.h"
#include "messagedialog.h"
#include "pausepage.h"

void landEventCalc(Dungeon *dungeon, Pokemon *pokemon) {
  int x = pokemon->x, y = pokemon->y;
  if (dungeon->event[x][y].type == Lock) { // 判断能不能开锁
    static char _openLock[200];
    sprintf(_openLock, "%s opened the %s.", pokemon->name,
            landEventsData[dungeon->event[x][y].type].name);
    emplaceMessage(_openLock);
    dungeon->mp[x][y] = Plain;
  } else if (dungeon->event[x][y].type == Damage ||
             dungeon->event[x][y].type == DamageOT) { // 受到伤害
    static char _hurt[200];
    sprintf(_hurt, "%s receives %d damage!", pokemon->name,
            dungeon->event[x][y].arg);
    emplaceMessage(_hurt);
    pokemon->hp -= dungeon->event[x][y].arg;
  } else if (dungeon->event[x][y].type == HealOT) { // 回复地板
    static char _heal[200];
    sprintf(_heal, "%s recovers %d HP.", pokemon->name,
            dungeon->event[x][y].arg);
    emplaceMessage(_heal);
    pokemon->hp += dungeon->event[x][y].arg;
  }

  if (!landEventsData[dungeon->event[x][y].type]
           .permanent) { // 是否是一次性事件
    dungeon->event[x][y].type = None;
  }
}

void pokemonStepOn(Dungeon *dungeon, Pokemon *pokemon, ItemBag *itemBag) {
  int x = pokemon->x, y = pokemon->y;
  if (!isInDungeon(dungeon, x, y))
    return;

  if (dungeon->item[x][y].type != INone) { // 捡拾道具
    static char _pickItem[200];
    sprintf(_pickItem, "%s finds a %s, and %s puts it into the bag.",
            pokemon->name, itemsData[dungeon->item[x][y].type].name,
            pokemon->gender ? "he" : "she");
    emplaceMessage(_pickItem);
    addIntoItemBag(itemBag, dungeon->item[x][y]);
    dungeon->item[x][y].type = INone;
  }

  landEventCalc(dungeon, pokemon); // 场地事件
  while (updatePokemonStat(pokemon))
    ;
}

void pokemonEnemyStepOn(Dungeon *dungeon, Pokemon *pokemon, Item *item) {
  int x = pokemon->x, y = pokemon->y;
  if (!isInDungeon(dungeon, x, y))
    return;

  if (dungeon->item[x][y].type != INone) { // 捡拾道具
    static char _pickItem[200];
    if (item->type == INone) {
      sprintf(_pickItem, "%s picks up a %s.", pokemon->name,
              itemsData[dungeon->item[x][y].type].name);
      emplaceMessage(_pickItem);
      *item = dungeon->item[x][y];
      dungeon->item[x][y].type = INone;
      makePause(0.5);
    } else {
      sprintf(_pickItem, "%s ignores a %s.", pokemon->name,
              itemsData[dungeon->item[x][y].type].name);
      emplaceMessage(_pickItem);
    }
  }

  // landEventCalc(dungeon, pokemon); // 敌人不触发场地事件
}
