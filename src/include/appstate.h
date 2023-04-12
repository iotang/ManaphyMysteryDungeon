#pragma once

#include <stdio.h>

typedef void (*voidFn)(void);
typedef void (*keyboardCallback)(int button, int event); // 键盘回调函数。
typedef void (*charCallback)(int ch); // 输入字符回调函数。
typedef void (*mouseCallback)(int x, int y, int button,
                              int event); // 鼠标回调函数。

typedef struct AppState {
  int uid; // 用于区分不同的状态。

  voidFn ctor; // 状态的构造函数。
  voidFn proc; // 状态的运行函数。
  voidFn dtor; // 状态的析构函数。

  keyboardCallback fnKey; // 状态的键盘回调函数。
  charCallback fnChar;    // 状态的输入字符回调函数。
  mouseCallback fnMouse;  // 状态的鼠标回调函数。
} AppState;
