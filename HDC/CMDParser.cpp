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
    File:    CMDParser.cpp
    Project: HDC
    Author:  Jesus7Freak
    Date:    11/22/2011
    Game:    Halo and Halo Custom Edition
    Version: all
*********************************************************************************/
#include "dllmain.h"

wchar_t *FailLocalCmd = L"Failed: Local Command Only.",
   *FailSvCmd = L"Failed: Server Command Only.",
   *FailInvalNum = L"Failed: Invalid number(s).",
   *FailInvalBool = L"Failed: Invalid boolean.",
   *FailBadSpawn = L"Failed: Unable to create Object(s) next to %s",
   //*FailBadExec = L"Failed: Execution failed, is dev enabled?.",
   *FailPlyrNtFnd = L"Failed: Player(s) not found.",
   *FailBadTeleLoc = L"Failed: Teleport location is not defined.",
   *FailMissingLoc = L"Failed: Missing teleport location name.",
   *FailNoSpaces = L"Failed: No spaces allowed in teleport location name.",
   *FailLowAdminLvl = L"Failed: %s has a higher admin level.",
   *FailPlyrNtSpwn = L"Failed: %s hasn't respawned.",
   *FailPlyrNtInVeh = L"Failed: %s is not in a vehicle.",
   *FailPlyrNoWep = L"Failed: %s does not have a weapon.";

//kill messages
wchar_t *SuccededKillMsgs[8] =
{
   L"%s was given lethal injection",
   L"%s was sent to a gas chamber",
   L"%s was killed by firing squad",
   L"%s was killed by hanging",
   L"%s was killed by The Guardians",
   L"THIS IS SPARTA!!!!!!!!! (%s gets kicked into a bottomless pit)",
   L"%s was killed by a vehicle",
   L"GIVE UP GIVE UP AND FEED THE MACHINE!!!!!!! (%s was fed to the machine)"
};

typedef BOOL (__fastcall *CmdFunc)(wchar_t *cmd_args, short exec_player_index);
CmdFunc HaloCmdFuncs[HALO_CMDS_SIZE] =
{
   Halo::CommandHelp,
   Halo::ListCommands,
   Halo::ListTeleportLocs,
   Halo::EnableConsole,
   Halo::EnableDevMode,
   Halo::CheatsDeathless,
   Halo::CheatsInfiniteAmmo,
   Halo::CheatsBottomlessClip,
   Halo::ShowHudFunc,
   Halo::LetterBoxFunc,
   Halo::RiderEjectionFunc,
   Halo::CheatsOmnipotent,
   Halo::CheatsJetPack,
   Halo::CheatsBumpPossession,
   Halo::CheatsSuperJump,
   Halo::CheatsMedusa,
   Halo::CheatsReflexiveDamage,
   Halo::CheatsXboxController,
   Halo::ShowWireFrame,
   Halo::ShowFog,
   Halo::ShowFogPlane,
   Halo::ShowFPS,
   Halo::Game_Speed,
   Halo::Rapid_Fire,
   Halo::Time_Freeze,
   Halo::Grav_Boots,
   Halo::Vehicle_NTR,
   Halo::Marines_HUD
};

CmdFunc RpgCmdFuncs[RPGB_CMDS_SIZE] =
{
   RPG::Environment_Day,
   RPG::Environment_Rain,
   RPG::Environment_Night,
   RPG::AirBase_Alarm,
   RPG::AirBase_LockDown,
   RPG::Fire_Halo,
   RPG::LockDown_Timer,
   RPG::Halo_Timer
};

CmdFunc PlayerCmdFuncs[PLAYER_CMDS_SIZE] =
{
   Player::Speed,
   Player::ActiveCamo,
   Player::Suspend,
   Player::Teleport,
   Player::Jump_Teleport,
   Player::Velocity,
   Player::Ammo,
   Player::Battery,
   Player::Health,
   Player::Shield,
   Player::AFK,
   Player::Team_Change,
   Player::Kick,
   Player::Ban,
   Player::Kill,
   Player::Eject,
   Player::Flip_Vehicle,
   Player::Admin,
   Player::Set_Teleport_Loc,
   Player::Spawn_Biped,
   Player::Spawn_Hog,
   Player::Spawn_All_Vehicles,
   Player::Spawn_All_Weapons,
   Player::Spawn_All_Powerups,
   Player::Copy_Vehicle,
   Player::Copy_Weapon,
   Player::Destroy_Objects_Mode,
   Player::Destroy_Weapon,
   Player::Say,
   Player::ObjectScale
};

CmdFunc *AllCmdFuncs[CMD_SET_SIZE] =
{
   PlayerCmdFuncs,
   HaloCmdFuncs,
   RpgCmdFuncs
};

int ParseCMDStrPlayers(wchar_t *cmd_str, short* player_index_array, int &pi_found)
{
   int max_players_to_find, wchars_processed = 0;
   HaloCE_lib::DATA_HEADER *Players = *Players_ptr;
   int NumOfPlayers = Players->NumOfItems;

   if (pi_found)
   {
      max_players_to_find = pi_found;
      pi_found = 0;
   }
   else
      max_players_to_find = NumOfPlayers;

   if (*cmd_str == L'\"')
   {
      cmd_str++;
      wchars_processed += 2;
   }

   for (int pi = 0, vpi = 0; pi_found < max_players_to_find && vpi < NumOfPlayers; pi++)
   {
      if (!players[pi].PlayerID) continue;
      else vpi++;

      int j = 0;
      wchar_t *player_name = players[pi].PlayerName0;
      bool str_contain_search = false;
      for (int i = 0; i < HaloCE_lib::PlayerNameMaxSize; i++)
      {
         wchar_t cmd_str_wchar = cmd_str[j];

         if (cmd_str_wchar == L'*')
         {
            wchar_t next_wchar = cmd_str[j + 1];
            if (!next_wchar || next_wchar == L' ' || next_wchar == L'\"')
            {
               wchars_processed = j;
               player_index_array[pi_found++] = pi;
               break;
            }
            else
            {
               cmd_str_wchar = cmd_str[++j];
               str_contain_search = true;
            }
         }

         if (cmd_str_wchar == L'?') continue;

         if (cmd_str_wchar != player_name[i])
         {
            if (!str_contain_search) break;
            else continue;
         }

         if (!player_name[i + 1])
         {
            wchars_processed = j;
            player_index_array[pi_found++] = pi;
            break;
         }

         j++;
      }
   }

   if (pi_found > 0) wchars_processed += 1;
   else wchars_processed = 0;

   return wchars_processed;
}

//functions calling this need to test if first CMD_CALL_INFO::cmd[0] == '/'
DWORD __fastcall CMDParser(wchar_t *cmd_str, short exec_player_index)
{
   if (!TempAdmin[exec_player_index]) return FALSE;

   int cmd_name_length = 0;
   while (cmd_str[cmd_name_length] && cmd_str[cmd_name_length++] != L' ');

   wchar_t *add_space = &cmd_str[cmd_name_length];
   if (!add_space[0])
   {
      add_space[1] = 0;
      add_space[0] = ' ';
      cmd_name_length++;
   }

   bool found = false;
   BOOL succeded = FALSE;
   for (int cmd_group_i = 0; !found && cmd_group_i < CMD_SET_SIZE; cmd_group_i++)
   {
      CMDsLib::COMMANDS *cmd_group = CMDsLib::all_commands[cmd_group_i];
      char **cmd_strs = cmd_group->cmd_strs;
      int group_size = cmd_group->size;
      CmdFunc *CmdGroupFuncs = AllCmdFuncs[cmd_group_i];

      for (int i = 0; i < group_size; i++)
      {
         //skip the / as it already has been check by the hook
         if (str_cmpAW(&cmd_strs[i][1], cmd_str, cmd_name_length))
         {
            found = true;
            succeded = (*CmdGroupFuncs[i])(&cmd_str[cmd_name_length], exec_player_index);
            break;
         }
      }
   }

   return succeded;
}

BOOL __fastcall Halo::CommandHelp(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   //uses the space like a null terminator
   short cmd_arg_length = 0; while (cmd_args[cmd_arg_length]) cmd_arg_length++;
   cmd_args[cmd_arg_length + 1] = 0;
   cmd_args[cmd_arg_length] = ' ';

   wchar_t *chat_header = NULL, *chat_usage = NULL, *chat_descript = NULL;

   for (int cmd_group_i = 0; !chat_header && cmd_group_i < 3; cmd_group_i++)
   {
      CMDsLib::COMMANDS *cmd_group = CMDsLib::all_commands[cmd_group_i];
      char **cmd_strs = cmd_group->cmd_strs;
      int group_size = cmd_group->size;

      for (int i = 0; !chat_header && i < group_size; i++)
      {
         if (str_cmpAW(cmd_strs[i], cmd_args, cmd_arg_length))
         {
            CMDsLib::CMD_DESCRIPT *pCDS = cmd_group->cmd_descripts;
            chat_header = pCDS[i].cmd_header;
            chat_usage = pCDS[i].cmd_usage;
            chat_descript = pCDS[i].cmd_descript;
            break;
         }
      }
   }

   if (!chat_header && str_cmpAW("[pExpression] ", cmd_args, cmd_arg_length))
   {
      HaloSay(L"example use of [pExpression]: Shadow, AoO Aurora, N®Þ»Jedi", exec_player_index);
      HaloSay(L"/spd Shadow 4       - normal use", exec_player_index);
      HaloSay(L"/spd \"AoO Aurora\" 4 - use quotes when the name has spaces", exec_player_index);
      HaloSay(L"/spd ????Jedi 4     - use ? when you don't know the character", exec_player_index);
      chat_header = L"/spd *Jedi 4        - all players with Jedi at the end of their name";
      chat_usage = L"/spd AoO* 4         - all players with AoO at the beggining of their name";
      chat_descript = L"/spd * 4            - all players";
   }
   //defualt to help desciption
   else if (!chat_header)
   {
      CMDsLib::CMD_DESCRIPT *pCDS = CMDsLib::halo_cmd_descripts;
      chat_header = pCDS[0].cmd_header;
      chat_usage = pCDS[0].cmd_usage;
   }


   if (chat_header) succeded = TRUE;
   else chat_header = L"Failed: command not found.";

   HaloSay(chat_header, exec_player_index);

   if (chat_usage) HaloSay(chat_usage, exec_player_index);
   if (chat_descript) HaloSay(chat_descript, exec_player_index);

   return succeded;
}

BOOL __fastcall Halo::ListCommands(wchar_t *cmd_args, short exec_player_index)
{
   int list_i = 0;
   wchar_t list_str[128];
   list_str[list_i++] = ' ';

   for (int cmd_group_i = 0; cmd_group_i < 3; cmd_group_i++)
   {
      CMDsLib::COMMANDS *cmd_group = CMDsLib::all_commands[cmd_group_i];
      char **cmd_strs = cmd_group->cmd_strs;
      int group_size = cmd_group->size;

      for (int i = 0; i < group_size; i++)
      {
         char *cmd_str = cmd_strs[i];
         int j = 0;
         char _char = ' ';

         do
         {
            list_str[list_i++] = (wchar_t)_char;
            _char = cmd_str[j++];
         }while (_char);

         if (list_i > 112)
         {
            list_str[list_i] = 0;//add null terminator
            HaloSay(list_str, exec_player_index);
            list_i = 1;
         }
      }

      //print the last line of the group
      if (list_i > 1)
      {
         list_str[list_i] = 0;//add null terminator
         HaloSay(list_str, exec_player_index);
         list_i = 1;
      }
   }

   return TRUE;
}

