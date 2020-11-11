#include <windows.h>

void DrawGraph(HWND, int, int);
void drawTicks(HDC hdc, int origin[], int axis_width, int axis_height, int wld_width, int wld_height);

#define GRAPH_SIZE_RATIO 0.6
#define GRAPH_TICK_TOTAL 16
