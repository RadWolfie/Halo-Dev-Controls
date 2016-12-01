/********************************************************************************
	 -- Halo Dev Controls
    Copyright © 2011 Jesus7Freak

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************************
	File:    WinMain.cpp
	Project: Halo Dev Controls
	Author:  Jesus7Freak
	Date:    11/22/2011
	Game:    Halo and Halo Custom Edition
	Version: all
*********************************************************************************/
#include "WinMain.h"

HWND hControls[NUM_OF_CONTROLS] = { NULL };
HMODULE hUxTheme = NULL;

RWMemory *Halo_Process = NULL;
HMODULE hHDC = NULL;
bool exiting = false,
   dev_enabled = false,
   console_enabled = false,
   alarm_on = false,
   rpgb6_2_running = false,
   Locked = false,
   Nuked = false;
      
BOOL theme_active = FALSE;

wchar_t *szWindowClass = L"WinMain",
   *szTitle = L"Halo Dev Controls  -  Jesus7Freak",
   *szHaloCE = L"Halo CE:",
   *szHaloPC = L"Halo PC:",
   *szHaloCE_exe = L"haloce.exe",
   *szHaloPC_exe = L"halo.exe",
   *szOn = L"On",
   *szOff = L"Off",
   *szMap = L"Map:",
   *szMainMenu = L"Main Menu",
   *szClient = L"Client",
   *szHost = L"Hosting",
   *szDisableDev = L"Disable Dev",
   *szEnableDev = L"Enable Dev",
   *szDisableConsole = L"Disable Console",
   *szEnableConsole = L"Enable Console",
   *Locations_File_Name = L"locations.bin",
   *Settings_File_Name = L"shortcuts.bin",
   *Dll_Name = L"HDC.drv",
   *main_module_name = NULL,
   *empty_str = L"",
   *setting_names[3] = { {L"Day"}, {L"Rain"}, {L"Night"} },
   *szAlarmOn = L"Alarm On",
   *szAlarmOff = L"Alarm Off",
   *szSet = L"Set",
   *szBaseLockAct = L"Activate",
   *szBaseLockLocked = L"Locked",
   *szSeconds = L"seconds",
   *szHaloFire = L"Fire",
   *szHaloCoolDown = L"Cool Down",
   szBuffer[SZ_BUFFER_SIZE] = { 0 };

int MouseOverControlID = 0;

DWORD scan_size = 0,
   Current_Map_address = NULL,
   Cheats_address = NULL,
   ServerType_address = NULL,
   Device_Groups_Header_ptr_address = NULL,
   HS_Global_Header_ptr_address = NULL,
   dll_addrs_ptr = NULL,
   Dev_enabled_address = NULL,
   Console_enabled_address = NULL,
   ShowHud_ptr_address = NULL,
   LetterBox_ptr_address = NULL,
   //RiderEjection_address = NULL;
   cse_set_video_func_address = NULL,
   cinematic_ptr = NULL;

game_types running_gt = not_running;
server_type running_sv_t = main_menu;

HaloCE_lib::DATA_HEADER Device_Groups_Header = {0},
   HS_Global_Header = {0};

bool (__stdcall *pMV_chkBx_CheckedChanged)(int number) = NULL;

HINSTANCE hInst;
bool MenuActive = false;
HMENU ExtrasMenu;
HFONT hFont = NULL;

BOOL (__stdcall *pIsAppThemed)();
HRESULT (__stdcall *pDrawThemeParentBackground)(HWND hwnd, HDC hdc, RECT *prc);

BOOL InitApp();
LRESULT CALLBACK WndMnProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//WndMnProc handlers
int OnCreate(HWND hwnd, LPCREATESTRUCT WinData);
void OnPaint(HWND hwnd);
void OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT *lpDrawItem);
BOOL OnNotify(HWND hwnd, int idCtrl, LPNMHDR pnmh);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
HBRUSH OnCtlColorStatic(HWND hwnd, HDC hdc, HWND hwndChild, int type);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
   //only run 1 instance
   //HANDLE lhMutex = CreateMutex(NULL, FALSE, L"Local\\Halo Dev Controls");
   //if (lhMutex && GetLastError () != ERROR_ALREADY_EXISTS) return 0;
   HWND hWin = FindWindowW(szWindowClass, szTitle);
   if (hWin)
   {
      SetForegroundWindow(hWin);
      return 0;
   }
   
   
   hInst = hInstance;
   if (!InitApp()) return 0;

   //Main Window
   HWND hMainWin = CreateWindowEx(
      WS_EX_TOOLWINDOW |
      WS_EX_CONTROLPARENT |
      WS_EX_APPWINDOW,
      szWindowClass,
      szTitle,
      WS_CAPTION |
      WS_SYSMENU,
      CW_USEDEFAULT, CW_USEDEFAULT,
      335, 175,
      NULL,
      NULL,
      hInstance,
      NULL
   );
   
   if (!hMainWin)
      return 2;
   
   hUxTheme = GetModuleHandle(L"uxtheme.dll");
   if (hUxTheme)
   {
      pIsAppThemed = (BOOL (__stdcall *)())GetProcAddress(hUxTheme, "IsAppThemed");
      pDrawThemeParentBackground = (HRESULT (__stdcall *)(HWND hwnd, HDC hdc, RECT *prc))GetProcAddress(hUxTheme, "DrawThemeParentBackground");
   }
   
   ShowWindow(hMainWin, nCmdShow);
   UpdateWindow(hMainWin);
   
   HANDLE hHDCThread = CreateThread(NULL, 0, HDCThread, (LPVOID)hMainWin, 0, NULL);
   
   // Main message loop:
   MSG msg;
   while (GetMessage(&msg, NULL, 0, 0))
   {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
   
   //tell hHDCThread to exit loop
   exiting = true;
   
   if (Halo_Process)
   {
      if (hHDC) Halo_Process->UnloadDLL(hHDC, false);
      
      delete Halo_Process;
      Halo_Process = NULL;
   }
   
   if (hHDCThread)
   {
      WaitForSingleObject(hHDCThread, 1000);
      CloseHandle(hHDCThread);
   }
   
   return 7;//because I can
}

