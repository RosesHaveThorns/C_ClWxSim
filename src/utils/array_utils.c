#include "array_utils.h"

// set all values in an array to a single value
void set_all (float arr[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE], float value) {
  for (int i = 0; i < MAX_WLDARRAY_SIZE; i++) {
    for (int j = 0; j < MAX_WLDARRAY_SIZE; j++) {
      arr[i][j] = value;
    }
  }
}


// set array to all zero values
void zeros (float arr[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE]) {
    set_all(arr, 0.0f);
}