BOOL __fastcall Halo::ListTeleportLocs(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   int list_i = 0;
   //seems like a good idea
   wchar_t list_str[128];
   list_str[list_i++] = ' ';

   unsigned int map_i = 0;
   if (FindMapIndex(&maps_tele_sites, Current_Map_Str, map_i))
   {
      int loc_num = maps_tele_sites[map_i].teleport_locations.size();
      TELEPORT_LOCATION *locs = &maps_tele_sites[map_i].teleport_locations[0];

      for (int loc_i = 0; loc_i < loc_num; loc_i++)
      {
         wchar_t *loc_str = locs[loc_i].teleport_loc_name;
         int j = 0;
         wchar_t _wchar = L' ';

         do
         {
            list_str[list_i++] = _wchar;
            _wchar = loc_str[j++];
         }while (_wchar);

         //extra padding
         list_str[list_i++] = L' ';

         if (list_i > 112)
         {
            list_str[list_i] = 0;//add null terminator
            HaloSay(list_str, exec_player_index);
            list_i = 1;
         }
      }

      //print the last line of the group
      if (list_i > 1)
      {
         list_str[list_i] = 0;//add null terminator
         HaloSay(list_str, exec_player_index);
      }
      succeded = TRUE;
   }
   else
      HaloSay(
         L"Failed: Their are no teleport locations defined, for this map.",
         exec_player_index);//, Current_Map_Str); is not a wchar_t*

   return succeded;
}

BOOL __fastcall Halo::EnableConsole(wchar_t *cmd_args, short exec_player_index)
{
   //host only cmd
   if (exec_player_index)
   {
      HaloSay(FailLocalCmd, exec_player_index);
      return FALSE;
   }

   BOOL succeded = FALSE;
   BOOL _bool;

   if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      *Console_enabled = (BYTE)_bool;
      succeded = TRUE;
   }
   else
      HaloSay(FailInvalBool, exec_player_index);

   return succeded;
}

BOOL __fastcall Halo::EnableDevMode(wchar_t *cmd_args, short exec_player_index)
{
   if (running_gt != haloce)
   {
      HaloSay(L"Failed: Halo Custom Edition only command", exec_player_index);
      return FALSE;
   }

   //host only cmd
   if (exec_player_index)
   {
      HaloSay(FailLocalCmd, exec_player_index);
      return FALSE;
   }

   BOOL succeded = FALSE;
   BOOL _bool;

   if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      *Dev_enabled = (BYTE)_bool;
      succeded = TRUE;
   }
   else
      HaloSay(FailInvalBool, exec_player_index);

   return succeded;
}

BOOL __fastcall Halo::CheatsDeathless(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   BOOL _bool;
   if (!*cmd_args)
   {
      _bool = cheats->Deathless;
      GenericMsg = L"Deathless is set at %i";
   }
   else if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      cheats->Deathless = (BYTE)_bool;
      succeded = TRUE;
      GenericMsg = L"Deathless has been set to %i";
   }
   else
      GenericMsg = FailInvalBool;

   HaloSay(GenericMsg, exec_player_index, _bool);

   return succeded;
}

BOOL __fastcall Halo::CheatsInfiniteAmmo(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   BOOL _bool;
   if (!*cmd_args)
   {
      _bool = cheats->Infinite_Ammo;
      GenericMsg = L"Infinite Ammo is set at %i";
   }
   else if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      cheats->Infinite_Ammo = (BYTE)_bool;
      succeded = TRUE;
      GenericMsg = L"Infinite Ammo has been set to %i";
   }
   else
      GenericMsg = FailInvalBool;

   HaloSay(GenericMsg, exec_player_index, _bool);

   return succeded;
}

BOOL __fastcall Halo::CheatsBottomlessClip(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   BOOL _bool;
   if (!*cmd_args)
   {
      _bool = cheats->Bottomless_Clip;
      GenericMsg = L"Bottomless Clip is set at %i";
   }
   else if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      cheats->Bottomless_Clip = (BYTE)_bool;
      succeded = TRUE;
      GenericMsg = L"Bottomless Clip has been set to %i";
   }
   else
      GenericMsg = FailInvalBool;

   HaloSay(GenericMsg, exec_player_index, _bool);

   return succeded;
}

BOOL __fastcall Halo::ShowHudFunc(wchar_t *cmd_args, short exec_player_index)
{
   //host only cmd
   if (exec_player_index)
   {
      HaloSay(FailLocalCmd, exec_player_index);
      return FALSE;
   }

   BOOL succeded = FALSE;
   BOOL _bool;

   if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      *ShowHud = (BYTE)_bool;
      succeded = TRUE;
   }
   else
      HaloSay(FailInvalBool, exec_player_index);

   return succeded;
}

BOOL __fastcall Halo::LetterBoxFunc(wchar_t *cmd_args, short exec_player_index)
{
   //host only cmd
   if (exec_player_index)
   {
      HaloSay(FailLocalCmd, exec_player_index);
      return FALSE;
   }

   BOOL succeded = FALSE;
   BOOL _bool;

   if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      *LetterBox = (BYTE)_bool;
      succeded = TRUE;
   }
   else
      HaloSay(FailInvalBool, exec_player_index);

   return succeded;
}

BOOL __fastcall Halo::RiderEjectionFunc(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   BOOL _bool;
   if (!*cmd_args)
   {
      _bool = *RiderEjection;
      GenericMsg = L"Rider Ejection is set at %i";
   }
   else if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      *RiderEjection = (BYTE)_bool;
      succeded = TRUE;
      GenericMsg = L"Rider Ejection has been set to %i";
   }
   else
      GenericMsg = FailInvalBool;

   HaloSay(GenericMsg, exec_player_index, _bool);

   return succeded;
}

BOOL __fastcall Halo::CheatsOmnipotent(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   BOOL _bool;
   if (!*cmd_args)
   {
      _bool = cheats->Omnipotent;
      GenericMsg = L"Omnipotent is set at %i";
   }
   else if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      cheats->Omnipotent = (BYTE)_bool;
      succeded = TRUE;
      GenericMsg = L"Omnipotent has been set to %i";
   }
   else
      GenericMsg = FailInvalBool;

   HaloSay(GenericMsg, exec_player_index, _bool);

   return succeded;
}

BOOL __fastcall Halo::CheatsJetPack(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   BOOL _bool;
   if (!*cmd_args)
   {
      _bool = !cheats->JetPack;
      GenericMsg = L"Fall Damage is set at %i";
   }
   else if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      cheats->JetPack = !(BYTE)_bool;
      succeded = TRUE;
      GenericMsg = L"Fall Damage has been set to %i";
   }
   else
      GenericMsg = FailInvalBool;

   HaloSay(GenericMsg, exec_player_index, _bool);

   return succeded;
}

BOOL __fastcall Halo::CheatsBumpPossession(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   BOOL _bool;
   if (!*cmd_args)
   {
      _bool = cheats->Bmp_Possession;
      GenericMsg = L"Bump Possession is set at %i";
   }
   else if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      cheats->Bmp_Possession = (BYTE)_bool;
      succeded = TRUE;
      GenericMsg = L"Bump Possession has been set to %i";
   }
   else
      GenericMsg = FailInvalBool;

   HaloSay(GenericMsg, exec_player_index, _bool);

   return succeded;
}

BOOL __fastcall Halo::CheatsSuperJump(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   BOOL _bool;
   if (!*cmd_args)
   {
      _bool = cheats->Super_jump;
      GenericMsg = L"Super Jump is set at %i";
   }
   else if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      cheats->Super_jump = (BYTE)_bool;
      succeded = TRUE;
      GenericMsg = L"Super Jump has been set to %i";
   }
   else
      GenericMsg = FailInvalBool;

   HaloSay(GenericMsg, exec_player_index, _bool);

   return succeded;
}

BOOL __fastcall Halo::CheatsReflexiveDamage(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   BOOL _bool;

   if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      cheats->Reflexive_damage = (BYTE)_bool;
      succeded = TRUE;
   }
   else
      HaloSay(FailInvalBool, exec_player_index);

   return succeded;
}

BOOL __fastcall Halo::CheatsMedusa(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   BOOL _bool;
   if (!*cmd_args)
   {
      _bool = cheats->Medusa;
      GenericMsg = L"Medusa is set at %i";
   }
   else if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      cheats->Medusa = (BYTE)_bool;
      succeded = TRUE;
      GenericMsg = L"Medusa has been set to %i";
   }
   else
      GenericMsg = FailInvalBool;

   HaloSay(GenericMsg, exec_player_index, _bool);

   return succeded;
}

BOOL __fastcall Halo::CheatsXboxController(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   BOOL _bool;

   if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      cheats->Controller = (BYTE)_bool;
      succeded = TRUE;
   }
   else
      HaloSay(FailInvalBool, exec_player_index);

   return succeded;
}

BOOL __fastcall Halo::ShowWireFrame(wchar_t *cmd_args, short exec_player_index)
{
   //host only cmd
   if (exec_player_index)
   {
      HaloSay(FailLocalCmd, exec_player_index);
      return FALSE;
   }

   BOOL succeded = FALSE;
   BOOL _bool;

   if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      rasterizer->WireFrame = (BYTE)_bool;
      succeded = TRUE;
   }
   else
      HaloSay(FailInvalBool, exec_player_index);

   return succeded;
}

BOOL __fastcall Halo::ShowFog(wchar_t *cmd_args, short exec_player_index)
{
   //host only cmd
   if (exec_player_index)
   {
      HaloSay(FailLocalCmd, exec_player_index);
      return FALSE;
   }

   BOOL succeded = FALSE;
   BOOL _bool;

   if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      rasterizer->FogAtmosphere = (BYTE)_bool;
      succeded = TRUE;
   }
   else
      HaloSay(FailInvalBool, exec_player_index);

   return succeded;
}

BOOL __fastcall Halo::ShowFogPlane(wchar_t *cmd_args, short exec_player_index)
{
   //host only cmd
   if (exec_player_index)
   {
      HaloSay(FailLocalCmd, exec_player_index);
      return FALSE;
   }

   BOOL succeded = FALSE;
   BOOL _bool;

   if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      rasterizer->FogPlane = (BYTE)_bool;
      succeded = TRUE;
   }
   else
      HaloSay(FailInvalBool, exec_player_index);

   return succeded;
}

BOOL __fastcall Halo::ShowFPS(wchar_t *cmd_args, short exec_player_index)
{
   //host only cmd
   if (exec_player_index)
   {
      HaloSay(FailLocalCmd, exec_player_index);
      return FALSE;
   }

   BOOL succeded = FALSE;
   BOOL _bool;

   if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      rasterizer->FPS = (BYTE)_bool;
      succeded = TRUE;
   }
   else
      HaloSay(FailInvalBool, exec_player_index);

   return succeded;
}

BOOL __fastcall Halo::Game_Speed(wchar_t *cmd_args, short exec_player_index)
{
   //host only cmd
   if (exec_player_index)
   {
      HaloSay(FailLocalCmd, exec_player_index);
      return FALSE;
   }

   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   float fnumber;
   if (!*cmd_args)
   {
      fnumber = *game_speed;
      GenericMsg = L"Game Speed is set at %.2f";
   }
   else if (CMDsLib::ParseStrFloat(cmd_args, &fnumber))
   {
      *game_speed = fnumber;
      succeded = TRUE;
      GenericMsg = L"Game Speed has been set to %.2f";
   }
   else
      GenericMsg = FailInvalBool;

   HaloSay(GenericMsg, exec_player_index, fnumber);

   return succeded;
}

