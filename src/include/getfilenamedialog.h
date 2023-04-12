#pragma once

#include "utils.h"

extern char dialogFileName[MaxFileNameLength + 1]; // 读取到的文件名。

void initGetFileNameDialog(char *name);

int drawGetFileNameDialog();