;notes by Jesus7Freak
;haloce1.09 console=help sv_map
;console 004C9BE3
...1 ;inspect
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F5DD8   haloce.0048C8D0
...2 ;help
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F9714   haloce.004854A0
...3 ;help
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F9714   haloce.004854A0
...4 ;inspect
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F5DD8   haloce.0048C8D0



...2
0048D1E0   51               PUSH ECX ;403D8901
0048D1E1   57               PUSH EDI ;F36F0002
0048D1E2   50               PUSH EAX ;0000003A
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F9714   haloce.004854A0
->
004854A0   0FBF4424 04      MOVSX EAX,WORD PTR SS:[ESP+4]
004854A5   8B0485 18416200  MOV EAX,DWORD PTR DS:[EAX*4+624118] ;->EAX=005F9714
004854AC   8B4C24 0C        MOV ECX,DWORD PTR SS:[ESP+C] ;->ECX=403D8901
004854B0   56               PUSH ESI
004854B1   8B7424 0C        MOV ESI,DWORD PTR SS:[ESP+C] ;->ESI=F36F0002
004854B5   51               PUSH ECX
004854B6   8D50 1C          LEA EDX,DWORD PTR DS:[EAX+1C] ;->EDX=005F9730
004854B9   0FBF40 1A        MOVSX EAX,WORD PTR DS:[EAX+1A] ;->EAX=00000001

004854BD   52               PUSH EDX
004854BE   50               PUSH EAX
004854BF   56               PUSH ESI
004854C0   E8 DB800000      CALL haloce.0048D5A0
004854C5   83C4 10          ADD ESP,10

004854C8   85C0             TEST EAX,EAX ;EAX=00000000
004854CA   74 11            JE SHORT haloce.004854DD
;004854CC   8B10             MOV EDX,DWORD PTR DS:[EAX]
;004854CE   E8 2D1A0000      CALL haloce.00486F00
;004854D3   8BCE             MOV ECX,ESI
;004854D5   33C0             XOR EAX,EAX
;004854D7   5E               POP ESI
;004854D8   E9 B37E0000      JMP haloce.0048D390
004854DD   5E               POP ESI
004854DE   C3               RETN
<-


...3
0048D1E0   51               PUSH ECX ;403D8900
0048D1E1   57               PUSH EDI ;F36F0002
0048D1E2   50               PUSH EAX ;0000003A ;index to console functions structs
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F9714   haloce.004854A0
->
004854A0   0FBF4424 04      MOVSX EAX,WORD PTR SS:[ESP+4] ;->EAX=0000003A
004854A5   8B0485 18416200  MOV EAX,DWORD PTR DS:[EAX*4+624118] ;->EAX=005F9714
004854AC   8B4C24 0C        MOV ECX,DWORD PTR SS:[ESP+C] ;->ECX=403D8900
004854B0   56               PUSH ESI
004854B1   8B7424 0C        MOV ESI,DWORD PTR SS:[ESP+C] ;->ESI=F36F0002
004854B5   51               PUSH ECX
004854B6   8D50 1C          LEA EDX,DWORD PTR DS:[EAX+1C] ;->EDX=005F9730
004854B9   0FBF40 1A        MOVSX EAX,WORD PTR DS:[EAX+1A] ;->EAX=00000001

004854BD   52               PUSH EDX
004854BE   50               PUSH EAX
004854BF   56               PUSH ESI
004854C0   E8 DB800000      CALL haloce.0048D5A0
004854C5   83C4 10          ADD ESP,10

004854C8   85C0             TEST EAX,EAX ;EAX=403D897A ptr to console function data
004854CA   74 11            JE SHORT haloce.004854DD
;help function
004854CC   8B10             MOV EDX,DWORD PTR DS:[EAX] ;->EDX=404C2AFB "sv_map"
004854CE   E8 2D1A0000      CALL haloce.00486F00

004854D3   8BCE             MOV ECX,ESI
004854D5   33C0             XOR EAX,EAX
004854D7   5E               POP ESI
004854D8   E9 B37E0000      JMP haloce.0048D390
...^
;004854DD   5E               POP ESI
;004854DE   C3               RETN