BOOL __fastcall Halo::Rapid_Fire(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   BOOL _bool;
   if (!*cmd_args)
   {
      _bool = ckbx_rapid_fire_CheckedChanged(-1);
      GenericMsg = L"Rapid Fire is set at %i";
   }
   else if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      ckbx_rapid_fire_CheckedChanged((BYTE)_bool);
      succeded = TRUE;
      GenericMsg = L"Rapid Fire has been set to %i";
   }
   else
      GenericMsg = FailInvalBool;

   HaloSay(GenericMsg, exec_player_index, _bool);

   return succeded;
}

BOOL __fastcall Halo::Time_Freeze(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   BOOL _bool;
   if (!*cmd_args)
   {
      _bool = ckbx_time_freeze_CheckedChanged(-1);
      GenericMsg = L"Time Freeze is set at %i";
   }
   else if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      ckbx_time_freeze_CheckedChanged((BYTE)_bool);
      succeded = TRUE;
      GenericMsg = L"Time Freeze has been set to %i";
   }
   else
      GenericMsg = FailInvalBool;

   HaloSay(GenericMsg, exec_player_index, _bool);

   return succeded;
}

BOOL __fastcall Halo::Grav_Boots(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   BOOL _bool;
   if (!*cmd_args)
   {
      _bool = ckbx_grav_boots_CheckedChanged(-1);
      GenericMsg = L"Grav Boots is set at %i";
   }
   else if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      ckbx_grav_boots_CheckedChanged((BYTE)_bool);
      succeded = TRUE;
      GenericMsg = L"Grav Boots has been set to %i";
   }
   else
      GenericMsg = FailInvalBool;

   HaloSay(GenericMsg, exec_player_index, _bool);

   return succeded;
}

BOOL __fastcall Halo::Vehicle_NTR(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   BOOL _bool;
   if (!*cmd_args)
   {
      _bool = ckbx_vehicle_ntr_CheckedChanged(-1);
      GenericMsg = L"Vehicle Team Restriction is set at %i";
   }
   else if (CMDsLib::ParseStrBool(cmd_args, &_bool))
   {
      ckbx_vehicle_ntr_CheckedChanged((BYTE)_bool);
      succeded = TRUE;
      GenericMsg = L"Vehicle Team Restriction has been set to %i";
   }
   else
      GenericMsg = FailInvalBool;

   HaloSay(GenericMsg, exec_player_index, _bool);

   return succeded;
}

/*BOOL __stdcall Halo::Execute_Console_Func(CMD_CALL_INFO *pCCI)
{
   BOOL succeded = FALSE;
   char *console_cmd = &Chat_Buffer_64A[cmd_info_array_index][pCCI->cmd_name_length];
   //for names with quotes
   if (*console_cmd == '\"')
   {
      int last_char_i = pCCI->cmd_length - pCCI->cmd_name_length - 1;
      if (console_cmd[last_char_i] == '\"')
      {
         console_cmd[last_char_i] = 0;
         console_cmd++;
      }
   }

   //this would be bad if this was allowed
   if (!str_cmpA(console_cmd, "quit"))
   {
       //if (!Console(cmd))
       //  HaloSay(FailBadExec);
       __asm
      {
         PUSH 0
         //compiler puts this on the stack, not a register...
         MOV EDI,[console_cmd];
         CALL DWORD PTR [Console_func_address]
         ADD ESP,4
         TEST AL,AL
         JNE SHORT console_succeded
      }

      HaloSay(FailBadExec, exec_player_index);

      __asm
      {
         console_succeded:
         MOV succeded,TRUE
      }
   }
   return succeded;
}*/

BOOL __fastcall Halo::Marines_HUD(wchar_t *cmd_args, short exec_player_index)
{
   //host only cmd
   if (exec_player_index)
   {
      HaloSay(FailLocalCmd, exec_player_index);
      return FALSE;
   }

   BOOL succeded = FALSE;

   int number = 0;
   if (CMDsLib::ParseStrInt(cmd_args, &number))
   {
      if (MV_chkBx_CheckedChanged(number))
         succeded = TRUE;
   }
   else
      HaloSay(FailInvalNum, exec_player_index);

   return succeded;
}



BOOL __fastcall RPG::Environment_Day(wchar_t *cmd_args, short exec_player_index)
{
   if (!rpgb6_2_running)
   {
      HaloSay(L"Failed: rpg_beta6_2 only command", exec_player_index);
      return FALSE;
   }

   short *setting = (short*)((*HS_Globals_ptr)->FirstItem
      + HCE_Lib::rpg_beta6_2_hs_global::setting_offset);

   if (*setting != 0)
   {
      *setting = 0;
      HaloSay(L"*the sun has come up*", -1);
   }

   return TRUE;
}

BOOL __fastcall RPG::Environment_Rain(wchar_t *cmd_args, short exec_player_index)
{
   if (!rpgb6_2_running)
   {
      HaloSay(L"Failed: rpg_beta6_2 only command", exec_player_index);
      return FALSE;
   }

   short *setting = (short*)((*HS_Globals_ptr)->FirstItem
      + HCE_Lib::rpg_beta6_2_hs_global::setting_offset);

   if (*setting != 1)
   {
      *setting = 1;
      HaloSay(L"*a dense fog covers the entire area as it starts to rain*", -1);
   }

   return TRUE;
}

BOOL __fastcall RPG::Environment_Night(wchar_t *cmd_args, short exec_player_index)
{
   if (!rpgb6_2_running)
   {
      HaloSay(L"Failed: rpg_beta6_2 only command", exec_player_index);
      return FALSE;
   }

   short *setting = (short*)((*HS_Globals_ptr)->FirstItem
      + HCE_Lib::rpg_beta6_2_hs_global::setting_offset);

   if (*setting != 2)
   {
      *setting = 2;
      HaloSay(L"*the sun has gone down*", -1);
   }
   return TRUE;
}

BOOL __fastcall RPG::AirBase_Alarm(wchar_t *cmd_args, short exec_player_index)
{
   if (!rpgb6_2_running)
   {
      HaloSay(L"Failed: rpg_beta6_2 only command", exec_player_index);
      return FALSE;
   }

   BOOL succeded = FALSE;
   wchar_t *GenericMsg;

   bool *alarmed = (bool*)((*HS_Globals_ptr)->FirstItem
      + HCE_Lib::rpg_beta6_2_hs_global::alarmed_offset);

   bool *alarm_control_2 = (bool*)((*Device_Groups_ptr)->FirstItem
      + HCE_Lib::rpg_beta6_2_device_groups::alarm_control_2_offset);

   int number = 0;
   if (CMDsLib::ParseStrBool(cmd_args, &number))
   {
      if (number == 1)
      {
         if (!*alarmed) *alarm_control_2 = true;

         GenericMsg = L"The Levis Station's alarm has been triggered.";
      }
      else if (number == 0)
      {
         if (*alarmed) *alarm_control_2 = true;

         GenericMsg = L"The Levis Station's alarm has been switched off.";
      }
      succeded = TRUE;
   }
   else
      GenericMsg = FailInvalNum;

   HaloSay(GenericMsg, -1);

   return succeded;
}

BOOL __fastcall RPG::AirBase_LockDown(wchar_t *cmd_args, short exec_player_index)
{
   if (!rpgb6_2_running)
   {
      HaloSay(L"Failed: rpg_beta6_2 only command", exec_player_index);
      return FALSE;
   }

   BOOL succeded = FALSE;
   wchar_t *GenericMsg;

   bool *locked = (bool*)((*HS_Globals_ptr)->FirstItem
      + HCE_Lib::rpg_beta6_2_hs_global::locked_offset);

   bool *lock_control = (bool*)((*Device_Groups_ptr)->FirstItem
      + HCE_Lib::rpg_beta6_2_device_groups::lock_control_offset);

   if (!*locked)
   {
      *lock_control = true;
      succeded = TRUE;
      GenericMsg = L"Levis Station's lockdown procedures have been initiated.";
   }
   else
      GenericMsg = L"Failed: Levis Station has already been locked down.";

   HaloSay(GenericMsg, -1);

   return succeded;
}

BOOL __fastcall RPG::Fire_Halo(wchar_t *cmd_args, short exec_player_index)
{
   if (!rpgb6_2_running)
   {
      HaloSay(L"Failed: rpg_beta6_2 only command", exec_player_index);
      return FALSE;
   }

   BOOL succeded = FALSE;
   wchar_t *GenericMsg;

   bool *nuked = (bool*)((*HS_Globals_ptr)->FirstItem
      + HCE_Lib::rpg_beta6_2_hs_global::nuked_offset);

   bool *boom_control = (bool*)((*Device_Groups_ptr)->FirstItem
      + HCE_Lib::rpg_beta6_2_device_groups::boom_control_offset);

   if (!*nuked)
   {
      *boom_control = true;
      succeded = TRUE;
      GenericMsg = L"Halo will be fired when someone is in close proximity to the control room.";
   }
   else
      GenericMsg = L"Failed: Halo is not ready to fire.";

   HaloSay(GenericMsg, -1);

   return succeded;
}

BOOL __fastcall RPG::LockDown_Timer(wchar_t *cmd_args, short exec_player_index)
{
   if (!rpgb6_2_running)
   {
      HaloSay(L"Failed: rpg_beta6_2 only command", exec_player_index);
      return FALSE;
   }

   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   short *lock_timer = (short*)((*HS_Globals_ptr)->FirstItem
      + HCE_Lib::rpg_beta6_2_hs_global::lock_timer_offset);

   int number;

   //displayer info
   if (!*cmd_args)
   {
      succeded = TRUE;
      GenericMsg = L"lock_timer var is set at %i seconds.";
   }
   else if (CMDsLib::ParseStrInt(cmd_args, &number))
   {
      *lock_timer = (short)(number * 30);
      succeded = TRUE;
      GenericMsg = L"lock_timer var has been set to %i seconds.";
   }
   else
      GenericMsg = FailInvalNum;

   HaloSay(GenericMsg, exec_player_index, *lock_timer / 30);

   return succeded;
}

BOOL __fastcall RPG::Halo_Timer(wchar_t *cmd_args, short exec_player_index)
{
   if (!rpgb6_2_running)
   {
      HaloSay(L"Failed: rpg_beta6_2 only command", exec_player_index);
      return FALSE;
   }

   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   short *boom_timer = (short*)((*HS_Globals_ptr)->FirstItem
      + HCE_Lib::rpg_beta6_2_hs_global::boom_timer_offset);

   int number;

   //displayer info
   if (!*cmd_args)
   {
      succeded = TRUE;
      GenericMsg = L"boom_timer var is set at %i seconds.";
   }
   else if (CMDsLib::ParseStrInt(cmd_args, &number))
   {
      *boom_timer = (short)(number * 30);
      succeded = TRUE;
      GenericMsg = L"boom_timer var has been set to %i seconds.";
   }
   else
      GenericMsg = FailInvalNum;

   HaloSay(GenericMsg, exec_player_index, *boom_timer / 30);

   return succeded;
}

inline DWORD GetObj(short obj_index)
{
   DWORD obj_address = NULL;
   if(obj_index != -1)//valid index?
      if (objects[obj_index].ObjectID)//valid ID?
         obj_address = objects[obj_index].Object_ptr;

   return obj_address;
}

inline HaloCE_lib::SPARTAN* GetPlayerObj(short player_index)
{
   return (HaloCE_lib::SPARTAN*)GetObj(players[player_index].PlayerObjTag.Index);
}

