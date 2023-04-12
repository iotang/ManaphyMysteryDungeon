#include "utils.h"

int go[5][2] = {
    {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {0, 0}}; // 朝向与坐标的对应换算。

int pick(int a, int b) { return ((a) >> (b)) & 1; }
int shl(int a) { return 1 << a; }