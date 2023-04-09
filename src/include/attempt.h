#pragma once

#include "utils.h"
#include "items.h"
#include "itembag.h"
#include "dungeon.h"
#include "pokemon.h"

#define PFace (10)
#define PMove (11)
#define PUseItem (12)
#define PUseMove (13)

int isFaceAttempt(int x) { return pick(x, PFace); }
int isMoveAttempt(int x) { return pick(x, PMove); }
int isUseItemAttempt(int x) { return pick(x, PUseItem); }
int isUseMoveAttempt(int x) { return pick(x, PUseMove); }

int makeFaceAttempt(Direction dir) { return shl(PFace) | dir; }
int makeMoveAttempt(Direction dir) { return shl(PMove) | dir; }
int makeUseItemAttempt(int id) { return shl(PUseItem) | id; }
int makeUseMoveAttempt(int id) { return shl(PUseMove) | id; }

int argFaceAttempt(int x) { return x & ((-1) ^ shl(PFace)); }
int argMoveAttempt(int x) { return x & ((-1) ^ shl(PMove)); }
int argUseItemAttempt(int x) { return x & ((-1) ^ shl(PUseItem)); }
int argUseMoveAttempt(int x) { return x & ((-1) ^ shl(PUseMove)); }