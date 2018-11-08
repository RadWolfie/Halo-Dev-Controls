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
    File:    WinTeleLoc.cpp
    Project: Halo Dev Controls
    Author:  Jesus7Freak
    Date:    11/22/2011
    Game:    Halo and Halo Custom Edition
    Version: all
*********************************************************************************/
#include "WinMain.h"

BOOL TLOnInitDialog(HWND hDlg, HWND hCtrl, LPARAM init_data);
void TLOnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

void UpdateMapList(HWND hwnd, int SelectedIndex);
void UpdateLocList(HWND hwnd, int MapsSelectedIndex, int LocsSelectedIndex);

bool EditNoSpaceMask(UINT vKey, bool Control);
bool EditFloatMask(UINT vKey, bool Control);

bool SuppressKeyPress = false,
   locations_changed = false;

std::vector<MAPS> *Locations;

BOOL CALLBACK TeleLocDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
   BOOL ret_val;
   switch (message)
   {
      case WM_CLOSE:
         ret_val = EndDialog(hDlg, 0);
         break;

      case WM_INITDIALOG:
         HANDLE_WM_INITDIALOG(hDlg, wParam, lParam, TLOnInitDialog);
         ret_val = TRUE;
         break;

      case WM_COMMAND:
         HANDLE_WM_COMMAND(hDlg, wParam, lParam, TLOnCommand);
         ret_val = TRUE;
         break;

      default:
         ret_val = FALSE;
   }

   return ret_val;
}

BOOL TLOnInitDialog(HWND hDlg, HWND hCtrl, LPARAM init_data)
{
   HWND hLocName_txtbx = GetDlgItem(hDlg, IDC_TL_LOCNAME_TXTBX);
   Edit_LimitText(hLocName_txtbx, 11);
   SetWindowSubclass(hLocName_txtbx, EditMaskProc, IDS_EDIT_MASK, (DWORD_PTR)EditNoSpaceMask);

   HWND hX_txtbx = GetDlgItem(hDlg, IDC_TL_X_TXTBX);
   Edit_LimitText(hX_txtbx, 16);
   SetWindowSubclass(hX_txtbx, EditMaskProc, IDS_EDIT_MASK, (DWORD_PTR)EditFloatMask);

   HWND hY_txtbx = GetDlgItem(hDlg, IDC_TL_Y_TXTBX);
   Edit_LimitText(hY_txtbx, 16);
   SetWindowSubclass(hY_txtbx, EditMaskProc, IDS_EDIT_MASK, (DWORD_PTR)EditFloatMask);

   HWND hZ_txtbx = GetDlgItem(hDlg, IDC_TL_Z_TXTBX);
   Edit_LimitText(hZ_txtbx, 16);
   SetWindowSubclass(hZ_txtbx, EditMaskProc, IDS_EDIT_MASK, (DWORD_PTR)EditFloatMask);

   Locations = (std::vector<MAPS>*)init_data;

   //trigger lstBx_SelectedIndexChanged
   //lstbx_maps->SelectedIndex = -1;
   HWND hMapList = GetDlgItem(hDlg, IDC_TL_MAPLIST);
   HWND hAddBtn = GetDlgItem(hDlg, IDC_TL_ADD_BTN);
   HWND hRemoveBtn = GetDlgItem(hDlg, IDC_TL_REMOVE_BTN);
   HWND hSetBtn = GetDlgItem(hDlg, IDC_TL_SET_BTN);
   if (Locations->size())
   {
      for (unsigned int i = 0; i < Locations->size(); i++)
         ListBox_AddString(hMapList, (*Locations)[i].map_name);

      ListBox_SetCurSel(hMapList, 0);

      EnableWindow(hAddBtn, TRUE);
      EnableWindow(hRemoveBtn, TRUE);
      EnableWindow(hSetBtn, TRUE);
      UpdateMapList(hDlg, 0);
   }
   else
   {
      EnableWindow(hAddBtn, FALSE);
      EnableWindow(hRemoveBtn, FALSE);
      EnableWindow(hSetBtn, FALSE);
   }

   return TRUE;
}

void TLOnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
   int MapSelectedIndex, LocSelectedIndex;

   switch (codeNotify)
   {
      case BN_CLICKED:
         switch(id)
         {
            case IDOK:
               EndDialog(hwnd, locations_changed);
               break;

            case IDC_TL_REMOVE_BTN:
            {
               HWND hLocList = GetDlgItem(hwnd, IDC_TL_LOCLIST);
               LocSelectedIndex = ListBox_GetCurSel(hLocList);

               HWND hMapList = GetDlgItem(hwnd, IDC_TL_MAPLIST);
               MapSelectedIndex = ListBox_GetCurSel(hMapList);
               if (MapSelectedIndex != LB_ERR)
               {
                  std::vector<TELEPORT_LOCATION> *tl = &(*Locations)[MapSelectedIndex].teleport_locations;

                  tl->erase(tl->begin() + LocSelectedIndex);
                  ListBox_DeleteString(hLocList, LocSelectedIndex);

                  if (!tl->size())
                  {
                     //remove map since theres no more loc's defined
                     Locations->erase(Locations->begin() + MapSelectedIndex);
                     ListBox_DeleteString(hMapList, MapSelectedIndex);

                     if (Locations->size())
                     {
                        ListBox_SetCurSel(hMapList, 0);
                        ListBox_SetCurSel(hLocList, 0);
                        UpdateMapList(hwnd, 0);
                     }
                  }
                  else
                  {
                     int index = tl->size() - 1;
                     ListBox_SetCurSel(hLocList, index);
                     UpdateLocList(hwnd, MapSelectedIndex, index);
                  }

                  locations_changed = true;
               }
               break;
            }
            case IDC_TL_ADD_BTN:
            {
               MapSelectedIndex = ListBox_GetCurSel(GetDlgItem(hwnd, IDC_TL_MAPLIST));
               if (MapSelectedIndex != LB_ERR)
               {
                  TELEPORT_LOCATION new_loc = { {L'n',L'e',L'w',L'_',L'l',L'o',L'c',L'\0'}, {0,0,0}};
                  (*Locations)[MapSelectedIndex].teleport_locations.push_back(new_loc);

                  UpdateMapList(hwnd, MapSelectedIndex);

                  int selectindex = (*Locations)[MapSelectedIndex].teleport_locations.size() - 1;

                  ListBox_SetCurSel(GetDlgItem(hwnd, IDC_TL_LOCLIST), selectindex);
                  UpdateLocList(hwnd, MapSelectedIndex, selectindex);

                  HWND hLocName_txtbx = GetDlgItem(hwnd, IDC_TL_LOCNAME_TXTBX);
                  SetFocus(hLocName_txtbx);
                  Edit_SetSel(hLocName_txtbx, 0, Edit_GetTextLength(hLocName_txtbx));

                  locations_changed = true;
               }
               break;
            }
            case IDC_TL_SET_BTN:
            {
               HWND hLocList = GetDlgItem(hwnd, IDC_TL_LOCLIST);
               LocSelectedIndex = ListBox_GetCurSel(hLocList);

               MapSelectedIndex = ListBox_GetCurSel(GetDlgItem(hwnd, IDC_TL_MAPLIST));
               if (MapSelectedIndex != LB_ERR && LocSelectedIndex != LB_ERR)
               {
                  HWND hLocName_txtbx = GetDlgItem(hwnd, IDC_TL_LOCNAME_TXTBX);
                  int locname_txtbx_length = GetWindowTextLengthW(hLocName_txtbx);
                  GetWindowTextW(hLocName_txtbx, szBuffer, SZ_BUFFER_SIZE);

                  wchar_t *locname_text = (wchar_t*)(*Locations)[MapSelectedIndex].teleport_locations[LocSelectedIndex].teleport_loc_name;

                  for (int i = 0; i < TELE_LOC_NAME_SIZE; i++)
                  {
                     if (i < locname_txtbx_length)
                        if (szBuffer[i] != L' ')
                           locname_text[i] = szBuffer[i];
                        else
                           locname_text[i] = L'_';
                     else
                        locname_text[i] = '\0';
                  }

                  //would be ListBox_EditString
                  ListBox_DeleteString(hLocList, LocSelectedIndex);
                  ListBox_InsertString(hLocList, LocSelectedIndex, szBuffer);
                  ListBox_SetCurSel(hLocList, LocSelectedIndex);

                  HWND hX_txtbx = GetDlgItem(hwnd, IDC_TL_X_TXTBX);
                  GetWindowTextW(hX_txtbx, szBuffer, SZ_BUFFER_SIZE);

                  float *LocCoord = (float*)(*Locations)[MapSelectedIndex].teleport_locations[LocSelectedIndex].coordinates;

                  float coordinate = 0;
                  CMDsLib::ParseStrFloat(szBuffer, &coordinate);
                  LocCoord[0] = coordinate;

                  HWND hY_txtbx = GetDlgItem(hwnd, IDC_TL_Y_TXTBX);
                  GetWindowTextW(hY_txtbx, szBuffer, SZ_BUFFER_SIZE);

                  coordinate = 0;
                  CMDsLib::ParseStrFloat(szBuffer, &coordinate);
                  LocCoord[1] = coordinate;

                  HWND hZ_txtbx = GetDlgItem(hwnd, IDC_TL_Z_TXTBX);
                  GetWindowTextW(hZ_txtbx, szBuffer, SZ_BUFFER_SIZE);

                  coordinate = 0;
                  CMDsLib::ParseStrFloat(szBuffer, &coordinate);
                  LocCoord[2] = coordinate;

                  locations_changed = true;
               }
               break;
            }
         }
         break;

      case LBN_SELCHANGE:
         switch (id)
         {
            case IDC_TL_MAPLIST:
               UpdateMapList(hwnd, ListBox_GetCurSel(hwndCtl));
               break;

            case IDC_TL_LOCLIST:
               UpdateLocList(hwnd, ListBox_GetCurSel(GetDlgItem(hwnd, IDC_TL_MAPLIST)), ListBox_GetCurSel(hwndCtl));
               break;
         }
         break;
   }
}

