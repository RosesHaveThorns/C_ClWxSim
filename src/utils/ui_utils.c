#include <stdio.h>
#include <windows.h>

#include <ui_utils.h>

#include <config.h>

void calc_rect_pos(int parent_width, int parent_height, int padding, float size_ratio, int *left, int *top, int *right, int *bottom) {
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

// returns the amount of digits in an int, 0 if negative, max 10 digits
int get_int_digits(int x) {
  if (x >= 1000000000) return 10;
  if (x >= 100000000)  return 9;
  if (x >= 10000000)   return 8;
  if (x >= 1000000)    return 7;
  if (x >= 100000)     return 6;
  if (x >= 10000)      return 5;
  if (x >= 1000)       return 4;
  if (x >= 100)        return 3;
  if (x >= 10)         return 2;
  if (x >= 0)          return 1;
  return 0;
}