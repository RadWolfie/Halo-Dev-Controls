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
	File:    WinCMDS.cpp
	Project: Halo Dev Controls
	Author:  Jesus7Freak
	Date:    11/22/2011
	Game:    Halo and Halo Custom Edition
	Version: all
*********************************************************************************/
#include "WinMain.h"
#include "KeyLib.h"

BOOL CMDSOnInitDialog(HWND hDlg, HWND hCtrl, LPARAM init_data);
void CMDSOnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

void UpdateCMDWindow(HWND hwnd, int SelectedIndex);

wchar_t szCMD_Title[88] = {L'<',L't',L'y',L'p',L'e',L'>',L' ',L' ',L' ',L' ',
   L' ',L' ',L'C',L'o',L'm',L'm',L'a',L'n',L'd',L's',L' ',L'a',L'n',L'd',L' ',
   L'K',L'e',L'y',L'b',L'o',L'a',L'd',L' ',L'S',L'h',L'o',L'r',L't',L'c',L'u',
   L't',L's',L' ',L' ',L' ',L' ',L' ',L'c',L'o',L'm',L'm',L'a',L'n',L'd',L's',
   L' ',L'w',L'o',L'r',L'k',L' ',L'i',L'n',L' ',L'c',L'o',L'n',L's',L'o',L'l',
   L'e',L',',L' ',L'r',L'c',L'o',L'n',L',',L' ',L'a',L'n',L'd',L' ',L'c',L'h',
   L'a',L't',L'\0'},//wont work as regular *
   *szPlayer2ndDescript = L"See readme for [pExpression] example";

CMDsLib::COMMANDS *Commands = NULL;
BOOL cmd_val_changed = FALSE;

BOOL CALLBACK CMDSDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
   BOOL ret_val;
   switch (message)
   {
      case WM_CLOSE:
         ret_val = EndDialog(hDlg, 0);
         break;

      case WM_INITDIALOG:
         HANDLE_WM_INITDIALOG(hDlg, wParam, lParam, CMDSOnInitDialog);
         ret_val = TRUE;
         break;

      case WM_COMMAND:
         HANDLE_WM_COMMAND(hDlg, wParam, lParam, CMDSOnCommand);
         ret_val = TRUE;
         break;

      default:
         ret_val = FALSE;
   }

   return ret_val;
}

BOOL CMDSOnInitDialog(HWND hDlg, HWND hCtrl, LPARAM init_data)
{
   Commands = (CMDsLib::COMMANDS*)init_data;
   cmd_val_changed = FALSE;

   wchar_t *type_name = Commands->cmds_group_name;
   for (int i = 0; type_name[i]; i++)
      szCMD_Title[i] = type_name[i];

   SetWindowText(hDlg, szCMD_Title);

   HWND hlistbox = GetDlgItem(hDlg, IDC_LISTBOX);
   for (int i = 0; i < Commands->size; i++)
      ListBox_AddString(hlistbox, Commands->cmd_descripts[i].cmd_title);

   HWND hEnableCkBx = GetDlgItem(hDlg, IDC_ENABLE);
   switch (Commands->Enable_Shrtcts)
   {
      case 0:
         Button_SetCheck(hEnableCkBx, FALSE);
         break;
      case 1:
         Button_SetCheck(hEnableCkBx, TRUE);
         break;
      case -1:
         Button_SetCheck(hEnableCkBx, FALSE);
         EnableWindow(hEnableCkBx, FALSE);
         ShowWindow(hEnableCkBx, 0);

         HWND h2nd_descript = GetDlgItem(hDlg, IDC_SEC_DESCRIPT);
         SetWindowTextW(h2nd_descript, szPlayer2ndDescript);
         ShowWindow(h2nd_descript, 1);
         break;
   }

   ListBox_SetCurSel(hlistbox, 0);
   UpdateCMDWindow(hDlg, 0);
   return TRUE;
}

void CMDSOnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
   switch (codeNotify)
   {
      case BN_CLICKED:
         switch(id)
         {
            case IDOK:
               EndDialog(hwnd, cmd_val_changed);
               break;

            case IDC_CC_SET_BTN:
               //change cmd name code here
               break;

            case IDC_ENABLE:
            {
               cmd_val_changed = TRUE;
               Commands->Enable_Shrtcts = Button_GetCheck(hwndCtl);
               //trigger lstBx_SelectedIndexChanged
               ListBox_SetCurSel(GetDlgItem(hwnd, IDC_LISTBOX), 0);
               UpdateCMDWindow(hwnd, 0);
               break;
            }
            case IDC_ACT_BTN:
            {
               int SelectedIndex = ListBox_GetCurSel(GetDlgItem(hwnd, IDC_LISTBOX));
               short *pOld_key = &Commands->cmd_keys[SelectedIndex].toggle_key;
               short old_key = *pOld_key;
               short new_key = DialogBoxParamW(hInst, MAKEINTRESOURCE(IDD_CHANGE_KEY), hwnd, ChangeKeyDlgProc, (LPARAM)old_key);
               if (old_key != new_key)
               {
                  *pOld_key = new_key;
                  SetWindowTextW(hwndCtl, KeyLib::Get_Key_Name(new_key));
                  cmd_val_changed = TRUE;
               }
               break;
            }
            case IDC_ON_BTN:
            {
               int SelectedIndex = ListBox_GetCurSel(GetDlgItem(hwnd, IDC_LISTBOX));
               short *pOld_key = &Commands->cmd_keys[SelectedIndex].on_key;
               short old_key = *pOld_key;
               short new_key = DialogBoxParamW(hInst, MAKEINTRESOURCE(IDD_CHANGE_KEY), hwnd, ChangeKeyDlgProc, (LPARAM)old_key);
               if (old_key != new_key)
               {
                  *pOld_key = new_key;
                  SetWindowTextW(hwndCtl, KeyLib::Get_Key_Name(new_key));
                  cmd_val_changed = TRUE;
               }
               break;
            }
            case IDC_OFF_BTN:
            {
               int SelectedIndex = ListBox_GetCurSel(GetDlgItem(hwnd, IDC_LISTBOX));
               short *pOld_key = &Commands->cmd_keys[SelectedIndex].off_key;
               short old_key = *pOld_key;
               short new_key = DialogBoxParamW(hInst, MAKEINTRESOURCE(IDD_CHANGE_KEY), hwnd, ChangeKeyDlgProc, (LPARAM)old_key);
               if (old_key != new_key)
               {
                  *pOld_key = new_key;
                  SetWindowTextW(hwndCtl, KeyLib::Get_Key_Name(new_key));
                  cmd_val_changed = TRUE;
               }
               break;
            }
         }
         break;

      case LBN_SELCHANGE:
         if (id == IDC_LISTBOX)
            UpdateCMDWindow(hwnd, ListBox_GetCurSel(hwndCtl));

         break;
   }
}

void UpdateCMDWindow(HWND hwnd, int SelectedIndex)
{
   if (SelectedIndex != LB_ERR)
   {
      CMDsLib::CMD_DESCRIPT *pCD = &Commands->cmd_descripts[SelectedIndex];
      SetWindowTextW(GetDlgItem(hwnd, IDC_CMD_TXTBX), pCD->cmd_header);
      SetWindowTextW(GetDlgItem(hwnd, IDC_USAGE_LBL), pCD->cmd_usage);
      SetWindowTextW(GetDlgItem(hwnd, IDC_DESCRIPT_LBL), pCD->cmd_descript);

      if (Commands->cmd_keys)
      {
         BOOL enable_checked = Button_GetCheck(GetDlgItem(hwnd, IDC_ENABLE));

         CMDsLib::CMD_SCKEYS *pCS = &Commands->cmd_keys[SelectedIndex];

         HWND hActBtn = GetDlgItem(hwnd, IDC_ACT_BTN);
         SetWindowTextW(hActBtn, KeyLib::Get_Key_Name(pCS->toggle_key));
         EnableWindow(hActBtn, pCS->toggle_key != -1 && enable_checked);

         HWND hOnBtn = GetDlgItem(hwnd, IDC_ON_BTN);
         SetWindowTextW(hOnBtn, KeyLib::Get_Key_Name(pCS->on_key));
         EnableWindow(hOnBtn, pCS->on_key != -1 && enable_checked);

         HWND hOffBtn = GetDlgItem(hwnd, IDC_OFF_BTN);
         SetWindowTextW(hOffBtn, KeyLib::Get_Key_Name(pCS->off_key));
         EnableWindow(hOffBtn, pCS->off_key != -1 && enable_checked);
      }
   }
}