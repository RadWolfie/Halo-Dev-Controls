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
    File:    cmds.h
    Project: Halo Dev Controls and HDC
    Author:  Jesus7Freak
    Date:    11/22/2011
    Game:    Halo and Halo Custom Edition
    Version: all
*********************************************************************************/
#pragma once

#define CMD_STR_SIZE 36

#define CMD_SET_SIZE 3

#define HALO_CMDS_SIZE 28
#define RPGB_CMDS_SIZE 8
#define PLAYER_CMDS_SIZE 30

namespace CMDsLib
{
   //for chat commands window
   struct CMD_DESCRIPT
   {
      wchar_t *cmd_title;//[20];
      wchar_t *cmd_header;
      wchar_t *cmd_usage;//[60]; //max 94
      wchar_t *cmd_descript;//[60]; //max 94
   };

   //written/read from file and to dll
   struct CMD_SCKEYS
   {
      short toggle_key;
      short on_key;
      short off_key;
   };

   struct COMMANDS
   {
      wchar_t *cmds_group_name;
      int size;
      int Enable_Shrtcts;
      CMD_DESCRIPT *cmd_descripts;
      char **cmd_strs;
      CMD_SCKEYS *cmd_keys;
   };

   void GetSKeysFromFile(wchar_t *FileName);
   void WriteSKeysToFile(wchar_t *FileName);

   int GetCMDArgCount(wchar_t *cmd_str);
   int FindCMDArgIndex(wchar_t *cmd_str, int num_of_spaces);
   int ParseStrFloat(wchar_t *str_num, float *num);
   int ParseStrInt(wchar_t *str_num, int *num);
   int ParseStrBool(wchar_t *str_bool, int *boolean);

   extern CMD_DESCRIPT halo_cmd_descripts[HALO_CMDS_SIZE];
   extern char *halo_cmd_strs[HALO_CMDS_SIZE];
   extern CMD_SCKEYS halo_cmd_keys[HALO_CMDS_SIZE];

   extern CMD_DESCRIPT rpg_beta6_2_cmd_descripts[RPGB_CMDS_SIZE];
   extern char *rpg_beta6_2_cmd_strs[RPGB_CMDS_SIZE];
   extern CMD_SCKEYS rpg_beta6_2_cmd_keys[RPGB_CMDS_SIZE];

   extern CMD_DESCRIPT player_cmd_descripts[PLAYER_CMDS_SIZE];
   extern char *player_cmd_strs[PLAYER_CMDS_SIZE];

   extern COMMANDS halo_commands;
   extern COMMANDS rpgbeta6_2_commands;
   extern COMMANDS player_commands;

   extern COMMANDS *all_commands[CMD_SET_SIZE];
}