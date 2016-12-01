;notes by Jesus7Freak
;haloce1.09 bloodgulch ctf  all vehicles 
;console 004C9BE3

...1 ;inspect
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F5DD8   haloce.0048C8D0
...2 ;object_destroy_all
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F6000   haloce.0047D570
...3 ;inspect
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F5DD8   haloce.0048C8D0


...2
0048D1E0   51               PUSH ECX ;403D8501
0048D1E1   57               PUSH EDI ;F3690000
0048D1E2   50               PUSH EAX ;0000002C
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F6000   haloce.0047D570
->
;\xC3\xCC\x83\xEC\x10\xA1\x00\x00\x00\x00\x55\x8B xxxxxx????xx
;object_destroy_all function only CE?
0047D570   E8 ABD50000      CALL haloce.0048AB20
->
 0048AB20   83EC 10          SUB ESP,10
 0048AB23   A1 C0558100      MOV EAX,DWORD PTR DS:[8155C0];players->EAX=402AAF94
 0048AB28   55               PUSH EBP
 0048AB29   894424 04        MOV DWORD PTR SS:[ESP+4],EAX;save players
 0048AB2D   57               PUSH EDI
 0048AB2E   35 72657469      XOR EAX,69746572;->EAX=295ECAE6
 0048AB33   83CD FF          OR EBP,FFFFFFFF;->EBP=FFFFFFFF
 
 ;return static player, get static player tag, and increment index
 0048AB36   8D7C24 08        LEA EDI,DWORD PTR SS:[ESP+8];struct ptr, players of struct?->EDI=0018CEB4
 0048AB3A   66:C74424 0C 0000 MOV WORD PTR SS:[ESP+C],0 ;player index?
 0048AB41   896C24 10        MOV DWORD PTR SS:[ESP+10],EBP ;-1 of struct?
 0048AB45   894424 14        MOV DWORD PTR SS:[ESP+14],EAX ;unknown of struct?
 0048AB49   E8 C28F0400      CALL haloce.004D3B10
 ->
  004D3B10   66:8B4F 04           MOV CX,WORD PTR DS:[EDI+4] ;->CX=0000
  004D3B14   53                   PUSH EBX
  004D3B15   55                   PUSH EBP
  004D3B16   56                   PUSH ESI
  004D3B17   8B37                 MOV ESI,DWORD PTR DS:[EDI] ;players->ESI=402AAF94
  004D3B19   0FBF6E 22            MOVSX EBP,WORD PTR DS:[ESI+22] ;static player size->EBP=00000200
  004D3B1D   8B5E 34              MOV EBX,DWORD PTR DS:[ESI+34] ;ptr to first static player->EBX=402AAFCC
  004D3B20   0FBFD1               MOVSX EDX,CX ;->EDX=00000000 index
  004D3B23   0FAFD5               IMUL EDX,EBP ;->EDX=00000000 index*size
  004D3B26   33C0                 XOR EAX,EAX ;->EAX=00000000
  004D3B28   03D3                 ADD EDX,EBX ;->EDX=402AAFCC player in array
  004D3B2A   66:3B4E 2E           CMP CX,WORD PTR DS:[ESI+2E] ;num of static players
  004D3B2E   7D 2D                JGE SHORT haloce.004D3B5D
  004D3B30   66:8B1A              /MOV BX,WORD PTR DS:[EDX] ;static player ID->BX=EC70
  004D3B33   0FBFF3               |MOVSX ESI,BX ;->ESI=FFFFEC70
  004D3B36   0FBFC1               |MOVSX EAX,CX ;static player index->EAX=00000000
  004D3B39   C1E6 10              |SHL ESI,10 ;->ESI=EC700000
  004D3B3C   0BF0                 |OR ESI,EAX ;static player tag->EC700000
  004D3B3E   41                   |INC ECX ;->ECX=403D0001
  004D3B3F   66:85DB              |TEST BX,BX ;BX=EC70
  004D3B42   75 14                |JNZ SHORT haloce.004D3B58
  ...
  ;004D3B44   8B07                 |MOV EAX,DWORD PTR DS:[EDI]
  ;004D3B46   03D5                 |ADD EDX,EBP
  ;004D3B48   66:3B48 2E           |CMP CX,WORD PTR DS:[EAX+2E]
  ;004D3B4C  ^7C E2                \JL SHORT haloce.004D3B30
  ;004D3B4E   5E                   POP ESI
  ;004D3B4F   5D                   POP EBP
  ;004D3B50   33C0                 XOR EAX,EAX
  ;004D3B52   66:894F 04           MOV WORD PTR DS:[EDI+4],CX
  ;004D3B56   5B                   POP EBX
  ;004D3B57   C3                   RETN
  ...
  004D3B58   8977 08              MOV DWORD PTR DS:[EDI+8],ESI ;save static player tag to struct
  004D3B5B   8BC2                 MOV EAX,EDX ;return static player->EAX=402AAFCC
  004D3B5D   5E                   POP ESI
  004D3B5E   5D                   POP EBP
  004D3B5F   66:894F 04           MOV WORD PTR DS:[EDI+4],CX ;save 00001 to struct
  004D3B63   5B                   POP EBX
  004D3B64   C3                   RETN
 <-
 0048AB4E   85C0                 TEST EAX,EAX ;EAX=402AAFCC
 0048AB50   74 4C                JE SHORT haloce.0048AB9E
 0048AB52   56                   PUSH ESI
 
 ;doesn't destroy player obj's or the vehs the players are in
 0048AB53   8B70 34              /MOV ESI,DWORD PTR DS:[EAX+34] ;player obj tag->ESI=E2A80039
 0048AB56   3BF5                 |CMP ESI,EBP ;test valid tag E2A80039 == -1?
 0048AB58   74 36                |JE SHORT haloce.0048AB90
 0048AB5A   8B0D B0B37F00        |MOV ECX,DWORD PTR DS:[7FB3B0];object
 0048AB60   8B49 34              |MOV ECX,DWORD PTR DS:[ECX+34];ptr to first obj table->ECX=400506EC
 0048AB63   8BC6                 |MOV EAX,ESI ;->EAX=E2A80039
 0048AB65   8BD0                 |/MOV EDX,EAX ;->EDX=E2A80039
 0048AB67   25 FFFF0000          ||AND EAX,0FFFF ;object index->EAX=00000039
 0048AB6C   8D0440               ||LEA EAX,DWORD PTR DS:[EAX+EAX*2];->EAX=000000AB
 0048AB6F   8B4481 08            ||MOV EAX,DWORD PTR DS:[ECX+EAX*4+8];player obj->EAX=40061EA8
 0048AB73   8B80 1C010000        ||MOV EAX,DWORD PTR DS:[EAX+11C] ;player obj, in veh, tag->EAX=FFFFFFFF
 0048AB79   3BC5                 ||CMP EAX,EBP ;test valid tag FFFFFFFF == -1? not valid
 0048AB7B  ^75 E8                |\JNZ SHORT haloce.0048AB65
 0048AB7D   3BD6                 |CMP EDX,ESI ;was the player in a veh?  E2A80039 != E2A80039 no
 0048AB7F   74 0F                |JE SHORT haloce.0048AB90
 ...
 ;0048AB81   6A 01                |PUSH 1
 ;0048AB83   6A 01                |PUSH 1
 ;0048AB85   6A 00                |PUSH 0
 ;0048AB87   56                   |PUSH ESI
 ;0048AB88   E8 F3570E00          |CALL haloce.00570380
 ;0048AB8D   83C4 10              |ADD ESP,10
 ...
 ;get the rest of the static players
 ;return static player, get static player tag, and increment index
 0048AB90   8D7C24 0C            |LEA EDI,DWORD PTR SS:[ESP+C];struct ptr, 
 0048AB94   E8 778F0400          |CALL haloce.004D3B10
 ->
 <-
 0048AB99   85C0                 |TEST EAX,EAX ;->EAX=00000000 no more static players
 0048AB9B  ^75 B6                \JNZ SHORT haloce.0048AB53
 0048AB9D   5E                   POP ESI                                  ; 403D8500
 
 0048AB9E   8D4C24 08            LEA ECX,DWORD PTR SS:[ESP+8] ;struct ptr->ECX=0018CEB4
 0048ABA2   51                   PUSH ECX ;first param struct ptr
 0048ABA3   C74424 18 86868686   MOV DWORD PTR SS:[ESP+18],86868686 ;5th member
 0048ABAB   896C24 0C            MOV DWORD PTR SS:[ESP+C],EBP ;1st member -1
 0048ABAF   C64424 10 00         MOV BYTE PTR SS:[ESP+10],0 ;2nd member (2byte aligned?)
 0048ABB4   66:C74424 12 0000    MOV WORD PTR SS:[ESP+12],0 ;3rd memeber
 0048ABBB   896C24 14            MOV DWORD PTR SS:[ESP+14],EBP ;4th memeber -1
 0048ABBF   E8 ECFF0600          CALL haloce.004FABB0
 ->
  004FABB0   53                   PUSH EBX
  004FABB1   55                   PUSH EBP
  004FABB2   8B6C24 0C            MOV EBP,DWORD PTR SS:[ESP+C] ;5 member struct ptr->EBP=0018CEB4
  004FABB6   66:8B55 06           MOV DX,WORD PTR SS:[EBP+6] ;3rd member->DX=0000
  004FABBA   0FBFCA               MOVSX ECX,DX;->ECX=00000000
  004FABBD   56                   PUSH ESI
  004FABBE   8D3449               LEA ESI,DWORD PTR DS:[ECX+ECX*2] ;->ESI=00000000
  004FABC1   8B0D B0B37F00        MOV ECX,DWORD PTR DS:[7FB3B0];object->ECX=400506B4
  004FABC7   66:8B59 2E           MOV BX,WORD PTR DS:[ECX+2E] ;num of objects->BX=0062
  004FABCB   57                   PUSH EDI
  004FABCC   8B79 34              MOV EDI,DWORD PTR DS:[ECX+34];ptr to first object table->EDI=400506EC
  004FABCF   33C0                 XOR EAX,EAX ;set EAX to 0
  004FABD1   66:3BD3              CMP DX,BX ;DX=0000 >= BX=0062
  004FABD4   8D34B7               LEA ESI,DWORD PTR DS:[EDI+ESI*4] ;object table 0->ESI=400506EC
  004FABD7   7D 4F                JGE SHORT haloce.004FAC28
  004FABD9   8DA424 00000000      LEA ESP,DWORD PTR SS:[ESP] ;even in halo, theres filler...
  004FABE0   66:8B0E              /MOV CX,WORD PTR DS:[ESI] ;object ID->CX=E26F
  004FABE3   0FBFF9               |MOVSX EDI,CX ;obj ID->EDI=FFFFE26F
  004FABE6   0FBFC2               |MOVSX EAX,DX ;obj Index->EAX=00000000
  004FABE9   C1E7 10              |SHL EDI,10 ;->EDI=E26F0000
  004FABEC   0BF8                 |OR EDI,EAX ;obj tag->EDI=E26F0000
  004FABEE   42                   |INC EDX ;->EDX=402A0001
  004FABEF   66:85C9              |TEST CX,CX ;test valid ID CX != 0
  004FABF2   74 1B                |JE SHORT haloce.004FAC0F
  004FABF4   8A4D 04              |MOV CL,BYTE PTR SS:[EBP+4] ;->CL=00
  004FABF7   8A46 02              |MOV AL,BYTE PTR DS:[ESI+2] ;->AL=63
  004FABFA   22C1                 |AND AL,CL ;->AL=00
  004FABFC   3AC1                 |CMP AL,CL ;AL == CL
  004FABFE   75 0F                |JNZ SHORT haloce.004FAC0F
  004FAC00   8A4E 03              |MOV CL,BYTE PTR DS:[ESI+3];->CL=00
  004FAC03   B8 01000000          |MOV EAX,1 ;->EAX=00000001
  004FAC08   D3E0                 |SHL EAX,CL ;->EAX=00000002
  004FAC0A   8545 00              |TEST DWORD PTR SS:[EBP],EAX ;-1 == 2
  004FAC0D   75 13                |JNZ SHORT haloce.004FAC22
  ...
  ;004FAC0F   83C6 0C              |ADD ESI,0C
  ;004FAC12   66:3BD3              |CMP DX,BX
  ;004FAC15  ^7C C9                \JL SHORT haloce.004FABE0
  ;004FAC17   5F                   POP EDI
  ;004FAC18   5E                   POP ESI
  ;004FAC19   66:8955 06           MOV WORD PTR SS:[EBP+6],DX
  ;004FAC1D   5D                   POP EBP
  ;004FAC1E   33C0                 XOR EAX,EAX
  ;004FAC20   5B                   POP EBX
  ;004FAC21   C3                   RETN
  ...
  004FAC22   897D 08              MOV DWORD PTR SS:[EBP+8],EDI ;save obj tag
  004FAC25   8B46 08              MOV EAX,DWORD PTR DS:[ESI+8] ;obj->EAX=4005673C
  004FAC28   5F                   POP EDI
  004FAC29   5E                   POP ESI
  004FAC2A   66:8955 06           MOV WORD PTR SS:[EBP+6],DX;save 0001
  004FAC2E   5D                   POP EBP
  004FAC2F   5B                   POP EBX
  004FAC30   C3                   RETN
 <-
 0048ABC4   83C4 04              ADD ESP,4
 0048ABC7   85C0                 TEST EAX,EAX;test valid obj EAX=4005673C
 0048ABC9   74 65                JE SHORT haloce.0048AC30
 0048ABCB   EB 03                JMP SHORT haloce.0048ABD0
 ...
 ;0048ABCD   8D49 00              LEA ECX,DWORD PTR DS:[ECX]
 ...
 0048ABD0   39A8 1C010000        /CMP DWORD PTR DS:[EAX+11C],EBP ;test if in veh -1 == -1
 0048ABD6   75 47                |JNZ SHORT haloce.0048AC1F
 0048ABD8   8B7C24 10            |MOV EDI,DWORD PTR SS:[ESP+10] ;obj  tag EDI=->E26F0000
 
 0048ABDC   57                   |PUSH EDI
 0048ABDD   E8 7EFDFFFF          |CALL haloce.0048A960
 ->
  0048A960   8B4C24 04        MOV ECX,DWORD PTR SS:[ESP+4] ;->ECX=E26F0000
  0048A964   53               PUSH EBX
  0048A965   55               PUSH EBP
  0048A966   8BC1             MOV EAX,ECX ;->EAX=E26F0000
  0048A968   56               PUSH ESI
  0048A969   8B35 B0B37F00    MOV ESI,DWORD PTR DS:[7FB3B0];"object" ->ESI=400506B4
  0048A96F   8B56 34          MOV EDX,DWORD PTR DS:[ESI+34];first obj table->EDX=400506EC
  0048A972   25 FFFF0000      AND EAX,0FFFF ;->EAX=00000000
  0048A977   8D0440           LEA EAX,DWORD PTR DS:[EAX+EAX*2] ;->EAX=00000000
  0048A97A   8B6C82 08        MOV EBP,DWORD PTR DS:[EDX+EAX*4+8] ;obj->EBP=4005673C
  
  0048A97E   51               PUSH ECX ;obj tag
  0048A97F   E8 6CD2FEFF      CALL haloce.00477BF0
  ->
   00477BF0   83EC 10            SUB ESP,10
   00477BF3   A1 C0558100        MOV EAX,DWORD PTR DS:[8155C0] ;"players"->EAX=402AAF94
   00477BF8   53                 PUSH EBX
   00477BF9   8B5C24 18          MOV EBX,DWORD PTR SS:[ESP+18] ;first param obj tag->EBX=E26F0000
   00477BFD   56                 PUSH ESI
   00477BFE   894424 08          MOV DWORD PTR SS:[ESP+8],EAX ;store "players"
   00477C02   57                 PUSH EDI
   00477C03   35 72657469        XOR EAX,69746572 ;->EAX=295ECAE6
   00477C08   83CE FF            OR ESI,FFFFFFFF ;->ESI=FFFFFFFF
   
   ;return static player, get static player tag, and increment index
   00477C0B   8D7C24 0C          LEA EDI,DWORD PTR SS:[ESP+C] ;struct ptr first element "players"->EDI=0018CE80
   00477C0F   66:C74424 10 0000  MOV WORD PTR SS:[ESP+10],0 ;2nd element (4 byte aligned?) player index
   00477C16   897424 14          MOV DWORD PTR SS:[ESP+14],ESI ;FFFFFFFF
   00477C1A   894424 18          MOV DWORD PTR SS:[ESP+18],EAX ;295ECAE6
   00477C1E   E8 EDBE0500        CALL haloce.004D3B10
   
   00477C23   85C0               TEST EAX,EAX ;->EAX=402AAFCC
   00477C25   74 16              JE SHORT haloce.00477C3D
   00477C27   3958 34            /CMP DWORD PTR DS:[EAX+34],EBX;is this a player obj? cmp obj tags E26F0000 != E2C40055
   00477C2A   75 04              |JNZ SHORT haloce.00477C30
   ...
   ;00477C2C   8B7424 14          |MOV ESI,DWORD PTR SS:[ESP+14]
   ...
   ;compares all player objs to it
   00477C30   8D7C24 0C          |LEA EDI,DWORD PTR SS:[ESP+C];struct->EDI=0018CE80
   00477C34   E8 D7BE0500        |CALL haloce.004D3B10
   00477C39   85C0               |TEST EAX,EAX ;->EAX=00000000 no more players
   00477C3B  ^75 EA              \JNZ SHORT haloce.00477C27
   00477C3D   5F                 POP EDI
   00477C3E   8BC6               MOV EAX,ESI ;not a player obj->EAX=FFFFFFFF
   00477C40   5E                 POP ESI
   00477C41   5B                 POP EBX
   00477C42   83C4 10            ADD ESP,10
   00477C45   C3                 RETN
  <-
  0048A984   83C4 04          ADD ESP,4
  
  0048A987   83F8 FF          CMP EAX,-1 ;EAX=-1
  0048A98A   0F95C3           SETNE BL ;FALSE->BL=00
  0048A98D   84DB             TEST BL,BL
  0048A98F   0F85 A2000000    JNZ haloce.0048AA37
  0048A995   8B8D 18010000    MOV ECX,DWORD PTR SS:[EBP+118];->ECX=FFFFFFFF
  0048A99B   83F9 FF          CMP ECX,-1 ;ECX=-1
  0048A99E   74 2F            JE SHORT haloce.0048A9CF
  ...
  ;check if their are more matches?
  ;0048A9A0   8B56 34          /MOV EDX,DWORD PTR DS:[ESI+34]
  ;0048A9A3   8BC1             |MOV EAX,ECX
  ;0048A9A5   25 FFFF0000      |AND EAX,0FFFF
  ;0048A9AA   8D0440           |LEA EAX,DWORD PTR DS:[EAX+EAX*2]
  ;0048A9AD   8B7482 08        |MOV ESI,DWORD PTR DS:[EDX+EAX*4+8]
  ;0048A9B1   51               |PUSH ECX
  ;0048A9B2   E8 A9FFFFFF      |CALL haloce.0048A960
  ;0048A9B7   83C4 04          |ADD ESP,4
  ;0048A9BA   84C0             |TEST AL,AL
  ;0048A9BC   75 6C            |JNZ SHORT haloce.0048AA2A
  ;0048A9BE   8B8E 14010000    |MOV ECX,DWORD PTR DS:[ESI+114]
  ;0048A9C4   83F9 FF          |CMP ECX,-1
  ;0048A9C7   8B35 B0B37F00    |MOV ESI,DWORD PTR DS:[7FB3B0]
  ;0048A9CD  ^75 D1            \JNZ SHORT haloce.0048A9A0
  ...
  0048A9CF   8B8D 1C010000    MOV ECX,DWORD PTR SS:[EBP+11C];obj in veh tag->ECX=-1
  0048A9D5   83F9 FF          CMP ECX,-1
  0048A9D8   57               PUSH EDI
  0048A9D9   74 2E            JE SHORT haloce.0048AA09
  ...
  ;check if the obj is in a veh?
  ;0048A9DB   EB 03            JMP SHORT haloce.0048A9E0
  ;0048A9DD   8D49 00          LEA ECX,DWORD PTR DS:[ECX]
  ;0048A9E0   8B56 34          /MOV EDX,DWORD PTR DS:[ESI+34]
  ;0048A9E3   8BC1             |MOV EAX,ECX
  ;0048A9E5   25 FFFF0000      |AND EAX,0FFFF
  ;0048A9EA   8D0440           |LEA EAX,DWORD PTR DS:[EAX+EAX*2]
  ;0048A9ED   8B7C82 08        |MOV EDI,DWORD PTR DS:[EDX+EAX*4+8]
  ;0048A9F1   51               |PUSH ECX
  ;0048A9F2   E8 F9D1FEFF      |CALL haloce.00477BF0
  ;0048A9F7   83C4 04          |ADD ESP,4
  ;0048A9FA   83F8 FF          |CMP EAX,-1
  ;0048A9FD   75 31            |JNZ SHORT haloce.0048AA30
  ;0048A9FF   8B8F 1C010000    |MOV ECX,DWORD PTR DS:[EDI+11C]
  ;0048AA05   3BC8             |CMP ECX,EAX
  ;0048AA07  ^75 D7            \JNZ SHORT haloce.0048A9E0
  ...
  0048AA09   8A8D B4000000    MOV CL,BYTE PTR SS:[EBP+B4] ;obj flag?->CL=02
  0048AA0F   B8 01000000      MOV EAX,1
  0048AA14   D3E0             SHL EAX,CL;->EAX=00000004
  0048AA16   A8 1C            TEST AL,1C
  0048AA18   74 09            JE SHORT haloce.0048AA23
  0048AA1A   F685 F4010000 02 TEST BYTE PTR SS:[EBP+1F4],2 ;another obj flag? [40056930]=08
  0048AA21   75 0D            JNZ SHORT haloce.0048AA30
  0048AA23   5F               POP EDI
  0048AA24   5E               POP ESI
  0048AA25   5D               POP EBP
  0048AA26   8AC3             MOV AL,BL ;->AL=00
  0048AA28   5B               POP EBX
  0048AA29   C3               RETN
  ;0048AA2A   5E               POP ESI
  ;0048AA2B   5D               POP EBP
  ;0048AA2C   B0 01            MOV AL,1
  ;0048AA2E   5B               POP EBX
  ;0048AA2F   C3               RETN
  ;0048AA30   5F               POP EDI
  ;0048AA31   5E               POP ESI
  ;0048AA32   5D               POP EBP
  ;0048AA33   B0 01            MOV AL,1
  ;0048AA35   5B               POP EBX
  ;0048AA36   C3               RETN
  ;0048AA37   5E               POP ESI
  ;0048AA38   5D               POP EBP
  ;0048AA39   8AC3             MOV AL,BL
  ;0048AA3B   5B               POP EBX
  ;0048AA3C   C3               RETN
 <-
 0048ABE2   83C4 04              |ADD ESP,4
 
 0048ABE5   84C0                 |TEST AL,AL
 0048ABE7   75 36                |JNZ SHORT haloce.0048AC1F
 0048ABE9   8BC7                 |MOV EAX,EDI ;->obj tag
 0048ABEB   25 FFFF0000          |AND EAX,0FFFF
 0048ABF0   8D1440               |LEA EDX,DWORD PTR DS:[EAX+EAX*2]
 0048ABF3   A1 B0B37F00          |MOV EAX,DWORD PTR DS:[7FB3B0]
 0048ABF8   8B48 34              |MOV ECX,DWORD PTR DS:[EAX+34]
 0048ABFB   8B4491 08            |MOV EAX,DWORD PTR DS:[ECX+EDX*4+8] ;obj->EAX=4005673C
 0048ABFF   8B40 04              |MOV EAX,DWORD PTR DS:[EAX+4] ;obj flag?->EAX=00000000
 0048AC02   85C0                 |TEST EAX,EAX
 0048AC04   74 09                |JE SHORT haloce.0048AC0F
 ...
 ;0048AC06   83F8 03              |CMP EAX,3
 ;0048AC09   75 14                |JNZ SHORT haloce.0048AC1F
 ;0048AC0B   85C0                 |TEST EAX,EAX
 ;0048AC0D   75 05                |JNZ SHORT haloce.0048AC14
 ...
 
 ;EDI obj tag
 0048AC0F   E8 1CEB0600          |CALL haloce.004F9730
 ->
  004F9730   83EC 0C            SUB ESP,0C
  004F9733   53                 PUSH EBX
  004F9734   33C0               XOR EAX,EAX;->EAX=00000000
  004F9736   83FF FF            CMP EDI,-1 ;EDI=E26F0000
  004F9739   56                 PUSH ESI
  004F973A   74 17              JE SHORT haloce.004F9753
  
  004F973C   8B35 F0266200      MOV ESI,DWORD PTR DS:[6226F0];->ESI=haloce.00622668
  004F9742   83C6 0C            ADD ESI,0C;->ESI=00622674
  004F9745   8BCF               MOV ECX,EDI;obj tag->ECX=E26F0000
  004F9747   E8 24ABFFFF        CALL haloce.004F4270
  ->
   004F4270   8A06               MOV AL,BYTE PTR DS:[ESI];->AL=01
   004F4272   57                 PUSH EDI
   004F4273   83CF FF            OR EDI,FFFFFFFF;->EDI=FFFFFFFF
   004F4276   3C 01              CMP AL,1
   004F4278   75 27              JNZ SHORT haloce.004F42A1
   004F427A   83F9 FF            CMP ECX,-1;ECX=E26F0000 -1?
   004F427D   74 22              JE SHORT haloce.004F42A1
   004F427F   85C9               TEST ECX,ECX;ECX=E26F0000 0?
   004F4281   8BC1               MOV EAX,ECX;->EAX=E26F0000
   004F4283   7D 02              JGE SHORT haloce.004F4287
   004F4285   F7D8               NEG EAX;->EAX=1D910000
   004F4287   99                 CDQ ;->EDX=00000000 "Convert double-word to quad-word" "	Sign-extends EAX into EDX, forming the quad-word EDX:EAX. Since (I)DIV uses EDX:EAX as its input, CDQ must be called after setting EAX if EDX is not manually initialized (as in 64/32 division) before (I)DIV."
   004F4288   F77E 04            IDIV DWORD PTR DS:[ESI+4];->EAX=003B2200
   004F428B   8B46 08            MOV EAX,DWORD PTR DS:[ESI+8];->EAX=0E2C7060
   004F428E   8B54D0 04          MOV EDX,DWORD PTR DS:[EAX+EDX*8+4];->EDX=110A8158 obj tag ptr
   004F4292   85D2               TEST EDX,EDX
   004F4294   74 0B              JE SHORT haloce.004F42A1
   004F4296   390A               /CMP DWORD PTR DS:[EDX],ECX ;[EDX]=E26F0000 ECX=E26F0000
   004F4298   74 0B              |JE SHORT haloce.004F42A5
   ...
   ;004F429A   8B52 08            |MOV EDX,DWORD PTR DS:[EDX+8]
   ;004F429D   85D2               |TEST EDX,EDX
   ;004F429F  ^75 F5              \JNZ SHORT haloce.004F4296
   ;004F42A1   8BC7               MOV EAX,EDI
   ;004F42A3   5F                 POP EDI
   ;004F42A4   C3                 RETN
   ...
   004F42A5   8B42 04            MOV EAX,DWORD PTR DS:[EDX+4] ;->EAX=00000001
   004F42A8   5F                 POP EDI
   004F42A9   C3                 RETN
  <-
  004F974C   83F8 FF            CMP EAX,-1
  004F974F   75 02              JNZ SHORT haloce.004F9753
  ...
  ;004F9751   33C0               XOR EAX,EAX
  ...
  004F9753   6A 00              PUSH 0
  004F9755   6A 01              PUSH 1
  004F9757   6A 00              PUSH 0
  004F9759   8D4C24 18          LEA ECX,DWORD PTR SS:[ESP+18];->ECX=0018CEA0
  004F975D   51                 PUSH ECX
  004F975E   894424 18          MOV DWORD PTR SS:[ESP+18],EAX ;save to ECX ptr
  004F9762   6A 00              PUSH 0
  004F9764   8D4424 1C          LEA EAX,DWORD PTR SS:[ESP+1C] ;->EAX=0018CE9C
  004F9768   894424 20          MOV DWORD PTR SS:[ESP+20],EAX
  004F976C   6A 00              PUSH 0
  004F976E   6A 00              PUSH 0
  004F9770   BA F87F0000        MOV EDX,7FF8
  004F9775   B8 20CE8000        MOV EAX,haloce.0080CE20
  004F977A   C74424 2C 00000000 MOV DWORD PTR SS:[ESP+2C],0
  004F9782   E8 496EFFFF        CALL haloce.004F05D0;big function
  004F9787   8BD8               MOV EBX,EAX;->EBX=00000010
  004F9789   8BC7               MOV EAX,EDI;->EAX=E26F0000
  004F978B   25 FFFF0000        AND EAX,0FFFF
  004F9790   8D1440             LEA EDX,DWORD PTR DS:[EAX+EAX*2]
  004F9793   A1 B0B37F00        MOV EAX,DWORD PTR DS:[7FB3B0]
  004F9798   8B48 34            MOV ECX,DWORD PTR DS:[EAX+34]
  004F979B   8A4491 02          MOV AL,BYTE PTR DS:[ECX+EDX*4+2] ;unknown in obj table->AL=23
  004F979F   83C4 1C            ADD ESP,1C
  
  004F97A2   A8 08              TEST AL,8
  004F97A4   75 0C              JNZ SHORT haloce.004F97B2
  004F97A6   8BF7               MOV ESI,EDI;->ESI=E26F0000
  
  004F97A8   B8 98266200        MOV EAX,haloce.00622698
  004F97AD   E8 FE41FFFF        CALL haloce.004ED9B0
  
  004F97B2   85DB               TEST EBX,EBX
  004F97B4   7E 1F              JLE SHORT haloce.004F97D5
  004F97B6   8B0D 84736B00      MOV ECX,DWORD PTR DS:[6B7384]            ; haloce.007FC340
  004F97BC   6A 03              PUSH 3
  004F97BE   6A 00              PUSH 0
  004F97C0   6A 00              PUSH 0
  004F97C2   6A 01              PUSH 1
  004F97C4   68 20CE8000        PUSH haloce.0080CE20
  004F97C9   6A 01              PUSH 1
  004F97CB   8BC3               MOV EAX,EBX
  004F97CD   E8 5EBAFEFF        CALL haloce.004E5230
  004F97D2   83C4 18            ADD ESP,18
  004F97D5   5E                 POP ESI
  004F97D6   5B                 POP EBX
  004F97D7   83C4 0C            ADD ESP,0C
  004F97DA   C3                 RETN
 <-
 0048AC14   6A 00                |PUSH 0
 0048AC16   57                   |PUSH EDI ;obj tag
 0048AC17   E8 44EA0600          |CALL haloce.004F9660
 ->
  004F9660   53                 PUSH EBX
  004F9661   8B5C24 08          MOV EBX,DWORD PTR SS:[ESP+8] ;first param obj tag->EBX=E26F0000
  004F9665   55                 PUSH EBP
  004F9666   8BC3               MOV EAX,EBX;->EAX=E26F0000
  004F9668   25 FFFF0000        AND EAX,0FFFF
  004F966D   56                 PUSH ESI
  004F966E   8D3440             LEA ESI,DWORD PTR DS:[EAX+EAX*2]
  004F9671   A1 B0B37F00        MOV EAX,DWORD PTR DS:[7FB3B0]
  004F9676   8B48 34            MOV ECX,DWORD PTR DS:[EAX+34]
  004F9679   C1E6 02            SHL ESI,2
  004F967C   8D2C31             LEA EBP,DWORD PTR DS:[ECX+ESI];ptr to first obj table->EBP=400506EC
  004F967F   57                 PUSH EDI
  004F9680   8B7D 08            MOV EDI,DWORD PTR SS:[EBP+8];obj->EDI=4005673C
  004F9683   8B87 18010000      MOV EAX,DWORD PTR DS:[EDI+118] ;WeaponTag->EAX=FFFFFFFF
  004F9689   83F8 FF            CMP EAX,-1;test if obj has weap
  004F968C   74 0B              JE SHORT haloce.004F9699
  ...
  ;recursive if obj has weap?
  ;004F968E   6A 01              PUSH 1
  ;004F9690   50                 PUSH EAX
  ;004F9691   E8 CAFFFFFF        CALL haloce.004F9660
  ;004F9696   83C4 08            ADD ESP,8
  ...
  004F9699   8A4424 18          MOV AL,BYTE PTR SS:[ESP+18];->AL=00
  004F969D   84C0               TEST AL,AL
  004F969F   74 16              JE SHORT haloce.004F96B7
  ...
  ;recursive if obj is gunner of veh
  ;004F96A1   8BBF 14010000      MOV EDI,DWORD PTR DS:[EDI+114]
  ;004F96A7   83FF FF            CMP EDI,-1
  ;004F96AA   74 0B              JE SHORT haloce.004F96B7
  ;004F96AC   6A 01              PUSH 1
  ;004F96AE   57                 PUSH EDI
  ;004F96AF   E8 ACFFFFFF        CALL haloce.004F9660
  ;004F96B4   83C4 08            ADD ESP,8
  ...
  004F96B7   8A45 02            MOV AL,BYTE PTR SS:[EBP+2];obj table unknown->AL=23
  004F96BA   8B15 B0B37F00      MOV EDX,DWORD PTR DS:[7FB3B0];"object"->EDX=400506B4
  004F96C0   0C 08              OR AL,8;->AL=2B
  004F96C2   8845 02            MOV BYTE PTR SS:[EBP+2],AL
  004F96C5   8B42 34            MOV EAX,DWORD PTR DS:[EDX+34]
  004F96C8   8B6C30 08          MOV EBP,DWORD PTR DS:[EAX+ESI+8];obj 0->EBP=4005673C
  004F96CC   8B4D 00            MOV ECX,DWORD PTR SS:[EBP];BipdMetaTag->ECX=E84A06D6
  004F96CF   8B15 E46D8100      MOV EDX,DWORD PTR DS:[816DE4];model array?->EDX=40440028
  004F96D5   81E1 FFFF0000      AND ECX,0FFFF
  004F96DB   C1E1 05            SHL ECX,5
  004F96DE   8B4411 14          MOV EAX,DWORD PTR DS:[ECX+EDX+14];obj model?->EAX=40886FA0
  004F96E2   8378 34 FF         CMP DWORD PTR DS:[EAX+34],-1 ;unknown tag E84B06D7 
  004F96E6   74 14              JE SHORT haloce.004F96FC
  004F96E8   F645 10 01         TEST BYTE PTR SS:[EBP+10],1 ;BitFlags_01 = 20h
  004F96EC   75 0E              JNZ SHORT haloce.004F96FC
  
  ;destroy obj function?
  004F96EE   6A 00              PUSH 0
  004F96F0   6A 01              PUSH 1
  004F96F2   8BC3               MOV EAX,EBX;obj tag->EAX=E26F0000
  004F96F4   E8 B73F0000        CALL haloce.004FD6B0
  004F96F9   83C4 08            ADD ESP,8

  004F96FC   8B0D B0B37F00      MOV ECX,DWORD PTR DS:[7FB3B0]
  004F9702   8B41 34            MOV EAX,DWORD PTR DS:[ECX+34]
  004F9705   8B55 10            MOV EDX,DWORD PTR SS:[EBP+10];BitFlags_01->EDX=00006820
  004F9708   03C6               ADD EAX,ESI
  004F970A   83CA 01            OR EDX,1;->EDX=00006821
  004F970D   8955 10            MOV DWORD PTR SS:[EBP+10],EDX
  004F9710   8A48 02            MOV CL,BYTE PTR DS:[EAX+2]
  004F9713   80E1 FD            AND CL,0FD
  004F9716   8BFB               MOV EDI,EBX
  004F9718   8848 02            MOV BYTE PTR DS:[EAX+2],CL
  004F971B   E8 70400000        CALL haloce.004FD790
  004F9720   5F                 POP EDI
  004F9721   5E                 POP ESI
  004F9722   5D                 POP EBP
  004F9723   5B                 POP EBX
  004F9724   C3                 RETN
 <-
 0048AC1C   83C4 08              |ADD ESP,8
 
 0048AC1F   8D5424 08            |LEA EDX,DWORD PTR SS:[ESP+8]
 
 ;get next obj to destroy
 0048AC23   52                   |PUSH EDX
 0048AC24   E8 87FF0600          |CALL haloce.004FABB0
 0048AC29   83C4 04              |ADD ESP,4
 
 0048AC2C   85C0                 |TEST EAX,EAX
 0048AC2E  ^75 A0                \JNZ SHORT haloce.0048ABD0
 0048AC30   5F                   POP EDI
 0048AC31   5D                   POP EBP
 0048AC32   83C4 10              ADD ESP,10
 0048AC35   C3                   RETN
<-
0047D575   8B4C24 08        MOV ECX,DWORD PTR SS:[ESP+8]
0047D579   33C0             XOR EAX,EAX
0047D57B   E9 10FE0000      JMP haloce.0048D390