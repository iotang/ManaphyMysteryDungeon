#pragma once

#define MaxHelpListLength (20) // 帮助栏最长条目数。

void clearHelpList();

void addHelpEntry(char *fun, char *key);

void drawHelpList(double basex, double topy);