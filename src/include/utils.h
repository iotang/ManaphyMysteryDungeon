#pragma once

#include "string.h"

#define ProjectName ("Manaphy's Mystery Dungeon") // 这个项目的全名。

#define FrameLength (5) // 每一帧的时间长度。

#define WindowWidthInch (16.00) // 窗口的宽度。
#define Window43Gap (2.00)  // 16:9 相比 4:3 左右分别多出来的长度。
#define Window43Left (2.00) // 4:3 相对 16:9 的宽度的开始位置。
#define Window43Right (14.00)   // 4:3 相对 16:9 的宽度的结束位置。
#define Window43Width (12.00)   // 4:3 下窗口的宽度。
#define WindowHeightInch (9.00) // 窗口的高度。

#define MenuHeight (WindowHeightInch * 0.03) // 工具栏等栏目的高度。

#define MaxFileNameLength (20) // 文件名长度的最大值。

typedef long long lint;

typedef enum {
  idMainMenu,
  idHelpPage,
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
} AppStateID; // 状态 ID。

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
} TimeEvent; // 计时器事件 ID。

typedef enum { Player, Enemy } Role; // 角色的阵营。
typedef enum {
  RIGHT,
  UP,
  LEFT,
  DOWN,
  NODIRECTION,
  ERRORDIRECTION
} Direction; // 朝向。

extern int go[5][2]; // 朝向与坐标的对应换算。

#define inf (0x3f3f3f3f) // int 下的无穷大，可以被 memset 出来。
#define linf (0x3f3f3f3f3f3f3f3fll) // long long 下的无穷大。

int pick(int a, int b);
int shl(int a);