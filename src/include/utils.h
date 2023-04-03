#pragma once

#include "string.h"

#define FrameLength (16)

#define WindowWidthInch (12.00)
#define WindowHeightInch (9.00)
#define MaxMoveCount (4)

typedef enum { Player, Enemy } Role;

typedef struct Move {
  char *name;
  int effect;
} Move;

typedef struct Pokemon {
  Role role;

  char *name;
  int species;
  int lv, exp;

  int hp;
  int atk, def;

  Move *move[MaxMoveCount];

  int x, y;
} Pokemon;

typedef struct Item {
  char *name;
  char *describe;

  int d_lv, d_exp;
  int d_hp, d_atk, d_def;
  int d_move;
} Item;
