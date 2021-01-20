#include "graph.h"

// File ID: 02
// ID is used in error msgs (first 2 digits of code)

// global vars
int origin[2];
int axis_height;
int cell_height;
int axis_width;
int cell_width;

HBITMAP graph_bmp;
int old_graph_width, old_graph_height;

// func definitions
void drawTicks(HDC, World *);
void drawAxis(HDC, int, int);
void drawPressure(HDC, World *);
void drawQuiver(HDC, World *, float);

// draw old graph bitmap, with rescaleing
void DrawGraph(HWND hwnd, int x_padding, int graph_top) {
  PAINTSTRUCT ps;
  RECT rect;

  GetClientRect(hwnd, &rect);

  if(rect.bottom == 0){
    return;
  }

  // GET WINDOW SIZE
  int rect_width = rect.right - rect.left;
  int rect_height = rect.bottom - rect.top;

  // CALC GRAPH POS VARS
  int graph_left, graph_right, graph_bottom;
  CalcRectPos(rect_width, rect_height, x_padding, GRAPH_SIZE_RATIO, &graph_left, &graph_top, &graph_right, &graph_bottom);

  int graph_width = graph_right-graph_left;
  int graph_height = graph_bottom-graph_top;

  // DRAW GRAPH

  HDC hdc = BeginPaint(hwnd, &ps);
  HDC hdc_bmp = CreateCompatibleDC(hdc);
  SelectObject(hdc_bmp, graph_bmp);

  // Draw bitmap to screen then CLEAN UP
  StretchBlt(hdc, graph_left, graph_top, graph_width, graph_height, hdc_bmp, 0, 0, old_graph_width, old_graph_height, SRCCOPY);

  DeleteDC(hdc_bmp);
  EndPaint(hwnd, &ps);
}

// update graph bitmap and draw
void DrawUpdateGraph(HWND hwnd, World *wld, int x_padding, int graph_top) {
  PAINTSTRUCT ps;
  RECT rect;

  #ifdef DEBUG_OUT
    printf("D0202 Update Graph\n");
  #endif

  GetClientRect(hwnd, &rect);

  if(rect.bottom == 0){
    return;
  }

  // GET WINDOW SIZE
  int rect_width = rect.right - rect.left;
  int rect_height = rect.bottom - rect.top;

  // CALC GRAPH POS VARS
  int graph_left, graph_right, graph_bottom;
  CalcRectPos(rect_width, rect_height, x_padding, GRAPH_SIZE_RATIO, &graph_left, &graph_top, &graph_right, &graph_bottom);

  int graph_width = graph_right-graph_left;
  old_graph_width = graph_width;
  int graph_height = graph_bottom-graph_top;
  old_graph_height = graph_height;

  origin[0] = graph_width * GRAPH_ORIGIN_OFFSET;
  origin[1] = graph_height * (1 - GRAPH_ORIGIN_OFFSET);

  axis_height = graph_height * (1 - (GRAPH_ORIGIN_OFFSET * 2));
  cell_height = axis_height / wld->height;

  axis_width = graph_width * (1 - (GRAPH_ORIGIN_OFFSET * 2));
  cell_width = axis_width / wld->width;

  // DRAW GRAPH

  HDC hdc = BeginPaint(hwnd, &ps);
  HDC hdc_bmp = CreateCompatibleDC(hdc);
  DeleteObject(graph_bmp);
  graph_bmp = CreateCompatibleBitmap(hdc, graph_width, graph_height);
  SelectObject(hdc_bmp, graph_bmp);

    // draw funcs
  drawAxis(hdc_bmp, graph_width, graph_height);
  drawTicks(hdc_bmp, wld);
  drawPressure(hdc_bmp, wld);
  drawQuiver(hdc_bmp, wld, 1);

  // Draw bitmap to screen then CLEAN UP
  BitBlt(hdc, graph_left, graph_top, graph_width, graph_height, hdc_bmp, 0, 0, SRCCOPY);
  DeleteDC(hdc_bmp);
  EndPaint(hwnd, &ps);
}

