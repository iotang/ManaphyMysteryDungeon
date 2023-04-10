#include "attempt.h"

int isFaceAttempt(int x) { return pick(x, PFace); }
int isMoveAttempt(int x) { return pick(x, PMove); }
int isUseItemAttempt(int x) { return pick(x, PUseItem); }
int isUseMoveAttempt(int x) { return pick(x, PUseMove); }
int isRemoveItemAttempt(int x) { return pick(x, PRemoveItem); }
int isRemoveMoveAttempt(int x) { return pick(x, PRemoveMove); }

int makeFaceAttempt(Direction dir) { return shl(PFace) | dir; }
int makeMoveAttempt(Direction dir) { return shl(PMove) | dir; }
int makeUseItemAttempt(int id) { return shl(PUseItem) | id; }
int makeUseMoveAttempt(int id) { return shl(PUseMove) | id; }
int makeRemoveItemAttempt(int id) { return shl(PRemoveItem) | id; }
int makeRemoveMoveAttempt(int id) { return shl(PRemoveMove) | id; }

int argFaceAttempt(int x) { return x & ((-1) ^ shl(PFace)); }
int argMoveAttempt(int x) { return x & ((-1) ^ shl(PMove)); }
int argUseItemAttempt(int x) { return x & ((-1) ^ shl(PUseItem)); }
int argUseMoveAttempt(int x) { return x & ((-1) ^ shl(PUseMove)); }
int argRemoveItemAttempt(int x) { return x & ((-1) ^ shl(PRemoveItem)); }
int argRemoveMoveAttempt(int x) { return x & ((-1) ^ shl(PRemoveMove)); }