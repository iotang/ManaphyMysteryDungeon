#pragma once

#include <string.h>
#include "utils.h"

#define MaxItemNumber (999)
#define MaxItemNameLength (16)

#define INone (0)
#define IApple (1)
#define IGummi (2)
#define IOranBerry (3)
#define ISitrusBerry (4)
#define IElixir (5)
#define IKey (6)
#define ITM (7)

typedef struct ItemData {
  char name[MaxItemNameLength + 1];
  double dexp;
  double dberry;
  int dhp, dmaxhp;
  int datk, ddef;
  int dmove;
  int keyid;
} ItemData;

typedef struct Item {
  int type;
  int arg;
} Item;

ItemData itemsData[MaxItemNumber + 1];

void initItems() {
  strcpy(itemsData[INone].name, "");
  itemsData[INone].dexp = 0;
  itemsData[INone].dberry = 0;
  itemsData[INone].dhp = 0;
  itemsData[INone].dmaxhp = 0;
  itemsData[INone].datk = 0;
  itemsData[INone].ddef = 0;
  itemsData[INone].dmove = 0;
  itemsData[INone].keyid = 0;

  strcpy(itemsData[IApple].name, "Apple");
  itemsData[IApple].dexp = 0;
  itemsData[IApple].dberry = 50;
  itemsData[IApple].dhp = 0;
  itemsData[IApple].dmaxhp = 0;
  itemsData[IApple].datk = 0;
  itemsData[IApple].ddef = 0;
  itemsData[IApple].dmove = 0;
  itemsData[IApple].keyid = 0;

  strcpy(itemsData[IGummi].name, "Gummi");
  itemsData[IGummi].dexp = 50;
  itemsData[IGummi].dberry = 10;
  itemsData[IGummi].dhp = 0;
  itemsData[IGummi].dmaxhp = 0;
  itemsData[IGummi].datk = 0;
  itemsData[IGummi].ddef = 0;
  itemsData[IGummi].dmove = 0;
  itemsData[IGummi].keyid = 0;

  strcpy(itemsData[IOranBerry].name, "Oran Berry");
  itemsData[IOranBerry].dexp = 0;
  itemsData[IOranBerry].dberry = 5;
  itemsData[IOranBerry].dhp = 100;
  itemsData[IOranBerry].dmaxhp = 0;
  itemsData[IOranBerry].datk = 0;
  itemsData[IOranBerry].ddef = 0;
  itemsData[IOranBerry].dmove = 0;
  itemsData[IOranBerry].keyid = 0;

  strcpy(itemsData[ISitrusBerry].name, "Sitrus Berry");
  itemsData[ISitrusBerry].dexp = 0;
  itemsData[ISitrusBerry].dberry = 5;
  itemsData[ISitrusBerry].dhp = 999999;
  itemsData[ISitrusBerry].dmaxhp = 5;
  itemsData[ISitrusBerry].datk = 0;
  itemsData[ISitrusBerry].ddef = 0;
  itemsData[ISitrusBerry].dmove = 0;
  itemsData[ISitrusBerry].keyid = 0;

  strcpy(itemsData[IElixir].name, "Elixir");
  itemsData[IElixir].dexp = 0;
  itemsData[IElixir].dberry = 5;
  itemsData[IElixir].dhp = 0;
  itemsData[IElixir].dmaxhp = 0;
  itemsData[IElixir].datk = 3;
  itemsData[IElixir].ddef = 3;
  itemsData[IElixir].dmove = 0;
  itemsData[IElixir].keyid = 0;

  strcpy(itemsData[IKey].name, "Key");
  itemsData[IKey].dexp = 0;
  itemsData[IKey].dberry = 0;
  itemsData[IKey].dhp = 0;
  itemsData[IKey].dmaxhp = 0;
  itemsData[IKey].datk = 0;
  itemsData[IKey].ddef = 0;
  itemsData[IKey].dmove = 0;
  itemsData[IKey].keyid = 1;

  strcpy(itemsData[ITM].name, "TM");
  itemsData[ITM].dexp = 0;
  itemsData[ITM].dberry = 0;
  itemsData[ITM].dhp = 0;
  itemsData[ITM].dmaxhp = 0;
  itemsData[ITM].datk = 0;
  itemsData[ITM].ddef = 0;
  itemsData[ITM].dmove = 1;
  itemsData[ITM].keyid = 0;
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