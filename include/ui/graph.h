#include "config.h"

#include <stdio.h>
#include <windows.h>
#include "ui_utils.h"
#include "world.h"


#define GRAPH_SIZE_RATIO 0.6
#define GRAPH_TICK_TOTAL 16

#define GRAPH_ORIGIN_OFFSET 0.1  // % of graph area a

#define MAX_QUIVER_SIZE 100
#define QUIVER_CELLS_BTWN_EACH 10


void DrawGraph(HWND hwnd, int x_padding, int graph_top);
void UpdateDrawGraph(HWND hwnd, World *wld, int x_padding, int graph_top);
