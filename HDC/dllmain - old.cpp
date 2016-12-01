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
	File:    dllmain - old.cpp
	Project: HDC
	Author:  Jesus7Freak
	Date:    11/22/2011
	Game:    Halo and Halo Custom Edition
	Version: all
*********************************************************************************/
#include "targetver.h"
#include "HaloLib.h"
#include "cmds.h"
#include "tele_loc_resources.h"

using namespace ReadWriteMemoryTools;

#define LOCATIONS_FILE_NAME L"locations.bin"

struct CMD_SK_ENABLED
{
   BOOL halo_Enable_Shrtcts;
   BOOL rpgb62_Enable_Shrtcts;
};

struct DLL_ADDRESSES
{
   char find_me_str[12];//set to "dll_st_addrs"
   CMD_SCKEYS **halo_cmd_keys;
   CMD_SCKEYS **rpgb62_cmd_keys;
   CMD_SK_ENABLED *cmd_sk_enabled;
   wchar_t *CurrentDir;
   bool locations_changed;
};

CMD_SK_ENABLED dll_sk_enabled = {0};

CMD_DATA halo_cmd_data[HALO_CMDS_SIZE] =
{
   { NULL, false, false },//0
   { NULL, true, false },//1
   { NULL, false, true },//2
   { NULL, false, true },//3
   { NULL, false, false },//4
   { NULL, false, false },//5
   { NULL, false, false },//6
   { NULL, false, true },//7
   { NULL, false, true },//8
   { NULL, false, true },//9
   { NULL, false, true },//10
   { NULL, false, false },//11
   { NULL, false, true },//12
   { NULL, false, true },//13
   { NULL, false, false },//14
   { NULL, false, false },//15
   { NULL, false, false },//16
   { NULL, false, false },//17
   { NULL, false, false },//18
   { NULL, false, true },//19
   { NULL, false, false },//20
   { NULL, false, false },//21
   { NULL, false, false },//22
   { NULL, false, false }//23
};
CMD_STRS halo_cmd_strs[HALO_CMDS_SIZE] =
{
   {L"", L"/console 1;", L"/console 0;"},//0
   {L"", L"/dev 1;", L"/dev 0;"},//1
   {L"", L"/deathless 1;", L"/deathless 0;"},//2
   {L"", L"/infammo 1;", L"/infammo 0;"},//3
   {L"", L"/bottomless 1;", L"/bottomless 0;"},//4
   {L"", L"/hud 1;", L"/hud 0;"},//5
   {L"", L"/letterbox 1;", L"/letterbox 0;"},//6
   {L"", L"/ejection 1;", L"/ejection 0;"},//7
   {L"", L"/one shot 1;", L"/one shot 0;"},//8
   {L"", L"/no fall damage 1;", L"/no fall damage 0;"},//9
   {L"", L"/possess 1;", L"/possess 0;"},//10
   {L"", L"/super jump 1;", L"/super jump 0;"},//11
   {L"", L"/reflex damage 1;", L"/reflex damage 0;",},//12
   {L"", L"/medusa 1;", L"/medusa 0;"},//13
   {L"", L"/controller 1;", L"/controller 0;"},//14
   {L"", L"/wireframe 1;", L"/wireframe 0;"},//15
   {L"", L"/fog 1;", L"/fog 0;"},//16
   {L"", L"/fogplane 1;", L"/fogplane 0;"},//17
   {L"", L"/fps 1;", L"/fps 0;"},//18
   {L"/game_speed #;", L"", L""},//19
   {L"", L"/rapid_fire 1;", L"/rapid_fire 0;"},//20
   {L"", L"/time_freeze 1;", L"/time_freeze 0;"},//21
   {L"", L"/grav_boots 1;", L"/grav_boots 0;"},//22
   {L"", L"/vehicle_str 1;", L"/vehicle_str 0;"}//23
};
//data that can be written to file
CMD_SCKEYS halo_cmd_keys[HALO_CMDS_SIZE] = 
{
   {0, 0, 0},//0
   {0, 33, 34},//1
   {97, 0, 0},//2
   {98, 0, 0},//3
   {0, 0, 0},//4
   {99, 0, 0},//5
   {100, 0, 0},//6
   {101, 0, 0},//7
   {0, 0, 0},//8
   {0, 0, 0},//9
   {0, 0, 0},//10
   {0, 0, 0},//11
   {0, 0, 0},//12
   {0, 0, 0},//13
   {0, 0, 0},//14
   {0, 0, 0},//15
   {0, 0, 0},//16
   {0, 0, 0},//17
   {0, 0, 0},//18
   {-1, -1, -1},//19
   {111, 0, 0},//20
   {106, 0, 0},//21
   {109, 0, 0},//22
   {0, 0, 0}//23
};

CMD_DATA rpg_beta6_2_cmd_data[RPGB_CMDS_SIZE] =
{
   { NULL, true, true },//0
   { NULL, true, true },//1
   { NULL, true, true },//2
   { NULL, true, true },//3
   { NULL, true, true },//4
   { NULL, true, true }//5
};
CMD_STRS rpg_beta6_2_cmd_strs[RPGB_CMDS_SIZE] =
{
   {L"/day;", L"/rain;", L"/night;"},//0
   {L"", L"/alarm 1;", L"/alarm 0;"},//1
   {L"/start lockdown;", L"", L""},//2
   {L"/fire halo;", L"", L""},//3
   {L"/lockdown timer #;", L"", L""},//4
   {L"/halo timer #;", L"", L""}//5
};
CMD_SCKEYS rpg_beta6_2_cmd_keys[RPGB_CMDS_SIZE] =
{
   {102, -1, -1},//0
   {103, 0, 0},//1
   {104, -1, -1},//2
   {105, -1, -1},//3
   {-1, -1, -1},//4
   {-1, -1, -1}//5
};

HANDLE hHDC_thread = NULL;
bool exit_thread = false;

#define MAIN_MENU 0
#define CLIENT 1
#define HOST 2
WORD *ServerType = NULL;

HaloCE_lib::DATA_HEADER *Players_Table_Header, 
                        *Device_Groups_Header, 
                        *Object_Table_Header,
                        *HS_Global_Header;

HaloCE_lib::STATIC_PLAYER *players = NULL;
HaloCE_lib::OBJECT_TABLE_ARRAY *objects = NULL;
  
DWORD Chat_address = NULL, 
      Current_Map_address = NULL,
      Cheats_address = NULL,
      Base_ptr_address = NULL,
      chat_address_ptr = NULL,
      Rasterizer_address = NULL,
      EngineDrawText_address = NULL,
      Console_func_address = NULL,
      HaloLocal_Chat_Func_address = NULL,
      sv_say_func_address = NULL,
      dwTemp = NULL;

bool *Console_chck = NULL;
char *Console_txt = NULL;

std::vector<MAPS> maps_tele_sites;
DLL_ADDRESSES dll_addresses = {{'d','l','l','_','s','t','_','a','d','d','r','s'}};

bool IsKeyDown(int keyCode)
{
   short result = ::GetAsyncKeyState(keyCode);
   return (result < 0);
}

void PressKey(BYTE key, short times)
{
   for (short i = 0; i < times; i++)
   {
      ::Sleep(150);
      ::keybd_event(key, 0x1C, 0, NULL);
      ::Sleep(150);
      ::keybd_event(key, 0x1C, KEYEVENTF_KEYUP, NULL);
   }
}

