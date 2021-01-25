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

// generate random data for testing
void fill_rnd_wldarray (float array[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE], float max, float min) {
  srand(GetTickCount());
  float val, scale;

  for (int i = 0; i < MAX_WLDARRAY_SIZE; i++) {
    for (int j = 0; j < MAX_WLDARRAY_SIZE; j++) {

      scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
      val = min + scale * ( max - min );      /* [min, max] */
      array[i][j] = val;
    }
  }
}

void fill_pattern_wldarray (float array[MAX_WLDARRAY_SIZE][MAX_WLDARRAY_SIZE], float max, float min) {
  float val = min+1;

  for (int j = 0; j < MAX_WLDARRAY_SIZE; j++) {
    for (int i = 0; i < MAX_WLDARRAY_SIZE; i++) {
      array[i][j] = (float)val;
      val++;
      if (val >= max) {
        val = min+1;
      }
    }
  }
}