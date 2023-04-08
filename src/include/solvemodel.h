#pragma once

#include "utils.h"
#include "dungeon.h"

#define SolveStateCount (50000)
#define SolveHeapSize (2000000)
#define DefaultHPPenalty (10000000ll)

typedef struct HeapNode {
  int stat;
  lint dis;
} HeapNode;

HeapNode heapBuf[SolveHeapSize + 1];
HeapNode infNode = {inf, linf};
int heapSize;

void clearHeap() { heapSize = 0; }

int isHeapEmpty() { return heapSize <= 0; }

void pushUpHeap(int x) {
  while (x > 1 && heapBuf[x].dis < heapBuf[x >> 1].dis) {
    HeapNode tmp = heapBuf[x >> 1];
    heapBuf[x >> 1] = heapBuf[x];
    heapBuf[x] = tmp;
    x >>= 1;
  }
}

void pushDownHeap(int x) {
  int b;
  while ((b = x << 1) <= heapSize) {
    if (b + 1 <= heapSize && heapBuf[b + 1].dis < heapBuf[b].dis)
      b++;
    if (heapBuf[b].dis > heapBuf[x].dis)
      break;
    HeapNode tmp = heapBuf[b];
    heapBuf[b] = heapBuf[x];
    heapBuf[x] = tmp;
    x = b;
  }
}

void popHeap() {
  if (isHeapEmpty())
    return;
  heapBuf[1] = heapBuf[heapSize--];
  pushDownHeap(1);
}

void emplaceHeap(HeapNode node) {
  heapBuf[++heapSize] = node;
  pushUpHeap(heapSize);
}

HeapNode topHeap() {
  if (isHeapEmpty())
    return infNode;
  return heapBuf[1];
}

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
  return (x * h + y) * allKey + (hasKey & (allKey - 1));
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

lint solveDis[SolveStateCount];
char solveVisited[SolveStateCount];
int solveFrom[SolveStateCount];

void makeDijkstra(Dungeon *dungeon, int sid, int tid, int allKey,
                  lint hpPenalty) {
  memset(solveDis, 0x3f, sizeof(solveDis));
  memset(solveVisited, 0, sizeof(solveVisited));
  memset(solveFrom, 0x3f, sizeof(solveFrom));

  int height = dungeon->height, sx, sy, skey, tx, ty, tkey;
  a2xy(sid, height, allKey, &sx, &sy, &skey);
  a2xy(tid, height, allKey, &tx, &ty, &tkey);

  solveDis[sid] = 0;
  if (dungeon->event[sx][sy].type == Damage ||
      dungeon->event[sx][sy].type == DamageOT) {
    solveDis[sid] += hpPenalty * dungeon->event[sx][sy].arg;
  }
  if (dungeon->item[sx][sy].type == IKey) {
    skey |= 1 << (dungeon->item[sx][sy].arg - 1);
  }
  sid = xy2a(sx, sy, height, skey, allKey);

  clearHeap();
  HeapNode node;
  node.stat = sid;
  node.dis = solveDis[sid];
  emplaceHeap(node);

  while (!isHeapEmpty()) {
    node = topHeap();
    popHeap();
    if (solveVisited[node.stat])
      continue;
    solveVisited[node.stat] = 1;

    int a = node.stat, x, y, hasKey;
    a2xy(a, height, allKey, &x, &y, &hasKey);

    for (int i = 0; i < 4; i++) {
      int dx = x + go[i][0], dy = y + go[i][1];
      if (dx >= 0 && dx < dungeon->width && dy >= 0 && dy < dungeon->height) {
        if ((dungeon->mp[dx][dy] == Block &&
             dungeon->event[dx][dy].type != Lock) ||
            (dungeon->event[dx][dy].type == Lock &&
             !pick(hasKey, dungeon->event[dx][dy].arg - 1)))
          continue;

        lint dis = solveDis[a] + 1;
        if (dungeon->event[dx][dy].type == Damage ||
            dungeon->event[dx][dy].type == DamageOT) {
          dis += hpPenalty * dungeon->event[dx][dy].arg;
        }

        int dkey = hasKey;
        if (dungeon->item[dx][dy].type == IKey &&
            dungeon->item[dx][dy].arg <= allKey) {
          dkey |= 1 << (dungeon->item[dx][dy].arg - 1);
        }

        int da = xy2a(dx, dy, height, dkey, allKey);

        if (solveDis[da] <= dis)
          continue;

        solveDis[da] = dis;
        solveFrom[da] = a;
        HeapNode tmp;
        tmp.dis = dis;
        tmp.stat = da;
        emplaceHeap(tmp);
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
  makeDijkstra(dungeon, sid, tid, allKey, DefaultHPPenalty);

  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      for (int stat = 0; stat < (1 << allKey); stat++) {
        solution->mp[i][j] = solveDis[xy2a(i, j, h, stat, allKey)] < linf;
        if (solution->mp[i][j])
          break;
      }
    }
  }

  lint minDistan = linf;
  int minLocation = 0;
  for (int stat = 0; stat < (1 << allKey); stat++) {
    int val = xy2a(tx, ty, h, stat, allKey);
    if (solveDis[val] < minDistan) {
      minDistan = solveDis[val];
      minLocation = val;
    }
  }

  solution->route = NULL;
  if (minDistan < linf) {
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

lint getDungeonDistance(Dungeon *dungeon, int sx, int sy, int skey, int tx,
                        int ty, lint hpPenalty) {
  if (!isInDungeon(dungeon, sx, sy) || !isInDungeon(dungeon, tx, ty))
    return linf;

  if (dungeon->mp[sx][sy] == Block)
    return linf;
  if (dungeon->event[sx][sy].type == Lock &&
      !pick(skey, dungeon->event[sx][sy].arg - 1))
    return linf;

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
  if (dungeon->item[sx][sy].type == IKey) {
    skey |= 1 << (dungeon->item[sx][sy].arg - 1);
  }

  int sid = xy2a(sx, sy, dungeon->height, skey, allKey);
  int tid = xy2a(tx, ty, dungeon->height, 0, allKey);
  makeDijkstra(dungeon, sid, tid, allKey, hpPenalty);

  lint minDistan = linf;
  for (int stat = 0; stat < (1 << allKey); stat++) {
    int val = xy2a(tx, ty, dungeon->height, stat, allKey);
    if (solveDis[val] < minDistan) {
      minDistan = solveDis[val];
    }
  }

  return minDistan;
}