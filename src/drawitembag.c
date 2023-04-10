#include "drawitembag.h"

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "utils.h"
#include "items.h"
#include "attempt.h"
#include "pokemon.h"

int drawItemBag(ItemBag *itemBag, double basex, double basey, int belong) {
  sortItemBag(itemBag);

  int _pointSize = GetPointSize();
  SetPenColor("White");
  drawRectangle(basex + Window43Gap * 0.03, basey + WindowHeightInch * 0.005,
                Window43Gap * 0.94, WindowHeightInch * 0.49, 1);

  int ret = -1, retval = 0,
      maxPage = (itemBag->count + ItemPerPage - 1) / ItemPerPage;
  double height = WindowHeightInch * 0.45 / ItemPerPage;
  if (maxPage <= 0)
    maxPage = 1;

  SetPointSize(16);
  setButtonColors("White", "Blue", "Cyan", "Blue", 1);
  if (button(GenUIID(0), basex + Window43Gap * 0.05,
             basey + WindowHeightInch * 0.46, Window43Gap * 0.22,
             WindowHeightInch * 0.03, "<", belong)) {
    itemBag->currentPage--;
  }

  SetPenColor("White");
  char _pageBuf[99];
  sprintf(_pageBuf, "%d / %d", itemBag->currentPage + 1, maxPage);
  drawBox(basex + Window43Gap * 0.30, basey + WindowHeightInch * 0.46,
          Window43Gap * 0.40, WindowHeightInch * 0.03, 1, _pageBuf, 'C',
          "Black");

  if (button(GenUIID(0), basex + Window43Gap * 0.73,
             basey + WindowHeightInch * 0.46, Window43Gap * 0.22,
             WindowHeightInch * 0.03, ">", belong)) {
    itemBag->currentPage++;
  }

  if (itemBag->currentPage <= 0)
    itemBag->currentPage = 0;

  if (itemBag->currentPage >= maxPage)
    itemBag->currentPage = maxPage - 1;

  SetPointSize(12);
  for (int i = 0; i < ItemPerPage; i++) {
    int num = i + itemBag->currentPage * ItemPerPage;
    if (num >= itemBag->count)
      break;

    double baseHeight = basey + WindowHeightInch * 0.455 - height * (i + 1);
    char _nameBuf[99];
    int type = itemBag->items[num].type;
    int arg = itemBag->items[num].arg;
    if (type == IKey) {
      sprintf(_nameBuf, "%s %d", itemsData[type].name, arg);
    } else if (type == ITM) {
      sprintf(_nameBuf, "%s %s", itemsData[type].name, movedex[arg].name);
    } else {
      strcpy(_nameBuf, itemsData[type].name);
    }

    int val = button(GenUIID(i), basex + Window43Gap * 0.05,
                     baseHeight + WindowHeightInch * 0.003, Window43Gap * 0.9,
                     height - WindowHeightInch * 0.006, NULL, belong);
    if (val != 0) {
      ret = num;
      retval = val;
    }

    drawBoxWithoutBorder(basex + Window43Gap * 0.06,
                         baseHeight + WindowHeightInch * 0.003,
                         Window43Gap * 0.88, height - WindowHeightInch * 0.006,
                         0, _nameBuf, 'L', "Black");
  }

  SetPointSize(_pointSize);

  if (ret >= 0) {
    return retval > 0 ? makeUseItemAttempt(ret) : makeRemoveItemAttempt(ret);
  }

  return 0;
}