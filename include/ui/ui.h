#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawGraph(HWND hwnd);
void CreateMenuBar(HWND);
void CreateButtons(HWND);

// IDM = Menus, IDB = Buttons, IDCB = Checkboxes
#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3
#define IDM_SIM_SETUP 4
#define IDM_SIM_START 5
#define IDM_SIM_RESUME 6
#define IDM_SIM_PAUSE 7

#define IDB_UPDATEVIEW 8

#define IDCB_AUTOUPDATEVIEW 9
