#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "utils.h"
#include "statemanager.h"
#include "imagesupport.h"

#define HelpPageCount (4)

HBITMAP spriteHelpPages[HelpPageCount];
int helpPageNumber;

void readHelpPageSprites() {
  static char _fileName[99];
  for (int i = 0; i < HelpPageCount; i++) {
    sprintf(_fileName, "assets/helps/%02d.bmp", i + 1);
    spriteHelpPages[i] = readBmpImage(_fileName);
  }
}

void initHelpPage() { helpPageNumber = 0; }

void drawHelpPage() {
  drawBmp(spriteHelpPages[helpPageNumber], WindowWidthInch / 2,
          WindowHeightInch / 2, WindowWidthInch, WindowHeightInch);

  setButtonColors("White", "Blue", "Blue", "White", 1);
  if (helpPageNumber > 0) {
    if (button(GenUIID(0), WindowWidthInch * 0.59, WindowHeightInch * 0.01,
               WindowWidthInch * 0.1, WindowHeightInch * 0.03, "Previous Page",
               idHelpPage)) {
      helpPageNumber--;
    }
  }
  if (helpPageNumber + 1 < HelpPageCount) {
    if (button(GenUIID(1), WindowWidthInch * 0.71, WindowHeightInch * 0.01,
               WindowWidthInch * 0.1, WindowHeightInch * 0.03, "Next Page",
               idHelpPage)) {
      helpPageNumber++;
    }
  }

  if (button(GenUIID(1), WindowWidthInch * 0.87, WindowHeightInch * 0.01,
             WindowWidthInch * 0.1, WindowHeightInch * 0.03, "Quit",
             idHelpPage)) {
    smPopState();
  }
}

void uiHelpPageGetKeyboard(int key, int event) {
  if (smStateTop()->uid == idHelpPage)
    uiGetKeyboard(key, event);
}

void uiHelpPageGetChar(int ch) {
  if (smStateTop()->uid == idHelpPage)
    uiGetChar(ch);
}

void uiHelpPageGetMouse(int x, int y, int button, int event) {
  if (smStateTop()->uid == idHelpPage)
    uiGetMouse(x, y, button, event);
}

AppState HelpPage = {idHelpPage,
                     initHelpPage,
                     drawHelpPage,
                     NULL,
                     uiHelpPageGetKeyboard,
                     uiHelpPageGetChar,
                     uiHelpPageGetMouse};

void gotoHelpPage() { smPushState(&HelpPage); }