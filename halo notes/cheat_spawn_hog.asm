;notes by Jesus7Freak
;haloce1.09 p0 bloodgulch
;console 004C9BE3
...1 ;inspect
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F5DD8   haloce.0048C8D0
...2 ;cheat_spawn_warthog
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F6DC0   haloce.0047FC70
...3 ;inspect
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F5DD8   haloce.0048C8D0

CMP BYTE PTR DS:[EDI+1],2F
JNZ SHORT 004C9F63

...2
0048D1E0   51               PUSH ECX;ECX=403D8501
0048D1E1   57               PUSH EDI;EDI=F3690000
0048D1E2   50               PUSH EAX;EAX=00000097
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F6DC0   haloce.0047FC70
->

;spawn warthog next to player func halopc1.09=0045A730
;\xA1\x00\x00\x00\x00\x8B\x88\x64\x01\x00\x00\x85\xC9\x57
;x????xxxxxxxxx
0047FC70   E8 CBB4FDFF      CALL haloce.0045B140
->
0045B140   A1 80226E00      MOV EAX,DWORD PTR DS:[6E2280];->EAX=40848198
0045B145   8B88 64010000    MOV ECX,DWORD PTR DS:[EAX+164];->ECX=00000001
0045B14B   85C9             TEST ECX,ECX
0045B14D   57               PUSH EDI;EDI=F3690000
0045B14E   8BB8 68010000    MOV EDI,DWORD PTR DS:[EAX+168];->EDI=40848B44 "paew?OF@"
0045B154   74 49            JE SHORT haloce.0045B19F
0045B156   8B47 20          MOV EAX,DWORD PTR DS:[EDI+20];->EAX=00000006
0045B159   55               PUSH EBP;EBP=00000000
0045B15A   8B6F 24          MOV EBP,DWORD PTR DS:[EDI+24];->EBP=40848BE4
0045B15D   56               PUSH ESI;->ESI=403D8500
0045B15E   33F6             XOR ESI,ESI;->ESI=00000000
0045B160   85C0             TEST EAX,EAX;EAX=00000006
0045B162   7E 39            JLE SHORT haloce.0045B19D
0045B164   53               PUSH EBX;EBX=00000000
0045B165   8D5D 04          LEA EBX,DWORD PTR SS:[EBP+4];->EBX=40848BE8
0045B168   8B03             /MOV EAX,DWORD PTR DS:[EBX];->EAX=40459A9D "vehicles\warthog\mp_warthog"

;object check?
0045B16A   68 D8EB5F00      |PUSH haloce.005FEBD8                    ; ASCII "warthog"
0045B16F   50               |PUSH EAX
0045B170   E8 DBF51600      |CALL haloce.005CA750
->
<-;->ECX=005FEBDC "hog"
0045B175   83C4 08          |ADD ESP,8
0045B178   85C0             |TEST EAX,EAX;EAX=40459AA6 "warthog\mp_warthog"
0045B17A   75 10            |JNZ SHORT haloce.0045B18C
...

