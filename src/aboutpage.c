#include "aboutpage.h"

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "statemanager.h"
#include "utils.h"

void drawAboutPage() {
  int _pointSize = GetPointSize();

  SetPointSize(64);
  drawBoxWithoutBorder(Window43Left, WindowHeightInch * 0.7, Window43Width,
                       WindowHeightInch * 0.2, 0, ProjectName, 'C', "Blue");
  SetPointSize(24);
  drawBoxWithoutBorder(Window43Left, WindowHeightInch * 0.6, Window43Width,
                       WindowHeightInch * 0.1, 0, "A Simple Dungeon Editor",
                       'C', "Black");
  SetPointSize(18);
  drawBoxWithoutBorder(Window43Left + Window43Width * 0.3,
                       WindowHeightInch * 0.5, Window43Width * 0.4,
                       WindowHeightInch * 0.05, 0, "Developer", 'L', "Black");
  drawBoxWithoutBorder(Window43Left + Window43Width * 0.3,
                       WindowHeightInch * 0.5, Window43Width * 0.4,
                       WindowHeightInch * 0.05, 0, "iotang", 'R', "Black");
  drawBoxWithoutBorder(Window43Left + Window43Width * 0.3,
                       WindowHeightInch * 0.45, Window43Width * 0.4,
                       WindowHeightInch * 0.05, 0, "Designer", 'L', "Black");
  drawBoxWithoutBorder(Window43Left + Window43Width * 0.3,
                       WindowHeightInch * 0.45, Window43Width * 0.4,
                       WindowHeightInch * 0.05, 0, "Kate da Explorers of Sky",
                       'R', "Black");
  drawBoxWithoutBorder(Window43Left + Window43Width * 0.3,
                       WindowHeightInch * 0.4, Window43Width * 0.4,
                       WindowHeightInch * 0.05, 0, "Special Thanks", 'L',
                       "Black");
  drawBoxWithoutBorder(Window43Left + Window43Width * 0.3,
                       WindowHeightInch * 0.4, Window43Width * 0.4,
                       WindowHeightInch * 0.05, 0,
                       "Nintendo, GameFreak, ChunSoft", 'R', "Black");

  if (button(GenUIID(0), Window43Left + Window43Width * 0.4,
             WindowHeightInch * 0.2, Window43Width * 0.2,
             WindowHeightInch * 0.05, "OK", idAboutPage)) {
    smPopState();
  }

  SetPointSize(_pointSize);
}

void uiAboutPageGetKeyboard(int key, int event) {
  if (smStateTop()->uid == idAboutPage)
    uiGetKeyboard(key, event);
}

void uiAboutPageGetChar(int ch) {
  if (smStateTop()->uid == idAboutPage)
    uiGetChar(ch);
}

void uiAboutPageGetMouse(int x, int y, int button, int event) {
  if (smStateTop()->uid == idAboutPage)
    uiGetMouse(x, y, button, event);
}

AppState AboutPage = {idAboutPage,
                      NULL,
                      drawAboutPage,
                      NULL,
                      uiAboutPageGetKeyboard,
                      uiAboutPageGetChar,
                      uiAboutPageGetMouse};

void gotoAboutPage() { smPushState(&AboutPage); }