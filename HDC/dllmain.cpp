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
	File:    dllmain.cpp
	Project: HDC
	Author:  Jesus7Freak
	Date:    11/22/2011
	Game:    Halo and Halo Custom Edition
	Version: all
*********************************************************************************/
// dllmain.cpp : Defines the entry point for the DLL application.
#include "dllmain.h"

bool exit_thread = false,
   rpgb6_2_running = false,
   *Console_chck = NULL;
   
char *Current_Map_Str = NULL;
      
wchar_t *LocationsFilePath = NULL,
   *Locations_File_Name = L"locations.bin";

BYTE *Dev_enabled = NULL,
   *Console_enabled = NULL,
   *ShowHud = NULL,
   *LetterBox = NULL,
   *RiderEjection = NULL;

short *ServerType = NULL,
   *Player0_index = NULL;
      
BOOL DestroyObjsEnabled = FALSE;

DWORD Current_Map_address = NULL,
   Rasterizer_address = NULL,
   EngineDrawText_address = NULL,
   Console_func_address = NULL,
   MsgInfo_ptr_address = NULL,
   ServerInfo_ptr_address = NULL,
   PrepMsg_func_address = NULL,
   SendMsgToPlayer_func_address = NULL,
   SendMsgToAll_func_address = NULL,
   Console_hook_address = NULL,
   Rcon_hook_address = NULL,
   C_Buffers_address = NULL,
   C_BuffersIndex_address = NULL,
   Server_chat_hook_address = NULL,
   PlayerCheck_Func_address = NULL,
   rapid_fire_hook_address = NULL,
   time_freeze_hook_address = NULL,
   grav_boots_hook_address = NULL,
   vehicle_ntr_hook_address = NULL,
   UnitExitVehicle_func_address = NULL,
   SpawnHog_func_address = NULL,
   ObjTagList_ptr_address = NULL,
   SpawnObjAtPlayer0_func_address = NULL,
   CreateObj_func_address = NULL,
   DestroyObj_func_address = NULL,
   ActiveCamo_func_address = NULL,
   PlayerDeath_func_address = NULL,
   sv_kick_func_address = NULL,
   sv_ban_func_address = NULL,
   cse_set_video_func_address = NULL,
   cinematic_ptr = NULL,
   console_tablist_hook_address = NULL,
   console_tablist_hookfunc_address = NULL,
   ctl_array_index_address = NULL,
   ctl_array_size_address = NULL,
   ctl_filter_str_ptr_address = NULL,
   TempAdmin[16] = {0xFFFFFFFF};

float *game_speed = NULL;

HALO_TYPE running_gt = (HALO_TYPE)0;

HaloCE_lib::CHEATS *cheats = NULL;
HaloCE_lib::RASTERIZER *rasterizer = NULL;

HaloCE_lib::DATA_HEADER **Players_ptr = NULL, 
   **Device_Groups_ptr = NULL, 
   **Object_ptr = NULL,
   **HS_Globals_ptr = NULL;

HaloCE_lib::STATIC_PLAYER *players = NULL;
HaloCE_lib::OBJECT_TABLE_ARRAY *objects = NULL;

HANDLE hHDC_thread = NULL; 

DLL_ADDRESSES dll_addresses = 
{
   {'d','l','l','_','s','t','_','a','d','d','r','s'},
   NULL,
   (CMDsLib::CMD_SCKEYS**)&CMDsLib::halo_cmd_keys,
   (CMDsLib::CMD_SCKEYS**)&CMDsLib::rpg_beta6_2_cmd_keys,
   &CMDsLib::halo_commands.Enable_Shrtcts,
   &CMDsLib::rpgbeta6_2_commands.Enable_Shrtcts,
   MV_chkBx_CheckedChanged,
   NULL,
   (WORD*)&running_gt,
   true
};

std::vector<MAPS> maps_tele_sites;

bool IsKeyDown(int keyCode)
{
   short result = ::GetAsyncKeyState(keyCode);
   return (result < 0);
}

