#include <stdio.h>
#include <windows.h>

#include <graph.h>

#include <config.h>

void DrawGraph(HWND hwnd){
  PAINTSTRUCT ps;
  RECT rect;

  GetClientRect(hwnd, &rect);

  if(rect.bottom == 0){
    return;
  }

  HDC hdc = BeginPaint(hwnd, &ps);

  for (int i=0; i<1000; i++){
    int x = rand() % rect.right;
    int y = rand() % rect.bottom;
    SetPixel(hdc, x, y, RGB(255, 0, 0));
  }

  EndPaint(hwnd, &ps);
}