bool ParseCMDStrPlayers(wchar_t *cmd_str, short* player_index_array, int &pi_found)
{
   //c string length
   int chat_length = 0; do chat_length++; while(cmd_str[chat_length]);
   int max_players_to_find;
   
   if (pi_found != 0)
   {
      max_players_to_find = pi_found;
      pi_found = 0;
   }
   else
      max_players_to_find = Players_Table_Header->NumOfItems;
   
   if (cmd_str[0] == '\"')
   {
      int new_length = 0; while(cmd_str[new_length + 1] != '\"' && new_length < chat_length - 2) new_length++;
      chat_length = new_length;
      cmd_str = &cmd_str[1];// new_length);
   }
   
   for (int pi = 0; pi < Players_Table_Header->NumOfItems && pi_found < max_players_to_find; pi++)
   {
      //static_player = Players_Table_Header->ItemArray_ptr + (pi * Players_Table_Header->ItemSize);
      //Halo_Process->ReadMemArray<wchar_t>((LPVOID)(static_player + 0x4), *player_name, HaloCE_lib::PlayerNameMaxSize);
      
      for (int i = 0; i < chat_length && i < HaloCE_lib::PlayerNameMaxSize; i++)
      {
            
         if (cmd_str[i] != '*' && cmd_str[i] != '?' && cmd_str[i] != players[pi].PlayerName0[i])
            break;
            
         if (cmd_str[i] == '*' || !players[pi].PlayerName0[i + 1])
         {
            player_index_array[pi_found++] = pi;
            break;
         }
      }
   }
   

   return pi_found > 0;
}

inline DWORD GetObj(short obj_index)
{
   DWORD obj_address = NULL;
   if(obj_index != -1)
      obj_address = objects[obj_index].Object_ptr;
         
   return obj_address;
}

inline HaloCE_lib::SPARTAN* GetPlayerObj(short player_index)
{
   return (HaloCE_lib::SPARTAN*)GetObj(players[player_index].ObjectIndex);
}

inline HaloCE_lib::VEHICLE_OBJECT* GetPlayerVehObj(HaloCE_lib::SPARTAN* player_obj)
{
   HaloCE_lib::VEHICLE_OBJECT *veh_obj_address = NULL;
   if (player_obj->VehicleIndex != -1)
      veh_obj_address = (HaloCE_lib::VEHICLE_OBJECT*)GetObj(player_obj->VehicleIndex);
      
   return veh_obj_address;
}
inline bool KillPlayer(short player_index)
{
   bool succeded = false;
   HaloCE_lib::SPARTAN *player_object = GetPlayerObj(player_index);
   
   if(player_object)
   {
      player_object->KillPlayer = 0x20;
      succeded = true;
   }
   
   return succeded;
}

void HaloDrawText(char *msg_str, BYTE alpha, BYTE red, BYTE green, BYTE blue)
{
   float *argb_ptr, ARGB[4] = {0};
   argb_ptr = (float*)&ARGB;
   ARGB[0] = (float)alpha / (float)255;
   ARGB[1] = (float)red / (float)255;
   ARGB[2] = (float)green / (float)255;
   ARGB[3] = (float)blue / (float)255;
   
   __asm
   {
      MOV EAX,argb_ptr
      PUSH msg_str
      CALL EngineDrawText_address
      ADD ESP,4
   }
}

void Console(char *func_str, int Unknown = 0)
{
   __asm
   {
      PUSH Unknown
      MOV  EDI,func_str
      CALL Console_func_address
      ADD  ESP,4
   }
}
void consolejpmp()
{
   
   __asm
   {
      
      CALL Console_func_address
      //JMP  0x004C9BE3
   }
}
void Local_Chat(wchar_t *message)
{
   __asm
   {
      PUSH message
      CALL HaloLocal_Chat_Func_address
      ADD  ESP,4
   }
}
void sv_say(char *message)
{
   __asm
   {
      MOV  ECX,message
      CALL sv_say_func_address
   }
}

//functions from mainform,
#define TOGGLE 2
void ckbx_rapid_fire_CheckedChanged(BYTE Checked)
{
   if (Checked == TOGGLE)
   {
      if (*(BYTE*)halo_cmd_data[20].cmd_address == 0xD9)
         Checked = true;
      else
         Checked = false;
   }
   
   DWORD dwOldProtect = NULL;
   ::VirtualProtect((LPVOID)halo_cmd_data[20].cmd_address, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &dwOldProtect);

   if (Checked)
      *(DWORD*)halo_cmd_data[20].cmd_address = 0xD9909090;
   else
      *(DWORD*)halo_cmd_data[20].cmd_address = 0xD91047D9;
   
   ::VirtualProtect((LPVOID)halo_cmd_data[20].cmd_address, sizeof(DWORD), dwOldProtect, &dwOldProtect);
}
void ckbx_time_freeze_CheckedChanged(BYTE Checked)
{
   if (Checked == TOGGLE)
   {
      if (*(BYTE*)halo_cmd_data[21].cmd_address == 0x74)
         Checked = true;
      else
         Checked = false;
   }
   
   DWORD dwOldProtect = NULL;
   ::VirtualProtect((LPVOID)halo_cmd_data[21].cmd_address, sizeof(WORD), PAGE_EXECUTE_READWRITE, &dwOldProtect);
   
   if (Checked)
      *(WORD*)halo_cmd_data[21].cmd_address = 0x3975;
   else
      *(WORD*)halo_cmd_data[21].cmd_address = 0x2A74;
   
   ::VirtualProtect((LPVOID)halo_cmd_data[21].cmd_address, sizeof(WORD), dwOldProtect, &dwOldProtect);
}
void ckbx_grav_boots_CheckedChanged(BYTE Checked)
{
   DWORD number_address = halo_cmd_data[22].cmd_address - 7;
   DWORD jmp_to_address = halo_cmd_data[22].cmd_address - 0xD;
   
   if (Checked == TOGGLE)
   {
      if (*(DWORD*)number_address == 0x00000242)
         Checked = false;
      else
         Checked = true;
   }
   
   //on find this -> 0x00560E70: eax,[eax+000002F4]
   //mov [eax+000002F4],00000242
   //jmp 00560E76
   
   //off
   //mov [eax+000002F4],00000202
   //jmp 00560E76

   DWORD dwOldProtect = NULL;
   ::VirtualProtect((LPVOID)jmp_to_address, 19, PAGE_EXECUTE_READWRITE, &dwOldProtect);
   
   if (*(BYTE*)(halo_cmd_data[22].cmd_address - 3) != 0xEB)
   {  
      BYTE *code_cave = (BYTE*)jmp_to_address;
      
      //mov [eax+000002F4],00000202
      code_cave[0] = 0xC7;
      code_cave[1] = 0x80;
      code_cave[2] = 0xF4;
      code_cave[3] = 0x02;
      code_cave[4] = 0x00;
      code_cave[5] = 0x00;
      code_cave[6] = 0x02;
      code_cave[7] = 0x02;
      code_cave[8] = 0x00;
      code_cave[9] = 0x00;
      //jmp
      code_cave[10] = 0xEB;
      code_cave[11] = 0x07;
      
      BYTE *jmp_to_code_cave = (BYTE*)halo_cmd_data[22].cmd_address;
      jmp_to_code_cave[0] = 0xEB;
      jmp_to_code_cave[1] = 0xF1;
      jmp_to_code_cave[2] = 0x90;
      jmp_to_code_cave[3] = 0x90;
      jmp_to_code_cave[4] = 0x90;
      jmp_to_code_cave[5] = 0x90;
   }
   
   if (Checked)
      *(DWORD*)number_address = 0x00000242;
   else
      *(DWORD*)number_address = 0x00000202;
      
   ::VirtualProtect((LPVOID)jmp_to_address, 19, dwOldProtect, &dwOldProtect);
}
void ckbx_vehicle_ntr_CheckedChanged(BYTE Checked)
{
   if (Checked == TOGGLE)
   {
      if (*(BYTE*)halo_cmd_data[23].cmd_address == 0x74)
         Checked = true;
      else
         Checked = false;
   }
   
   DWORD dwOldProtect = NULL;
   ::VirtualProtect((LPVOID)halo_cmd_data[23].cmd_address, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &dwOldProtect);
   
   if (Checked)
      *(BYTE*)halo_cmd_data[23].cmd_address = 0xEB;
   else
      *(BYTE*)halo_cmd_data[23].cmd_address = 0x74;
   
   ::VirtualProtect((LPVOID)halo_cmd_data[23].cmd_address, sizeof(BYTE), dwOldProtect, &dwOldProtect);
}