BOOL InitApp()
{
   HBITMAP BmpBrush = LoadBitmapW(hInst, MAKEINTRESOURCE(IDB_MW_BG));
   HBRUSH brPattern = CreatePatternBrush(BmpBrush);
   DeleteObject(BmpBrush);
   
   WNDCLASSEX wcex;
   wcex.cbSize = sizeof(WNDCLASSEX);

   wcex.style          = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc    = WndMnProc;
   wcex.cbClsExtra     = 0;
   wcex.cbWndExtra     = 0;
   
   wcex.hInstance      = hInst;
   wcex.hIcon          = LoadIcon(hInst, MAKEINTRESOURCE(IDI_WINMAIN));
   wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground  = brPattern;
   wcex.lpszMenuName   = NULL;
   wcex.lpszClassName  = szWindowClass;
   wcex.hIconSm        = NULL;
   
   if (!RegisterClassEx(&wcex)) return FALSE;
   
   INITCOMMONCONTROLSEX InitCtrlEx;
   InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCtrlEx.dwICC  = ICC_STANDARD_CLASSES;
   InitCommonControlsEx(&InitCtrlEx); /* In case we use a common control */
   
   CMDsLib::GetSKeysFromFile(Settings_File_Name);
   
   //see if dll is in same folder
	HANDLE hFile;
	if ((hFile = CreateFileW(
      Dll_Name, 
      GENERIC_READ, 
      FILE_SHARE_READ, 
      NULL,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      NULL)) == INVALID_HANDLE_VALUE)
	{
	   int dll_name_length = 0; do dll_name_length++; while(Dll_Name[dll_name_length]);
	   wchar_t *caption = new wchar_t[0xB + dll_name_length]();
   	
	   for (int i = 0; i < dll_name_length; i++)
	      caption[i] = Dll_Name[i];
   	
	   caption[dll_name_length + 0] = L' ';
	   caption[dll_name_length + 1] = L'n';
	   caption[dll_name_length + 2] = L'o';
	   caption[dll_name_length + 3] = L't';
	   caption[dll_name_length + 4] = L' ';
	   caption[dll_name_length + 5] = L'f';
	   caption[dll_name_length + 6] = L'o';
	   caption[dll_name_length + 7] = L'u';
	   caption[dll_name_length + 8] = L'n';
	   caption[dll_name_length + 9] = L'd';
	
	   ::MessageBoxW(
         NULL, 
         L"Commands and key-shortcuts will not work\nNeeds to be in the same folder\nas Halo Dev Controls", 
         caption, 
         MB_OK | MB_ICONWARNING | MB_TASKMODAL
         );
         
      delete[] caption;
   }
   else
      CloseHandle(hFile);
      
   return TRUE;
}

LRESULT CALLBACK WndMnProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   LRESULT ret_val = 0;

   switch (message)
   {
      case WM_CREATE:
         ret_val = HANDLE_WM_CREATE(hwnd, wParam, lParam, OnCreate);
         break;
         
      case WM_DESTROY:
         if (ExtrasMenu) DestroyMenu(ExtrasMenu);
         if (hFont) DeleteObject(hFont);
         PostQuitMessage(0);
         break;
         
      case WM_PAINT:
         ret_val = HANDLE_WM_PAINT(hwnd, wParam, lParam, OnPaint);
         break;

      case WM_DRAWITEM:
         ret_val = HANDLE_WM_DRAWITEM(hwnd, wParam, lParam, OnDrawItem);
         break;
         
      case WM_NOTIFY:
         ret_val = HANDLE_WM_NOTIFY(hwnd, wParam, lParam, OnNotify);
         break;
         
      case WM_COMMAND:
         ret_val = HANDLE_WM_COMMAND(hwnd, wParam, lParam, OnCommand);
         break;
         
      case WM_CTLCOLORBTN:
         ret_val = (LRESULT)GetStockBrush(NULL_BRUSH);
         break;
         
      case WM_CTLCOLORSTATIC:
         ret_val = HANDLE_WM_CTLCOLORSTATIC(hwnd, wParam, lParam, OnCtlColorStatic);
         break;

      default:
         ret_val = DefWindowProc(hwnd, message, wParam, lParam);
         break;
   }

   return ret_val;
}