inline HaloCE_lib::VEHICLE_OBJECT* GetPlayerVehObj(HaloCE_lib::SPARTAN* player_obj)
{
   HaloCE_lib::VEHICLE_OBJECT *veh_obj_address = NULL;
   if (player_obj->VehicleTag.Index != -1)
      veh_obj_address = (HaloCE_lib::VEHICLE_OBJECT*)GetObj(player_obj->VehicleTag.Index);

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

//HaloCE_lib::OBJECT_TAG CreateObject(HaloCE_lib::OBJECT_TAG ObjTypeTag, float coordinates[3])
void __declspec(naked) CreateObject()
{
   //HaloCE_lib::OBJECT_TAG NewObjTag;
   __asm
   {
      PUSH 0x3F800000
      PUSH 0x3F800000
      PUSH 0x3F800000

      PUSH 0x3F800000
      PUSH 0x3F800000
      PUSH 0x3F800000

      PUSH 0x3F800000
      PUSH 0x3F800000
      PUSH 0x3F800000

      PUSH 0x3F800000
      PUSH 0x3F800000
      PUSH 0x3F800000
      //spin vector
      PUSH 0//0x3F800000
      PUSH 0
      PUSH 0
      //m_Scale
      PUSH 0x3F800000//0xBF800000
      PUSH 0x80000000
      PUSH 0x80000000
      //m_LowerRot
      PUSH 0
      PUSH 0//0xBEEDC4BB
      PUSH 0x3F800000//0x3F62B8A6
      //m_Velocity?
      PUSH 0
      PUSH 0
      PUSH 0
      PUSH 0
      //coordinates
      PUSH DWORD PTR [ECX+8]//0x42DD6B85//0x42DF3BE8
      PUSH DWORD PTR [ECX+4]//0x441BDE9D//0x441D0C44
      PUSH DWORD PTR [ECX]//0xC3933039//0xC38D246E

      PUSH 0x0000FFFF
      PUSH 0
      PUSH 0xFFFFFFFF
      PUSH 0xFFFFFFFF
      PUSH 0
      PUSH EDX ;//ObjTypeTag

      MOV EDX,ESP
      PUSH ECX;//saving ECX too

      PUSH 0
      PUSH EDX
      CALL DWORD PTR [CreateObj_func_address]
      ADD ESP,8

      POP ECX
      POP EDX ;//restore EDX
      ADD ESP,84h
      RETN
   }
   //return NewObjTag;
}

BOOL __fastcall Player::Speed(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   short Player_Indexes[16];
   int pi_found = 0;

   int arg_len;
   if ((arg_len = ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)))
   {
      cmd_args += arg_len;
      float fnumber = 0;

      //displayer info
      if (!*cmd_args)
      {
         for (int i = 0; i < pi_found; i++)
         {
            HaloCE_lib::STATIC_PLAYER *pSP = &players[Player_Indexes[i]];

            HaloSay(
               L"%s's speed modifier is %.2f",
               exec_player_index,
               pSP->PlayerName0,
               pSP->SpeedModifier);
         }
         succeded = TRUE;
      }
      else if (CMDsLib::ParseStrFloat(++cmd_args, &fnumber))
      {
         for (int i = 0; i < pi_found; i++)
         {
            HaloCE_lib::STATIC_PLAYER *pSP = &players[Player_Indexes[i]];
            wchar_t *SpecificMsg = NULL;

            if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
            {
               HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]),
                  *vehicle_object;
               if (player_object &&
                  (vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object)))
               {
                  //vehicle_object;
                  __asm
                  {
                     MOV EDX,vehicle_object
                     MOV EDX,DWORD PTR [EDX]
                     AND EDX,0xFFFF
                     SHL EDX,5

                     MOV ECX,0x00816DE4;
                     MOV ECX,DWORD PTR [ECX]

                     MOV EAX,DWORD PTR [EDX+ECX+14h]

                     MOV EDX,fnumber
                     MOV DWORD PTR [EAX+2F8h],EDX
                  }
                  SpecificMsg = L"%s's vehicle's speed modifier has been set to %.2f";
               }
               else
               {
                  pSP->SpeedModifier = fnumber;
                  SpecificMsg = L"%s's speed modifier has been set to %.2f";
               }

               succeded = TRUE;
            }
            else
               SpecificMsg = FailLowAdminLvl;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  pSP->PlayerName0,
                  fnumber);
            }
         }
      }
      else GenericMsg = FailInvalNum;
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg)
      HaloSay(GenericMsg, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::ActiveCamo(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   short Player_Indexes[16];
   int pi_found = 0;

   int arg_len;
   if ((arg_len = ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)))
   {
      int seconds;
      if (CMDsLib::ParseStrInt((++cmd_args += arg_len), &seconds))
      {
         short durration = seconds * 30; //halo time units = 30 * seconds

         for (int i = 0; i < pi_found; i++)
         {
            wchar_t *SpecificMsg = NULL;

            if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
            {
               HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]);
               if (player_object)
               {
                  //player_object->IsInvisible = buffer_num;
                  DWORD playertag = players[Player_Indexes[i]].PlayerID;
                  playertag <<= 16;
                  playertag |= Player_Indexes[i];
                  __asm
                  {
                     MOVSX EDX,durration
                     PUSH EDX
                     PUSH 0
                     MOV EBX,playertag
                     CALL DWORD PTR [ActiveCamo_func_address]
                     ADD ESP,8
                     MOV succeded,TRUE
                  }
                  SpecificMsg = L"%s has been given active camouflage for %i seconds";
               }
               else
                  SpecificMsg = FailPlyrNtSpwn;
            }
            else
               SpecificMsg = FailLowAdminLvl;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0,
                  durration / 30);
            }
         }
      }
      else
         GenericMsg = FailInvalNum;
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg) HaloSay(GenericMsg, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Suspend(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   BOOL suspend;

   short Player_Indexes[16];
   int pi_found = 0;

   int arg_len;
   if ((arg_len = ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)))
   {
      if (CMDsLib::ParseStrBool((++cmd_args += arg_len), &suspend))
      {
         for (int i = 0; i < pi_found; i++)
         {
            wchar_t *SpecificMsg = NULL;

            if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
            {
               HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]);
               if (player_object)
               {
                  //HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
                  //if (vehicle_object) player_object = vehicle_object;

                  player_object->IsSuspended = (BYTE)suspend;
                  succeded = TRUE;

                  if (suspend) SpecificMsg = L"%s is now suspended.";
                  else SpecificMsg = L"%s is now unsuspended.";
               }
               else
                  SpecificMsg = FailPlyrNtSpwn;
            }
            else
               SpecificMsg = FailLowAdminLvl;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0);
            }
         }
      }
      else
         GenericMsg = FailInvalNum;
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg)
      HaloSay(GenericMsg, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Teleport(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL, *SpecificMsg = NULL, *teleport_to;

   short Player_Indexes[16];
   int pi_found = 0;

   float coordinates[3];
   int teleport_type = 0;

   int arg_len;
   if (str_cmpW(cmd_args, L"remove ", 7))
   {
      cmd_args += 7;
      if (*cmd_args)
      {
         unsigned int map_i = 0, tele_loc_i = 0;
         if (FindMapIndex(&maps_tele_sites, Current_Map_Str, map_i) &&
            FindTeleLocNameIndex(&maps_tele_sites[map_i].teleport_locations, cmd_args, tele_loc_i))
         {
            std::vector<TELEPORT_LOCATION> *tl = &maps_tele_sites[map_i].teleport_locations;
            tl->erase(tl->begin() + tele_loc_i);
            //delete map if their are no more locs
            if (!tl->size())
               maps_tele_sites.erase(maps_tele_sites.begin() + map_i);

            WriteLocationsToFile(LocationsFilePath, &maps_tele_sites);
            SpecificMsg = L"\"%s\" has been removed.";
         }
         else
            SpecificMsg = FailBadTeleLoc;
      }
      else
         SpecificMsg = FailMissingLoc;

      HaloSay(
         SpecificMsg,
         exec_player_index,
         cmd_args);

   }
   else if ((arg_len = ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)))
   {
      cmd_args += arg_len;
      int arg_count = CMDsLib::GetCMDArgCount(cmd_args);

      if (arg_count == 3)
      {
         //use x y z coordinates
         int arg_len;
         if ((arg_len = CMDsLib::ParseStrFloat(++cmd_args, &coordinates[0])) &&
            (arg_len = CMDsLib::ParseStrFloat((++cmd_args += arg_len), &coordinates[1])) &&
            CMDsLib::ParseStrFloat((++cmd_args += arg_len), &coordinates[2]))
         {
            teleport_type = 1;
         }
         else
            GenericMsg = FailInvalNum;
      }
      else if (arg_count == 1)
      {
         int pi2_to_find = 1; short player2_index;
         if (ParseCMDStrPlayers(++cmd_args, &player2_index, pi2_to_find))
         {
            HaloCE_lib::SPARTAN *player2_object = GetPlayerObj(player2_index);
            if (player2_object)
            {
               //if player is in a vehicle, use vehicle's coords_or_vectors
               HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player2_object);
               if (vehicle_object) player2_object = vehicle_object;

               for (int i = 0; i < 3; i++)
                  coordinates[i] = player2_object->m_World[i];

               teleport_to = players[player2_index].PlayerName0;
               teleport_type = 2;
            }
         }

         if (!teleport_type)
         {
            unsigned int map_i = 0, tele_loc_i = 0;
            if (FindMapIndex(&maps_tele_sites, Current_Map_Str, map_i) &&
               FindTeleLocNameIndex(&maps_tele_sites[map_i].teleport_locations, cmd_args, tele_loc_i))
            {
               TELEPORT_LOCATION *pTL = &maps_tele_sites[map_i].teleport_locations[tele_loc_i];
               for (int i = 0; i < 3; i++)
                  coordinates[i] =  pTL->coordinates[i];

               teleport_to = pTL->teleport_loc_name;
               teleport_type = 3;
            }
            else
               GenericMsg = FailBadTeleLoc;
         }
      }
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg) HaloSay(GenericMsg, exec_player_index);

   if (teleport_type)
   {
      for (int i = 0; i < pi_found; i++)
      {
         if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
         {
            HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]);
            if (player_object)
            {
               //if player is in a vehicle, use vehicle's coords_or_vectors
               HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
               if (vehicle_object) player_object = vehicle_object;

               player_object->m_World[0] = coordinates[0];
               player_object->m_World[1] = coordinates[1];
               player_object->m_World[2] = coordinates[2] + 1 * i;
               succeded = TRUE;

               switch (teleport_type)
               {
                  case 1:
                     HaloSay(
                        L"%s has been teleported to %.2fx %.2fy %.2fz",
                        exec_player_index,
                        players[Player_Indexes[i]].PlayerName0,
                        coordinates[0],
                        coordinates[1],
                        coordinates[2]);
                     break;

                  case 2:
                     SpecificMsg = L"%s has been teleported to %s";
                     player_object->m_World[2] += 1;//does float support ++?
                     break;

                  case 3:
                     SpecificMsg = L"%s has been teleported to \"%s\"";
                     break;
               }
            }
            else
               SpecificMsg = FailPlyrNtSpwn;
         }
         else
            SpecificMsg = FailLowAdminLvl;

         if (SpecificMsg)
         {
            HaloSay(
               SpecificMsg,
               exec_player_index,
               players[Player_Indexes[i]].PlayerName0,
               teleport_to);
         }
      }
   }
   return succeded;
}

