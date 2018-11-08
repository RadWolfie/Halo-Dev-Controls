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
	File:    dllmain.h
	Project: HDC
	Author:  Jesus7Freak, RadWolfie
	Date:    11/30/2016
	Game:    Halo and Halo Custom Edition
	Version: all
*********************************************************************************/
#pragma once
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "libcpmt.lib")
#pragma comment(lib, "LIBCMT.lib")

#include "../Halo Dev Controls/HaloLib.h"
#include "../Halo Dev Controls/cmds.h"
#include "../Halo Dev Controls/tele_loc_resources.h"

using namespace ReadWriteMemoryTools;

#define SV_NAME_SIZE 14
#define SV_BUFFER_SIZE 128

#define TOGGLE 2

#define MAIN_MENU 0
#define CLIENT 1
#define HOST 2

#define DO_NOT_SEND_MSG 0x100 

enum HALO_TYPE : WORD { halo = 1, haloce = 2} extern running_gt;

//Global = msg + players name, Team & Vehicle = msg + players name in brackets, Server = msg
enum CHAT_TYPE : int { Global = 0, Team = 1, Vehicle = 2, Server = 3 };

struct CHAT_INFO
{
   CHAT_TYPE ChatType;
   char From_PlayerIndex;
   wchar_t *msg_str;
};

struct DLL_ADDRESSES
{
   char find_me_str[12];//set to "dll_st_addrs"
   HWND hHaloWin;
   CMDsLib::CMD_SCKEYS **halo_cmd_keys;
   CMDsLib::CMD_SCKEYS **rpgb62_cmd_keys;
   BOOL *halo_sk_enabled;
   BOOL *rpg_beta62_sk_enabled;
   bool (__stdcall *pFunc)(int);
   wchar_t *CurrentDir;
   WORD *game;
   bool locations_changed;
}extern dll_addresses;

extern bool exit_thread,
   rpgb6_2_running,
   *Console_chck;
   
extern char *Current_Map_Str;

extern wchar_t *LocationsFilePath,
   *Locations_File_Name,
   HaloSay_server_buffer[SV_NAME_SIZE+SV_BUFFER_SIZE];
   
extern BYTE *Dev_enabled,
   *Console_enabled,
   *ShowHud,
   *LetterBox,
   *RiderEjection;

extern short *ServerType,
   *Player0_index;
   
extern BOOL DestroyObjsEnabled;

extern DWORD Current_Map_address,
   Rasterizer_address,
   EngineDrawText_address,
   Console_func_address,
   MsgInfo_ptr_address,
   ServerInfo_ptr_address,
   SendMsgToAll_func_address,
   PrepMsg_func_address,
   SendMsgToPlayer_func_address,
   Console_hook_address,
   Rcon_hook_address,
   C_Buffers_address,
   C_BuffersIndex_address,
   Server_chat_hook_address,
   PlayerCheck_Func_address,
   rapid_fire_hook_address,
   time_freeze_hook_address,
   grav_boots_hook_address,
   vehicle_ntr_hook_address,
   UnitExitVehicle_func_address,
   SpawnHog_func_address,
   ObjTagList_ptr_address,
   SpawnObjAtPlayer0_func_address,
   CreateObj_func_address,
   DestroyObj_func_address,
   ActiveCamo_func_address,
   PlayerDeath_func_address,
   sv_kick_func_address,
   sv_ban_func_address,
   cse_set_video_func_address,
   cinematic_ptr,
   console_tablist_hook_address,
   console_tablist_hookfunc_address,
   ctl_array_index_address,
   ctl_array_size_address,
   ctl_filter_str_ptr_address,
   TempAdmin[16];
   
extern float *game_speed;

extern HaloCE_lib::CHEATS *cheats;
extern HaloCE_lib::RASTERIZER *rasterizer;

extern HaloCE_lib::DATA_HEADER **Players_ptr, 
   **Device_Groups_ptr, 
   **Object_ptr,
   **HS_Globals_ptr;
   
extern HaloCE_lib::STATIC_PLAYER *players;
extern HaloCE_lib::OBJECT_TABLE_ARRAY *objects;

extern HANDLE hHDC_thread;

extern std::vector<MAPS> maps_tele_sites;

bool str_cmpAW(char *str1, wchar_t *wstr2, int length);

void HaloSay(wchar_t *message, short to_player_index, ...);
void HaloDrawText(char *msg_str, BYTE alpha, BYTE red, BYTE green, BYTE blue);
void ServerSay(CHAT_INFO chat_info, char to_player);

BOOL __fastcall ckbx_rapid_fire_CheckedChanged(BYTE Checked);
BOOL __fastcall ckbx_time_freeze_CheckedChanged(BYTE Checked);
BOOL __fastcall ckbx_grav_boots_CheckedChanged(BYTE Checked);
BOOL __fastcall ckbx_vehicle_ntr_CheckedChanged(BYTE Checked);
bool __stdcall MV_chkBx_CheckedChanged(int number);

void CheckCmdScKeys();
void DestroyObjsCheck();
DWORD WINAPI HDC_thread(LPVOID hModule);
//functions calling this need to test if first CMD_CALL_INFO::cmd[0] == '/'
DWORD __fastcall CMDParser(wchar_t *cmd_str, short exec_player_index);

namespace Halo
{
   BOOL __fastcall CommandHelp(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall ListCommands(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall ListTeleportLocs(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall EnableConsole(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall EnableDevMode(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall CheatsDeathless(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall CheatsInfiniteAmmo(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall CheatsBottomlessClip(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall ShowHudFunc(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall LetterBoxFunc(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall RiderEjectionFunc(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall CheatsOmnipotent(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall CheatsJetPack(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall CheatsBumpPossession(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall CheatsSuperJump(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall CheatsReflexiveDamage(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall CheatsMedusa(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall CheatsXboxController(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall ShowWireFrame(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall ShowFog(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall ShowFogPlane(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall ShowFPS(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Game_Speed(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Rapid_Fire(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Time_Freeze(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Grav_Boots(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Vehicle_NTR(wchar_t *cmd_args, short exec_player_index);
   //BOOL __fastcall Execute_Console_Func(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Marines_HUD(wchar_t *cmd_args, short exec_player_index);
};

namespace RPG
{
   BOOL __fastcall Environment_Day(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Environment_Rain(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Environment_Night(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall AirBase_Alarm(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall AirBase_LockDown(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Fire_Halo(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall LockDown_Timer(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Halo_Timer(wchar_t *cmd_args, short exec_player_index);
};

namespace Player
{
   BOOL __fastcall Speed(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall ActiveCamo(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Suspend(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Teleport(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Jump_Teleport(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Velocity(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Ammo(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Battery(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Health(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Shield(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall AFK(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Team_Change(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Kick(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Ban(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Kill(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Eject(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Flip_Vehicle(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Admin(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Set_Teleport_Loc(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Spawn_Biped(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Spawn_Hog(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Spawn_All_Vehicles(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Spawn_All_Weapons(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Spawn_All_Powerups(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Copy_Vehicle(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Copy_Weapon(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Destroy_Objects_Mode(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Destroy_Weapon(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall Say(wchar_t *cmd_args, short exec_player_index);
   BOOL __fastcall ObjectScale(wchar_t *cmd_args, short exec_player_index);
};
//hooks
void ConsoleHook();
void ChatHook();
void ConsoleTablistHook();
