#include <stdio.h>
#include <windows.h>

#include <graph.h>
#include <ui_utils.h>

#include <config.h>

// File ID: 02
// ID is used in error msgs (first 2 digits of code)

// Private func defintions
void drawTicks(HDC hdc);
void drawAxis(HDC hdc, int graph_left, int graph_top, int graph_right, int graph_bottom);
void drawPressure(HDC hdc);

// global vars
float eg_pressure_array[128][128];
int wld_width = 128;
int wld_height = 128;

int origin[2];
int axis_height;
int cell_height;
int axis_width;
int cell_width;

// Main graph draw function
void DrawGraph(HWND hwnd, int x_padding, int graph_top){
  PAINTSTRUCT ps;
  RECT rect;

  #ifdef DEBUG_OUT
    printf("D0202 Draw Graph\n");
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

  origin[0] = graph_left + ((graph_right - graph_left) * 0.05);
  origin[1] = graph_bottom - ((graph_bottom - graph_top) * 0.05);

  axis_height = (graph_bottom - graph_top)-((graph_bottom - graph_top) * 0.05 * 2);
  cell_height = axis_height / wld_height;

  axis_width = (graph_right - graph_left)-((graph_right - graph_left) * 0.05 * 2);
  cell_width = axis_width / wld_width;

  // DRAW GRAPH
  int graph_width = graph_right-graph_left;
  int graph_height = graph_bottom-graph_top;

  HDC hdc = BeginPaint(hwnd, &ps);
  HDC hdc_bmp = CreateCompatibleDC(hdc);
  HBITMAP graph_bmp = CreateCompatibleBitmap(hdc, graph_width, graph_height);
  SelectObject(hdc_bmp, graph_bmp);

    // draw background color
  DWORD colour = GetSysColor(COLOR_BTNFACE);
  SetBkColor(hdc_bmp, colour);

    // draw funcs
  drawAxis(hdc_bmp, graph_left, graph_top, graph_right, graph_bottom);
  drawTicks(hdc_bmp);
  drawPressure(hdc_bmp);

  // Draw bitmap to screen then CLEAN UP
  BitBlt(hdc, graph_left, graph_bottom, graph_width, graph_height, hdc_bmp, 0, 0, SRCCOPY);
  DeleteObject(graph_bmp);
  EndPaint(hwnd, &ps);
}

// Draw functions:

void fillRow(HDC hdc, int y) {
  int bott, left, right, top;
  for (int x = 0; x < wld_width; x++) {
    bott = origin[1] - (cell_height * y);
    left = origin[0] + (cell_width * x);
    right = left + cell_width;
    top = bott - cell_height;

    Rectangle(hdc, left, top, right, bott);
    }
}

void drawPressure(HDC hdc) {
  // Fill each cell

  #ifdef DEBUG_OUT
    printf("D0201 Draw Pressure\n");
  #endif

  for(int y = 0; y < wld_height; y++) {
    fillRow(hdc, y);
  }

}


void drawAxis(HDC hdc, int graph_left, int graph_top, int graph_right, int graph_bottom) {
    // draw outline rect
  Rectangle(hdc, graph_left, graph_top, graph_right, graph_bottom);  // Background Rect

    // draw axis lines
  MoveToEx(hdc, origin[0], origin[1], NULL);
  LineTo(hdc, graph_right - ((graph_right - graph_left) * 0.05), origin[1]);

  MoveToEx(hdc, origin[0], origin[1], NULL);
  LineTo(hdc, origin[0], graph_top + ((graph_bottom - graph_top) * 0.05));
}

void drawTicks(HDC hdc) {
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
    itoa((wld_height/GRAPH_TICK_TOTAL*i), tick_num, 10);   // conv int (base 10) to str
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
    itoa((wld_width/GRAPH_TICK_TOTAL*i), tick_num, 10);   // conv int (base 10) to str
    *tick_num_const = tick_num;

    TextOut(hdc, tick_x-10, origin[1]+10, *tick_num_const,  4);
  }

  // clean up
  SelectObject(hdc, hOldFont);
  DeleteObject(hFont);
}
