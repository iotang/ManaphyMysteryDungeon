#pragma once

#include "utils.h"
#include "imagesupport.h"

#define MaxMoveCount (5) // 一个宝可梦最多能学会的招式数量。
#define MaxPokemonNameLength (16) // 宝可梦名字的最大长度。
#define MaxMoveNameLength (16)    // 招式名字的最大长度。

#define PokemonSpeciesNumber (1011) // 宝可梦图鉴的最大编号。
// 宝可梦图鉴编号。
#define NKate (0)
#define NManaphy (490)
#define NCresselia (488)
#define NRemoraid (223)
#define NSuicune (245)

#define MoveNumber (999) // 招式的最大编号。
// 招式的编号。
#define MTackle (0)
#define MWaterGun (1)
#define MBubbleBeam (2)
#define MSurf (3)
#define MHydroPump (4)
#define MPsychic (5)

typedef struct Move {
  char name[MaxMoveNameLength + 1];
  int effect, pp;
} Move; // 招式。

typedef enum Gender { Female, Male } Gender; // 性别或性别认同。

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

  int x, y;            // 在迷宫中的位置。
  Direction direction; // 朝向。
} Pokemon;             // 宝可梦的数据结构。

typedef struct Pokedex {
  char name[MaxPokemonNameLength + 1];
  double hpBase;
  double atkBase, defBase;
  double hpGrowth;
  double atkGrowth, defGrowth;

  HBITMAP sprites[4]; // 在迷宫中的形象。
  HBITMAP portrait;   // 头像。
} Pokedex; // 宝可梦的对应种族的基础数据，即宝可梦图鉴。

extern Pokedex pokedex[PokemonSpeciesNumber + 1]; // 宝可梦图鉴。
extern Move movedex[MoveNumber + 1];              // 招式图鉴。

void initPokedex();

void initMovedex();

double getBaseStat(int lv, double base, double growth);

double calcDamage(int lv, double effect, double atk, double def);

double calcExp(int aLv, int bLv);

void makePokemonStatBound(Pokemon *pokemon);

int updatePokemonStat(Pokemon *pokemon);

void spawnPokemon(Pokemon *pokemon, Role role, int species, int gender);