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

Pokemon *player = NULL;
Dungeon *dungeon = NULL;

void bindingControl(Pokemon *_player, Dungeon *_dungeon) {
  player = _player;
  dungeon = _dungeon;
}

void ScreenRender() {
  /*
  InitConsole();
  printf("State Stack Size: %llu\n", stateStack.top);
*/

  DisplayClear();
  if (currentStateProc != NULL) {
    currentStateProc();
  }
}

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

void playerMove(int event) {
  if (!player || !dungeon)
    return;
  if (event == MoveRight) {
    if (player->x + 1 < dungeon->width) {
      player->x++;
    }
  }
  if (event == MoveUp) {
    if (player->y + 1 < dungeon->height) {
      player->y++;
    }
  }
  if (event == MoveLeft) {
    if (player->x > 0) {
      player->x--;
    }
  }
  if (event == MoveDown) {
    if (player->y > 0) {
      player->y--;
    }
  }
}

void controlKeyboard(int key, int event) {
  if (event == KEY_DOWN) {
    if (key == VK_RIGHT || key == 'D') {
      if (!timerStarted[MoveRight]) {
        playerMove(MoveRight);
        startTimer(MoveRight, MOVEGAP);
        timerStarted[MoveRight] = 1;
      }
    }
    if (key == VK_UP || key == 'W') {
      if (!timerStarted[MoveUp]) {
        playerMove(MoveUp);
        startTimer(MoveUp, MOVEGAP);
        timerStarted[MoveUp] = 1;
      }
    }
    if (key == VK_LEFT || key == 'A') {
      if (!timerStarted[MoveLeft]) {
        playerMove(MoveLeft);
        startTimer(MoveLeft, MOVEGAP);
        timerStarted[MoveLeft] = 1;
      }
    }
    if (key == VK_DOWN || key == 'S') {
      if (!timerStarted[MoveDown]) {
        playerMove(MoveDown);
        startTimer(MoveDown, MOVEGAP);
        timerStarted[MoveDown] = 1;
      }
    }
  } else if (event == KEY_UP) {
    if (key == VK_RIGHT || key == 'D') {
      cancelTimer(MoveRight);
      timerStarted[MoveRight] = 0;
    }
    if (key == VK_UP || key == 'W') {
      cancelTimer(MoveUp);
      timerStarted[MoveUp] = 0;
    }
    if (key == VK_LEFT || key == 'A') {
      cancelTimer(MoveLeft);
      timerStarted[MoveLeft] = 0;
    }
    if (key == VK_DOWN || key == 'S') {
      cancelTimer(MoveDown);
      timerStarted[MoveDown] = 0;
    }
  }
}