int OnCreate(HWND hwnd, LPCREATESTRUCT WinData)
{
   //halo type label
   hControls[HHALO_TYPE] = CreateWindowEx(
      NULL,
      L"STATIC",
      szHaloCE,
      WS_CHILD | 
      WS_VISIBLE |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      SS_LEFT,
      2, 1,
      55, 16,
      hwnd,
      (HMENU)IDC_HALO_TYPE,
      WinData->hInstance,
      NULL
   );
   
   //halo status label
   hControls[HHALO_STATUS] = CreateWindowEx(
      NULL,
      L"STATIC",
      szOff,
      WS_CHILD | 
      WS_VISIBLE |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      SS_LEFT,
      57, 1,
      25, 16,
      hwnd,
      (HMENU)IDC_HALO_STATUS,
      WinData->hInstance,
      NULL
   );
   
   //map label
   hControls[HMAP_LBL] = CreateWindowEx(
      NULL,
      L"STATIC",
      empty_str,
      WS_CHILD | 
      WS_VISIBLE |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      SS_LEFT,
      81, 1,
      30, 16,
      hwnd,
      (HMENU)IDC_MAP_LBL,
      WinData->hInstance,
      NULL
   );
   
   //map status label
   hControls[HMAP_STATUS] = CreateWindowEx(
      NULL,
      L"STATIC",
      empty_str,
      WS_CHILD | 
      WS_VISIBLE |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      SS_LEFT,
      114, 1,
      140, 16,
      hwnd,
      (HMENU)IDC_MAP_STATUS,
      WinData->hInstance,
      NULL
   );
	
   ExtrasMenu = CreatePopupMenu();

   /*MENUITEMINFO extras;
   extras.cbSize = sizeof(MENUITEMINFO);
   extras.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STRING;// | MIIM_SUBMENU;
   extras.fType = MFT_STRING | MFT_RIGHTJUSTIFY;
   //extras.fState = MFS_ENABLED;
   extras.wID = IDM_EXTRAS;
   //extras.hSubMenu = ExtrasMenu;
   //extras.hbmpChecked = NULL;
   //extras.hbmpUnchecked = NULL;
   //extras.dwItemData = 0;
   extras.dwTypeData = L"Extras";
   extras.cch = 7;
   //extras.hbmpItem = NULL;
   
   //InsertMenuItem(hMenu, 0, TRUE, &extras);*/
   AppendMenu(ExtrasMenu, MF_STRING, IDM_DEV_COMMANDS ,L"Dev Commands");
   AppendMenu(ExtrasMenu, MF_STRING, IDM_RPGBETA_COMMANDS ,L"RPG_Beta6_2 Commands");
   AppendMenu(ExtrasMenu, MF_STRING, IDM_PLAYER_COMMANDS ,L"Player Commands");
   AppendMenu(ExtrasMenu, MF_STRING, IDM_TELEPORT_LOCATIONS, L"Teleport Locations");
   AppendMenu(ExtrasMenu, MF_SEPARATOR ,0 , L"");
   AppendMenu(ExtrasMenu, MF_STRING, IDM_ABOUT, L"About Halo Dev Controls");
   
   HWND hMenuBtn = CreateWindowEx(
      NULL,
      L"BUTTON",
      L"Extras",
      WS_CHILD | 
      WS_VISIBLE |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      BS_OWNERDRAW,
      255, 0,
      74, 19,
      hwnd,
      (HMENU)IDC_MENU_BUTTON,
      WinData->hInstance,
      NULL
   );
   hControls[HMENU_BUTTON] = hMenuBtn;
   SetWindowSubclass(hMenuBtn, MouseOverProc, IDS_MOUSEOVER, IDC_MENU_BUTTON);
   
   //server status label
   hControls[HSERVER_STATUS] = CreateWindowEx(
      NULL,
      L"STATIC",
      szMainMenu,
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      SS_OWNERDRAW,
      119, 26,
      70, 17,
      hwnd,
      (HMENU)IDC_SERVER_STATUS,
      WinData->hInstance,
      NULL
   );
   
   //Dev Button
   HWND hDev = CreateWindowEx(
      NULL,
      L"BUTTON",
      szEnableDev,
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      BS_OWNERDRAW,
      16, 26,
      89, 25,
      hwnd,
      (HMENU)IDC_DEV,
      WinData->hInstance,
      NULL
   );
   hControls[HDEV] = hDev;
   SetWindowSubclass(hDev, MouseOverProc, IDS_MOUSEOVER, IDC_DEV);
   
   //Console Button
   HWND hConsole = CreateWindowEx(
      NULL,
      L"BUTTON",
      szEnableConsole,
      WS_CHILD | 
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      BS_OWNERDRAW,
      3, 26,
      115, 25,
      hwnd,
      (HMENU)IDC_CONSOLE,
      WinData->hInstance,
      NULL
   );
   hControls[HCONSOLE] = hConsole;
   SetWindowSubclass(hConsole, MouseOverProc, IDS_MOUSEOVER, IDC_CONSOLE);
   
   //Deathless Checkbox
   hControls[HDEATHLESS] = CreateWindowEx(
      NULL,
      L"BUTTON",
      L"Deathless",
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      BS_AUTOCHECKBOX,
      3, 55,
      81, 19,
      hwnd,
      (HMENU)IDC_DEATHLESS,
      WinData->hInstance,
      NULL
   );
   
   //Infinite Ammo Checkbox
   hControls[HINFAMMO] = CreateWindowEx(
      NULL,
      L"BUTTON",
      L"Infinite Ammo",
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      BS_AUTOCHECKBOX,
      3, 74,
      101, 19,
      hwnd,
      (HMENU)IDC_INFAMMO,
      WinData->hInstance,
      NULL
   );
   
   //Show HUD Checkbox
   hControls[HSHOWHUD] = CreateWindowEx(
      NULL,
      L"BUTTON",
      L"Show HUD",
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      BS_AUTOCHECKBOX,
      3, 93,
      87, 19,
      hwnd,
      (HMENU)IDC_SHOWHUD,
      WinData->hInstance,
      NULL
   );
   
   //Letter Box Checkbox
   hControls[HLETTERBOX] = CreateWindowEx(
      NULL,
      L"BUTTON",
      L"Letter Box",
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      BS_AUTOCHECKBOX,
      3, 112,
      81, 19,
      hwnd,
      (HMENU)IDC_LETTERBOX,
      WinData->hInstance,
      NULL
   );
   
   //Rider Ejection Checkbox
   hControls[HMHUD] = CreateWindowEx(
      NULL,
      L"BUTTON",
      L"Marines HUD",//L"Rider Ejection",
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      BS_AUTO3STATE,
      3, 131,
      103, 19,
      hwnd,
      (HMENU)IDC_MHUD,//IDC_EJECTION,
      WinData->hInstance,
      NULL
   );
   
   //RPG_Beta6_2 controls
   //Setting Combo Box
   hControls[HSETTING] = CreateWindowEx(
      NULL,
      L"COMBOBOX",
      NULL,
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_OVERLAPPED |
      WS_CLIPCHILDREN |
      CBS_DROPDOWN |
      CBS_HASSTRINGS,
      193, 23,
      56, 23,
      hwnd,
      (HMENU)IDC_SETTING,
      WinData->hInstance,
      NULL
   );
   
   for (int i = 0; i < 3; i++)
      SendMessage(hControls[HSETTING], CB_ADDSTRING, 0, (LPARAM)setting_names[i]);

   //Alarm Button
   HWND hAlarm = CreateWindowEx(
      NULL,
      L"BUTTON",
      szAlarmOn,
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      BS_OWNERDRAW, 
      251, 23,
      75, 23,
      hwnd,
      (HMENU)IDC_ALARM,
      WinData->hInstance,
      NULL
   );
   hControls[HALARM] = hAlarm;
   SetWindowSubclass(hAlarm, MouseOverProc, IDS_MOUSEOVER, IDC_ALARM);
   
   //base lockdown label
   hControls[HBLD_LBL] = CreateWindowEx(
      NULL,
      L"STATIC",
      L"Base Lock Down",
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      SS_OWNERDRAW,
      162, 48,
      102, 15,
      hwnd,
      (HMENU)IDC_BLD_LBL,
      WinData->hInstance,
      NULL
   );
   
   //base lockdown textbox
   hControls[HBLD_TXTBX] = CreateWindowEx(
      WS_EX_CLIENTEDGE,
      L"EDIT",
      szSeconds,
      WS_CHILD |
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      ES_LEFT |
      //ES_AUTOVSCROLL |
      //ES_AUTOHSCROLL |
      ES_NUMBER,
      116, 66,
      57, 21,
      hwnd,
      (HMENU)IDC_BLD_TXTBX,
      WinData->hInstance,
      NULL
   );
   Edit_LimitText(hControls[HBLD_TXTBX], 3);
   
   //base lockdown set Button
   HWND hBld_set = CreateWindowEx(
      NULL,
      L"BUTTON",
      szSet,
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      BS_OWNERDRAW, 
      178, 64,
      62, 25,
      hwnd,
      (HMENU)IDC_BLD_SET_BTN,
      WinData->hInstance,
      NULL
   );
   hControls[HBLD_SET_BTN] = hBld_set;
   SetWindowSubclass(hBld_set, MouseOverProc, IDS_MOUSEOVER, IDC_BLD_SET_BTN);
   
   //base lockdown activate Button
   HWND hBld_act = CreateWindowEx(
      NULL,
      L"BUTTON",
      szBaseLockAct,
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      BS_OWNERDRAW, 
      246, 64,
      80, 25,
      hwnd,
      (HMENU)IDC_BLD_ACT_BTN,
      WinData->hInstance,
      NULL
   );
   hControls[HBLD_ACT_BTN] = hBld_act;
   SetWindowSubclass(hBld_act, MouseOverProc, IDS_MOUSEOVER, IDC_BLD_ACT_BTN);
   
   //halo label
   hControls[HHALO_LBL] = CreateWindowEx(
      NULL,
      L"STATIC",
      L"Halo",
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      SS_OWNERDRAW,
      197, 89,
      33, 15,
      hwnd,
      (HMENU)IDC_HALO_LBL,
      WinData->hInstance,
      NULL
   );
   
   //halo textbox
   hControls[HHALO_TXTBX] = CreateWindowEx(
      WS_EX_CLIENTEDGE,
      L"EDIT",
      szSeconds,
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      ES_LEFT |
      //ES_AUTOVSCROLL |
      //ES_AUTOHSCROLL |
      ES_NUMBER,
      116, 107,
      57, 21,
      hwnd,
      (HMENU)IDC_HALO_TXTBX,
      WinData->hInstance,
      NULL
   );
   Edit_LimitText(hControls[HHALO_TXTBX], 3);
   
   //halo set Button
   HWND hHalo_set = CreateWindowEx(
      NULL,
      L"BUTTON",
      szSet,
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      BS_OWNERDRAW, 
      178, 105,
      62, 25,
      hwnd,
      (HMENU)IDC_HALO_SET_BTN,
      WinData->hInstance,
      NULL
   );
   hControls[HHALO_SET_BTN] = hHalo_set;
   SetWindowSubclass(hHalo_set, MouseOverProc, IDS_MOUSEOVER, IDC_HALO_SET_BTN);
   
   //halo activate Button
   HWND hHalo_act = CreateWindowEx(
      NULL,
      L"BUTTON",
      szHaloFire,
      WS_CHILD | 
      WS_VISIBLE |
      WS_DISABLED |
      WS_CLIPSIBLINGS |
      WS_CLIPCHILDREN |
      BS_OWNERDRAW, 
      246, 105,
      80, 25,
      hwnd,
      (HMENU)IDC_HALO_ACT_BTN,
      WinData->hInstance,
      NULL
   );
   hControls[HHALO_ACT_BTN] = hHalo_act;
   SetWindowSubclass(hHalo_act, MouseOverProc, IDS_MOUSEOVER, IDC_HALO_ACT_BTN);
   
   //(HFONT)::GetStockObject(DEFAULT_GUI_FONT);
   hFont = CreateFontW(
      16, 0, 
      0, 
      0,
      FW_NORMAL, 
      FALSE, 
      FALSE,
      FALSE, 
      DEFAULT_CHARSET, 
      OUT_DEFAULT_PRECIS, 
      CLIP_DEFAULT_PRECIS, 
      DEFAULT_QUALITY, 
      DEFAULT_PITCH, 
      L"Microsoft Sans Serif"
      );
         
   for (int i = 0; i < NUM_OF_CONTROLS; i++)
      SetWindowFont(hControls[i], hFont, TRUE);

   //display first item in checkbox
   SendMessage(hControls[HSETTING], CB_SETCURSEL, 0, 0);
  
   return TRUE;
}

