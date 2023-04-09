#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "statemanager.h"
#include "dungeon.h"
#include "hintvalue.h"
#include "utils.h"

#define MOVEGAP (150)
#define USEMOVEGAP (1500)

extern void (*currentStateProc)(void);

int timerStarted[99] = {0};
int isUsingMove = 0;

void ScreenRender() {
  DisplayClear();
  if (currentStateProc != NULL) {
    currentStateProc();
  }
}

void (*playerMove)(int event);
void (*autoMove)(void);

void bindPlayerMove(void (*_playerMove)(int)) { playerMove = _playerMove; }
void bindAutoMove(void (*_autoMove)(void)) { autoMove = _autoMove; }

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

  if (id == UseMove1)
    playerMove(UseMove1);
  if (id == UseMove2)
    playerMove(UseMove2);
  if (id == UseMove3)
    playerMove(UseMove3);
  if (id == UseMove4)
    playerMove(UseMove4);
  if (id == UseMove5)
    playerMove(UseMove5);

  if (id == HintExpire)
    clearHint();
  if (id == AutoRun)
    autoMove();

  if (id == ClearUsingMove) {
    isUsingMove = 0;
    cancelTimer(ClearUsingMove);
  }
}

int controlPressed = 0;
int shiftPressed = 0;

void controlKeyboard(int key, int event) {
  if (event == KEY_DOWN) {
    if (key == VK_RIGHT || (key == 'D' && !controlPressed)) {
      if (shiftPressed) {
        playerMove(FaceRight);
      } else if (!timerStarted[MoveRight]) {
        playerMove(MoveRight);
        startTimer(MoveRight, MOVEGAP);
        timerStarted[MoveRight] = 1;
      }
    }
    if (key == VK_UP || (key == 'W' && !controlPressed)) {
      if (shiftPressed) {
        playerMove(FaceUp);
      } else if (!timerStarted[MoveUp]) {
        playerMove(MoveUp);
        startTimer(MoveUp, MOVEGAP);
        timerStarted[MoveUp] = 1;
      }
    }
    if (key == VK_LEFT || (key == 'A' && !controlPressed)) {
      if (shiftPressed) {
        playerMove(FaceLeft);
      } else if (!timerStarted[MoveLeft]) {
        playerMove(MoveLeft);
        startTimer(MoveLeft, MOVEGAP);
        timerStarted[MoveLeft] = 1;
      }
    }
    if (key == VK_DOWN || (key == 'S' && !controlPressed)) {
      if (shiftPressed) {
        playerMove(FaceDown);
      } else if (!timerStarted[MoveDown]) {
        playerMove(MoveDown);
        startTimer(MoveDown, MOVEGAP);
        timerStarted[MoveDown] = 1;
      }
    }
    if (key == '1') {
      if (!isUsingMove) {
        playerMove(UseMove1);
        isUsingMove = 1;
        startTimer(ClearUsingMove, USEMOVEGAP);
        // startTimer(UseMove1, USEMOVEGAP);
        // timerStarted[UseMove1] = 1;
      }
    }
    if (key == '2') {
      if (!isUsingMove) {
        playerMove(UseMove2);
        isUsingMove = 1;
        startTimer(ClearUsingMove, USEMOVEGAP);
        // startTimer(UseMove2, USEMOVEGAP);
        // timerStarted[UseMove2] = 1;
      }
    }
    if (key == '3') {
      if (!isUsingMove) {
        playerMove(UseMove3);
        isUsingMove = 1;
        startTimer(ClearUsingMove, USEMOVEGAP);
        // startTimer(UseMove3, USEMOVEGAP);
        // timerStarted[UseMove3] = 1;
      }
    }
    if (key == '4') {
      if (!isUsingMove) {
        playerMove(UseMove4);
        isUsingMove = 1;
        startTimer(ClearUsingMove, USEMOVEGAP);
        // startTimer(UseMove4, USEMOVEGAP);
        // timerStarted[UseMove4] = 1;
      }
    }
    if (key == '5') {
      if (!isUsingMove) {
        playerMove(UseMove5);
        isUsingMove = 1;
        startTimer(ClearUsingMove, USEMOVEGAP);
        // startTimer(UseMove5, USEMOVEGAP);
        // timerStarted[UseMove5] = 1;
      }
    }
    if (key == VK_CONTROL) {
      controlPressed = 1;
    }
    if (key == VK_SHIFT) {
      shiftPressed = 1;
    }
  } else if (event == KEY_UP) {
    if (key == VK_RIGHT || (key == 'D' && !controlPressed)) {
      if (timerStarted[MoveRight]) {
        cancelTimer(MoveRight);
        timerStarted[MoveRight] = 0;
      }
    }
    if (key == VK_UP || (key == 'W' && !controlPressed)) {
      if (timerStarted[MoveUp]) {
        cancelTimer(MoveUp);
        timerStarted[MoveUp] = 0;
      }
    }
    if (key == VK_LEFT || (key == 'A' && !controlPressed)) {
      if (timerStarted[MoveLeft]) {
        cancelTimer(MoveLeft);
        timerStarted[MoveLeft] = 0;
      }
    }
    if (key == VK_DOWN || (key == 'S' && !controlPressed)) {
      if (timerStarted[MoveDown]) {
        cancelTimer(MoveDown);
        timerStarted[MoveDown] = 0;
      }
    }
    if (key == '1') {
      if (isUsingMove) {
        isUsingMove = 0;
        cancelTimer(ClearUsingMove);
        // cancelTimer(UseMove1);
        // timerStarted[UseMove1] = 0;
      }
    }
    if (key == '2') {
      if (isUsingMove) {
        isUsingMove = 0;
        cancelTimer(ClearUsingMove);
        // cancelTimer(UseMove2);
        // timerStarted[UseMove2] = 0;
      }
    }
    if (key == '3') {
      if (isUsingMove) {
        isUsingMove = 0;
        cancelTimer(ClearUsingMove);
        // cancelTimer(UseMove3);
        // timerStarted[UseMove3] = 0;
      }
    }
    if (key == '4') {
      if (isUsingMove) {
        isUsingMove = 0;
        cancelTimer(ClearUsingMove);
        // cancelTimer(UseMove4);
        // timerStarted[UseMove4] = 0;
      }
    }
    if (key == '5') {
      if (isUsingMove) {
        isUsingMove = 0;
        cancelTimer(ClearUsingMove);
        // cancelTimer(UseMove5);
        // timerStarted[UseMove5] = 0;
      }
    }
    if (key == VK_CONTROL) {
      controlPressed = 0;
    }
    if (key == VK_SHIFT) {
      shiftPressed = 0;
    }
  }
}