BOOL __fastcall Player::Jump_Teleport(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL, *SpecificMsg = NULL;

   short Player_Indexes[16];
   int pi_found = 0;

   float coordinates[3];

   int arg_len;
   if ((arg_len = ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)))
   {
      cmd_args += arg_len;

      if (!*cmd_args)
      {
         for (int i = 0; i < pi_found; i++)
         {
            SpecificMsg = NULL;

            HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]);
            if (player_object)
            {
               //if player is in a vehicle, use vehicle's coords_or_vectors
               HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
               if (vehicle_object) player_object = vehicle_object;

               for (int i = 0; i < 3; i++)
                  coordinates[i] = player_object->m_World[i];

               succeded = TRUE;
               SpecificMsg = L"%s's current coordinates is %.2fx %.2fy %.2fz";
            }
            else
               SpecificMsg = FailPlyrNtSpwn;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0,
                  coordinates[0],
                  coordinates[1],
                  coordinates[2]);
            }
         }
      }
      else if ((arg_len = CMDsLib::ParseStrFloat(++cmd_args, &coordinates[0])) &&
         (arg_len = CMDsLib::ParseStrFloat((++cmd_args += arg_len), &coordinates[1])) &&
         CMDsLib::ParseStrFloat((++cmd_args += arg_len), &coordinates[2]))
      {
         for (int i = 0; i < pi_found; i++)
         {
            SpecificMsg = NULL;

            if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
            {
               HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]);
               if (player_object)
               {
                  //if player is in a vehicle, use vehicle's coords_or_vectors
                  HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
                  if (vehicle_object) player_object = vehicle_object;

                  for (int i = 0; i < 3; i++)
                     player_object->m_World[i] += coordinates[i];

                  succeded = TRUE;
                  SpecificMsg = L"%s's coordinates has been adjusted by %.2fx %.2fy %.2fz";
               }
               else
                  SpecificMsg = FailPlyrNtSpwn;
            }
            else
               SpecificMsg = FailLowAdminLvl;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0,
                  coordinates[0],
                  coordinates[1],
                  coordinates[2]);
            }
         }
      }
      else
         GenericMsg = FailInvalNum;
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg) HaloSay(GenericMsg, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Velocity(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL, *SpecificMsg = NULL;

   short Player_Indexes[16];
   int pi_found = 0;

   float vectors[3];

   int arg_len;
   if ((arg_len = ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)))
   {
      cmd_args += arg_len;

      if (!*cmd_args)
      {
         for (int i = 0; i < pi_found; i++)
         {
            SpecificMsg = NULL;

            HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]);
            if (player_object)
            {
               //if player is in a vehicle, use vehicle's coords_or_vectors
               HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
               if (vehicle_object) player_object = vehicle_object;

               for (int i = 0; i < 3; i++)
                  vectors[i] = player_object->m_Velocity[i];

               succeded = TRUE;
               SpecificMsg = L"%s's current vector is %.2fx %.2fy %.2fz";
            }
            else
               SpecificMsg = FailPlyrNtSpwn;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0,
                  vectors[0],
                  vectors[1],
                  vectors[2]);
            }
         }
      }
      else if ((arg_len = CMDsLib::ParseStrFloat(++cmd_args, &vectors[0])) &&
         (arg_len = CMDsLib::ParseStrFloat((++cmd_args += arg_len), &vectors[1])) &&
         CMDsLib::ParseStrFloat((++cmd_args += arg_len), &vectors[2]))
      {
         for (int i = 0; i < pi_found; i++)
         {
            SpecificMsg = NULL;

            if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
            {
               HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]);
               if (player_object)
               {
                  //if player is in a vehicle, use vehicle's coords_or_vectors
                  HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
                  if (vehicle_object) player_object = vehicle_object;

                  for (int i = 0; i < 3; i++)
                     player_object->m_Velocity[i] = vectors[i];

                  succeded = TRUE;
                  SpecificMsg = L"%s's vector has been changed to %.2fx %.2fy %.2fz";
               }
               else
                  SpecificMsg = FailPlyrNtSpwn;
            }
            else
               SpecificMsg = FailLowAdminLvl;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0,
                  vectors[0],
                  vectors[1],
                  vectors[2]);
            }
         }
      }
      else
         GenericMsg = FailInvalNum;
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg)
      HaloSay(GenericMsg, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Ammo(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL, *SpecificMsg = NULL;

   short Player_Indexes[16];
   int pi_found = 0;

   int arg_len;
   if ((arg_len = ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)))
   {
      cmd_args += arg_len;
      int number;

      //displayer info
      if (!*cmd_args)
      {
         for (int i = 0; i < pi_found; i++)
         {
            SpecificMsg = NULL;

            HaloCE_lib::SPARTAN* player_object = GetPlayerObj(Player_Indexes[i]);
            if(player_object)
            {
               HaloCE_lib::WEAPON_OBJECT *weapon_object = (HaloCE_lib::WEAPON_OBJECT*)GetObj(player_object->WeaponTag.Index);
               if (weapon_object)
               {
                  number = weapon_object->rounds_total;
                  succeded = TRUE;
                  SpecificMsg = L"%s's weapon's ammo is at %i";
               }
               else
                  SpecificMsg = FailPlyrNoWep;
            }
            else
               SpecificMsg = FailPlyrNtSpwn;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0,
                  number);
            }
         }
      }
      else if (CMDsLib::ParseStrInt(++cmd_args, &number))
      {
         for (int i = 0; i < pi_found; i++)
         {
            SpecificMsg = NULL;

            if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
            {
               HaloCE_lib::SPARTAN* player_object = GetPlayerObj(Player_Indexes[i]);
               if(player_object)
               {
                  HaloCE_lib::WEAPON_OBJECT *weapon_object = (HaloCE_lib::WEAPON_OBJECT*)GetObj(player_object->WeaponTag.Index);
                  if (weapon_object)
                  {
                     weapon_object->rounds_total = (short)number;
                     succeded = TRUE;
                     SpecificMsg = L"%s's weapon's ammo has been changed to %i";
                  }
                  else
                     SpecificMsg = FailPlyrNoWep;
               }
               else
                  SpecificMsg = FailPlyrNtSpwn;
            }
            else
               SpecificMsg = FailLowAdminLvl;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0,
                  number);
            }
         }
      }
      else
         GenericMsg = FailInvalNum;
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg)
      HaloSay(GenericMsg, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Battery(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL, *SpecificMsg = NULL;

   short Player_Indexes[16];
   int pi_found = 0;

   int arg_len;
   if ((arg_len = ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)))
   {
      cmd_args += arg_len;
      float fnumber;

      //displayer info
      if (!*cmd_args)
      {
         for (int i = 0; i < pi_found; i++)
         {
            SpecificMsg = NULL;

            HaloCE_lib::SPARTAN* player_object = GetPlayerObj(Player_Indexes[i]);
            if(player_object)
            {
               HaloCE_lib::WEAPON_OBJECT *weapon_object = (HaloCE_lib::WEAPON_OBJECT*)GetObj(player_object->WeaponTag.Index);
               if (weapon_object)
               {
                  fnumber = 100.0f - (weapon_object->battery_used * 100.0f);
                  succeded = TRUE;
                  SpecificMsg = L"%s's weapon's battery is at %.2f%%";
               }
               else
                  SpecificMsg = FailPlyrNoWep;
            }
            else
               SpecificMsg = FailPlyrNtSpwn;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0,
                  fnumber);
            }
         }
      }
      else if (CMDsLib::ParseStrFloat(++cmd_args, &fnumber))
      {
         for (int i = 0; i < pi_found; i++)
         {
            SpecificMsg = NULL;

            if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
            {
               HaloCE_lib::SPARTAN* player_object = GetPlayerObj(Player_Indexes[i]);
               if(player_object)
               {
                  HaloCE_lib::WEAPON_OBJECT *weapon_object = (HaloCE_lib::WEAPON_OBJECT*)GetObj(player_object->WeaponTag.Index);
                  if (weapon_object)
                  {
                     weapon_object->battery_used = (100.0f - fnumber)/ 100.0f;
                     succeded = TRUE;
                     SpecificMsg = L"%s's weapon's battery has been changed to %.2f%%";
                  }
                  else
                     SpecificMsg = FailPlyrNoWep;
               }
               else
                  SpecificMsg = FailPlyrNtSpwn;
            }
            else
               SpecificMsg = FailLowAdminLvl;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0,
                  fnumber);
            }
         }
      }
      else
         GenericMsg = FailInvalNum;
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg)
      HaloSay(GenericMsg, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Health(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL, *SpecificMsg = NULL;

   short Player_Indexes[16];
   int pi_found = 0;

   int arg_len;
   if ((arg_len = ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)))
   {
      cmd_args += arg_len;
      float fnumber;

      //displayer info
      if (!*cmd_args)
      {
         for (int i = 0; i < pi_found; i++)
         {
            SpecificMsg = NULL;

            HaloCE_lib::SPARTAN* player_object = GetPlayerObj(Player_Indexes[i]);
            if(player_object)
            {
               fnumber = player_object->Health * 100.0f;
               succeded = TRUE;
               SpecificMsg = L"%s's health is at %.2f%%";
            }
            else
               SpecificMsg = FailPlyrNtSpwn;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0,
                  fnumber);
            }
         }
      }
      else if (CMDsLib::ParseStrFloat(++cmd_args, &fnumber))
      {
         for (int i = 0; i < pi_found; i++)
         {
            SpecificMsg = NULL;

            if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
            {
               HaloCE_lib::SPARTAN* player_object = GetPlayerObj(Player_Indexes[i]);
               if(player_object)
               {
                  player_object->Health = fnumber / 100.0f;
                  succeded = TRUE;
                  SpecificMsg = L"%s's health has been set to %.2f%%";
               }
               else
                  SpecificMsg = FailPlyrNtSpwn;
            }
            else
               SpecificMsg = FailLowAdminLvl;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0,
                  fnumber);
            }
         }
      }
      else
         GenericMsg = FailInvalNum;
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg)
      HaloSay(GenericMsg, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Shield(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL, *SpecificMsg = NULL;

   short Player_Indexes[16];
   int pi_found = 0;

   int arg_len;
   if ((arg_len = ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)))
   {
      cmd_args += arg_len;
      float fnumber;

      //displayer info
      if (!*cmd_args)
      {
         for (int i = 0; i < pi_found; i++)
         {
            SpecificMsg = NULL;

            HaloCE_lib::SPARTAN* player_object = GetPlayerObj(Player_Indexes[i]);
            if(player_object)
            {
               fnumber = player_object->Shield_00 * 100.0f;
               succeded = TRUE;
               SpecificMsg = L"%s's shield is at %.2f%%";
            }
            else
               SpecificMsg = FailPlyrNtSpwn;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0,
                  fnumber);
            }
         }
      }
      else if (CMDsLib::ParseStrFloat(++cmd_args, &fnumber))
      {
         for (int i = 0; i < pi_found; i++)
         {
            SpecificMsg = NULL;

            if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
            {
               HaloCE_lib::SPARTAN* player_object = GetPlayerObj(Player_Indexes[i]);
               if(player_object)
               {
                  player_object->Shield_00 = fnumber / 100.0f;
                  succeded = TRUE;
                  SpecificMsg = L"%s's shield has been set to %.2f%%";
               }
               else
                  SpecificMsg = FailPlyrNtSpwn;
            }
            else
               SpecificMsg = FailLowAdminLvl;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0,
                  fnumber);
            }
         }
      }
      else
         GenericMsg = FailInvalNum;
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg)
      HaloSay(GenericMsg, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::AFK(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   short Player_Indexes[16];
   int pi_found = 0;

   if (ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found))
   {
      for (int i = 0; i < pi_found; i++)
      {
         wchar_t *SpecificMsg = NULL;

         if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
         {
            HaloCE_lib::OBJECT_TAG ObjTag = players[Player_Indexes[i]].PlayerObjTag;
            if (ObjTag.Tag != -1)
            {
               DWORD playertag = players[Player_Indexes[i]].PlayerID;
               playertag <<= 16;
               playertag |= Player_Indexes[i];
               __asm
               {
                  PUSH 0x7FFFFFF ;//respawn time after death
                  MOV EBX,playertag
                  CALL DWORD PTR [PlayerDeath_func_address]
                  ADD ESP,4
               }

               SpecificMsg = L"%s is now afk.";
            }
            else
            {
               players[Player_Indexes[i]].RespawnTimer = 30;//1 sec
               SpecificMsg = L"%s is no longer afk.";
            }

            succeded = TRUE;
         }
         else
            SpecificMsg = FailLowAdminLvl;

         if (SpecificMsg)
         {
            HaloSay(
               SpecificMsg,
               -1,
               players[Player_Indexes[i]].PlayerName0);
         }
      }
   }
   else
      HaloSay(FailPlyrNtFnd, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Team_Change(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   DWORD Team = 0;
   short Player_Indexes[16];
   int pi_found = 0;

   if (ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found))
   {
      for (int i = 0; i < pi_found; i++)
      {
         wchar_t *SpecificMsg = NULL, *team_str;

         if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
         {
            KillPlayer(Player_Indexes[i]);

            Team = players[Player_Indexes[i]].Team;
            if (Team)
            {
               Team = 0;
               team_str = L"Red";
            }
            else
            {
               Team = 1;
               team_str = L"Blue";
            }

            players[Player_Indexes[i]].Team = Team;
            succeded = TRUE;

            SpecificMsg = L"%s has been switched to %s team.";
         }
         else
            SpecificMsg = FailLowAdminLvl;

         if (SpecificMsg)
         {
            HaloSay(
               SpecificMsg,
               -1,
               players[Player_Indexes[i]].PlayerName0,
               team_str);
         }
      }
   }
   else
      HaloSay(FailPlyrNtFnd, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Kick(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   short Player_Indexes[16];
   int pi_found = 0;

   if (ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found))
   {
      for (int i = 0; i < pi_found; i++)
      {
         wchar_t *SpecificMsg = NULL;

         if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
         {
            if (Player_Indexes[i])
            {
               char _pi_char[3] = {'\0','\0','\0'};
               if (Player_Indexes[i] > 8)
               {
                  _pi_char[0] = '1';
                  _pi_char[1] = (Player_Indexes[i] - 10) + '1';
               }
               else
               {
                  _pi_char[0] = Player_Indexes[i] + '1';
                  _pi_char[1] = '\0';
               }

               __asm
               {
                  LEA EAX,[_pi_char]
                  CALL DWORD PTR [sv_kick_func_address]
                  MOV succeded,TRUE
               }

               SpecificMsg = L"%s has been kicked.";
            }
            else
               SpecificMsg = L"Failed: Cannot kick host (%s)!";
         }
         else
            SpecificMsg = FailLowAdminLvl;

         if (SpecificMsg)
         {
            HaloSay(
               SpecificMsg,
               -1,
               players[Player_Indexes[i]].PlayerName0);
         }
      }
   }
   else
      HaloSay(FailPlyrNtFnd, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Ban(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   short Player_Indexes[16];
   int pi_found = 0;

   if (ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found))
   {
      for (int i = 0; i < pi_found; i++)
      {
         wchar_t *SpecificMsg = NULL;

         if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
         {
            if (Player_Indexes[i])
            {
               char *ban_params[2];
               char player_index_char[3] = {'\0','\0','\0'};

               if (Player_Indexes[i] > 8)
               {
                  player_index_char[0] = '1';
                  player_index_char[1] = (Player_Indexes[i] - 10) + '1';
               }
               else
                  player_index_char[0] = Player_Indexes[i] + '1';

               char dhms_chars = '\0';

               ban_params[0] = player_index_char;
               ban_params[1] = &dhms_chars;

               __asm
               {
                  LEA ECX,[ban_params]
                  MOV EAX,1
                  CALL DWORD PTR [sv_ban_func_address]
                  MOV succeded,TRUE
               }

               SpecificMsg = L"%s has been banned.";
            }
            else
               SpecificMsg = L"Failed: Cannot ban host (%s)!";
         }
         else
            SpecificMsg = FailLowAdminLvl;

         if (SpecificMsg)
         {
            HaloSay(
               SpecificMsg,
               -1,
               players[Player_Indexes[i]].PlayerName0);
         }
      }
   }
   else
      HaloSay(FailPlyrNtFnd, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Kill(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   short Player_Indexes[16];
   int pi_found = 0;

   if (ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found))
   {
      for (int i = 0; i < pi_found; i++)
      {
         wchar_t *SpecificMsg = NULL;

         if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
         {
            if (KillPlayer(Player_Indexes[i]))
            {
               succeded = TRUE;
               //get random kill msg
               SYSTEMTIME systime;
               GetLocalTime(&systime);
               int rand = (systime.wMilliseconds >> 2) + Player_Indexes[i];
               rand &= 7;//only 0 to 7 indexs' are valid
               SpecificMsg = SuccededKillMsgs[rand];
            }
            else
               SpecificMsg = FailPlyrNtSpwn;
         }
         else
            SpecificMsg = FailLowAdminLvl;

         if (SpecificMsg)
         {
            HaloSay(
               SpecificMsg,
               -1,
               players[Player_Indexes[i]].PlayerName0);
         }
      }
   }
   else
      HaloSay(FailPlyrNtFnd, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Eject(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   short Player_Indexes[16];
   int pi_found = 0;

   if (ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found))
   {
      for (int i = 0; i < pi_found; i++)
      {
         wchar_t *SpecificMsg = NULL;

         if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
         {
            HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]);
            if (player_object)
            {
               HaloCE_lib::VEHICLE_OBJECT *vehicle_object = GetPlayerVehObj(player_object);
               if (vehicle_object)
               {
                  HaloCE_lib::OBJECT_TAG PlayerObjTag = players[Player_Indexes[i]].PlayerObjTag;
                  __asm
                  {
                     MOV EAX,PlayerObjTag
                     CALL UnitExitVehicle_func_address
                  }
                  succeded = TRUE;
                  SpecificMsg = L"%s has been ejected from a vehicle.";
               }
               else
                  SpecificMsg = FailPlyrNtInVeh;
            }
            else
               SpecificMsg = FailPlyrNtSpwn;
         }
         else
            SpecificMsg = FailLowAdminLvl;

         if (SpecificMsg)
         {
            HaloSay(
               SpecificMsg,
               exec_player_index,
               players[Player_Indexes[i]].PlayerName0);
         }
      }
   }
   else
      HaloSay(FailPlyrNtFnd, exec_player_index);

   return succeded;
}
BOOL __fastcall Player::Flip_Vehicle(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   short Player_Indexes[16];
   int pi_found = 0;

   if (ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found))
   {
      for (int i = 0; i < pi_found; i++)
      {
         wchar_t *SpecificMsg = NULL;

         if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
         {
            HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]);
            if (player_object)
            {
               //if player is in a vehicle, use vehicle's coords_or_vectors
               HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
               if (vehicle_object)
               {
                  player_object = vehicle_object;
                  //player_object->m_LowerRot[0] = 0;
                  //player_object->m_LowerRot[1] = 0;
                  //player_object->m_LowerRot[2] = 1;
                  //wierd but its really "m_LowerRot"
                  player_object->m_Scale[0] = 0;
                  player_object->m_Scale[1] = 0;

                  float z_axis = player_object->m_Scale[2];
                  //flip 180
                  if (z_axis < 0) z_axis = 1;
                  else z_axis = -1;

                  player_object->m_Scale[2] = z_axis;
                  succeded = TRUE;
                  SpecificMsg = L"%s's vehicle has been fliped 180°.";
               }
               else
                  SpecificMsg = FailPlyrNtInVeh;
            }
            else
               SpecificMsg = FailPlyrNtSpwn;
         }
         else
            SpecificMsg = FailLowAdminLvl;

         if (SpecificMsg)
         {
            HaloSay(
               SpecificMsg,
               exec_player_index,
               players[Player_Indexes[i]].PlayerName0);
         }
      }
   }
   else
      HaloSay(FailPlyrNtFnd, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Admin(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   short Player_Indexes[16];
   int buffer_num = 0, pi_found = 0;

   int arg_len;
   if ((arg_len = ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)))
   {
      cmd_args += arg_len;

      if (!*cmd_args)
      {
         succeded = TRUE;
         for (int i = 0; i < pi_found; i++)
         {
            HaloSay(
               L"%s's current admin level is %u.",
               -1,
               players[Player_Indexes[i]].PlayerName0,
               TempAdmin[Player_Indexes[i]]);
         }
      }
      else if (CMDsLib::ParseStrInt(++cmd_args, &buffer_num))
      {
         DWORD admin_level = (DWORD)buffer_num;
         if (TempAdmin[exec_player_index] >= admin_level)
         {
            for (int i = 0; i < pi_found; i++)
            {
               wchar_t *SpecificMsg = NULL;

               if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
               {
                  if (Player_Indexes[i])//same as != 0
                  {
                     TempAdmin[Player_Indexes[i]] = admin_level;
                     succeded = TRUE;
                     SpecificMsg = L"%s has been changed to admin level %u.";
                  }
                  else
                     SpecificMsg = L"Failed: Cannot change host (%s)'s admin level";
               }
               else
                  SpecificMsg = FailLowAdminLvl;

               if (SpecificMsg)
               {
                  HaloSay(
                     SpecificMsg,
                     -1,
                     players[Player_Indexes[i]].PlayerName0,
                     admin_level);
               }
            }
         }
         else
            GenericMsg = L"Failed: You can't promote an admin higher yous.";
      }
      else
         GenericMsg = FailInvalNum;
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg)
      HaloSay(GenericMsg, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Set_Teleport_Loc(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   TELEPORT_LOCATION tele_site;
   wchar_t *GenericMsg = NULL;

   short Player_Index;
   int pi_found = 1;
   int arg_len;
   if ((arg_len = ParseCMDStrPlayers(cmd_args, &Player_Index, pi_found)))
   {
      cmd_args += arg_len;
      if (*++cmd_args)
      {
         //count the number of spaces
         bool space_found = false;
         wchar_t *cmd_w_ptr = cmd_args;
         while (*cmd_w_ptr++)
            if (*cmd_w_ptr == ' ') space_found = true;

         if (!space_found)
         {
            int i = 0; wchar_t wchar;
            do
            {
               wchar = cmd_args[i];
               tele_site.teleport_loc_name[i] = wchar;
            }
            while (wchar && i++ < TELE_LOC_NAME_SIZE);

            //add null at end if too long
            tele_site.teleport_loc_name[TELE_LOC_NAME_SIZE - 1] = '\0';


            wchar_t *SpecificMsg = NULL;

            HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Index);
            if (player_object)
            {
               //if player is in a vehicle, use vehicle's coords_or_vectors
               HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
               if (vehicle_object) player_object = vehicle_object;

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
               succeded = TRUE;
               //unfortunatly the location name comes first, but in the error messages, player comes first
               HaloSay(
                  L"New location \"%s\", has been set for %s's current position.",
                  -1,
                  cmd_args,
                  players[Player_Index].PlayerName0);
            }
            else
               SpecificMsg = FailPlyrNtSpwn;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Index].PlayerName0);
            }
         }
         else
            GenericMsg = FailNoSpaces;
      }
      else
         GenericMsg = FailMissingLoc;
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg)
      HaloSay(GenericMsg, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Spawn_Biped(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;
   short Player_Indexes[16];
   int pi_found = 0;

   if ((cmd_args += ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)))
   {
      int how_many_to_spawn;
      if (CMDsLib::ParseStrInt(++cmd_args, &how_many_to_spawn))
      {
         for (int i = 0; i < pi_found; i++)
         {
            wchar_t *SpecificMsg = NULL;

            if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
            {
               HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]);
               if (player_object)
               {
                  HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
                  if (vehicle_object) player_object = vehicle_object;

                  float coords[3];
                  coords[0] = player_object->m_World[0] + 1;
                  coords[1] = player_object->m_World[1] + 1;
                  coords[2] = player_object->m_World[2] + 0.5f;

                  __asm
                  {
                     MOV ECX,DWORD PTR [ObjTagList_ptr_address]
                     MOV ECX, DWORD PTR [ECX]

                     MOV EDX,DWORD PTR [ECX+174h]
                     MOV EDX,DWORD PTR [EDX+0Ch]

                     CMP EDX,0xFF
                     JE SHORT biped_inval
                     ;//6E2280->addr+168h]->addr+1Ch]-> player obj type tag
                     MOV ECX,DWORD PTR [ECX+168h]
                     ;//__fastcall params
                     MOV EDX,DWORD PTR [ECX+1Ch]
                     LEA ECX,[coords]

                     XOR ESI,ESI
                     continue_biped_create_loop:
                     CMP ESI,how_many_to_spawn
                     JGE SHORT bipeds_spawned//exit_biped_create_loop
                     CALL CreateObject
                     INC ESI
                     JMP SHORT continue_biped_create_loop
                     //exit_biped_create_loop:

                     biped_inval:
                     MOV ECX,DWORD PTR [FailBadSpawn]
                     MOV SpecificMsg,ECX
                     JMP SHORT biped_failed

                     bipeds_spawned:
                     MOV succeded,TRUE
                  }
                  SpecificMsg = L"Biped(s) spawned next to %s";
                  __asm biped_failed:
               }
               else
                  SpecificMsg = FailPlyrNtSpwn;
            }
            else
               SpecificMsg = FailLowAdminLvl;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0);
            }
         }
      }
      else
         GenericMsg = FailInvalNum;
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg)
      HaloSay(GenericMsg, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Spawn_Hog(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   short Player_Indexes[16];
   int pi_found = 0;

   if (ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found))
   {
      DWORD dwOldProtect;
      if (VirtualProtect(
         (LPVOID)Player0_index,
         sizeof(WORD),
         PAGE_EXECUTE_READWRITE,
         &dwOldProtect))
      {
         for (int i = 0; i < pi_found; i++)
         {
            wchar_t *SpecificMsg = NULL;

            if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
            {
               *Player0_index = Player_Indexes[i];
               //doesnt spawn on some custom maps
               //__asm CALL SpawnHog_func_address
               __asm
               {
                  //DWORD objlist = *(DWORD*)ObjTagList_ptr_address;
                  MOV ECX,DWORD PTR [ObjTagList_ptr_address]
                  MOV ECX, DWORD PTR [ECX]
                  //DWORD num_of_objs = *(DWORD*)(objlist+0x164);
                  //if (num_of_objs)
                  CMP DWORD PTR [ECX+164h],0
                  JE SHORT hog_bad_list
                  //DWORD vehicle_list_header = *(DWORD*)(objlist+0x168);
                  MOV ECX,DWORD PTR [ECX+168h]
                  //DWORD num_of_veh = *(WORD*)(vehicle_list_header+0x20);
                  MOVZX EAX,WORD PTR [ECX+20h]
                  //DWORD vehicle_list = *(DWORD*)(vehicle_list_header+0x24);
                  MOV ECX,DWORD PTR [ECX+24h]
                  //if (num_of_veh)
                  TEST EAX,EAX
                  JE SHORT hog_bad_list
                  PUSH 1// num_of_veh
                  PUSH ECX//vehicle_list
                  CALL DWORD PTR [SpawnObjAtPlayer0_func_address]
                  ADD ESP,8
                  CMP EAX,-1
                  JNZ SHORT hog_val_player
                  MOV ECX,DWORD PTR [FailPlyrNtSpwn]
                  MOV SpecificMsg,ECX
                  JMP SHORT hog_failed

                  hog_val_player:
                  TEST AX,AX
                  JNZ SHORT hog_spawned
                  hog_bad_list:
                  MOV ECX,DWORD PTR [FailBadSpawn]
                  MOV SpecificMsg,ECX
                  JMP SHORT hog_failed

                  hog_spawned:
                  MOV succeded,TRUE
               }
                  SpecificMsg = L"Warthog spawned next to %s";
               __asm hog_failed:
            }
            else
               SpecificMsg = FailLowAdminLvl;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0);
            }
         }

         *Player0_index = 0;//change back to 0 when finished
         VirtualProtect(
            (LPVOID)Player0_index,
            sizeof(WORD),
            dwOldProtect,
            &dwOldProtect);
      }
   }
   else
      HaloSay(FailPlyrNtFnd, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Spawn_All_Vehicles(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   short Player_Indexes[16];
   int pi_found = 0;

   if (ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found))
   {
      DWORD dwOldProtect;
      if (VirtualProtect(
         (LPVOID)Player0_index,
         sizeof(WORD),
         PAGE_EXECUTE_READWRITE,
         &dwOldProtect))
      {
         for (int i = 0; i < pi_found; i++)
         {
            wchar_t *SpecificMsg = NULL;

            if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
            {
               //[40848BF0+i*10]= vehicle tag
               //[40848BE4+0C]->E3D40260 (1st one?)
               //[40848B44+24]->40848BE4
               //[40848198+168]->40848B44
               //[6E2280]->40848198
               //[ObjTagList_ptr_address]->6E2280
               *Player0_index = Player_Indexes[i];
               __asm
               {
                  //DWORD objlist = *(DWORD*)ObjTagList_ptr_address;
                  MOV ECX,DWORD PTR [ObjTagList_ptr_address]
                  MOV ECX, DWORD PTR [ECX]
                  //DWORD num_of_objs = *(DWORD*)(objlist+0x164);
                  //if (num_of_objs)
                  CMP DWORD PTR [ECX+164h],0
                  JE SHORT vehs_bad_list
                  //DWORD vehicle_list_header = *(DWORD*)(objlist+0x168);
                  MOV ECX,DWORD PTR [ECX+168h]
                  //DWORD num_of_veh = *(WORD*)(vehicle_list_header+0x20);
                  MOVZX EAX,WORD PTR [ECX+20h]
                  //DWORD vehicle_list = *(DWORD*)(vehicle_list_header+0x24);
                  MOV ECX,DWORD PTR [ECX+24h]
                  //if (num_of_veh)
                  TEST EAX,EAX
                  JE SHORT vehs_bad_list
                  PUSH EAX
                  PUSH ECX
                  CALL DWORD PTR SpawnObjAtPlayer0_func_address
                  ADD ESP,8
                  CMP EAX,-1
                  JNZ SHORT vehs_val_player
                  MOV ECX,DWORD PTR [FailPlyrNtSpwn]
                  MOV SpecificMsg,ECX
                  JMP SHORT vehs_failed

                  vehs_val_player:
                  TEST AX,AX
                  JNZ SHORT vehs_spawned
                  vehs_bad_list:
                  MOV ECX,DWORD PTR [FailBadSpawn]
                  MOV SpecificMsg,ECX
                  JMP SHORT vehs_failed

                  vehs_spawned:
                  MOV succeded,TRUE
               }

               SpecificMsg = L"Vehicles spawned next to %s";
               __asm vehs_failed:
            }
            else
               SpecificMsg = FailLowAdminLvl;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0);
            }
         }

         *Player0_index = 0;//change back to 0 when finished
         VirtualProtect(
            (LPVOID)Player0_index,
            sizeof(WORD),
            dwOldProtect,
            &dwOldProtect);
      }
   }
   else
      HaloSay(FailPlyrNtFnd, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Spawn_All_Weapons(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   short Player_Indexes[16];
   int pi_found = 0;

   if (ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found))
   {
      DWORD dwOldProtect;
      if (VirtualProtect(
         (LPVOID)Player0_index,
         sizeof(WORD),
         PAGE_EXECUTE_READWRITE,
         &dwOldProtect))
      {
         for (int i = 0; i < pi_found; i++)
         {
            wchar_t *SpecificMsg = NULL;

            if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
            {
               *Player0_index = Player_Indexes[i];
               __asm
               {
                  //DWORD objlist = *(DWORD*)ObjTagList_ptr_address;
                  MOV ECX,DWORD PTR [ObjTagList_ptr_address]
                  MOV ECX, DWORD PTR [ECX]
                  //DWORD num_of_objs = *(DWORD*)(objlist+0x14C);
                  //if (num_of_objs)
                  MOV EAX,DWORD PTR [ECX+14Ch]
                  TEST EAX,EAX
                  JE SHORT weps_bad_list
                  //DWORD weapon_list = *(DWORD*)(objlist+0x150);
                  MOV ECX,DWORD PTR [ECX+150h]
                  //if (weapon_list)
                  TEST ECX,ECX
                  JE SHORT weps_bad_list
                  PUSH EAX
                  PUSH ECX
                  CALL DWORD PTR [SpawnObjAtPlayer0_func_address]
                  ADD ESP,8
                  CMP EAX,-1
                  JNZ SHORT weps_val_player
                  MOV ECX,DWORD PTR [FailPlyrNtSpwn]
                  MOV SpecificMsg,ECX
                  JMP SHORT weps_failed

                  weps_val_player:
                  TEST AX,AX
                  JNZ SHORT weps_spawned
                  weps_bad_list:
                  MOV ECX,DWORD PTR [FailBadSpawn]
                  MOV SpecificMsg,ECX
                  JMP SHORT weps_failed

                  weps_spawned:
                  MOV succeded,1
               }

               SpecificMsg = L"Weapons spawned next to %s";
               __asm weps_failed:
            }
            else
               SpecificMsg = FailLowAdminLvl;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0);
            }
         }

         *Player0_index = 0;//change back to 0 when finished
         VirtualProtect(
            (LPVOID)Player0_index,
            sizeof(WORD),
            dwOldProtect,
            &dwOldProtect);
      }
   }
   else
      HaloSay(FailPlyrNtFnd, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Spawn_All_Powerups(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   short Player_Indexes[16];
   int pi_found = 0;

   if (ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found))
   {
      DWORD dwOldProtect;
      if (VirtualProtect(
         (LPVOID)Player0_index,
         sizeof(WORD),
         PAGE_EXECUTE_READWRITE,
         &dwOldProtect))
      {
         for (int i = 0; i < pi_found; i++)
         {
            wchar_t *SpecificMsg = NULL;

            if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
            {
               *Player0_index = Player_Indexes[i];
               __asm
               {
                  //DWORD objlist = *(DWORD*)ObjTagList_ptr_address;
                  MOV ECX,DWORD PTR [ObjTagList_ptr_address]
                  MOV ECX, DWORD PTR [ECX]
                  //DWORD num_of_objs = *(DWORD*)(objlist+0x158);
                  //if (num_of_objs)
                  MOV EAX,DWORD PTR [ECX+158h]
                  TEST EAX,EAX
                  JE SHORT pwrups_bad_list
                  //DWORD powerup_list = *(DWORD*)(objlist+0x15C);
                  MOV ECX,DWORD PTR [ECX+15Ch]
                  //if (powerup_list)
                  TEST ECX,ECX
                  JE SHORT pwrups_bad_list
                  PUSH EAX
                  PUSH ECX
                  CALL DWORD PTR [SpawnObjAtPlayer0_func_address]
                  ADD ESP,8
                  CMP EAX,-1
                  JNZ SHORT pwrups_val_player
                  MOV ECX,DWORD PTR [FailPlyrNtSpwn]
                  MOV SpecificMsg,ECX
                  JMP SHORT pwrups_failed

                  pwrups_val_player:
                  TEST AX,AX
                  JNZ SHORT pwrups_spawned
                  pwrups_bad_list:
                  MOV ECX,DWORD PTR [FailBadSpawn]
                  MOV SpecificMsg,ECX
                  JMP SHORT pwrups_failed

                  pwrups_spawned:
                  MOV succeded,TRUE
               }

               SpecificMsg = L"Powerups spawned next to %s";
               __asm pwrups_failed:
            }
            else
               SpecificMsg = FailLowAdminLvl;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0);
            }

         }

         *Player0_index = 0;//change back to 0 when finished
         VirtualProtect(
            (LPVOID)Player0_index,
            sizeof(WORD),
            dwOldProtect,
            &dwOldProtect);
      }
   }
   else
      HaloSay(FailPlyrNtFnd, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Copy_Vehicle(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   short Player_Indexes[16];
   int pi_found = 0;
   wchar_t *GenericMsg = NULL;

   int pi2_to_find = 1; short player2_index;
   if ((cmd_args += ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)) &&
      ParseCMDStrPlayers(++cmd_args, &player2_index, pi2_to_find))
   {
      if (TempAdmin[exec_player_index] >= TempAdmin[player2_index])
      {
         HaloCE_lib::SPARTAN *player2_object = GetPlayerObj(player2_index);
         if (player2_object)
         {
            //if player is in a vehicle, use vehicle's coords_or_vectors
            HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player2_object);
            if (vehicle_object) player2_object = vehicle_object;

            float coords[3];
            coords[0] = player2_object->m_World[0] + 1;
            coords[1] = player2_object->m_World[1] + 1;
            coords[2] = player2_object->m_World[2] + 0.5f;

            for (int i = 0; i < pi_found; i++)
            {
               wchar_t *SpecificMsg = NULL;

               if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
               {

                  HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]);
                  if (player_object)
                  {
                     //if player is in a vehicle, use vehicle's coords_or_vectors
                     HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
                     if (vehicle_object)
                     {
                        __asm
                        {
                           ;//__fastcall params
                           MOV EDX,DWORD PTR [vehicle_object]
                           MOV EDX,DWORD PTR [EDX]
                           LEA ECX,[coords]
                           CALL CreateObject
                           MOV succeded,TRUE
                        }

                        HaloSay(
                           L"%s's vehicle has been spawned next to %s",
                           exec_player_index,
                           players[player2_index].PlayerName0,
                           players[Player_Indexes[i]].PlayerName0);
                     }
                     else
                        SpecificMsg = FailPlyrNtInVeh;
                  }
                  else
                     SpecificMsg = FailPlyrNtSpwn;
               }
               else
                  SpecificMsg = FailLowAdminLvl;

               if (SpecificMsg)
               {
                  HaloSay(
                     SpecificMsg,
                     exec_player_index,
                     players[Player_Indexes[i]].PlayerName0);
               }
            }
         }
         else
            GenericMsg = FailPlyrNtSpwn;
      }
      else
         GenericMsg = FailLowAdminLvl;
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg)
      HaloSay(GenericMsg, exec_player_index, players[player2_index].PlayerName0);

   return succeded;
}

