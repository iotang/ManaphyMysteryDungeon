#pragma once

#include <stdio.h>

typedef void (*voidFn)(void);
typedef void (*keyboardCallback)(int button, int event);
typedef void (*charCallback)(int ch);
typedef void (*mouseCallback)(int x, int y, int button, int event);

typedef struct AppState {
  int uid;

  voidFn ctor;
  voidFn proc;
  voidFn dtor;

  keyboardCallback fnKey;
  charCallback fnChar;
  mouseCallback fnMouse;
} AppState;
