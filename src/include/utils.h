#ifndef UTILS_H
#define UTILS_H

#include "string.h"

#define FrameLength (5)

#define WindowWidthInch (16.00)
#define Window43Gap (2.00)
#define Window43Left (2.00)
#define Window43Right (14.00)
#define Window43Width (12.00)
#define WindowHeightInch (9.00)

#define MenuHeight (WindowHeightInch * 0.03)

#define MaxFileNameLength (20)

typedef long long lint;

typedef enum {
  idMainMenu,
  idAboutPage,
  idAlertDialog,
  idConfirmDialog,
  idNewPage,
  idOpenPage,
  idSaveAsPage,
  idEditPage,
  idSimPage,
  idExplorer,
  idPausePage
} AppStateID;

typedef enum TimeEvent {
  ScreenRend,
  FaceRight,
  FaceUp,
  FaceLeft,
  FaceDown,
  MoveRight,
  MoveUp,
  MoveLeft,
  MoveDown,
  MoveNoDirection,
  UseMove1,
  UseMove2,
  UseMove3,
  UseMove4,
  UseMove5,
  MessageExpire,
  AutoRun,
  ClearUsingMove,
  PauseBufferExpire
} TimeEvent;

typedef enum { Player, Enemy } Role;
typedef enum { RIGHT, UP, LEFT, DOWN, NODIRECTION, ERRORDIRECTION } Direction;

extern int go[5][2];

#define inf (0x3f3f3f3f)
#define linf (0x3f3f3f3f3f3f3f3fll)

int pick(int a, int b);
int shl(int a);

#endif