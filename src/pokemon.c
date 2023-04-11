#include "pokemon.h"

#include <stdio.h>
#include <string.h>

Pokedex pokedex[PokemonSpeciesNumber];
Move movedex[MoveNumber];

void readPokemonSprites(int num) {
  static char _fileName[99];
  for (int i = 0; i < 4; i++) {
    sprintf(_fileName, "assets/sprites/%04d%c.bmp", num, "RULD"[i]);
    pokedex[num].sprites[i] = readBmpImage(_fileName);
  }
  sprintf(_fileName, "assets/portraits/%04d.bmp", num);
  pokedex[num].portrait = readBmpImage(_fileName);
}

void initPokedex() {
  strcpy(pokedex[NKate].name, "Kate");
  pokedex[NKate].hpBase = 200;
  pokedex[NKate].atkBase = 90;
  pokedex[NKate].defBase = 75;
  pokedex[NKate].hpGrowth = 12.5;
  pokedex[NKate].atkGrowth = 14.0;
  pokedex[NKate].defGrowth = 10.5;

  strcpy(pokedex[NManaphy].name, "Manaphy");
  pokedex[NManaphy].hpBase = 30;
  pokedex[NManaphy].atkBase = 6;
  pokedex[NManaphy].defBase = 5;
  pokedex[NManaphy].hpGrowth = 2.3;
  pokedex[NManaphy].atkGrowth = 1.8;
  pokedex[NManaphy].defGrowth = 1.7;
  readPokemonSprites(NManaphy);

  strcpy(pokedex[NRemoraid].name, "Remoraid");
  pokedex[NRemoraid].hpBase = 18;
  pokedex[NRemoraid].atkBase = 5;
  pokedex[NRemoraid].defBase = 3;
  pokedex[NRemoraid].hpGrowth = 1.2;
  pokedex[NRemoraid].atkGrowth = 1.5;
  pokedex[NRemoraid].defGrowth = 1.0;
  readPokemonSprites(NRemoraid);

  strcpy(pokedex[NSuicune].name, "Suicune");
  pokedex[NSuicune].hpBase = 30;
  pokedex[NSuicune].atkBase = 6;
  pokedex[NSuicune].defBase = 7;
  pokedex[NSuicune].hpGrowth = 2.3;
  pokedex[NSuicune].atkGrowth = 1.7;
  pokedex[NSuicune].defGrowth = 1.9;

  strcpy(pokedex[NCresselia].name, "Cresselia");
  pokedex[NCresselia].hpBase = 36;
  pokedex[NCresselia].atkBase = 5.5;
  pokedex[NCresselia].defBase = 7;
  pokedex[NCresselia].hpGrowth = 3.3;
  pokedex[NCresselia].atkGrowth = 1.6;
  pokedex[NCresselia].defGrowth = 2.1;
  readPokemonSprites(NCresselia);
};

void initMovedex() {
  strcpy(movedex[MTackle].name, "Tackle");
  movedex[MTackle].effect = 20;
  movedex[MTackle].pp = -1;

  strcpy(movedex[MWaterGun].name, "Water Gun");
  movedex[MWaterGun].effect = 40;
  movedex[MWaterGun].pp = 25;

  strcpy(movedex[MBubbleBeam].name, "Bubble Beam");
  movedex[MBubbleBeam].effect = 65;
  movedex[MBubbleBeam].pp = 20;

  strcpy(movedex[MSurf].name, "Surf");
  movedex[MSurf].effect = 80;
  movedex[MSurf].pp = 15;

  strcpy(movedex[MHydroPump].name, "Hydro Pump");
  movedex[MHydroPump].effect = 100;
  movedex[MHydroPump].pp = 5;

  strcpy(movedex[MPsychic].name, "Psychic");
  movedex[MPsychic].effect = 90;
  movedex[MPsychic].pp = 10;
}

double getBaseStat(int lv, double base, double growth) {
  return base + growth * (lv - 1);
}

double calcDamage(int lv, double effect, double atk, double def) {
  return (effect * (lv + 20.00) / 120.00) * atk / def;
}

