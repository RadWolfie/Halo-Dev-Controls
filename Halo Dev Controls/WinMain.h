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
#pragma once
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "version.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "libcpmt.lib")
#pragma comment(lib, "LIBCMT.lib")


// Enabling Windows XP visual effects (aka themes)
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include "Resource.h"
#include "HaloLib.h"
#include "tele_loc_resources.h"
#include "cmds.h"

#include <windowsx.h>
#include "Commctrl.h"

typedef bool (__stdcall *EditMaskFnType)(UINT, bool);

struct DLL_ADDRS
{
   HWND hHaloWin;
   CMDsLib::CMD_SCKEYS **halo_cmd_keys;
   CMDsLib::CMD_SCKEYS **rpgb62_cmd_keys;
   BOOL *halo_sk_enabled;
   BOOL *rpg_beta62_sk_enabled;
   bool (__stdcall *pFunc)(int);
   wchar_t *CurrentDir;
   WORD *game;
}extern dll_addresses;

BOOL CALLBACK CMDSDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) ;
BOOL CALLBACK ChangeKeyDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK TeleLocDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK EditMaskProc(
   HWND hCtrl,
   UINT message,
   WPARAM wParam,
   LPARAM lParam,
   UINT_PTR uIdSubclass,
   DWORD_PTR dwRefData
   );

LRESULT CALLBACK MouseOverProc(
   HWND hCtrl,
   UINT message,
   WPARAM wParam,
   LPARAM lParam,
   UINT_PTR uIdSubclass,
   DWORD_PTR dwRefData
   );

DWORD WINAPI HDCThread(LPVOID lpMainWin);

//for transparent controls
BOOL RedrawTpWindow(HWND hMainWin, HWND hControl);
BOOL EnableTpWindow(HWND hMainWin, HWND hControl, BOOL bEnable);
BOOL SetTpWindowText(HWND hMainWin, HWND hControl, LPCWSTR lpwString);
BOOL ShowTpWindow(HWND hMainWin, HWND hControl, BOOL bShow);

void __stdcall MV_chkBx_CheckedChanged(int number);
//from dll
extern bool (__stdcall *pMV_chkBx_CheckedChanged)(int number);

//need to make sure wstr is big enough for str
inline void str_to_wstr(char *str, wchar_t *wstr);

#define NUM_OF_CONTROLS 23
#define SZ_BUFFER_SIZE 32

#define HHALO_TYPE      0
#define HHALO_STATUS    1
#define HMAP_LBL        2
#define HMAP_STATUS     3
#define HMENU_BUTTON    4
#define HSERVER_STATUS  5

#define HDEV            6
#define HCONSOLE        7
#define HDEATHLESS      8
#define HINFAMMO        9
#define HSHOWHUD        10
#define HLETTERBOX      11
//#define HEJECTION       12
#define HMHUD           12

#define HSETTING        13
#define HALARM          14
#define HBLD_LBL        15
#define HBLD_TXTBX      16
#define HBLD_SET_BTN    17
#define HBLD_ACT_BTN    18
#define HHALO_LBL       19
#define HHALO_TXTBX     20
#define HHALO_SET_BTN   21
#define HHALO_ACT_BTN   22


extern HWND hControls[NUM_OF_CONTROLS];
extern HMODULE hUxTheme;

extern RWMemory *Halo_Process;
extern HMODULE hHDC;
extern bool exiting,
   dev_enabled,
   console_enabled,
   alarm_on,
   rpgb6_2_running,
   Locked,
   Nuked;

extern BOOL theme_active;

extern wchar_t *szWindowClass,
   *szTitle,
   *szHaloCE,
   *szHaloPC,
   *szHaloCE_exe,
   *szHaloPC_exe,
   *szOn,
   *szOff,
   *szMap,
   *szMainMenu,
   *szClient,
   *szHost,
   *szDisableDev,
   *szEnableDev,
   *szDisableConsole,
   *szEnableConsole,
   *Locations_File_Name,
   *Settings_File_Name,
   *Dll_Name,
   *main_module_name,
   *empty_str,
   *setting_names[3],
   *szAlarmOn,
   *szAlarmOff,
   *szSet,
   *szBaseLockAct,
   *szBaseLockLocked,
   *szSeconds,
   *szHaloFire,
   *szHaloCoolDown,
   szBuffer[SZ_BUFFER_SIZE];

extern int MouseOverControlID;

extern DWORD scan_size,
   Current_Map_address,
   Cheats_address,
   ServerType_address,
   Device_Groups_Header_ptr_address,
   HS_Global_Header_ptr_address,
   dll_addrs_ptr,
   Dev_enabled_address,
   Console_enabled_address,
   ShowHud_ptr_address,
   LetterBox_ptr_address,
   //RiderEjection_address;
   cse_set_video_func_address,
   cinematic_ptr;

enum game_types : WORD
   { not_running = 0, Halo = 1, HCE = 2 } extern running_gt;

enum server_type : WORD
   { main_menu = 0, client = 1, host = 2 } extern running_sv_t;

extern HaloCE_lib::DATA_HEADER Device_Groups_Header,
   HS_Global_Header;

extern HINSTANCE hInst;