;spawn next to player 0
0045B18C   C1E6 04          SHL ESI,4;->ESI=00000000
0045B18F   6A 01            PUSH 1;->0018CEB0=00000001
0045B191   03F5             ADD ESI,EBP;->ESI=40848BE4
0045B193   56               PUSH ESI;->0018CEAC=40848BE4
0045B194   E8 E7010000      CALL haloce.0045B380
->
 0045B380   81EC B8000000    SUB ESP,0B8
 0045B386   E8 95FFFFFF      CALL haloce.0045B320
 ->
  0045B320   83EC 10          SUB ESP,10
  0045B323   A1 C0558100      MOV EAX,DWORD PTR DS:[8155C0];players
  0045B328   56               PUSH ESI
  0045B329   894424 04        MOV DWORD PTR SS:[ESP+4],EAX;->0018CDDC=402AAF94  ASCII "players"
  0045B32D   57               PUSH EDI
  0045B32E   35 72657469      XOR EAX,69746572;->EAX=295ECAE6
  0045B333   83CE FF          OR ESI,FFFFFFFF;->ESI=FFFFFFFF
  0045B336   8D7C24 08        LEA EDI,DWORD PTR SS:[ESP+8];->EDI=0018CDDC
  0045B33A   66:C74424 0C 0000 MOV WORD PTR SS:[ESP+C],0;hard coded player 0
  0045B341   897424 10        MOV DWORD PTR SS:[ESP+10],ESI;ESI=FFFFFFFF
  0045B345   894424 14        MOV DWORD PTR SS:[ESP+14],EAX;EAX=295ECAE6
  0045B349   E8 C2870700      CALL haloce.004D3B10
  ->
   004D3B10   66:8B4F 04             MOV CX,WORD PTR DS:[EDI+4];CX=0000
   004D3B14   53                     PUSH EBX;save 40848BE8
   004D3B15   55                     PUSH EBP;save 40848BE4
   004D3B16   56                     PUSH ESI;save FFFFFFFF
   004D3B17   8B37                   MOV ESI,DWORD PTR DS:[EDI];->ESI=402AAF94 "players"
   004D3B19   0FBF6E 22              MOVSX EBP,WORD PTR DS:[ESI+22];->EBP=00000200 (size of static player)
   004D3B1D   8B5E 34                MOV EBX,DWORD PTR DS:[ESI+34];->EBX=402AAFCC (ptr to first static player in array)
   004D3B20   0FBFD1                 MOVSX EDX,CX;->EDX=00000000 index
   004D3B23   0FAFD5                 IMUL EDX,EBP;0*200 = 0
   004D3B26   33C0                   XOR EAX,EAX;->EAX=00000000
   004D3B28   03D3                   ADD EDX,EBX;->EDX=402AAFCC (static player)
   004D3B2A   66:3B4E 2E             CMP CX,WORD PTR DS:[ESI+2E];check if in range of NumOfItems
   004D3B2E   7D 2D                  JGE SHORT haloce.004D3B5D
   004D3B30   66:8B1A                /MOV BX,WORD PTR DS:[EDX];->BX=EC70 (static player ID)
   004D3B33   0FBFF3                 |MOVSX ESI,BX;->ESI=FFFFEC70
   004D3B36   0FBFC1                 |MOVSX EAX,CX;->EAX=00000000
   004D3B39   C1E6 10                |SHL ESI,10;shift bits ->ESI=EC700000
   004D3B3C   0BF0                   |OR ESI,EAX;add player index (its now a player tag (ID + index))->ESI=EC700000
   004D3B3E   41                     |INC ECX;->ECX=005F0001
   004D3B3F   66:85DB                |TEST BX,BX;valid ID test BX=EC70 (invalid = 0000)
   004D3B42   75 14                  |JNZ SHORT haloce.004D3B58
   ;004D3B44   8B07             MOV EAX,DWORD PTR DS:[EDI]
   ;004D3B46   03D5             ADD EDX,EBP
   ;004D3B48   66:3B48 2E       CMP CX,WORD PTR DS:[EAX+2E]
   ;004D3B4C  ^7C E2            JL SHORT haloce.004D3B30
   ;004D3B4E   5E               POP ESI
   ;004D3B4F   5D               POP EBP
   ;004D3B50   33C0             XOR EAX,EAX
   ;004D3B52   66:894F 04       MOV WORD PTR DS:[EDI+4],CX
   ;004D3B56   5B               POP EBX
   ;004D3B57   C3               RETN
   004D3B58   8977 08                MOV DWORD PTR DS:[EDI+8],ESI;ptr to player tag (invalid tag = -1)->0018CDE4=EC700000
   004D3B5B   8BC2                   MOV EAX,EDX;return ptr to static player->EAX=402AAFCC
   004D3B5D   5E                     POP ESI
   004D3B5E   5D                     POP EBP
   004D3B5F   66:894F 04             MOV WORD PTR DS:[EDI+4],CX;CX=0001 ptr ->0018CDE0=655F0001
   004D3B63   5B                     POP EBX
   004D3B64   C3                     RETN
  <-
  0045B34E   85C0                   TEST EAX,EAX;test static player tag
  0045B350   74 12                  JE SHORT haloce.0045B364
  0045B352   3970 34                /CMP DWORD PTR DS:[EAX+34],ESI;test player obj tag ESI=FFFFFFFF == E2BD004E?
  0045B355   75 15                  |JNZ SHORT haloce.0045B36C
  ;0045B357   8D7C24 08        LEA EDI,DWORD PTR SS:[ESP+8]
  ;0045B35B   E8 B0870700      CALL haloce.004D3B10
  ;0045B360   85C0             TEST EAX,EAX
  ;0045B362  ^75 EE            JNZ SHORT haloce.0045B352
  ;0045B364   5F               POP EDI
  ;0045B365   8BC6             MOV EAX,ESI
  ;0045B367   5E               POP ESI
  ;0045B368   83C4 10          ADD ESP,10
  ;0045B36B   C3               RETN
  0045B36C   8B4424 10              MOV EAX,DWORD PTR SS:[ESP+10];return static player tag->EAX=EC700000
  0045B370   5F                     POP EDI;restore->EDI=40848B44
  0045B371   5E                     POP ESI;restore->ESI=40848BE4
  0045B372   83C4 10                ADD ESP,10
  0045B375   C3                     RETN
 <-
 0045B38B   83F8 FF                CMP EAX,-1;valid tag test EAX=EC700000 == FFFFFFFF?
 0045B38E   0F84 9E010000          JE haloce.0045B532 ;rets -1
 0045B394   8B0D C0558100          MOV ECX,DWORD PTR DS:[8155C0];players->ECX=402AAF94
 0045B39A   25 FFFF0000            AND EAX,0FFFF;get index from tag->EAX=00000000
 0045B39F   53                     PUSH EBX;save 40848BE8
 0045B3A0   8B59 34                MOV EBX,DWORD PTR DS:[ECX+34];ptr to static player array->EBX=402AAFCC
 0045B3A3   C1E0 09                SHL EAX,9 ;same as 0*2^9 or 0*200->EAX=00000000
 0045B3A6   03C3                   ADD EAX,EBX;add offset to ptr to get static player at index->EAX=402AAFCC
 0045B3A8   56                     PUSH ESI;save 40848BE4
 0045B3A9   8BF0                   MOV ESI,EAX ;->ESI=402AAFCC
 0045B3AB   8B4E 34                MOV ECX,DWORD PTR DS:[ESI+34];player obj tag->ECX=E2BD004E
 0045B3AE   8D4424 20              LEA EAX,DWORD PTR SS:[ESP+20];load players ptr to EAX->EAX=0018CE08
 0045B3B2   E8 D9F10900            CALL haloce.004FA590
 ->;get players xyz coordinates
  004FA590   8B15 B0B37F00          MOV EDX,DWORD PTR DS:[7FB3B0];object
  004FA596   8B52 34                MOV EDX,DWORD PTR DS:[EDX+34]
  004FA599   81E1 FFFF0000          AND ECX,0FFFF
  004FA59F   8D0C49                 LEA ECX,DWORD PTR DS:[ECX+ECX*2]
  004FA5A2   56                     PUSH ESI
  004FA5A3   8B748A 08              MOV ESI,DWORD PTR DS:[EDX+ECX*4+8];player obj->ESI=4006825C
  004FA5A7   8B8E 1C010000          MOV ECX,DWORD PTR DS:[ESI+11C];player in veh,veh tag->ECX=FFFFFFFF
  004FA5AD   83F9 FF                CMP ECX,-1 ;test if valid tag,tag->ECX=FFFFFFFF not valid
  004FA5B0   75 17                  JNZ SHORT haloce.004FA5C9
  004FA5B2   83C6 5C                ADD ESI,5C;player obj's xyz coordinates (floats)->ESI=400682B8
  004FA5B5   8B0E                   MOV ECX,DWORD PTR DS:[ESI];x coord->ECX=421395E8 
  004FA5B7   8BD0                   MOV EDX,EAX;->EDX=0018CE08
  004FA5B9   890A                   MOV DWORD PTR DS:[EDX],ECX;save x coordiante->0018CE08=421395E8
  004FA5BB   8B4E 04                MOV ECX,DWORD PTR DS:[ESI+4];y coord->ECX=C2B46901
  004FA5BE   894A 04                MOV DWORD PTR DS:[EDX+4],ECX;save y coord->0018CE08+4=C2B46901
  004FA5C1   8B4E 08                MOV ECX,DWORD PTR DS:[ESI+8];z coordinate->ECX=3D8E4636
  004FA5C4   894A 08                MOV DWORD PTR DS:[EDX+8],ECX;save z coord->0018CE08+8=3D8E4636
  004FA5C7   5E                     POP ESI;restore->ESI=402AAFCC
  004FA5C8   C3                     RETN
  ;004FA5C9   81E1 FFFF0000    AND ECX,0FFFF
  ;004FA5CF   8D0C49           LEA ECX,DWORD PTR DS:[ECX+ECX*2]
  ;004FA5D2   8B4C8A 08        MOV ECX,DWORD PTR DS:[EDX+ECX*4+8]
  ;004FA5D6   8D56 5C          LEA EDX,DWORD PTR DS:[ESI+5C]
  ;004FA5D9   0FBEB6 20010000  MOVSX ESI,BYTE PTR DS:[ESI+120]
  ;004FA5E0   6BF6 34          IMUL ESI,ESI,34
  ;004FA5E3   57               PUSH EDI
  ;004FA5E4   0FBFB9 F2010000  MOVSX EDI,WORD PTR DS:[ECX+1F2]
  ;004FA5EB   03F1             ADD ESI,ECX
  ;004FA5ED   03FE             ADD EDI,ESI
  ;004FA5EF   57               PUSH EDI
  ;004FA5F0   E8 2B4DFDFF      CALL haloce.004CF320
  ;004FA5F5   83C4 04          ADD ESP,4
  ;004FA5F8   5F               POP EDI
  ;004FA5F9   5E               POP ESI
  ;004FA5FA   C3               RETN
 <-
 0045B3B7   8B4E 34                MOV ECX,DWORD PTR DS:[ESI+34];player obj tag->ECX=E2BD004E
 0045B3BA   8D5424 2C              LEA EDX,DWORD PTR SS:[ESP+2C];load ptr to -1->EDX=0018CE14
 0045B3BE   52                     PUSH EDX;save 0018CE14
 0045B3BF   8D4424 18              LEA EAX,DWORD PTR SS:[ESP+18];load ptr to 0->EAX=0018CDFC
 0045B3C3   E8 38F20900            CALL haloce.004FA600
 ->;get players m_LowerRot and scale xyz
  004FA600   8B15 B0B37F00          MOV EDX,DWORD PTR DS:[7FB3B0];object
  004FA606   8B52 34                MOV EDX,DWORD PTR DS:[EDX+34]
  004FA609   81E1 FFFF0000          AND ECX,0FFFF
  004FA60F   53                     PUSH EBX
  004FA610   8B5C24 08              MOV EBX,DWORD PTR SS:[ESP+8];->EBX=0018CE14
  004FA614   8D0C49                 LEA ECX,DWORD PTR DS:[ECX+ECX*2]
  004FA617   57                     PUSH EDI
  004FA618   8B7C8A 08              MOV EDI,DWORD PTR DS:[EDX+ECX*4+8];player obj->EDI=4006825C
  004FA61C   8B8F 1C010000          MOV ECX,DWORD PTR DS:[EDI+11C];player in veh, veh obj tag->ECX=FFFFFFFF
  004FA622   83F9 FF                CMP ECX,-1;test if player is in veh
  004FA625   75 34                  JNZ SHORT haloce.004FA65B
  004FA627   85C0                   TEST EAX,EAX;EAX=0018CDFC
  004FA629   74 13                  JE SHORT haloce.004FA63E
  004FA62B   8D57 74                LEA EDX,DWORD PTR DS:[EDI+74];m_LowerRot xyz->EDX=400682D0
  004FA62E   8B0A                   MOV ECX,DWORD PTR DS:[EDX];rot z->ECX=3F77C14F
  004FA630   8908                   MOV DWORD PTR DS:[EAX],ECX;save rot z->0018CDFC+0=3F77C14F
  004FA632   8B4A 04                MOV ECX,DWORD PTR DS:[EDX+4];rot y->ECX=BE80E4D4
  004FA635   8948 04                MOV DWORD PTR DS:[EAX+4],ECX;save rot y->0018CDFC+4=BE80E4D4
  004FA638   8B52 08                MOV EDX,DWORD PTR DS:[EDX+8];rot z->ECX=00000000
  004FA63B   8950 08                MOV DWORD PTR DS:[EAX+8],EDX;save rot z->0018CDFC+8=00000000
  004FA63E   85DB                   TEST EBX,EBX;test valid ptr EBX=0018CE14
  004FA640   74 62                  JE SHORT haloce.004FA6A4
  004FA642   81C7 80000000          ADD EDI,80;player obj + 0x80 =m_Scale->EDI=400682DC
  004FA648   8B07                   MOV EAX,DWORD PTR DS:[EDI];scale x->EAX=00000000
  004FA64A   8903                   MOV DWORD PTR DS:[EBX],EAX;save scale x->0018CE14+0=00000000
  004FA64C   8B4F 04                MOV ECX,DWORD PTR DS:[EDI+4];scale y->ECX=00000000
  004FA64F   894B 04                MOV DWORD PTR DS:[EBX+4],ECX;save scale x->0018CE14+4=00000000
  004FA652   8B57 08                MOV EDX,DWORD PTR DS:[EDI+8];scale z->EDX=3F800000 (1.0)
  004FA655   5F                     POP EDI;->EDI=40848B44
  004FA656   8953 08                MOV DWORD PTR DS:[EBX+8],EDX;save scale x->0018CE14+8=3F800000 (1.0)
  004FA659   5B                     POP EBX;->EBX=402AAFCC
  004FA65A   C3                     RETN
  ;004FA65B   81E1 FFFF0000    AND ECX,0FFFF
  ;004FA661   8D0C49           LEA ECX,DWORD PTR DS:[ECX+ECX*2]
  ;004FA664   8B4C8A 08        MOV ECX,DWORD PTR DS:[EDX+ECX*4+8]
  ;004FA668   0FBE97 20010000  MOVSX EDX,BYTE PTR DS:[EDI+120]
  ;004FA66F   6BD2 34          IMUL EDX,EDX,34
  ;004FA672   56               PUSH ESI
  ;004FA673   0FBFB1 F2010000  MOVSX ESI,WORD PTR DS:[ECX+1F2]
  ;004FA67A   03D1             ADD EDX,ECX
  ;004FA67C   03F2             ADD ESI,EDX
  ;004FA67E   85C0             TEST EAX,EAX
  ;004FA680   74 0C            JE SHORT haloce.004FA68E
  ;004FA682   8D57 74          LEA EDX,DWORD PTR DS:[EDI+74]
  ;004FA685   56               PUSH ESI
  ;004FA686   E8 754DFDFF      CALL haloce.004CF400
  ;004FA68B   83C4 04          ADD ESP,4
  ;004FA68E   85DB             TEST EBX,EBX
  ;004FA690   74 11            JE SHORT haloce.004FA6A3
  ;004FA692   8D97 80000000    LEA EDX,DWORD PTR DS:[EDI+80]
  ;004FA698   56               PUSH ESI
  ;004FA699   8BC3             MOV EAX,EBX
  ;004FA69B   E8 604DFDFF      CALL haloce.004CF400
  ;004FA6A0   83C4 04          ADD ESP,4
  ;004FA6A3   5E               POP ESI
  ;004FA6A4   5F               POP EDI
  ;004FA6A5   5B               POP EBX
  ;004FA6A6   C3               RETN
 <-
 0045B3C8   66:8B8424 CC000000     MOV AX,WORD PTR SS:[ESP+CC];->AX=0001 param2
 0045B3D0   83C4 04                ADD ESP,4
 0045B3D3   66:85C0                TEST AX,AX
 0045B3D6   0F8E 54010000          JLE haloce.0045B530
 0045B3DC   8B8C24 C4000000        MOV ECX,DWORD PTR SS:[ESP+C4];->ECX=40848BE4
 0045B3E3   8B5C24 30              MOV EBX,DWORD PTR SS:[ESP+30];->EBX=00000000
 0045B3E7   55                     PUSH EBP
 0045B3E8   8B6C24 30              MOV EBP,DWORD PTR SS:[ESP+30];->EBP=00000000
 0045B3EC   0FB7D0                 MOVZX EDX,AX;->EDX=00000001
 0045B3EF   33F6                   XOR ESI,ESI;->ESI=00000000
 0045B3F1   83C1 0C                ADD ECX,0C;->ECX=40848BF0  (function + 0x73 = 0x0C)
 0045B3F4   57                     PUSH EDI
 0045B3F5   8B7C24 3C              MOV EDI,DWORD PTR SS:[ESP+3C];->EDI=3F800000 (1.0)
 0045B3F9   894C24 14              MOV DWORD PTR SS:[ESP+14],ECX
 0045B3FD   895424 18              MOV DWORD PTR SS:[ESP+18],EDX;->EDX=00000001
 0045B401   8B4C24 14              /MOV ECX,DWORD PTR SS:[ESP+14];->ECX=40848BF0
 0045B405   8B09                   |MOV ECX,DWORD PTR DS:[ECX];->E3D40260
 0045B407   83F9 FF                |CMP ECX,-1;test
 0045B40A   0F84 03010000          |JE haloce.0045B513
 0045B410   0FBFD0                 |MOVSX EDX,AX
 0045B413   895424 10              |MOV DWORD PTR SS:[ESP+10],EDX
 0045B417   DB4424 10              |FILD DWORD PTR SS:[ESP+10];convert to float?->ST0=1.0
 0045B41B   D83D 60236100          |FDIVR DWORD PTR DS:[612360];ST=1.0 DS:[00612360]=6.283185->ST0=6.2831854820251464840
 0045B421   D815 98286100          |FCOM DWORD PTR DS:[612898];ST=6.2831854820251464840 == DS:[00612898]=0.3926991?
 0045B427   DFE0                   |FSTSW AX;->AX=3822
 0045B429   F6C4 41                |TEST AH,41;AH=38
 0045B42C   75 08                  |JNZ SHORT haloce.0045B436
 0045B42E   DDD8                   |FSTP ST
 0045B430   D905 98286100          |FLD DWORD PTR DS:[612898];ST0=0.3926990926265716553
 0045B436   D94424 1C              |FLD DWORD PTR SS:[ESP+1C];ST0=0.9677934050559997559 ST1=0.3926990926265716553
 0045B43A   8BC2                   |MOV EAX,EDX;->EDX=00000001
 0045B43C   D94424 20              |FLD DWORD PTR SS:[ESP+20];ST0=-0.2517458200454711914 ST1=0.9677934050559997559 ST2=0.3926990926265716553
 0045B440   99                     |CDQ;->EDX=00000000
 0045B441   D9F3                   |FPATAN;->ST0=1.8252800737494432540 ST1=0.3926990926265716553
 0045B443   2BC2                   |SUB EAX,EDX;1-0->EAX=00000001
 0045B445   D1F8                   |SAR EAX,1;->EAX=00000000
 0045B447   8BD6                   |MOV EDX,ESI;->EDX=00000000
 0045B449   2BD0                   |SUB EDX,EAX;0-0=0
 0045B44B   895424 10              |MOV DWORD PTR SS:[ESP+10],EDX;->0018CDF0=00000000
 0045B44F   6A FF                  |PUSH -1
 0045B451   51                     |PUSH ECX;ECX=E3D40260
 0045B452   8D4424 48              |LEA EAX,DWORD PTR SS:[ESP+48];->EAX=0018CE20 (ptr to 00000001)
 0045B456   DB4424 18              |FILD DWORD PTR SS:[ESP+18];ST0=0.0 ST1=1.8252800737494432540 ST0=0.3926990926265716553
 0045B45A   D8CA                   |FMUL ST,ST(2)
 0045B45C   DEC1                   |FADDP ST(1),ST;->ST0=1.8252800737494432540 ST1=0.3926990926265716553
 0045B45E   DDD9                   |FSTP ST(1);->ST0=1.8252800737494432330
 0045B460   E8 CBDB0900            |CALL haloce.004F9030
 ->
  004F9030   8B15 081D6300          MOV EDX,DWORD PTR DS:[631D08]            ; haloce.005FA80C
  004F9036   53                     PUSH EBX
  004F9037   8B5C24 0C              MOV EBX,DWORD PTR SS:[ESP+C]
  004F903B   56                     PUSH ESI
  004F903C   8BF0                   MOV ESI,EAX
  004F903E   33C0                   XOR EAX,EAX
  004F9040   57                     PUSH EDI
  004F9041   B9 22000000            MOV ECX,22
  004F9046   8BFE                   MOV EDI,ESI
  004F9048   F3:AB                  REP STOS DWORD PTR ES:[EDI];EAX=00000000
  004F904A   8B4424 10              MOV EAX,DWORD PTR SS:[ESP+10];->EAX=E3D40260
  004F904E   8906                   MOV DWORD PTR DS:[ESI],EAX
  004F9050   33FF                   XOR EDI,EDI
  004F9052   897E 04                MOV DWORD PTR DS:[ESI+4],EDI
  004F9055   8B02                   MOV EAX,DWORD PTR DS:[EDX]
  004F9057   8D4E 34                LEA ECX,DWORD PTR DS:[ESI+34]
  004F905A   8901                   MOV DWORD PTR DS:[ECX],EAX
  004F905C   8B42 04                MOV EAX,DWORD PTR DS:[EDX+4]
  004F905F   8941 04                MOV DWORD PTR DS:[ECX+4],EAX
  004F9062   8B52 08                MOV EDX,DWORD PTR DS:[EDX+8]
  004F9065   8951 08                MOV DWORD PTR DS:[ECX+8],EDX
  004F9068   8B0D 101D6300          MOV ECX,DWORD PTR DS:[631D10]            ; haloce.005FA824
  004F906E   8B11                   MOV EDX,DWORD PTR DS:[ECX]
  004F9070   8D46 40                LEA EAX,DWORD PTR DS:[ESI+40]
  004F9073   8910                   MOV DWORD PTR DS:[EAX],EDX
  004F9075   8B51 04                MOV EDX,DWORD PTR DS:[ECX+4]
  004F9078   8950 04                MOV DWORD PTR DS:[EAX+4],EDX
  004F907B   8B49 08                MOV ECX,DWORD PTR DS:[ECX+8]
  004F907E   8948 08                MOV DWORD PTR DS:[EAX+8],ECX
  004F9081   6A FF                  PUSH -1
  004F9083   8BCB                   MOV ECX,EBX
  004F9085   66:897E 16             MOV WORD PTR DS:[ESI+16],DI
  004F9089   E8 C21A0000            CALL haloce.004FAB50
  ->
   004FAB50   57                     PUSH EDI
   004FAB51   33FF                   XOR EDI,EDI
   004FAB53   83F9 FF                CMP ECX,-1
   004FAB56   74 39                  JE SHORT haloce.004FAB91
   ...
   004FAB91   33C0                   XOR EAX,EAX
   004FAB93   85FF                   TEST EDI,EDI
   004FAB95   74 13                  JE SHORT haloce.004FABAA
   ...
   004FABAA   5F                     POP EDI
   004FABAB   C3                     RETN
  <-
  004F908E   83C4 04                ADD ESP,4
  004F9091   3BC7                   CMP EAX,EDI
  004F9093   74 19                  JE SHORT haloce.004F90AE
  ...
  004F90AE   C746 0C FFFFFFFF       MOV DWORD PTR DS:[ESI+C],-1
  004F90B5   C746 08 FFFFFFFF       MOV DWORD PTR DS:[ESI+8],-1
  004F90BC   66:C746 14 FFFF        MOV WORD PTR DS:[ESI+14],0FFFF
  004F90C2   8B15 C4206200          MOV EDX,DWORD PTR DS:[6220C4]            ; haloce.005F364C
  004F90C8   8D46 58                LEA EAX,DWORD PTR DS:[ESI+58]
  004F90CB   B9 04000000            MOV ECX,4
  004F90D0   8BF2                   MOV ESI,EDX
  004F90D2   8B1E                   MOV EBX,DWORD PTR DS:[ESI]
  004F90D4   8BF8                   MOV EDI,EAX
  004F90D6   891F                   MOV DWORD PTR DS:[EDI],EBX
  004F90D8   8B5E 04                MOV EBX,DWORD PTR DS:[ESI+4]
  004F90DB   895F 04                MOV DWORD PTR DS:[EDI+4],EBX
  004F90DE   8B76 08                MOV ESI,DWORD PTR DS:[ESI+8]
  004F90E1   83C0 0C                ADD EAX,0C
  004F90E4   49                     DEC ECX
  004F90E5   8977 08                MOV DWORD PTR DS:[EDI+8],ESI
  004F90E8  ^75 E6                  JNZ SHORT haloce.004F90D0
  ...*3^
  004F90EA   5F                     POP EDI
  004F90EB   5E                     POP ESI
  004F90EC   5B                     POP EBX
  004F90ED   C3                     RETN
 <-
 0045B465   D9C0                   |FLD ST
 0045B467   8B4424 24              |MOV EAX,DWORD PTR SS:[ESP+24]
 0045B46B   D9FF                   |FCOS
 0045B46D   8B4C24 28              |MOV ECX,DWORD PTR SS:[ESP+28]
 0045B471   8B5424 2C              |MOV EDX,DWORD PTR SS:[ESP+2C]
 0045B475   83C4 08                |ADD ESP,8
 0045B478   66:833D D0476B00 02    |CMP WORD PTR DS:[6B47D0],2
 0045B480   894424 74              |MOV DWORD PTR SS:[ESP+74],EAX
 0045B484   894C24 78              |MOV DWORD PTR SS:[ESP+78],ECX
 0045B488   895424 7C              |MOV DWORD PTR SS:[ESP+7C],EDX
 0045B48C   89AC24 80000000        |MOV DWORD PTR SS:[ESP+80],EBP
 0045B493   899C24 84000000        |MOV DWORD PTR SS:[ESP+84],EBX
 0045B49A   89BC24 88000000        |MOV DWORD PTR SS:[ESP+88],EDI
 0045B4A1   B8 03000000            |MOV EAX,3
 0045B4A6   D80D E4236100          |FMUL DWORD PTR DS:[6123E4]
 0045B4AC   D84424 28              |FADD DWORD PTR SS:[ESP+28]
 0045B4B0   D95C24 58              |FSTP DWORD PTR SS:[ESP+58]
 0045B4B4   D9FE                   |FSIN
 0045B4B6   D80D E4236100          |FMUL DWORD PTR DS:[6123E4]
 0045B4BC   D84424 2C              |FADD DWORD PTR SS:[ESP+2C]
 0045B4C0   D95C24 5C              |FSTP DWORD PTR SS:[ESP+5C]
 0045B4C4   D94424 30              |FLD DWORD PTR SS:[ESP+30]
 0045B4C8   D805 D8236100          |FADD DWORD PTR DS:[6123D8]
 0045B4CE   D95C24 60              |FSTP DWORD PTR SS:[ESP+60]
 0045B4D2   75 29                  |JNZ SHORT haloce.0045B4FD
 0045B4D4   8B4C24 40              |MOV ECX,DWORD PTR SS:[ESP+40]
 0045B4D8   8B15 E46D8100          |MOV EDX,DWORD PTR DS:[816DE4]
 0045B4DE   81E1 FFFF0000          |AND ECX,0FFFF
 0045B4E4   C1E1 05                |SHL ECX,5
 0045B4E7   8B4C11 14              |MOV ECX,DWORD PTR DS:[ECX+EDX+14]
 0045B4EB   0FBF11                 |MOVSX EDX,WORD PTR DS:[ECX]
 0045B4EE   8B0C95 A4766300        |MOV ECX,DWORD PTR DS:[EDX*4+6376A4]
 0045B4F5   8379 10 FF             |CMP DWORD PTR DS:[ECX+10],-1
 0045B4F9   74 02                  |JE SHORT haloce.0045B4FD
 
 0045B4FB   33C0                   |XOR EAX,EAX
 0045B4FD   50                     |PUSH EAX;EAX=00000000
 0045B4FE   8D5424 44              |LEA EDX,DWORD PTR SS:[ESP+44]
 ;struct create_obj_data
 ;{
 ;   int obj_type_tag;
 ;   0
 ;   -1
 ;   -1
 ;   0
 ;   0000FFFF
 ;   C38D246E (-282.28460693359375) x coord  m_World
 ;   441D0C44 (628.191650390625)    y coord
 ;   42DF3BE8 (111.61700439453125)  z coord
 ;   0           x vect m_Velocity
 ;   0           y vect
 ;   0           z vect
 ;   0 (0.0)                        
 ;   3F62B8A6 (0.8856300115585327)  x rot m_LowerRot   (create script obj (1.0))
 ;   BEEDC4BB (-0.4643915593624115) y rot              (create script obj (0.0))
 ;   0                              z rot
 ;   0              x scale m_Scale                    (create script obj 80000000(0.0))
 ;   0              y                                  (create script obj 80000000(0.0))
 ;   3F800000 (1.0) z
 
 ;   0 x vect spin
 ;   0 y vect
 ;   0 z vect
 
 ;   3F800000 (1.0)
 ;   3F800000 (1.0)
 ;   3F800000 (1.0)
 
 ;   3F800000 (1.0)
 ;   3F800000 (1.0)
 ;   3F800000 (1.0)
 
 ;   3F800000 (1.0)
 ;   3F800000 (1.0)
 ;   3F800000 (1.0)
 
 ;   3F800000 (1.0)
 ;   3F800000 (1.0)
 ;   3F800000 (1.0)
 ;};
 0045B502   52                     |PUSH EDX;EDX=0018CE20 (ptr to E3D40260)
 0045B503   E8 38DC0900            |CALL haloce.004F9140
 ->;huge function,2 parameters __cdecl create object(int, struct?vehicle tag?) ?
 <-;return obj tag
 0045B508   66:8B8424 D8000000 |MOV AX,WORD PTR SS:[ESP+D8]
 0045B510   83C4 08          |ADD ESP,8
 0045B513   8B5424 14        |MOV EDX,DWORD PTR SS:[ESP+14]
 0045B517   8B4C24 18        |MOV ECX,DWORD PTR SS:[ESP+18]
 0045B51B   83C2 10          |ADD EDX,10
 0045B51E   46               |INC ESI
 0045B51F   49               |DEC ECX
 0045B520   895424 14        |MOV DWORD PTR SS:[ESP+14],EDX
 0045B524   894C24 18        |MOV DWORD PTR SS:[ESP+18],ECX
 0045B528  ^0F85 D3FEFFFF    \JNZ haloce.0045B401
 0045B52E   5F               POP EDI
 0045B52F   5D               POP EBP
 0045B530   5E               POP ESI
 0045B531   5B               POP EBX
 0045B532   81C4 B8000000    ADD ESP,0B8
 0045B538   C3               RETN
<-

 ;player  coordinates: x=85.24 y=-157.77  z=-0.02
 ;vehicle coordinates: x=84.72 y=-156.36   z=0.78
 ;difference:          x=-0.52 y=1.41      z=0.80














0045B199   83C4 08          ADD ESP,8
0045B19C   5B               POP EBX
0045B19D   5E               POP ESI
0045B19E   5D               POP EBP
0045B19F   5F               POP EDI
0045B1A0   C3               RETN