BOOL __fastcall Player::Copy_Weapon(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   short Player_Indexes[16];
   int pi_found = 0;
   wchar_t *GenericMsg = NULL;

   int pi2_to_find = 1; short player2_index;
   if ((cmd_args += ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)) &&
      ParseCMDStrPlayers(++cmd_args, &player2_index, pi2_to_find))
   {
      if (TempAdmin[exec_player_index] >= TempAdmin[player2_index])
      {
         HaloCE_lib::SPARTAN *player2_object = GetPlayerObj(player2_index);
         if (player2_object)
         {
            //if player is in a vehicle, use vehicle's coords_or_vectors
            HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player2_object);
            if (vehicle_object) player2_object = vehicle_object;

            float coords[3];
            coords[0] = player2_object->m_World[0] + 1;
            coords[1] = player2_object->m_World[1] + 1;
            coords[2] = player2_object->m_World[2] + 0.5f;

            for (int i = 0; i < pi_found; i++)
            {
               wchar_t *SpecificMsg = NULL;

               if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
               {

                  HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]);
                  if (player_object)
                  {
                     short weapon_index = player_object->WeaponTag.Index;

                     //if player is in a vehicle, use vehicle's weapon
                     //doesn't work on vehicle weapons
                     //short veh_wep_index = player_object->VehicleWeaponTag.Index;
                     //if (veh_wep_index != -1) weapon_index = veh_wep_index;

                     DWORD weapon_obj = GetObj(weapon_index);
                     if (weapon_obj)
                     {
                        __asm
                        {
                           ;//__fastcall params
                           MOV EDX,DWORD PTR [weapon_obj]
                           MOV EDX,DWORD PTR [EDX]
                           LEA ECX,[coords]
                           CALL CreateObject
                           MOV succeded,TRUE
                        }

                        HaloSay(
                           L"%s's weapon has been spawned next to %s",
                           exec_player_index,
                           players[player2_index].PlayerName0,
                           players[Player_Indexes[i]].PlayerName0);
                     }
                     else
                        SpecificMsg = FailPlyrNoWep;
                  }
                  else
                     SpecificMsg = FailPlyrNtSpwn;
               }
               else
                  SpecificMsg = FailLowAdminLvl;

               if (SpecificMsg)
               {
                  HaloSay(
                     SpecificMsg,
                     exec_player_index,
                     players[Player_Indexes[i]].PlayerName0);
               }
            }
         }
         else
            GenericMsg = FailPlyrNtSpwn;
      }
      else
         GenericMsg = FailLowAdminLvl;
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg)
      HaloSay(GenericMsg, exec_player_index, players[player2_index].PlayerName0);

   return succeded;
}

