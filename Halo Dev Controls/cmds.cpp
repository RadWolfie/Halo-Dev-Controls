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
	File:    cmds.cpp
	Project: Halo Dev Controls and HDC
	Author:  Jesus7Freak
	Date:    11/22/2011
	Game:    Halo and Halo Custom Edition
	Version: all
*********************************************************************************/
#include "cmds.h"
// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#include <windows.h>

namespace CMDsLib
{
   void WriteSKeysToFile(wchar_t *FileName)
   {
      HANDLE hFile;
      if ((hFile = CreateFileW(
         FileName,
         GENERIC_WRITE,
         FILE_SHARE_WRITE,
         NULL,
         CREATE_ALWAYS,
         FILE_ATTRIBUTE_NORMAL,
         NULL)) != INVALID_HANDLE_VALUE)
      {
         DWORD size_in_bytes = (sizeof(CMD_SCKEYS) * (HALO_CMDS_SIZE + RPGB_CMDS_SIZE))
            + (sizeof(bool) * 2);

         char *buffer = new char[size_in_bytes];

         char *byte_p_hck = (char*)&halo_cmd_keys;
         int hcs = sizeof(CMD_SCKEYS) * HALO_CMDS_SIZE;
         DWORD buffer_loc = hcs;
         for (int bi = 0; bi < hcs; bi++)
            buffer[bi] = byte_p_hck[bi];


         char *byte_p_rbck = (char*)&rpg_beta6_2_cmd_keys;
         char *byte_p_buffer = &buffer[buffer_loc];
         int rbcs = sizeof(CMD_SCKEYS) * RPGB_CMDS_SIZE;
         buffer_loc += rbcs;
         for (int bi = 0; bi < rbcs; bi++)
            byte_p_buffer[bi] = byte_p_rbck[bi];

         buffer[buffer_loc++] = (char)halo_commands.Enable_Shrtcts;
         buffer[buffer_loc] = (char)rpgbeta6_2_commands.Enable_Shrtcts;

         DWORD written;
         if(!WriteFile(hFile, buffer, size_in_bytes, &written, NULL))
            DWORD dosretval = GetLastError();

         delete[] buffer;
         CloseHandle(hFile);
      }
   }

   void GetSKeysFromFile(wchar_t *FileName)
   {
      HANDLE hFile;
      if ((hFile = CreateFileW(
         FileName,
         GENERIC_READ,
         FILE_SHARE_READ,
         NULL,
         OPEN_EXISTING,
         FILE_ATTRIBUTE_NORMAL,
         NULL)) != INVALID_HANDLE_VALUE)
      {
         DWORD size_in_bytes = (sizeof(CMD_SCKEYS) * (HALO_CMDS_SIZE + RPGB_CMDS_SIZE))
            + (sizeof(bool) * 2);

         char *buffer = new char[size_in_bytes];

         DWORD bytes_read;
         if (!ReadFile(hFile, buffer, size_in_bytes, &bytes_read, NULL))
            DWORD dosretval = GetLastError();

         char *byte_p_hck = (char*)&halo_cmd_keys;
         int hcs = sizeof(CMD_SCKEYS) * HALO_CMDS_SIZE;
         DWORD buffer_loc = hcs;
         for (int bi = 0; bi < hcs; bi++)
            byte_p_hck[bi] = buffer[bi];

         char *byte_p_rbck = (char*)&rpg_beta6_2_cmd_keys;
         char *byte_p_buffer = &buffer[buffer_loc];
         int rbcs = sizeof(CMD_SCKEYS) * RPGB_CMDS_SIZE;
         buffer_loc += rbcs;
         for (int bi = 0; bi < rbcs; bi++)
            byte_p_rbck[bi] = byte_p_buffer[bi];

         halo_commands.Enable_Shrtcts = (int)buffer[buffer_loc++];
         rpgbeta6_2_commands.Enable_Shrtcts = (int)buffer[buffer_loc];

         delete[] buffer;
         CloseHandle(hFile);
      }
   }

