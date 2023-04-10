#pragma once

#include "appstate.h"
#include "utils.h"

void setConfirmDialog1(voidFn value, char *argv0);

void setConfirmDialog2(voidFn value, char *argv0, char *argv1);

void setConfirmDialog3(voidFn value, char *argv0, char *argv1, char *argv2);

void setConfirmDialog4(voidFn value, char *argv0, char *argv1, char *argv2,
                       char *argv3);

void drawConfirmDialog();

extern AppState ConfirmDialog;

void gotoConfirmDialog();