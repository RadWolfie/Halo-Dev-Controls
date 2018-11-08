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
	File:    HaloLib.cpp
	Project: Halo Dev Controls and HDC
	Author:  Jesus7Freak, RadWolfie
	Date:    11/30/2016
	Game:    Halo and Halo Custom Edition
	Version: all
*********************************************************************************/
#include "HaloLib.h"

/// <summary>
/// contains info that are the same in both games
/// </summary>
namespace HaloCE_lib
{
   ADDRESS_SIG Current_map_addr_sig =
   //{ "\x7F\x1F\xB8\x00\x00\x00\x00\x8D\x50", "x?x????xx", 3 }; //Does not work for 1.10 version
   { "\xB8\x64\x30\x64\x00\x8D\x50\x01\x8A\x08\x40\x84\xC9\x75\xF9\x2B\xC2\x83\xF8\x1F", "x????x??xxxxxx?x?xxx", 1 };

   ADDRESS_SIG Rider_Eject_addr_sig =
   { "\x04\xA0\xAC\x49\x62\x00\x84\xC0\x0F\x84\x00\x00\x00\x00\x8B\x4D\xEC", "xx????xxxx????xxx", 2 };

   ADDRESS_SIG Console_addr_sig =
   { "\x51\xA0\x70\x1F\x65\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x55\x8B", "xx????xxxx????xx", 2 };

   ADDRESS_SIG Base_ptr_addr_sig = //ce1.08 = 0x006BD194 pc1.09 = 0x00721ea4
   { "\x8B\x0D\x00\x00\x00\x00\x50\x51\xC7\x05", "xx????xxxx", 2 };

   ADDRESS_SIG Cheats_addr_sig =
   { "\x74\x09\x80\x3D\x00\x00\x00\x00\x00", "xxxx????x", 4 };

   ADDRESS_SIG Show_Hud_ptr_addr_sig =
   { "\x8B\x0D\x00\x00\x00\x00\x85\xC9\x0F\x84\x00\x00\x00\x00\x80", "xx????xxxx????x", 2 };

   //offset ptr by 8
   ADDRESS_SIG letterbox_ptr_addr_sig =
   { "\x0F\x84\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x8A\x48", "xx????x????xx", 7 };

   ADDRESS_SIG Rasterizer_addr_sig =
   { "\xA0\xC0\x49\x62\x00\x83\xEC\x0C\x84\xC0\x75\x0E", "x????xxxxxxx", 1 };

   ADDRESS_SIG Cinematic_addr_sig =
   { "\x01\x8B\x15\x00\x00\x00\x00\x3B\xD3", "xxx????xx", 3 };

   //ADDRESS_SIG ServerType_ptr_addr_sig =
   //{ "\x66\x83\x3D\xD0\x47\x6B\x00\x02\x89\x44\x24\x38", "xxx????xxxxx", 3 };

   //offset ptr by 0x18
   ADDRESS_SIG Game_Speed_ptr_addr_sig =
   { "\x8B\xFD\xF3\xAB\xA1\x00\x00\x00\x00\x8B", "xxxxx????x", 5 };

   ADDRESS_SIG Grav_Boots_addr_sig =
   { "\x83\xEC\x38\x56\x8B\xF1\xB1\x04", "xxxxxxxx", -6 };

   ADDRESS_SIG Time_Freeze_addr_sig =
   { "\xC1\xE0\x10\x0B\xC2\x84\xC9", "xxxxxxx", 7 };

   ADDRESS_SIG Rapid_Fire_addr_sig =
   { "\xD9\x81\x34\x02\x00\x00\xEB\x03", "xxxxxxxx", 8 };

   ADDRESS_SIG Vehicle_NTR_addr_sig =// change from 0x74 to 0xEB
   { "\x66\x8B\x8E\xB8\x00\x00\x00\x66\x8B\x90\xB8\x00\x00\x00", "xxxxxxxxxxxxxx", 0x15 };

