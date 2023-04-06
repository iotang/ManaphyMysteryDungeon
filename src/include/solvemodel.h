#pragma once

#include "dungeon.h"

typedef struct RouteNode {
  int x, y;
  struct RouteNode *nex;
} RouteNode;

typedef struct DungeonSolution {
  int mp[MaxDungeonWidth][MaxDungeonHeight];
  int routeValid;
  RouteNode *route;
} DungeonSolution;

RouteNode *newRouteNode(int x, int y) {
  RouteNode *v = (RouteNode *)malloc(sizeof(RouteNode));
  v->x = x;
  v->y = y;
  v->nex = NULL;
  return v;
}

int getDungeonSolution(Dungeon *dungeon, DungeonSolution *solution) {
  if (!isDungeonValid(dungeon))
    return -1;

  int w = dungeon->width, h = dungeon->height;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      solution->mp[i][j] = 0;
    }
  }
  solution->routeValid = 0;
  solution->route = NULL;
}