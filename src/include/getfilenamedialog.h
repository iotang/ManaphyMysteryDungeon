#pragma once

#include "utils.h"

extern char dialogFileName[MaxFileNameLength + 1];

void initGetFileNameDialog(char *name);

int drawGetFileNameDialog();