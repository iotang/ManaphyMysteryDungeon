#include "dungeon.h"

#include "pokemon.h"
#include "items.h"
#include "landevents.h"

#include <string.h>

void sortDungeon(Dungeon *dungeon) {
  int sx = -1, sy = -1, tx = -1, ty = -1;
  if (dungeon->width < 2)
    dungeon->width = 2;
  if (dungeon->height < 2)
    dungeon->height = 2;
  if (dungeon->width > MaxDungeonWidth)
    dungeon->width = MaxDungeonWidth;
  if (dungeon->height > MaxDungeonHeight)
    dungeon->height = MaxDungeonHeight;

  for (int x = 0; x < dungeon->width; x++) {
    for (int y = 0; y < dungeon->height; y++) {
      if (dungeon->mp[x][y] == Start || dungeon->mp[x][y] == End) {
        if (dungeon->mp[x][y] == Start) {
          if (sx < 0 || sy < 0) {
            sx = x;
            sy = y;
          } else {
            dungeon->mp[x][y] = Plain;
          }
        }
        if (dungeon->mp[x][y] == End) {
          if (tx < 0 || ty < 0) {
            tx = x;
            ty = y;
          } else {
            dungeon->mp[x][y] = Plain;
          }
        }
        dungeon->event[x][y].type = dungeon->event[x][y].arg = 0;
        dungeon->item[x][y].type = dungeon->item[x][y].arg = 0;
      } else {
        if (dungeon->event[x][y].type < 0 || dungeon->event[x][y].type > 4) {
          dungeon->event[x][y].type = dungeon->event[x][y].arg = 0;
        }

        if (dungeon->item[x][y].type < 0 ||
            dungeon->item[x][y].type > MaxItemNumber) {
          dungeon->item[x][y].type = dungeon->item[x][y].arg = 0;
        } else if (dungeon->item[x][y].type == IKey &&
                   (dungeon->item[x][y].arg < 1 ||
                    dungeon->item[x][y].arg > MaxKeyID)) {
          dungeon->item[x][y].type = dungeon->item[x][y].arg = 0;
        } else if (dungeon->item[x][y].type == ITM &&
                   (dungeon->item[x][y].arg < 1 ||
                    dungeon->item[x][y].arg > MaxMoveCount)) {
          dungeon->item[x][y].type = dungeon->item[x][y].arg = 0;
        }
      }
    }
  }
  if (sx < 0 || sy < 0 || tx < 0 || ty < 0) {
    if (sx >= 0 && sy >= 0)
      dungeon->mp[sx][sy] = Plain;
    if (tx >= 0 && ty >= 0)
      dungeon->mp[sx][sy] = Plain;
    dungeon->mp[0][0] = Start;
    dungeon->mp[dungeon->width - 1][dungeon->height - 1] = End;
  }
}

int isDungeonValid(Dungeon *dungeon) {
  if (dungeon->width < 2 || dungeon->width > MaxDungeonWidth)
    return 0;

  if (dungeon->height < 2 || dungeon->height > MaxDungeonHeight)
    return 0;

  int gotStart = 0, gotEnd = 0;
  for (int x = 0; x < dungeon->width; x++) {
    for (int y = 0; y < dungeon->height; y++) {
      if (dungeon->mp[x][y] == Start)
        gotStart++;
      if (dungeon->mp[x][y] == End)
        gotEnd++;
    }
  }
  return gotStart == 1 && gotEnd == 1;
}

int isInDungeon(Dungeon *dungeon, int x, int y) {
  return (x >= 0 && x < dungeon->width && y >= 0 && y < dungeon->height);
}

void getDungeonStart(Dungeon *dungeon, int *_x, int *_y) {
  *_x = *_y = -1;
  for (int x = 0; x < dungeon->width; x++) {
    for (int y = 0; y < dungeon->height; y++) {
      if (dungeon->mp[x][y] == Start) {
        *_x = x;
        *_y = y;
        return;
      }
    }
  }
}

void getDungeonEnd(Dungeon *dungeon, int *_x, int *_y) {
  *_x = *_y = -1;
  for (int x = 0; x < dungeon->width; x++) {
    for (int y = 0; y < dungeon->height; y++) {
      if (dungeon->mp[x][y] == End) {
        *_x = x;
        *_y = y;
        return;
      }
    }
  }
}

