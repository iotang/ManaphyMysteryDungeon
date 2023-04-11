#include "messagedialog.h"

#include <stdio.h>
#include <string.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "utils.h"

char MessageString[MaxMessageLength] = {0};

void setMessage(char *s) {
  strcpy(MessageString, s);
  cancelTimer(MessageExpire);
  startTimer(MessageExpire, MessageExpireTime);
}

void emplaceMessage(char *s) {
  static char _MessageString[MaxMessageLength - 1];
  strcpy(_MessageString, MessageString);
  sprintf(MessageString, "%s\n%s", _MessageString, s);
  cancelTimer(MessageExpire);
  startTimer(MessageExpire, MessageExpireTime);
}

int isMessageEmpty() { return MessageString[0] == 0; }

void clearMessage() { MessageString[0] = 0; }

void drawMessageDialog() {
  if (!isMessageEmpty()) {
    /*
    SetPenColor("White");
    drawRectangleDensity(Window43Left + Window43Width * 0.1,
                         WindowHeightInch * 0.05, Window43Width * 0.8,
                         WindowHeightInch * 0.2, 1, 0.5);
                         */
    SetPenColor("Pink");

    drawBoldRectangle(Window43Left + Window43Width * 0.08,
                      WindowHeightInch * 0.04, Window43Width * 0.84,
                      WindowHeightInch * 0.21, Window43Width * 0.01);
    drawBoldRectangle(Window43Left + Window43Width * 0.09,
                      WindowHeightInch * 0.04, Window43Width * 0.82,
                      WindowHeightInch * 0.21, Window43Width * 0.01);
    int _pointSize = GetPointSize();
    SetPointSize(24);
    double h = GetFontHeight();

    static char tmp[MaxMessageLine][MaxMessageLength];
    int now = 0;
    char *cur = MessageString, *wr = tmp[now];
    for (int i = 0; i < MaxMessageLine; i++)
      tmp[i][0] = 0;

    while (*cur) {
      *wr++ = *cur++;
      if (*cur == '\n') {
        *wr = 0;
        now++;
        if (now >= MaxMessageLine)
          now = 0;
        wr = tmp[now];
      }
    }
    *wr = 0;
    for (int i = 0; i < MaxMessageLine; i++) {
      now++;
      if (now >= MaxMessageLine)
        now = 0;
      if (tmp[now][0] != 0)
        break;
    }

    for (int i = 0; i < MaxMessageLine; i++) {
      if (tmp[now][0]) {
        static double fuzz[8][2] = {
            {0.08, 0.00}, {-0.08, 0.00}, {0.00, 0.08},   {0.00, -0.08},
            {0.08, 0.08}, {0.08, -0.08}, {-0.08, -0.08}, {-0.08, 0.08}};
        for (int x = 0; x < 8; x++) {
          drawBoxWithoutBorder(
              Window43Left + Window43Width * 0.11 + h * fuzz[x][0],
              WindowHeightInch * 0.245 -
                  WindowHeightInch * 0.19 / MaxMessageLine * (i + 1) -
                  h * fuzz[x][1],
              Window43Width * 0.78, WindowHeightInch * 0.19 / MaxMessageLine, 0,
              tmp[now], 'L', "White");
        }
        drawBoxWithoutBorder(
            Window43Left + Window43Width * 0.11,
            WindowHeightInch * 0.245 -
                WindowHeightInch * 0.19 / MaxMessageLine * (i + 1),
            Window43Width * 0.78, WindowHeightInch * 0.19 / MaxMessageLine, 0,
            tmp[now], 'L', "Black");
        now++;
        if (now >= MaxMessageLine)
          now = 0;
      }
    }
    SetPointSize(_pointSize);
  }
}