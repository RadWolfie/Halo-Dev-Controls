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
	File:    HaloLib.h
	Project: Halo Dev Controls and HDC
	Authors: Steve(Del), Abyll, Rad!, and Jesus7Freak
	Date:    11/22/2011
	Game:    Halo and Halo Custom Edition
	Version: all

   credit to Steve(Del)... for the player info structures
   Abyll, and Rad! for further functionality of halo structures
*********************************************************************************/
#pragma once
#include "RWMemory.h"

using namespace ReadWriteMemoryTools;

namespace HaloCE_lib
{
   extern ADDRESS_SIG Current_map_addr_sig;
   extern ADDRESS_SIG Rider_Eject_addr_sig;
   extern ADDRESS_SIG Console_addr_sig;
   extern ADDRESS_SIG Base_ptr_addr_sig;
   extern ADDRESS_SIG Cheats_addr_sig;
   extern ADDRESS_SIG Show_Hud_ptr_addr_sig;
   extern ADDRESS_SIG letterbox_ptr_addr_sig;
   extern ADDRESS_SIG Rasterizer_addr_sig;
   extern ADDRESS_SIG Cinematic_addr_sig;
   
   enum SERVER_TYPE { main_menu = 0, client = 1, host = 2 };
   //extern ADDRESS_SIG ServerType_ptr_addr_sig; use sv_kick sig
   extern ADDRESS_SIG Game_Speed_ptr_addr_sig;
   extern ADDRESS_SIG Grav_Boots_addr_sig;
   extern ADDRESS_SIG Time_Freeze_addr_sig;
   extern ADDRESS_SIG Rapid_Fire_addr_sig;
   extern ADDRESS_SIG Vehicle_NTR_addr_sig;
   extern ADDRESS_SIG Local_Chat_func_addr_sig;
   extern ADDRESS_SIG Local_Chat_hook_addr_sig;
   extern ADDRESS_SIG Server_Chat_hook_addr_sig;
   extern ADDRESS_SIG SendChatMsgToPlayer_func_addr_sig;
   extern ADDRESS_SIG SendChatMsgToAll_func_addr_sig;
   extern ADDRESS_SIG Console_hook_addr_sig;
   extern ADDRESS_SIG Console_func_addr_sig;
   extern ADDRESS_SIG Rcon_hook_addr_sig;
   extern ADDRESS_SIG EngineDrawText_func_addr_sig;
   extern ADDRESS_SIG UnitExitVehicle_func_addr_sig;
   extern ADDRESS_SIG SpawnHog_func_addr_sig;
   //object tag list use [SpawnHog_func_addr_sig + 1]
   extern ADDRESS_SIG SpawnObjAtPlayer0_func_addr_sig;
   extern ADDRESS_SIG DestroyObj_func_addr_sig;
   extern ADDRESS_SIG sv_kick_func_addr_sig;
   extern ADDRESS_SIG sv_ban_func_addr_sig;
   extern ADDRESS_SIG cse_set_video_func_addr_sig;
   extern ADDRESS_SIG console_tablist_hook_addr_sig;
   extern ADDRESS_SIG console_tablist_vars_hook_addr_sig;
   extern ADDRESS_SIG data_header_bug_addr_sig;
   //extern ADDRESS_SIG map_info_struct_ptr_addr_sig;
   extern ADDRESS_SIG CreateScriptedObj_func_addr_sig;
   extern ADDRESS_SIG scripted_objtags_ptr_addr_sig;
   extern ADDRESS_SIG CreateObject_func_addr_sig;
   extern ADDRESS_SIG ActiveCamo_func_addr_sig;
   extern ADDRESS_SIG PlayerDeath_func_addr_sig;
   
   namespace Console
   {
      extern const BYTE Check_offset;
      extern const BYTE Enabled_offset;
      extern const BYTE Text_offset;
      extern const WORD C_Buffers_offset;
      extern const WORD C_Buffers_index_offset;
   }
   
   //for internal processes
   struct RASTERIZER
   {
      BYTE FPS;
      BYTE Unknown1[5];
      BYTE WireFrame;
      BYTE Unknown2[0x20];
      BYTE FogAtmosphere;
      BYTE FogPlane;
   };
   
