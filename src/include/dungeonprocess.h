#pragma once

#include "itembag.h"
#include "dungeon.h"
#include "pokemon.h"

void landEventCalc(Dungeon *dungeon, Pokemon *pokemon);

void pokemonStepOn(Dungeon *dungeon, Pokemon *pokemon, ItemBag *itemBag);

void pokemonOneItemStepOn(Dungeon *dungeon, Pokemon *pokemon, Item *item);