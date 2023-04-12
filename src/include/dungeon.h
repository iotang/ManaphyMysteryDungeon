#pragma once

#include <stdio.h>

#include "items.h"
#include "landevents.h"
#include "random.h"

#define MaxDungeonWidth (50)  // 最大的迷宫宽度。
#define MaxDungeonHeight (50) // 最大的迷宫高度。

typedef enum LandType { Plain, Block, Start, End } LandType; // 迷宫格子类型。

typedef struct Dungeon {
  int width, height;
  int mp[MaxDungeonWidth][MaxDungeonHeight];
  LandEvent event[MaxDungeonWidth][MaxDungeonHeight]; // 场地事件。
  Item item[MaxDungeonWidth][MaxDungeonHeight];       // 道具。
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