void OnPaint(HWND hwnd)
{
   PAINTSTRUCT Ps;
   HDC hdc = BeginPaint(hwnd, &Ps);
   HPEN hWhitePen = GetStockPen(WHITE_PEN);
   HGDIOBJ hOldObj = SelectObject(hdc, hWhitePen);
   
   MoveToEx(hdc, 0, 19, NULL);
   LineTo(hdc, 335, 19);
   
   SelectObject(hdc, hOldObj);
   EndPaint(hwnd, &Ps);
}

void OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT *lpDrawItem)
{
   UINT iState = lpDrawItem->itemState;
   BOOL bIsPressed =	(iState & ODS_SELECTED);
   BOOL bIsFocused  = (iState & ODS_FOCUS);
   BOOL bIsDisabled = (iState & ODS_DISABLED);
   //BOOL bIsMouseOver = (lpDrawItem->itemState & ODS_HOTLIGHT);
   BOOL bDrawOutLine = (lpDrawItem->CtlType == ODT_BUTTON);

   HBRUSH brBackground;
   COLORREF forecolor = 0;
   
   int Ctrl_ID = lpDrawItem->CtlID;
   bool highlight = bIsPressed || MouseOverControlID == Ctrl_ID;
   switch (Ctrl_ID)
   {
      case IDC_MENU_BUTTON:
         if (highlight || MenuActive) brBackground = CreateSolidBrush(RGB(51,153,255));
         else brBackground = GetStockBrush(NULL_BRUSH);

         forecolor = RGB(255,255,255);
         bDrawOutLine = FALSE;
         break;

      case IDC_SERVER_STATUS:
         brBackground = GetStockBrush(NULL_BRUSH);
         bIsDisabled = running_gt == not_running;
         forecolor = running_sv_t == host ? RGB(0,128,0) : RGB(255,0,0);
         break;
         
      case IDC_DEV:
         if (highlight) brBackground = CreateSolidBrush(RGB(229,229,229));
         else brBackground = GetStockBrush(NULL_BRUSH);

         forecolor = dev_enabled ? RGB(0,128,0) : RGB(255,0,0);
         break;

      case IDC_CONSOLE:
         if (highlight) brBackground = CreateSolidBrush(RGB(229,229,229));
         else brBackground = GetStockBrush(NULL_BRUSH);
         
         forecolor = console_enabled ? RGB(0,128,0) : RGB(255,0,0);
         break;
                  
      case IDC_ALARM:
         if (highlight) brBackground = CreateSolidBrush(RGB(229,229,229));
         else brBackground = GetStockBrush(NULL_BRUSH);
         
         forecolor = alarm_on ? RGB(51,153,255) : RGB(255,0,0);
         break;
         
      case IDC_BLD_LBL:
      case IDC_HALO_LBL:
         brBackground = GetStockBrush(NULL_BRUSH);
         bIsDisabled = running_sv_t != host || !rpgb6_2_running;
         forecolor = 0x00FFFFFF;//RGB(255,255,255);
         break;
         
      case IDC_BLD_SET_BTN:
      case IDC_HALO_SET_BTN:
         if (highlight) brBackground = CreateSolidBrush(RGB(229,229,229));
         else brBackground = GetStockBrush(NULL_BRUSH);
         
         forecolor = RGB(51,153,255);
         break;
         
      case IDC_BLD_ACT_BTN:
         if (highlight) brBackground = CreateSolidBrush(RGB(229,229,229));
         else brBackground = GetStockBrush(NULL_BRUSH);
         
         forecolor = Locked ? RGB(255,0,0) : RGB(51,153,255);
         break;
         
      case IDC_HALO_ACT_BTN:
         if (highlight) brBackground = CreateSolidBrush(RGB(229,229,229));
         else brBackground = GetStockBrush(NULL_BRUSH);
         
         forecolor = Nuked ? RGB(255,0,0) : RGB(51,153,255);
         break;
   }
   
   HDC hdc = lpDrawItem->hDC;//optimizer does this
   FillRect(hdc, &(lpDrawItem->rcItem), brBackground);
   DeleteObject(brBackground);
   
   int pnWidthEx = 0;
	if (bIsFocused) pnWidthEx++;
	   
	if (bDrawOutLine)
	{
	   HPEN pnForeColor = CreatePen(PS_SOLID, 1 + pnWidthEx, forecolor);
      SelectObject(hdc, pnForeColor);
      Rectangle(hdc, lpDrawItem->rcItem.left + pnWidthEx, lpDrawItem->rcItem.top + pnWidthEx, lpDrawItem->rcItem.right, lpDrawItem->rcItem.bottom);
      DeleteObject(pnForeColor);
   }
   
   int str_len = GetWindowTextW(lpDrawItem->hwndItem, szBuffer, SZ_BUFFER_SIZE);
         
   SIZE dimensions = {0};
   GetTextExtentPoint32W(hdc, szBuffer, str_len, &dimensions);
   int xPos = (lpDrawItem->rcItem.right - dimensions.cx) / 2;
   int yPos = ((lpDrawItem->rcItem.bottom - dimensions.cy) / 2) + 1;
         
   SetBkMode(hdc, TRANSPARENT);
   SetTextColor(hdc, bIsDisabled ? GetSysColor(COLOR_GRAYTEXT) : forecolor);
   TextOut(hdc, xPos, yPos, szBuffer, str_len);
}

