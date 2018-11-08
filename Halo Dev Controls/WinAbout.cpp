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
	File:    WinAbout.cpp
	Project: Halo Dev Controls
	Author:  Jesus7Freak
	Date:    11/22/2011
	Game:    Halo and Halo Custom Edition
	Version: all
*********************************************************************************/
#include "WinMain.h"

void AboutOnDrawItem(HWND hwnd, const DRAWITEMSTRUCT *lpDrawItem);
BOOL AboutOnInitDialog(HWND hDlg, HWND hCtrl, LPARAM init_data);
HBRUSH AboutOnCtlColorStatic(HWND hwnd, HDC hdc, HWND hwndChild, int type);

HFONT h12Font, h13Font;

BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
   BOOL ret_val;
   switch (message)
   {
      case WM_DRAWITEM:
         ret_val = HANDLE_WM_DRAWITEM(hDlg, wParam, lParam, AboutOnDrawItem);
         break;

      case WM_INITDIALOG:
         HANDLE_WM_INITDIALOG(hDlg, wParam, lParam, AboutOnInitDialog);
         ret_val = TRUE;
         break;

      case WM_COMMAND:
         if (LOWORD(wParam) == IDOK)
         {
            DeleteObject(h12Font);
            DeleteObject(h13Font);
            EndDialog(hDlg, IDOK);
         }
         ret_val = TRUE;
         break;

      case WM_CTLCOLORBTN:
         ret_val = (LRESULT)GetStockBrush(NULL_BRUSH);
         break;

      case WM_CTLCOLORSTATIC:
         ret_val = HANDLE_WM_CTLCOLORSTATIC(hDlg, wParam, lParam, AboutOnCtlColorStatic);
         break;

      default:
         ret_val = FALSE;
   }

   return ret_val;
}

void AboutOnDrawItem(HWND hwnd, const DRAWITEMSTRUCT *lpDrawItem)
{
   if (lpDrawItem->CtlID == IDOK)
   {
      UINT iState = lpDrawItem->itemState;
      BOOL bIsFocused  = (iState & ODS_FOCUS);
      BOOL bIsDisabled = (iState & ODS_DISABLED);

      bool highlight = (iState & ODS_SELECTED) || MouseOverControlID == lpDrawItem->CtlID;
      HBRUSH brBackground;

      if (highlight) brBackground = CreateSolidBrush(RGB(229,229,229));
      else brBackground = GetStockBrush(NULL_BRUSH);

      HDC hdc = lpDrawItem->hDC;//optimizer does this
      FillRect(hdc, &(lpDrawItem->rcItem), brBackground);
      DeleteObject(brBackground);

      int pnWidthEx = 0;
      if (bIsFocused) pnWidthEx++;

      HPEN pnForeColor = CreatePen(PS_SOLID, 1 + pnWidthEx, 0);
      SelectObject(hdc, pnForeColor);
      Rectangle(hdc, lpDrawItem->rcItem.left + pnWidthEx, lpDrawItem->rcItem.top + pnWidthEx, lpDrawItem->rcItem.right, lpDrawItem->rcItem.bottom);
      DeleteObject(pnForeColor);

      int str_len = GetWindowTextW(lpDrawItem->hwndItem, szBuffer, SZ_BUFFER_SIZE);

      SIZE dimensions = {0};
      GetTextExtentPoint32W(hdc, szBuffer, str_len, &dimensions);
      int xPos = (lpDrawItem->rcItem.right - dimensions.cx) / 2;
      int yPos = ((lpDrawItem->rcItem.bottom - dimensions.cy) / 2) + 1;

      SetBkMode(hdc, TRANSPARENT);
      SetTextColor(hdc, bIsDisabled ? GetSysColor(COLOR_GRAYTEXT) : 0);
      TextOut(hdc, xPos, yPos, szBuffer, str_len);
   }
}

BOOL AboutOnInitDialog(HWND hDlg, HWND hCtrl, LPARAM init_data)
{
   HBITMAP BmpBkg = LoadBitmapW(hInst, MAKEINTRESOURCE(IDB_EYE));
   SendDlgItemMessage(hDlg, IDC_BACKGROUND, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BmpBkg);

   SetWindowSubclass(GetDlgItem(hDlg, IDOK), MouseOverProc, IDS_MOUSEOVER, IDOK);

   h12Font = CreateFontW(
      20, 0,
      0,
      0,
      FW_NORMAL,
      FALSE,
      FALSE,
      FALSE,
      ANSI_CHARSET,//DEFAULT_CHARSET,
      OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS,
      DEFAULT_QUALITY,
      DEFAULT_PITCH,
      L"Microsoft Sans Serif"
      );

   h13Font = CreateFontW(
      23, 0,
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

   SetWindowFont(GetDlgItem(hDlg, IDC_ABOUT_NAME), h12Font, TRUE);
   SetWindowFont(GetDlgItem(hDlg, IDC_ABOUT_CREDITS), h12Font, TRUE);
   SetWindowFont(GetDlgItem(hDlg, IDC_ABOUT_THANKS), h12Font, TRUE);
   SetWindowFont(GetDlgItem(hDlg, IDC_ABOUT_PLAYER_CMDS), h12Font, TRUE);
   SetWindowFont(GetDlgItem(hDlg, IDC_ABOUT_VERSION), h12Font, TRUE);
   SetWindowFont(GetDlgItem(hDlg, IDC_ABOUT_AUTHOR), h13Font, TRUE);

   HRSRC hVersion = FindResourceW(
      (HMODULE)init_data,
      MAKEINTRESOURCEW(VS_VERSION_INFO),
      RT_VERSION); // RT_VERSION macro is using MAKEINTRESOURCE macro

   if (hVersion)
   {
      HGLOBAL hGlobal = LoadResource((HMODULE)init_data, hVersion);
      if (hGlobal)
      {
         LPVOID versionInfo = LockResource(hGlobal);
         if (versionInfo)
         {
            LPVOID retbuf; UINT vLen;
            if (VerQueryValueW(
               versionInfo,
               L"\\StringFileInfo\\040904b0\\FileVersion",
               &retbuf,
               &vLen))
            {
               static wchar_t text_buffer[20] = {L'V',L'e',L'r',L's',L'i',L'o',L'n',L':',L' ',L' '};

               wchar_t *version_str = &text_buffer[10];
               for (UINT i = 0; i < vLen; i++)
               {
                  wchar_t retbuf_wchar = ((wchar_t*)retbuf)[i];
                  version_str[i] = retbuf_wchar;
               }

               SetWindowTextW(GetDlgItem(hDlg, IDC_ABOUT_VERSION), text_buffer);
            }
         }
      }
   }
   return TRUE;
}

HBRUSH AboutOnCtlColorStatic(HWND hwnd, HDC hdc, HWND hwndChild, int type)
{
   COLORREF textcolor;

   switch (GetDlgCtrlID(hwndChild))
   {
      case IDC_ABOUT_NAME:
      case IDC_ABOUT_CREDITS:
      case IDC_ABOUT_THANKS:
      case IDC_ABOUT_PLAYER_CMDS:
      case IDC_ABOUT_VERSION:
         textcolor = RGB(255,255,255);
         break;

      case IDC_ABOUT_AUTHOR:
         textcolor = RGB(0,255,0);
         break;

      case IDOK:
         textcolor = RGB(0,0,0);
         break;
   }

   SetTextColor(hdc, textcolor);
   //transparent background for all static controls
   SetBkMode(hdc, TRANSPARENT);
   return GetStockBrush(NULL_BRUSH);
}