      /*Local Chat func
      004AE755  |. 50             PUSH EAX
      004AE756  |. 68 7CA46000    PUSH haloce.0060A47C ;  UNICODE "SERVER"
      004AE75B  |. 68 60A46000    PUSH haloce.0060A460 ;  UNICODE "** %s **  %s"
      004AE760  |. 8D9424 2401000>LEA EDX,DWORD PTR SS:[ESP+124]
      004AE767  |. E8 94CC0A00    CALL haloce.0055B400 <-- conjugate strings
      004AE76C  |. 8D8C24 2401000>LEA ECX,DWORD PTR SS:[ESP+124]

      004AE773  |. 51             PUSH ECX
      004AE774  |. E8 57030000    CALL haloce.004AEAD0
      004AE779  |. 83C4 10        ADD ESP,10 ;2 func's combined; should be 4
      */
   ADDRESS_SIG Local_Chat_func_addr_sig =
   { "\xA1\x00\x00\x00\x00\x83\xEC\x08\x83\xF8\x08", "x????xxxxxx", 0 };

   /*
   **warning** objects created in main thread here does not sync
   004AE687   8D9424 18050000  LEA EDX,DWORD PTR SS:[ESP+518]
   004AE68E   52               PUSH EDX
   004AE68F   8D8424 1C010000  LEA EAX,DWORD PTR SS:[ESP+11C]           ; load chat into EAX
   004AE696   50               PUSH EAX
   004AE697   E8 14C81100      CALL haloce.005CAEB0                     ; hook here
   004AE69C   8D8C24 20010000  LEA ECX,DWORD PTR SS:[ESP+120]
   004AE6A3   51               PUSH ECX
   004AE6A4   E8 27040000      CALL haloce.004AEAD0
   004AE6A9   83C4 0C          ADD ESP,0C
   */
   //MOVSX ECX,BYTE PTR [ESP+1Ch] to get player index for hooking func
   ADDRESS_SIG Local_Chat_hook_addr_sig =
   { "\x50\xE8\x00\x00\x00\x00\x8D\x8C\x24\x00\x00\x00\x00\x51\xE8", "xx????xxx????xx", 2 };

   /*004AE06A   E8 B1210400      CALL haloce.004F0220
   004AE06F   84C0             TEST AL,AL
   004AE071   0F84 12010000    JE haloce.004AE189
   004AE077   8B8424 2C020000  MOV EAX,DWORD PTR SS:[ESP+22C]
   004AE07E   E8 FD630300      CALL haloce.004E4480            ;hook here
   004AE083   83F8 FF          CMP EAX,-1
   004AE086   0F84 FD000000    JE haloce.004AE189*/
   //add 1 more to the offset for dedicated server
   //add 0x2E to offset for MsgInfo_ptr_address
   //add 0x37 to offset for PrepMsg_func_address  build network packet function
   //add 0x3D to offset for ServerInfo_ptr_address
   ADDRESS_SIG Server_Chat_hook_addr_sig =
   { "\x8B\x08\x8B\x11\x81\xEC\x00\x00\x00\x00\x57", "xxxxxx????x", 0x3F };

   /*004AE158   83F8 FF          CMP EAX,-1 ;EAX =player index
   004AE15B   74 1A            JE SHORT haloce.004AE177
   004AE15D   6A 03            PUSH 3
   004AE15F   6A 01            PUSH 1
   004AE161   6A 00            PUSH 0
   004AE163   6A 01            PUSH 1
   004AE165   55               PUSH EBP ;ret val from PrepMsg func
   004AE166   68 20CE8000      PUSH haloce.0080CE20  ;MsgInfo_ptr_address
   004AE16B   6A 01            PUSH 1
   004AE16D   8BF3             MOV ESI,EBX ;EBX= [6B7384] ServerInfo_ptr_address
   004AE16F   E8 6C6F0300      CALL haloce.004E50E0
   004AE174   83C4 1C          ADD ESP,1C*/

   ADDRESS_SIG SendChatMsgToPlayer_func_addr_sig =
   { "\x51\x53\x57\x8B\xF8\x32\xC0\x33\xC9", "xxxxxxxxx", 0 };

