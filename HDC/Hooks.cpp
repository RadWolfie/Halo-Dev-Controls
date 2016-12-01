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
	File:    Hooks.cpp
	Project: HDC
	Author:  Jesus7Freak
	Date:    11/22/2011
	Game:    Halo and Halo Custom Edition
	Version: all
*********************************************************************************/
#include "dllmain.h"

void HaloDrawText(char *msg_str, BYTE alpha, BYTE red, BYTE green, BYTE blue)
{
   float ARGB[4];
   ARGB[0] = (float)alpha / 255.0f;
   ARGB[1] = (float)red / 255.0f;
   ARGB[2] = (float)green / 255.0f;
   ARGB[3] = (float)blue / 255.0f;
   
   __asm
   {
      LEA EAX,[ARGB]
      PUSH msg_str
      CALL DWORD PTR [EngineDrawText_address]
      ADD ESP,4
   }
}

//halo's limit is about 130 characters on screen
wchar_t HaloSay_server_buffer[SV_NAME_SIZE+SV_BUFFER_SIZE] =
   {'*','*',' ','S','E','R','V','E','R',' ','*','*',' ',' '};
//switchs between console messages and server messages
//based on wether or not console is open and cmd
//came from player 0
void HaloSay(wchar_t *message, short to_player_index, ...)
{
   va_list arglist;
   va_start(arglist, message);
   
   _vswprintf_s_l(&HaloSay_server_buffer[SV_NAME_SIZE], SV_BUFFER_SIZE, message, NULL, arglist);
   //va_end(arglist); its not used in swprintf.c, so why do i need to?
   
   if (*ServerType != HOST || //ServerSay only works for server/hosts
      to_player_index != -1 && players[to_player_index].IsLocalPlayer != -1 && *Console_chck)
   {
      wchar_t *HDT_msgW = &HaloSay_server_buffer[SV_NAME_SIZE];
      char *HDT_msgA = (char*)HDT_msgW;
         
      //convert the HaloSay_buffer to ansi
      do *++HDT_msgA = (char)*++HDT_msgW; while (*HDT_msgW);
      
      //send console message
      HaloDrawText((char*)&HaloSay_server_buffer[SV_NAME_SIZE], 255, 255, 255, 255);
   }
   else
   {
      //haloce already adds the "** SERVER **" part
      //if (running_gt == haloce) message += SV_NAME_SIZE;
         
      CHAT_INFO chat_info;
      if (to_player_index != -1)
      {
         chat_info.ChatType = Vehicle;
         chat_info.From_PlayerIndex = (char)to_player_index;
      }
      else
      {
         chat_info.ChatType = Server;
         // -1 when used with chat type 3 (server) in halo results in ** SERVER **
         chat_info.From_PlayerIndex = 0;//-1;
      }
      
      chat_info.msg_str = HaloSay_server_buffer;
       
      ServerSay(chat_info, (char)to_player_index);
   }
}

void ServerSay(CHAT_INFO chat_info, char to_player)
{
   if (PrepMsg_func_address && SendMsgToPlayer_func_address &&
      ServerInfo_ptr_address && MsgInfo_ptr_address && SendMsgToAll_func_address)
   {
      //__asm ;client msg side
      //   {
      //      MOV EAX,1;//64E7ACh chat type
      //     // MOV EAX,DWORD PTR [EAX]
      //      PUSH player_index
      //      MOV EDX, Local_Buffer_84
      //      MOV ECX, 004ADF70h
      //      CALL ECX
      //      ADD ESP,4
      //   }
      
      __asm ;server msg side
      {
         LEA ECX,chat_info
         PUSH ECX
         MOV ECX,ESP
         
         MOV EDI,DWORD PTR[MsgInfo_ptr_address]
         //push 1
         //pop esi
         //mov esi,dword ptr[esp]
         XOR EDX,EDX
         PUSH EDX
         PUSH 1
         PUSH EDX
         PUSH ECX
         PUSH EDX
         PUSH 0Fh
         PUSH EDX
         
         MOV EDX,7FF8h
         MOV EAX,EDI
         CALL DWORD PTR [PrepMsg_func_address] ;//build network packet
         POP EDX ;//restore EDX as 0
         ADD ESP,1Ch
         
         MOV ECX,DWORD PTR [ServerInfo_ptr_address]
         MOV ECX,DWORD PTR [ECX]
         
         TEST ECX,ECX
         JNZ SHORT msg_ready
         PUSH 9
         CALL DWORD PTR [Sleep]
         TEST ECX,ECX
         JE SHORT sv_chat_exit
         msg_ready:
         //use different function to send to everyone at once
         MOVSX EBX,to_player
         CMP EBX,-1
         JE SHORT send_msg_to_all
         
         PUSH 3
         PUSH 1
         PUSH EDX
         PUSH 1
         PUSH EAX
         PUSH EDI
         PUSH 1
         
         MOV ESI,ECX
         MOVSX EAX,EBX ;//chat to player index
         CALL DWORD PTR [SendMsgToPlayer_func_address]
         ADD ESP,1Ch

         JMP SHORT sv_chat_exit
         
         send_msg_to_all:
         PUSH 3
         PUSH 1
         PUSH EDX
         PUSH 1
         PUSH EDI
         PUSH 1
       
         CALL DWORD PTR [SendMsgToAll_func_address]
         ADD ESP,18h
         
         sv_chat_exit:
      }
   }
}
                     