   //for external processes
   namespace RasterizerEx
   {
      extern const BYTE FPS_offset;
      extern const BYTE WireFrame_offset;
      extern const BYTE FogAtmosphere_offset;
      extern const BYTE FogPlane_offset;
   }
   
   namespace Cinematic
   {
      extern const BYTE Marine_View_f_offset;
      extern const BYTE screen_effect_start_offset;
   }
   
   //for internal processes (rad!)
   struct CHEATS
   {
      BYTE Deathless;
      BYTE JetPack;
      BYTE Infinite_Ammo;
      BYTE Bmp_Possession;
      BYTE Super_jump;
      BYTE Reflexive_damage;
      BYTE Medusa;
      BYTE Omnipotent;
      BYTE Controller;
      BYTE Bottomless_Clip;
   };
   
   //for external processes
   namespace CheatsEx
   {
      extern const BYTE Deathless_offset;
      extern const BYTE JetPack_offset;
      extern const BYTE Infinite_Ammo_offset;
      extern const BYTE Bmp_Possession_offset;
      extern const BYTE Super_jmp_offset;
      extern const BYTE Reflexive_damage_offset;
      extern const BYTE Medusa_offset;
      extern const BYTE Omnipotent_offset;
      extern const BYTE Controller_offset;
      extern const BYTE Bottomless_Clip_offset;
   }
   
   union OBJECT_TAG
   {
      int Tag; 
      struct 
      {
         short Index;
         short ID;
      };
   };
   
   struct OBJECT_TABLE_ARRAY
   {
      WORD ObjectID;           // Matches up to Object ID in player table ( for players )
      BYTE Unknown0;//+0x2  
      BYTE ObjectType;//+0x3 //player obj=0 veh=1 wep=2 scenery=6? 
      WORD Unknown1;//+0x4
      WORD Size;//+0x6                 // Structure size
      DWORD Object_ptr;//+0x8                // Pointer to the object data structure
   };
   
   extern const short PlayerNameMaxSize;
   
   extern ADDRESS_SIG Camera_Data_ptr_addr_sig;
      
   struct CAMERA_DATA
   {
      DWORD unknown;       //always 1?
	   float m_fWorld[3];
   };
   
   extern ADDRESS_SIG Local_Player_ptr_addr_sig;
      
   struct LOCAL_PLAYER
   {
      WORD Unknown00;
      WORD Unknown01;//+0x2
      DWORD Unknown02[3];//+0x4
	   WORD ObjectIndex;//+0x10
	   WORD ObjectID;//+0x12
	   BYTE Jumping;//+0x14  //jumping = 2, standing = 0
	   BYTE Unknown03[7];//+0x15
	   float m_fRot[3];//+0x1C
   };
      
