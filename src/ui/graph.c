#include "graph.h"

// File ID: 02
// ID is used in error msgs (first 2 digits of code)


// global vars
int origin[2];
int axis_height;
float ui_cell_height;
int axis_width;
float ui_cell_width;

HBITMAP graph_bmp;
int old_graph_width, old_graph_height;

// func definitions
void drawAxis(HDC hdc, World *wld);
void drawBg(HDC hdc, int graph_width, int graph_height);
void drawPressure(HDC, World *);
void drawWindVelQuiver(HDC hdc, World *wld);

// draw old graph bitmap, with rescaling
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
  calc_rect_pos(rect_width, rect_height, x_padding, GRAPH_SIZE_RATIO, &graph_left, &graph_top, &graph_right,
                &graph_bottom);

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
void UpdateDrawGraph(HWND hwnd, World *wld, int x_padding, int graph_top) {
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
  calc_rect_pos(rect_width, rect_height, x_padding, GRAPH_SIZE_RATIO, &graph_left, &graph_top, &graph_right,
                &graph_bottom);

  int graph_width = graph_right-graph_left;
  old_graph_width = graph_width;  // stored for scaling the graph blit
  int graph_height = graph_bottom-graph_top;
  old_graph_height = graph_height;  // stored for scaling the graph blit

  origin[0] = graph_width * GRAPH_ORIGIN_OFFSET;  // GRAPH_ORIGIN_OFFSET is the fraction of the graph width on either side of the axis
  origin[1] = graph_height * (1 - GRAPH_ORIGIN_OFFSET);

  axis_height = graph_height * (1- (GRAPH_ORIGIN_OFFSET * 2));
  // makes axis_height a multiple of origin_offset, as pxls are ints
  int y_tick_gap = axis_height / GRAPH_TICK_TOTAL;
  axis_height = y_tick_gap * GRAPH_TICK_TOTAL;

  axis_width = graph_width * (1- (GRAPH_ORIGIN_OFFSET * 2));
  // makes axis_width a multiple of origin_offset, as pxls are ints
  int x_tick_gap = axis_width / GRAPH_TICK_TOTAL;
  axis_width = x_tick_gap * GRAPH_TICK_TOTAL;

  ui_cell_height = (float)axis_height / wld->height;
  ui_cell_width = (float)axis_width / wld->width;

  // DRAW GRAPH

  HDC hdc = BeginPaint(hwnd, &ps);
  HDC hdc_bmp = CreateCompatibleDC(hdc);
  DeleteObject(graph_bmp);
  graph_bmp = CreateCompatibleBitmap(hdc, graph_width, graph_height);
  SelectObject(hdc_bmp, graph_bmp);

    // draw funcs
  drawBg(hdc_bmp, graph_width, graph_height);
  drawAxis(hdc_bmp, wld);
  drawPressure(hdc_bmp, wld);
  drawWindVelQuiver(hdc_bmp, wld);

  // Draw bitmap to screen then CLEAN UP
  BitBlt(hdc, graph_left, graph_top, graph_width, graph_height, hdc_bmp, 0, 0, SRCCOPY);
  DeleteDC(hdc_bmp);
  EndPaint(hwnd, &ps);
}

// DRAW FUNCTIONS:
void drawWindVelQuiver(HDC hdc, World *wld) {
  // Draw velocity arrow for each cell
  #ifdef DEBUG_OUT
    printf("D0201 Draw Quiver\n");
  #endif

  int centre_x, centre_y, end_x, end_y;
  for (int y = 0; y < wld->height; y+=QUIVER_CELLS_BTWN_EACH){
    for (int x = 0; x < wld->width; x+=QUIVER_CELLS_BTWN_EACH){
      centre_y = origin[1] - ((float)y*(ui_cell_height)) + (0.5 * ui_cell_height);
      centre_x = origin[0] + ((float)x*(ui_cell_width)) + (0.5 * ui_cell_width);

      // scale data so that a value of EXPECTED_WINDVEL_MAX gives a quiver length of MAX_QUIVER_SIZE
      end_y = centre_y - ((wld->wind_vel_y[x][y]) / EXPECTED_WINDVEL_MAX) * MAX_QUIVER_SIZE;
      end_x = centre_x + ((wld->wind_vel_x[x][y]) / EXPECTED_WINDVEL_MAX) * MAX_QUIVER_SIZE;

      COLORREF col = RGB(256,256,256);
      HPEN h_new_pen = CreatePen(PS_SOLID, 2, col);
      HPEN h_old_pen = SelectObject(hdc, h_new_pen);

      MoveToEx(hdc, centre_x,centre_y, NULL);
      LineTo(hdc, end_x, end_y);

      SelectObject(hdc, h_old_pen);
    }
  }
}

