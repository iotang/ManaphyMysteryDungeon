#pragma once

#include <stdio.h>
#include <string.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "utils.h"

#define MaxHintLength (200000)
#define HintExpireTime (3000)
#define MaxHintLine (4)

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

void drawHintDialog() {
  if (!isHintEmpty()) {
    /*
    SetPenColor("White");
    drawRectangle(Window43Left + Window43Width * 0.1, WindowHeightInch * 0.05,
                   Window43Width * 0.8, WindowHeightInch * 0.2, 1);
    */
    SetPenColor("Pink");
    drawRectangle(Window43Left + Window43Width * 0.1, WindowHeightInch * 0.05,
                  Window43Width * 0.8, WindowHeightInch * 0.2, 0);
    drawRectangle(Window43Left + Window43Width * 0.11, WindowHeightInch * 0.055,
                  Window43Width * 0.78, WindowHeightInch * 0.19, 0);
    int _pointSize = GetPointSize();
    SetPointSize(24);
    double h = GetFontHeight();

    static char tmp[MaxHintLine][MaxHintLength];
    int now = 0;
    char *cur = hintString, *wr = tmp[now];
    for (int i = 0; i < MaxHintLine; i++)
      tmp[i][0] = 0;

    while (*cur) {
      *wr++ = *cur++;
      if (*cur == '\n') {
        *wr = 0;
        now++;
        if (now >= MaxHintLine)
          now = 0;
        wr = tmp[now];
      }
    }
    *wr = 0;
    for (int i = 0; i < MaxHintLine; i++) {
      now++;
      if (now >= MaxHintLine)
        now = 0;
      if (tmp[now][0] != 0)
        break;
    }

    for (int i = 0; i < MaxHintLine; i++) {
      if (tmp[now][0]) {
        drawBoxWithoutBorder(
            Window43Left + Window43Width * 0.11 + h * 0.08,
            WindowHeightInch * 0.245 -
                WindowHeightInch * 0.19 / MaxHintLine * (i + 1) - h * 0.08,
            Window43Width * 0.78, WindowHeightInch * 0.19 / MaxHintLine, 0,
            tmp[now], 'L', "White");
        drawBoxWithoutBorder(
            Window43Left + Window43Width * 0.11,
            WindowHeightInch * 0.245 -
                WindowHeightInch * 0.19 / MaxHintLine * (i + 1),
            Window43Width * 0.78, WindowHeightInch * 0.19 / MaxHintLine, 0,
            tmp[now], 'L', "Black");
        now++;
        if (now >= MaxHintLine)
          now = 0;
      }
    }
    SetPointSize(_pointSize);
  }
}