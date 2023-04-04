#pragma once

#include <stdio.h>
#include <string.h>

#define MaxDungeonWidth (200)
#define MaxDungeonHeight (200)

typedef enum LandType { Plain, Block, Start, End } LandType;

typedef enum LandEventType {
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
  struct LandEvent *nex;
} LandEvent;

typedef struct Dungeon {
  int width, height;
  int mp[MaxDungeonWidth][MaxDungeonHeight];
  LandEvent *event[MaxDungeonWidth][MaxDungeonHeight];
} Dungeon;

Dungeon currentDungeon;

int isInDungeon(Dungeon *dungeon, int x, int y) {
  return (x >= 0 && x < dungeon->width && y >= 0 && y < dungeon->height);
}

void deleteDungeonCellEvent(Dungeon *dungeon, int x, int y) {
  if (!isInDungeon(dungeon, x, y))
    return;

  LandEvent *now = dungeon->event[x][y], *nex;
  while (now) {
    nex = now->nex;
    free(now);
    now = nex;
  }

  dungeon->event[x][y] = NULL;
}

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
      int cnt;
      LandEvent *hed = NULL, *now = NULL;
      fscanf(file, "%d", &cnt);
      for (int k = 0; k < cnt; k++) {
        int type, arg;
        fscanf(file, "%d%d", &type, &arg);
        LandEvent *val = (LandEvent *)malloc(sizeof(LandEvent));
        val->type = type;
        val->arg = arg;
        val->nex = NULL;
        if (hed == NULL) {
          hed = now = val;
        } else
          now->nex = val;
        now = val;
      }
      dungeon->event[i][j] = hed;
    }
  }
}

void saveDungeon(Dungeon *dungeon, FILE *file) {
  fprintf(file, "%d %d\n", dungeon->width, dungeon->height);

  for (int i = 0; i < dungeon->width; i++) {
    for (int j = 0; j < dungeon->height; j++) {
      fprintf(file, "%d ", dungeon->mp[i][j]);
      int cnt = 0;
      for (LandEvent *x = dungeon->event[i][j]; x; x = x->nex)
        cnt++;
      printf("%d", cnt);
      for (LandEvent *x = dungeon->event[i][j]; x; x = x->nex) {
        printf(" %d %d", x->type, x->arg);
      }
      puts("");
    }
  }
}