   /*004AE218   6A 03            PUSH 3
   004AE21A   6A 01            PUSH 1
   004AE21C   57               PUSH EDI ;0
   004AE21D   6A 01            PUSH 1
   004AE21F   68 20CE8000      PUSH haloce.0080CE20 ;MsgInfo_ptr_address
   004AE224   6A 01            PUSH 1
   004AE226   8BC5             MOV EAX,EBP ;ret val from PrepMsg func
   004AE228   E8 03700300      CALL haloce.004E5230
   004AE22D   83C4 18          ADD ESP,18*/
   //use (address + 4) - *address to get actually address
   ADDRESS_SIG SendChatMsgToAll_func_addr_sig =
   { "\x8B\xC5\xE8\x00\x00\x00\x00\x83\xC4\x18\x5D\x5B", "xxx????xxxxx", 3 };

      /*console
      004C9BDD   53               PUSH EBX                  //0
      004C9BDE   BF 28206500      MOV EDI,haloce.00652028   //string
      004C9BE3   E8 58030000      CALL haloce.004C9F40                     ;hook here
      004C9BE8   83C4 04          ADD ESP,4

      returns bool (BYTE) succeded
      */
   //used with the console buffer
   ADDRESS_SIG Console_hook_addr_sig =
   { "\x53\xBF\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x04", "xx????x????xxx", 7 };

   ADDRESS_SIG Console_func_addr_sig =
   { "\x8A\x07\x81\xEC\x00\x05\x00\x00\x3C\x3B", "xxxxxxxxxx", 0 };

   /*rcon call to console
   004C9F20   6A 00            PUSH 0
   004C9F22   A3 6CDE6400      MOV DWORD PTR DS:[64DE6C],EAX
   004C9F27   E8 14000000      CALL haloce.004C9F40
   004C9F2C   83C4 04          ADD ESP,4*/
   ADDRESS_SIG Rcon_hook_addr_sig =
   { "\x6A\x00\xA3\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x04", "xxx????x????xxx", 8 };

      /*engine draw text
      004E7291   A1 D8076200      MOV EAX,DWORD PTR DS:[6207D8]   ;ARGB color (float array)
      004E7296   68 E0D06000      PUSH haloce.0060D0E0         ; ASCII "sv_kick:  Can't kick a local client!"
      004E729B   E8 E029FBFF      CALL haloce.00499C80
      004E72A0   83C4 04          ADD ESP,4
      */
   //use (address + 4) - *address to get actually address
   ADDRESS_SIG EngineDrawText_func_addr_sig =
   { "\x83\xEC\x10\x57\x8B\xF8\xA0", "xxxxxxx", 0 };

   /*0047ED8C   8B00             MOV EAX,DWORD PTR DS:[EAX] ;Player Obj Tag
   0047ED8E   E8 9D050F00      CALL haloce.0056F330
   returns 01 if succeded and > 01 if not*/
   ADDRESS_SIG UnitExitVehicle_func_addr_sig =
   { "\x55\x8B\xEC\x83\xE4\xF8\x81\xEC\x00\x00\x00\x00\x53\x56\x8B\xF0\x83\xC9", "xxxxxxxx????xxxxxx", 0 };

   //no paramters
   ADDRESS_SIG SpawnHog_func_addr_sig =
   { "\xA1\x00\x00\x00\x00\x8B\x88\x64\x01\x00\x00\x85\xC9\x57", "x????xxxxxxxxx", 0 };


   /*[40848BF0+i*10]= vehicle biped tag ptr
   [40848BE4+0C]->E3D40260 (1st one?)
   [40848B44+24]->40848BE4
   [40848198+168]->40848B44
   [6E2280]->40848198

    0045B0D9   50               PUSH EAX;00000010 (how many objects to spawn)
    0045B0DA   51               PUSH ECX;408489F4 (ptr to BipdMetaTag)
    0045B0DB   E8 A0020000      CALL haloce.0045B380
    0045B0E0   83C4 08          ADD ESP,8
    returns how many objects spawned*/
   ADDRESS_SIG SpawnObjAtPlayer0_func_addr_sig =
   { "\x81\xEC\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xF8\xFF\x0F\x84", "xx????x????xxxxx", 0};

   /*0047D411   8BC6             MOV EAX,ESI ;obj tag
   0047D413   E8 48C40700      CALL haloce.004F9860*/
   //add 2 bytes to get address of ptr to object data header
   ADDRESS_SIG DestroyObj_func_addr_sig =
   { "\x8B\x0D\x00\x00\x00\x00\x8B\x51\x34\x57", "xx????xxxx", 0 };