bool str_cmpAW(char *str1, wchar_t *wstr2, int length)
{
   bool match = true;
   for (int i = 0; i < length; i++)
   {
      if (str1[i] != (char)wstr2[i])
      {
         match = false;
         break;
      }
   }
   return match;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason, LPVOID lpReserved)
{
	switch (ul_reason)
	{
	case DLL_PROCESS_ATTACH:
	   hHDC_thread = ::CreateThread(0, 0, HDC_thread, hModule, 0, 0);
	   break;
	case DLL_THREAD_ATTACH:
	   break;
	case DLL_THREAD_DETACH:
	   break;
	case DLL_PROCESS_DETACH:
	   exit_thread = true;
	   
	   //wait for HDC_thread to exit if valid
	   if (hHDC_thread)
	   {
	      WaitForSingleObject(hHDC_thread, 50);
         CloseHandle(hHDC_thread);
      }
      
	   //detach hooks
	   DWORD dwOldProtect = NULL;
	   if (Console_hook_address)
	   {
         ::VirtualProtect((LPVOID)Console_hook_address, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &dwOldProtect);
         *(DWORD*)Console_hook_address = Console_func_address - (Console_hook_address + 4);
         ::VirtualProtect((LPVOID)Console_hook_address, sizeof(DWORD), dwOldProtect, &dwOldProtect);
      }
      
      if (Console_hook_address && Rcon_hook_address)
      {
         ::VirtualProtect((LPVOID)Rcon_hook_address, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &dwOldProtect);
         *(DWORD*)Rcon_hook_address = Console_func_address - (Rcon_hook_address + 4);
         ::VirtualProtect((LPVOID)Rcon_hook_address, sizeof(DWORD), dwOldProtect, &dwOldProtect);
      }
   
      if (Server_chat_hook_address && PlayerCheck_Func_address)
      {
         ::VirtualProtect((LPVOID)Server_chat_hook_address, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &dwOldProtect);
         *(DWORD*)Server_chat_hook_address = PlayerCheck_Func_address - (Server_chat_hook_address + 4);
         ::VirtualProtect((LPVOID)Server_chat_hook_address, sizeof(DWORD), dwOldProtect, &dwOldProtect);
      }
      
      if (console_tablist_hook_address && console_tablist_hookfunc_address)
      {
         ::VirtualProtect((LPVOID)console_tablist_hook_address, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &dwOldProtect);
         *(DWORD*)console_tablist_hook_address = console_tablist_hookfunc_address - (console_tablist_hook_address + 4);
         ::VirtualProtect((LPVOID)console_tablist_hook_address, sizeof(DWORD), dwOldProtect, &dwOldProtect);
      }
		break;
	}
	return TRUE;
}

