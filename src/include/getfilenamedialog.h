#pragma once

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "utils.h"

char dialogFileName[MaxFileNameLength + 1];

void initGetFileNameDialog(char *name) {
  if (strlen(name) > MaxFileNameLength) {
    name[MaxFileNameLength + 1] = 0;
  }
  strcpy(dialogFileName, name);
}

int drawGetFileNameDialog() {
  double xlen = WindowWidthInch / 3;
  double ylen = WindowHeightInch / 3;
  double xstart = (WindowWidthInch - xlen) / 2;
  double ystart = (WindowHeightInch - ylen) / 2;
  SetPenColor("White");
  drawRectangle(xstart, ystart, xlen, ylen, 1);
  SetPenColor("Blue");
  drawRectangle(xstart, ystart, xlen, ylen, 0);

  SetPointSize(16);
  SetPenColor("Blue");
  MovePen(xstart + xlen * 0.1, ystart + ylen * 0.8);
  DrawTextString("Input File Name:");

  SetPenColor("Blue");
  double fontHeight = GetFontHeight();
  double h = fontHeight * 1.5;
  drawRectangle(1.1 * WindowWidthInch / 3, WindowHeightInch / 2, 0.8 * xlen, h,
                0);

  return textbox(GenUIID(0), 1.1 * WindowWidthInch / 3, WindowHeightInch / 2,
                 0.8 * xlen, h, dialogFileName, MaxFileNameLength);
}