double calcExp(int aLv, int bLv) {
  int base = 30 + bLv - aLv;
  if (base < 1)
    base = 1;
  return 20.00 * base / (aLv + 20);
}

void makePokemonStatBound(Pokemon *pokemon) {
  if (pokemon->maxhp < 1)
    pokemon->maxhp = 1;
  if (pokemon->hp < 0)
    pokemon->hp = 0;
  if (pokemon->hp > pokemon->maxhp)
    pokemon->hp = pokemon->maxhp;

  if (pokemon->maxbelly < 1)
    pokemon->maxbelly = 1;
  if (pokemon->belly < 0)
    pokemon->belly = 0;
  if (pokemon->belly > pokemon->maxbelly)
    pokemon->belly = pokemon->maxbelly;

  if (pokemon->atk < 1)
    pokemon->atk = 1;
  if (pokemon->def < 1)
    pokemon->def = 1;
}

int updatePokemonStat(Pokemon *pokemon) {
  makePokemonStatBound(pokemon);

  if (pokemon->exp < 100)
    return 0;

  pokemon->exp -= 100;
  pokemon->hp -= getBaseStat(pokemon->lv, pokedex[pokemon->species].hpBase,
                             pokedex[pokemon->species].hpGrowth);
  pokemon->maxhp -= getBaseStat(pokemon->lv, pokedex[pokemon->species].hpBase,
                                pokedex[pokemon->species].hpGrowth);
  pokemon->atk -= getBaseStat(pokemon->lv, pokedex[pokemon->species].atkBase,
                              pokedex[pokemon->species].atkGrowth);
  pokemon->def -= getBaseStat(pokemon->lv, pokedex[pokemon->species].defBase,
                              pokedex[pokemon->species].defGrowth);

  pokemon->lv++;

  pokemon->hp += getBaseStat(pokemon->lv, pokedex[pokemon->species].hpBase,
                             pokedex[pokemon->species].hpGrowth);
  pokemon->maxhp += getBaseStat(pokemon->lv, pokedex[pokemon->species].hpBase,
                                pokedex[pokemon->species].hpGrowth);
  pokemon->atk += getBaseStat(pokemon->lv, pokedex[pokemon->species].atkBase,
                              pokedex[pokemon->species].atkGrowth);
  pokemon->def += getBaseStat(pokemon->lv, pokedex[pokemon->species].defBase,
                              pokedex[pokemon->species].defGrowth);

  makePokemonStatBound(pokemon);

  return 1;
}

void spawnPokemon(Pokemon *pokemon, Role role, int species, int gender) {
  pokemon->role = role;
  strcpy(pokemon->name, pokedex[species].name);
  pokemon->gender = gender;
  pokemon->species = species;
  pokemon->lv = 1;
  pokemon->exp = 0;
  pokemon->hp = pokemon->maxhp = getBaseStat(
      pokemon->lv, pokedex[species].hpBase, pokedex[species].hpGrowth);
  pokemon->atk = getBaseStat(pokemon->lv, pokedex[species].atkBase,
                             pokedex[species].atkGrowth);
  pokemon->def = getBaseStat(pokemon->lv, pokedex[species].defBase,
                             pokedex[species].defGrowth);
  memset(pokemon->move, 0, sizeof(pokemon->move));
  memset(pokemon->pp, 0, sizeof(pokemon->pp));
  pokemon->moveCount = 1;
  pokemon->move[0] = MTackle;
  pokemon->pp[0] = movedex[MTackle].pp;

  pokemon->belly = pokemon->maxbelly = 100.00;

  if (species == NManaphy || species == NRemoraid) {
    pokemon->moveCount = 2;
    pokemon->move[1] = MWaterGun;
    pokemon->pp[1] = movedex[MWaterGun].pp;
  } else if (species == NCresselia) {
    pokemon->moveCount = 2;
    pokemon->move[1] = MPsychic;
    pokemon->pp[1] = movedex[MPsychic].pp;
  }

  pokemon->x = pokemon->y = -1;
  pokemon->direction = DOWN;
}