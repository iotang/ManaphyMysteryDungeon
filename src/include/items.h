#pragma once

#include <string.h>
#include "utils.h"
#include "imagesupport.h"

#define MaxItemNumber (8)      // 道具编号的最大值。
#define MaxItemNameLength (16) // 道具名字长度最大值。

// 道具编号定义。
#define INone (0)
#define IApple (1)
#define IGummi (2)
#define IOranBerry (3)
#define IOrenBerry (4)
#define ISitrusBerry (5)
#define IElixir (6)
#define IKey (7)
#define ITM (8)

#define MaxKeyID (4) // 钥匙的最大编号。钥匙的编号从 1 开始。

typedef struct ItemData {
  char name[MaxItemNameLength + 1];
  double dexp;
  double dbelly;
  int dhp, dmaxhp;
  int datk, ddef;
  int dmove; // 可以学会的招式编号。
  int keyid; // 作为钥匙的编号。
  int defaultArg;
  HBITMAP sprite;
} ItemData; // 道具数据。

typedef struct Item {
  int type;
  int arg;
} Item; // 具体的道具。

extern ItemData itemsData[MaxItemNumber + 1]; // 道具图鉴。

void initItems();

int cmpItems(const void *lhs, const void *rhs);