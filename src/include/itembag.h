#pragma once

#include "items.h"

#define MaxItemBagVolume (200000)

typedef struct ItemBag {
  size_t count;
  Item items[MaxItemBagVolume];
  int currentPage;
} ItemBag;

void clearItemBag(ItemBag *itemBag);

int addIntoItemBag(ItemBag *itemBag, Item item);

int removeOutItemBag(ItemBag *itemBag, size_t index);

int getKeyInItemBag(ItemBag *itemBag);

void sortItemBag(ItemBag *itemBag);