void setDungeonStart(Dungeon *dungeon, int sx, int sy) {
  if (!isInDungeon(dungeon, sx, sy))
    return;
  int x, y;
  getDungeonStart(dungeon, &x, &y);
  if (x >= 0 && y >= 0)
    dungeon->mp[x][y] = Plain;
  dungeon->mp[sx][sy] = Start;
}

void setDungeonEnd(Dungeon *dungeon, int sx, int sy) {
  if (!isInDungeon(dungeon, sx, sy))
    return;
  int x, y;
  getDungeonEnd(dungeon, &x, &y);
  if (x >= 0 && y >= 0)
    dungeon->mp[x][y] = Plain;
  dungeon->mp[sx][sy] = End;
}

void setDungeonSize(Dungeon *dungeon, int w, int h) {
  if (w < 2)
    w = 2;
  if (w > MaxDungeonWidth)
    w = MaxDungeonWidth;
  if (h < 2)
    h = 2;
  if (h > MaxDungeonHeight)
    h = MaxDungeonHeight;

  int sx, sy, tx, ty;
  getDungeonStart(dungeon, &sx, &sy);
  getDungeonEnd(dungeon, &tx, &ty);

  if (sx >= w || sy >= h) {
    setDungeonStart(dungeon, w - 1, h - 1);
  }
  if (tx >= w || ty >= h) {
    setDungeonEnd(dungeon, w - 1, h - 1 - (dungeon->mp[w - 1][h - 1] == Start));
  }
  dungeon->width = w;
  dungeon->height = h;
}

void setDefaultDungeon(Dungeon *dungeon) {
  dungeon->width = MaxDungeonWidth;
  dungeon->height = MaxDungeonHeight;

  for (int i = 0; i < MaxDungeonWidth; i++) {
    for (int j = 0; j < MaxDungeonHeight; j++) {
      dungeon->mp[i][j] = Plain;
      dungeon->event[i][j].type = 0;
      dungeon->item[i][j].type = 0;
    }
  }

  dungeon->width = dungeon->height = 10;
  setDungeonStart(dungeon, 0, 0);
  setDungeonEnd(dungeon, 9, 9);
}

void randomizeDungeon(Dungeon *dungeon) {
  for (int i = 0; i < dungeon->width; i++) {
    for (int j = 0; j < dungeon->height; j++) {
      dungeon->mp[i][j] = RandomChance(0.35) ? Block : Plain;
      dungeon->event[i][j].type = 0;
      dungeon->item[i][j].type = 0;
    }
  }
  int x, y, sx, sy;
  sx = x = RandomInteger(0, dungeon->width - 1);
  sy = y = RandomInteger(0, dungeon->height - 1);
  setDungeonStart(dungeon, x, y);
  while (sx == x && sy == y) {
    x = RandomInteger(0, dungeon->width - 1);
    y = RandomInteger(0, dungeon->height - 1);
  }
  setDungeonEnd(dungeon, x, y);
}

void loadDungeon(Dungeon *dungeon, FILE *file) {
  fscanf(file, "%d%d", &dungeon->width, &dungeon->height);

  if (dungeon->width > MaxDungeonWidth)
    dungeon->width = MaxDungeonWidth;
  if (dungeon->width < 1)
    dungeon->width = 2;

  if (dungeon->height > MaxDungeonHeight)
    dungeon->height = MaxDungeonHeight;
  if (dungeon->height < 1)
    dungeon->height = 2;

  for (int i = 0; i < dungeon->width; i++) {
    for (int j = 0; j < dungeon->height; j++) {
      fscanf(file, "%d", &dungeon->mp[i][j]);
      fscanf(file, "%d %d", &dungeon->event[i][j].type,
             &dungeon->event[i][j].arg);
      fscanf(file, "%d %d", &dungeon->item[i][j].type,
             &dungeon->item[i][j].arg);
    }
  }

  sortDungeon(dungeon);

  fclose(file);
}

void saveDungeon(Dungeon *dungeon, FILE *file) {
  sortDungeon(dungeon);

  fprintf(file, "%d %d\n", dungeon->width, dungeon->height);

  for (int i = 0; i < dungeon->width; i++) {
    for (int j = 0; j < dungeon->height; j++) {
      fprintf(file, "%d ", dungeon->mp[i][j]);
      fprintf(file, "%d %d ", dungeon->event[i][j].type,
              dungeon->event[i][j].arg);
      fprintf(file, "%d %d\n", dungeon->item[i][j].type,
              dungeon->item[i][j].arg);
    }
  }

  fclose(file);
}
