#pragma once

#include "utils.h"
#include "items.h"
#include "itembag.h"
#include "dungeon.h"
#include "pokemon.h"
#include "hintvalue.h"

void pokemonStepOn(Dungeon *dungeon, Pokemon *pokemon, ItemBag *itemBag) {
  int x = pokemon->x, y = pokemon->y;
  if (!isInDungeon(dungeon, x, y))
    return;

  if (dungeon->item[x][y].type != INone) {
    static char _pickItem[200];
    sprintf(_pickItem, "%s finds a %s, and %s puts it into the bag.",
            pokemon->name, itemsData[dungeon->item[x][y].type].name,
            pokemon->gender ? "he" : "she");
    emplaceHint(_pickItem);
    addIntoItemBag(itemBag, dungeon->item[x][y]);
    dungeon->item[x][y].type = INone;
  }

  if (dungeon->event[x][y].type == Lock) {
    static char _openLock[200];
    sprintf(_openLock, "%s opened the %s.", pokemon->name,
            landEventsData[dungeon->event[x][y].type].name);
    emplaceHint(_openLock);
    dungeon->mp[x][y] = Plain;
  } else if (dungeon->event[x][y].type == Damage ||
             dungeon->event[x][y].type == DamageOT) {
    static char _hurt[200];
    sprintf(_hurt, "%s receives %d damage!", pokemon->name,
            dungeon->event[x][y].arg);
    emplaceHint(_hurt);
    pokemon->hp -= dungeon->event[x][y].arg;
  } else if (dungeon->event[x][y].type == HealOT) {
    static char _heal[200];
    sprintf(_heal, "%s recovers %d HP.", pokemon->name,
            dungeon->event[x][y].arg);
    emplaceHint(_heal);
    pokemon->hp += dungeon->event[x][y].arg;
  }

  if (!landEventsData[dungeon->event[x][y].type].permanent) {
    dungeon->event[x][y].type = None;
  }

  while (updatePokemonStat(pokemon))
    ;
}