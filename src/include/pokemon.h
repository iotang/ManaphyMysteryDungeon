#pragma once

#include "utils.h"
#include "imagesupport.h"

#define MaxMoveCount (5)
#define MaxPokemonNameLength (16)
#define MaxMoveNameLength (16)

#define PokemonSpeciesNumber (1011)
#define NKate (0)
#define NManaphy (490)
#define NCresselia (488)
#define NRemoraid (223)
#define NSuicune (245)

#define MoveNumber (999)
#define MTackle (0)
#define MWaterGun (1)
#define MBubbleBeam (2)
#define MSurf (3)
#define MHydroPump (4)
#define MPsychic (5)

typedef struct Move {
  char name[MaxMoveNameLength + 1];
  int effect, pp;
} Move;

typedef enum Gender { Female, Male } Gender;

typedef struct Pokemon {
  Role role;

  char name[MaxPokemonNameLength + 1];
  Gender gender;
  int species;
  int lv;
  double exp;

  double belly, maxbelly;

  int hp, maxhp;
  int atk, def;

  int moveCount;
  int move[MaxMoveCount], pp[MaxMoveCount];

  int x, y;
  Direction direction;
} Pokemon;

typedef struct Pokedex {
  char name[MaxPokemonNameLength + 1];
  double hpBase;
  double atkBase, defBase;
  double hpGrowth;
  double atkGrowth, defGrowth;

  HBITMAP sprites[4];
} Pokedex;

extern Pokedex pokedex[PokemonSpeciesNumber];
extern Move movedex[MoveNumber];

void initPokedex();

void initMovedex();

double getBaseStat(int lv, double base, double growth);

double calcDamage(int lv, double effect, double atk, double def);

double calcExp(int aLv, int bLv);

void makePokemonStatBound(Pokemon *pokemon);

int updatePokemonStat(Pokemon *pokemon);

void spawnPokemon(Pokemon *pokemon, Role role, int species, int gender);