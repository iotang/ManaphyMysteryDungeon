#include "imagesupport.h"

HBITMAP dungeonSprites[4];

HBITMAP readBmpImage(char *fileName) {
  return LoadImage(NULL, (LPCSTR)fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}