DWORD WINAPI HDC_thread(LPVOID hModule)
{
   DWORD base_address = NULL,
         scan_size = NULL;
         
   bool display_txt = true;
    
   DWORD hHalo = (DWORD)::GetModuleHandleA(NULL);
   IMAGE_DOS_HEADER *IDH = (IMAGE_DOS_HEADER*)hHalo;
   IMAGE_NT_HEADERS *INH = (IMAGE_NT_HEADERS*)(hHalo + IDH->e_lfanew);
   base_address = (DWORD)hHalo + INH->OptionalHeader.BaseOfCode;
   
   IMAGE_SECTION_HEADER *text_section = (IMAGE_SECTION_HEADER*)((DWORD)hHalo + IDH->e_lfanew + sizeof(INH->Signature) + sizeof(INH->FileHeader) + INH->FileHeader.SizeOfOptionalHeader);
   scan_size = text_section->Misc.VirtualSize;
   
   //scan_size = INH->OptionalHeader.SizeOfCode;
   
   //wait for app to get and write values to dll
   while (!running_gt)
      ::Sleep(10);
   
   //find patterns
   //halo functions
   EngineDrawText_address = CurrentProcess::FindMemPattern(
       base_address,
       scan_size,
       HaloCE_lib::EngineDrawText_func_addr_sig);
   
   DWORD ch_hook = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Console_hook_addr_sig);
      
   if (!ch_hook)
      HaloDrawText("Console_hook_addr_sig failed", 255, 255, 0, 0);
   else
   {
      Console_hook_address = ch_hook;
      Console_func_address = (ch_hook + 4) + *(DWORD*)ch_hook;
   }
   
   DWORD rh_hook = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Rcon_hook_addr_sig);
      
   if (!rh_hook)
      HaloDrawText("Rcon_hook_addr_sig failed", 255, 255, 0, 0);
   else
      Rcon_hook_address = rh_hook;
   
   
   DWORD sch_hook = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Server_Chat_hook_addr_sig);
      
   if (!sch_hook)
      HaloDrawText("Server_Chat_hook_addr_sig failed", 255, 255, 0, 0);
   else
   {
      Server_chat_hook_address = sch_hook;
      PlayerCheck_Func_address = (sch_hook + 4) + *(DWORD*)sch_hook;
      
      sch_hook += 0x2E;
      MsgInfo_ptr_address = *(DWORD*)sch_hook;
      
      sch_hook += 9;
      PrepMsg_func_address = (sch_hook + 4) + *(DWORD*)sch_hook;
      
      sch_hook += 6;
      ServerInfo_ptr_address = *(DWORD*)sch_hook;
   }
   
 
   DWORD scmta_faster_scan_address = sch_hook;
   DWORD scmta_func = CurrentProcess::FindMemPattern(
      scmta_faster_scan_address,
      scan_size - (scmta_faster_scan_address - base_address),
      HaloCE_lib::SendChatMsgToAll_func_addr_sig);
      
   if (!scmta_func)
      HaloDrawText("SendChatMsgToAll_func_addr_sig failed", 255, 255, 0, 0);
   else
      SendMsgToAll_func_address = (scmta_func + 4) + *(DWORD*)scmta_func;
      
      
   DWORD scmtp_func = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::SendChatMsgToPlayer_func_addr_sig);
      
   if (!scmtp_func)
      HaloDrawText("SendChatMsgToPlayer_func_addr_sig failed", 255, 255, 0, 0);
   else
      SendMsgToPlayer_func_address = scmtp_func;
   

   DWORD ctvh_hook = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::console_tablist_vars_hook_addr_sig);
      
   if (!ctvh_hook)
      HaloDrawText("console_tablist_vars_hook_addr_sig failed", 255, 255, 0, 0);
   else
   {
      ctl_array_index_address = *(DWORD*)ctvh_hook;
      ctl_array_size_address = *(DWORD*)(ctvh_hook + 7);
      ctl_filter_str_ptr_address = *(DWORD*)(ctvh_hook + 18);
   }
   
   DWORD cth_faster_scan_address = ctvh_hook;
   DWORD cth_hook = CurrentProcess::FindMemPattern(
      cth_faster_scan_address,
      scan_size - (cth_faster_scan_address - base_address),
      HaloCE_lib::console_tablist_hook_addr_sig);
      
   if (!cth_hook)
      HaloDrawText("console_tablist_hook_addr_sig failed", 255, 255, 0, 0);
   else
   {
      console_tablist_hook_address = cth_hook;
      console_tablist_hookfunc_address = (cth_hook + 4) + *(DWORD*)cth_hook;
   }      
   
   DWORD uev_func = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::UnitExitVehicle_func_addr_sig);
   
   if (!uev_func)
      HaloDrawText("UnitExitVehicle_func_addr_sig failed", 255, 255, 0, 0);
   else
      UnitExitVehicle_func_address = uev_func;
   
   
   DWORD sh_func = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::SpawnHog_func_addr_sig);
   
   if (!sh_func)
      HaloDrawText("SpawnHog_func_addr_sig failed", 255, 255, 0, 0);
   else
   {
      SpawnHog_func_address = sh_func;
      sh_func += 1;
      ObjTagList_ptr_address = *(DWORD*)sh_func;
   }
   
   
   DWORD soap0_func = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::SpawnObjAtPlayer0_func_addr_sig);
      
   if (!soap0_func)
      HaloDrawText("SpawnObjAtPlayer0_func_addr_sig failed", 255, 255, 0, 0);
   else
   {
      SpawnObjAtPlayer0_func_address = soap0_func;
      soap0_func += 7; 
      soap0_func = ((soap0_func + 4) + *(int*)soap0_func);//get player 0 func
      Players_ptr = *(HaloCE_lib::DATA_HEADER ***)(soap0_func + 4);
      Player0_index = (short*)(soap0_func + 0x1F);
      
      bool *P_Initialized = &(*Players_ptr)->Initialized;
      while(!*P_Initialized)
         Sleep(10);
         
      players = (HaloCE_lib::STATIC_PLAYER*)(*Players_ptr)->FirstItem;
   }
      
   
   DWORD co_func = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::CreateObject_func_addr_sig);
   
   if (!co_func)
      HaloDrawText("CreateObject_func_addr_sig failed", 255, 255, 0, 0);
   else
      CreateObj_func_address = co_func;
      
      
   DWORD sk_func = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::sv_kick_func_addr_sig);
   
   if (!sk_func)
      HaloDrawText("sv_kick_func_addr_sig failed", 255, 255, 0, 0);
   else
   {
      sv_kick_func_address = sk_func;
      sk_func += 3;
      ServerType = *(short**)sk_func;
   }
   
   
   DWORD sb_func = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::sv_ban_func_addr_sig);
   
   if (!sb_func)
      HaloDrawText("sv_ban_func_addr_sig failed", 255, 255, 0, 0);
   else
      sv_ban_func_address = sb_func;
   
   
   //halo DATA_HEADER's
   DWORD dg_ptr = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Device_Groups_ptr_sig);
   
   if (!dg_ptr)
      HaloDrawText("Device_Groups_ptr_sig failed", 255, 255, 0, 0);
   else
      Device_Groups_ptr = *(HaloCE_lib::DATA_HEADER***)dg_ptr;
   
   
   DWORD do_func = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::DestroyObj_func_addr_sig);
      
   if (!do_func)
      HaloDrawText("DestroyObj_func_addr_sig failed", 255, 255, 0, 0);
   else
   {
      DestroyObj_func_address = do_func;
      do_func += 2;
      Object_ptr = *(HaloCE_lib::DATA_HEADER***)do_func;
      
      bool *O_Initialized = &(*Object_ptr)->Initialized;
      while(!*O_Initialized)
         Sleep(10);
         
      objects = (HaloCE_lib::OBJECT_TABLE_ARRAY*)(*Object_ptr)->FirstItem;
   }
   
   
   DWORD ac_func = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::ActiveCamo_func_addr_sig);
   
   if (!ac_func)
      HaloDrawText("ActiveCamo_func_addr_sig failed", 255, 255, 0, 0);
   else
      ActiveCamo_func_address = ac_func;
      
      
   DWORD pd_func = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::PlayerDeath_func_addr_sig);
   
   if (!pd_func)
      HaloDrawText("PlayerDeath_func_addr_sig failed", 255, 255, 0, 0);
   else
      PlayerDeath_func_address = pd_func;
      
   
   DWORD csesv_func = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::cse_set_video_func_addr_sig);
      
   if (!csesv_func)
      HaloDrawText("cse_set_video_func_addr_sig failed", 255, 255, 0, 0);
   else
   {
      cse_set_video_func_address = csesv_func;
      cinematic_ptr = *(DWORD*)(csesv_func + 2);
   }

   
   DWORD hsg_ptr = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::HS_Globals_ptr_sig);
      
   if (!hsg_ptr)
      HaloDrawText("HS_Globals_ptr_sig failed", 255, 255, 0, 0);
   else
      HS_Globals_ptr = *(HaloCE_lib::DATA_HEADER***)hsg_ptr;


   //global vars
   if (running_gt == haloce)
   {
      DWORD dev_ptr = CurrentProcess::FindMemPattern(
         base_address,
         scan_size,
         HCE_Lib::Dev_addr_sig);
         
      if (!dev_ptr)
         HaloDrawText("Dev_addr_sig failed", 255, 255, 0, 0);
      else
         Dev_enabled = *(BYTE**)dev_ptr;
   }


   DWORD console_ptr = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Console_addr_sig);

   if (!console_ptr)
      HaloDrawText("Console_addr_sig failed", 255, 255, 0, 0);
   else
   {
      console_ptr = *(DWORD*)console_ptr;
      
      Console_chck = (bool*)console_ptr;
      Console_enabled = (BYTE*)(console_ptr + HaloCE_lib::Console::Enabled_offset);
      C_Buffers_address = console_ptr + HaloCE_lib::Console::C_Buffers_offset;
      C_BuffersIndex_address = console_ptr + HaloCE_lib::Console::C_Buffers_index_offset;
   }
   
   
   DWORD cma_ptr = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Current_map_addr_sig);
   
   if (!cma_ptr)
      HaloDrawText("Current_map_addr_sig failed", 255, 255, 0, 0);
   else
      Current_Map_Str = (char*)*(DWORD*)cma_ptr;
      

   DWORD cheats_ptr = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Cheats_addr_sig);
   
   if (!cheats_ptr)
      HaloDrawText("Cheats_addr_sig failed", 255, 255, 0, 0);
   else
      cheats = *(HaloCE_lib::CHEATS**)cheats_ptr;
                        
      
   DWORD sh_ptr_ptr = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Show_Hud_ptr_addr_sig);
   
   if (!sh_ptr_ptr)
      HaloDrawText("Show_Hud_ptr_addr_sig failed", 255, 255, 0, 0);
   else
   {
      sh_ptr_ptr = *(DWORD*)sh_ptr_ptr;
      do 
      {
         Sleep(10);//wait for valid address
         ShowHud = *(BYTE**)sh_ptr_ptr;
      }while(!ShowHud);
   }
           
                         
   DWORD lb_ptr_ptr = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::letterbox_ptr_addr_sig);

   if (!lb_ptr_ptr)
      HaloDrawText("letterbox_ptr_addr_sig failed", 255, 255, 0, 0);
   else
      LetterBox = **(BYTE***)lb_ptr_ptr + 8;
      

   DWORD re_ptr = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Rider_Eject_addr_sig);
   
   if (!re_ptr)
      HaloDrawText("Rider_Eject_addr_sig failed", 255, 255, 0, 0);
   else
      RiderEjection = *(BYTE**)re_ptr;
      
      
   DWORD rast_ptr = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Rasterizer_addr_sig);
   
   if (!rast_ptr)
      HaloDrawText("Rasterizer_addr_sig failed", 255, 255, 0, 0);
   else
      rasterizer = *(HaloCE_lib::RASTERIZER**)rast_ptr;
   
    
   DWORD gs_ptr_ptr = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Game_Speed_ptr_addr_sig);
   
   if (!gs_ptr_ptr)
      HaloDrawText("Game_Speed_ptr_addr_sig failed", 255, 255, 0, 0);
   else
      game_speed = (float*)(**(DWORD**)gs_ptr_ptr + 0x18);
      
                          
   DWORD rf_ptr = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Rapid_Fire_addr_sig);
   
   if (!rf_ptr)
      HaloDrawText("Rapid_Fire_addr_sig failed", 255, 255, 0, 0);
   else
      rapid_fire_hook_address = rf_ptr;
      
      
   DWORD tf_ptr = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Time_Freeze_addr_sig);
      
   if (!tf_ptr)
      HaloDrawText("Time_Freeze_addr_sig failed", 255, 255, 0, 0);
   else
      time_freeze_hook_address = tf_ptr;
      
      
   DWORD gb_ptr = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Grav_Boots_addr_sig);
   
   if (!gb_ptr)
      HaloDrawText("Grav_Boots_addr_sig failed", 255, 255, 0, 0);
   else
      grav_boots_hook_address = gb_ptr;
      
         
   DWORD vntr_ptr = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Vehicle_NTR_addr_sig);
   
   if (!vntr_ptr)
      HaloDrawText("Vehicle_NTR_addr_sig failed", 255, 255, 0, 0);
   else
      vehicle_ntr_hook_address = vntr_ptr;
      
      
   //c string length
   wchar_t *CD = dll_addresses.CurrentDir;
   if (CD)
   {
      int length = 0; while(CD[length]) length++;
      
      int loc_name_length = 0; while(Locations_File_Name[loc_name_length]) loc_name_length++;
            
      LocationsFilePath = new wchar_t[(length + loc_name_length + 1)];
      for (int i = 0; i < length; i++)
         LocationsFilePath[i] = dll_addresses.CurrentDir[i];
      
      CurrentProcess::FreeMemory((LPVOID)dll_addresses.CurrentDir);
      dll_addresses.CurrentDir = NULL;
      
      LocationsFilePath[length] = L'\\';
      
      wchar_t *copy_to_str = &LocationsFilePath[length + 1];
      for (int i = 0; i <= loc_name_length; i++)
         copy_to_str[i] = Locations_File_Name[i];
   }
   
   //write hooks
   DWORD dwOldProtect = NULL;
   if (Console_hook_address)
   {
      ::VirtualProtect((LPVOID)Console_hook_address, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &dwOldProtect);
      *(DWORD*)Console_hook_address = (DWORD)&ConsoleHook - (Console_hook_address + 4);
      ::VirtualProtect((LPVOID)Console_hook_address, sizeof(DWORD), dwOldProtect, &dwOldProtect);
   }
   
   if (Rcon_hook_address)
   {
      ::VirtualProtect((LPVOID)Rcon_hook_address, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &dwOldProtect);
      *(DWORD*)Rcon_hook_address = (DWORD)&ConsoleHook - (Rcon_hook_address + 4);
      ::VirtualProtect((LPVOID)Rcon_hook_address, sizeof(DWORD), dwOldProtect, &dwOldProtect);
   }
   
   if (Server_chat_hook_address)
   {
      ::VirtualProtect((LPVOID)Server_chat_hook_address, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &dwOldProtect);
      *(DWORD*)Server_chat_hook_address = (DWORD)&ChatHook - (Server_chat_hook_address + 4);
      ::VirtualProtect((LPVOID)Server_chat_hook_address, sizeof(DWORD), dwOldProtect, &dwOldProtect);
   }

   if (console_tablist_hook_address)
   {
      ::VirtualProtect((LPVOID)console_tablist_hook_address, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &dwOldProtect);
      *(DWORD*)console_tablist_hook_address = (DWORD)&ConsoleTablistHook - (console_tablist_hook_address + 4);
      ::VirtualProtect((LPVOID)console_tablist_hook_address, sizeof(DWORD), dwOldProtect, &dwOldProtect);
   }
   
   //main loop
   while(!exit_thread)
	{
	   if (dll_addresses.locations_changed)
	   {
	      //free memory
	      if (maps_tele_sites.size())
	      {
	         for (unsigned int i = 0; i < maps_tele_sites.size(); i++)
	            maps_tele_sites[i].teleport_locations.clear();
	            
	         maps_tele_sites.clear();
	      }
	      
	      GetLocationsFromFile(LocationsFilePath, &maps_tele_sites);
	      dll_addresses.locations_changed = false;
	   }
	   
	   if (Current_Map_Str)
	   {
	      if (str_cmpA("ui", Current_Map_Str) && display_txt)
         {
            HaloDrawText("Halo Dev Controls - Jesus7Freak", 255, 255, 255, 255);
            display_txt = false;
         }
         else if (!str_cmpA("ui", Current_Map_Str))
            display_txt = true;
            
         if (str_cmpA(Current_Map_Str, "rpg_beta6_2"))
            rpgb6_2_running = true;
         else
            rpgb6_2_running = false;
      }
      
      //set admin to zero when player leaves
      if (Players_ptr && *Players_ptr)
      {
         for (int i = 1; i < 16; i++)
         {
            if (!players[i].PlayerID) TempAdmin[i] = 0;
         }
      }

      CheckCmdScKeys();
      DestroyObjsCheck();
      
      //so it doesn't slow halo down
	   //for (int i = 0; i < 10 && !exit_thread; i++)
      //   Sleep(5);
	}
	
	delete[] LocationsFilePath;
   return 1;
}

