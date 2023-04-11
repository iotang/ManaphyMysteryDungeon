#pragma once

#include <string.h>
#include "utils.h"
#include "imagesupport.h"

#define MaxItemNumber (8)
#define MaxItemNameLength (16)

#define INone (0)
#define IApple (1)
#define IGummi (2)
#define IOranBerry (3)
#define IOrenBerry (4)
#define ISitrusBerry (5)
#define IElixir (6)
#define IKey (7)
#define ITM (8)

#define MaxKeyID (4)

typedef struct ItemData {
  char name[MaxItemNameLength + 1];
  double dexp;
  double dbelly;
  int dhp, dmaxhp;
  int datk, ddef;
  int dmove;
  int keyid;
  int defaultArg;
  HBITMAP sprite;
} ItemData;

typedef struct Item {
  int type;
  int arg;
} Item;

extern ItemData itemsData[MaxItemNumber + 1];

void initItems();

int cmpItems(const void *lhs, const void *rhs);