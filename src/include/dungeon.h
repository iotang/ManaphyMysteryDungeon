#pragma once

#include <stdio.h>

#define MaxDungeonWidth (200)
#define MaxDungeonHeight (200)

typedef enum LandType { Plain, Block, Start, End } LandType;

typedef enum LandEventType {
  None,
  Key,
  Lock,
  Damage,
  Heal,
  DamageOT,
  HealOT
} LandEventType;

typedef struct LandEvent {
  int type;
  int arg;
} LandEvent;

typedef struct Dungeon {
  int width, height;
  int mp[MaxDungeonWidth][MaxDungeonHeight];
  LandEvent event[MaxDungeonWidth][MaxDungeonHeight];
} Dungeon;

Dungeon currentDungeon;

void loadDungeon(Dungeon *dungeon, FILE *file) {
  fscanf(file, "%d%d", &dungeon->width, &dungeon->height);

  if (dungeon->width > MaxDungeonWidth)
    dungeon->width = MaxDungeonWidth;
  if (dungeon->width < 1)
    dungeon->width = 2;

  if (dungeon->height > MaxDungeonHeight)
    dungeon->height = MaxDungeonHeight;
  if (dungeon->height < 1)
    dungeon->height = 2;

  for (int i = 0; i < dungeon->width; i++) {
    for (int j = 0; j < dungeon->height; j++) {
      fscanf(file, "%d", &dungeon->mp[i][j]);
      fscanf(file, "%d", &dungeon->event[i][j].type);
      fscanf(file, "%d", &dungeon->event[i][j].arg);
    }
  }
}

void saveDungeon(Dungeon *dungeon, FILE *file) {
  fprintf(file, "%d %d\n", dungeon->width, dungeon->height);

  for (int i = 0; i < dungeon->width; i++) {
    for (int j = 0; j < dungeon->height; j++) {
      fprintf(file, "%d ", dungeon->mp[i][j]);
      fprintf(file, "%d ", dungeon->event[i][j].type);
      fprintf(file, "%d\n", dungeon->event[i][j].arg);
    }
  }
}
