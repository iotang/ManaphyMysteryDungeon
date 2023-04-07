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

  static int dis[SolveStateCount], ed[SolveStateCount], fr[SolveStateCount];
  static int go[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  memset(dis, 0x3f, sizeof(dis));
  memset(ed, 0, sizeof(ed));
  memset(fr, 0x3f, sizeof(fr));

  static int que[SolveQueueLength + 1], he, ta;
  he = 0, ta = 0;
  int height = dungeon->height, sx, sy, sid, tx, ty, tid;
  getDungeonStart(dungeon, &sx, &sy);
  getDungeonEnd(dungeon, &tx, &ty);
  sid = xy2a(sx, sy, height);
  tid = xy2a(tx, ty, height);
  dis[sid] = 0, que[++he] = sid;

  while (he != ta) {
    int a = que[++ta];
    if (ta >= SolveQueueLength) {
      ta -= SolveQueueLength;
    }

    int x, y;
    a2xy(a, height, &x, &y);
    // printf("%d %d %d\n", x, y, a);

    for (int i = 0; i < 4; i++) {
      int dx = x + go[i][0], dy = y + go[i][1];
      if (dx >= 0 && dx < dungeon->width && dy >= 0 && dy < dungeon->height) {
        if (dungeon->mp[dx][dy] == Block)
          continue;
        int da = xy2a(dx, dy, height);
        if (ed[da])
          continue;
        // printf("%d %d %d -> %d %d %d\n", x, y, a, dx, dy, da);
        dis[da] = dis[a] + 1;
        fr[da] = a;
        que[++he] = da;
        ed[da] = 1;
        if (he >= SolveQueueLength) {
          he -= SolveQueueLength;
        }
      }
    }
    // printf("%d : %d\n", he, ta);
  }

  for (int i = 0; i < dungeon->width; i++) {
    for (int j = 0; j < dungeon->height; j++) {
      solution->mp[i][j] = dis[xy2a(i, j, height)] < inf;
    }
  }

  solution->route = NULL;
  if (dis[tid] < inf) {
    solution->routeValid = 1;
    int x = tx, y = ty;
    do {
      RouteNode *v = newRouteNode(x, y);
      v->nex = solution->route;
      solution->route = v;
      a2xy(fr[xy2a(x, y, height)], height, &x, &y);
    } while (x != sx || y != sy);

  } else {
    solution->routeValid = 0;
  }

  return solution->routeValid;
}