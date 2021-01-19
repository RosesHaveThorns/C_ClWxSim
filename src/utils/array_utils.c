#include <array_utils.h>

// set array to all zero values
void zeros (float[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE] arr) {
    set_all(arr, 0.0);
}

// set all values in an array to a single value
void set_all (float[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE] arr, float value) {
  for (int i = 0; i < MAX_WLDARRAY_SIZE; i++) {
    for (int j = 0; j < MAX_WLDARRAY_SIZE; j++) {
      arr[i][j] = value;
    }
  }
}