void UpdateMapList(HWND hwnd, int SelectedIndex)
{
   if (SelectedIndex != LB_ERR)
   {
      HWND hLocList = GetDlgItem(hwnd, IDC_TL_LOCLIST);

      ListBox_ResetContent(hLocList);
      UINT tl_size = (*Locations)[SelectedIndex].teleport_locations.size();
      std::vector<TELEPORT_LOCATION> *tl = &(*Locations)[SelectedIndex].teleport_locations;

      for (UINT i = 0; i < tl_size; i++)
         ListBox_AddString(hLocList, (*tl)[i].teleport_loc_name);

      //trigger lstBx_SelectedIndexChanged
      ListBox_SetCurSel(hLocList, 0);
      UpdateLocList(hwnd, SelectedIndex, 0);
   }
}

void UpdateLocList(HWND hwnd, int MapsSelectedIndex, int LocsSelectedIndex)
{
   if (MapsSelectedIndex != LB_ERR && LocsSelectedIndex != LB_ERR)
   {
      //pointer is faster
      TELEPORT_LOCATION *tl = &(*Locations)[MapsSelectedIndex].teleport_locations[LocsSelectedIndex];

      SetDlgItemText(hwnd, IDC_TL_LOCNAME_TXTBX, tl->teleport_loc_name);

      swprintf_s(szBuffer, SZ_BUFFER_SIZE, L"%f", tl->coordinates[0]);
      SetDlgItemText(hwnd, IDC_TL_X_TXTBX, szBuffer);

      swprintf_s(szBuffer, SZ_BUFFER_SIZE, L"%f", tl->coordinates[1]);
      SetDlgItemText(hwnd, IDC_TL_Y_TXTBX, szBuffer);

      swprintf_s(szBuffer, SZ_BUFFER_SIZE, L"%f", tl->coordinates[2]);
      SetDlgItemText(hwnd, IDC_TL_Z_TXTBX, szBuffer);
   }
}

