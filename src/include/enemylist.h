#pragma once

#include "utils.h"
#include "items.h"
#include "pokemon.h"

#define MaxEnemyCount (200)

typedef struct EnemyList {
  Pokemon enemy[MaxEnemyCount];
  Item item[MaxEnemyCount];
  size_t count;
} EnemyList;

void clearEnemyList(EnemyList *enemyList) { enemyList->count = 0; }

int isEnemyListEmpty(EnemyList *enemyList) { return enemyList->count <= 0; }

void emplaceEnemyList(EnemyList *enemyList, Pokemon pokemon) {
  if (enemyList->count < MaxEnemyCount) {
    enemyList->enemy[enemyList->count] = pokemon;
    enemyList->item[enemyList->count].type = 0;
    enemyList->count++;
  }
}

void emplaceEnemyListWithItem(EnemyList *enemyList, Pokemon pokemon,
                              Item item) {
  if (enemyList->count < MaxEnemyCount) {
    enemyList->enemy[enemyList->count] = pokemon;
    enemyList->item[enemyList->count] = item;
    enemyList->count++;
  }
}

int isOnEnemyList(EnemyList *enemyList, int x, int y) {
  for (size_t i = 0; i < enemyList->count; i++) {
    if (enemyList->enemy[i].x == x && enemyList->enemy[i].y == y) {
      return 1;
    }
  }
  return 0;
}

int locationEnemyList(EnemyList *enemyList, int x, int y) {
  for (size_t i = 0; i < enemyList->count; i++) {
    if (enemyList->enemy[i].x == x && enemyList->enemy[i].y == y) {
      return i;
    }
  }
  return -1;
}

void removeAtEnemyList(EnemyList *enemyList, size_t id) {
  if (id >= enemyList->count)
    return;
  for (size_t i = id; i < enemyList->count; i++) {
    enemyList->enemy[i] = enemyList->enemy[i + 1];
    enemyList->item[i] = enemyList->item[i + 1];
  }
  enemyList->count--;
}

void setItemAtEnemyList(EnemyList *enemyList, size_t id, Item item) {
  if (id >= enemyList->count)
    return;
  enemyList->item[id] = item;
}