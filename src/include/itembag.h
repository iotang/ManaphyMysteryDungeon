#pragma once

#include "items.h"

#define MaxItemBagVolume (2000) // 最多持有的道具数量。

typedef struct ItemBag {
  size_t count;                 // 道具数量。
  Item items[MaxItemBagVolume]; // 道具数据。
  int currentPage;              // 现在在道具栏的哪一页。
} ItemBag;

void clearItemBag(ItemBag *itemBag);

int addIntoItemBag(ItemBag *itemBag, Item item);

int removeOutItemBag(ItemBag *itemBag, size_t index);

int getKeyInItemBag(ItemBag *itemBag);

void sortItemBag(ItemBag *itemBag);