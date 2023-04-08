#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "utils.h"
#include "dungeon.h"
#include "pokemon.h"
#include "solvemodel.h"

static bool inBox(double x, double y, double x1, double x2, double y1,
                  double y2) {
  return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
}

void getCellLocation(Dungeon *dungeon, int basex, int basey, double size,
                     double lx, double ly, int *_x, int *_y) {
  *_x = *_y = -1;
  if (lx < Window43Left || lx > Window43Right)
    return;
  for (int x = 0; x < dungeon->width; x++) {
    double xloc = size * (x - basex) + (WindowWidthInch / 2 - size / 2);
    if (xloc + size < Window43Left || xloc > Window43Right)
      continue;
    for (int y = 0; y < dungeon->height; y++) {
      double yloc = size * (y - basey) + (WindowHeightInch / 2 - size / 2);
      if (yloc + size < 0 || yloc > WindowHeightInch)
        continue;

      if (inBox(lx, ly, xloc, xloc + size, yloc, yloc + size)) {
        *_x = x;
        *_y = y;

        return;
      }
    }
  }
}

void drawDungeonPokemon(Dungeon *dungeon, int basex, int basey, double size,
                        Pokemon *pokemon) {
  int x = pokemon->x, y = pokemon->y;
  double xloc = size * (x - basex) + (WindowWidthInch / 2 - size / 2);
  double yloc = size * (y - basey) + (WindowHeightInch / 2 - size / 2);
  if (xloc + size < Window43Left || xloc > Window43Right)
    return;
  if (yloc + size < 0 || yloc > WindowHeightInch)
    return;

  static char *dirTag[5] = {">", "^", "<", "v", "x"};

  if (pokemon->role == Player) {
    SetPenColor("Blue");
    drawBox(xloc + 0.15 * size, yloc + 0.15 * size, 0.7 * size, 0.7 * size, 0,
            dirTag[pokemon->direction], 'C', "Blue");
  } else if (pokemon->role == Enemy) {
    SetPenColor("Red");
    drawBox(xloc + 0.15 * size, yloc + 0.15 * size, 0.7 * size, 0.7 * size, 0,
            dirTag[pokemon->direction], 'C', "Red");
  }
}

void drawDungeon(Dungeon *dungeon, int basex, int basey, double size,
                 int showTag, DungeonSolution *solution, int enableSolution) {
  static char *dirTag[5] = {">", "^", "<", "v", "x"};
  for (int x = 0; x < dungeon->width; x++) {
    double xloc = size * (x - basex) + (WindowWidthInch / 2 - size / 2);
    if (xloc + size < Window43Left || xloc > Window43Right)
      continue;
    for (int y = 0; y < dungeon->height; y++) {
      double yloc = size * (y - basey) + (WindowHeightInch / 2 - size / 2);
      if (yloc + size < 0 || yloc > WindowHeightInch)
        continue;

      int isfill = 0;
      if (dungeon->mp[x][y] == Plain) {
        SetPenColor("Gray");
      } else if (dungeon->mp[x][y] == Block) {
        SetPenColor("Gray");
        isfill = 1;
      } else if (dungeon->mp[x][y] == Start) {
        SetPenColor("Cyan");
        isfill = 1;
      } else if (dungeon->mp[x][y] == End) {
        SetPenColor("Green");
        isfill = 1;
      }

      if (enableSolution) {
        if (dungeon->mp[x][y] == Plain && solution->mp[x][y]) {
          SetPenColor("Yellow");
          isfill = 1;
        }
      }

      drawRectangle(xloc + 0.05 * size, yloc + 0.05 * size, 0.9 * size,
                    0.9 * size, isfill);

      if (size >= 0.8 && showTag) {
        SetPenColor("Black");
        char locationTag[99];
        sprintf(locationTag, "(%d, %d)", x, y);
        drawLabel(xloc + 0.1 * size, yloc + 0.1 * size, locationTag);
      }
    }
  }

  if (enableSolution) {
    if (solution->routeValid) {
      int _pointSize = GetPointSize();
      SetPointSize(28);
      RouteNode *now = solution->route;
      int lasx = -1, lasy = -1, dir;
      while (now) {
        int x = now->x, y = now->y;
        if (lasx + 1 == x && lasy == y) {
          dir = RIGHT;
        } else if (lasx == x && lasy + 1 == y) {
          dir = UP;
        } else if (lasx - 1 == x && lasy == y) {
          dir = LEFT;
        } else if (lasx == x && lasy - 1 == y) {
          dir = DOWN;
        } else {
          dir = 4;
        }

        if (lasx >= 0 && lasy >= 0) {
          double xloc =
              size * (lasx - basex) + (WindowWidthInch / 2 - size / 2);
          double yloc =
              size * (lasy - basey) + (WindowHeightInch / 2 - size / 2);
          if (xloc + size >= Window43Left && xloc <= Window43Right &&
              yloc + size >= 0 && yloc <= WindowHeightInch) {
            SetPenColor("Red");
            drawBox(xloc + 0.05 * size, yloc + 0.05 * size, 0.9 * size,
                    0.9 * size, 0, dirTag[dir], 'C', "Red");
          }
        }

        now = now->nex;
        lasx = x;
        lasy = y;
      }
      SetPointSize(_pointSize);
    }
  }
}

void drawDungeonHighlightCellAt(Dungeon *dungeon, int basex, int basey,
                                double size, int x, int y, double length,
                                int fill, char *color) {
  if (!isInDungeon(dungeon, x, y))
    return;

  double xloc = size * (x - basex) + (WindowWidthInch / 2 - size / 2);
  double yloc = size * (y - basey) + (WindowHeightInch / 2 - size / 2);
  SetPenColor(color);
  drawBox(xloc + (size - length) / 2, yloc + (size - length) / 2, length,
          length, fill, NULL, 'C', "Black");
  return;
}

void drawDungeonHighlightCell(Dungeon *dungeon, int basex, int basey,
                              double size, double lx, double ly, double length,
                              int fill, char *color) {
  int x = -1, y = -1;
  getCellLocation(dungeon, basex, basey, size, lx, ly, &x, &y);
  if (x < 0 || y < 0)
    return;
  drawDungeonHighlightCellAt(dungeon, basex, basey, size, x, y, length, fill,
                             color);
}