BOOL __fastcall Player::Destroy_Objects_Mode(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL;

   int enable;
   if (!*cmd_args)
   {
      enable = DestroyObjsEnabled;
      GenericMsg = L"Destroy objects mode is set at %u";
      succeded = TRUE;
   }
   else if (CMDsLib::ParseStrInt(cmd_args, &enable))
   {
      DestroyObjsEnabled = enable;
      GenericMsg = L"Destroy objects mode has been set to %u";
      succeded = TRUE;
   }
   else
      GenericMsg = FailInvalNum;

   HaloSay(GenericMsg, exec_player_index, enable);

   return succeded;
}

BOOL __fastcall Player::Destroy_Weapon(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   short Player_Indexes[16];
   int pi_found = 0;

   if (ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found))
   {
      for (int i = 0; i < pi_found; i++)
      {
         wchar_t *SpecificMsg = NULL;

         if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
         {
            HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]);
            if (player_object)
            {
               HaloCE_lib::OBJECT_TAG WepObjTag = player_object->WeaponTag;
               //use vehicle turret if valid tag
               HaloCE_lib::OBJECT_TAG VehWepObjTag = player_object->VehicleWeaponTag;
               if (VehWepObjTag.Tag != -1) WepObjTag = VehWepObjTag;

               if (WepObjTag.Tag != -1)
               {
                  __asm
                  {
                     MOV EAX,WepObjTag
                     CALL DWORD PTR [DestroyObj_func_address]
                     MOV succeded,TRUE
                  }
                  SpecificMsg = L"%s's weapon has been destroyed";
               }
               else
                  SpecificMsg = FailPlyrNoWep;
            }
            else
               SpecificMsg = FailPlyrNtSpwn;
         }
         else
            SpecificMsg = FailLowAdminLvl;

         if (SpecificMsg)
         {
            HaloSay(
               SpecificMsg,
               exec_player_index,
               players[Player_Indexes[i]].PlayerName0);
         }
      }
   }
   else
      HaloSay(FailPlyrNtFnd, exec_player_index);

   return succeded;
}

