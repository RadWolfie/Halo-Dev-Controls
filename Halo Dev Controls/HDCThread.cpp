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
    File:    HDCThread.cpp
    Project: Halo Dev Controls
    Author:  Jesus7Freak
    Date:    11/22/2011
    Game:    Halo and Halo Custom Edition
    Version: all
*********************************************************************************/
#include "WinMain.h"

char map_str[MAP_STR_SIZE] = {0},
   BufferA[32] = {0};

DLL_ADDRS dll_addresses = {0};
tagPROCESSENTRY32W pHalo = {0};

DWORD WINAPI HDCThread(LPVOID lpMainWin)
{
   HWND hMainWin = (HWND)lpMainWin;

   //thread vars should be a little faster than static
   bool control_enabled_change = false,
      dll_injector_failed = false;

   BYTE C_Setting = 0;

   short Lock_sec = 0,
         Halo_sec = 0;

   ULONG_PTR base_address = NULL;

   while (!exiting)
   {
      HWND hHaloWin = FindWindowW(L"Halo", L"Halo");
      if (hHaloWin)
      {
         DWORD dwProcessId;
         GetWindowThreadProcessId(hHaloWin, &dwProcessId);
         pHalo = RWMemory::GetProcessByID_W(dwProcessId);

         if (str_cmpW(pHalo.szExeFile, szHaloPC_exe))
         {
            running_gt = Halo;
            main_module_name = szHaloPC_exe;
            SetTpWindowText(hMainWin, hControls[HHALO_TYPE], szHaloPC);
         }
         else if (str_cmpW(pHalo.szExeFile, szHaloCE_exe))
         {
            running_gt = HCE;
            main_module_name = szHaloCE_exe;
            SetTpWindowText(hMainWin, hControls[HHALO_TYPE], szHaloCE);
         }
      }
      else
      {
         if (Halo_Process)
         {
            running_gt = not_running;
            rpgb6_2_running = false;

            SetTpWindowText(hMainWin, hControls[HHALO_TYPE], szHaloCE);
            SetTpWindowText(hMainWin, hControls[HHALO_STATUS], szOff);
            //status_lbl2->ForeColor = System::Drawing::Color::Red;

            main_module_name = empty_str;
            SetTpWindowText(hMainWin, hControls[HMAP_LBL], empty_str);
            SetTpWindowText(hMainWin, hControls[HMAP_STATUS], empty_str);

            dll_injector_failed = false;
            base_address = NULL;
            scan_size = 0;

            HS_Global_Header_ptr_address = NULL;
            Device_Groups_Header_ptr_address = NULL;

            Current_Map_address = NULL;
            Cheats_address = NULL;
            ServerType_address = NULL;

            control_enabled_change = true;

            Dev_enabled_address = NULL;
            Console_enabled_address = NULL;
            ShowHud_ptr_address = NULL;
            LetterBox_ptr_address = NULL;
            //RiderEjection_address = NULL;
            cse_set_video_func_address = NULL;
            cinematic_ptr = NULL;

            hHDC = NULL;

            //force update
            running_sv_t = host;

            delete Halo_Process;
            Halo_Process = NULL;
         }
      }

      if (running_gt != not_running)
      {
         if (!Halo_Process)
         {
            Halo_Process = new RWMemory(pHalo);

            SetTpWindowText(hMainWin, hControls[HMAP_LBL], szMap);
            SetTpWindowText(hMainWin, hControls[HHALO_STATUS], szOn);
            //status_lbl2->ForeColor = System::Drawing::Color::Green;

            if (Halo_Process->LastWin32Error == ERROR_ACCESS_DENIED)
            {
               ::MessageBoxW(
                  hMainWin,
                  L"This App needs to be run as administrator",
                  L"Error",
                  MB_OK | MB_ICONWARNING | MB_TASKMODAL
                  );

               SendMessage(hMainWin, WM_DESTROY, 0, 0);
            }

            //find halo module info
            Halo_Process->ModuleSectionAddr_Size(
               Halo_Process->GetProcessModuleByNameW(main_module_name).hModule,
               base_address,
               scan_size,
               (BYTE*)".text\0\0",
               false);

            //find patterns
            ULONG_PTR DGHptr_ptr = Halo_Process->FindMemPattern(
               base_address,
               scan_size,
               HaloCE_lib::Device_Groups_ptr_sig);

            Device_Groups_Header_ptr_address = Halo_Process->ReadMem<DWORD>((LPVOID)DGHptr_ptr);


            ULONG_PTR HSGptr_ptr = Halo_Process->FindMemPattern(
               base_address,
               scan_size,
               HaloCE_lib::HS_Globals_ptr_sig);

            HS_Global_Header_ptr_address = Halo_Process->ReadMem<DWORD>((LPVOID)HSGptr_ptr);

            if (running_gt == HCE)
            {
               ULONG_PTR Dev_ptr = Halo_Process->FindMemPattern(
                  base_address,
                  scan_size,
                  HCE_Lib::Dev_addr_sig);

               Dev_enabled_address = Halo_Process->ReadMem<DWORD>((LPVOID)Dev_ptr);
            }

            ULONG_PTR console_ptr = Halo_Process->FindMemPattern(
               base_address,
               scan_size,
               HaloCE_lib::Console_addr_sig);

            Console_enabled_address = Halo_Process->ReadMem<DWORD>((LPVOID)console_ptr) + HaloCE_lib::Console::Enabled_offset;

            ULONG_PTR CMA_ptr = Halo_Process->FindMemPattern(
               base_address,
               scan_size,
               HaloCE_lib::Current_map_addr_sig);

            Current_Map_address = Halo_Process->ReadMem<DWORD>((LPVOID)CMA_ptr);


            ULONG_PTR mem_region_ptr = Halo_Process->FindMemPattern(
               base_address,
               scan_size,
               HaloCE_lib::Cheats_addr_sig);

            Cheats_address = Halo_Process->ReadMem<DWORD>((LPVOID)mem_region_ptr);


            ULONG_PTR SHp_ptr = Halo_Process->FindMemPattern(
               base_address,
               scan_size,
               HaloCE_lib::Show_Hud_ptr_addr_sig);

            ShowHud_ptr_address = Halo_Process->ReadMem<DWORD>((LPVOID)SHp_ptr);


            ULONG_PTR lb_ptr = Halo_Process->FindMemPattern(
               base_address,
               scan_size,
               HaloCE_lib::letterbox_ptr_addr_sig);

            LetterBox_ptr_address = Halo_Process->ReadMem<DWORD>((LPVOID)lb_ptr);


            //ULONG_PTR re_ptr = Halo_Process->FindMemPattern(
            //   base_address,
            //   scan_size,
            //   HaloCE_lib::Rider_Eject_addr_sig);
            //
            //RiderEjection_address = Halo_Process->ReadMem<DWORD>((LPVOID)re_ptr);

            ULONG_PTR svhc_ptr = Halo_Process->FindMemPattern(
               base_address,
               scan_size,
               HaloCE_lib::sv_ban_func_addr_sig);

            ServerType_address = Halo_Process->ReadMem<DWORD>((LPVOID)(svhc_ptr + 3));


            ULONG_PTR cse_ptr = Halo_Process->FindMemPattern(
               base_address,
               scan_size,
               HaloCE_lib::cse_set_video_func_addr_sig);

            cse_set_video_func_address = cse_ptr;
            cinematic_ptr = Halo_Process->ReadMem<DWORD>((LPVOID)(cse_ptr + 2));

            control_enabled_change = true;
         }

         ////give game time to load then inject dll
         if (!hHDC && !dll_injector_failed && Halo_Process->ReadMem<DWORD>((LPVOID)HS_Global_Header_ptr_address))
         {
            wchar_t *current_directory = NULL; DWORD cd_size = 0;
            cd_size = ::GetCurrentDirectoryW(cd_size, current_directory);

            int dll_name_length = 0; do dll_name_length++; while(Dll_Name[dll_name_length]);

            current_directory = new wchar_t[cd_size + 1 + dll_name_length];
            ::GetCurrentDirectoryW(cd_size, current_directory);

            //dll releases the memory
            DWORD CurrentDir_address = (DWORD)Halo_Process->AllocateMemory(cd_size);
            Halo_Process->WriteMemArray<wchar_t>((LPVOID)CurrentDir_address, current_directory, cd_size);

            current_directory[cd_size - 1] = L'\\';

            for (int i = 0; i <= dll_name_length; i++)
               current_directory[cd_size + i] = Dll_Name[i];

            hHDC = Halo_Process->InjectDLL(current_directory, true, false);
            if (!hHDC)
            {
               dll_injector_failed = true;
            }
            else
            {
               Halo_Process->ModuleSectionAddr_Size((HMODULE)hHDC, base_address, scan_size, (BYTE*)".data\0\0", false);

               dll_addrs_ptr = Halo_Process->FindMemPattern(
                  base_address,
                  scan_size,
                  (BYTE*)"dll_st_addrs",
                  "xxxxxxxxxxxx") + 12;

               dll_addresses = Halo_Process->ReadMem<DLL_ADDRS>((LPVOID)dll_addrs_ptr);
               Halo_Process->WriteMem<DWORD>((LPVOID)(dll_addrs_ptr + FIELD_OFFSET(DLL_ADDRS, CurrentDir)), CurrentDir_address);

               //copy current settings to dll
               Halo_Process->WriteMem<HWND>((LPVOID)(dll_addrs_ptr + FIELD_OFFSET(DLL_ADDRS, hHaloWin)), hHaloWin);

               Halo_Process->WriteMemArray<CMDsLib::CMD_SCKEYS>((LPVOID)
                  dll_addresses.halo_cmd_keys,
                  CMDsLib::halo_cmd_keys,
                  HALO_CMDS_SIZE);

               Halo_Process->WriteMemArray<CMDsLib::CMD_SCKEYS>((LPVOID)
                  dll_addresses.rpgb62_cmd_keys,
                  CMDsLib::rpg_beta6_2_cmd_keys,
                  RPGB_CMDS_SIZE);

               Halo_Process->WriteMem<BOOL>((LPVOID)dll_addresses.halo_sk_enabled, CMDsLib::halo_commands.Enable_Shrtcts);
               Halo_Process->WriteMem<BOOL>((LPVOID)dll_addresses.rpg_beta62_sk_enabled, CMDsLib::rpgbeta6_2_commands.Enable_Shrtcts);

               pMV_chkBx_CheckedChanged = dll_addresses.pFunc;

               Halo_Process->WriteMem<WORD>((LPVOID)dll_addresses.game, (WORD)running_gt);

               base_address = NULL;//reset for below
            }
            delete[] current_directory;
         }

         if (Device_Groups_Header_ptr_address)
            Device_Groups_Header = Halo_Process->ReadMem<HaloCE_lib::DATA_HEADER>(
               (LPVOID)Halo_Process->ReadMem<DWORD>((LPVOID)Device_Groups_Header_ptr_address));

         if (HS_Global_Header_ptr_address)
            HS_Global_Header = Halo_Process->ReadMem<HaloCE_lib::DATA_HEADER>(
               (LPVOID)Halo_Process->ReadMem<DWORD>((LPVOID)HS_Global_Header_ptr_address));

         if (Current_Map_address)
            Halo_Process->ReadMemString((LPVOID)Current_Map_address, map_str);

         if (ServerType_address)
         {
         //update host/client lbl 0 - main menu, 1 - client, 2 - host
            BYTE svtemp = (BYTE)running_sv_t;
            running_sv_t = Halo_Process->ReadMem<server_type>((LPVOID)ServerType_address);

            if ((BYTE)running_sv_t != svtemp)
            {
               switch (running_sv_t)
               {
                  case main_menu:
                     //if (str_cmp(map_str, "ui"))
                     //{
                        //----rcon_lbl->ForeColor = System::Drawing::Color::Red;
                        SetTpWindowText(hMainWin, hControls[HSERVER_STATUS], szMainMenu);
                     //}
                     //else
                     //{
                     //   rcon_lbl->ForeColor = System::Drawing::SystemColors::Highlight;
                     //   rcon_lbl->Text = "Campaign";
                     //}
                     break;

                  case client:
                     SetTpWindowText(hMainWin, hControls[HSERVER_STATUS], szClient);
                     //rcon_lbl->ForeColor = System::Drawing::Color::Red;
                     break;

                  case host:
                     SetTpWindowText(hMainWin, hControls[HSERVER_STATUS], szHost);
                     //rcon_lbl->ForeColor = System::Drawing::Color::Green;
                     break;
               }
            }
         }

         if (Dev_enabled_address)
         {
             //update dev btn
            bool devtemp = Halo_Process->ReadMem<bool>((LPVOID)Dev_enabled_address);
            if (dev_enabled != devtemp)
            {
               dev_enabled = devtemp;
               if (dev_enabled)
               {
                  SetTpWindowText(hMainWin, hControls[HDEV], szDisableDev);
                  //btn_dev->ForeColor = System::Drawing::Color::Green;//Chartreuse;
               }
               else
               {
                  SetTpWindowText(hMainWin, hControls[HDEV], szEnableDev);
                  //btn_dev->ForeColor = System::Drawing::Color::Red;
               }
            }
         }

         if (Console_enabled_address)
         {
            //update console btn
            bool consoletemp = Halo_Process->ReadMem<bool>((LPVOID)Console_enabled_address);
            if (console_enabled != consoletemp)
            {
               console_enabled = consoletemp;
               if (console_enabled)
               {
                  SetTpWindowText(hMainWin, hControls[HCONSOLE], szDisableConsole);
                  //btn_console->ForeColor = System::Drawing::Color::Green;//Chartreuse;
               }
               else
               {
                  SetTpWindowText(hMainWin, hControls[HCONSOLE], szEnableConsole);
                  //btn_console->ForeColor = System::Drawing::Color::Red;
               }
            }
         }

         if (Cheats_address)
         {
            //update deathless value
            int deathlesstemp = (int)Halo_Process->ReadMem<bool>((LPVOID)(Cheats_address + HaloCE_lib::CheatsEx::Deathless_offset));
            if (Button_GetCheck(hControls[HDEATHLESS]) != deathlesstemp)
               Button_SetCheck(hControls[HDEATHLESS], deathlesstemp);

            //update infinite ammo value
            int infammotemp = (int)Halo_Process->ReadMem<bool>((LPVOID)(Cheats_address + HaloCE_lib::CheatsEx::Infinite_Ammo_offset));
            if (Button_GetCheck(hControls[HINFAMMO]) != infammotemp)
               Button_SetCheck(hControls[HINFAMMO], infammotemp);
         }

         if (ShowHud_ptr_address)
         {
            //update show hud value
            DWORD ShowHud_address = Halo_Process->ReadMem<DWORD>((LPVOID)ShowHud_ptr_address);
            if (ShowHud_address)
            {
               int hudtemp = (int)Halo_Process->ReadMem<bool>((LPVOID)ShowHud_address);
               if (Button_GetCheck(hControls[HSHOWHUD]) != hudtemp)
                  Button_SetCheck(hControls[HSHOWHUD], hudtemp);
            }
         }

         if (LetterBox_ptr_address)
         {
            //update letter box value
            DWORD LetterBox_address = Halo_Process->ReadMem<DWORD>((LPVOID)LetterBox_ptr_address);
            if (LetterBox_address)
            {
               LetterBox_address += 8;
               int lettemp = (int)Halo_Process->ReadMem<bool>((LPVOID)LetterBox_address);
               if (Button_GetCheck(hControls[HLETTERBOX]) != lettemp)
                  Button_SetCheck(hControls[HLETTERBOX], lettemp);
            }
         }

         //if (RiderEjection_address)
         //{
            //update rider ejction value
         //   int retemp = (int)Halo_Process->ReadMem<bool>((LPVOID)RiderEjection_address);
         //   if (Button_GetCheck(hControls[HEJECTION]) != retemp)
         //      Button_SetCheck(hControls[HEJECTION], retemp);

         if (cinematic_ptr)
         {
            //update marines hud value
            DWORD dwCinematic = Halo_Process->ReadMem<DWORD>((LPVOID)cinematic_ptr);
            BYTE cintemp = Halo_Process->ReadMem<BYTE>((LPVOID)(dwCinematic + 0x38));
            if (cintemp)
            {
               cintemp = Halo_Process->ReadMem<BYTE>((LPVOID)(dwCinematic + 0x24));
               if (cintemp == 2)
               {
                  float ftemp = Halo_Process->ReadMem<float>((LPVOID)(dwCinematic + 0x2C));

                  if (ftemp < 1.0f) cintemp = BST_INDETERMINATE;
                  else cintemp = BST_CHECKED;

                  if (Button_GetCheck(hControls[HMHUD]) != cintemp)
                     Button_SetCheck(hControls[HMHUD], cintemp);
               }
            }
            else
            {
               if (Button_GetCheck(hControls[HMHUD]) != cintemp)
                  Button_SetCheck(hControls[HMHUD], cintemp);
            }
         }

         //test for rpg_beta6_2 map///////////////////////////////////////
         if (str_cmpA(map_str, "rpg_beta6_2"))
         {
            //get address for this map
            if (!rpgb6_2_running)
            {
               control_enabled_change = true;
               rpgb6_2_running = true;
            }

            //status_lbl4->ForeColor = System::Drawing::Color::Green;

            if (HS_Global_Header_ptr_address)
            {
               //update alarm button
               bool alarmtemp = Halo_Process->ReadMem<bool>((LPVOID)(HS_Global_Header.FirstItem + HCE_Lib::rpg_beta6_2_hs_global::alarmed_offset));
               if (alarm_on != alarmtemp)
               {
                  alarm_on = alarmtemp;
                  if (alarm_on)
                  {
                     SetTpWindowText(hMainWin, hControls[HALARM], szAlarmOff);
                     //alarm_btn->ForeColor = System::Drawing::SystemColors::Highlight;
                  }
                  else
                  {
                     SetTpWindowText(hMainWin, hControls[HALARM], szAlarmOn);
                     //alarm_btn->ForeColor = System::Drawing::Color::Red;
                  }
               }

               //update setting
               BYTE settemp = Halo_Process->ReadMem<BYTE>((LPVOID)(HS_Global_Header.FirstItem + HCE_Lib::rpg_beta6_2_hs_global::setting_offset));
               if (C_Setting != settemp)
               {
                  C_Setting = settemp;
                  if (C_Setting >= 0 && C_Setting <= 2)
                     SendMessage(hControls[HSETTING], CB_SETCURSEL, C_Setting, 0);
               }

               //update lockdown timer
               short locktimtemp = Halo_Process->ReadMem<short>((LPVOID)(HS_Global_Header.FirstItem + HCE_Lib::rpg_beta6_2_hs_global::lock_timer_offset));
               if (Lock_sec != locktimtemp)
               {
                  Lock_sec = locktimtemp;
                  swprintf_s((wchar_t*)szBuffer, SZ_BUFFER_SIZE, L"%lu",(Lock_sec / 30));
                  SetWindowText(hControls[HBLD_TXTBX], (wchar_t*)szBuffer);
               }

               //update lockdown button
               bool locktemp = Halo_Process->ReadMem<bool>((LPVOID)(HS_Global_Header.FirstItem + HCE_Lib::rpg_beta6_2_hs_global::locked_offset));
               if (Locked != locktemp)
               {
                  Locked = locktemp;
                  if (!Locked)
                  {
                     SetTpWindowText(hMainWin, hControls[HBLD_ACT_BTN], szBaseLockAct);
                     //BLD_activate->ForeColor = System::Drawing::SystemColors::Highlight;
                  }
                  else
                  {
                     SetTpWindowText(hMainWin, hControls[HBLD_ACT_BTN], szBaseLockLocked);
                     //BLD_activate->ForeColor = System::Drawing::Color::Red;
                  }
               }

               //update fire halo button
               bool halotemp = Halo_Process->ReadMem<bool>((LPVOID)(HS_Global_Header.FirstItem + HCE_Lib::rpg_beta6_2_hs_global::nuked_offset));
               if (Nuked != halotemp)
               {
                  Nuked = halotemp;
                  if (!Nuked)
                  {
                     SetTpWindowText(hMainWin, hControls[HHALO_ACT_BTN], szHaloFire);
                     //halo_activate->ForeColor = System::Drawing::SystemColors::Highlight;
                  }
                  else
                  {
                     SetTpWindowText(hMainWin, hControls[HHALO_ACT_BTN], szHaloCoolDown);
                     //halo_activate->ForeColor = System::Drawing::Color::Red;
                  }
               }

               //update halo timer
               short halotimtemp = Halo_Process->ReadMem<short>((LPVOID)(HS_Global_Header.FirstItem + HCE_Lib::rpg_beta6_2_hs_global::boom_timer_offset));
               if (Halo_sec != halotimtemp)
               {
                  Halo_sec = halotimtemp;
                  swprintf_s((wchar_t*)szBuffer, SZ_BUFFER_SIZE, L"%lu",(Halo_sec / 30));
                  SetWindowText(hControls[HHALO_TXTBX], (wchar_t*)szBuffer);
               }
            }
         }
         else
         {
            rpgb6_2_running = false;
            //status_lbl4->ForeColor = System::Drawing::Color::Red;
         }

         HWND hMap = hControls[HMAP_STATUS];
         GetWindowTextA(hMap, (LPSTR)BufferA, 32);
         //only change if its different
         if (!str_cmpA((char*)BufferA, (char*)map_str))
         {
            SetWindowTextA(hMap, map_str);
            RedrawTpWindow(hMainWin, hMap);
            control_enabled_change = true;
         }
      }

      if (control_enabled_change)
      {
         //server status label
         EnableTpWindow(hMainWin, hControls[HSERVER_STATUS], running_gt != not_running && ServerType_address);
         //dev button
         EnableTpWindow(hMainWin, hControls[HDEV], running_gt == HCE && Dev_enabled_address);
         ShowTpWindow(hMainWin, hControls[HDEV], running_gt != Halo);
         //console button
         EnableTpWindow(hMainWin, hControls[HCONSOLE], running_gt == Halo && Console_enabled_address);
         ShowTpWindow(hMainWin, hControls[HCONSOLE], running_gt == Halo);
         //Deathless chkBx
         EnableTpWindow(hMainWin, hControls[HDEATHLESS], running_sv_t != client && running_gt != not_running);
         //infammo chkBx
         EnableTpWindow(hMainWin, hControls[HINFAMMO], running_sv_t != client && running_gt != not_running);
         //showhud chkBx
         EnableTpWindow(hMainWin, hControls[HSHOWHUD], running_gt != not_running && ShowHud_ptr_address);
         //letterbox chkBx
         EnableTpWindow(hMainWin, hControls[HLETTERBOX], running_gt != not_running && LetterBox_ptr_address);
         //ejection chkBx
         //EnableTpWindow(hMainWin, hControls[HEJECTION], running_sv_t != client && running_gt != not_running && RiderEjection_address);
         //marines hud
         BOOL valid_address = FALSE;
         if (pMV_chkBx_CheckedChanged) valid_address = true;
         else if (cse_set_video_func_address) valid_address = true;

         EnableTpWindow(hMainWin, hControls[HMHUD], running_gt != not_running && valid_address);

         //rpg_beta6_2 functions
         //setting combo box
         EnableWindow(hControls[HSETTING], running_sv_t == host && rpgb6_2_running && HS_Global_Header_ptr_address);
         //alarm button
         EnableTpWindow(hMainWin, hControls[HALARM], running_sv_t == host && rpgb6_2_running && Device_Groups_Header_ptr_address);
         //base lockdown label
         EnableTpWindow(hMainWin, hControls[HBLD_LBL], running_sv_t == host && rpgb6_2_running);
         //base lockdown timer textbox
         EnableWindow(hControls[HBLD_TXTBX], running_sv_t == host && rpgb6_2_running && HS_Global_Header_ptr_address);
         //base lockdown set button
         EnableTpWindow(hMainWin, hControls[HBLD_SET_BTN], running_sv_t == host && rpgb6_2_running && HS_Global_Header_ptr_address);
         //base lockdown activate button
         EnableTpWindow(hMainWin, hControls[HBLD_ACT_BTN], running_sv_t == host && rpgb6_2_running && Device_Groups_Header_ptr_address);
         //halo label
         EnableTpWindow(hMainWin, hControls[HHALO_LBL], running_sv_t == host && rpgb6_2_running);
         //halo timer textbox
         EnableWindow(hControls[HHALO_TXTBX], running_sv_t == host && rpgb6_2_running && HS_Global_Header_ptr_address);
         //halo set button
         EnableTpWindow(hMainWin, hControls[HHALO_SET_BTN], running_sv_t == host && rpgb6_2_running && HS_Global_Header_ptr_address);
         //halo activate button
         EnableTpWindow(hMainWin, hControls[HHALO_ACT_BTN], running_sv_t == host && rpgb6_2_running && Device_Groups_Header_ptr_address);

         control_enabled_change = false;
      }

      for (int i = 0; i < 14 && !exiting; i++)
         Sleep(50);
   }

   return TRUE;
}