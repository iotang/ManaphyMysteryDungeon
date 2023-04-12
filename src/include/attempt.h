#pragma once

#include "utils.h"

#define PFace (10)       // 改变朝向的指令的这一位为 1。
#define PMove (11)       // 移动指令的这一位为 1。
#define PUseItem (12)    // 使用道具指令的这一位为 1。
#define PUseMove (13)    // 使用招式指令的这一位为 1。
#define PRemoveItem (14) // 丢弃道具指令的这一位为 1。
#define PRemoveMove (15) // 遗忘招式指令的这一位为 1。

int isFaceAttempt(int x);
int isMoveAttempt(int x);
int isUseItemAttempt(int x);
int isUseMoveAttempt(int x);
int isRemoveItemAttempt(int x);
int isRemoveMoveAttempt(int x);

int makeFaceAttempt(Direction dir);
int makeMoveAttempt(Direction dir);
int makeUseItemAttempt(int id);
int makeUseMoveAttempt(int id);
int makeRemoveItemAttempt(int id);
int makeRemoveMoveAttempt(int id);

int argFaceAttempt(int x);
int argMoveAttempt(int x);
int argUseItemAttempt(int x);
int argUseMoveAttempt(int x);
int argRemoveItemAttempt(int x);
int argRemoveMoveAttempt(int x);