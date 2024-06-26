#include "controller.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "utils.h"
#include "statemanager.h"
#include "messagedialog.h"
#include "pausepage.h"

extern void (*currentStateProc)(void); // 当前状态的运行函数。在每一帧调用。

int timerStarted[99] = {0}; // 定时器是否已经启动。
int isUsingMove = 0;        // 是否有招式键被按下。
int pauseBuffer = 0; // 用于优化暂停下的回调阻塞。克雷色利亚教我写的

int controlPressed = 0; // 是否按下了 Control。
int shiftPressed = 0;   // 是否按下了 Shift。

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

void playerMoves(int event) {
  if (!isPausing())
    playerMove(event);
}

void clearTimers() {
  timerStarted[MoveRight] = 0;
  timerStarted[MoveUp] = 0;
  timerStarted[MoveLeft] = 0;
  timerStarted[MoveDown] = 0;
  timerStarted[MoveNoDirection] = 0;
  isUsingMove = 0;
  controlPressed = 0;
  shiftPressed = 0;
  cancelTimer(MoveRight);
  cancelTimer(MoveUp);
  cancelTimer(MoveLeft);
  cancelTimer(MoveDown);
  cancelTimer(MoveNoDirection);
  cancelTimer(ClearUsingMove);
}

void render(int id) {
  if (pauseBuffer) {
    clearTimers();
  }
  if (id == ScreenRend) {
    ScreenRender();
  }
  if (id == MoveRight)
    playerMoves(MoveRight);
  if (id == MoveUp)
    playerMoves(MoveUp);
  if (id == MoveLeft)
    playerMoves(MoveLeft);
  if (id == MoveDown)
    playerMoves(MoveDown);
  if (id == MoveNoDirection)
    playerMoves(MoveNoDirection);

  if (id == MessageExpire)
    clearMessage();
  if (id == AutoRun)
    autoMove();

  if (id == ClearUsingMove) {
    isUsingMove = 0;
    cancelTimer(ClearUsingMove);
  }

  if (id == PauseBufferExpire) {
    pauseBuffer = 0;
  }
}

void setPauseBuffer() {
  pauseBuffer = 1;
  startTimer(PauseBufferExpire, 50);
}

void controlKeyboard(int key, int event) {
  if (pauseBuffer) {
    clearTimers();
    return;
  }

  if (event == KEY_DOWN) {
    if (key == VK_RIGHT || (key == 'D' && !controlPressed)) {
      if (shiftPressed) {
        playerMoves(FaceRight);
      } else if (!timerStarted[MoveRight]) {
        playerMoves(MoveRight);
        startTimer(MoveRight, MoveGap);
        timerStarted[MoveRight] = 1;
      }
    }
    if (key == VK_UP || (key == 'W' && !controlPressed)) {
      if (shiftPressed) {
        playerMoves(FaceUp);
      } else if (!timerStarted[MoveUp]) {
        playerMoves(MoveUp);
        startTimer(MoveUp, MoveGap);
        timerStarted[MoveUp] = 1;
      }
    }
    if (key == VK_LEFT || (key == 'A' && !controlPressed)) {
      if (shiftPressed) {
        playerMoves(FaceLeft);
      } else if (!timerStarted[MoveLeft]) {
        playerMoves(MoveLeft);
        startTimer(MoveLeft, MoveGap);
        timerStarted[MoveLeft] = 1;
      }
    }
    if (key == VK_DOWN || (key == 'S' && !controlPressed)) {
      if (shiftPressed) {
        playerMoves(FaceDown);
      } else if (!timerStarted[MoveDown]) {
        playerMoves(MoveDown);
        startTimer(MoveDown, MoveGap);
        timerStarted[MoveDown] = 1;
      }
    }
    if (key == ' ') {
      if (!timerStarted[MoveNoDirection]) {
        playerMoves(MoveNoDirection);
        startTimer(MoveNoDirection, MoveGap);
        timerStarted[MoveNoDirection] = 1;
      }
    }
    if (key == '1') {
      if (!isUsingMove) {
        playerMoves(UseMove1);
        isUsingMove = 1;
        startTimer(ClearUsingMove, UseMoveGap);
      }
    }
    if (key == '2') {
      if (!isUsingMove) {
        playerMoves(UseMove2);
        isUsingMove = 1;
        startTimer(ClearUsingMove, UseMoveGap);
      }
    }
    if (key == '3') {
      if (!isUsingMove) {
        playerMoves(UseMove3);
        isUsingMove = 1;
        startTimer(ClearUsingMove, UseMoveGap);
      }
    }
    if (key == '4') {
      if (!isUsingMove) {
        playerMoves(UseMove4);
        isUsingMove = 1;
        startTimer(ClearUsingMove, UseMoveGap);
      }
    }
    if (key == '5') {
      if (!isUsingMove) {
        playerMoves(UseMove5);
        isUsingMove = 1;
        startTimer(ClearUsingMove, UseMoveGap);
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
    if (key == ' ') {
      if (timerStarted[MoveNoDirection]) {
        cancelTimer(MoveNoDirection);
        timerStarted[MoveNoDirection] = 0;
      }
    }
    if (key >= '1' && key <= '5') {
      isUsingMove = 0;
    }
    if (key == VK_CONTROL) {
      controlPressed = 0;
    }
    if (key == VK_SHIFT) {
      shiftPressed = 0;
    }
  }
}