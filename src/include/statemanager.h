#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "appstate.h"

#define MaxStateNumber 99

typedef struct StateStack {
  AppState *stk[MaxStateNumber];
  size_t top;
} StateStack;

StateStack stateStack;
void (*currentStateProc)(void);

void smInit() { stateStack.top = 0; }

bool smIsStateEmpty() { return stateStack.top <= 0; }

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

void smPopState() {
  if (smIsStateEmpty())
    return;

  smTopChanging();
  AppState *now = smStateTop();
  callDtor(now);
  stateStack.stk[--stateStack.top] = NULL;
  smRebuildTop();
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