bool EditNoSpaceMask(UINT vKey, bool Control)
{
   return vKey == VK_SPACE;
}

bool EditFloatMask(UINT vKey, bool Control)
{
   bool SuppressKey = true;
   /* checking the 24th bit
   ((vkFlags & 0x1000000) >> == 0x1000000)
   01021AD0 8B 4C 24 08      mov         ecx,dword ptr [esp+8]
   011F1AD4 81 E1 00 00 00 01 and         ecx,1000000h
   011F1ADA 81 F9 00 00 00 01 cmp         ecx,1000000h
   011F1AE0 0F 94 C2         sete        dl

   (((BYTE)(vkFlags >> 24)) & 1)   //faster by 8 bytes
   01021AD0 8B 4C 24 08      mov         ecx,dword ptr [esp+8]
   01021AD4 B0 01            mov         al,1
   01021AD6 C1 E9 18         shr         ecx,18h
   01021AD9 22 C8            and         cl,al

   .net e->Control   KeyEventArgs::KeyData
   00000000 8B 41 04         mov         eax,dword ptr [ecx+4]
   00000003 25 00 00 02 00   and         eax,20000h
   00000008 3D 00 00 02 00   cmp         eax,20000h
   0000000d 0F 94 C0         sete        al
   00000010 0F B6 C0         movzx       eax,al
   00000013 C3               ret
   */
   //bool Control = (((BYTE)(vkFlags >> 17)) & 1);

   if (vKey >= '0' && vKey <= '9' ||
      vKey >= VK_NUMPAD0 && vKey <= VK_NUMPAD9 ||
      vKey == VK_OEM_MINUS ||
      vKey == VK_OEM_PERIOD ||
      vKey == VK_SUBTRACT ||
      vKey == VK_DECIMAL ||
      vKey == VK_BACK ||
      vKey == VK_DELETE ||
      Control)//GetAsyncKeyState(VK_CONTROL))
   {
      SuppressKey = false;
   }

   return SuppressKey;
}

LRESULT CALLBACK EditMaskProc(
   HWND hCtrl,
   UINT message,
   WPARAM wParam,
   LPARAM lParam,
   UINT_PTR uIdSubclass,
   DWORD_PTR dwRefData)
{
   static bool control_key_down;

   switch (message)
   {
      case WM_GETDLGCODE:
         if (SuppressKeyPress)
         {
            lParam = NULL;
            return 0;
         }
         break;

      case WM_KEYDOWN:
         if (wParam == VK_CONTROL) control_key_down = true;
         //if (wParam == VK_SHIFT) ModifierKeys.Control = true;

         if (dwRefData)
         {
            EditMaskFnType EditMaskFn = (EditMaskFnType)dwRefData;

            if (EditMaskFn((UINT)wParam, control_key_down)) SuppressKeyPress = true;
         }
         break;

      case WM_KEYUP:
         if (wParam == VK_CONTROL) control_key_down = false;
         //if (wParam == VK_SHIFT) ModifierKeys.Control = false;

         SuppressKeyPress = false;
         break;

      //case WM_SYSKEYDOWN:
      //   if (wParam == VK_MENU) ModifierKeys.Alt = true;
      //   break;

      //case WM_SYSKEYUP:
      //   if (wParam == VK_MENU) ModifierKeys.Alt = false;
      //   break;

      case WM_NCDESTROY:
         RemoveWindowSubclass(hCtrl, EditMaskProc, IDS_EDIT_MASK);
   }
   return DefSubclassProc(hCtrl, message, wParam, lParam);
}