   struct STATIC_PLAYER//512bytes 0x402AAFCC
   {
      short PlayerID;           // Stats at 0x70EC
      short IsLocalPlayer;//+0x2               // 0 = local -1 = not
      wchar_t PlayerName0[12];//+0x4           // Unicode / Max - 11 Chars + EOS (12 total)
      short UnknownIndex0;//+0x1C                    // Always -1 / 0xFFFFFFFF
      short UnknownID0;//+0x1E
      DWORD Team;//+0x20               // 0 = Red / 1 = Blue
      long SwapID;//+0x24              // ObjectID
      short SwapType;//+0x28           // 8 = Vehicle / 6 = Weapon
      short SwapSeat;//+0x2A                    // Warthog - Driver = 0 / Passenger = 1 / Gunner = 2 / Weapon = -1
      DWORD RespawnTimer;//+0x2C        // ?????? Counts down when dead, Alive = 0
      DWORD Unknown1;//+0x30           // Always 0
      OBJECT_TAG PlayerObjTag;//+0x34
      OBJECT_TAG PrevPlayerObjTag;//+0x38            // Some sort of ID
      short LocationIndex;//+0x3E          // This is very, very interesting. BG is split into 25 location ID's. 1 -19
      long Unknown2;//+0x40                     // Always -1 / 0xFFFFFFFF
      DWORD BulletCount;//+0x44         // Something to do with bullets increases - weird.
      wchar_t PlayerName1[12];//+0x48           // Unicode / Max - 11 Chars + EOS (12 total)
      OBJECT_TAG UnknownIndex;//0x60         //02 00  FF FF
      char PlayerRespIndex;//+0x64
      char Uknown23;//+0x65
      BYTE Uknown3;//+0x66
      char PlayerChatIndex;//+0x67    same as player index, if changed to another players index, it will use their name
      WORD ActiveCamoTimer;//+0x68            
      WORD Unknown4;//+0x6A
      float SpeedModifier;//+0x6C               // Normal = 1
      DWORD	UnknownIdent3[4];//+0x70
	   long	Unknown5;//+0x80
	   long	LastDeathTime;//+0x84		// since game start(0)
	   BYTE	Unknown6[18];//+0x88
	   short	KillsCount;//+0x9A
	   BYTE	Unknown7[6];//+0xA2
	   short	AssistsCount;//+0xA8
	   BYTE	Unknown8[8];//+0xAA
	   short	BetrayedCount;//+0xB2
	   short	DeathsCount;//+0xB4 actually //+0xB0
	   short	SuicideCount;//+0xB6
	   BYTE	Unknown9[18];
	   short	FlagStealCount;
	   short	FlagReturnCount;
	   short	FlagCaptureCount;
	   BYTE	Unknown10[6];
	   DWORD	UnknownIdent4;
	   BYTE	Unknown11[8];
      short Ping;//+0xDC   
      BYTE	Unknown12[14];
	   DWORD	Unknown13;//+0xDE
	   long	Unknown14;
	   long	SomeTime;
	   float m_World[3];//+0x;
	   DWORD	UnknownIdent6;
	   BYTE	Unknown111[20];
	   BYTE	Melee		:	1;
	   BYTE	Action		:	1;
	   BYTE	UnknownBit	:   1;
	   BYTE	Flashlight	:	1;
	   BYTE	UnknownBit1	:	4;
	   BYTE	UnknownBit2	:	5;
	   BYTE	Reload		:	1;
	   BYTE	UnknownBit3	:	2;
	   BYTE	Unknown15[26];
	   float m_LowerRot[3];//+0x;		// Yaw, Pitch (again, in radians.
	   float	ForwardVelocityMultiplier;
	   float	HorizontalVelocityMultiplier;
	   float	RateOfFireVelocityMultiplier;
	   short	HeldWeaponIndex;
	   short	GrenadeIndex;
	   BYTE	Unknown16[4];
	   float	LookVect[3];
	   BYTE	Unknown17[16];
	   float m_World_delay[3];	// Oddly enough... it matches the world vect, but seems to lag behind (Possibly what the client reports is _its_ world coord?)
	   BYTE	Unknown18[128];
   };
   
   //the STATIC_PLAYER_HEADER is pretty much the same as the OBJECT_TABLE_HEADER
   struct DATA_HEADER
   {
      char TName[32];            // Item name
      short MaxItems;//+0x20      // max number of items
      short ItemSize;//+0x22      // size of each item
      bool Initialized;//+0x24      // setup at the start of the game
      BYTE Unknown;//+0x25          //maybe Initialized is a BOOL?
      short Unknown2;//+0x26        //however halo accesses it as a byte
      char Data[4];//+0x28       // '@t@d' - translated as 'data'
      short NextValidItemIndex;//+0x2C    //something about the items index
      short NumOfItems;//+0x2E    // # of items
      short NextItemIndex;//+0x30 // Index number of the next Item to be created
      short NextItemID;//+0x32    // ID number of the next Item to be created
      DWORD FirstItem;//+0x34// Pointer to the first Item in the array
   };
   
   // [Animation Trigger]
   extern ADDRESS_SIG AnimTriggers_ptr_sig;
   
   //DATA_HEADER pointer sigs
   //extern ADDRESS_SIG Object_ptr_sig; use DestroyObj_func_addr_sig + 2
   extern ADDRESS_SIG Device_Groups_ptr_sig;
   extern ADDRESS_SIG Players_ptr_sig;
   extern ADDRESS_SIG HS_Globals_ptr_sig;
   extern ADDRESS_SIG Object_List_Header_ptr_sig;
   //list object reference ptr is + 4 from object list header ptr
   