   /*0048592C   8B00             MOV EAX,DWORD PTR DS:[EAX] ;EAX char pointer to player index, 1 based
   0048592E   E8 FD180600      CALL haloce.004E7230
   returns bool succeded*/
   //add 3 bytes to get address of ptr to ServerType
   ADDRESS_SIG sv_kick_func_addr_sig =
   { "\x66\x83\x3D\xD0\x47\x6B\x00\x02\x75\x6C\xE8\x61\x06\x00\x00\x85\xC0", "xxx????xx?x????xx", 0 };

   /*0048597E   8B4C24 04        MOV ECX,DWORD PTR SS:[ESP+4] ;ECX is struct ptr to ban time "dhms"?
   00485982   8B4424 08        MOV EAX,DWORD PTR SS:[ESP+8] ;EAX is player index (1 based)
   00485986   E8 35190600      CALL haloce.004E72C0
   returns bool succeded*/
   //add 3 bytes to get address of ptr to ServerType
   ADDRESS_SIG sv_ban_func_addr_sig =
   { "\x66\x83\x3D\xD0\x47\x6B\x00\x02\x0F\x85\x00\x00\x00\x00\x85\xC0", "xxx????xxx????xx", 0 };

   /*cinematic_screen_effect_set_video <short>2 <real>2
   004840DC   8B48 04          MOV ECX,DWORD PTR DS:[EAX+4] ;pc1.09 004814BC
   004840DF   33D2             XOR EDX,EDX
   004840E1   66:8B10          MOV DX,WORD PTR DS:[EAX]
   004840E4   51               PUSH ECX   ;40000000 (2.0)
   004840E5   52               PUSH EDX   ;0002
   004840E6   E8 751E0900      CALL haloce.00515F60
   004840EB   83C4 08          ADD ESP,8*/
   //add 2 bytes to get cinematic ptr
   ADDRESS_SIG cse_set_video_func_addr_sig =
   { "\x8B\x15\x00\x00\x00\x00\x56\x33\xF6\x3B\xD6", "xx????xxxxx", 0 };

   /*00486A43   68 20634800      PUSH haloce.00486320
   00486A48   6A 04            PUSH 4
   00486A4A   52               PUSH EDX
   00486A4B   55               PUSH EBP;0018D840 array of strings
   00486A4C   E8 0F1E1400      CALL haloce.005C8860;misc dev funtions?(huge function)(hook here)
   00486A51   66:A1 00C46400   MOV AX,WORD PTR DS:[64C400];index to the array of strings
   00486A57   83C4 10          ADD ESP,10
   00486A5A   5F               POP EDI
   00486A5B   5E               POP ESI
   00486A5C   5D               POP EBP
   00486A5D   C705 04C46400 00000000 MOV DWORD PTR DS:[64C404],0;array of strings pointer*/
   //add 12 bytes to get index to the array of strings, add 24 bytes to get the array of strings on the stack
   //array size address is -16 bytes from the array index, were the pointer to the array is +4 bytes from the index
   ADDRESS_SIG console_tablist_hook_addr_sig =
   { "\x6A\x04\x52\x55\xE8\x0F\x1E\x14\x00\x66\xA1", "xxxxx????xx", 5 };

   /*004869EC   66:8935 00C46400 MOV WORD PTR DS:[64C400],SI;array of strings index
   004869F3   66:890D F0C36400 MOV WORD PTR DS:[64C3F0],CX;array size?
   004869FA   892D 04C46400    MOV DWORD PTR DS:[64C404],EBP;pointer to the array
   00486A00   A3 F4C36400      MOV DWORD PTR DS:[64C3F4],EAX;pointer to the filter string (ex: obj, search for functions that have obj in them)
   00486A05   75 0A            JNZ SHORT haloce.00486A11
   ;00486A07   C705 F4C36400 3C365F00    MOV DWORD PTR DS:[64C3F4],haloce.005F363C
   00486A11   66:8915 FCC36400 MOV WORD PTR DS:[64C3FC],DX
   00486A18   BF 60496200      MOV EDI,haloce.00624960 ;pointer to array of functions*/
   ADDRESS_SIG console_tablist_vars_hook_addr_sig =
   { "\x57\x66\x89\x35\x00\xC4\x64\x00\x66\x89\x0D", "xxxx????xxx", 4 };