//functions from mainform,
__declspec(noinline) BOOL __fastcall ckbx_rapid_fire_CheckedChanged(BYTE Checked)
{
   if (Checked != (BYTE)-1)
   {
      if (Checked == TOGGLE)
         Checked = *(BYTE*)rapid_fire_hook_address == 0xD9;

      DWORD dwOldProtect;
      if (VirtualProtect(
         (LPVOID)rapid_fire_hook_address,
         sizeof(DWORD),
         PAGE_EXECUTE_READWRITE,
         &dwOldProtect))
      {
         if (Checked)
            *(DWORD*)rapid_fire_hook_address = 0xD9909090;
         else
            *(DWORD*)rapid_fire_hook_address = 0xD91047D9;
      
         VirtualProtect(
            (LPVOID)rapid_fire_hook_address,
            sizeof(DWORD),
            dwOldProtect,
            &dwOldProtect);
      }
   }
   return *(BYTE*)rapid_fire_hook_address == 0x90;
}

__declspec(noinline) BOOL __fastcall ckbx_time_freeze_CheckedChanged(BYTE Checked)
{
   if (Checked != (BYTE)-1)
   {
      if (Checked == TOGGLE)
         Checked = *(BYTE*)time_freeze_hook_address == 0x74;

      DWORD dwOldProtect;
      if (VirtualProtect(
         (LPVOID)time_freeze_hook_address,
         sizeof(WORD),
         PAGE_EXECUTE_READWRITE,
         &dwOldProtect))
      {
         if (Checked)
            *(WORD*)time_freeze_hook_address = 0x3975;
         else
            *(WORD*)time_freeze_hook_address = 0x2A74;
      
         VirtualProtect(
            (LPVOID)time_freeze_hook_address,
            sizeof(WORD),
            dwOldProtect,
            &dwOldProtect);
      }
   }
   return *(BYTE*)time_freeze_hook_address == 0x75;
}

__declspec(noinline) BOOL __fastcall ckbx_grav_boots_CheckedChanged(BYTE Checked)
{
   DWORD number_address = grav_boots_hook_address - 7;
   DWORD jmp_to_address = grav_boots_hook_address - 0xD;
   
   if (Checked != (BYTE)-1)
   {
      if (Checked == TOGGLE)
         Checked = *(DWORD*)number_address != 0x00000242;

         
      //on find this -> 0x00560E70: eax,[eax+000002F4]
      //mov [eax+000002F4],00000242
      //jmp 00560E76
      
      //off
      //mov [eax+000002F4],00000202
      //jmp 00560E76

      DWORD dwOldProtect;
      if (VirtualProtect((LPVOID)jmp_to_address, 19, PAGE_EXECUTE_READWRITE, &dwOldProtect))
      {
         if (*(BYTE*)(grav_boots_hook_address - 3) != 0xEB)
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
            
            BYTE *jmp_to_code_cave = (BYTE*)grav_boots_hook_address;
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
            
         VirtualProtect((LPVOID)jmp_to_address, 19, dwOldProtect, &dwOldProtect);
      }
   }
   return *(DWORD*)number_address == 0x00000242;
}

__declspec(noinline) BOOL __fastcall ckbx_vehicle_ntr_CheckedChanged(BYTE Checked)
{
   if (Checked != (BYTE)-1)
   {
      if (Checked == TOGGLE)
         Checked = *(BYTE*)vehicle_ntr_hook_address == 0x74;


      DWORD dwOldProtect;
      if (VirtualProtect(
         (LPVOID)vehicle_ntr_hook_address,
         sizeof(BYTE),
         PAGE_EXECUTE_READWRITE,
         &dwOldProtect))
      {
         if (Checked)
            *(BYTE*)vehicle_ntr_hook_address = 0xEB;
         else
            *(BYTE*)vehicle_ntr_hook_address = 0x74;
         
         VirtualProtect(
            (LPVOID)vehicle_ntr_hook_address,
            sizeof(BYTE),
            dwOldProtect,
            &dwOldProtect);
      }
   }
   return *(BYTE*)vehicle_ntr_hook_address == 0xEB;
}

