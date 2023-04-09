#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "utils.h"
#include "pokemon.h"

void drawStatusBar(Pokemon *pokemon, double basex, double basey, int belong) {
  SetPenColor("Black");

  double expRatio = 0.01 * pokemon->exp;
  int _pointSize = GetPointSize();
  SetPointSize(16);
  drawBoxWithoutBorder(basex + Window43Gap * 0.05,
                       basey + WindowHeightInch * 0.18, Window43Gap * 0.90,
                       WindowHeightInch * 0.03, 0, pokemon->name, 'L', "Black");
  char lvTag[99];
  sprintf(lvTag, "%d", pokemon->lv);
  SetPenColor("Light Gray");
  drawRectangle(basex + Window43Gap * 0.05, basey + WindowHeightInch * 0.153,
                Window43Gap * 0.90, WindowHeightInch * 0.01, 1);
  SetPenColor("Cyan");
  if (expRatio > 0) {
    drawRectangle(basex + Window43Gap * 0.05, basey + WindowHeightInch * 0.153,
                  Window43Gap * 0.90 * expRatio, WindowHeightInch * 0.01, 1);
  }
  drawBoxWithoutBorder(basex + Window43Gap * 0.05,
                       basey + WindowHeightInch * 0.15, Window43Gap * 0.90,
                       WindowHeightInch * 0.03, 0, "Lv.", 'L', "Black");
  drawBoxWithoutBorder(basex + Window43Gap * 0.05,
                       basey + WindowHeightInch * 0.15, Window43Gap * 0.90,
                       WindowHeightInch * 0.03, 0, lvTag, 'R', "Black");
  SetPointSize(_pointSize);

  double hpRatio = 1.00 * pokemon->hp / pokemon->maxhp;
  if (hpRatio > 1)
    hpRatio = 1;
  if (hpRatio < 0)
    hpRatio = 0;

  SetPenColor("Light Gray");
  drawRectangle(basex + Window43Gap * 0.05, basey + WindowHeightInch * 0.09,
                Window43Gap * 0.90, WindowHeightInch * 0.05, 1);
  if (hpRatio > 0.50) {
    SetPenColor("Green");
  } else if (hpRatio > 0.20) {
    SetPenColor("Yellow");
  } else {
    SetPenColor("Red");
  }
  if (hpRatio > 0) {
    drawRectangle(basex + Window43Gap * 0.05, basey + WindowHeightInch * 0.09,
                  Window43Gap * 0.90 * hpRatio, WindowHeightInch * 0.05, 1);
  }

  SetPenColor("Black");
  char hpTag[99];
  _pointSize = GetPointSize();
  SetPointSize(32);
  sprintf(hpTag, "%d / %d", pokemon->hp, pokemon->maxhp);
  drawBoxWithoutBorder(basex + Window43Gap * 0.05,
                       basey + WindowHeightInch * 0.093, Window43Gap * 0.90,
                       WindowHeightInch * 0.05, 0, hpTag, 'R', "Black");
  SetPointSize(_pointSize);

  char bellyTag[99];
  double bellyRatio = 1.00 * pokemon->belly / pokemon->maxbelly;
  sprintf(bellyTag, "%.0lf%%", pokemon->belly);
  SetPenColor("Light Gray");
  drawRectangle(basex + Window43Gap * 0.05, basey + WindowHeightInch * 0.063,
                Window43Gap * 0.90, WindowHeightInch * 0.02, 1);
  SetPenColor("Orange");
  if (bellyRatio > 0) {
    drawRectangle(basex + Window43Gap * 0.05, basey + WindowHeightInch * 0.063,
                  Window43Gap * 0.90 * bellyRatio, WindowHeightInch * 0.02, 1);
  }
  drawBoxWithoutBorder(basex + Window43Gap * 0.05,
                       basey + WindowHeightInch * 0.06, Window43Gap * 0.90,
                       WindowHeightInch * 0.03, 0, "Belly", 'L', "Black");
  drawBoxWithoutBorder(basex + Window43Gap * 0.05,
                       basey + WindowHeightInch * 0.06, Window43Gap * 0.90,
                       WindowHeightInch * 0.03, 0, bellyTag, 'R', "Black");
  SetPointSize(_pointSize);

  _pointSize = GetPointSize();
  SetPointSize(16);
  char statTag[99];
  sprintf(statTag, "%d", pokemon->atk);
  SetPenColor("Light Gray");
  drawBoxWithoutBorder(basex + Window43Gap * 0.05,
                       basey + WindowHeightInch * 0.03, Window43Gap * 0.90,
                       WindowHeightInch * 0.03, 0, "Attack", 'L', "Black");
  drawBoxWithoutBorder(basex + Window43Gap * 0.05,
                       basey + WindowHeightInch * 0.03, Window43Gap * 0.90,
                       WindowHeightInch * 0.03, 0, statTag, 'R', "Black");
  sprintf(statTag, "%d", pokemon->def);
  SetPenColor("Light Gray");
  drawBoxWithoutBorder(basex + Window43Gap * 0.05, basey, Window43Gap * 0.90,
                       WindowHeightInch * 0.03, 0, "Defense", 'L', "Black");
  drawBoxWithoutBorder(basex + Window43Gap * 0.05, basey, Window43Gap * 0.90,
                       WindowHeightInch * 0.03, 0, statTag, 'R', "Black");
  SetPointSize(_pointSize);
}

int drawMoveList(Pokemon *pokemon, double basex, double basey, int belong) {
  SetPenColor("White");
  drawRectangle(basex + Window43Gap * 0.03, basey + WindowHeightInch * 0.005,
                Window43Gap * 0.94, WindowHeightInch * 0.49, 1);

  int ret = -1;
  for (int i = 0; i < pokemon->moveCount; i++) {
    double baseHeight = basey + WindowHeightInch * (0.40 - i * 0.10 + 0.01);
    SetPenColor("White");

    setButtonColors("White", "Blue", "Cyan", "Blue", 1);
    if (button(GenUIID(i), basex + Window43Gap * 0.05, baseHeight,
               Window43Gap * 0.9, WindowHeightInch * 0.08, NULL, belong)) {
      ret = i;
    }

    SetPenColor("Black");
    int _pointSize = GetPointSize();
    SetPointSize(18);
    drawBoxWithoutBorder(basex + Window43Gap * 0.05,
                         baseHeight + WindowHeightInch * 0.03,
                         Window43Gap * 0.9, WindowHeightInch * 0.05, 0,
                         movedex[pokemon->move[i]].name, 'C', "Black");
    SetPointSize(_pointSize);
    _pointSize = GetPointSize();
    SetPointSize(8);
    char effectTag[99];
    sprintf(effectTag, "Eff. %d", movedex[pokemon->move[i]].effect);
    char ppTag[99];
    if (movedex[pokemon->move[i]].pp > 0) {
      sprintf(ppTag, "%d / %d", pokemon->pp[i], movedex[pokemon->move[i]].pp);
    } else {
      sprintf(ppTag, "Unlimited");
    }

    drawBoxWithoutBorder(basex + Window43Gap * 0.05, baseHeight,
                         Window43Gap * 0.9, WindowHeightInch * 0.03, 0,
                         effectTag, 'L', "Black");
    drawBoxWithoutBorder(basex + Window43Gap * 0.05, baseHeight,
                         Window43Gap * 0.9, WindowHeightInch * 0.03, 0, ppTag,
                         'R', "Black");
    SetPointSize(_pointSize);
  }

  return ret;
}