int OnNotify(HWND hwnd, int idCtrl, LPNMHDR pnmh)
{
   int ret_val = 0;
   
   if (pnmh->code == NM_CUSTOMDRAW)
   {
      LPNMCUSTOMDRAW lpcd = (LPNMCUSTOMDRAW)pnmh;
      if (lpcd->dwDrawStage == CDDS_PREPAINT)
      {
         switch (idCtrl)
         {
            case IDC_DEATHLESS:
            case IDC_INFAMMO:
            case IDC_SHOWHUD:
            case IDC_LETTERBOX:
            case IDC_MHUD://IDC_EJECTION:
            {
               int str_len = GetWindowTextW(lpcd->hdr.hwndFrom, szBuffer, SZ_BUFFER_SIZE);
               
               HDC hdc = lpcd->hdc;//optimizer does this
               SetBkMode(hdc, TRANSPARENT);
               SetTextColor(hdc, (lpcd->uItemState ^ CDIS_DISABLED) ? RGB(51,153,255) : GetSysColor(COLOR_GRAYTEXT));
               TextOut(hdc, 18, 2, szBuffer, str_len);
               
               ret_val = CDRF_SKIPDEFAULT;
               break;
            }
         }
      }
   }
   
   return ret_val;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
   switch (codeNotify)
   {
      case BN_CLICKED:
         switch(id)
         {
            case IDM_DEV_COMMANDS:
               if (DialogBoxParamA(hInst, MAKEINTRESOURCEA(IDD_CMDS), hwnd, CMDSDlgProc, (LPARAM)&CMDsLib::halo_commands))
               {
                  if (hHDC)
                  {
                     //update keys in dll too
                     Halo_Process->WriteMemArray<CMDsLib::CMD_SCKEYS>((LPVOID)
                        dll_addresses.halo_cmd_keys, 
                        CMDsLib::halo_cmd_keys, 
                        HALO_CMDS_SIZE);
                     
                     Halo_Process->WriteMem<BOOL>((LPVOID)dll_addresses.halo_sk_enabled, CMDsLib::halo_commands.Enable_Shrtcts);
                  }
                  CMDsLib::WriteSKeysToFile(Settings_File_Name);
               }
               break;
               
            case IDM_RPGBETA_COMMANDS:
               if (DialogBoxParamA(hInst, MAKEINTRESOURCEA(IDD_CMDS), hwnd, CMDSDlgProc, (LPARAM)&CMDsLib::rpgbeta6_2_commands))
               {
                  if (hHDC)
                  {
                     //update keys in dll too
                     Halo_Process->WriteMemArray<CMDsLib::CMD_SCKEYS>((LPVOID)
                        dll_addresses.rpgb62_cmd_keys, 
                        CMDsLib::rpg_beta6_2_cmd_keys, 
                        RPGB_CMDS_SIZE);
                     
                     Halo_Process->WriteMem<BOOL>((LPVOID)dll_addresses.rpg_beta62_sk_enabled, CMDsLib::rpgbeta6_2_commands.Enable_Shrtcts);
                  }
                  CMDsLib::WriteSKeysToFile(Settings_File_Name);
               }
               break;
               
            case IDM_PLAYER_COMMANDS:
               DialogBoxParamA(hInst, MAKEINTRESOURCEA(IDD_CMDS), hwnd, CMDSDlgProc, (LPARAM)&CMDsLib::player_commands);
               break;
            
            case IDM_TELEPORT_LOCATIONS:
            {
               std::vector<MAPS> maps_tele_sites;
               GetLocationsFromFile(Locations_File_Name, &maps_tele_sites);
   
               if (DialogBoxParamA(hInst, MAKEINTRESOURCEA(IDD_TELE_LOC), hwnd, TeleLocDlgProc, (LPARAM)&maps_tele_sites))
               {
                  WriteLocationsToFile(Locations_File_Name, &maps_tele_sites);
                  if (hHDC)
                     Halo_Process->WriteMem<bool>((LPVOID)(dll_addrs_ptr + sizeof(DLL_ADDRS)), true);
               }
               break;  
            }
            case IDM_ABOUT:
               DialogBoxParamA(hInst, MAKEINTRESOURCEA(IDD_ABOUT), hwnd, AboutDlgProc, (LPARAM)hInst);
               break;
               
            case IDC_MENU_BUTTON:
            {
               MenuActive = true;
               RECT rControl;
               GetWindowRect(hwndCtl, &rControl);
               TrackPopupMenu(ExtrasMenu, TPM_LEFTBUTTON, rControl.left, rControl.top + 19, 0, hwnd, NULL);
               
               MenuActive = false;
               //needed for a transparent background
               MapWindowRect(HWND_DESKTOP, hwnd, &rControl);
               InvalidateRect(hwnd, &rControl, TRUE);
               break;
            }
            case IDC_DEV:
            {
               bool Dev_enabled = Halo_Process->ReadMem<bool>((LPVOID)Dev_enabled_address);

               if (!Dev_enabled)
                  //enable console
                  Halo_Process->WriteMem<bool>((LPVOID)Console_enabled_address, true);

               Halo_Process->WriteMem<bool>((LPVOID)Dev_enabled_address, !Dev_enabled);
               break;
            }
            case IDC_CONSOLE:
            
               Halo_Process->WriteMem<bool>((LPVOID)Console_enabled_address, !Halo_Process->ReadMem<bool>((LPVOID)Console_enabled_address));
               break;
               
            case IDC_DEATHLESS:
               Halo_Process->WriteMem<BYTE>(
                  (LPVOID)(Cheats_address + HaloCE_lib::CheatsEx::Deathless_offset), 
                  (BYTE)Button_GetCheck(hControls[HDEATHLESS]));
               break;
            
            case IDC_INFAMMO:
               Halo_Process->WriteMem<BYTE>(
                  (LPVOID)(Cheats_address + HaloCE_lib::CheatsEx::Infinite_Ammo_offset),
                  (BYTE)Button_GetCheck(hControls[HINFAMMO]));
               break;
               
            case IDC_SHOWHUD:
            {
               DWORD ShowHud_address = Halo_Process->ReadMem<DWORD>((LPVOID)ShowHud_ptr_address);
               if (ShowHud_address)
               {
                  Halo_Process->WriteMem<BYTE>(
                     (LPVOID)ShowHud_address,
                     (BYTE)Button_GetCheck(hControls[HSHOWHUD]));
               }
               break;
            }
            case IDC_LETTERBOX:
            {
               DWORD LetterBox_address = Halo_Process->ReadMem<DWORD>((LPVOID)LetterBox_ptr_address);
               if (LetterBox_address)
               {
                  LetterBox_address += 8;
                  Halo_Process->WriteMem<BYTE>(
                     (LPVOID)LetterBox_address,
                     (BYTE)Button_GetCheck(hControls[HLETTERBOX]));
               }
               break;
            }
            case IDC_MHUD://IDC_EJECTION:
            {
               //Halo_Process->WriteMem<BYTE>(
               //   (LPVOID)RiderEjection_address,
               //   (BYTE)Button_GetCheck(hControls[HEJECTION]));
               
               MV_chkBx_CheckedChanged(Button_GetCheck(hControls[HMHUD]));
               break;
            }
            case IDC_ALARM:
               Halo_Process->WriteMem<bool>((LPVOID)(Device_Groups_Header.FirstItem 
                  + HCE_Lib::rpg_beta6_2_device_groups::alarm_control_2_offset), true);
               break;
               
            case IDC_BLD_SET_BTN:
            {
               int seconds = 0;
               GetWindowTextW(hControls[HBLD_TXTBX], szBuffer, SZ_BUFFER_SIZE);
               
               
               if (CMDsLib::ParseStrInt((wchar_t*)szBuffer, &seconds))
                  Halo_Process->WriteMem<short>((LPVOID)(HS_Global_Header.FirstItem  + HCE_Lib::rpg_beta6_2_hs_global::lock_timer_offset), seconds * 30);
               break;
            }
            case IDC_BLD_ACT_BTN:
            {
               if (!Halo_Process->ReadMem<bool>((LPVOID)(HS_Global_Header.FirstItem + HCE_Lib::rpg_beta6_2_hs_global::locked_offset)))
                  Halo_Process->WriteMem<BYTE>((LPVOID)(Device_Groups_Header.FirstItem
                     + HCE_Lib::rpg_beta6_2_device_groups::lock_control_offset), 1);
               else
               {
                  //Halo_Process->WriteMemBool(HCE_Lib.Locked, false);
               }
               break;
            }
            case IDC_HALO_SET_BTN:
            {
               int seconds = 0;
               GetWindowTextW(hControls[HHALO_TXTBX], szBuffer, SZ_BUFFER_SIZE);
               
               if (CMDsLib::ParseStrInt((wchar_t*)szBuffer, &seconds))
                  Halo_Process->WriteMem<short>((LPVOID)(HS_Global_Header.FirstItem + HCE_Lib::rpg_beta6_2_hs_global::boom_timer_offset), seconds * 30);
               break;
            }
            case IDC_HALO_ACT_BTN:
            {
               if (!Halo_Process->ReadMem<bool>((LPVOID)(HS_Global_Header.FirstItem + HCE_Lib::rpg_beta6_2_hs_global::nuked_offset)))
                  Halo_Process->WriteMem<BYTE>((LPVOID)(Device_Groups_Header.FirstItem 
                     + HCE_Lib::rpg_beta6_2_device_groups::boom_control_offset), 1);
               else
               {
                  
               }
            break;
            }
         }
         break;
         
      case CBN_SELCHANGE:
         if (id ==  IDC_SETTING)
         {
            int ItemIndex = SendMessage(hwndCtl, CB_GETCURSEL, 0, 0);
            Halo_Process->WriteMem<BYTE>((LPVOID)(HS_Global_Header.FirstItem 
               + HCE_Lib::rpg_beta6_2_hs_global::setting_offset), (BYTE)ItemIndex);
         }
         break;
   }
}

