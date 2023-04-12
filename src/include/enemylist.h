#pragma once

#include "utils.h"
#include "items.h"
#include "pokemon.h"

#define MaxEnemyCount (100) // 敌人的数量上限。

typedef struct EnemyList {
  Pokemon enemy[MaxEnemyCount + 1]; // 敌人的数据。
  Item item[MaxEnemyCount + 1];     // 敌人携带的道具。
  size_t count;                     // 敌人的数量。
} EnemyList;                        // 存储敌人列表的数据结构。

void clearEnemyList(EnemyList *enemyList);

int isEnemyListEmpty(EnemyList *enemyList);

void emplaceEnemyList(EnemyList *enemyList, Pokemon pokemon);

void emplaceEnemyListWithItem(EnemyList *enemyList, Pokemon pokemon, Item item);

int isOnEnemyList(EnemyList *enemyList, int x, int y);

int locationEnemyList(EnemyList *enemyList, int x, int y);

void removeAtEnemyList(EnemyList *enemyList, size_t id);

void setItemAtEnemyList(EnemyList *enemyList, size_t id, Item item);