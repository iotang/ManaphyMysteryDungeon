#pragma once

#include "dungeon.h"

extern Dungeon
    currentDungeon; // 当前文件的迷宫。只能和编辑页面之间进行交互，只在读取文件和保存文件的时候更改。
extern int isDungeonOpened; // 当前是否打开了迷宫。
extern int
    isDungeonGameOver; // 游玩页面中，游玩是否结束（走到终点或者在迷宫中倒下）。
extern int
    isDungeonSimTerminated; // 自动执行页面中，执行是否结束（克雷色利亚走到终点或者在迷宫中倒下）。
extern int modifiedSinceLastSave; // 自从上一次保存后，是否对迷宫进行了更改。