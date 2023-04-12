#pragma once

#include "utils.h"
#include "dungeon.h"

#define SolveStateCount (40007) // 求解状态的数量限制，至少要有 n^2 2^k。
#define DefaultHPPenalty (10000000ll) // 默认 HP 损失惩罚系数。

typedef struct RouteNode {
  int x, y;
  struct RouteNode *nex;
} RouteNode; // 用于存储迷宫解路线的链表的结点。

typedef struct DungeonSolution {
  int mp[MaxDungeonWidth][MaxDungeonHeight]; // 是否到达过 / 已经到达。
  int routeValid;                            // 迷宫解路线是否可用。
  RouteNode *route; // 迷宫解路线的链表的头结点，从起点到终点。
} DungeonSolution; // 用于存储迷宫解的数据结构。

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