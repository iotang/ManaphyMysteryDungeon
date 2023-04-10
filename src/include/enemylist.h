#pragma once

#include "utils.h"
#include "items.h"
#include "pokemon.h"

#define MaxEnemyCount (100)

typedef struct EnemyList {
  Pokemon enemy[MaxEnemyCount + 1];
  Item item[MaxEnemyCount + 1];
  size_t count;
} EnemyList;

void clearEnemyList(EnemyList *enemyList);

int isEnemyListEmpty(EnemyList *enemyList);

void emplaceEnemyList(EnemyList *enemyList, Pokemon pokemon);

void emplaceEnemyListWithItem(EnemyList *enemyList, Pokemon pokemon, Item item);

int isOnEnemyList(EnemyList *enemyList, int x, int y);

int locationEnemyList(EnemyList *enemyList, int x, int y);

void removeAtEnemyList(EnemyList *enemyList, size_t id);

void setItemAtEnemyList(EnemyList *enemyList, size_t id, Item item);