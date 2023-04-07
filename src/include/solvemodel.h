#pragma once

#include "utils.h"
#include "dungeon.h"

#define SolveStateCount (10000)
#define SolveQueueLength (2000000)

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

int xy2a(int x, int y, int h) { return x * h + y; }

void a2xy(int a, int h, int *x, int *y) {
  *x = a / h;
  *y = a % h;
}

void clearDungeonSolution(DungeonSolution *solution) {
  memset(solution->mp, 0, sizeof(solution->mp));
  solution->routeValid = 0;
  solution->route = NULL;
}

int solveDis[SolveStateCount];
char solveVisited[SolveStateCount];
int solveFrom[SolveStateCount];

void makeBfs(Dungeon *dungeon, int sid, int tid) {
  memset(solveDis, 0x3f, sizeof(solveDis));
  memset(solveVisited, 0, sizeof(solveVisited));
  memset(solveFrom, 0x3f, sizeof(solveFrom));

  static int que[SolveQueueLength + 1];
  int he = 0, ta = 0;
  int height = dungeon->height;
  solveDis[sid] = 0, que[++he] = sid;
  solveVisited[sid] = 1;

  while (he != ta) {
    int a = que[++ta];
    if (ta >= SolveQueueLength) {
      ta -= SolveQueueLength;
    }

    int x, y;
    a2xy(a, height, &x, &y);

    for (int i = 0; i < 4; i++) {
      int dx = x + go[i][0], dy = y + go[i][1];
      if (dx >= 0 && dx < dungeon->width && dy >= 0 && dy < dungeon->height) {
        if (dungeon->mp[dx][dy] == Block)
          continue;
        int da = xy2a(dx, dy, height);
        if (solveVisited[da])
          continue;
        solveDis[da] = solveDis[a] + 1;
        solveFrom[da] = a;
        que[++he] = da;
        solveVisited[da] = 1;
        if (he >= SolveQueueLength) {
          he -= SolveQueueLength;
        }
      }
    }
  }
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
  int sx, sy, tx, ty;
  getDungeonStart(dungeon, &sx, &sy);
  getDungeonEnd(dungeon, &tx, &ty);
  int sid = xy2a(sx, sy, h);
  int tid = xy2a(tx, ty, h);
  makeBfs(dungeon, sid, tid);

  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      solution->mp[i][j] = solveDis[xy2a(i, j, h)] < inf;
    }
  }

  solution->route = NULL;
  if (solveDis[tid] < inf) {
    solution->routeValid = 1;
    int x = tx, y = ty;
    do {
      RouteNode *v = newRouteNode(x, y);
      v->nex = solution->route;
      solution->route = v;
      a2xy(solveFrom[xy2a(x, y, h)], h, &x, &y);
    } while (x != sx || y != sy);

  } else {
    solution->routeValid = 0;
  }

  return solution->routeValid;
}

int getDungeonDistance(Dungeon *dungeon, int sx, int sy, int tx, int ty) {
  if (!isInDungeon(dungeon, sx, sy) || !isInDungeon(dungeon, tx, ty))
    return inf;

  if (dungeon->mp[sx][sy] == Block)
    return inf;

  int sid = xy2a(sx, sy, dungeon->height);
  int tid = xy2a(tx, ty, dungeon->height);
  makeBfs(dungeon, sid, tid);
  return solveDis[tid];
}