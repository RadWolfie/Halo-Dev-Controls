;notes by Jesus7Freak
;haloce.1.09 unit_suspend (unit (list_get (players) 0)) true
;console 004C9BE3

;biped.0x204 (biped properties)
0047709D - 8B 96 04020000  - mov edx,[esi+00000204]
00566154 - 8B 83 04020000  - mov eax,[ebx+00000204]
005662B6 - F7 83 04020000 00000008 - test [ebx+00000204],8000000
0056513F - 8B 87 04020000  - mov eax,[edi+00000204]
00567076 - 8B 83 04020000  - mov eax,[ebx+00000204]
0056709C - 8B 83 04020000  - mov eax,[ebx+00000204]
005671C8 - 8B 83 04020000  - mov eax,[ebx+00000204]
0055F7B0 - 8B 87 04020000  - mov eax,[edi+00000204]
0056170F - F7 85 04020000 00000001 - test [ebp+00000204],1000000
00562032 - 8B 86 04020000  - mov eax,[esi+00000204]
00561CDD - 8B 87 04020000  - mov eax,[edi+00000204]
0056084E - 8D 7C 24 28  - lea edi,[esp+28]
004B3BA9 - F7 81 04020000 00000800 - test [ecx+00000204],80000

0056E01A - 89 B1 04020000  - mov [ecx+00000204],esi

...1 ;inspect
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F5DD8   haloce.0048C8D0
...2 ;unit_suspended
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F6B6C   haloce.0047F4A0
...3 ;unit
0048D1E3   FF52 0C       CALL DWORD PTR DS:[EDX+C] ;EDX=005F5DF4  ; haloce.0048C9D0
...4 ;list_get
0048D1E3   FF52 0C       CALL DWORD PTR DS:[EDX+C] ;EDX=005F60A0  ; haloce.0047D6C0
...5 ;players
0048D1E3   FF52 0C       CALL DWORD PTR DS:[EDX+C] ;EDX=005F5EA4  ; haloce.0047D1D0
...6 ;list_get
0048D1E3   FF52 0C       CALL DWORD PTR DS:[EDX+C] ;EDX=005F60A0  ; haloce.0047D6C0
...7 ;list_get
0048D1E3   FF52 0C       CALL DWORD PTR DS:[EDX+C] ;EDX=005F60A0  ; haloce.0047D6C0
...8 ;unit
0048D1E3   FF52 0C       CALL DWORD PTR DS:[EDX+C] ;EDX=005F5DF4  ; haloce.0048C9D0
...9 ;unit_suspended
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F6B6C   haloce.0047F4A0
...10 ;unit_suspended
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F6B6C   haloce.0047F4A0
...11 ;inspect
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F5DD8   haloce.0048C8D0


