#include "imagesupport.h"

HBITMAP readBmpImage(char *fileName) {
  return LoadImage(NULL, (LPCSTR)fileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}