   //[[this address] + 204] = size of name list
   //[[this address] + 208] = ptr to name list
   //ADDRESS_SIG map_info_struct_ptr_addr_sig =
   //{ "\xEB\x14\x8B\x0D\x00\x00\x00\x00\x6B\xD2\x5C", "xxxx????xxx", 4 };

   /*0048AAC8   8BCE            MOV ECX,ESI ;fast call, ecx = scripted obj index
   0048AACA   5E              POP ESI
   0048AACB   E9 30050700     JMP haloce.004FB000*/
   //add 1 to get map_info_struct_ptr address
   ADDRESS_SIG CreateScriptedObj_func_addr_sig =
   { "\xA1\x00\x00\x00\x00\x8B\x90\x08\x02\x00\x00", "x????xxxxxx", 0 };

   //[this address] = ptr to scripted obj tags     DWORD obj_tags[size]
   ADDRESS_SIG scripted_objtags_ptr_addr_sig =
   { "\x7D\x09\x8B\x35\x00\x00\x00\x00\x89\x0C\x86", "xxxx????xxx", 4 };

   /*0045B4FB   33C0             XOR EAX,EAX
   0045B4FD   50               PUSH EAX ;unknown
   0045B4FE   8D5424 44        LEA EDX,DWORD PTR SS:[ESP+44]
   0045B502   52               PUSH EDX ;ptr to new obj data, size 88h, see cheat_spawn_hog.asm notes
   0045B503   E8 38DC0900      CALL haloce.004F9140
   0045B508   66:8B8424 D8000000 MOV AX,WORD PTR SS:[ESP+D8]
   0045B510   83C4 08          ADD ESP,8
    returns new obj tag*/
   ADDRESS_SIG CreateObject_func_addr_sig =
   { "\x81\xEC\x1C\x02\x00\x00\x8B\x0D\x00\x00\x00\x00\x53\x55", "xxxxxxxx????xx", 0 };

   /*0047C7C4   8B6C24 14        MOV EBP,DWORD PTR SS:[ESP+14]
   0047C7C8   8B5C24 14        MOV EBX,DWORD PTR SS:[ESP+14] ;static player tag
   0047C7CC   50               PUSH EAX ;durration = 30 * secs
   0047C7CD   55               PUSH EBP ;unknown 0 for camo or 1
   0047C7CE   E8 6D010000      CALL haloce.0047C940
   0047C7D3   83C4 08          ADD ESP,8*/
   ADDRESS_SIG ActiveCamo_func_addr_sig =
   { "\x83\xFB\xFF\x55\x8B\x6C\x24\x08\x56\x57", "xxxxxxxxxx", 0 };

   /*004646F1   8BD8             MOV EBX,EAX ;static player tag
   004646F3   A1 44CD6800      MOV EAX,DWORD PTR DS:[68CD44]
   004646F8   85C0             TEST EAX,EAX
   004646FA   74 24            JE SHORT haloce.00464720
   004646FC   6A 00            PUSH 0 ;respawn time modifier
   004646FE   E8 7D490100      CALL haloce.00479080
   00464703   A1 44CD6800      MOV EAX,DWORD PTR DS:[68CD44]
   00464708   8B80 98000000    MOV EAX,DWORD PTR DS:[EAX+98]
   0046470E   83C4 04          ADD ESP,4*/
   ADDRESS_SIG PlayerDeath_func_addr_sig =
   { "\x51\x83\xFB\xFF\x55\x8B\x6C\x24\x0C", "xxxxxxxxx", 0 };

   namespace Console
   {
      const BYTE Check_offset = 0;
      const BYTE Enabled_offset = 1;
      const BYTE Text_offset = 0xB8;
      const WORD C_Buffers_offset = 0x1C4;
      const WORD C_Buffers_index_offset = 0x9BE;
   }

