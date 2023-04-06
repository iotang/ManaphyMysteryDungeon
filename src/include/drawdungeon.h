#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "utils.h"
#include "dungeon.h"

void drawDungeon(Dungeon *dungeon, int basex, int basey, double size,
                 int showTag) {
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
      }
      if (dungeon->mp[x][y] == Block) {
        SetPenColor("Gray");
        isfill = 1;
      }
      if (dungeon->mp[x][y] == Start) {
        SetPenColor("Cyan");
        isfill = 1;
      }
      if (dungeon->mp[x][y] == End) {
        SetPenColor("Green");
        isfill = 1;
      }

      drawRectangle(xloc + 0.05 * size, yloc + 0.05 * size, 0.9 * size,
                    0.9 * size, isfill);

      if (size >= 0.8 && showTag) {
        SetPenColor("Black");
        char locationTag[99];
        sprintf(locationTag, "(%d, %d)", x, y);
        drawLabel(xloc + 0.1 * size, yloc + 0.1 * size, locationTag);
      }

      if (x == basex && y == basey) {
        SetPenColor("Black");
        drawBox(WindowWidthInch / 2 - size / 4, WindowHeightInch / 2 - size / 4,
                size / 2, size / 2, 0, "You", 'C', "Blue");
      }
    }
  }
}

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

void drawDungeonCell(Dungeon *dungeon, int basex, int basey, double size,
                     double lx, double ly) {
  int x = -1, y = -1;
  getCellLocation(dungeon, basex, basey, size, lx, ly, &x, &y);
  if (x < 0 || y < 0)
    return;
  double xloc = size * (x - basex) + (WindowWidthInch / 2 - size / 2);
  double yloc = size * (y - basey) + (WindowHeightInch / 2 - size / 2);
  SetPenColor("Magenta");
  drawRectangle(xloc, yloc, size, size, 0);
  return;
}