HBRUSH OnCtlColorStatic(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{
   COLORREF textcolor;
   int stock_brush_type = WHITE_BRUSH;
   
   if (pIsAppThemed) theme_active = pIsAppThemed();
  
   switch (GetDlgCtrlID(hwndChild))
   {
      case IDC_HALO_TYPE:
         textcolor = RGB(240,240,240);
         break;

      case IDC_HALO_STATUS:
         textcolor = running_gt != not_running ? RGB(0,128,0) : RGB(255,0,0);
         break;

      case IDC_MAP_LBL:
         textcolor = RGB(240,240,240);
         break;
         
      case IDC_MAP_STATUS:
         textcolor = rpgb6_2_running ? RGB(0,128,0) : RGB(255,0,0);
         break;
         
      case IDC_DEATHLESS:
         if (theme_active)
         {
            if (pDrawThemeParentBackground) pDrawThemeParentBackground(hwndChild, hdc, NULL);
         }
         textcolor = RGB(51,153,255);
         break;
         
      case IDC_INFAMMO:
         if (theme_active)
         {
            if (pDrawThemeParentBackground) pDrawThemeParentBackground(hwndChild, hdc, NULL);
         }
         textcolor = RGB(51,153,255);
         break;
         
      case IDC_SHOWHUD:
         if (theme_active)
         {
            if (pDrawThemeParentBackground) pDrawThemeParentBackground(hwndChild, hdc, NULL);
         }
         textcolor = RGB(51,153,255);
         break;
     
     case IDC_LETTERBOX:
         if (theme_active)
         {
            if (pDrawThemeParentBackground) pDrawThemeParentBackground(hwndChild, hdc, NULL);
         }
         textcolor = RGB(51,153,255);
         break;
         
      case IDC_MHUD://IDC_EJECTION:
         if (theme_active)
         {
            if (pDrawThemeParentBackground) pDrawThemeParentBackground(hwndChild, hdc, NULL);
         }
         textcolor = RGB(51,153,255);
         break;
         
      default:
         return GetStockBrush(WHITE_BRUSH);
   }
   
   SetTextColor(hdc, textcolor);
   //transparent background for all static controls
   SetBkMode(hdc, TRANSPARENT);
   return GetStockBrush(NULL_BRUSH);
}

BOOL SetTpWindowText(HWND hMainWin, HWND hControl, LPCWSTR lpwString)
{
   BOOL succeded;
   GetWindowText(hControl, (LPWSTR)szBuffer, SZ_BUFFER_SIZE);
   //only change if its different
   if (!str_cmpW((wchar_t*)szBuffer, (wchar_t*)lpwString))
   {
      succeded = SetWindowTextW(hControl, lpwString);
      RedrawTpWindow(hMainWin, hControl);
   }
   else
   {
      succeded = TRUE;
   }
   return succeded;
}

BOOL EnableTpWindow(HWND hMainWin, HWND hControl, BOOL bEnable)
{
   BOOL succeded;
   if (IsWindowEnabled(hControl) == bEnable)
   {
      succeded = TRUE;
   }
   else
   {
      succeded = EnableWindow(hControl, bEnable);
      RedrawTpWindow(hMainWin, hControl);
   }
   return succeded;
}

BOOL ShowTpWindow(HWND hMainWin, HWND hControl, BOOL bShow)
{
   BOOL succeded;
   if (IsWindowVisible(hControl) == bShow)
   {
      succeded = TRUE;
   }
   else
   {
      succeded = ShowWindow(hControl, bShow);
      RedrawTpWindow(hMainWin, hControl);
   }
   return succeded;
}

BOOL RedrawTpWindow(HWND hMainWin, HWND hControl)
{
   RECT rectControl;
   GetWindowRect(hControl, &rectControl);   
   MapWindowRect(HWND_DESKTOP, hMainWin, &rectControl);

   return InvalidateRect(hMainWin, &rectControl, TRUE);
}

LRESULT CALLBACK MouseOverProc(
   HWND hCtrl, 
   UINT message, 
   WPARAM wParam, 
   LPARAM lParam, 
   UINT_PTR uIdSubclass, 
   DWORD_PTR dwRefData)
{ 
   switch (message)
   {
      case WM_MOUSEMOVE:
      {
         TRACKMOUSEEVENT tme;
         tme.cbSize = sizeof(TRACKMOUSEEVENT);
         tme.dwFlags = TME_LEAVE;
         tme.hwndTrack = hCtrl;
          _TrackMouseEvent(&tme);
          
         if (!MouseOverControlID)
         {
            MouseOverControlID = dwRefData;
            InvalidateRect(hCtrl, NULL, FALSE);
         }
         break;
      }
      case WM_MOUSELEAVE:
      {
         MouseOverControlID = 0;
         RedrawTpWindow(GetParent(hCtrl), hCtrl);
         break;
      }
      case WM_NCDESTROY:
         RemoveWindowSubclass(hCtrl, MouseOverProc, IDS_MOUSEOVER);
   }
   return DefSubclassProc(hCtrl, message, wParam, lParam);
}

void __stdcall MV_chkBx_CheckedChanged(int number)
{
   LPTHREAD_START_ROUTINE func_address;
   BYTE *asm_func;
   DWORD asm_func_loc;
   
   //use function in dll if dll is loaded
   if (pMV_chkBx_CheckedChanged)
   {
      func_address = (LPTHREAD_START_ROUTINE)pMV_chkBx_CheckedChanged;
   }
   //if not do it the hard way
   else if (cinematic_ptr && cse_set_video_func_address)
   {
      int asm_func_size = 0x6C;
      asm_func = new BYTE[asm_func_size];
      asm_func_loc = (DWORD)Halo_Process->AllocateMemory(asm_func_size);
      
      int byte_loc = 0;
      asm_func[byte_loc++] = 0x55;      //push ebp
      
      asm_func[byte_loc++] = 0x8B;      //mov  ebp,esp
      asm_func[byte_loc++] = 0xEC;
      
      asm_func[byte_loc++] = 0x83;      //cmp dword ptr [ebp+8],2 
      asm_func[byte_loc++] = 0x7D;
      asm_func[byte_loc++] = 0x08;
      asm_func[byte_loc++] = 0x02;
    
      asm_func[byte_loc++] = 0x57;      //push edi
      
      asm_func[byte_loc++] = 0xC6;      //mov  byte ptr [ebp-1],0
      asm_func[byte_loc++] = 0x45;
      asm_func[byte_loc++] = 0xFF;
      asm_func[byte_loc++] = 0x00;

      asm_func[byte_loc++] = 0x75;      //jne short 4
      asm_func[byte_loc++] = 0x04;
      
      asm_func[byte_loc++] = 0xD9;      //fldz
      asm_func[byte_loc++] = 0xEE;       

      asm_func[byte_loc++] = 0xEB;      //jmp short 6
      asm_func[byte_loc++] = 0x06;
      
      asm_func[byte_loc++] = 0xD9;      //fld [__real@40000000]
      asm_func[byte_loc++] = 0x05;
      TO_BYTES(DWORD, &asm_func[byte_loc], asm_func_loc + 0x68);
      byte_loc += sizeof(DWORD);
      
      asm_func[byte_loc++] = 0xD9;      //fstp
      asm_func[byte_loc++] = 0x5D;
      asm_func[byte_loc++] = 0xF8;
      
      asm_func[byte_loc++] = 0x8B;      //mov ecx,dword ptr [ebp+8]
      asm_func[byte_loc++] = 0x4D;
      asm_func[byte_loc++] = 0x08;
      
      asm_func[byte_loc++] = 0x8B;      //mov edx,dword ptr [cinematic_ptr]
      asm_func[byte_loc++] = 0x15;
      TO_BYTES(DWORD, &asm_func[byte_loc], cinematic_ptr);
      byte_loc += sizeof(DWORD);
      
      asm_func[byte_loc++] = 0x85;      //test edx,edx
      asm_func[byte_loc++] = 0xD2;
      
      asm_func[byte_loc++] = 0x74;      //je short 36h
      asm_func[byte_loc++] = 0x36;
      
      asm_func[byte_loc++] = 0x85;      //test ecx,ecx
      asm_func[byte_loc++] = 0xC9;
      
      asm_func[byte_loc++] = 0x74;      //je short 2Ah
      asm_func[byte_loc++] = 0x2A;
      
      asm_func[byte_loc++] = 0x80;      //cmp byte ptr[edx+38h],0
      asm_func[byte_loc++] = 0x7A;
      asm_func[byte_loc++] = 0x38;
      asm_func[byte_loc++] = 0x00;
      
      asm_func[byte_loc++] = 0x75;      //jnz short 14h
      asm_func[byte_loc++] = 0x14;
      
      asm_func[byte_loc++] = 0xB9;      //mov ecx,0Eh
      asm_func[byte_loc++] = 0x0E;
      asm_func[byte_loc++] = 0x00;
      asm_func[byte_loc++] = 0x00;
      asm_func[byte_loc++] = 0x00;
      
      asm_func[byte_loc++] = 0x33;      //xor eax,eax
      asm_func[byte_loc++] = 0xC0;
      
      asm_func[byte_loc++] = 0x8B;      //mov edi,eax
      asm_func[byte_loc++] = 0xFA;
      
      asm_func[byte_loc++] = 0xF3;      //rep stos dword ptr[edi]
      asm_func[byte_loc++] = 0xAB;
      
      asm_func[byte_loc++] = 0x3E;      //mov byte ptr[edx+39h],1
      asm_func[byte_loc++] = 0xC6;
      asm_func[byte_loc++] = 0x42;
      asm_func[byte_loc++] = 0x39;
      asm_func[byte_loc++] = 0x01;
      
      asm_func[byte_loc++] = 0xC6;      //mov byte ptr[edx+38h],1
      asm_func[byte_loc++] = 0x42;
      asm_func[byte_loc++] = 0x38;
      asm_func[byte_loc++] = 0x01;
      
      asm_func[byte_loc++] = 0x8B;      //mov eax,dword ptr[ebp-8]
      asm_func[byte_loc++] = 0x45;
      asm_func[byte_loc++] = 0xF8;
      
      asm_func[byte_loc++] = 0x50;      //push eax
      
      asm_func[byte_loc++] = 0x6A;      //push 2
      asm_func[byte_loc++] = 0x02;
      
      asm_func[byte_loc++] = 0xE8;      //call cse_set_video_func_address
      TO_BYTES(DWORD, &asm_func[byte_loc], cse_set_video_func_address - (asm_func_loc + byte_loc + 4));
      byte_loc += sizeof(DWORD);
      
      asm_func[byte_loc++] = 0x83;      //add esp,8
      asm_func[byte_loc++] = 0xC4;
      asm_func[byte_loc++] = 0x08;
      
      asm_func[byte_loc++] = 0xEB;      //jmp short 4
      asm_func[byte_loc++] = 0x04;
      
      asm_func[byte_loc++] = 0xC6;      //mov byte ptr[edx+38h],0
      asm_func[byte_loc++] = 0x42;
      asm_func[byte_loc++] = 0x38;
      asm_func[byte_loc++] = 0x00;
      
      asm_func[byte_loc++] = 0xC6;      //mov byte ptr[ebp-1],1
      asm_func[byte_loc++] = 0x45;
      asm_func[byte_loc++] = 0xFF;
      asm_func[byte_loc++] = 0x01;
      
      asm_func[byte_loc++] = 0x8A;      //mov al,byte ptr[ebp-1]
      asm_func[byte_loc++] = 0x45;
      asm_func[byte_loc++] = 0xFF;
      
      asm_func[byte_loc++] = 0x5F;      //pop edi
      
      asm_func[byte_loc++] = 0x8B;      //mov esp,ebp
      asm_func[byte_loc++] = 0xE5;
      
      asm_func[byte_loc++] = 0x5D;      //pop ebp
      
      asm_func[byte_loc++] = 0xC2;      //retn 4
      asm_func[byte_loc++] = 0x04;
      asm_func[byte_loc++] = 0x00;

      TO_BYTES(DWORD, &asm_func[byte_loc], 0x40000000);//2.0
      
      //write the function to allocated space
      Halo_Process->WriteMemArray<BYTE>((LPVOID)asm_func_loc, asm_func, asm_func_size);

      //set the address to the injected function
      func_address = (LPTHREAD_START_ROUTINE)asm_func_loc;
    }
   
   HANDLE NewThreadhnd = CreateRemoteThread(
      Halo_Process->GetProcessHandle(),
      NULL,
      0,
      func_address,
      (LPVOID)number, 
      0,
      NULL
      );
      
   if (NewThreadhnd)
   {         
      WaitForSingleObject(NewThreadhnd, 1000);
      CloseHandle(NewThreadhnd);
   }
   else
      DWORD doslasterror = GetLastError();
      
   if (!pMV_chkBx_CheckedChanged)
   {
      Halo_Process->FreeMemory((LPVOID)asm_func_loc);
      delete[] asm_func;
   }
}
/*
6E265E00 55               push        ebp  
6E265E01 8B EC            mov         ebp,esp 
6E265E03 83 EC 08         sub         esp,8 
   291:    bool succeded = false;
   292:    
   293:    float fnum;
   294:    if (number == 2) fnum = 0;
6E265E06 83 7D 08 02      cmp         dword ptr [ebp+8],2 
6E265E0A 57               push        edi  
6E265E0B C6 45 FF 00      mov         byte ptr [succeded],0 
6E265E0F 75 04            jne         MV_chkBx_CheckedChanged+15h (6E265E15h) 
6E265E11 D9 EE            fldz             
6E265E13 EB 06            jmp         MV_chkBx_CheckedChanged+1Bh (6E265E1Bh) 
   295:    else fnum = 2;
6E265E15 D9 05 90 9E 27 6E fld         dword ptr [__real@40000000 (6E279E90h)] 
6E265E1B D9 5D F8         fstp        dword ptr [fnum] 
   296:    
   297:    __asm
   298:    {
   299:       MOV ECX,number
6E265E1E 8B 4D 08         mov         ecx,dword ptr [number] 
   300:       MOV EDX,DWORD PTR [cinematic_ptr]
6E265E21 8B 15 7C EA 27 6E mov         edx,dword ptr [cinematic_ptr (6E27EA7Ch)] 
   301:       MOV EDX,DWORD PTR [EDX]
6E265E27 8B 12            mov         edx,dword ptr [edx] 
   302:       TEST EDX,EDX
6E265E29 85 D2            test        edx,edx 
   303:       JE SHORT cin_failed
6E265E2B 74 37            je          cin_failed (6E265E64h) 
   304:       TEST ECX,ECX
6E265E2D 85 C9            test        ecx,ecx 
   305:       JE SHORT cin_stop
6E265E2F 74 2B            je          cin_stop (6E265E5Ch) 
   306:       CMP BYTE PTR [EDX+38h],0
6E265E31 80 7A 38 00      cmp         byte ptr [edx+38h],0 
   307:       JNZ SHORT skip_cin_start
6E265E35 75 14            jne         skip_cin_start (6E265E4Bh) 
   308:       
   309:       MOV ECX,0Eh
6E265E37 B9 0E 00 00 00   mov         ecx,0Eh 
   310:       XOR EAX,EAX
6E265E3C 33 C0            xor         eax,eax 
   311:       MOV EDI,EDX
6E265E3E 8B FA            mov         edi,edx 
   312:       REP STOS DWORD PTR [EDI]
6E265E40 F3 AB            rep stos    dword ptr es:[edi] 
   313:       MOV BYTE PTR DS:[EDX+39h],1
6E265E42 3E C6 42 39 01   mov         byte ptr ds:[edx+39h],1 
   314:       MOV BYTE PTR [EDX+38h],1               
6E265E47 C6 42 38 01      mov         byte ptr [edx+38h],1 
   315:       
   316:       skip_cin_start:
   317:       MOV EAX,fnum
6E265E4B 8B 45 F8         mov         eax,dword ptr [fnum] 
   318:       PUSH EAX
6E265E4E 50               push        eax  
   319:       PUSH 2
6E265E4F 6A 02            push        2    
   320:       CALL DWORD PTR cse_set_video_func_address
6E265E51 FF 15 78 EA 27 6E call        dword ptr [cse_set_video_func_address (6E27EA78h)] 
   321:       ADD ESP,8
6E265E57 83 C4 08         add         esp,8 
   322:       JMP SHORT cin_succeded
6E265E5A EB 04            jmp         cin_succeded (6E265E60h) 
   323:       
   324:       cin_stop:
   325:       MOV BYTE PTR [EDX+38h],0
6E265E5C C6 42 38 00      mov         byte ptr [edx+38h],0 
   326:       
   327:       cin_succeded:
   328:       MOV succeded,1
6E265E60 C6 45 FF 01      mov         byte ptr [succeded],1 
   329:       
   330:       cin_failed:
   331:    }
   332:    return succeded;
6E265E64 8A 45 FF         mov         al,byte ptr [succeded] 
   333: }
6E265E67 5F               pop         edi  
6E265E68 8B E5            mov         esp,ebp 
6E265E6A 5D               pop         ebp  
6E265E6B C2 04 00         ret         4*/