#include "statemanager.h"

#define MaxStateNumber (99) // 最大的状态堆栈数量。（实际上连 10 都不会超过。）

typedef struct StateStack {
  AppState *stk[MaxStateNumber];
  size_t top;
} StateStack; // 用于存放状态的栈数据结构。

StateStack stateStack; // 用于存放状态的栈。对于其他模块不可见。
void (*currentStateProc)(void); // 当前状态的运行函数。

void smInit() { stateStack.top = 0; }

bool smIsStateEmpty() { return stateStack.top <= 0; }

int smStateCount() { return stateStack.top; }

AppState *smStateTop() {
  if (smIsStateEmpty())
    return NULL;
  return stateStack.stk[stateStack.top - 1];
}

void callCtor(AppState *now) {
  if (now->ctor != NULL)
    now->ctor();
}

void callProc(AppState *now) {
  if (now->proc != NULL)
    now->proc();
}

void callDtor(AppState *now) {
  if (now->dtor != NULL)
    now->dtor();
}

void smLastProc() {
  if (stateStack.top > 1) {
    callProc(stateStack.stk[stateStack.top - 2]);
  }
}

void smTopChanging() { currentStateProc = NULL; }

void smRebuildTop() {
  if (smIsStateEmpty()) {
    registerKeyboardEvent(NULL);
    registerCharEvent(NULL);
    registerMouseEvent(NULL);
    return;
  }

  AppState *now = smStateTop();
  callCtor(now);
  if (now->proc != NULL) {
    currentStateProc = now->proc;
  }
  registerKeyboardEvent(now->fnKey);
  registerCharEvent(now->fnChar);
  registerMouseEvent(now->fnMouse);
}

void smPushState(AppState *state) {
  smTopChanging();
  stateStack.stk[stateStack.top++] = state;
  smRebuildTop();
}

void smBarePushState(AppState *state) {
  stateStack.stk[stateStack.top++] = state;
}

void smPopState() {
  if (smIsStateEmpty())
    return;

  smTopChanging();
  AppState *now = smStateTop();
  callDtor(now);
  stateStack.stk[--stateStack.top] = NULL;
  smRebuildTop();
}

void smBarePopState() {
  if (smIsStateEmpty())
    return;
  stateStack.stk[--stateStack.top] = NULL;
}

void smPopStateUntil(int target) {
  if (smIsStateEmpty())
    return;

  smTopChanging();
  while (!smIsStateEmpty()) {
    AppState *now = smStateTop();
    if (now->uid == target)
      break;

    callDtor(now);
    stateStack.stk[--stateStack.top] = NULL;
  }

  smRebuildTop();
}

void smClearState() {
  do {
    smPopState();
  } while (!smIsStateEmpty());
}