BOOL __fastcall Player::Say(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   short Player_Indexes[16];
   int pi_found = 0;
   wchar_t *msg_to = cmd_args, *GenericMsg = NULL;;

   if (*ServerType == HOST)
   {
      int arg_len;
      if ((arg_len = ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)))
      {
         cmd_args += arg_len;
         if (pi_found == (*Players_ptr)->NumOfItems)
         {
           HaloSay(++cmd_args, -1);
         }
         else
         {
            msg_to[cmd_args - msg_to] = 0;//add null terminator

            wchar_t *sv_buffer = &HaloSay_server_buffer[SV_NAME_SIZE];
            swprintf_s(
               sv_buffer,
               SV_BUFFER_SIZE,
               L"[Private Message][from %s][to %s]: %s",
               players[exec_player_index].PlayerName0,
               msg_to,
               ++cmd_args);

            CHAT_INFO chat_info;
            chat_info.ChatType = Server;
            chat_info.From_PlayerIndex = 0;
            chat_info.msg_str = sv_buffer;
            for (int i = 0; i < pi_found; i++)
            {
               ServerSay(chat_info, players[Player_Indexes[i]].PlayerChatIndex);

               HaloSay(
                  L"Message sent to %s",
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0);
            }
         }

         succeded = TRUE;
      }
      else
         GenericMsg = FailPlyrNtFnd;
   }
   else
      GenericMsg = FailSvCmd;

   if (GenericMsg) HaloSay(GenericMsg, exec_player_index);

   return succeded | DO_NOT_SEND_MSG;
}

BOOL __fastcall Player::ObjectScale(wchar_t *cmd_args, short exec_player_index)
{
   BOOL succeded = FALSE;
   wchar_t *GenericMsg = NULL, *SpecificMsg = NULL;

   short Player_Indexes[16];
   int pi_found = 0;

   float scale;

   int arg_len;
   if ((arg_len = ParseCMDStrPlayers(cmd_args, Player_Indexes, pi_found)))
   {
      cmd_args += arg_len;

      if (!*cmd_args)
      {
         for (int i = 0; i < pi_found; i++)
         {
            SpecificMsg = NULL;

            HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]);
            if (player_object)
            {
               //if player is in a vehicle, use vehicle's coords_or_vectors
               HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
               if (vehicle_object) player_object = vehicle_object;

               succeded = TRUE;
               SpecificMsg = L"%s's current object scale is %.2f%%";
            }
            else
               SpecificMsg = FailPlyrNtSpwn;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0,
                  player_object->obj_scale * 100.0f);
            }
         }
      }
      else if (CMDsLib::ParseStrFloat(++cmd_args, &scale))
      {
         for (int i = 0; i < pi_found; i++)
         {
            SpecificMsg = NULL;

            if (TempAdmin[exec_player_index] >= TempAdmin[Player_Indexes[i]])
            {
               HaloCE_lib::SPARTAN *player_object = GetPlayerObj(Player_Indexes[i]);
               if (player_object)
               {
                  //if player is in a vehicle, use vehicle's coords_or_vectors
                  HaloCE_lib::SPARTAN *vehicle_object = (HaloCE_lib::SPARTAN*)GetPlayerVehObj(player_object);
                  if (vehicle_object) player_object = vehicle_object;

                  player_object->obj_scale = scale;

                  succeded = TRUE;
                  SpecificMsg = L"%s's object scale has been changed to %.2f%%";
               }
               else
                  SpecificMsg = FailPlyrNtSpwn;
            }
            else
               SpecificMsg = FailLowAdminLvl;

            if (SpecificMsg)
            {
               HaloSay(
                  SpecificMsg,
                  exec_player_index,
                  players[Player_Indexes[i]].PlayerName0,
                  scale * 100.0f);
            }
         }
      }
      else
         GenericMsg = FailInvalNum;
   }
   else
      GenericMsg = FailPlyrNtFnd;

   if (GenericMsg) HaloSay(GenericMsg, exec_player_index);

   return succeded;
}