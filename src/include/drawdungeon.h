#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "dungeon.h"
#include "pokemon.h"
#include "solvemodel.h"

void getCellLocation(Dungeon *dungeon, int basex, int basey, double size,
                     double lx, double ly, int *_x, int *_y);

void drawDungeonPokemon(Dungeon *dungeon, int basex, int basey, double size,
                        Pokemon *pokemon);

void drawDungeon(Dungeon *dungeon, int basex, int basey, double size,
                 int showTag, DungeonSolution *solution, int enableSolution);

void drawDungeonHighlightCellAt(Dungeon *dungeon, int basex, int basey,
                                double size, int x, int y, double length,
                                int fill, char *color, double dx, double dy);

void drawDungeonHighlightCell(Dungeon *dungeon, int basex, int basey,
                              double size, double lx, double ly, double length,
                              int fill, char *color, double dx, double dy);

int drawDungeonEventEditOverride(LandEvent *landEvent, Item *item, double basex,
                                 double basey, char *bgcolor, int isEdit,
                                 int belong, int controlOverride,
                                 int *overrideItem, int *overrideLandEvent);

int drawDungeonEventEdit(LandEvent *landEvent, Item *item, double basex,
                         double basey, char *bgcolor, int isEdit, int belong);

bool notInAllMenu(double x, double y);