   /*requires #include <fstream>
   void WriteSKeysToFile(wchar_t *FileName)
   {
      std::fstream File;
      File.open(FileName, std::ios::out | std::ios::binary);
      //save settings changes to file
      File.write(
         reinterpret_cast<char*>(&halo_cmd_keys),
         sizeof(CMD_SCKEYS) * HALO_CMDS_SIZE
         );
      File.write(
         reinterpret_cast<char*>(&rpg_beta6_2_cmd_keys),
         sizeof(CMD_SCKEYS) * RPGB_CMDS_SIZE
         );
      File.write(
         reinterpret_cast<char*>(&halo_commands.Enable_Shrtcts),
         sizeof(bool)
         );
      File.write(
         reinterpret_cast<char*>(&rpgbeta6_2_commands.Enable_Shrtcts),
         sizeof(bool)
         );
      File.close();
   }

   void GetSKeysFromFile(wchar_t *FileName)
   {
      std::fstream File;
      File.open(FileName, std::ios::in | std::ios::binary);
      if (!File.fail())
      {
         File.read(
            reinterpret_cast<char*>(&halo_cmd_keys),
            sizeof(CMD_SCKEYS) * HALO_CMDS_SIZE
            );
         File.read(
            reinterpret_cast<char*>(&rpg_beta6_2_cmd_keys),
            sizeof(CMD_SCKEYS) * RPGB_CMDS_SIZE
            );
         File.read(
            reinterpret_cast<char*>(&halo_commands.Enable_Shrtcts),
            sizeof(bool)
            );
         File.read(
            reinterpret_cast<char*>(&rpgbeta6_2_commands.Enable_Shrtcts),
            sizeof(bool)
            );
         File.close();
      }
   }*/

   int GetCMDArgCount(wchar_t *cmd_str)
   {
      //count the number of spaces
      int arg_count = 0; bool is_in_parenth = false;
      wchar_t wchar;

      do
      {
         wchar = *cmd_str++;

         if (wchar == L'\"')
            is_in_parenth = !is_in_parenth;

         if (wchar == ' ' && !is_in_parenth)
            ++arg_count;

      }while (wchar);

      return arg_count;
   }

   int FindCMDArgIndex(wchar_t *cmd_str, int num_of_spaces)
   {
      //count the number of spaces
      int str_2arg_index = 0; bool is_in_parenth = false;
      for (int spaces_cnt = 0; cmd_str[str_2arg_index] && spaces_cnt < num_of_spaces; str_2arg_index++)
      {
         if (cmd_str[str_2arg_index] == '\"')
            is_in_parenth = !is_in_parenth;

         if (cmd_str[str_2arg_index] == ' ' && !is_in_parenth)
            spaces_cnt++;
      }
      return str_2arg_index;
   }

   int ParseStrFloat(wchar_t *str_num, float *num)
   {
      bool succeded = true, negative = false; int wchar_i = 0;
      float _num = 0; int num_of_digits = 0, fp_index = 0;

      //check if its a negative number
      if (*str_num == '-')
      {
         negative = true;
         wchar_i++;
         fp_index--;
      }

      for (;str_num[wchar_i] && str_num[wchar_i] != L' '; wchar_i++)
      {
         //check if its not a number
         if (str_num[wchar_i] < '.' || str_num[wchar_i] > '9')
         {
            succeded = false;
            break;
         }

         //check if its a floating point number
         if (str_num[wchar_i] == '.')
         {
            fp_index += wchar_i;
            continue;
         }

         _num *= 10;
         _num += str_num[wchar_i] - 48;

         num_of_digits++;
      }

      for (int i = 0; fp_index > 0 && i < num_of_digits - fp_index; i++)
         _num /= 10.0f;

      if (negative)
         _num = 0 - _num;

      *num = _num;

      if (!succeded) wchar_i = 0;

      return wchar_i;
   }

   //returns the characters parsed count
   int ParseStrInt(wchar_t *str_num, int *num)
   {
      bool succeded = true;
      bool negative = false; int wchar_i = 0;
      int _num = 0; int num_of_digits = 0;

      //check if its a negative number
      if (*str_num == '-')
      {
         negative = true;
         wchar_i++;
      }

      for (;str_num[wchar_i] && str_num[wchar_i] != L' '; wchar_i++)
      {
         //check if its not a number
         if (str_num[wchar_i] < '0' || str_num[wchar_i] > '9')
         {
            succeded = false;
            break;
         }

         _num *= 10;
         _num += str_num[wchar_i] - 48;

         num_of_digits++;
      }

      if (negative)
         _num = 0 - _num;

      *num = _num;

      if (!succeded) wchar_i = 0;

      return wchar_i;
   }

