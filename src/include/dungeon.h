#pragma once

#include <stdio.h>
#include <string.h>
#include "random.h"

#define MaxDungeonWidth (50)
#define MaxDungeonHeight (50)

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

void getDungeonStart(Dungeon *dungeon, int *_x, int *_y) {
  *_x = *_y = -1;
  for (int x = 0; x < dungeon->width; x++) {
    for (int y = 0; y < dungeon->height; y++) {
      if (dungeon->mp[x][y] == Start) {
        *_x = x;
        *_y = y;
        return;
      }
    }
  }
}

void getDungeonEnd(Dungeon *dungeon, int *_x, int *_y) {
  *_x = *_y = -1;
  for (int x = 0; x < dungeon->width; x++) {
    for (int y = 0; y < dungeon->height; y++) {
      if (dungeon->mp[x][y] == End) {
        *_x = x;
        *_y = y;
        return;
      }
    }
  }
}

void setDungeonStart(Dungeon *dungeon, int sx, int sy) {
  if (!isInDungeon(dungeon, sx, sy))
    return;
  int x, y;
  getDungeonStart(dungeon, &x, &y);
  if (x >= 0 && y >= 0)
    dungeon->mp[x][y] = Plain;
  dungeon->mp[sx][sy] = Start;
}

void setDungeonEnd(Dungeon *dungeon, int sx, int sy) {
  if (!isInDungeon(dungeon, sx, sy))
    return;
  int x, y;
  getDungeonEnd(dungeon, &x, &y);
  if (x >= 0 && y >= 0)
    dungeon->mp[x][y] = Plain;
  dungeon->mp[sx][sy] = End;
}

void setDungeonSize(Dungeon *dungeon, int w, int h) {
  if (w < 2)
    w = 2;
  if (w > MaxDungeonWidth)
    w = MaxDungeonWidth;
  if (h < 2)
    h = 2;
  if (h > MaxDungeonHeight)
    h = MaxDungeonHeight;

  int sx, sy, tx, ty;
  getDungeonStart(dungeon, &sx, &sy);
  getDungeonEnd(dungeon, &tx, &ty);

  if (sx >= w || sy >= h) {
    setDungeonStart(dungeon, w - 1, h - 1);
  }
  if (tx >= w || ty >= h) {
    setDungeonEnd(dungeon, w - 1, h - 1 - (dungeon->mp[w - 1][h - 1] == Start));
  }
  dungeon->width = w;
  dungeon->height = h;
}

void setDefaultDungeon(Dungeon *dungeon) {
  dungeon->width = MaxDungeonWidth;
  dungeon->height = MaxDungeonHeight;

  for (int i = 0; i < MaxDungeonWidth; i++) {
    for (int j = 0; j < MaxDungeonHeight; j++) {
      dungeon->mp[i][j] = Plain;
      deleteDungeonCellEvent(dungeon, i, j);
    }
  }

  dungeon->width = dungeon->height = 2;
  setDungeonStart(dungeon, 0, 0);
  setDungeonEnd(dungeon, 1, 1);
}

void randomizeDungeon(Dungeon *dungeon) {
  for (int i = 0; i < dungeon->width; i++) {
    for (int j = 0; j < dungeon->height; j++) {
      deleteDungeonCellEvent(dungeon, i, j);
      dungeon->mp[i][j] = RandomChance(0.35) ? Block : Plain;
    }
  }
  int x, y, sx, sy;
  sx = x = RandomInteger(0, dungeon->width - 1);
  sy = y = RandomInteger(0, dungeon->height - 1);
  setDungeonStart(dungeon, x, y);
  while (sx == x && sy == y) {
    x = RandomInteger(0, dungeon->width - 1);
    y = RandomInteger(0, dungeon->height - 1);
  }
  setDungeonEnd(dungeon, x, y);
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

  fclose(file);
}

void saveDungeon(Dungeon *dungeon, FILE *file) {
  fprintf(file, "%d %d\n", dungeon->width, dungeon->height);

  for (int i = 0; i < dungeon->width; i++) {
    for (int j = 0; j < dungeon->height; j++) {
      fprintf(file, "%d ", dungeon->mp[i][j]);
      int cnt = 0;
      for (LandEvent *x = dungeon->event[i][j]; x; x = x->nex)
        cnt++;
      fprintf(file, "%d", cnt);
      for (LandEvent *x = dungeon->event[i][j]; x; x = x->nex) {
        fprintf(file, " %d %d", x->type, x->arg);
      }
      fprintf(file, "\n");
    }
  }

  fclose(file);
}
