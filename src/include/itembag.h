#pragma once

#include <stdlib.h>

#include "utils.h"
#include "items.h"

#define MaxItemBagVolume (48)

typedef struct ItemBag {
  size_t count;
  Item items[MaxItemBagVolume];
  int currentPage;
} ItemBag;

void clearItemBag(ItemBag *itemBag) {
  itemBag->count = 0;
  itemBag->currentPage = 0;
}

int addIntoItemBag(ItemBag *itemBag, Item item) {
  if (itemBag->count >= MaxItemBagVolume)
    return 0;
  itemBag->items[itemBag->count++] = item;
  return 1;
}

int removeOutItemBag(ItemBag *itemBag, size_t index) {
  if (index < 0 || index >= itemBag->count)
    return 0;
  for (size_t i = index; i < itemBag->count; i++) {
    itemBag->items[i] = itemBag->items[i + 1];
  }
  itemBag->count--;
}

void sortItemBag(ItemBag *itemBag) {
  qsort(itemBag->items, itemBag->count, sizeof(Item), cmpItems);
}