   namespace RasterizerEx
   {
      const BYTE FPS_offset = 0;
      const BYTE WireFrame_offset = 6;
      const BYTE FogAtmosphere_offset = 0x27;
      const BYTE FogPlane_offset = 0x28;
   }

   namespace Cinematic
   {
      const BYTE Marine_View_f_offset = 0x23;
      const BYTE screen_effect_start_offset = 0x38;
   }

   //for external processes
   namespace CheatsEx
   {
      const BYTE Deathless_offset = 0;
      const BYTE JetPack_offset = 1;
      const BYTE Infinite_Ammo_offset = 2;
      const BYTE Bmp_Possession_offset = 3;
      const BYTE Super_jmp_offset = 4;
      const BYTE Reflexive_damage_offset = 5;
      const BYTE Medusa_offset = 6;
      const BYTE Omnipotent_offset = 7;
      const BYTE Controller_offset = 8;
      const BYTE Bottomless_Clip_offset = 9;
   }

   const short PlayerNameMaxSize = 12;

   // [Animation Trigger]
   ADDRESS_SIG AnimTriggers_ptr_sig =
   { "\x8B\x0A\x8B\x15\x00\x00\x00\x00\x81\xE1", "xxxx????xx", 4 };

   ADDRESS_SIG Camera_Data_ptr_addr_sig =
   { "\x8D\x9B\x00\x00\x00\x00\xB9\x1A\x00\x00\x00", "xx????xxxxx", 2 };

   ADDRESS_SIG Local_Player_ptr_addr_sig =
   { "\x8B\x0D\x00\x00\x00\x00\x8D\x54\x24\x04", "xx????xxxx", 2 };

   //use DestroyObj_func_addr_sig + 2
   //ADDRESS_SIG Object_ptr_sig =
   //{ "\x8B\x45\x18\x8B\x0D\x00\x00\x00\x00\x8B\x51\x34", "xxxxx????xxx", 5 };

   ADDRESS_SIG Device_Groups_ptr_sig = //ce 815DB0 pc 87A870
   { "\x66\x83\xF9\xFF\xA1\x00\x00\x00\x00\x74", "xxxxx????x", 5 };

   ADDRESS_SIG Players_ptr_sig =
   { "\xBA\x04\x8B\x15\x00\x00\x00\x00\x25", "xxxx????x", 4 };

   ADDRESS_SIG HS_Globals_ptr_sig =
   { "\x83\xC4\x10\x85\xC9\xA3\x00\x00\x00\x00\x5B", "xxxxxx????x", 6 };

   ADDRESS_SIG Object_List_Header_ptr_sig =
   { "\xA1\x00\x00\x00\x00\x8B\x70\x34\x8B\xD3", "x????xxxxx", 1 };
}

/// <summary>
/// Halo Combat Evolved specific info, v1.09
/// </summary>
namespace Halo_Lib
{
   /*
   const DWORD Deathless_address = 0x87A840;//byte
   const DWORD Infinite_Ammo_address = 0x87A842;//byte
   const DWORD Bottomless_address = 0x87A849;//byte

   const DWORD ObjectTableHeader_ptr_address = 0x00860030;
   const DWORD StaticPlayerHeader_ptr_address = 0x0087A100;
   */
}

/// <summary>
/// Halo Custom Edition specific info, v1.09
//401000 to 5DF000
/// </summary>
namespace HCE_Lib
{
   ADDRESS_SIG Dev_addr_sig =
   { "\x8A\x0D\x00\x00\x00\x00\x80\xE2\x07\x84\xC9", "xx????xxxxx", 2 };

   /*sv_say
   00485B0C   8B08             MOV ECX,DWORD PTR DS:[EAX]
   00485B0E   E8 DD0B0600      CALL haloce.004E66F0
   */
   ADDRESS_SIG sv_say_func_addr_sig =
   { "\x51\x66\x83\x3D\xD0\x47\x6B\x00\x02\x75\x2F", "xxxx????xxx", 0 };

   namespace rpg_beta6_2_hs_global
   {
      const WORD alarmed_offset = 0xF8C;
      const WORD locked_offset = 0xF84;
      const WORD nuked_offset = 0xF94;
      const WORD setting_offset = 0xF9C;
      const WORD lock_timer_offset = 0xFAC;
      const WORD boom_timer_offset = 0xFB4;
   }

