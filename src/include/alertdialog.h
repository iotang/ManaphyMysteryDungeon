#pragma once

#include "appstate.h"

void setAlertDialog1(char *argv0);

void setAlertDialog2(char *argv0, char *argv1);

void setAlertDialog3(char *argv0, char *argv1, char *argv2);

void setAlertDialog4(char *argv0, char *argv1, char *argv2, char *argv3);

void drawAlertDialog();

void uiAlertDialogGetMouse(int x, int y, int button, int event);

extern AppState AlertDialog;

void gotoAlertDialog();