void CheckCmdScKeys()
{
   //only check for shortcuts if halo is in focus
   if (GetForegroundWindow() != dll_addresses.hHaloWin) return;
   
   //keyboard shortcuts
   int cmd_id = -1; BYTE value;
   for (int cmd_group_i = 1; cmd_id == -1 && cmd_group_i < CMD_SET_SIZE; cmd_group_i++)
   {
      CMDsLib::COMMANDS *cmd_group = CMDsLib::all_commands[cmd_group_i];
      if (!cmd_group->Enable_Shrtcts) continue;
      
      CMDsLib::CMD_SCKEYS *cmd_keys = cmd_group->cmd_keys;
      int group_size = cmd_group->size;
      
      for (int i = 0; !exit_thread && i < group_size; i++)
      {
         //toggle cmd on/off
         if (IsKeyDown(cmd_keys[i].toggle_key))
         {
            cmd_id = i + (100 * cmd_group_i);
            value = TOGGLE;
            break;
         }
         //turn cmd on
         else if (IsKeyDown(cmd_keys[i].on_key))
         {
            cmd_id = i + (100 * cmd_group_i);
            value = true;
            break;
         }
         //turn cmd off
         else if (IsKeyDown(cmd_keys[i].off_key))
         {
            cmd_id = i + (100 * cmd_group_i);
            value = false;
            break;
         }
      }
   }
      
   switch (cmd_id)
   {
      case 103://Console
      {
         if (value == TOGGLE)
            *Console_enabled = !*Dev_enabled;
         else
            *Console_enabled = value;
         break;
      }
      case 104://Dev Mode
      {
         if (running_gt != haloce) break;
         
         if (value == TOGGLE)
            *Dev_enabled = !*Console_enabled;
         else
            *Dev_enabled = value;
         break;
      }
      case 105://Deathless
      {
         if (value == TOGGLE)
            cheats->Deathless = !cheats->Deathless;
         else
            cheats->Deathless = value;
         break;
      }
      case 106://Infinite Ammo
      {
         if (value == TOGGLE)
            cheats->Infinite_Ammo = !cheats->Infinite_Ammo;
         else
            cheats->Infinite_Ammo = value;
         break;
      }
      case 107://Bottomless Clip
      {
         if (value == TOGGLE)
            cheats->Bottomless_Clip = !cheats->Bottomless_Clip;
         else
            cheats->Bottomless_Clip = value;
         break;
      }
      case 108://Show Hud
      {
         if (value == TOGGLE)
            *ShowHud = !*ShowHud;
         else
            *ShowHud = value;
         break;
      }
      case 109://LetterBox
      {
         if (value == TOGGLE)
            *LetterBox = !*LetterBox;
         else
            *LetterBox = value;
         break;
      }
      case 110://Rider Ejection
      {
         if (value == TOGGLE)
            *RiderEjection = !*RiderEjection;
         else
            *RiderEjection = value;
         break;
      }
      case 111://Omnipotent
      {
         if (value == TOGGLE)
            cheats->Omnipotent = !cheats->Omnipotent;
         else
            cheats->Omnipotent = value;
         break;
      }
      case 112://JetPack
      {
         if (value == TOGGLE)
            cheats->JetPack = !cheats->JetPack;
         else
            cheats->JetPack = value;
         break;
      }
      case 113://Bump Possession
      {
         if (value == TOGGLE)
            cheats->Bmp_Possession = !cheats->Bmp_Possession;
         else
            cheats->Bmp_Possession = value;
         break;
      }
      case 114://Super Jump
      {
         if (value == TOGGLE)
            cheats->Super_jump = !cheats->Super_jump;
         else
            cheats->Super_jump = value;
         break;
      }
      case 115://Medusa
      {
         if (value == TOGGLE)
            cheats->Medusa = !cheats->Medusa;
         else
            cheats->Medusa = value;
         break;
      }
      case 116://Reflexive Damage
      {
         if (value == TOGGLE)
            cheats->Reflexive_damage = !cheats->Reflexive_damage;
         else
            cheats->Reflexive_damage = value;
         break;
      }
      case 117://Xbox Controller
      {
         if (value == TOGGLE)
            cheats->Controller = !cheats->Controller;
         else
            cheats->Controller = value;
         break;
      }
      case 118://WireFrame
      {
         if (value == TOGGLE)
            rasterizer->WireFrame = !rasterizer->WireFrame;
         else
            rasterizer->WireFrame = value;
         break;
      }
      case 119://Fog
      {
         if (value == TOGGLE)
            rasterizer->FogAtmosphere = !rasterizer->FogAtmosphere;
         else
            rasterizer->FogAtmosphere = value;
         break;
      }
      case 120://FogPlane
      {
         if (value == TOGGLE)
            rasterizer->FogPlane = !rasterizer->FogPlane;
         else
            rasterizer->FogPlane = value;
         break;
      }
      case 121://FPS
      {
         if (value == TOGGLE)
            rasterizer->FPS = !rasterizer->FPS;
         else
            rasterizer->FPS = value;
         break;
      }
      case 123://Rapid Fire
         ckbx_rapid_fire_CheckedChanged(value);
         break;
      case 124://Time Freeze
         ckbx_time_freeze_CheckedChanged(value);
         break;
      case 125://Gravity Boots
         ckbx_grav_boots_CheckedChanged(value);
         break;
      case 126://Vehicle NTR
         ckbx_vehicle_ntr_CheckedChanged(value);
         break;
      case 127://Marines HUD
      {
         if (cinematic_ptr)
         {
            if (value == TOGGLE)
            {
               DWORD Cinematic = *(DWORD*)cinematic_ptr;
               if (*(BYTE*)(Cinematic + 0x38))
               {
                  if (*(WORD*)(Cinematic + 0x24) == 2)
                  {
                     if (*(float*)(Cinematic + 0x2C) < 1.0f)
                        value = 0;
                  }
               }
               else
                  value = 1;
            }
            
            MV_chkBx_CheckedChanged(value);
         }
         break;
      }
      
      case 200://day Setting toggle
      case 201://rain
      case 202://night
      {
         if (!rpgb6_2_running) break;
   
         short *setting = (short*)((*HS_Globals_ptr)->FirstItem 
            + HCE_Lib::rpg_beta6_2_hs_global::setting_offset);
            
         if (value == TOGGLE)
         {
            if (*setting != 2)
               *setting += 1;
            else
               *setting = 0;
         }
         break;
      }
      case 203://Air Base Alarm
      {
         if (!rpgb6_2_running) break;
         
         bool *alarmed = (bool*)((*HS_Globals_ptr)->FirstItem 
            + HCE_Lib::rpg_beta6_2_hs_global::alarmed_offset);
         
         bool *alarm_control_2 = (bool*)((*Device_Groups_ptr)->FirstItem 
            + HCE_Lib::rpg_beta6_2_device_groups::alarm_control_2_offset);
            
         if (value == TOGGLE)
            *alarm_control_2 = true;
         else if (value)
         {
            if (!*alarmed)
               *alarm_control_2 = true;
         }
         else if (!value)
         {
            if (*alarmed)
               *alarm_control_2 = true;
         }
         break;
      }
      case 204://Air Base LockDown
      {
         if (!rpgb6_2_running) break;
         
         bool *locked = (bool*)((*HS_Globals_ptr)->FirstItem 
            + HCE_Lib::rpg_beta6_2_hs_global::locked_offset);
         
         bool *lock_control = (bool*)((*Device_Groups_ptr)->FirstItem 
            + HCE_Lib::rpg_beta6_2_device_groups::lock_control_offset);
         
         if (value == TOGGLE)
         {
            if (!*locked)
               *lock_control = true;
            else
            {
               //Halo_Process.WriteMemBool(HCE_Lib.Locked, false);
            }
         }
         break;
      }
      case 205://Fire Halo
      {
         if (!rpgb6_2_running) break;
         
         bool *nuked = (bool*)((*HS_Globals_ptr)->FirstItem 
            + HCE_Lib::rpg_beta6_2_hs_global::nuked_offset);
         
         bool *boom_control = (bool*)((*Device_Groups_ptr)->FirstItem 
            + HCE_Lib::rpg_beta6_2_device_groups::boom_control_offset);
         
         if (value == TOGGLE)
         {
            if (!*nuked)
               *boom_control = true;
            else
            {
               //Halo_Process.WriteMemBool(HCE_Lib.Nuked, false);
            }
         }
         break;
      }
   }    
   
   if (cmd_id != -1) Sleep(200);
}

__declspec(noinline) void DestroyObjsCheck()
{
   if (!DestroyObjsEnabled) return;
   
   int NumOfObjs = (*Object_ptr)->NumOfItems;
   int ObjTag;
   //int valid_objs = 0;
   for (int obj_index = 0; obj_index < NumOfObjs; obj_index++)
   {
      HaloCE_lib::OBJECT_TABLE_ARRAY *obj_header = &objects[obj_index];
      ObjTag = obj_header->ObjectID;
      
      if (!ObjTag) continue;
      //else valid_objs++;
      
      //player object type is 0
      if (obj_header->ObjectType == 0) continue;
      
      if (DestroyObjsEnabled < 2)
         if (obj_header->ObjectType > 3) continue;
      
      HaloCE_lib::SPARTAN *pObject = (HaloCE_lib::SPARTAN*)obj_header->Object_ptr;
      if (!pObject) continue;
      
      int damage_player_index = pObject->DamageFromPlayer;
      if (damage_player_index == -1) continue;
      
      //create full obj tag
      ObjTag <<= 16;
      ObjTag |= obj_index;
      __asm
      {
         MOV EAX,ObjTag
         CALL DWORD PTR [DestroyObj_func_address]
      }
   }
   return;
}