   namespace rpg_beta6_2_device_groups
   {
      const DWORD alarm_control_1_offset = 0x55C;
      const DWORD boom_control_offset = 0x56C;
      const DWORD alarm_control_2_offset = 0x57C;
      const DWORD alarm_control_3_offset = 0x58C;
      const DWORD alarm_control_4_offset = 0x59C;
      const DWORD lock_control_offset = 0x5AC;
   }

   /*
   const DWORD Rider_Eject_address = 0x006249AC;//byte
   const DWORD Rasterizer_FPS_address = 0x006249C0;//byte
   const DWORD WireFrame_address = 0x006249C6;//byte
   const DWORD FogAtmosphere_address = 0x006249E7;//byte
   const DWORD FogPlane_address = 0x006249E8;//byte

   const DWORD Dev_address = 0x006BD17E;//byte


   //halo ce 1.09, 0xE8 offset fro, 1.08
   const DWORD Console_Check_address = 0x00651F70;//byte
   const DWORD Console_address = 0x00651F71;//byte

   00651FF8 console color->alpha
   00651FFC console color->red
   00652000 console color->green
   00652004 console color->blue
   0x00652008 console title "halo( "
   const DWORD Console_Buffer_address = 0x00652028;//string, size = 61 os 0xB8

   //console can remember your last 8 commands//strings, size = 0xFF
   //char c_buffers[8][0xFF]
   const DWORD C_Buffer_address1 = 0x00652134;
   const DWORD C_Buffer_address2 = 0x00652233;
   const DWORD C_Buffer_address3 = 0x00652332;
   const DWORD C_Buffer_address4 = 0x00652431;
   const DWORD C_Buffer_address5 = 0x00652530;
   const DWORD C_Buffer_address6 = 0x0065262F;
   const DWORD C_Buffer_address7 = 0x0065272E;
   const DWORD C_Buffer_address8 = 0x0065282D;

   const DWORD Current_Map_address = 0x00643084;//string

   //active camo offsets
   //0x34, eax*4+08, 0x204

   //game speed 0x40000300
   //0x400002E8 + 0x18
   //004856AE - 8B 15 90CD6800             - mov edx,[0068CD90] : [400002E8]
   //004856B4 - 89 4A 18                   - mov [edx+18],ecx
   const DWORD Game_Speed_ptr_address = 0x0068CD90;//float

   const DWORD Deathless_address = 0x00815D80;//byte  64AC0
   const DWORD JetPack_address = 0x00815D81;//byte
   const DWORD Infinite_Ammo_address = 0x00815D82;//byte
   const DWORD Bmp_Possession_address = 0x00815D83;//byte
   const DWORD Super_jmp_address = 0x00815D84;//byte
   const DWORD Reflexive_damage_address = 0x00815D85;//byte
   const DWORD Medusa_address = 0x00815D86;//byte
   const DWORD Omnipotent_addrress = 0x00815D87;//byte
   const DWORD Controller_address = 0x00815D88;//byte
   const DWORD Bottomless_Clip_address = 0x00815D89;//byte
   //0049753F
   //006B44C8

   //00714E7C //00712542
   const DWORD Rcon_Pass_address = 0x006B74C0;//string
   const DWORD Edit_Name_Buffer = 0x006AFF2A;//11 character unicode string

   const DWORD Local_Player_ptr_address = 0x0064C3AC;

   @t@d header ptrs:

   terminal output 0064DE50 (aka console)

   pc texture
   pc texture cache
   pc sound
   pc sound cache
   sounds
   looping sounds
   update client queues
   terminal output
   weather particles
   const DWORD Update_Server_Queues_ptr_address = 0x0068CDB0

   cached object render states
   widget
   flag
   antenna
   glow
   glow particles
   light volumes
   lightnings
   const DWORD Device_Groups_ptr_address = 0x00815DB0;
   lights
   cluster light reference
   light cluster reference 007FBB28
   const DWORD Object_ptr_address = 0x007FB3B0; //400506B4
   cluster collideable object refe
   collideable object cluster refe
   cluster noncollideable object r
   noncollideable object r
   decals
   decal vertex cache
   const DWORD Players_ptr_address = 0x008155C0;// points to 0x402AAF94
   teams
   contrail
   contrail point
   particle
   effect
   effect locations
   particle systems
   particle system particles
   object looping sounds
   actor
   swarm
   swarm component
   const DWORD Prop_ptr_address = 0x0081B4A0;
   const DWORD Encounter_ptr_address = 0x0081B4A8;
   const DWORD AI_Pursuit_ptr_address = 0x0081B4B0;
   const DWORD AI_Conversation_ptr_address = 0x0081B4B4;
   const DWORD Object_List_Header_ptr_address = 0x008155A4;
   const DWORD List_Object_Reference_ptr_address = 0x008155A8;
   const DWORD HS_Thread_ptr_address = 0x008155B0;
   const DWORD HS_Globals_ptr_address = 0x008155AC;
   const DWORD Recorded_Animations_ptr_address = 0x0064B960;
   const DWORD Script_Node_ptr_address = 0x008155B4;

   const DWORD Server_chat_address = 0x404D2239;

   //marine view
   const DWORD Marine_View_f_address = 0x4000017F;//byte
   const DWORD MV_fparameter2_address = 0x40000188;//float
   //005DC930 - 0FB6 06                    - movzx eax,byte ptr [esi]
   //005DC933 - 50                         - push eax
   //005DC934 - 53                         - push ebx
   //005DC935 - E8 D5D0FEFF                - call 005C9A0F

   //pointer 006B827C + 0x38
   const DWORD Cinematic_address = 0x40000194;//byte
   const DWORD Show_Hud_address = 0x400003BC;//byte

   const DWORD LetterBox_address = 0x403FD678;//byte

   //rpg_beta6_2
   const DWORD Alarm_Control_1_address = 0x40027164;//float
   const DWORD Boom_Control_address = 0x40027174;//float
   const DWORD Alarm_Control_2_address = 0x40027184;//float
   const DWORD Alarm_Control_3_address = 0x40027194;//float
   const DWORD Alarm_Control_4_address = 0x400271A4;//float
   const DWORD Lock_Control_address = 0x400271B4;//float

   //lockdown positions from object table
   const DWORD LD1_pos_address = 0x400967B0;//float
   const DWORD LD2_pos_address = 0x40091A10;//float
   const DWORD LD3_pos_address = 0x40096AF4;//float
   const DWORD LD4_pos_address = 0x40090880;//float
   const DWORD LD5_pos_address = 0x4008F6BC;//float
   const DWORD LD6_pos_address = 0x4008F998;//float
   const DWORD LD7_pos_address = 0x4008FC74;//float
   const DWORD LD8_pos_address = 0x4008FF50;//float
   const DWORD LD9_pos_address = 0x40090260;//float
   const DWORD LD10_pos_address = 0x40090570;//float
   //lockdown power
   const DWORD LD1_pow_address = 0x4002712C;//float
   const DWORD LD2_pow_address = 0x40026F6C;//float
   const DWORD LD3_pow_address = 0x4002713C;//float
   const DWORD LD4_pow_address = 0x40026F0C;//float
   const DWORD LD5_pow_address = 0x40026EAC;//float
   const DWORD LD6_pow_address = 0x40026EBC;//float
   const DWORD LD7_pow_address = 0x4008FC68;//float
   const DWORD LD8_pow_address = 0x4008FF44;//float
   const DWORD LD9_pow_address = 0x40090254;//float

   const DWORD Locked_address = 0x403FACBC;//bool
   const DWORD Alarmed_address = 0x403FACC4;//bool
   const DWORD Nuked_address = 0x403FACCC;//bool
   const DWORD Setting_address = 0x403FACD4;//short
   const DWORD Lock_Timer_address = 0x403FACE4;//short
   const DWORD Boom_Timer_address = 0x403FACEC;//short
   */
}

//haloceded 1.09
namespace HCED_Lib
{
   /*
   const DWORD ObjectTableHeader_ptr_address = 0x006C69F0;
   const DWORD StaticPlayerHeader_ptr_address = 0x006E1480;
   //cheats 0x006E1C40;
   */
}