DWORD WINAPI HDC_thread(LPVOID hModule)
{
   dll_addresses.halo_cmd_keys = (CMD_SCKEYS**)&halo_cmd_keys;
   dll_addresses.rpgb62_cmd_keys = (CMD_SCKEYS**)&rpg_beta6_2_cmd_keys;
   dll_addresses.cmd_sk_enabled = &dll_sk_enabled;
   dll_addresses.locations_changed = true;
   
   DWORD base_address = NULL,
         scan_size = NULL;
         
   bool display_txt = true, 
        erase_chat = false,
        erase_cmd_terminator = false;
   
   int chat_Length = 0;
   
   char *Current_Map_Str = NULL;
   wchar_t chat[CMD_STR_SIZE] = {0};
   
   enum HALO_TYPE { halo = 0, haloce = 1} running_gt;
   if (ReadWriteMemoryTools::RWMemory::IsProcessOpenA("halo.exe"))
      running_gt = halo;
   else if (ReadWriteMemoryTools::RWMemory::IsProcessOpenA("haloce.exe"))
      running_gt = haloce;
      
   DWORD hHalo = (DWORD)::GetModuleHandleA(NULL);
   IMAGE_DOS_HEADER *IDH = (IMAGE_DOS_HEADER*)hHalo;
   IMAGE_NT_HEADERS *INH = (IMAGE_NT_HEADERS*)(hHalo + IDH->e_lfanew);
   base_address = (DWORD)hHalo + INH->OptionalHeader.BaseOfCode;
   scan_size = INH->OptionalHeader.SizeOfCode;
   
   
   //find patterns
   //halo functions
   EngineDrawText_address = CurrentProcess::FindMemPattern(
       base_address,
       scan_size,
       HaloCE_lib::EngineDrawText_func_addr_sig);
      
   Console_func_address = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Console_func_addr_sig);
      
   HaloLocal_Chat_Func_address = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Local_Chat_func_addr_sig);
      
   if (running_gt == haloce)
      sv_say_func_address = CurrentProcess::FindMemPattern(
         base_address,
         scan_size,
         HCE_Lib::sv_say_func_addr_sig);
         
   //halo DATA_HEADER's
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Players_Table_Header_ptr_sig);
      
   if (dwTemp < 50)
      HaloDrawText("Players_Table_Header_ptr_sig failed", 255, 255, 0, 0);
   else
   {
      DWORD Players_Table_Header_ptr_address = *(DWORD*)dwTemp;
      //wait for halo to load
      while(*(DWORD*)Players_Table_Header_ptr_address == NULL)
         Sleep(100);

      Players_Table_Header = (HaloCE_lib::DATA_HEADER*)*(DWORD*)Players_Table_Header_ptr_address;
      players = (HaloCE_lib::STATIC_PLAYER*)Players_Table_Header->ItemArray_ptr;
   }
   
   dwTemp = NULL;
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Device_Groups_Header_ptr_sig);
   
   if (dwTemp < 50)
      HaloDrawText("Device_Groups_Header_ptr_sig failed", 255, 255, 0, 0);
   else
      Device_Groups_Header = (HaloCE_lib::DATA_HEADER*)**(DWORD**)dwTemp;
   
   dwTemp = NULL;
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Object_Table_Header_ptr_sig);
   
   if (dwTemp < 50)
      HaloDrawText("Object_Table_Header_ptr_sig failed", 255, 255, 0, 0);
   else
   {
      Object_Table_Header = (HaloCE_lib::DATA_HEADER*)**(DWORD**)dwTemp;
      objects = (HaloCE_lib::OBJECT_TABLE_ARRAY*)Object_Table_Header->ItemArray_ptr;
   }
   
   dwTemp = NULL;
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::HS_Global_Header_ptr_sig);
      
   if (dwTemp < 50)
      HaloDrawText("HS_Global_Header_ptr_sig failed", 255, 255, 0, 0);
   else
      HS_Global_Header = (HaloCE_lib::DATA_HEADER*)**(DWORD**)dwTemp;
      
     
      //global vars
   if (running_gt == haloce)
   {
      dwTemp = NULL;
      dwTemp = CurrentProcess::FindMemPattern(
         base_address,
         scan_size,
         HCE_Lib::Dev_addr_sig);
         
      if (dwTemp < 50)
         HaloDrawText("Dev_addr_sig failed", 255, 255, 0, 0);
      else
         halo_cmd_data[1].cmd_address = *(DWORD*)dwTemp;
   }
      
   dwTemp = NULL;
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Console_addr_sig);

   if (dwTemp < 50)
      HaloDrawText("Console_addr_sig failed", 255, 255, 0, 0);
   else
   {
      Console_chck = (bool*)*(DWORD*)dwTemp;
      halo_cmd_data[0].cmd_address = (DWORD)Console_chck + HaloCE_lib::Console::Enabled_offset;
      Console_txt = (char*)((DWORD)Console_chck + HaloCE_lib::Console::Text_offset);
   }
   
   dwTemp = NULL;
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Current_map_addr_sig);
   
   if (dwTemp < 50)
      HaloDrawText("Current_map_addr_sig failed", 255, 255, 0, 0);
   else
      Current_Map_Str = (char*)*(DWORD*)dwTemp;
      
   dwTemp = NULL;
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Cheats_addr_sig);
   
   if (dwTemp < 50)
      HaloDrawText("Cheats_addr_sig failed", 255, 255, 0, 0);
   else
   {
      Cheats_address = *(DWORD*)dwTemp;
                        
      halo_cmd_data[2].cmd_address = Cheats_address + HaloCE_lib::Cheats::Deathless_offset;
      halo_cmd_data[3].cmd_address = Cheats_address + HaloCE_lib::Cheats::Infinite_Ammo_offset;
      halo_cmd_data[4].cmd_address = Cheats_address + HaloCE_lib::Cheats::Bottomless_Clip_offset;
      halo_cmd_data[8].cmd_address = Cheats_address + HaloCE_lib::Cheats::Omnipotent_offset;
      halo_cmd_data[9].cmd_address = Cheats_address + HaloCE_lib::Cheats::JetPack_offset;
      halo_cmd_data[10].cmd_address = Cheats_address + HaloCE_lib::Cheats::Bmp_Possession_offset;
      halo_cmd_data[11].cmd_address = Cheats_address + HaloCE_lib::Cheats::Super_jmp_offset;
      halo_cmd_data[12].cmd_address = Cheats_address + HaloCE_lib::Cheats::Reflexive_damage_offset;
      halo_cmd_data[13].cmd_address = Cheats_address + HaloCE_lib::Cheats::Medusa_offset;
      halo_cmd_data[14].cmd_address = Cheats_address + HaloCE_lib::Cheats::Controller_offset;
   }

   dwTemp = NULL;
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Base_ptr_addr_sig);

   if (dwTemp < 50)
      HaloDrawText("Base_ptr_addr_sig failed", 255, 255, 0, 0);
   else
      Base_ptr_address = *(DWORD*)dwTemp;
      
      
   dwTemp = NULL;
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Show_Hud_ptr_addr_sig);
   
   if (dwTemp < 50)
      HaloDrawText("Show_Hud_ptr_addr_sig failed", 255, 255, 0, 0);
   else
      halo_cmd_data[5].cmd_address = **(DWORD**)dwTemp;
           
           
   dwTemp = NULL;               
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::letterbox_ptr_addr_sig);

   if (dwTemp < 50)
      HaloDrawText("letterbox_ptr_addr_sig failed", 255, 255, 0, 0);
   else
      halo_cmd_data[6].cmd_address = **(DWORD**)dwTemp + 8;
      
      
   dwTemp = NULL;   
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Rider_Eject_addr_sig);
   
   if (dwTemp < 50)
      HaloDrawText("Rider_Eject_addr_sig failed", 255, 255, 0, 0);
   else
      halo_cmd_data[7].cmd_address = *(DWORD*)dwTemp;
      
      
   dwTemp = NULL;   
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Rasterizer_addr_sig);
   
   if (dwTemp < 50)
      HaloDrawText("Rasterizer_addr_sig failed", 255, 255, 0, 0);
   else
   {
      Rasterizer_address = *(DWORD*)dwTemp;
                           
      halo_cmd_data[15].cmd_address = Rasterizer_address + HaloCE_lib::Rasterizer::WireFrame_offset;
      halo_cmd_data[16].cmd_address = Rasterizer_address + HaloCE_lib::Rasterizer::FogAtmosphere_offset;
      halo_cmd_data[17].cmd_address = Rasterizer_address + HaloCE_lib::Rasterizer::FogPlane_offset;
      halo_cmd_data[18].cmd_address = Rasterizer_address + HaloCE_lib::Rasterizer::FPS_offset;
   }
     
   
   dwTemp = NULL;   
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Game_Speed_ptr_addr_sig);
   
   if (dwTemp < 50)
      HaloDrawText("Game_Speed_ptr_addr_sig failed", 255, 255, 0, 0);
   else
      halo_cmd_data[19].cmd_address = **(DWORD**)dwTemp + 0x18;
      
      
   dwTemp = NULL;                     
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Rapid_Fire_addr_sig);
   
   if (dwTemp < 50)
      HaloDrawText("Rapid_Fire_addr_sig failed", 255, 255, 0, 0);
   else
      halo_cmd_data[20].cmd_address = dwTemp;
      
      
   dwTemp = NULL;   
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Time_Freeze_addr_sig);
      
   if (dwTemp < 50)
      HaloDrawText("Time_Freeze_addr_sig failed", 255, 255, 0, 0);
   else
      halo_cmd_data[21].cmd_address = dwTemp;
      
      
   dwTemp = NULL;   
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Grav_Boots_addr_sig);
   
   if (dwTemp < 50)
      HaloDrawText("Grav_Boots_addr_sig failed", 255, 255, 0, 0);
   else
      halo_cmd_data[22].cmd_address = dwTemp;
      
         
   dwTemp = NULL;   
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::Vehicle_NTR_addr_sig);
   
   if (dwTemp < 50)
      HaloDrawText("Vehicle_NTR_addr_sig failed", 255, 255, 0, 0);
   else
      halo_cmd_data[23].cmd_address = dwTemp;
      
      
   dwTemp = NULL;   
   dwTemp = CurrentProcess::FindMemPattern(
      base_address,
      scan_size,
      HaloCE_lib::ServerType_ptr_addr_sig);
   
   if (dwTemp < 50)
      HaloDrawText("ServerType_ptr_addr_sig failed", 255, 255, 0, 0);
   else
      ServerType = (WORD*)*(DWORD*)dwTemp;
      
         
   //rpg_beta6_2 vars
   if (running_gt == haloce)
   {
      rpg_beta6_2_cmd_data[0].cmd_address = HS_Global_Header->ItemArray_ptr
      + HCE_Lib::rpg_beta6_2_hs_global::setting_offset;
            
      rpg_beta6_2_cmd_data[1].cmd_address = HS_Global_Header->ItemArray_ptr 
      + HCE_Lib::rpg_beta6_2_hs_global::alarmed_offset;
            
      rpg_beta6_2_cmd_data[2].cmd_address = HS_Global_Header->ItemArray_ptr 
      + HCE_Lib::rpg_beta6_2_hs_global::locked_offset;
            
      rpg_beta6_2_cmd_data[3].cmd_address = HS_Global_Header->ItemArray_ptr 
      + HCE_Lib::rpg_beta6_2_hs_global::nuked_offset;
            
      rpg_beta6_2_cmd_data[4].cmd_address = HS_Global_Header->ItemArray_ptr 
      + HCE_Lib::rpg_beta6_2_hs_global::lock_timer_offset;
               
      rpg_beta6_2_cmd_data[5].cmd_address = HS_Global_Header->ItemArray_ptr 
      + HCE_Lib::rpg_beta6_2_hs_global::boom_timer_offset;
   }
   
   while (!dll_addresses.CurrentDir)
      ::Sleep(50);
      
   //c string length
   int length = 0; do length++; while(dll_addresses.CurrentDir[length]);
   
   wchar_t *loc_file_name = LOCATIONS_FILE_NAME; 
   int loc_name_length = 0; do loc_name_length++; while(loc_file_name[loc_name_length]);
         
   wchar_t *LocationsFilePath = new wchar_t[length + 14];
   for (int i = 0; i < length; i++)
      LocationsFilePath[i] = dll_addresses.CurrentDir[i];
   
   CurrentProcess::FreeMemory((LPVOID)dll_addresses.CurrentDir);
   
   LocationsFilePath[length] = L'\\';
   
   for (int i = 0; i <= loc_name_length; i++)
      LocationsFilePath[length + i + 1] = loc_file_name[i];
   
   //main loop
   while(!exit_thread)
	{
	   ::Sleep(50);//so it doesn't slow halo down
	   if (exit_thread)
	   {
	      DWORD ExitCode = 0x006E6977;
	      ::ExitThread(ExitCode);
	   }
	   
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
	   
	   if (str_cmp("ui", Current_Map_Str) && display_txt)
      {
         HaloDrawText("Halo Dev Controls - Jesus7Freak", 255, 255, 255, 255);
         display_txt = false;
      }
      else if (!str_cmp("ui", Current_Map_Str))
         display_txt = true;
         
      //keyboard shortcuts
      if (dll_sk_enabled.halo_Enable_Shrtcts)
      {
         if (IsKeyDown(halo_cmd_keys[20].toggle_key))
         {
            ckbx_rapid_fire_CheckedChanged(TOGGLE);
            ::Sleep(200);
         }
         else if (IsKeyDown(halo_cmd_keys[20].on_key))
         {
            ckbx_rapid_fire_CheckedChanged(true);
            ::Sleep(200);
         }
         else if (IsKeyDown(halo_cmd_keys[20].off_key))
         {
            ckbx_rapid_fire_CheckedChanged(false);
            ::Sleep(200);
         }
         else if (IsKeyDown(halo_cmd_keys[21].toggle_key))
         {
            ckbx_time_freeze_CheckedChanged(TOGGLE);
            ::Sleep(200);
         }
         else if (IsKeyDown(halo_cmd_keys[21].on_key))
         {
            ckbx_time_freeze_CheckedChanged(true);
            ::Sleep(200);
         }
         else if (IsKeyDown(halo_cmd_keys[21].off_key))
         {
            ckbx_time_freeze_CheckedChanged(false);
            ::Sleep(200);
         }
         else if (IsKeyDown(halo_cmd_keys[22].toggle_key))
         {
            ckbx_grav_boots_CheckedChanged(TOGGLE);
            ::Sleep(200);
         }
         else if (IsKeyDown(halo_cmd_keys[22].on_key))
         {
            ckbx_grav_boots_CheckedChanged(true);
            ::Sleep(200);
         }
         else if (IsKeyDown(halo_cmd_keys[22].off_key))
         {
            ckbx_grav_boots_CheckedChanged(false);
            ::Sleep(200);
         }
         else if (IsKeyDown(halo_cmd_keys[23].toggle_key))
         {
            ckbx_vehicle_ntr_CheckedChanged(TOGGLE);
            ::Sleep(200);
         }
         else if (IsKeyDown(halo_cmd_keys[23].on_key))
         {
            ckbx_vehicle_ntr_CheckedChanged(true);
            ::Sleep(200);
         }
         else if (IsKeyDown(halo_cmd_keys[23].off_key))
         {
            ckbx_vehicle_ntr_CheckedChanged(false);
            ::Sleep(120000);
         }
         else
         {
            for (int i = 0; i < HALO_CMDS_SIZE - 4; i++)
            {
               //skip if not running the specified game type for game dependent cmds
               if (halo_cmd_data[i].gametype_affect)
                  if (running_gt != haloce)
                     continue;
                     
               //skip if not hosting for host dependent cmds
               if (halo_cmd_data[i].host_affected)
                  if (*ServerType == CLIENT)
                     continue;
                  
               //toggle cmd on/off
               if (IsKeyDown(halo_cmd_keys[i].toggle_key))
               {
                  *(bool*)halo_cmd_data[i].cmd_address =  !*(bool*)halo_cmd_data[i].cmd_address;
                  ::Sleep(200);
                  break;
               }
               //turn cmd on
               else if (IsKeyDown(halo_cmd_keys[i].on_key))
               {
                  *(bool*)halo_cmd_data[i].cmd_address = true;
                  ::Sleep(200);
                  break;
               }
               //turn cmd off
               else if (IsKeyDown(halo_cmd_keys[i].off_key))
               {
                  *(bool*)halo_cmd_data[i].cmd_address = false;
                  ::Sleep(200);
                  break;
               }
            }
         }
      }
      
      //if console isnt open, use chat for commands
      if (!*Console_chck)
      {
         //check for in game chat commands///
         if (*ServerType != MAIN_MENU)
         {
            chat_address_ptr = *(DWORD*)(
               *(DWORD*)(
               *(DWORD*)(
               *(DWORD*)(
               *(DWORD*)Base_ptr_address + 0x94) + 0x4) + 0x8)) + 0x94;
            
            Chat_address = *(DWORD*)chat_address_ptr;
            if (Chat_address)
            {
               wchar_t *chat_str = (wchar_t*)Chat_address;
               for (chat_Length = 0; chat_Length < CMD_STR_SIZE && chat_str[chat_Length]; chat_Length++)
                  chat[chat_Length] = chat_str[chat_Length];
                  
               chat[chat_Length] = L'\0';
            }
         }
      }
      else//check console
      {
         for (chat_Length = 0; chat_Length < CMD_STR_SIZE && Console_txt[chat_Length]; chat_Length++)
            chat[chat_Length] = (wchar_t)Console_txt[chat_Length];
         
         Chat_address = (DWORD)Console_txt;
         chat[chat_Length] = '\0';
      }
      
      if (chat[0] == '/' && chat[chat_Length - 1] == ';')
      {
         for (int i = 0; i < HALO_CMDS_SIZE - 4; i++)
         {
         
            //skip if not running the specified game type for game dependent cmds
            if (halo_cmd_data[i].gametype_affect)
               if (running_gt != haloce)
                  continue;
                     
            //skip if not hosting for host dependent cmds
            if (halo_cmd_data[i].host_affected)
               if (*ServerType == CLIENT)
                  continue;
                  
            //toggle cmd on/off
            if (str_cmp(chat, halo_cmd_strs[i].cmd_on))
            {
               *(bool*)halo_cmd_data[i].cmd_address = true;
               erase_chat = true;
               break;
            }
            //turn cmd off
            else if (str_cmp(chat, halo_cmd_strs[i].cmd_off))
            {
               *(bool*)halo_cmd_data[i].cmd_address = false;
               erase_chat = true;
               break;
            }
         }
         
         if (!erase_chat && *ServerType != CLIENT)
         {
            if (str_cmp(chat, L"/game_speed ", 12))
            {
               float number = 0;
               if (ParseCMDStrNumber(&chat[12], chat_Length - 12 - 1, &number))
               {
                  *(float*)halo_cmd_data[19].cmd_address = number;
                  erase_chat = true;
               }
               erase_cmd_terminator = !erase_chat;
            }
            else if (str_cmp(chat, L"/spd ", 5))
            {
               float number = 0; 
                  
               int str_2arg_index = FindCMDArgIndex(chat, 2);
               short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
                  
               if (ParseCMDStrPlayers(&chat[5], player_index, pi_found))
               {
                  if (ParseCMDStrNumber(&chat[str_2arg_index], chat_Length - str_2arg_index - 1, &number))
                  {    
                     for (int i = 0; i < pi_found; i++)
                        players[player_index[i]].SpeedModifier = number;
                        
                     erase_chat = true;
                  }
               }
               
               erase_cmd_terminator = !erase_chat;
               if (player_index)
                  delete[] player_index;
            }   
            if (str_cmp(chat, L"/team ", 6))
            {
               DWORD Team = 0;
               short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
               if (ParseCMDStrPlayers(&chat[6], player_index, pi_found))
               {
                  for (int i = 0; i < pi_found; i++)
                  {
                     KillPlayer(player_index[i]);
                     
                     Team = players[player_index[i]].Team;
                     if (Team)
                        Team = 0;
                     else
                        Team = 1;
                      
                     players[player_index[i]].Team = Team;
                      
                     erase_chat = true;
                  }
               }
               
               erase_cmd_terminator = !erase_chat;
               if (player_index)
                  delete[] player_index;
            }
            else if (str_cmp(chat, L"/afk ", 5))
            {
               DWORD number = 90;
               short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
               if (ParseCMDStrPlayers(&chat[5], player_index, pi_found))
               {
                  for (int i = 0; i < pi_found; i++)
                  {
                     if (KillPlayer(player_index[i]))
                     {
                        number = 0x7FFFFFFF;
                        ::Sleep(50);
                     }
                        
                     players[player_index[i]].RespawnTimer = number;
                     erase_chat = true;
                  }
                  
                  erase_cmd_terminator = !erase_chat;
                  if (player_index)
                     delete[] player_index;
               }
            }
            if (str_cmp(chat, L"/k ", 3))
            {
               short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
               if (ParseCMDStrPlayers(&chat[3], player_index, pi_found))
               {
                  for (int i = 0; i < pi_found; i++)
                  {
                     char _str[11] = {'s','v','_','k','i','c','k',' '};
                     
                     if (++player_index[i] > 9)
                     {
                        _str[8] = '1';
                        _str[9] = (player_index[i] - 10) + 48;
                     }
                     else
                     {
                        _str[8] = '0';
                        _str[9] = player_index[i] + 48;
                     }
                     
                     Console(_str);
                     erase_chat = true;
                  }
               }
               
               erase_cmd_terminator = erase_chat;
               if (player_index)
                  delete[] player_index;
            }
            else if (str_cmp(chat, L"/b ", 3))
            {
               short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
               if (ParseCMDStrPlayers(&chat[3], player_index, pi_found))
               {
                  for (int i = 0; i < pi_found; i++)
                  {
                     char _str[10] = {'s','v','_','b','a','n',' '};
                     
                     if (++player_index[i] > 9)
                     {
                        _str[7] = '1';
                        _str[8] = (player_index[i] - 10) + 48;
                     }
                     else
                     {
                        _str[7] = '0';
                        _str[8] = player_index[i] + 48;
                     }
                     
                     Console(_str);
                     erase_chat = true;
                  }
               }
               
               erase_cmd_terminator = erase_chat;
               if (player_index)
                  delete[] player_index;
            }
            else if (str_cmp(chat, L"/kill ", 6))
            {
               short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
               if (ParseCMDStrPlayers(&chat[6], player_index, pi_found))
               {
                  for (int i = 0; i < pi_found; i++)
                  {
                     KillPlayer(player_index[i]);
                     erase_chat = true;
                  }
               }
               
               erase_cmd_terminator = erase_chat;
               if (player_index)
                  delete[] player_index;
            }
            if (str_cmp(chat, L"/health ", 8))
            {
               float number = 0;
                 
               int str_2arg_index = FindCMDArgIndex(chat, 2);
               if (ParseCMDStrNumber(&chat[str_2arg_index], chat_Length - str_2arg_index - 1, &number))
               {
                  short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
                  if (ParseCMDStrPlayers(&chat[8], player_index, pi_found))
                  {
                     for (int i = 0; i < pi_found; i++)
                     {
                        HaloCE_lib::SPARTAN* player_object = GetPlayerObj(player_index[i]);
                        if(player_object)
                        {
                           player_object->Health = number;
                           erase_chat = true;
                        } 
                     } 
                  }
                  
                  erase_cmd_terminator = !erase_chat;
                  if (player_index)
                     delete[] player_index;
               }
            }
            else if (str_cmp(chat, L"/shield ", 8))
            {
               float number = 0;
                  
               int str_2arg_index = FindCMDArgIndex(chat, 2);
               if (ParseCMDStrNumber(&chat[str_2arg_index], chat_Length - str_2arg_index - 1, &number))
               {
                  short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
                  if (ParseCMDStrPlayers(&chat[8], player_index, pi_found))
                  {
                     for (int i = 0; i < pi_found; i++)
                     {
                        HaloCE_lib::SPARTAN* player_object = GetPlayerObj(player_index[i]);
                        if(player_object)
                        {
                           player_object->Shield_00 = number;
                           erase_chat = true;
                        }
                     }
                  }
                  
                  erase_cmd_terminator = !erase_chat;
                  if (player_index)
                     delete[] player_index;
               }
            }
            if (str_cmp(chat, L"/t ", 3))
            {
               float coordinates[3] = {0}; 
               short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
               bool teleport = false;
               
               if (ParseCMDStrPlayers(&chat[3], player_index, pi_found))
               {
                  //count the number of spaces
                  int num_of_chars = 0;
                  for (int i = 0; i < chat_Length; i++)
                     if (chat[i] == ' ')
                        num_of_chars++;
                     
                  if (num_of_chars == 4)
                  {
                     int str_2arg_index = FindCMDArgIndex(chat, 2);
                     int str_3arg_index = str_2arg_index + FindCMDArgIndex(&chat[str_2arg_index], 1);
                     int str_4arg_index = str_3arg_index + FindCMDArgIndex(&chat[str_3arg_index], 1);
                     if (ParseCMDStrNumber(&chat[str_2arg_index], str_3arg_index - str_2arg_index - 1, &coordinates[0]))
                        if (ParseCMDStrNumber(&chat[str_3arg_index], str_4arg_index - str_3arg_index - 1, &coordinates[1]))
                           if (ParseCMDStrNumber(&chat[str_4arg_index], chat_Length - str_4arg_index - 1, &coordinates[2]))
                                 teleport = true;
                  }
                  else if (num_of_chars == 2)
                  {
                     wchar_t tele_loc_name[TELE_LOC_NAME_SIZE] = {0};
                     int str_2arg_index = FindCMDArgIndex(chat, 2);
                     for (int i = 0; chat[str_2arg_index + i + 1] && i < TELE_LOC_NAME_SIZE; i++)
                        tele_loc_name[i] = chat[str_2arg_index + i];
                           
                     unsigned int map_i = 0;
                     if (FindMapIndex(&maps_tele_sites, Current_Map_Str, map_i))
                     {
                        unsigned int tele_loc_i = 0;
                        if (FindTeleLocNameIndex(&maps_tele_sites[map_i].teleport_locations, tele_loc_name, tele_loc_i))
                        {
                           for (int i = 0; i < 3; i++)
                              coordinates[i] =  maps_tele_sites[map_i].teleport_locations[tele_loc_i].coordinates[i];
                                 
                           teleport = true;
                        }
                     }
                  }
               }
               
               if (teleport)
               {
                  for (int i = 0; i < pi_found; i++)
                  {
                     HaloCE_lib::SPARTAN *player_object = GetPlayerObj(player_index[i]);
                     if (player_object)
                     {        
                        //if player is in a vehicle, use vehicle's coordinates
                        HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
                        if (vehicle_object)
                           player_object = vehicle_object;
                                    
                        for (int i = 0; i < 3; i++)    
                           player_object->m_World[i] = coordinates[i];

                        erase_chat = true;
                     }
                  }
               }
               
               erase_cmd_terminator = !erase_chat;
               if (player_index)
                  delete[] player_index;
            }
            else if (str_cmp(chat, L"/tp ", 4))
            {
               short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
               short player2_index = 0; int pi2_to_find = 1;
                  
               if (ParseCMDStrPlayers(&chat[4], player_index, pi_found))
               {
                  int str_2p_index = FindCMDArgIndex(chat, 2);
                  if (ParseCMDStrPlayers(&chat[str_2p_index], &player2_index, pi2_to_find))
                  {
                     HaloCE_lib::SPARTAN *player2_object = GetPlayerObj(player2_index);
                     if (player2_object)
                     {        
                        //if player is in a vehicle, use vehicle's coordinates
                        HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player2_object);
                        if (vehicle_object)
                           player2_object = vehicle_object;
                              
                        for (int i = 0; i < pi_found; i++)
                        {
                           HaloCE_lib::SPARTAN *player_object = GetPlayerObj(player_index[i]);
                           if (player_object)
                           {        
                              //if player is in a vehicle, use vehicle's coordinates
                              HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
                              if (vehicle_object)
                                 player_object = vehicle_object;
                                
                              player_object->m_World[0] = player2_object->m_World[0];
                              player_object->m_World[1] = player2_object->m_World[1];
                              player_object->m_World[2] = player2_object->m_World[2] + 1 + i;
                              erase_chat = true;
                           }
                        }
                     }
                  }
               }
               
               erase_cmd_terminator = !erase_chat;
               if (player_index)
                  delete[] player_index;
            }
            else if (str_cmp(chat, L"/v ", 3))
            {
               float velocity_direction[3] = {0}; 
               short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
               
               int str_2arg_index = FindCMDArgIndex(chat, 2);
               int str_3arg_index = str_2arg_index + FindCMDArgIndex(&chat[str_2arg_index], 1);
               int str_4arg_index = str_3arg_index + FindCMDArgIndex(&chat[str_3arg_index], 1);
               if (ParseCMDStrNumber(&chat[str_2arg_index], str_3arg_index - str_2arg_index - 1, &velocity_direction[0]))
               if (ParseCMDStrNumber(&chat[str_3arg_index], str_4arg_index - str_3arg_index - 1, &velocity_direction[1]))
               if (ParseCMDStrNumber(&chat[str_4arg_index], chat_Length - str_4arg_index - 1, &velocity_direction[2]))
               {
                  if (ParseCMDStrPlayers(&chat[3], player_index, pi_found))
                  {
                     for (int i = 0; i < pi_found; i++)
                     {
                        HaloCE_lib::SPARTAN *player_object = GetPlayerObj(player_index[i]);
                        if (player_object)
                        {        
                           //if player is in a vehicle, use vehicle's coordinates
                           HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
                           if (vehicle_object)
                              player_object = vehicle_object;
                                       
                           for (int i = 0; i < 3; i++)    
                              player_object->m_Velocity[i] += velocity_direction[i];  
                           erase_chat = true;
                        }
                     }
                  }
               }
               
               erase_cmd_terminator = !erase_chat;
               if (player_index)
                  delete[] player_index;
            }
            else if (str_cmp(chat, L"/st ", 4))
            {
               float coordinates[3] = {0};
               TELEPORT_LOCATION tele_site = {0};
               
               //count the number of spaces
               int num_of_chars = 0;
               for (int i = 0; i < chat_Length; i++)
                  if (chat[i] == ' ')
                     num_of_chars++;
                  
               if (num_of_chars == 2)
               {
                  int str_2arg_index = FindCMDArgIndex(chat, 2);
                  for (int i = 0; chat[str_2arg_index + i + 1] && i < TELE_LOC_NAME_SIZE; i++)
                     tele_site.teleport_loc_name[i] = chat[str_2arg_index + i];
                  
                  //add null at end if too long
                  tele_site.teleport_loc_name[TELE_LOC_NAME_SIZE - 1] = '\0';
               
               
                  short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 1;
                  if (ParseCMDStrPlayers(&chat[4], player_index, pi_found))
                  {
                     HaloCE_lib::SPARTAN *player_object = GetPlayerObj(player_index[0]);
                     if (player_object)
                     {        
                        //if player is in a vehicle, use vehicle's coordinates
                        HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
                        if (vehicle_object)
                           player_object = vehicle_object;
                                       
                        for (int i = 0; i < 3; i++)    
                           tele_site.coordinates[i] = player_object->m_World[i];
                              
                        unsigned int map_i = 0, tele_loc_i = 0;
                        if (!FindMapIndex(&maps_tele_sites, Current_Map_Str, map_i))
                        {
                           MAPS map_loc;
                                 
                           for (int i = 0; i < MAP_STR_SIZE; i++)
                              map_loc.map_name[i] = Current_Map_Str[i];
                                    
                           maps_tele_sites.push_back(map_loc);
                           map_i = maps_tele_sites.size() - 1;
                        }
                              
                        //if the tele site exists, overwrite it
                        if (FindTeleLocNameIndex(&maps_tele_sites[map_i].teleport_locations, tele_site.teleport_loc_name, tele_loc_i))
                           maps_tele_sites[map_i].teleport_locations[tele_loc_i] = tele_site;
                        else
                           maps_tele_sites[map_i].teleport_locations.push_back(tele_site);
                           
                        WriteLocationsToFile(LocationsFilePath, &maps_tele_sites);
                        erase_chat = true;
                     }     
                  }
                  
                  erase_cmd_terminator = !erase_chat;
                  if (player_index)
                     delete[] player_index;
               }
            }
            else if (str_cmp(chat, L"/j ", 3))
            {
               float coordinates[3] = {0};
                  
               int str_2arg_index = FindCMDArgIndex(chat, 2);
               int str_3arg_index = str_2arg_index + FindCMDArgIndex(&chat[str_2arg_index], 1);
               int str_4arg_index = str_3arg_index + FindCMDArgIndex(&chat[str_3arg_index], 1);
               if (ParseCMDStrNumber(&chat[str_2arg_index], str_3arg_index - str_2arg_index - 1, &coordinates[0]))
               if (ParseCMDStrNumber(&chat[str_3arg_index], str_4arg_index - str_3arg_index - 1, &coordinates[1]))
               if (ParseCMDStrNumber(&chat[str_4arg_index], chat_Length - str_4arg_index - 1, &coordinates[2]))
               {
                  short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
                  if (ParseCMDStrPlayers(&chat[3], player_index, pi_found))
                  {
                     for (int i = 0; i < pi_found; i++)
                     {
                        HaloCE_lib::SPARTAN *player_object = GetPlayerObj(player_index[i]);
                        if (player_object)
                        {        
                           //if player is in a vehicle, use vehicle's coordinates
                           HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
                           if (vehicle_object)
                              player_object = vehicle_object;
                           //HaloDebugNum((DWORD)player_object, 16);
                           for (int i = 0; i < 3; i++)    
                              player_object->m_World[i] += coordinates[i];

                           erase_chat = true; 
                        }    
                     } 
                  }
                  
                  if (player_index)
                     delete[] player_index;
               }
               
               erase_cmd_terminator = !erase_chat;
            }
            else if (str_cmp(chat, L"/ammo ", 6))
            {
               float number = 0;
                  
               int str_2arg_index = FindCMDArgIndex(chat, 2);
               if (ParseCMDStrNumber(&chat[str_2arg_index], chat_Length - str_2arg_index - 1, &number))
               {
                  short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
                  if (ParseCMDStrPlayers(&chat[6], player_index, pi_found))
                  {
                     for (int i = 0; i < pi_found; i++)
                     { 
                        HaloCE_lib::SPARTAN *player_object = GetPlayerObj(player_index[i]);
                        if (player_object)
                        {
                           HaloCE_lib::WEAPON_OBJECT *weapon_object = (HaloCE_lib::WEAPON_OBJECT*)GetObj(player_object->WeaponIndex);
                           if (weapon_object)
                           { 
                              weapon_object->rounds_total = (WORD)number;
                              erase_chat = true;
                           }
                        }
                     }
                  }
                  if (player_index)
                     delete[] player_index;
               }
               
               erase_cmd_terminator = !erase_chat;
            }
            else if (str_cmp(chat, L"/bat ", 5))
            {
               float number = 0;
                  
               int str_2arg_index = FindCMDArgIndex(chat, 2);
               if (ParseCMDStrNumber(&chat[str_2arg_index], chat_Length - str_2arg_index - 1, &number))
               {
                  short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
                  if (ParseCMDStrPlayers(&chat[5], player_index, pi_found))
                  {
                     for (int i = 0; i < pi_found; i++)
                     { 
                        HaloCE_lib::SPARTAN *player_object = GetPlayerObj(player_index[i]);
                        if (player_object)
                        {
                           HaloCE_lib::WEAPON_OBJECT *weapon_object = (HaloCE_lib::WEAPON_OBJECT*)GetObj(player_object->WeaponIndex);
                           if (weapon_object)
                           {
                              float battery_used = (100 - number)/ 100;
                              weapon_object->battery_used = battery_used;
                              erase_chat = true;
                           }
                        }
                     }
                  }
                  if (player_index)
                     delete[] player_index;
               }
               
               erase_cmd_terminator = !erase_chat;
            }
            else if (str_cmp(chat, L"/eject ", 7))
            {
               short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
               if (ParseCMDStrPlayers(&chat[7], player_index, pi_found))
               {
                  for (int i = 0; i < pi_found; i++)
                  { 
                     HaloCE_lib::SPARTAN *player_object = GetPlayerObj(player_index[i]);
                     if (player_object)
                     {
                        if (player_object->VehicleIndex != -1)
                        {
                           //player_object->AntrMetaIndex = 0xEA3808C4;
                           //player_object->Animation = 0x22; //"W-gunner exit"
                           player_object->BitFlags_02[0] = 0;
                           player_object->BitFlags_01 &= 0xFFFFFFFE;
                           player_object->Action = 0x1B;
                           erase_chat = true;
                        }
                     }
                  }
               }
               if (player_index)
                  delete[] player_index;
               
               erase_cmd_terminator = !erase_chat;
            }
         }
         if (!erase_chat)
         {
            if (str_cmp(chat, L"/camo ", 6))
            {
               float buffer_num = 0; WORD bit_field = 0;
                  
               int str_2arg_index = FindCMDArgIndex(chat, 2);
               if (ParseCMDStrNumber(&chat[str_2arg_index], chat_Length - str_2arg_index - 1, &buffer_num))
               {
                  if (buffer_num == 1)
                     bit_field = 0x51;
                  else if (buffer_num == 0)
                     bit_field = 0x41;
                        
                  short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
                  if (ParseCMDStrPlayers(&chat[6], player_index, pi_found))
                  {
                     for (int i = 0; i < pi_found; i++)
                     { 
                        HaloCE_lib::SPARTAN *player_object = GetPlayerObj(player_index[i]);
                        if (player_object)
                        {
                           player_object->IsInvisible = bit_field;
                           erase_chat = true;
                        }
                     }
                  }
                  
                  erase_cmd_terminator = !erase_chat;
                  if (player_index)
                     delete[] player_index;
               }
            }
            else if (str_cmp(chat, L"/tc ", 4))
            {
               DWORD Team = 0;
               short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
               if (ParseCMDStrPlayers(&chat[4], player_index, pi_found))
               {
                  for (int i = 0; i < pi_found; i++)
                  {
                     Team = players[player_index[i]].Team;
                     if (Team)
                        Team = 0;
                     else
                        Team = 1;
                      
                     players[player_index[i]].Team = Team;
                     erase_chat = true;
                  }
               }
               
               erase_cmd_terminator = !erase_chat;
               if (player_index)
                  delete[] player_index;
            }
            else if (str_cmp(chat, L"/suspend ", 9))
            {
               float number = 0;
               
               int str_2arg_index = FindCMDArgIndex(chat, 2);
               if (ParseCMDStrNumber(&chat[str_2arg_index], chat_Length - str_2arg_index - 1, &number))
               {
                  short *player_index = new short[Players_Table_Header->NumOfItems](); int pi_found = 0;
                  if (ParseCMDStrPlayers(&chat[9], player_index, pi_found))
                  {
                     for (int i = 0; i < pi_found; i++)
                     {
                        HaloCE_lib::SPARTAN *player_object = GetPlayerObj(player_index[i]);
                        if (player_object)
                        {
                           player_object->IsSuspended = (BYTE)number;
                           erase_chat = true;
                        }
                     }
                  }
                  
                  erase_cmd_terminator = !erase_chat;
                  if (player_index)
                     delete[] player_index;
               }
            }
            else if (str_cmp(chat, L"/rapid_fire ", 12))
            {
               float number = 0;
               if (ParseCMDStrNumber(&chat[12], chat_Length - 12 - 1, &number))
               {
                  ckbx_rapid_fire_CheckedChanged((BYTE)number);
                  erase_chat = true;
               }
            }
            else if (str_cmp(chat, L"/time_freeze ", 13))
            {
               float number = 0;
               if (ParseCMDStrNumber(&chat[13], chat_Length - 13 - 1, &number))
               {
                  ckbx_time_freeze_CheckedChanged((BYTE)number);
                  erase_chat = true;
               }
               
               erase_cmd_terminator = !erase_chat;
            }
            else if (str_cmp(chat, L"/grav_boots ", 12))
            {
               float number = 0;
               if (ParseCMDStrNumber(&chat[12], chat_Length - 12 - 1, &number))
               {
                  ckbx_grav_boots_CheckedChanged((BYTE)number);
                  erase_chat = true;
               }
               
               erase_cmd_terminator = !erase_chat;
            }
            else if (str_cmp(chat, L"/vehicle_ntr ", 13))
            {
               float number = 0;
               if (ParseCMDStrNumber(&chat[13], chat_Length - 13 - 1, &number))
               {
                  ckbx_vehicle_ntr_CheckedChanged((BYTE)number);
                  erase_chat = true;
               }
               
               erase_cmd_terminator = !erase_chat;
            }
         }
      }
      if (str_cmp(Current_Map_Str, "rpg_beta6_2"))
      {
         //keyboard shortcuts
         if (dll_sk_enabled.rpgb62_Enable_Shrtcts && *ServerType == HOST)
         {
            //setting
            if (IsKeyDown(rpg_beta6_2_cmd_keys[0].toggle_key))
            {
               if (*(WORD*)rpg_beta6_2_cmd_data[0].cmd_address != 2)
                  *(WORD*)rpg_beta6_2_cmd_data[0].cmd_address += 1;
               else
                  *(WORD*)rpg_beta6_2_cmd_data[0].cmd_address -= 2;
                  
               ::Sleep(200);
            }
            //alarm toggle
            if (IsKeyDown(rpg_beta6_2_cmd_keys[1].toggle_key))
            {
               *(bool*)(Device_Groups_Header->ItemArray_ptr 
               + HCE_Lib::rpg_beta6_2_device_groups::alarm_control_2_offset) = true;
               
               ::Sleep(200);
            }
            //alarm on
            if (IsKeyDown(rpg_beta6_2_cmd_keys[1].on_key))
            {
               if (!*(bool*)rpg_beta6_2_cmd_data[1].cmd_address)
                  *(bool*)(Device_Groups_Header->ItemArray_ptr 
                     + HCE_Lib::rpg_beta6_2_device_groups::alarm_control_2_offset) = true;
               
               ::Sleep(200);
            }
            //alarm off
            if (IsKeyDown(rpg_beta6_2_cmd_keys[1].off_key))
            {
               if (*(bool*)rpg_beta6_2_cmd_data[1].cmd_address)
                  *(bool*)(Device_Groups_Header->ItemArray_ptr 
                     + HCE_Lib::rpg_beta6_2_device_groups::alarm_control_2_offset) = true;
               
               ::Sleep(200);
            }
            //lockdown
            if (IsKeyDown(rpg_beta6_2_cmd_keys[2].toggle_key))
            {
               if (!*(bool*)rpg_beta6_2_cmd_data[2].cmd_address)
                  *(bool*)(Device_Groups_Header->ItemArray_ptr
                     + HCE_Lib::rpg_beta6_2_device_groups::lock_control_offset) = true;
               else
               {
                  //Halo_Process.WriteMemBool(HCE_Lib.Locked, false);
               }
               
               ::Sleep(200);
            }
            
            //fire halo
            if (IsKeyDown(rpg_beta6_2_cmd_keys[3].toggle_key))
            {
               if (!*(bool*)rpg_beta6_2_cmd_data[3].cmd_address)
                  *(bool*)(Device_Groups_Header->ItemArray_ptr 
                     + HCE_Lib::rpg_beta6_2_device_groups::boom_control_offset) = true;
               else
               {
                  
               }
               
               ::Sleep(200);
            }
         }
         
         //check for rpg_beta chat commands//
         if (chat[0] == '/' && !erase_chat && *ServerType == HOST && chat[chat_Length - 1] == ';')
            {      
               //change setting to day
               if (str_cmp(chat, rpg_beta6_2_cmd_strs[0].cmd_activate))
               {
                  erase_chat = true;
                  *(WORD*)rpg_beta6_2_cmd_data[0].cmd_address = 0;
               }
               //change setting to rain
               else if (str_cmp(chat, rpg_beta6_2_cmd_strs[0].cmd_on))
               {
                  erase_chat = true;
                  *(WORD*)rpg_beta6_2_cmd_data[0].cmd_address = 1;
               }
               //change setting to night
               else if (str_cmp(chat, rpg_beta6_2_cmd_strs[0].cmd_off))
               {
                  erase_chat = true;
                  *(WORD*)rpg_beta6_2_cmd_data[0].cmd_address = 2;
               }
               //turn alarm on
               else if (str_cmp(chat, rpg_beta6_2_cmd_strs[1].cmd_on))
               {
                  erase_chat = true;
                  if (!*(bool*)rpg_beta6_2_cmd_data[1].cmd_address)
                     *(bool*)(Device_Groups_Header->ItemArray_ptr 
                        + HCE_Lib::rpg_beta6_2_device_groups::alarm_control_2_offset) = true;
               }
               //turn alarm off
               else if (str_cmp(chat, rpg_beta6_2_cmd_strs[1].cmd_off))
               {
                  erase_chat = true;
                  if (*(bool*)rpg_beta6_2_cmd_data[1].cmd_address)
                     *(bool*)(Device_Groups_Header->ItemArray_ptr 
                        + HCE_Lib::rpg_beta6_2_device_groups::alarm_control_2_offset) = true;
               }
               //trigger lockdown
               else if (str_cmp(chat, rpg_beta6_2_cmd_strs[2].cmd_activate))
               {
                  erase_chat = true;
                  if (!*(bool*)rpg_beta6_2_cmd_data[2].cmd_address)
                     *(bool*)(Device_Groups_Header->ItemArray_ptr
                        + HCE_Lib::rpg_beta6_2_device_groups::lock_control_offset) = true;
               }
               //fire halo
               else if (str_cmp(chat, rpg_beta6_2_cmd_strs[3].cmd_activate))
               {
                  erase_chat = true;
                  if (!*(bool*)rpg_beta6_2_cmd_data[3].cmd_address)
                     *(bool*)(Device_Groups_Header->ItemArray_ptr 
                        + HCE_Lib::rpg_beta6_2_device_groups::boom_control_offset) = true;
               }
               ///lockdown timer <num> 
               if (str_cmp(chat, L"/lockdown timer ", 16))
               {
                  float number = 0;
                  if (ParseCMDStrNumber(&chat[16], chat_Length - 16 - 1, &number))
                  {
                     *(short*)rpg_beta6_2_cmd_data[4].cmd_address = (short)number * 30;
                     erase_chat = true;
                  }
                  
                  erase_cmd_terminator = !erase_chat;
               }
               ///halo timer <num>
               if (str_cmp(chat, L"/halo timer ", 12))
               {
                  float number = 0;
                  if (ParseCMDStrNumber(&chat[12], chat_Length - 12 - 1, &number))
                  {
                     *(short*)rpg_beta6_2_cmd_data[5].cmd_address = (short)number * 30;
                     erase_chat = true;
                  }
                  
                  erase_cmd_terminator = !erase_chat;
               }
            
         }
      }
      if (erase_chat)
      {
         if (!*Console_chck)
            *(wchar_t*)(Chat_address + (chat_Length - 1) * sizeof(wchar_t)) = L'\0';
         else
            *(char*)(Chat_address + (chat_Length - 1) * sizeof(char)) = '\0';
            
         PressKey(VK_RETURN, 2);
         *(wchar_t*)Chat_address = L'\0';
         chat[0] = '\0';
         erase_chat = false;
      }
      else if (erase_cmd_terminator)
      {
         PressKey(VK_BACK, 1);
         erase_cmd_terminator = false;
      }
	}
	
	delete[] LocationsFilePath;
   return 1;
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
	   //wait for HDC_thread to exit
	   ::Sleep(50);
		break;
	}
	return TRUE;
}