__declspec(noinline) bool __stdcall MV_chkBx_CheckedChanged(int number)
{
   bool succeded = false;
   
   float fnum;
   if (number == 2) fnum = 0;
   else fnum = 2;
   
   __asm
   {
      MOV ECX,number
      MOV EDX,DWORD PTR [cinematic_ptr]
      MOV EDX,DWORD PTR [EDX]
      TEST EDX,EDX
      JE SHORT cin_failed
      TEST ECX,ECX
      JE SHORT cin_stop
      CMP BYTE PTR [EDX+38h],0
      JNZ SHORT skip_cin_start
      
      MOV ECX,0Eh
      XOR EAX,EAX
      MOV EDI,EDX
      REP STOS DWORD PTR [EDI]
      MOV BYTE PTR DS:[EDX+39h],1
      MOV BYTE PTR [EDX+38h],1               
      
      skip_cin_start:
      MOV EAX,fnum
      PUSH EAX
      PUSH 2
      CALL DWORD PTR [cse_set_video_func_address]
      ADD ESP,8
      JMP SHORT cin_succeded
      
      cin_stop:
      MOV BYTE PTR [EDX+38h],0
      
      cin_succeded:
      MOV succeded,1
      
      cin_failed:
   }
   return succeded;
}

void __declspec(naked) ConsoleHook()
{ //save ebx,edi,ebp
   __asm//EDI has *console_str, EDX has console_str_length, but not for rcon
   {
      //if (*console_str == '/')
      CMP BYTE PTR [EDI],2Fh
      JNE SHORT jmp_to_console
      
      INC EDI
      
      MOV EBX,80h
      SUB ESP,EBX ;//space to convert str to unicocde
      
      XOR EAX,EAX
      SHR EBX,1
      
      console_bufferw_loop:
      MOVSX CX,BYTE PTR [EDI+EAX]
      MOV WORD PTR [ESP+EAX*2],CX
      INC EAX
      ;//i < 40h * sizeof(whar_t)
      CMP EAX,EBX
      JGE SHORT console_bufferqw_limit_reached
      ;//console_str[i]
      TEST CX,CX
      JNZ SHORT console_bufferw_loop
      console_bufferqw_limit_reached:
      DEC EDI
      SHL EBX,1
      
      MOV EDX,EBP ;//EBP contains player index
      MOV ECX,ESP
      //EDX
      //ECX
      CALL CMDParser
      
      ADD ESP,EBX
      
      TEST EAX,EAX
      JNZ SHORT RememberConsoleCmd
      
      jmp_to_console:
      JMP DWORD PTR [Console_func_address]

      RememberConsoleCmd:
      //haloce1.09 console@004C9F92 to 004CA00C
      MOV ESI,[C_BuffersIndex_address]
      MOVSX EAX,WORD PTR DS:[ESI]
      INC EAX
      AND EAX,0x80000007
      JNS SHORT jmp_unused_code
      
      DEC EAX
      OR EAX,0xFFFFFFF8
      INC EAX
      jmp_unused_code:
      
      MOV WORD PTR [ESI],AX
      MOVSX EAX,AX
      IMUL EAX,EAX,0xFF
      MOV EDX,EDI
      
      ADD EAX,DWORD PTR [C_Buffers_address]
      LEA ESP,DWORD PTR [ESP]
      XOR EBX,EBX
      
      copy_str_loop:
      MOV CL,BYTE PTR [EDX]
      INC EDX
      MOV BYTE PTR [EAX],CL
      INC EAX
      CMP CL,BL ;//BL needs to be 0
      JNZ SHORT copy_str_loop
      
      MOVSX EAX,WORD PTR [ESI-02]
      INC EAX
      CMP EAX,8
      MOV WORD PTR [ESI-02],8
      JG SHORT jmp_c_limit_mov
      MOV WORD PTR [ESI-02],AX
      jmp_c_limit_mov:
      
      MOV WORD PTR [ESI+02],0xFFFF
      MOV  AL,1
      RETN
   }
}