   int ParseStrBool(wchar_t *str_bool, int *boolean)
   {
      int succeded = true; int wchar_i = 0;

      if (!str_bool[1] || str_bool[1] == L' ')
      {
         wchar_i = ParseStrInt(str_bool, boolean);
      }
      else
      {
         char *strue = "true";

         for (wchar_i = 0; wchar_i <= 4; wchar_i++)
         {
            if (strue[wchar_i] != (char)str_bool[wchar_i])
            {
               succeded = false;
               break;
            }
         }

         if (succeded) *boolean = TRUE;
         else
         {
            succeded = true;
            char *sfalse = "false";

            for (wchar_i = 0; wchar_i <= 5; wchar_i++)
            {
               if (sfalse[wchar_i] != (char)str_bool[wchar_i])
               {
                  succeded = false;
                  break;
               }
            }

            if (succeded) *boolean = FALSE;
            else wchar_i = 0;
         }
      }
      return wchar_i;
   }

   CMD_DESCRIPT halo_cmd_descripts[HALO_CMDS_SIZE] =
   {
      {//0
         L"Command Help",
         L"/help <string>",
         L"<string>: Displays info on the command specified"
      }, //2346578911234567892123456789312345678941234567895123456789612345678971234567898123456789912234
      {//1
         L"List Commands",
         L"/list_cmds",
         L"Lists all the server commands, to list all of the commands, use / in console, and press tab."
      },
      {//2
         L"List Teleport Locs",
         L"/list_locs",
         L"Lists all the teleport locations for that map."
      },
      {//3
         L"Console",
         L"/console <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Enable Console"
      },
      {//4
         L"Dev Mode",
         L"/dev <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Enable Developers Mode"
      },
      {//5
         L"Deathless",//cheat_deathless_player
         L"/deathless <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Makes the players invulnerable to almost all damage."
      },
      {//6
         L"Infinite Ammo",//cheat_infinite_ammo
         L"/infammo <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Gives the players unlimited ammunition."
      },
      {//7
         L"Bottomless Clip",//cheat_bottomless_clip
         L"/bottomless <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Players never have to reload, and weapons never overheat."
      },
      {//8
         L"Show Hud",//show_hud
         L"/hud <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Turns HUD off/on"
      },
      {//9
         L"LetterBox",//cinematic_show_letterbox
         L"/letterbox <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Sets or removes the letterbox bars."
      },
      {//10
         L"Rider Ejection",//rider_ejection
         L"/ejection <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Toggles wether the passenger(s) will fall out of vehicle if fliped."
      },
      {//11
         L"Omnipotent",//cheat_omnipotent
         L"/one_shot <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Player's shots kills anything they hit."
      },
      {//12
         L"Fall Damage",//cheat_jetpack
         L"/fall_damage <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Toggles fall damage."
      },
      {//13
         L"Bump Possession",//cheat_bump_possession
         L"/possess <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Allows the player to control any character by bumping into them."
      },
      {//14
         L"Super Jump",//cheat_super_jump
         L"/super_jump <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Allows the players to jump very high."
      },
      {//16
         L"Medusa",//cheat_medusa
         L"/medusa <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Any enemy unit that sees the player will be instantly killed."
      },
      {//15
         L"Reflexive Damage",//cheat_reflexive_damage_effects
         L"/reflex_damage <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Your Direction of Fire Indicator displays when you attack the enemy too."
      },
      {//17
         L"Xbox Controller",//cheat_controller
         L"/controller <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Enables Xbox controller to change cheats ingame."
      },
      {//18
         L"WireFrame",//rasterizer_wireframe
         L"/wireframe <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Shows the map in wireframe."
      },
      {//19
         L"Fog",//rasterizer_fog_atmosphere
         L"/fog <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Turns atmospheric fog on/off."
      },
      {//20
         L"FogPlane",//rasterizer_fog_plane
         L"/fogplane <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Turns fog plane on/off."
      },
      {//21
         L"FPS",//rasterizer_fps
         L"/fps <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Displays FPS for halo."
      },
      {//22
         L"Game Speed",
         L"/game_speed <short>",
         L"<short>: 0 to 20"
         L"game_speed only works in single player.",
      },
      {//23
         L"Rapid Fire",
         L"/rapid_fire <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Increases rate of fire."
      },
      {//24
         L"Time Freeze",
         L"/time_freeze <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off"
         L"Freezes all objects, except players."
      },
      {//25
         L"Gravity Boots",
         L"/grav_boots <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Walking anywere."
      },
      {//26
         L"Vehicle No Team",
         L"/veh_no_team <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Vehicle no team restriction \nplayers from different teams can enter \nthe same vehicle."
      },
      /*{//27
         L"Exec Console Func",
         L"/e <string>",
         L"Executes any console function."
      },*/
      {//28
         L"Marines HUD",
         L"/mhud <short>",
         L"<short>: 0 = off, 1 = on, 2 = on with no static",
         L"Sets Marines Recording Vision."
      }
   };