// DRAW FUNCTIONS:
void drawQuiver(HDC hdc, World *wld, float scale) {
  // Draw velocity arrow for each cell
  #ifdef DEBUG_OUT
    printf("D0201 Draw Quiver\n");
  #endif

  int centre_x, centre_y, end_x, end_y;
  for (int y = 0; y < wld->height; y++){
    for (int x = 0; x < wld->width; x++){
      centre_y = origin[1] - (cell_height * y) + (0.5 * cell_height);
      centre_x = origin[0] + (cell_width * x) + (0.5 * cell_width);

      end_y = centre_y - (wld->wind_vel_y[x][y] * scale);
      end_x = centre_x + (wld->wind_vel_y[x][y] * scale);

      MoveToEx(hdc, centre_x,centre_y, NULL);
      LineTo(hdc, end_x, end_y);
    }
  }
}

void drawPressure(HDC hdc, World *wld) {
  // Fill each cell
  #ifdef DEBUG_OUT
    printf("D0201 Draw Pressure\n");
  #endif

  int bott, left, right, top;

  for(int y = 0; y < wld->height; y++) {
    for (int x = 0; x < wld->width; x++) {
      bott = origin[1] - (cell_height * y);
      left = origin[0] + (cell_width * x);
      right = left + cell_width;
      top = bott - cell_height;

      Rectangle(hdc, left, top, right, bott);
    }
  }
}


void drawAxis(HDC hdc, int graph_width, int graph_height) {
    // draw outline rect
  Rectangle(hdc, 0, 0, graph_width, graph_height);  // Background Rect

    // draw axis lines
  MoveToEx(hdc, origin[0], origin[1], NULL);
  LineTo(hdc, graph_width - (graph_width * GRAPH_ORIGIN_OFFSET), origin[1]);

  MoveToEx(hdc, origin[0], origin[1], NULL);
  LineTo(hdc, origin[0], 0 + (graph_height * GRAPH_ORIGIN_OFFSET));
}

void drawTicks(HDC hdc, World *wld) {
    // select tick text font
  HFONT hFont = CreateFont(15, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0, 0, 0, 0, 0, "Georgia");
  HFONT hOldFont = SelectObject(hdc, hFont);

    // draw y axis ticks
  int tick_y = 0;
  for (int i = 0; i <= GRAPH_TICK_TOTAL; i++){
    tick_y = origin[1] - ((axis_height / GRAPH_TICK_TOTAL) * i);

    MoveToEx(hdc, origin[0], tick_y, NULL);
    LineTo(hdc, origin[0] - 10, tick_y);

    char tick_num[4] = {' ',' ',' ',' '}; // must set to empty, otherwise it uses old vals in empty spaces
    LPCSTR tick_num_const[4];
    itoa((wld->height / GRAPH_TICK_TOTAL * i), tick_num, 10);   // conv int (base 10) to str
    *tick_num_const = tick_num;

    TextOut(hdc, origin[0]-30, tick_y-10, *tick_num_const,  sizeof(*tick_num_const));
  }

    // draw x axis ticks
  int tick_x = 0;
  for (int i = 0; i <= GRAPH_TICK_TOTAL; i++){
    tick_x = origin[0] + ((axis_width / GRAPH_TICK_TOTAL) * i);

    MoveToEx(hdc, tick_x, origin[1], NULL);
    LineTo(hdc, tick_x, origin[1] + 10);

    char tick_num[4] = {' ',' ',' ',' '}; // must set to empty, otherwise it uses old vals in empty spaces
    LPCSTR tick_num_const[4];
    itoa((wld->width/GRAPH_TICK_TOTAL*i), tick_num, 10);   // conv int (base 10) to str
    *tick_num_const = tick_num;

    TextOut(hdc, tick_x-10, origin[1]+10, *tick_num_const,  4);
  }

  // clean up
  SelectObject(hdc, hOldFont);
  DeleteObject(hFont);
}