;help function
004854CC   8B10             MOV EDX,DWORD PTR DS:[EAX] ;->EDX=404C2AFB "sv_map"
004854CE   E8 2D1A0000      CALL haloce.00486F00
->
 00486F00   81EC 00080000    SUB ESP,800
 00486F06   56               PUSH ESI
 00486F07   E8 84F3FFFF      CALL haloce.00486290 ;find dev fucntion info struct index
 00486F0C   8BF0             MOV ESI,EAX ;->ESI=000001EC
 00486F0E   66:83FE FF       CMP SI,0FFFF
 00486F12   74 73            JE SHORT haloce.00486F87
 00486F14   8D4C24 04        LEA ECX,DWORD PTR SS:[ESP+4]
 00486F18   E8 33010000      CALL haloce.00487050
 00486F1D   8D4424 04        LEA EAX,DWORD PTR SS:[ESP+4]
 00486F21   50               PUSH EAX
 00486F22   33C0             XOR EAX,EAX
 00486F24   E8 572D0100      CALL haloce.00499C80
 00486F29   0FBFCE           MOVSX ECX,SI
 00486F2C   8B148D 18416200  MOV EDX,DWORD PTR DS:[ECX*4+624118]
 00486F33   8B4A 10          MOV ECX,DWORD PTR DS:[EDX+10]
 00486F36   83C4 04          ADD ESP,4
 00486F39   8D5424 04        LEA EDX,DWORD PTR SS:[ESP+4]
 00486F3D   8D49 00          LEA ECX,DWORD PTR DS:[ECX]
 00486F40   8A01             /MOV AL,BYTE PTR DS:[ECX]
 00486F42   41               |INC ECX
 00486F43   8802             |MOV BYTE PTR DS:[EDX],AL
 00486F45   42               |INC EDX
 00486F46   84C0             |TEST AL,AL
 00486F48  ^75 F6            \JNZ SHORT haloce.00486F40
 00486F4A   8D4424 04        LEA EAX,DWORD PTR SS:[ESP+4]
 00486F4E   6A 0A            PUSH 0A
 00486F50   50               PUSH EAX
 00486F51   E8 AA3B1400      CALL haloce.005CAB00
 00486F56   8BF0             MOV ESI,EAX
 00486F58   83C4 08          ADD ESP,8
 00486F5B   85F6             TEST ESI,ESI
 00486F5D   74 41            JE SHORT haloce.00486FA0
 00486F5F   57               PUSH EDI
 00486F60   8D7C24 08        LEA EDI,DWORD PTR SS:[ESP+8]
 00486F64   33C0             /XOR EAX,EAX
 00486F66   85F6             |TEST ESI,ESI
 00486F68   57               |PUSH EDI
 00486F69   74 24            |JE SHORT haloce.00486F8F
 00486F6B   8806             |MOV BYTE PTR DS:[ESI],AL
 00486F6D   E8 0E2D0100      |CALL haloce.00499C80
 00486F72   8D7E 01          |LEA EDI,DWORD PTR DS:[ESI+1]
 00486F75   6A 0A            |PUSH 0A
 00486F77   57               |PUSH EDI
 00486F78   E8 833B1400      |CALL haloce.005CAB00
 00486F7D   83C4 0C          |ADD ESP,0C
 00486F80   85FF             |TEST EDI,EDI
 00486F82   8BF0             |MOV ESI,EAX
 00486F84  ^75 DE            \JNZ SHORT haloce.00486F64
 00486F86   5F               POP EDI
 00486F87   5E               POP ESI
 00486F88   81C4 00080000    ADD ESP,800
 00486F8E   C3               RETN
 00486F8F   E8 EC2C0100      CALL haloce.00499C80
 00486F94   83C4 04          ADD ESP,4
 00486F97   5F               POP EDI
 00486F98   5E               POP ESI
 00486F99   81C4 00080000    ADD ESP,800
 00486F9F   C3               RETN
 00486FA0   8D4C24 04        LEA ECX,DWORD PTR SS:[ESP+4]
 00486FA4   51               PUSH ECX
 00486FA5   33C0             XOR EAX,EAX
 00486FA7   E8 D42C0100      CALL haloce.00499C80
 00486FAC   83C4 04          ADD ESP,4
 00486FAF   5E               POP ESI
 00486FB0   81C4 00080000    ADD ESP,800
 00486FB6   C3               RETN
<-