void drawPressure(HDC hdc, World *wld) {
  // Fill each cell
  #ifdef DEBUG_OUT
    printf("D0201 Draw Pressure\n");
  #endif

  RECT draw_rect;
  COLORREF rnd_colour;
  HBRUSH col_brush;
  for(int y = 0; y < wld->height; y++) {
    for (int x = 0; x < wld->width; x++) {
      draw_rect.bottom = origin[1] - ((float)y*(ui_cell_height));
      draw_rect.left = origin[0] + ((float)x*(ui_cell_width));
      draw_rect.right = origin[0] + ((float)(x+1)*(ui_cell_width));
      draw_rect.top = origin[1] - ((float)(y+1)*(ui_cell_height));

      // scaled data val between 0 and 256 for colour representation
      float val_single_rgb = ((wld->pressure[x][y] - EXPECTED_PRESSURE_MIN) / (EXPECTED_PRESSURE_MAX - EXPECTED_PRESSURE_MIN)) * 256;

      // draw data color representation
      rnd_colour = RGB(val_single_rgb, 0, 256-val_single_rgb);
      col_brush = SelectObject(hdc, CreateSolidBrush(rnd_colour));
      FillRect(hdc, &draw_rect, col_brush);
      DeleteObject(col_brush);
    }
  }
}


void drawBg(HDC hdc, int graph_width, int graph_height) {
    // draw outline rect
  Rectangle(hdc, 0, 0, graph_width, graph_height);  // Background Rect
}

void drawAxis(HDC hdc, World *wld) {
  // draw axis lines
  MoveToEx(hdc, origin[0], origin[1], NULL);
  LineTo(hdc, origin[0] + axis_width, origin[1]);

  MoveToEx(hdc, origin[0], origin[1], NULL);
  LineTo(hdc, origin[0], origin[1]-axis_height);
    // select tick text font
  HFONT h_font = CreateFont(15, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0, 0, 0, 0, 0, "Georgia");
  HFONT h_old_font = SelectObject(hdc, h_font);

    // draw y axis ticks
  int tick_y = 0;
  for (int i = 0; i <= GRAPH_TICK_TOTAL; i++){
    tick_y = origin[1] - (axis_height / GRAPH_TICK_TOTAL * i);

    int tick_num_int = wld->height / GRAPH_TICK_TOTAL * i;

    char tick_num[get_int_digits(tick_num_int)];
    for (int j = 0; j < sizeof(tick_num); j++) { // must set to empty, otherwise it uses old vals in empty spaces
      tick_num[j] = ' ';
    }

    LPCSTR tick_num_const[sizeof(tick_num)];
    itoa((tick_num_int), tick_num, 10);   // conv int (base 10) to str
    *tick_num_const = tick_num;

    TextOut(hdc, origin[0]-50, tick_y-10, *tick_num_const,  sizeof(*tick_num_const));

    MoveToEx(hdc, origin[0], tick_y, NULL);
    LineTo(hdc, origin[0] - 10, tick_y);
  }

    // draw x axis ticks
  int tick_x = 0;
  for (int i = 0; i <= GRAPH_TICK_TOTAL; i++){
    tick_x = origin[0] + ((axis_width / GRAPH_TICK_TOTAL) * i);

    int tick_num_int = (wld->width / GRAPH_TICK_TOTAL) * i;

    char tick_num[5] = {' ',' ',' ',' ',' '}; // must set to empty, otherwise it uses old vals in empty spaces
    LPCSTR tick_num_const[5];
    itoa(tick_num_int, tick_num, 10);   // conv int (base 10) to str
    *tick_num_const = tick_num;

    TextOut(hdc, tick_x-10, origin[1]+10, *tick_num_const, 5);

    MoveToEx(hdc, tick_x, origin[1], NULL);
    LineTo(hdc, tick_x, origin[1] + 10);
  }

  // clean up
  SelectObject(hdc, h_old_font);
  DeleteObject(h_font);
}