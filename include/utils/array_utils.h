#include "config.h"
#include <unistd.h>
#include <windows.h>

void set_all (float arr[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE], float value);
void zeros (float arr[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE]);
void fill_rnd_wldarray (float array[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE], float, float);
void fill_pattern_wldarray (float array[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE], float, float);