#pragma once

#include "utils.h"
#include "dungeon.h"

#define SolveStateCount (50000)
#define DefaultHPPenalty (10000000ll)

typedef struct RouteNode {
  int x, y;
  struct RouteNode *nex;
} RouteNode;

typedef struct DungeonSolution {
  int mp[MaxDungeonWidth][MaxDungeonHeight];
  int routeValid;
  RouteNode *route;
} DungeonSolution;

RouteNode *newRouteNode(int x, int y);

int xy2a(int x, int y, int h, int hasKey, int allKey);

void a2xy(int a, int h, int allKey, int *x, int *y, int *hasKey);

void clearDungeonSolution(DungeonSolution *solution);

void makeDijkstra(Dungeon *dungeon, int sid, int tid, int allKey,
                  lint hpPenalty, lint limit);

int getDungeonSolutionWithLimit(Dungeon *dungeon, DungeonSolution *solution,
                                lint limit, lint hpPenalty);

int getDungeonSolution(Dungeon *dungeon, DungeonSolution *solution);

lint getDungeonDistance(Dungeon *dungeon, int sx, int sy, int skey, int tx,
                        int ty, lint hpPenalty, int enableKey);