...10 ;unit_suspended
0048D1D5   8B148D 18416200  MOV EDX,DWORD PTR DS:[ECX*4+624118] ;->EDX=005F6B6C
0048D1DC   8B4C24 14        MOV ECX,DWORD PTR SS:[ESP+14] ;->ECX=403D8500
0048D1E0   51               PUSH ECX
0048D1E1   57               PUSH EDI ;F3690000
0048D1E2   50               PUSH EAX ;dev function index 00000083
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F6B6C   haloce.0047F4A0
->
0047F4A0   0FBF4424 04      MOVSX EAX,WORD PTR SS:[ESP+4] ;->EAX=00000083
0047F4A5   8B0485 18416200  MOV EAX,DWORD PTR DS:[EAX*4+624118] ;dev function struct->EAX=005F6B6C
0047F4AC   8B4C24 0C        MOV ECX,DWORD PTR SS:[ESP+C] ;->ECX=403D8500
0047F4B0   56               PUSH ESI
0047F4B1   8B7424 0C        MOV ESI,DWORD PTR SS:[ESP+C] ;->ESI=F3690000
0047F4B5   51               PUSH ECX
0047F4B6   8D50 1C          LEA EDX,DWORD PTR DS:[EAX+1C] ;->EDX=005F6B88
0047F4B9   0FBF40 1A        MOVSX EAX,WORD PTR DS:[EAX+1A] ;->EAX=00000002
0047F4BD   52               PUSH EDX
0047F4BE   50               PUSH EAX
0047F4BF   56               PUSH ESI
0047F4C0   E8 DBE00000      CALL haloce.0048D5A0
0047F4C5   83C4 10          ADD ESP,10
0047F4C8   85C0             TEST EAX,EAX ;ptr to unit_suspended params EAX=403D854A
0047F4CA   74 1A            JE SHORT haloce.0047F4E6
0047F4CC   33C9             XOR ECX,ECX
0047F4CE   8A48 04          MOV CL,BYTE PTR DS:[EAX+4] ;true or false ->CL=01
0047F4D1   8B00             MOV EAX,DWORD PTR DS:[EAX] ;player biped obj tag->EAX=E2CD005E
0047F4D3   51               PUSH ECX
0047F4D4   E8 F7EA0E00      CALL haloce.0056DFD0
->
 0056DFD0   83F8 FF          CMP EAX,-1 ;valid tag check
 0056DFD3   74 71            JE SHORT haloce.0056E046
 0056DFD5   8B15 B0B37F00    MOV EDX,DWORD PTR DS:[7FB3B0] ;"object"
 0056DFDB   8B4A 34          MOV ECX,DWORD PTR DS:[EDX+34] ;ptr to first obj->ECX=400506EC
 0056DFDE   25 FFFF0000      AND EAX,0FFFF
 0056DFE3   53               PUSH EBX
 0056DFE4   8A5C24 08        MOV BL,BYTE PTR SS:[ESP+8] ;enable or disable suspend attribute->BL=01
 0056DFE8   8D0440           LEA EAX,DWORD PTR DS:[EAX+EAX*2]
 0056DFEB   C1E0 02          SHL EAX,2
 0056DFEE   84DB             TEST BL,BL
 0056DFF0   8B4C01 08        MOV ECX,DWORD PTR DS:[ECX+EAX+8] ;player biped obj->ECX=4006B45C
 0056DFF4   56               PUSH ESI
 0056DFF5   8BB1 04020000    MOV ESI,DWORD PTR DS:[ECX+204] ;->ESI=00000041
 0056DFFB   57               PUSH EDI
 0056DFFC   74 08            JE SHORT haloce.0056E006
 0056DFFE   81CE 00000001    OR ESI,1000000 ;enable suspend obj attribute->ESI=01000041
 0056E004   EB 06            JMP SHORT haloce.0056E00C
 ;0056E006   81E6 FFFFFFFE    AND ESI,FEFFFFFF ;disable suspend obj attribute
 0056E00C   66:83B9 B4000000 00   CMP WORD PTR DS:[ECX+B4],0 ;biped obj.0xB4=0000
 0056E014   8B3D 041D6300    MOV EDI,DWORD PTR DS:[631D04] ; ->EDI=haloce.005FA830
 0056E01A   89B1 04020000    MOV DWORD PTR DS:[ECX+204],ESI ;writes val to biped obj
 0056E020   8B1F             MOV EBX,DWORD PTR DS:[EDI] ;->EBX=00000000
 0056E022   8D71 68          LEA ESI,DWORD PTR DS:[ECX+68] ;biped obj.m_Velocity[3]->ESI=4006B4C4
 0056E025   891E             MOV DWORD PTR DS:[ESI],EBX ; ->x vect=0.0
 0056E027   8B5F 04          MOV EBX,DWORD PTR DS:[EDI+4] ;->EBX=00000000
 0056E02A   895E 04          MOV DWORD PTR DS:[ESI+4],EBX ;->y vect=0.0
 0056E02D   8B7F 08          MOV EDI,DWORD PTR DS:[EDI+8] ;->EDI=00000000
 0056E030   897E 08          MOV DWORD PTR DS:[ESI+8],EDI ;->z vect=0.0
 0056E033   5F               POP EDI
 0056E034   5E               POP ESI
 0056E035   5B               POP EBX
 0056E036   75 0E            JNZ SHORT haloce.0056E046
 0056E038   8B52 34          MOV EDX,DWORD PTR DS:[EDX+34] ;ptr to first obj->EDX=400506EC
 0056E03B   8B4402 08        MOV EAX,DWORD PTR DS:[EDX+EAX+8] ;player biped obj->EAX=4006B45C
 0056E03F   83A0 CC040000 FE AND DWORD PTR DS:[EAX+4CC],FFFFFFFE ;biped obj.0x4CC DS:[4006B928]=00000000->=00000000
 0056E046   C3               RETN
<-
0047F4D9   83C4 04          ADD ESP,4
0047F4DC   8BCE             MOV ECX,ESI
0047F4DE   33C0             XOR EAX,EAX
0047F4E0   5E               POP ESI
0047F4E1   E9 AADE0000      JMP haloce.0048D390
;0047F4E6   5E               POP ESI
;0047F4E7   C3               RETN












;possible syncs
005671C2   8983 04020000             MOV DWORD PTR DS:[EBX+204],EAX
0056C24C   8996 04020000             MOV DWORD PTR DS:[ESI+204],EDX
0056C8AD   898E 04020000             MOV DWORD PTR DS:[ESI+204],ECX

