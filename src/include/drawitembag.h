#pragma once

#include "itembag.h"

#define ItemPerPage (8)
#define ItemBagLastPage (-2)
#define ItemBagNextPage (-3)

int drawItemBag(ItemBag *itemBag, double basex, double basey, int belong);