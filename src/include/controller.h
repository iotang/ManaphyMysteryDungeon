#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "statemanager.h"
#include "dungeon.h"
#include "utils.h"

#define MOVEGAP (333)

extern void (*currentStateProc)(void);

typedef enum TimeEvent {
  ScreenRend,
  MoveRight,
  MoveUp,
  MoveLeft,
  MoveDown
} TimeEvent;

int timerStarted[99] = {0};

void ScreenRender() {
  // printf("sm: %llu\n", stateStack.top);

  DisplayClear();
  if (currentStateProc != NULL) {
    currentStateProc();
  }
}

void (*playerMove)(int event);

void bindPlayerMove(void (*_playerMove)(int)) { playerMove = _playerMove; }

void render(int id) {
  if (id == ScreenRend)
    ScreenRender();
  if (id == MoveRight)
    playerMove(MoveRight);
  if (id == MoveUp)
    playerMove(MoveUp);
  if (id == MoveLeft)
    playerMove(MoveLeft);
  if (id == MoveDown)
    playerMove(MoveDown);
}

int controlPressed = 0;

void controlKeyboard(int key, int event) {
  if (event == KEY_DOWN) {
    if (key == VK_RIGHT || (key == 'D' && !controlPressed)) {
      if (!timerStarted[MoveRight]) {
        playerMove(MoveRight);
        startTimer(MoveRight, MOVEGAP);
        timerStarted[MoveRight] = 1;
      }
    }
    if (key == VK_UP || (key == 'W' && !controlPressed)) {
      if (!timerStarted[MoveUp]) {
        playerMove(MoveUp);
        startTimer(MoveUp, MOVEGAP);
        timerStarted[MoveUp] = 1;
      }
    }
    if (key == VK_LEFT || (key == 'A' && !controlPressed)) {
      if (!timerStarted[MoveLeft]) {
        playerMove(MoveLeft);
        startTimer(MoveLeft, MOVEGAP);
        timerStarted[MoveLeft] = 1;
      }
    }
    if (key == VK_DOWN || (key == 'S' && !controlPressed)) {
      if (!timerStarted[MoveDown]) {
        playerMove(MoveDown);
        startTimer(MoveDown, MOVEGAP);
        timerStarted[MoveDown] = 1;
      }
    }
    if (key == VK_CONTROL) {
      controlPressed = 1;
    }
  } else if (event == KEY_UP) {
    if (key == VK_RIGHT || (key == 'D' && !controlPressed)) {
      cancelTimer(MoveRight);
      timerStarted[MoveRight] = 0;
    }
    if (key == VK_UP || (key == 'W' && !controlPressed)) {
      cancelTimer(MoveUp);
      timerStarted[MoveUp] = 0;
    }
    if (key == VK_LEFT || (key == 'A' && !controlPressed)) {
      cancelTimer(MoveLeft);
      timerStarted[MoveLeft] = 0;
    }
    if (key == VK_DOWN || (key == 'S' && !controlPressed)) {
      cancelTimer(MoveDown);
      timerStarted[MoveDown] = 0;
    }
    if (key == VK_CONTROL) {
      controlPressed = 0;
    }
  }
}