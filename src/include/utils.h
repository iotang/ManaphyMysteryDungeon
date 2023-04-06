#pragma once

#include "string.h"

#define FrameLength (16)

#define WindowWidthInch (16.00)
#define Window43Gap (2.00)
#define Window43Left (2.00)
#define Window43Right (14.00)
#define Window43Width (12.00)
#define WindowHeightInch (9.00)

#define MaxFileNameLength (20)

typedef struct Item {
  char *name;
  char *describe;

  int d_lv, d_exp;
  int d_hp, d_atk, d_def;
  int d_move;
} Item;

typedef enum {
  idMainMenu,
  idAlertDialog,
  idConfirmDialog,
  idNewPage,
  idOpenPage,
  idSaveAsPage,
  idEditPage,
  idExplorer
} AppStateID;

typedef enum TimeEvent {
  ScreenRend,
  MoveRight,
  MoveUp,
  MoveLeft,
  MoveDown,
  FaceRight,
  FaceUp,
  FaceLeft,
  FaceDown,
  HintExpire
} TimeEvent;

typedef enum { Player, Enemy } Role;
typedef enum { RIGHT, UP, LEFT, DOWN } Direction;

#define inf (0x3f3f3f3f)