   char *halo_cmd_strs[HALO_CMDS_SIZE] =
   {
      "/help ",//0
      "/list_cmds ",//1
      "/list_locs ",//2
      "/console ",//3
      "/dev ",//4
      "/deathless ",//5
      "/infammo ",//6
      "/bottomless ",//7
      "/hud ",//8
      "/letterbox ",//9
      "/ejection ",//10
      "/one_shot ",//11
      "/fall_damage ",//12
      "/possess ",//13
      "/super_jump ",//14
      "/medusa ",//16
      "/reflex_damage ",//15
      "/controller ",//17
      "/wireframe ",//18
      "/fog ",//19
      "/fogplane ",//20
      "/fps ",//21
      "/game_speed ",//22
      "/rapid_fire ",//23
      "/time_freeze ",//24
      "/grav_boots ",//25
      "/veh_no_team ",//26
      //"/e ",//27
      "/mhud "//28
   };

   //data that can be written to file
   CMD_SCKEYS halo_cmd_keys[HALO_CMDS_SIZE] =
   {
      {-1, -1, -1},//0
      {-1, -1, -1},//1
      {-1, -1, -1},//2
      {0, 0, 0},//3
      {0, 33, 34},//4
      {97, 0, 0},//5
      {98, 0, 0},//6
      {0, 0, 0},//7
      {99, 0, 0},//8
      {100, 0, 0},//9
      {0, 0, 0},//10
      {0, 0, 0},//11
      {0, 0, 0},//12
      {0, 0, 0},//13
      {0, 0, 0},//14
      {0, 0, 0},//15
      {0, 0, 0},//16
      {0, 0, 0},//17
      {0, 0, 0},//18
      {0, 0, 0},//19
      {0, 0, 0},//20
      {0, 0, 0},//21
      {-1, -1, -1},//22
      {111, 0, 0},//23
      {106, 0, 0},//24
      {109, 0, 0},//25
      {0, 0, 0},//26
      //{-1, -1, -1},//27
      {101, 0, 0}//28
   };


   CMD_DESCRIPT rpg_beta6_2_cmd_descripts[RPGB_CMDS_SIZE] =
   {
      {//0
         L"Setting - Day",//set setting 0
         L"/day",
         L"This will change the environment of the map."
      },
      {//1
         L"Setting - Rain",//set setting 1
         L"/rain",
         L"This will change the environment of the map."
      },
      {//2
         L"Setting - Night",//set setting 2
         L"/night",
         L"This will change the environment of the map."
      },
      {//3
         L"Air Base Alarm",//device_set_position alarm_control_1 1
         L"/alarm <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"This will toggle Red Alert on or off."
      },
      {//4
         L"Air Base LockDown",//device_set_position lock_control 1
         L"/lockdown",
         L"This will activate Lockdown mode."
      },
      {//5
         L"Fire Halo",//device_set_position boom_control 1
         L"/fire_halo",
         L"This will initiate Halo's activation."
      },
      {//6
         L"LockDown Timer",//lock_timer (* 30 [# of seconds])
         L"/lockdown_timer <short>",
         L"<short>: 0 to 999 seconds",
         L"This will change the amount of time that the doors stay closed while Lockdown is activated."
      },
      {//7
         L"Fire Halo Timer",//boom_timer (* 30 [# of seconds])
         L"/halo_timer <short>",
         L"<short>: 0 to 999 seconds",
         L"This will change the minimum delay between firings of Halo."
      }
   };

