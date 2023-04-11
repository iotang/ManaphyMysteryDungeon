#include "drawdungeon.h"

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "utils.h"
#include "pokemon.h"
#include "imagesupport.h"

HBITMAP spritePlain, spriteBlock, spriteStart, spriteEnd;

void initDungeonSprites() {
  spritePlain = readBmpImage("assets/dungeon/Plain.bmp");
  spriteBlock = readBmpImage("assets/dungeon/Block.bmp");
  spriteStart = readBmpImage("assets/dungeon/Start.bmp");
  spriteEnd = readBmpImage("assets/dungeon/End.bmp");
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

void drawDungeonPokemon(Dungeon *dungeon, int basex, int basey, double size,
                        Pokemon *pokemon) {
  int x = pokemon->x, y = pokemon->y;
  if (!isInDungeon(dungeon, x, y))
    return;
  double xloc = size * (x - basex) + (WindowWidthInch / 2 - size / 2);
  double yloc = size * (y - basey) + (WindowHeightInch / 2 - size / 2);
  if (xloc + size < Window43Left || xloc > Window43Right)
    return;
  if (yloc + size < 0 || yloc > WindowHeightInch)
    return;

  if (pokemon->role == Player) {
    SetPenColor("Blue");
  } else if (pokemon->role == Enemy) {
    SetPenColor("Red");
  }
  drawBoldRectangle(xloc + 0.1 * size, yloc + 0.1 * size, 0.8 * size,
                    0.8 * size, 0.03 * size);
  drawBmp(pokedex[pokemon->species].sprites[pokemon->direction],
          xloc + 0.5 * size, yloc + 0.5 * size, 0.9 * size, 0.9 * size, SRCAND);
}

void drawDungeon(Dungeon *dungeon, int basex, int basey, double size,
                 int showTag, int showName, DungeonSolution *solution,
                 int enableSolution) {
  int _pointSize = GetPointSize();
  SetPointSize(4);
  for (int x = 0; x < dungeon->width; x++) {
    double xloc = size * (x - basex) + (WindowWidthInch / 2 - size / 2);
    if (xloc + size < Window43Left || xloc > Window43Right)
      continue;
    for (int y = 0; y < dungeon->height; y++) {
      double yloc = size * (y - basey) + (WindowHeightInch / 2 - size / 2);
      if (yloc + size < 0 || yloc > WindowHeightInch)
        continue;

      if (dungeon->mp[x][y] == Plain) {
        drawBmp(spritePlain, xloc + 0.5 * size, yloc + 0.5 * size, size, size,
                SRCCOPY);
      } else if (dungeon->mp[x][y] == Block) {
        drawBmp(spriteBlock, xloc + 0.5 * size, yloc + 0.5 * size, size, size,
                SRCCOPY);
      } else if (dungeon->mp[x][y] == Start) {
        drawBmp(spriteStart, xloc + 0.5 * size, yloc + 0.5 * size, size, size,
                SRCCOPY);
        SetPenColor("Cyan");
        drawBoldRectangle(xloc, yloc, size, size, 0.1 * size);
      } else if (dungeon->mp[x][y] == End) {
        drawBmp(spriteEnd, xloc + 0.5 * size, yloc + 0.5 * size, size, size,
                SRCCOPY);
        SetPenColor("Green");
        drawBoldRectangle(xloc, yloc, size, size, 0.1 * size);
      }

      if (enableSolution) {
        if (dungeon->mp[x][y] == Plain && solution->mp[x][y]) {
          SetPenColor("Yellow");
          drawRectangle(xloc + 0.05 * size, yloc + 0.05 * size, 0.9 * size,
                        0.9 * size, 1);
        }
      }

      if (size >= 0.8 && showTag) {
        SetPenColor("Black");
        char locationTag[99];
        sprintf(locationTag, "(%d, %d)", x, y);
        drawLabel(xloc + 0.1 * size, yloc + 0.1 * size, locationTag);
      }
    }
  }
  SetPointSize(_pointSize);

  for (int x = 0; x < dungeon->width; x++) {
    double xloc = size * (x - basex) + (WindowWidthInch / 2 - size / 2);
    if (xloc + size < Window43Left || xloc > Window43Right)
      continue;
    for (int y = 0; y < dungeon->height; y++) {
      double yloc = size * (y - basey) + (WindowHeightInch / 2 - size / 2);
      if (yloc + size < 0 || yloc > WindowHeightInch)
        continue;

      _pointSize = GetPointSize();
      if (size >= 0.8)
        SetPointSize(8);
      else if (size >= 0.6)
        SetPointSize(4);
      else
        SetPointSize(2);
      char _tag[133];
      double h = GetFontHeight() * 0.7;
      if (dungeon->item[x][y].type != INone) {
        if (dungeon->item[x][y].type == IKey ||
            dungeon->item[x][y].type == ITM) {
          sprintf(_tag, "%s %d", itemsData[dungeon->item[x][y].type].name,
                  dungeon->item[x][y].arg);
        } else {
          sprintf(_tag, "%s", itemsData[dungeon->item[x][y].type].name);
        }
        drawBmp(itemsData[dungeon->item[x][y].type].sprite, xloc + 0.5 * size,
                yloc + 0.3 * size, 0.4 * size, 0.4 * size, SRCAND);

        if (showName) {
          if (size > 0.2) {
            drawLabelWithOutline(xloc + 0.1 * size, yloc + 0.9 * size - h, _tag,
                                 "Brown", "White");
          } else {
            SetPenColor("Brown");
            drawRectangle(xloc + 0.15 * size, yloc + 0.75 * size, 0.1 * size,
                          0.1 * size, 1);
          }
        }
      }
      if (dungeon->event[x][y].type != None) {
        if (dungeon->event[x][y].type == Lock) {
          sprintf(_tag, "%s %d", landEventsData[dungeon->event[x][y].type].name,
                  dungeon->event[x][y].arg);
        } else {
          sprintf(_tag, "%s", landEventsData[dungeon->event[x][y].type].name);
        }
        drawBmp(landEventsData[dungeon->event[x][y].type].sprite,
                xloc + 0.5 * size, yloc + 0.5 * size, size, size, SRCAND);

        if (showName || dungeon->event[x][y].type == Lock) {
          if (size > 0.2) {
            drawLabelWithOutline(xloc + 0.1 * size, yloc + 0.9 * size - h - h,
                                 _tag, "Orange", "Black");
          } else {
            SetPenColor("Orange");
            drawRectangle(xloc + 0.35 * size, yloc + 0.75 * size, 0.1 * size,
                          0.1 * size, 1);
          }
        }
      }
      SetPointSize(_pointSize);
    }
  }

  if (enableSolution) {
    if (solution->routeValid) {
      int _pointSize = GetPointSize();
      SetPointSize(8);
      double h = GetFontHeight() * 1;
      RouteNode *now = solution->route;
      // int lasx = -1, lasy = -1, dir;
      int count = 0;
      static int visitCount[MaxDungeonWidth][MaxDungeonHeight];

      for (int x = 0; x < dungeon->width; x++) {
        for (int y = 0; y < dungeon->height; y++) {
          visitCount[x][y] = 0;
        }
      }

      while (now) {
        int x = now->x, y = now->y;
        static char _tag[12];
        sprintf(_tag, "%d", ++count);
        double xloc = size * (x - basex) + (WindowWidthInch / 2 - size / 2);
        double yloc = size * (y - basey) + (WindowHeightInch / 2 - size / 2);
        if (xloc + size >= Window43Left && xloc <= Window43Right &&
            yloc + size >= 0 && yloc <= WindowHeightInch) {
          drawBoxWithoutBorder(xloc + 0.15 * size,
                               yloc + 0.85 * size - h * (visitCount[x][y] + 1),
                               0.7 * size, h, 0, _tag, 'R', "Red");
          visitCount[x][y]++;
        }
        now = now->nex;
      }
      SetPointSize(_pointSize);
    }
  }
}

void drawDungeonHighlightCellAt(Dungeon *dungeon, int basex, int basey,
                                double size, int x, int y, double length,
                                char *color, double dx, double dy) {
  if (!isInDungeon(dungeon, x, y))
    return;

  double xloc = size * (x - basex) + (WindowWidthInch / 2 - size / 2);
  double yloc = size * (y - basey) + (WindowHeightInch / 2 - size / 2);
  SetPenColor(color);
  drawBoldRectangle(xloc + (size - length) / 2 + dx * size,
                    yloc + (size - length) / 2 + dy * size, length, length,
                    0.03 * size);
  return;
}

void drawDungeonHighlightCell(Dungeon *dungeon, int basex, int basey,
                              double size, double lx, double ly, double length,
                              char *color, double dx, double dy) {
  int x = -1, y = -1;
  getCellLocation(dungeon, basex, basey, size, lx, ly, &x, &y);
  if (x < 0 || y < 0)
    return;
  drawDungeonHighlightCellAt(dungeon, basex, basey, size, x, y, length, color,
                             dx, dy);
}

int drawDungeonEventEditOverride(LandEvent *landEvent, Item *item, double basex,
                                 double basey, char *bgcolor, int isEdit,
                                 int belong, int controlOverride,
                                 int *overrideItem, int *overrideLandEvent) {
  int modified = 0;

  int _pointSize = GetPointSize();
  SetPenColor(bgcolor);
  drawRectangle(basex + Window43Gap * 0.03, basey + WindowHeightInch * 0.005,
                Window43Gap * 0.94, WindowHeightInch * 0.49, 1);

  SetPointSize(24);
  SetPenColor("White");
  drawBox(basex + Window43Gap * 0.05, basey + WindowHeightInch * 0.45,
          Window43Gap * 0.9, WindowHeightInch * 0.04, 1, "Item", 'C', "Black");

  SetPenColor("White");
  drawRectangle(basex + Window43Gap * 0.05, basey + WindowHeightInch * 0.255,
                Window43Gap * 0.9, WindowHeightInch * 0.195, 1);

  SetPointSize(16);
  drawBoxWithoutBorder(basex + Window43Gap * 0.06,
                       basey + WindowHeightInch * 0.415, Window43Gap * 0.88,
                       WindowHeightInch * 0.03, 0, itemsData[item->type].name,
                       'C', "Black");

  char _arg[99];
  if (item->type == IKey || item->type == ITM) {
    if (item->type == IKey) {
      sprintf(_arg, "ID: %d", item->arg);
      SetPointSize(16);
    } else if (item->type == ITM) {
      sprintf(_arg, "No. %d \"%s\"", item->arg, movedex[item->arg].name);
      SetPointSize(12);
    }
    drawBoxWithoutBorder(basex + Window43Gap * 0.06,
                         basey + WindowHeightInch * 0.345, Window43Gap * 0.88,
                         WindowHeightInch * 0.03, 0, _arg, 'C', "Black");
  }

  if (isEdit) {
    setButtonColors("White", "Blue", "Blue", "White", 1);
    SetPointSize(8);
    if (button(GenUIID(0), basex + Window43Gap * 0.06,
               basey + WindowHeightInch * 0.38, Window43Gap * 0.43,
               WindowHeightInch * 0.03, "Last Item", belong)) {
      if (item->type > 0) {
        item->type--;
      } else {
        item->type = MaxItemNumber;
      }
      item->arg = itemsData[item->type].defaultArg;
      modified = 1;
    }
    if (button(GenUIID(0), basex + Window43Gap * 0.51,
               basey + WindowHeightInch * 0.38, Window43Gap * 0.43,
               WindowHeightInch * 0.03, "Next Item", belong)) {
      if (item->type < MaxItemNumber) {
        item->type++;
      } else {
        item->type = 0;
      }
      item->arg = itemsData[item->type].defaultArg;
      modified = 1;
    }

    if (item->type == IKey || item->type == ITM) {
      if (button(GenUIID(0), basex + Window43Gap * 0.06,
                 basey + WindowHeightInch * 0.31, Window43Gap * 0.25,
                 WindowHeightInch * 0.03, "-10", belong)) {
        if (item->arg > 1) {
          item->arg -= 10;
          if (item->arg < 1)
            item->arg = 1;
          modified = 1;
        }
      }
      if (button(GenUIID(0), basex + Window43Gap * 0.33,
                 basey + WindowHeightInch * 0.31, Window43Gap * 0.17,
                 WindowHeightInch * 0.03, "-", belong)) {
        if (item->arg > 1) {
          item->arg--;
          modified = 1;
        }
      }
      if (button(GenUIID(0), basex + Window43Gap * 0.51,
                 basey + WindowHeightInch * 0.31, Window43Gap * 0.17,
                 WindowHeightInch * 0.03, "+", belong)) {
        if (item->type == IKey && item->arg < MaxKeyID) {
          item->arg++;
          modified = 1;
        } else if (item->type == ITM && item->arg < MaxMoveCount) {
          item->arg++;
          modified = 1;
        }
      }
      if (button(GenUIID(0), basex + Window43Gap * 0.69,
                 basey + WindowHeightInch * 0.31, Window43Gap * 0.25,
                 WindowHeightInch * 0.03, "+10", belong)) {
        if (item->type == IKey && item->arg < MaxKeyID) {
          item->arg += 10;
          if (item->arg > MaxKeyID)
            item->arg = MaxKeyID;
          modified = 1;
        } else if (item->type == ITM && item->arg < MaxMoveCount) {
          item->arg += 10;
          if (item->arg > MaxMoveCount)
            item->arg = MaxMoveCount;
          modified = 1;
        }
      }
    }
  }

  if (controlOverride) {
    if (*overrideItem) {
      setButtonColors("Blue", "White", "White", "Blue", 1);
    } else {
      setButtonColors("White", "Blue", "Blue", "White", 1);
    }
    if (button(GenUIID(0), basex + Window43Gap * 0.06,
               basey + WindowHeightInch * 0.27, Window43Gap * 0.88,
               WindowHeightInch * 0.03,
               *overrideItem ? "Override" : "Don't Override", belong)) {
      *overrideItem ^= 1;
    }
  }

  SetPenColor("Black");
  drawRectangle(basex + Window43Gap * 0.05, basey + WindowHeightInch * 0.255,
                Window43Gap * 0.9, WindowHeightInch * 0.235, 0);

  SetPointSize(24);
  SetPenColor("White");
  drawBox(basex + Window43Gap * 0.05, basey + WindowHeightInch * 0.205,
          Window43Gap * 0.9, WindowHeightInch * 0.04, 1, "Land Event", 'C',
          "Black");

  SetPenColor("White");
  drawRectangle(basex + Window43Gap * 0.05, basey + WindowHeightInch * 0.01,
                Window43Gap * 0.9, WindowHeightInch * 0.195, 1);

  SetPointSize(16);
  drawBoxWithoutBorder(basex + Window43Gap * 0.06,
                       basey + WindowHeightInch * 0.17, Window43Gap * 0.88,
                       WindowHeightInch * 0.03, 0,
                       landEventsData[landEvent->type].name, 'C', "Black");

  if (landEvent->type != None) {
    if (landEvent->type == Lock) {
      sprintf(_arg, "ID: %d", landEvent->arg);
      SetPointSize(16);
    } else if (landEvent->type == Damage || landEvent->type == DamageOT) {
      sprintf(_arg, "%d Damage", landEvent->arg);
      SetPointSize(16);
    } else if (landEvent->type == HealOT) {
      sprintf(_arg, "Heal %d HP", landEvent->arg);
      SetPointSize(16);
    }
    drawBoxWithoutBorder(basex + Window43Gap * 0.06,
                         basey + WindowHeightInch * 0.1, Window43Gap * 0.88,
                         WindowHeightInch * 0.03, 0, _arg, 'C', "Black");
  }

  if (isEdit) {
    setButtonColors("White", "Blue", "Blue", "White", 1);
    SetPointSize(8);
    if (button(GenUIID(0), basex + Window43Gap * 0.06,
               basey + WindowHeightInch * 0.135, Window43Gap * 0.43,
               WindowHeightInch * 0.03, "Last Event", belong)) {
      if (landEvent->type > 0) {
        landEvent->type--;
      } else {
        landEvent->type = MaxLandEventTypeNumber;
      }
      landEvent->arg = landEventsData[landEvent->type].defaultArg;
      modified = 1;
    }
    if (button(GenUIID(0), basex + Window43Gap * 0.51,
               basey + WindowHeightInch * 0.135, Window43Gap * 0.43,
               WindowHeightInch * 0.03, "Next Event", belong)) {
      if (landEvent->type < MaxLandEventTypeNumber) {
        landEvent->type++;
      } else {
        landEvent->type = 0;
      }
      landEvent->arg = landEventsData[landEvent->type].defaultArg;
      modified = 1;
    }
    if (landEvent->type != None) {
      if (button(GenUIID(0), basex + Window43Gap * 0.06,
                 basey + WindowHeightInch * 0.065, Window43Gap * 0.25,
                 WindowHeightInch * 0.03, "-10", belong)) {
        if (landEvent->arg > 1) {
          landEvent->arg -= 10;
          if (landEvent->arg < 1)
            landEvent->arg = 1;
          modified = 1;
        }
      }
      if (button(GenUIID(0), basex + Window43Gap * 0.33,
                 basey + WindowHeightInch * 0.065, Window43Gap * 0.17,
                 WindowHeightInch * 0.03, "-", belong)) {
        if (landEvent->arg > 1) {
          landEvent->arg--;
          modified = 1;
        }
      }
      if (button(GenUIID(0), basex + Window43Gap * 0.51,
                 basey + WindowHeightInch * 0.065, Window43Gap * 0.17,
                 WindowHeightInch * 0.03, "+", belong)) {
        if (landEvent->type != Lock || landEvent->arg < MaxKeyID) {
          landEvent->arg++;
          modified = 1;
        }
      }
      if (button(GenUIID(0), basex + Window43Gap * 0.69,
                 basey + WindowHeightInch * 0.065, Window43Gap * 0.25,
                 WindowHeightInch * 0.03, "+10", belong)) {
        if (landEvent->type != Lock || landEvent->arg < MaxKeyID) {
          landEvent->arg += 10;
          if (landEvent->type == Lock && landEvent->arg > MaxKeyID) {
            landEvent->arg = MaxKeyID;
          }
          modified = 1;
        }
      }
    }
  }

  if (controlOverride) {
    if (*overrideLandEvent) {
      setButtonColors("Blue", "White", "White", "Blue", 1);
    } else {
      setButtonColors("White", "Blue", "Blue", "White", 1);
    }
    if (button(GenUIID(0), basex + Window43Gap * 0.06,
               basey + WindowHeightInch * 0.025, Window43Gap * 0.88,
               WindowHeightInch * 0.03,
               *overrideLandEvent ? "Override" : "Don't Override", belong)) {
      *overrideLandEvent ^= 1;
    }
  }

  SetPenColor("Black");
  drawRectangle(basex + Window43Gap * 0.05, basey + WindowHeightInch * 0.01,
                Window43Gap * 0.9, WindowHeightInch * 0.235, 0);

  SetPointSize(_pointSize);

  return modified;
}

int drawDungeonEventEdit(LandEvent *landEvent, Item *item, double basex,
                         double basey, char *bgcolor, int isEdit, int belong) {
  return drawDungeonEventEditOverride(landEvent, item, basex, basey, bgcolor,
                                      isEdit, belong, 0, NULL, NULL);
}

bool notInAllMenu(double x, double y) {
  if (y >= WindowHeightInch - MenuHeight)
    return 0;
  return notInMenu(x, y);
}