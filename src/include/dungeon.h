#pragma once

#include <stdio.h>

#include "items.h"
#include "landevents.h"
#include "random.h"

#define MaxDungeonWidth (50)
#define MaxDungeonHeight (50)

typedef enum LandType { Plain, Block, Start, End } LandType;

typedef struct Dungeon {
  int width, height;
  int mp[MaxDungeonWidth][MaxDungeonHeight];
  LandEvent event[MaxDungeonWidth][MaxDungeonHeight];
  Item item[MaxDungeonWidth][MaxDungeonHeight];
} Dungeon;

void sortDungeon(Dungeon *dungeon);

int isDungeonValid(Dungeon *dungeon);

int isInDungeon(Dungeon *dungeon, int x, int y);

void getDungeonStart(Dungeon *dungeon, int *_x, int *_y);

void getDungeonEnd(Dungeon *dungeon, int *_x, int *_y);

void setDungeonStart(Dungeon *dungeon, int sx, int sy);

void setDungeonEnd(Dungeon *dungeon, int sx, int sy);

void setDungeonSize(Dungeon *dungeon, int w, int h);

void setDefaultDungeon(Dungeon *dungeon);

void randomizeDungeon(Dungeon *dungeon);

void loadDungeon(Dungeon *dungeon, FILE *file);

void saveDungeon(Dungeon *dungeon, FILE *file);