   char *rpg_beta6_2_cmd_strs[RPGB_CMDS_SIZE] =
   {
      "/day ",//0
      "/rain ",//1
      "/night ",//2
      "/alarm ",//3
      "/lockdown ",//4
      "/fire_halo ",//5
      "/lockdown_timer ",//6
      "/halo_timer "//7
   };

   CMD_SCKEYS rpg_beta6_2_cmd_keys[RPGB_CMDS_SIZE] =
   {
      {102, -1, -1},//0
      {-1, -1, -1},//1
      {-1, -1, -1},//2
      {103, 0, 0},//3
      {104, -1, -1},//4
      {105, -1, -1},//5
      {-1, -1, -1},//6
      {-1, -1, -1}//7
   };

   CMD_DESCRIPT player_cmd_descripts[PLAYER_CMDS_SIZE] =
   {
      {//0
         L"Speed",
         L"/spd [pExpression] <real>",
         L"<real>: 0 to 999",
         L"Adjusts players speed."
      },
      {//1
         L"Active Camo",//cheat_active_camouflage
         L"/camo [pExpression] <short>",
         L"<short>: camo duration in seconds",
         L"Gives player active camo."
      },
      {//2
         L"Suspend",//unit_suspended
         L"/suspend [pExpression] <boolean>",
         L"<boolean>: 1 or true = on, 0 or false = off",
         L"Suspends player."
      },
      {//3
         L"Teleport",
         L"/t [pExpression] [Coordinates]",
         L"[Coordinates]: x<real>, y<real>, z<real> or player or location_name",
         L"Teleport player to x y z coordinates \nor to a defined location."
      },
      {//5
         L"Jump Teleport",
         L"/j [pExpression] <real> <real> <real>",
         L"x<real>, y<real>, z<real>",
         L"Teleport player to current position, \ncoordinates with offset."
      },
      {//6
         L"Velocity",
         L"/v [pExpression] <real> <real> <real>",
         L"x<real>, y<real>, z<real>",
         L"Changes the players velocity."
      },
      {//7
         L"Ammo",
         L"/ammo [pExpression] <short>",
         L"<short>: 0 to 999",
         L"Adjusts player ammo."
      },
      {//8
         L"Battery",
         L"/bat [pExpression] <real>",
         L"<real>: 0 to 999",
         L"Adjusts player battery."
      },
      {//9
         L"Health",//unit_set_maximum_vitality  unit_set_current_vitality
         L"/health [pExpression] <real>",
         L"<real>: % non-negative",
         L"Adjusts player health, percent of health."
      },
      {//10
         L"Shield",//unit_set_maximum_vitality  unit_set_current_vitality
         L"/shield [pExpression] <real>",
         L"<real>: % non-negative",
         L"Adjusts player shieldws, percent of shields."
      },
      {//11
         L"AFK",
         L"/afk [pExpression]",
         L"Player doesn't respawn in afk mode, \nwhen used while dead, disables afk mode."
      },
      {//12
         L"Team Change",//change_team
         L"/team [pExpression]",
         L"Changes player to other team."
      },
      {//13
         L"Kick",//sv_kick
         L"/k [pExpression]",
         L"Kicks player from server."
      },
      {//14
         L"Ban",//sv_ban
         L"/b [pExpression]",
         L"Bans player from server."
      },
      {//15
         L"Kill",//unit_kill
         L"/kill [pExpression]",
         L"Kills a player."
      },
      {//16
         L"Eject",//unit_exit_vehicle
         L"/eject [pExpression]",
         L"Ejects a player from a vehicle"
      },
      {//17
         L"Flip Vehicle",
         L"/flip [pExpression]",
         L"Flips a vehicle 180°, up-right or down"
      },
      {//18
         L"Admin",
         L"/a [pExpression] <int>",
         L"<int>: non-negative, \ndefault clients admin level = 0",
         L"Gives player temporary admin level."
      },
      {//19
         L"Set Teleport Loc",
         L"/st [pExpression] <string>",
         L"<string>: no spaces in location name",
         L"Defines a teleport location at player's coordinates."
      },
      {
         L"Spawn Biped",
         L"/biped [pExpression] <short>",
         L"<short>: amount of bipeds to spawn"
         L"Spawns bipeds near a player."
      },
      {//20
         L"Spawn Hog",//cheat_spawn_warthog
         L"/hog [pExpression]",
         L"Spawn a hog near a player."
      },
      {//21
         L"Spawn All Vehicles",//cheat_all_vehicles
         L"/vehs [pExpression]",
         L"Spawns all vehicles near a player."
      },
      {//22
         L"Spawn All Weapons",//cheat_all_weapons
         L"/weps [pExpression]",
         L"Spawns all weapons near a player."
      },
      {//23
         L"Spawn All PowerUps",//cheat_all_powerups
         L"/powerups [pExpression]",
         L"Spawns all powerups near a player."
      },
      {//24
         L"Copy Vehicle",//object_create
         L"/copy_veh [from pExpression] [to pExpression]",
         L"Copys the player's vehicle and spawns it near a player."
      },
      {//25
         L"Copy Weapon",//object_create
         L"/copy_wep [from pExpression] [to pExpression]",
         L"Copys the player's weapon and spawns it near a player."
      },
      {//26
         L"Destroy Objects Mode",//object_destroy
         L"/dest_objs_mode <boolean>",
         L"Destroys any object the player shoots at."
      },
      {//27
         L"Destroy Weapon",//object_destroy
         L"/dest_wep [pExpression]",
         L"Destroys the player's weapon, including vehicle weapons."
      },
      {//28
         L"Say",//sv_say
         L"/say [pExpression] <string>",
         L"Sends a message to a specific player."
      },
      {//29
         L"Object Scale",//object_set_scale
         L"/scale [pExpression] <real>",
         L"Changes a player's object scale."
      }
   };