   /*struct OBJECT_TABLE_HEADER
   {
      char TName[32];          // 'object'
      WORD MaxObjects;//+0x20  // Maximum number of objects - 0x800(2048 objects)
      WORD Size;//+0x22        // Size of each object array - 0x0C(12 bytes)
      DWORD Unknown0;//+0x24   // always 1?
      char Data[4];//+0x28     // '@t@d' - translates to 'data'?
      WORD Max;                  // Max number of objects the game has reached (slots maybe?)
      WORD Num;                  // Number of objects in the current game
      WORD NextObjectIndex; // Index number of the next object to spawn
      WORD NextObjectID;      // ID number of the next object to spawn
      DWORD FirstObject;          // Pointer to the first object in the table 
   }; 
   
   struct STATIC_PLAYER_HEADER
   {
      char TName[32];         // 'players'
      WORD MaxSlots;//+0x20   // Max number of slots/players possible
      WORD SlotSize;//+0x22   // Size of each Static_Player struct
      DWORD Unknown;//+0x24   // always 1?
      char Data[4];//+0x28    // '@t@d' - translated as 'data'?
      WORD InMainMenu;//+0x2C // 0 = in game 1 = in main menu / not in game
      WORD SlotsTaken;//+0x2E // or # of players
      WORD NextPlayerIndex;//+0x30 // Index # of the next player to join
      WORD NextPlayerID;//+0x32    // ID # of the next player to join
      DWORD Static_Player_ptr;//+0x34  // Pointer to the first static player
   };*/
   
   struct WEAPON_OBJECT//size 0x66C 0x584 different for each one
   {
      BYTE unknown[0x240];
      float battery_used;//+0x240
      BYTE unknown1[0x72];//+0x72
      //rapid fire, NOP bytes (1.08 pc) 004C3379: D9 47 10
      //(1.09 ce) 004C6769
      //float fire rate 0x260 or 0x270?
      short rounds_total;//+0x2B6
      short rounds_loaded;//+0x2B8
      float battery_gauge;//+0x438
   };
   
   struct VEHICLE_OBJECT//
   {
      OBJECT_TAG BipdMetaTag;   // [Biped]
      DWORD Zeros_00;//+0x4
      BYTE BitFlags_00[4];//+0x8
      DWORD Timer_00;//+0xC
      BYTE BitFlags_01[4];//+0x10
      DWORD Timer_01;//+0x14
      BYTE Zeros_01[68];//+0x18
      float m_World[3];//+0x5C
      float m_Velocity[3];//+0x68
      float m_LowerRot[3];//+0x74
      float m_Scale[3];//+0x80
      BYTE unknown[0x40];//+0x8C
      OBJECT_TAG AntrMetaTag;//+0xCC // [Animation Trigger]characters\cyborg\cyborg
      short Animation_Trigger;//+0xD0
      short Flag1;//+0xD2
      short Flag2;//+0xD4
      short Flag3;//+0xD6
      BYTE Unknown1[274];//+0xD8
      short ptr_index1;//+0x1EA
      BYTE Uknown2[2];//+0x1EC
      short ptr_index2;//+0x1EE
      BYTE Unkown3[176];//+0x1F0
      BYTE standflag;//+0x2A0
      BYTE fixedflag;//+0x2A1
      BYTE Unknown4;//+0x2A2
      BYTE Action;//+0x2A3
      BYTE Unknown5[128];//+2A4
      OBJECT_TAG DriverObjTag;//+0x324
   };
   
