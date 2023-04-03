#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "statemanager.h"

extern void (*currentStateProc)(void);

typedef enum TimeEvent { ScreenRend } TimeEvent;

void ScreenRender() {
  InitConsole();
  printf("State Stack Size: %llu\n", stateStack.top);

  DisplayClear();
  if (currentStateProc != NULL) {
    currentStateProc();
  }
}

void render(int id) {
  if (id == ScreenRend)
    ScreenRender();
}