#include "config.h"

#include "world.h"
#include "ui_utils.h"

//#include <windows.h>
#include <stdio.h>


#define GRAPH_SIZE_RATIO 0.6
#define GRAPH_TICK_TOTAL 16

#define GRAPH_ORIGIN_OFFSET 0.1  // % of graph area a

void DrawGraph(HWND hwnd, int x_padding, int graph_top);
void DrawUpdateGraph(HWND hwnd, World *wld, int x_padding, int graph_top);