   char *player_cmd_strs[PLAYER_CMDS_SIZE] =
   {
      "/spd ",//0
      "/camo ",//1
      "/suspend ",//2
      "/t ",//3
      "/j ",//5
      "/v ",//6
      "/ammo ",//7
      "/bat ",//8
      "/health ",//9
      "/shield ",//10
      "/afk ",//11
      "/team ",//12
      "/k ",//13
      "/b ",//14
      "/kill ",//15
      "/eject ",//16
      "/flip ",//17
      "/a ",//18
      "/st ",//19
      "/biped ",
      "/hog ",//20
      "/vehs ",//21
      "/weps ",//22
      "/powerups ",//23
      "/copy_veh ",//24
      "/copy_wep ",//25
      "/dest_objs_mode ",//26
      "/dest_wep ",//27
      "/say ",//28
      "/scale "//29
   };

   COMMANDS halo_commands =
   {
      L"Dev        ", HALO_CMDS_SIZE, 1,
      reinterpret_cast<CMD_DESCRIPT*>(&halo_cmd_descripts),
      reinterpret_cast<char**>(&halo_cmd_strs),
      reinterpret_cast<CMD_SCKEYS*>(&halo_cmd_keys)
   };

   COMMANDS rpgbeta6_2_commands =
   {
      L"RPG_Beta6_2", RPGB_CMDS_SIZE, 1,
      reinterpret_cast<CMD_DESCRIPT*>(&rpg_beta6_2_cmd_descripts),
      reinterpret_cast<char**>(&rpg_beta6_2_cmd_strs),
      reinterpret_cast<CMD_SCKEYS*>(&rpg_beta6_2_cmd_keys)
   };

   COMMANDS player_commands =
   {
      L"Player     ", PLAYER_CMDS_SIZE, -1,
      reinterpret_cast<CMD_DESCRIPT*>(&player_cmd_descripts),
      reinterpret_cast<char**>(&player_cmd_strs),
      NULL
   };

   COMMANDS *all_commands[CMD_SET_SIZE] =
   {
      &player_commands,//these are used most
      &halo_commands,
      &rpgbeta6_2_commands
   };
}