;notes by Jesus7Freak
;haloce.1.09 player death
;unit_kill write 0x20 to player biped obj.0x106;

;biped obj.0x106 , 0x20 =unit_kill value
004F16E3 - 66 89 86 06010000          - mov [esi+00000106],ax;ax=0024
004F180B - 66 09 9E 06010000  - or [esi+00000106],bx;bx=0008 ->=0x002C
004F1286 - 66 81 A6 06010000 9FDF - and word ptr [esi+00000106],DF9F ;->=0x000C


;team change (kills player, then switches them to different team)
00473836   8B4C24 10        MOV ECX,DWORD PTR SS:[ESP+10]
0047383A   8B4424 20        MOV EAX,DWORD PTR SS:[ESP+20];static player tag
0047383E   51               PUSH ECX
0047383F   E8 AC0EFFFF      CALL haloce.004646F0
->
004646F0   53               PUSH EBX
004646F1   8BD8             MOV EBX,EAX
004646F3   A1 44CD6800      MOV EAX,DWORD PTR DS:[68CD44]
004646F8   85C0             TEST EAX,EAX
004646FA   74 24            JE SHORT haloce.00464720
004646FC   6A 00            PUSH 0 ;respawn time modifer
004646FE   E8 7D490100      CALL haloce.00479080
->;player death
00479080   51               PUSH ECX
00479081   83FB FF          CMP EBX,-1;static player tag:EC700000
00479084   55               PUSH EBP
00479085   8B6C24 0C        MOV EBP,DWORD PTR SS:[ESP+C];->EBP=00000000
00479089   56               PUSH ESI
0047908A   57               PUSH EDI
0047908B   0F84 87000000    JE haloce.00479118
00479091   A1 C0558100      MOV EAX,DWORD PTR DS:[8155C0] ;"players"
00479096   8B48 34          MOV ECX,DWORD PTR DS:[EAX+34]
00479099   8BF3             MOV ESI,EBX
0047909B   81E6 FFFF0000    AND ESI,0FFFF
004790A1   C1E6 09          SHL ESI,9
004790A4   8B7C0E 34        MOV EDI,DWORD PTR DS:[ESI+ECX+34];static player.player biped obj tag->EDI=E2E5006C
004790A8   03F1             ADD ESI,ECX;static player->ESI=402AAFCC
004790AA   83FF FF          CMP EDI,-1
004790AD   74 69            JE SHORT haloce.00479118
004790AF   A1 44CD6800      MOV EAX,DWORD PTR DS:[68CD44];->EAX=006232E0
004790B4   85C0             TEST EAX,EAX
004790B6   74 0F            JE SHORT haloce.004790C7
004790B8   8B15 505B8100    MOV EDX,DWORD PTR DS:[815B50];->EDX=00000000
004790BE   85D2             TEST EDX,EDX
004790C0   0F94C0           SETE AL;->AL=01
004790C3   84C0             TEST AL,AL
004790C5   74 12            JE SHORT haloce.004790D9
;player death
004790C7   6A 00            PUSH 0
004790C9   6A FF            PUSH -1
004790CB   6A FF            PUSH -1
004790CD   6A FF            PUSH -1
004790CF   8BC7             MOV EAX,EDI;player biped obj tag E2E5006C
004790D1   E8 EA9BFFFF      CALL haloce.00472CC0
004790D6   83C4 10          ADD ESP,10
004790D9   85ED             TEST EBP,EBP;EBP=00000000
004790DB   74 03            JE SHORT haloce.004790E0
;004790DD   896E 2C          MOV DWORD PTR DS:[ESI+2C],EBP ;static player respawn timer
004790E0   8B15 B0B37F00    MOV EDX,DWORD PTR DS:[7FB3B0];"object"
004790E6   8BC7             MOV EAX,EDI
004790E8   25 FFFF0000      AND EAX,0FFFF
004790ED   8D0C40           LEA ECX,DWORD PTR DS:[EAX+EAX*2]
004790F0   8B42 34          MOV EAX,DWORD PTR DS:[EDX+34]
004790F3   8B4C88 08        MOV ECX,DWORD PTR DS:[EAX+ECX*4+8];player biped obj->ECX=400792CC
004790F7   8BC7             MOV EAX,EDI
004790F9   C781 E0000000 00000000   MOV DWORD PTR DS:[ECX+E0],0 ;sets biped obj.Health to 0
00479103   E8 A8850700      CALL haloce.004F16B0
00479108   8BC3             MOV EAX,EBX
0047910A   E8 112B0F00      CALL haloce.0056BC20
0047910F   53               PUSH EBX
00479110   E8 3BEBFFFF      CALL haloce.00477C50
00479115   83C4 04          ADD ESP,4
00479118   5F               POP EDI
00479119   5E               POP ESI
0047911A   5D               POP EBP
0047911B   59               POP ECX
0047911C   C3               RETN
<-
00464703   A1 44CD6800      MOV EAX,DWORD PTR DS:[68CD44]
00464708   8B80 98000000    MOV EAX,DWORD PTR DS:[EAX+98]
0046470E   83C4 04          ADD ESP,4
00464711   85C0             TEST EAX,EAX
00464713   74 0B            JE SHORT haloce.00464720
00464715   8B4C24 08        MOV ECX,DWORD PTR SS:[ESP+8]
00464719   51               PUSH ECX
0046471A   53               PUSH EBX
0046471B   FFD0             CALL EAX
0046471D   83C4 08          ADD ESP,8
00464720   5B               POP EBX
00464721   C3               RETN
<-
00473844   83C4 04          ADD ESP,4






