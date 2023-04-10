#pragma once

#include "utils.h"

#define PFace (10)
#define PMove (11)
#define PUseItem (12)
#define PUseMove (13)
#define PRemoveItem (14)
#define PRemoveMove (15)

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