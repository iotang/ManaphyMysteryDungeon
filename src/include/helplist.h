#pragma once

#define MaxHelpListLength (20)

void clearHelpList();

void addHelpEntry(char *fun, char *key);

void drawHelpList(double basex, double topy);