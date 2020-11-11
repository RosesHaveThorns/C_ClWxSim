#include <stdio.h>
#include <windows.h>

#include <graph.h>
#include <ui_utils.h>

#include <config.h>

// File ID: 02
// ID is used in error msgs (first 2 digits of code)

// Private func defintions
void drawTicks(HDC hdc, int origin[], int axis_width, int axis_height, int wld_width, int wld_height);
void drawAxis(HDC hdc, int origin[], int graph_left, int graph_top, int graph_right, int graph_bottom);
void drawPressure(HDC hdc, int origin[], float pressure_array[128][128], int wld_width, int wld_height, int cell_width, int cell_height);

// Main graph draw function
void DrawGraph(HWND hwnd, int x_padding, int graph_top){
  PAINTSTRUCT ps;
  RECT rect;

  float eg_pressure_array[128][128];
  int eg_world_width = 128;
  int eg_world_height = 128;

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

  int origin[2] = {graph_left + ((graph_right - graph_left) * 0.05),
                   graph_bottom - ((graph_bottom - graph_top) * 0.05)};

  int axis_height = (graph_bottom - graph_top)-((graph_bottom - graph_top) * 0.05 * 2);
  int cell_height = axis_height / eg_world_height;

  int axis_width = (graph_right - graph_left)-((graph_right - graph_left) * 0.05 * 2);
  int cell_width = axis_width / eg_world_width;

  // DRAW GRAPH

  HDC hdc = BeginPaint(hwnd, &ps);

    // draw background color
  DWORD colour = GetSysColor(COLOR_BTNFACE);
  SetBkColor(hdc, colour);

    // draw funcs
  drawAxis(hdc, origin, graph_left, graph_top, graph_right, graph_bottom);
  drawTicks(hdc, origin, axis_width, axis_height, eg_world_width, eg_world_height);
  drawPressure(hdc, origin, eg_pressure_array, eg_world_width, eg_world_height, cell_width, cell_height);

  // CLEAN UP

  EndPaint(hwnd, &ps);
}

// Draw functions:
void fillCell(HDC hdc, COLORREF colour, int top, int left, int bott, int right){
  for (int y = top; y < bott; y++){
    for (int x = left; x < right; x++){
      SetPixel(hdc, top, left, colour);
    }
  }
}

void drawPressure(HDC hdc, int origin[], float pressure_array[128][128], int wld_width, int wld_height, int cell_width, int cell_height) {
  // Fill each cell
  int top, right, bott, left;

  COLORREF col = RGB(255, 0, 0);

  for(int y = 0; y < wld_height; y++){
    for (int x = 0; x < wld_width; x++) {
      bott = origin[1] - (cell_height * y);
      left = origin[0] + (cell_width * x);
      right = left + cell_width;
      top = bott - cell_height;

      fillCell(hdc, col, x, y, bott, right);
    }
  }
}


void drawAxis(HDC hdc, int origin[], int graph_left, int graph_top, int graph_right, int graph_bottom) {
    // draw outline rect
  Rectangle(hdc, graph_left, graph_top, graph_right, graph_bottom);  // Background Rect

    // draw axis lines
  MoveToEx(hdc, origin[0], origin[1], NULL);
  LineTo(hdc, graph_right - ((graph_right - graph_left) * 0.05), origin[1]);

  MoveToEx(hdc, origin[0], origin[1], NULL);
  LineTo(hdc, origin[0], graph_top + ((graph_bottom - graph_top) * 0.05));
}

void drawTicks(HDC hdc, int origin[], int axis_width, int axis_height, int wld_width, int wld_height) {
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