   struct SPARTAN//size 0x91C - 2332
   {
      OBJECT_TAG BipdMetaTag;   // [Biped]characters\cyborg_mp\cyborg_mp
      DWORD Zeros_00;//+0x4
      BYTE BitFlags_00[4];//+0x8
      DWORD Timer_00;//+0xC
      DWORD BitFlags_01;//+0x10
      DWORD Timer_01;//+0x14
      BYTE Zeros_01[68];//+0x18
      float m_World[3];//+0x5C
      float m_Velocity[3];//+0x68
      float m_LowerRot[3];//+0x74
      float m_Scale[3];//+0x80
      float m_SpinVelocity[3];//+0x8C
      DWORD LocationID;//+0x98
      DWORD Pointer_00;//+0x9C
      float xUnknown;//+0xA0
      float yUnknown;//+0xA4
      float zUnknown;//+0xA8
      float Unknownf0;//+0xAC
      float obj_scale;//+0xB0 object_set_scale 1st param
      float Unknownf1;//+0xB4
      float Unknownf2;//+0xB8
      float Unknownf3;//+0xBC
      OBJECT_TAG PlayerTag;//+0xC0
      DWORD Unknown00;//+0xC4
      BYTE Zeros_04[4];//+0xC8
      OBJECT_TAG AntrMetaTag;//+0xCC // [Animation Trigger]characters\cyborg\cyborg
      short Animation_Trigger;//0xD0
      short BitFlags_02;//+0xD2
      short obj_scale_2nd_param; //+0xD4  object_set_scale 2nd param
      short unknownflag; //+0xD6
      float Max_vitality_param1;//+0xD8
      float Max_vitality_param2;//+0xDC
      float Health;//+0xE0 
      float Shield_00;//+0xE4
      DWORD Zeros_05;//+0xE8
      float Unknown02;//+0xEC
      DWORD Unknown03;//+0xF0
      float Unknown04;//+0xF4
      float Unknown05;//+0xF8 --
      DWORD Unknown06a;//+0xFC
      int DamageFromPlayer;//+0x100
      BYTE Unknown06b[2];//+0x104
      BYTE KillPlayer;//+0x106 write 0x20 = unit_kill, 0x40 unit_kill_silent, 0xC = death, 0x10=overshield
      BYTE Unknown06c[13];//+0x107
      OBJECT_TAG VehicleWeaponTag;//+0x114
      OBJECT_TAG WeaponTag;//+0x118
      OBJECT_TAG VehicleTag;//+0x11C // Ex: Turret on Warthog
      short SeatType;//+0x120
      BYTE BitFlags_03[2];//+0x122
      DWORD Zeros_06;//+0x124
      float Shield_01;//+0x128
      float Flashlight_00;//+0x12C
      float Zeros_07;//+0x130
      float Flashlight_01;//+0x134
      BYTE Unknown10[204];//+0x138
      WORD IsInvisible;//+0x204 // normal = 0x41 invis = 0x51 (bitfield?)
      BYTE Unknown11;//+0x206
      BYTE IsSuspended;//+0x207
      BYTE IsCrouching;//+0x208      // crouch = 1, jump = 2
      BYTE Unknown12[3];//+0x209
      BYTE Unknown13[148];//+0x20C
      BYTE AnimationType;//+0x2A0
      BYTE Unknown135[2];//+0x2A1
      BYTE Action;//+0x2A3 enter vehicle = 0x1A, exit vehicle = 0x1B
      BYTE Unknown14[76];//+0x2A4
      BYTE VehSeatType;//+0x2F0
      BYTE Unknown145[655];//+0x2F1
      float LeftThigh[13];//+0x580
      float RightThigh[13];//+0x5B4
      float Pelvis[13];//+0x5E8
      float LeftCalf[13];//+0x61C
      float RightCalf[13];//+0x650
      float Spine[13];//+0x684
      float LeftClavicle[13];//+0x6B8
      float LeftFoot[13];//+0x6E7
      float Neck[13];//+0x720
      float RightClavicle[13];//+0x754
      float RightFoot[13];//+0x788
      float Head[13];//+0x7BC
      float LeftUpperArm[13];//+0x7F0
      float RightUpperArm[13];//+0x824
      float LeftLowerArm[13];//+0x858
      float RightLowerArm[13];//+0x88C
      float LeftHand[13];//+0x8C0
      float RightHand[13];//+0x8F4
   };
}

namespace Halo_Lib
{
   extern const DWORD Deathless_address;
   extern const DWORD Infinite_Ammo_address;
   extern const DWORD Bottomless_address;
}

namespace HCE_Lib
{
   extern ADDRESS_SIG Dev_addr_sig;
   extern ADDRESS_SIG sv_say_func_addr_sig;
   
