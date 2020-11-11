#include <stdio.h>
#include <windows.h>

#include <ui_utils.h>

#include <config.h>

void CalcRectPos(int parent_width, int parent_height, int padding, float size_ratio, int *left, int *top, int *right, int *bottom) {
  *left = padding;
  *right = parent_width - padding;
  *bottom = ((*right - *left) * size_ratio) + *top;

  if (*bottom > parent_height - padding){   // if too tall to fit on screen, lock height and width, change posx
    *bottom = parent_height - padding;

    int width = (*bottom - *top) / size_ratio;
    *left = (parent_width - width) / 2;
    *right = width + *left;
  }
}
