#include "WinMain.h"
#include "KeyLib.h"

void UpdateEditKey(UINT vkey);
BOOL ChangeKeyOnInitDialog(HWND hDlg, HWND hCtrl, LPARAM init_data);
void ChangeKeyOnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

bool EditNoCharMask(UINT vKey, bool Control);

HWND WinChangeKey = NULL;
UINT Orig_Key = 0, Displayed_vKey = 0;

BOOL CALLBACK ChangeKeyDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
   BOOL ret_val;
   switch (message)
   {
      case WM_CLOSE:
         ret_val = EndDialog(hDlg, Orig_Key);
         break;

      case WM_INITDIALOG:
         ret_val = HANDLE_WM_INITDIALOG(hDlg, wParam, lParam, ChangeKeyOnInitDialog);
         break;

      case WM_COMMAND:
         HANDLE_WM_COMMAND(hDlg, wParam, lParam, ChangeKeyOnCommand);
         ret_val = TRUE;
         break;

      default:
         ret_val = FALSE; //DefDlgProc
   }

   return ret_val;
}

BOOL ChangeKeyOnInitDialog(HWND hDlg, HWND hCtrl, LPARAM init_data)
{
   WinChangeKey = hDlg;

   Orig_Key = (UINT)init_data;
   UpdateEditKey((UINT)init_data);

   SetWindowSubclass(GetDlgItem(hDlg, IDC_CK_CODE), EditMaskProc, IDS_EDIT_MASK, (DWORD_PTR)EditNoCharMask);
   SetWindowSubclass(GetDlgItem(hDlg, IDC_CK_HCODE), EditMaskProc, IDS_EDIT_MASK, (DWORD_PTR)EditNoCharMask);
   return TRUE;
}

void ChangeKeyOnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
   switch (codeNotify)
   {
      case BN_CLICKED:
         switch(id)
         {
            case IDOK:
            {
               EndDialog(hwnd, Displayed_vKey);
               break;
            }
            case IDC_CK_BTN_NONE:
               UpdateEditKey(0);
               break;
         }
         break;
   }
}

bool EditNoCharMask(UINT vKey, bool Control)
{
   UpdateEditKey(vKey);
   return true;
}

void UpdateEditKey(UINT vkey)
{
   Displayed_vKey = vkey;
   SetWindowTextW(GetDlgItem(WinChangeKey, IDC_CK_NAME_LBL), KeyLib::Get_Key_Name(vkey));

   swprintf_s(szBuffer, 6, L"%li", vkey);
   SetWindowTextW(GetDlgItem(WinChangeKey, IDC_CK_CODE), szBuffer);

   szBuffer[0] = L'0';
   szBuffer[1] = L'x';
   swprintf_s(&szBuffer[2], 4, L"%lX", vkey);
   SetWindowTextW(GetDlgItem(WinChangeKey, IDC_CK_HCODE), szBuffer);
}