   namespace rpg_beta6_2_device_groups
   {
      extern const DWORD alarm_control_1_offset;
      extern const DWORD boom_control_offset;
      extern const DWORD alarm_control_2_offset;
      extern const DWORD alarm_control_3_offset;
      extern const DWORD alarm_control_4_offset;
      extern const DWORD lock_control_offset;
   }
   
   namespace rpg_beta6_2_hs_global
   {
      extern const WORD alarmed_offset;
      extern const WORD locked_offset;
      extern const WORD nuked_offset;
      extern const WORD setting_offset;
      extern const WORD lock_timer_offset;
      extern const WORD boom_timer_offset;
   }
   
   /*
   extern const DWORD Rider_Eject_address;
   extern const DWORD Rasterizer_FPS_address;
   extern const DWORD WireFrame_address;
   extern const DWORD FogAtmosphere_address;
   extern const DWORD FogPlane_address;
   
   extern const DWORD Dev_address;
   
      
   extern const DWORD Console_Check_address;
   extern const DWORD Console_address;

   extern const DWORD Console_Buffer_address;
   extern const DWORD C_Buffer_address1;
   extern const DWORD C_Buffer_address2;
   extern const DWORD C_Buffer_address3;
   extern const DWORD C_Buffer_address4;
   extern const DWORD C_Buffer_address5;
   extern const DWORD C_Buffer_address6;
   extern const DWORD C_Buffer_address7;
   extern const DWORD C_Buffer_address8;
   
   extern const DWORD Current_Map_address;

   extern const DWORD Game_Speed_ptr_address;
   
   extern const DWORD Deathless_address;
   extern const DWORD JetPack_address;
   extern const DWORD Infinite_Ammo_address;
   extern const DWORD Bmp_Possession_address;
   extern const DWORD Super_jmp_address;
   extern const DWORD Reflexive_damage_address;
   extern const DWORD Medusa_address;
   extern const DWORD Omnipotent_addrress;
   extern const DWORD Controller_address;
   extern const DWORD Bottomless_Clip_address;

   extern const DWORD Rcon_Pass_address;
   extern const DWORD Edit_Name_Buffer;
   
   extern const DWORD Local_Player_ptr_address;
   extern const DWORD ObjectTableHeader_ptr_address;
   extern const DWORD StaticPlayerHeader_ptr_address;
   extern const DWORD Server_chat_address;
   
   extern const DWORD Marine_View_f_address;
   extern const DWORD MV_fparameter2_address;
   
   extern const DWORD Cinematic_address;
   extern const DWORD Show_Hud_address;

   extern const DWORD LetterBox_address;
   
   //rpg_beta6_2
   extern const DWORD Alarm_Control_1_address;
   extern const DWORD Boom_Control_address;
   extern const DWORD Alarm_Control_2_address;
   extern const DWORD Alarm_Control_3_address;
   extern const DWORD Alarm_Control_4_address;
   extern const DWORD Lock_Control_address;

   //lockdown positions from object table
   extern const DWORD LD1_pos_address;
   extern const DWORD LD2_pos_address;
   extern const DWORD LD3_pos_address;
   extern const DWORD LD4_pos_address;
   extern const DWORD LD5_pos_address;
   extern const DWORD LD6_pos_address;
   extern const DWORD LD7_pos_address;
   extern const DWORD LD8_pos_address;
   extern const DWORD LD9_pos_address;
   extern const DWORD LD10_pos_address;
   //lockdown power
   extern const DWORD LD1_pow_address;
   extern const DWORD LD2_pow_address;
   extern const DWORD LD3_pow_address;
   extern const DWORD LD4_pow_address;
   extern const DWORD LD5_pow_address;
   extern const DWORD LD6_pow_address;
   extern const DWORD LD7_pow_address;
   extern const DWORD LD8_pow_address;
   extern const DWORD LD9_pow_address;
   
   
   
   extern const DWORD Locked_address;
   extern const DWORD Alarmed_address;
   extern const DWORD Nuked_address;
   extern const DWORD Setting_address;
   extern const DWORD Lock_Timer_address;
   extern const DWORD Boom_Timer_address;
   */
}

//haloceded 1.09
namespace HCED_Lib
{
   /*
   extern const DWORD ObjectTableHeader_ptr_address;
   extern const DWORD StaticPlayerHeader_ptr_address;
   */
}