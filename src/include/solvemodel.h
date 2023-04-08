#pragma once

#include "utils.h"
#include "dungeon.h"

#define SolveStateCount (50000)
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

int xy2a(int x, int y, int h, int hasKey, int allKey) {
  allKey = (1 << allKey);
  return (x * h + y) * allKey + hasKey;
}

void a2xy(int a, int h, int allKey, int *x, int *y, int *hasKey) {
  allKey = (1 << allKey);
  *x = (a / allKey) / h;
  *y = (a / allKey) % h;
  *hasKey = a % allKey;
}

void clearDungeonSolution(DungeonSolution *solution) {
  memset(solution->mp, 0, sizeof(solution->mp));
  solution->routeValid = 0;
  solution->route = NULL;
}

int solveDis[SolveStateCount];
char solveVisited[SolveStateCount];
int solveFrom[SolveStateCount];

#define pick(a, b) (((a) >> (b)) & 1)

void makeBfs(Dungeon *dungeon, int sid, int tid, int allKey) {
  memset(solveDis, 0x3f, sizeof(solveDis));
  memset(solveVisited, 0, sizeof(solveVisited));
  memset(solveFrom, 0x3f, sizeof(solveFrom));

  static int que[SolveQueueLength + 1];
  int he = 0, ta = 0, tx, ty, tkey;
  int height = dungeon->height;
  solveDis[sid] = 0, que[++he] = sid;
  solveVisited[sid] = 1;
  a2xy(tid, height, allKey, &tx, &ty, &tkey);

  while (he != ta) {
    int a = que[++ta];
    if (ta >= SolveQueueLength) {
      ta -= SolveQueueLength;
    }

    int x, y, hasKey;
    a2xy(a, height, allKey, &x, &y, &hasKey);

    for (int i = 0; i < 4; i++) {
      int dx = x + go[i][0], dy = y + go[i][1];
      if (dx >= 0 && dx < dungeon->width && dy >= 0 && dy < dungeon->height) {
        if ((dungeon->mp[dx][dy] == Block &&
             dungeon->event[dx][dy].type != Lock) ||
            (dungeon->event[dx][dy].type == Lock &&
             !pick(hasKey, dungeon->event[dx][dy].arg - 1)))
          continue;
        int dkey = hasKey;
        if (dungeon->item[dx][dy].type == IKey &&
            dungeon->item[dx][dy].arg <= allKey) {
          dkey |= 1 << (dungeon->item[dx][dy].arg - 1);
        }
        int da = xy2a(dx, dy, height, dkey, allKey);
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
  int sx, sy, tx, ty, allKey = 0;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      if (dungeon->event[i][j].type == Lock) {
        if (dungeon->event[i][j].arg > allKey) {
          allKey = dungeon->event[i][j].arg;
        }
      }
    }
  }

  getDungeonStart(dungeon, &sx, &sy);
  getDungeonEnd(dungeon, &tx, &ty);
  int sid = xy2a(sx, sy, h, 0, allKey);
  int tid = xy2a(tx, ty, h, 0, allKey);
  makeBfs(dungeon, sid, tid, allKey);

  printf("%d %d\n", sid, allKey);

  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      for (int stat = 0; stat < (1 << allKey); stat++) {
        solution->mp[i][j] = solveDis[xy2a(i, j, h, stat, allKey)] < inf;
        if (solution->mp[i][j])
          break;
      }
    }
  }

  int minDistan = inf;
  int minLocation = 0;
  for (int stat = 0; stat < (1 << allKey); stat++) {
    int val = xy2a(tx, ty, h, stat, allKey);
    if (solveDis[val] < minDistan) {
      minDistan = solveDis[val];
      minLocation = val;
    }
  }

  solution->route = NULL;
  if (minDistan < inf) {
    solution->routeValid = 1;
    int x, y, key;
    a2xy(minLocation, h, allKey, &x, &y, &key);
    do {
      RouteNode *v = newRouteNode(x, y);
      v->nex = solution->route;
      solution->route = v;
      a2xy(solveFrom[xy2a(x, y, h, key, allKey)], h, allKey, &x, &y, &key);
    } while (x != sx || y != sy || key != 0);

  } else {
    solution->routeValid = 0;
  }

  return solution->routeValid;
}

int getDungeonDistance(Dungeon *dungeon, int sx, int sy, int skey, int tx,
                       int ty) {
  if (!isInDungeon(dungeon, sx, sy) || !isInDungeon(dungeon, tx, ty))
    return inf;

  if (dungeon->mp[sx][sy] == Block)
    return inf;

  int allKey = 0;
  for (int i = 0; i < dungeon->width; i++) {
    for (int j = 0; j < dungeon->height; j++) {
      if (dungeon->event[i][j].type == Lock) {
        if (dungeon->event[i][j].arg > allKey) {
          allKey = dungeon->event[i][j].arg;
        }
      }
    }
  }
  int sid = xy2a(sx, sy, dungeon->height, skey, allKey);
  int tid = xy2a(tx, ty, dungeon->height, 0, allKey);
  makeBfs(dungeon, sid, tid, allKey);

  int minDistan = inf;
  for (int stat = 0; stat < (1 << allKey); stat++) {
    int val = xy2a(tx, ty, dungeon->height, stat, allKey);
    if (solveDis[val] < minDistan) {
      minDistan = solveDis[val];
    }
  }

  return minDistan;
}

#undef pick