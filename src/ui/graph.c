#include <stdio.h>
#include <windows.h>

#include <graph.h>
#include <ui_utils.h>

#include <config.h>

// File ID: 02
// ID is used in error msgs (first 2 digits of code)

void DrawGraph(HWND hwnd, int x_padding, int graph_top){
  PAINTSTRUCT ps;
  RECT rect;

  float eg_wind_array[128][128];
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

    // draw outline rect
  Rectangle(hdc, graph_left, graph_top, graph_right, graph_bottom);  // Background Rect

    // draw axis lines
  MoveToEx(hdc, origin[0], origin[1], NULL);
  LineTo(hdc, graph_right - ((graph_right - graph_left) * 0.05), origin[1]);

  MoveToEx(hdc, origin[0], origin[1], NULL);
  LineTo(hdc, origin[0], graph_top + ((graph_bottom - graph_top) * 0.05));


    // select tick text font
  HFONT hFont = CreateFont(15, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0, 0, 0, 0, 0, "Georgia");
  HFONT hOldFont = SelectObject(hdc, hFont);

    // draw y axis ticks
  int tick_y;
  for (int i = 0; i <= GRAPH_TICK_TOTAL; i++){
    tick_y = origin[1] - ((axis_height / GRAPH_TICK_TOTAL) * i);

    MoveToEx(hdc, origin[0], tick_y, NULL);
    LineTo(hdc, origin[0] - 10, tick_y);

    char tick_num[4];
    LPCSTR tick_num_const[4];
    itoa((eg_world_height/GRAPH_TICK_TOTAL*i), tick_num, 10);   // conv int (base 10) to str
    *tick_num_const = tick_num;

    TextOut(hdc, origin[0]-30, tick_y-10, *tick_num_const,  4);
  }

    // draw x axis ticks
  int tick_x;
  for (int i = 0; i <= GRAPH_TICK_TOTAL; i++){
    tick_x = origin[0] + ((axis_width / GRAPH_TICK_TOTAL) * i);

    MoveToEx(hdc, tick_x, origin[1], NULL);
    LineTo(hdc, tick_x, origin[1] + 10);

    char tick_num[4];
    LPCSTR tick_num_const[4];
    itoa((eg_world_width/GRAPH_TICK_TOTAL*i), tick_num, 10);   // conv int (base 10) to str
    *tick_num_const = tick_num;

    TextOut(hdc, tick_x-10, origin[1]+10, *tick_num_const,  4);
  }


  // CLEAN UP
  SelectObject(hdc, hOldFont);
  DeleteObject(hFont);

  EndPaint(hwnd, &ps);
}
