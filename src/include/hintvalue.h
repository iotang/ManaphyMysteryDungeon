#pragma once

#include <stdio.h>
#include <string.h>

#include "graphics.h"
#include "utils.h"

#define MaxHintLength (2000)
#define HintExpireTime (5000)

char hintString[MaxHintLength] = {0};

void setHint(char *s) {
  strcpy(hintString, s);
  cancelTimer(HintExpire);
  startTimer(HintExpire, HintExpireTime);
}

void emplaceHint(char *s) {
  static char _hintString[MaxHintLength];
  strcpy(_hintString, hintString);
  sprintf(hintString, "%s\n%s", _hintString, s);
  cancelTimer(HintExpire);
  startTimer(HintExpire, HintExpireTime);
}

int isHintEmpty() { return hintString[0] == 0; }

void clearHint() { hintString[0] = 0; }