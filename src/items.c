#include "items.h"

#include <stdio.h>
#include <string.h>
#include "utils.h"

ItemData itemsData[MaxItemNumber + 1];

void readItemSprites(int num) {
  static char _fileName[99];
  sprintf(_fileName, "assets/items/%s.bmp", itemsData[num].name);
  itemsData[num].sprite = readBmpImage(_fileName);
}

void initItems() {
  strcpy(itemsData[INone].name, "No Item");
  itemsData[INone].dexp = 0;
  itemsData[INone].dbelly = 0;
  itemsData[INone].dhp = 0;
  itemsData[INone].dmaxhp = 0;
  itemsData[INone].datk = 0;
  itemsData[INone].ddef = 0;
  itemsData[INone].dmove = 0;
  itemsData[INone].keyid = 0;
  itemsData[INone].defaultArg = 0;

  strcpy(itemsData[IApple].name, "Apple");
  itemsData[IApple].dexp = 0;
  itemsData[IApple].dbelly = 50;
  itemsData[IApple].dhp = 0;
  itemsData[IApple].dmaxhp = 0;
  itemsData[IApple].datk = 0;
  itemsData[IApple].ddef = 0;
  itemsData[IApple].dmove = 0;
  itemsData[IApple].keyid = 0;
  itemsData[IApple].defaultArg = 0;
  readItemSprites(IApple);

  strcpy(itemsData[IGummi].name, "Gummi");
  itemsData[IGummi].dexp = 50;
  itemsData[IGummi].dbelly = 10;
  itemsData[IGummi].dhp = 0;
  itemsData[IGummi].dmaxhp = 0;
  itemsData[IGummi].datk = 0;
  itemsData[IGummi].ddef = 0;
  itemsData[IGummi].dmove = 0;
  itemsData[IGummi].keyid = 0;
  itemsData[IGummi].defaultArg = 0;
  readItemSprites(IGummi);

  strcpy(itemsData[IOranBerry].name, "Oran Berry");
  itemsData[IOranBerry].dexp = 0;
  itemsData[IOranBerry].dbelly = 5;
  itemsData[IOranBerry].dhp = 100;
  itemsData[IOranBerry].dmaxhp = 0;
  itemsData[IOranBerry].datk = 0;
  itemsData[IOranBerry].ddef = 0;
  itemsData[IOranBerry].dmove = 0;
  itemsData[IOranBerry].keyid = 0;
  itemsData[IOranBerry].defaultArg = 0;
  readItemSprites(IOranBerry);

  strcpy(itemsData[ISitrusBerry].name, "Sitrus Berry");
  itemsData[ISitrusBerry].dexp = 0;
  itemsData[ISitrusBerry].dbelly = 5;
  itemsData[ISitrusBerry].dhp = 999999;
  itemsData[ISitrusBerry].dmaxhp = 5;
  itemsData[ISitrusBerry].datk = 0;
  itemsData[ISitrusBerry].ddef = 0;
  itemsData[ISitrusBerry].dmove = 0;
  itemsData[ISitrusBerry].keyid = 0;
  itemsData[ISitrusBerry].defaultArg = 0;
  readItemSprites(ISitrusBerry);

  strcpy(itemsData[IElixir].name, "Elixir");
  itemsData[IElixir].dexp = 0;
  itemsData[IElixir].dbelly = 5;
  itemsData[IElixir].dhp = 0;
  itemsData[IElixir].dmaxhp = 0;
  itemsData[IElixir].datk = 3;
  itemsData[IElixir].ddef = 3;
  itemsData[IElixir].dmove = 0;
  itemsData[IElixir].keyid = 0;
  itemsData[IElixir].defaultArg = 0;
  readItemSprites(IElixir);

  strcpy(itemsData[IKey].name, "Key");
  itemsData[IKey].dexp = 0;
  itemsData[IKey].dbelly = 0;
  itemsData[IKey].dhp = 0;
  itemsData[IKey].dmaxhp = 0;
  itemsData[IKey].datk = 0;
  itemsData[IKey].ddef = 0;
  itemsData[IKey].dmove = 0;
  itemsData[IKey].keyid = 1;
  itemsData[IKey].defaultArg = 1;
  readItemSprites(IKey);

  strcpy(itemsData[ITM].name, "TM");
  itemsData[ITM].dexp = 0;
  itemsData[ITM].dbelly = 0;
  itemsData[ITM].dhp = 0;
  itemsData[ITM].dmaxhp = 0;
  itemsData[ITM].datk = 0;
  itemsData[ITM].ddef = 0;
  itemsData[ITM].dmove = 1;
  itemsData[ITM].keyid = 0;
  itemsData[ITM].defaultArg = 1;
  readItemSprites(ITM);

  strcpy(itemsData[IOrenBerry].name, "Oren Berry");
  itemsData[IOrenBerry].dexp = 0;
  itemsData[IOrenBerry].dbelly = 5;
  itemsData[IOrenBerry].dhp = -10;
  itemsData[IOrenBerry].dmaxhp = 0;
  itemsData[IOrenBerry].datk = 0;
  itemsData[IOrenBerry].ddef = 0;
  itemsData[IOrenBerry].dmove = 0;
  itemsData[IOrenBerry].keyid = 0;
  itemsData[IOrenBerry].defaultArg = 0;
  readItemSprites(IOrenBerry);
}

int cmpItems(const void *lhs, const void *rhs) {
  int typel = (*(Item *)lhs).type;
  int typer = (*(Item *)rhs).type;
  int argl = (*(Item *)lhs).arg;
  int argr = (*(Item *)rhs).arg;
  if (typel != typer)
    return typel - typer;
  return argl - argr;
}