void __declspec(naked) ChatHook()
{
   __asm
   {
      //WORD PTR [EAX+0Ch] = player index
      //DWORD PTR [ESP+14h] = "Message"
      //DWORD PTR [ESP+0Ch] = chat type (0=global 1=team 2=vehicle 3=server)
      
      MOV ECX,DWORD PTR [ESP+14h]
      MOV EDX,DWORD PTR [ESP+0Ch]
      
      ;// test for cmd /
      CMP WORD PTR [ECX],2Fh
      JNE SHORT msg_cmb_func
      
      ;//one less character to test
      INC ECX
      INC ECX
      
      ;// global only cmds
      TEST EDX,EDX ;//Global
      JNZ SHORT msg_cmb_func
      
      //player index
      MOVSX EDX,WORD PTR [EAX+0Ch]
      
      PUSH EAX;//save EAX for the jmp to func
      
      //edx
      //ecx
      CALL CMDParser
      
      MOV ECX,EAX
      POP EAX
      
      ;//cmds have the option to silent the chat cmd message
      TEST CH,CH ;//DO_NOT_SEND_MSG
      JNZ SHORT ch_ret
      
      msg_cmb_func:
      //returned address already pushed so instead of calling it, we'll jmp to it;
      JMP DWORD PTR [PlayerCheck_Func_address]
      
      ch_ret:
      RETN
   }
}

bool __cdecl hstr_cmp(char *str1, char *str2, int length)
{
   bool match = true;
   if (length >= 0)
   {
      //specific length comparison
      for (int i = 0; i < length; i++)
      {
         if (str1[i] != str2[i])
         {
            match = false;
            break;
         }
      }
   }
   return match;
}
    
void __declspec(naked) ConsoleTablistHook()
{
   //available registers:
   //EAX misc
   //ECX console str
   //EDX index so far | cmd str
   //EBX console str size
   //EBP array pointer 
   //ESI array index ptr
   //EDI cmd str index
   __asm
   {
      MOV ECX,DWORD PTR [ctl_filter_str_ptr_address]
      MOV ECX,DWORD PTR [ECX] ;//should be console str
      CMP BYTE PTR [ECX],2Fh ;//all the custom cmds have '/'
      JNE SHORT exit_ctl_hook
      
      //custom cmds shouldn't go over 256 (array limit)
      MOV ESI,DWORD PTR [ctl_array_size_address]
      MOVSX ESI, WORD PTR [ESI]
      CMP DX,SI ;//array size >= index?
      JGE SHORT exit_ctl_hook
      
      MOV ESI,DWORD PTR [ctl_array_index_address]
      
      //copy and get length of console str
      MOV EBX,ECX
      LEA EDI,DWORD PTR [ECX+1]
      loop_str_length:
      MOV AL,BYTE PTR [EBX]
      INC EBX
      TEST AL,AL
      JNZ SHORT loop_str_length
      SUB EBX,EDI
      
      XOR EDI,EDI
      PUSH EDI ;//finished looping when EPS points to 0
      
      PUSH PLAYER_CMDS_SIZE*4
      PUSH OFFSET CMDsLib::player_cmd_strs
      
      PUSH RPGB_CMDS_SIZE*4
      PUSH OFFSET CMDsLib::rpg_beta6_2_cmd_strs
      
      PUSH HALO_CMDS_SIZE*4
      PUSH OFFSET CMDsLib::halo_cmd_strs
      
      cmd_strs_loop:
      MOV EDX,DWORD PTR [ESP]
      MOV EDX,DWORD PTR [EDX+EDI]
      PUSH EBX//length
      PUSH ECX//str
      PUSH EDX//str
      CALL hstr_cmp//__cdecl filter cmds
      POP EDX;//restore EDX
      POP ECX;//restore ECX
      POP EBX ;//faster than ADD ESP,4
      TEST AL,AL
      JE SHORT no_match
      
      MOVSX EAX,WORD PTR [ESI] ;//array index
      MOV DWORD PTR [EBP+EAX*4],EDX ;//copy to array
      INC EAX
      MOV WORD PTR [ESI],AX ;//store index
      
      no_match:
      ADD EDI,4
      CMP EDI,DWORD PTR [ESP+4]
      JL SHORT cmd_strs_loop
      
      XOR EDI,EDI ;//reset counter
      POP EAX ;//ADD ESP,8
      POP EAX
      CMP DWORD PTR [ESP],EDI
      JNZ SHORT cmd_strs_loop
      POP EAX ;//ADD ESP,4
      
      exit_ctl_hook:
      JMP DWORD PTR [console_tablist_hookfunc_address]
   }
}
