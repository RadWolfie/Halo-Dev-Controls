;notes by Jesus7Freak
;haloce1.09 map=ui dev=enabled console buffer="obj"

haloce.00624118;dev fucntion info struct array pointer
struct ;size 0x1C or 0x20
{
   DWORD Unknown_num; 3
   char *function_name;
   DWORD Unknown_function_address;
   void (__cdecl *script_function_address)(int)(int script_tag)(int); //0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C]
   DWORD *Unknown;
   BYTE *dev_relative;
   WORD Unknown;
   WORD num_of_parameters;
   DWORD extra;
};



;displays cmds in console when tab is pressed
004C9C10   E8 9B040000      CALL haloce.004CA0B0
->
...

;console buffer = "obj"
004CA103   8B0D 44CD6800    MOV ECX,DWORD PTR DS:[68CD44]
004CA109   0FBF05 D0476B00  MOVSX EAX,WORD PTR DS:[6B47D0]
004CA110   33D2             XOR EDX,EDX
004CA112   85C9             TEST ECX,ECX
004CA114   74 14            JE SHORT haloce.004CA12A
;004CA116   83F8 01          CMP EAX,1
;004CA119   0F85 EF000000    JNZ haloce.004CA20E
;004CA11F   BA 11000000      MOV EDX,11
;004CA124   81E2 FDFF0000    AND EDX,0FFFD
004CA12A   8D8424 28040000  LEA EAX,DWORD PTR SS:[ESP+428] ;->EAX=0018D840 pointer to stack to store the string pointers
004CA131   50               PUSH EAX
004CA132   83CA 20          OR EDX,20 ;->EDX=00000020
004CA135   6A 28            PUSH 28 ;limit? (40) string pointers, no its used for functions calling
004CA137   B9 00010000      MOV ECX,100;this may be the limit 256 strings
004CA13C   8BC3             MOV EAX,EBX;->EAX=00652028 "obj"  console buffer pointer (filter string?)
004CA13E   E8 9DC8FBFF      CALL haloce.004869E0;copies string pointers to stack
->
 004869E0   53               PUSH EBX   ; haloce.00652028
 004869E1   55               PUSH EBP
 004869E2   8B6C24 10        MOV EBP,DWORD PTR SS:[ESP+10];->EBX=0018D840 pointer to stack space to store string pointers
 004869E6   56               PUSH ESI
 004869E7   33F6             XOR ESI,ESI
 004869E9   3BC6             CMP EAX,ESI;valid string pointer
 ;hook vars sig
 ;\x57\x66\x89\x35\x00\xC4\x64\x00\x66\x89\x0D
 ;xxxx????xxx
 004869EB   57               PUSH EDI
 004869EC   66:8935 00C46400 MOV WORD PTR DS:[64C400],SI;array of strings index
 004869F3   66:890D F0C36400 MOV WORD PTR DS:[64C3F0],CX;array size?
 004869FA   892D 04C46400    MOV DWORD PTR DS:[64C404],EBP;pointer to the array
 00486A00   A3 F4C36400      MOV DWORD PTR DS:[64C3F4],EAX;pointer to the filter string (ex: obj, search for functions that have obj in them)
 00486A05   75 0A            JNZ SHORT haloce.00486A11
 ;00486A07   C705 F4C36400 3C365F00    MOV DWORD PTR DS:[64C3F4],haloce.005F363C
 00486A11   66:8915 FCC36400 MOV WORD PTR DS:[64C3FC],DX
 00486A18   BF 60496200      MOV EDI,haloce.00624960 ;pointer to array of functions
 00486A1D   BB 12000000      MOV EBX,12
 00486A22   8BCE             /MOV ECX,ESI;->ECX=00000000
 00486A24   B8 01000000      |MOV EAX,1
 00486A29   D3E0             |SHL EAX,CL
 00486A2B   8B4C24 14        |MOV ECX,DWORD PTR SS:[ESP+14];->ECX=00000028
 00486A2F   85C1             |TEST ECX,EAX
 00486A31   74 02            |JE SHORT haloce.00486A35
 00486A33   FF17             |CALL DWORD PTR DS:[EDI];when EBX=0F, or EBX=0D
 1->;when EBX=0F
  00486611   55                        PUSH EBP
  00486612   56                        PUSH ESI
  00486613   57                        PUSH EDI                                        ; haloce.0062496C
  00486614   BB 18416200               MOV EBX,haloce.00624118;dev fucntion info struct array pointer
  00486619   BD 11020000               MOV EBP,211;number of console functions
  0048661E   8BFF                      MOV EDI,EDI
  00486620   8B33                      MOV ESI,DWORD PTR DS:[EBX];->ESI=005F5B70
  00486622   8A4E 18                   MOV CL,BYTE PTR DS:[ESI+18]
  00486625   E8 16FDFFFF               CALL haloce.00486340;dev check function?
  ->
   00486340   8AD1                      MOV DL,CL
   00486342   C0E9 03                   SHR CL,3
   00486345   56                        PUSH ESI
   00486346   66:0FB6F1                 MOVZX SI,CL
   0048634A   8A0D 7ED16B00             MOV CL,BYTE PTR DS:[6BD17E];dev check address
   00486350   80E2 07                   AND DL,7
   00486353   84C9                      TEST CL,CL
   00486355   B0 01                     MOV AL,1
   00486357   66:0FB6D2                 MOVZX DX,DL
   0048635B   0F85 87000000             JNZ haloce.004863E8
   ;00486361   8A0D FCC36400             MOV CL,BYTE PTR DS:[64C3FC]
   ;00486367   F6C1 20                   TEST CL,20
   ;0048636A   0FB7C2                    MOVZX EAX,DX
   ;0048636D   74 18                     JE SHORT haloce.00486387
   ;0048636F   85C0                      TEST EAX,EAX
   ;00486371   7E 39                     JLE SHORT haloce.004863AC
   ;00486373   83F8 06                   CMP EAX,6
   ;00486376   7E 6E                     JLE SHORT haloce.004863E6
   ;00486378   83F8 07                   CMP EAX,7
   ;0048637B   75 2F                     JNZ SHORT haloce.004863AC
   ;0048637D   C0E9 03                   SHR CL,3
   ;00486380   80E1 01                   AND CL,1
   ;00486383   8AC1                      MOV AL,CL
   ;00486385   5E                        POP ESI
   ;00486386   C3                        RETN
   ;00486387   48                        DEC EAX                                         ; Switch (cases 1..7)
   ;00486388   83F8 06                   CMP EAX,6
   ;0048638B   77 1F                     JA SHORT haloce.004863AC
   ;0048638D   FF2485 EC634800           JMP DWORD PTR DS:[EAX*4+4863EC]
   ;00486394   F6C1 07                   TEST CL,7                                       ; Case 2 of switch 00486387
   ;00486397   75 29                     JNZ SHORT haloce.004863C2
   ;00486399   32C0                      XOR AL,AL
   ;0048639B   5E                        POP ESI
   ;0048639C   C3                        RETN
   ;0048639D   8AC1                      MOV AL,CL                                       ; Case 3 of switch 00486387
   ;0048639F   EB 1B                     JMP SHORT haloce.004863BC
   ;004863A1   8AC1                      MOV AL,CL                                       ; Case 4 of switch 00486387
   ;004863A3   F6D0                      NOT AL
   ;004863A5   EB 15                     JMP SHORT haloce.004863BC
   ;004863A7   F6C1 06                   TEST CL,6                                       ; Case 5 of switch 00486387
   ;004863AA   75 16                     JNZ SHORT haloce.004863C2
   ;004863AC   32C0                      XOR AL,AL                                       ; Default case of switch 004863C5
   ;004863AE   5E                        POP ESI
   ;004863AF   C3                        RETN
   ;004863B0   8AC1                      MOV AL,CL                                       ; Case 6 of switch 00486387
   ;004863B2   C0E8 02                   SHR AL,2
   ;004863B5   EB 05                     JMP SHORT haloce.004863BC
   ;004863B7   8AC1                      MOV AL,CL                                       ; Case 7 of switch 00486387
   ;004863B9   C0E8 03                   SHR AL,3
   ;004863BC   24 01                     AND AL,1
   ;004863BE   84C0                      TEST AL,AL
   ;004863C0   74 26                     JE SHORT haloce.004863E8
   ;004863C2   0FB7C6                    MOVZX EAX,SI                                    ; Case 1 of switch 00486387
   ;004863C5   83E8 00                   SUB EAX,0                                       ; Switch (cases 0..2)
   ;004863C8   74 1C                     JE SHORT haloce.004863E6
   ;004863CA   48                        DEC EAX
   ;004863CB   74 0D                     JE SHORT haloce.004863DA
   ;004863CD   48                        DEC EAX
   ;004863CE  ^75 DC                     JNZ SHORT haloce.004863AC
   ;004863D0   C0E9 04                   SHR CL,4                                        ; Case 2 of switch 004863C5
   ;004863D3   80E1 01                   AND CL,1
   ;004863D6   8AC1                      MOV AL,CL
   ;004863D8   5E                        POP ESI
   ;004863D9   C3                        RETN
   ;004863DA   C0E9 04                   SHR CL,4                                        ; Case 1 of switch 004863C5
   ;004863DD   F6D1                      NOT CL
   ;004863DF   80E1 01                   AND CL,1
   ;004863E2   8AC1                      MOV AL,CL
   ;004863E4   5E                        POP ESI
   ;004863E5   C3                        RETN
   ;004863E6   B0 01                     MOV AL,1                                        ; Case 0 of switch 004863C5
   004863E8   5E                        POP ESI
   004863E9   C3                        RETN
  <-
  0048662A   84C0                      TEST AL,AL;AL=01
  0048662C   74 54                     JE SHORT haloce.00486682
  0048662E   66:A1 00C46400            MOV AX,WORD PTR DS:[64C400];->AX=0000
  00486634   66:3B05 F0C36400          CMP AX,WORD PTR DS:[64C3F0];[64C3F0]=0100 ;index within array bounds?
  0048663B   8B76 04                   MOV ESI,DWORD PTR DS:[ESI+4];->ESI=006048EC "begin"
  0048663E   7D 42                     JGE SHORT haloce.00486682
  00486640   8B15 F4C36400             MOV EDX,DWORD PTR DS:[64C3F4]; haloce.00652028 (console buffer) "obj"
  00486646   8BC2                      MOV EAX,EDX ;->EAX=00652028 "obj"
  00486648   8D78 01                   LEA EDI,DWORD PTR DS:[EAX+1];EDI=00652029 "bj"
  0048664B   EB 03                     JMP SHORT haloce.00486650
  ;0048664D   8D49 00                   LEA ECX,DWORD PTR DS:[ECX]
  
  00486650   8A08                      MOV CL,BYTE PTR DS:[EAX];->CL=6F
  00486652   40                        INC EAX
  00486653   84C9                      TEST CL,CL;test format string
  00486655  ^75 F9                     JNZ SHORT haloce.00486650
  
  00486657   2BC7                      SUB EAX,EDI;EAX=00000003
  
  00486659   50                        PUSH EAX;format str length
  0048665A   52                        PUSH EDX;format str
  0048665B   56                        PUSH ESI;cmd str
  0048665C   E8 8C621500               CALL haloce.005DC8ED
  00486661   83C4 0C                   ADD ESP,0C
  
  00486664   85C0                      TEST EAX,EAX;FFFFFFF3 (-13)
  00486666   75 1A                     JNZ SHORT haloce.00486682
  ;when match is found
  00486668   66:A1 00C46400            MOV AX,WORD PTR DS:[64C400];index to the string array
  0048666E   8B15 04C46400             MOV EDX,DWORD PTR DS:[64C404];pointer to the stack, were the matching strings are stored
  00486674   0FBFC8                    MOVSX ECX,AX
  00486677   66:40                     INC AX
  00486679   89348A                    MOV DWORD PTR DS:[EDX+ECX*4],ESI
  0048667C   66:A3 00C46400            MOV WORD PTR DS:[64C400],AX
  
  00486682   83C3 04                   ADD EBX,4 ;next cmd struct in array
  00486685   4D                        DEC EBP ;cmds left
  00486686  ^75 98                     JNZ SHORT haloce.00486620
  
  00486688   5F                        POP EDI
  00486689   5E                        POP ESI
  0048668A   5D                        POP EBP
  0048668B   5B                        POP EBX
  0048668C   C3                        RETN
 <-
 2->;when EBX=0D
  004866C0   53                       PUSH EBX
  004866C1   55                       PUSH EBP
  004866C2   56                       PUSH ESI
  004866C3   57                       PUSH EDI
  004866C4   BB 88696200              MOV EBX,haloce.00626988;rasterizer functions
  004866C9   BD EC010000              MOV EBP,1EC ;num of rasterizer functions?
  004866CE   8BFF                     MOV EDI,EDI
  004866D0   8B33                     MOV ESI,DWORD PTR DS:[EBX];->ESI=00624C28
  
  004866D2   8A4E 0C                  MOV CL,BYTE PTR DS:[ESI+C];->CL=00
  004866D5   E8 66FCFFFF              CALL haloce.00486340;dev check function? (dev is enabled)
  
  004866DA   84C0                     TEST AL,AL;AL=01
  004866DC   74 54                    JE SHORT haloce.00486732
  004866DE   66:A1 00C46400           MOV AX,WORD PTR DS:[64C400];index ->AX=001E
  004866E4   66:3B05 F0C36400         CMP AX,WORD PTR DS:[64C3F0];[64C3F0]=0100 max num of strings?
  004866EB   8B36                     MOV ESI,DWORD PTR DS:[ESI]
  004866ED   7D 43                    JGE SHORT haloce.00486732
  004866EF   8B15 F4C36400            MOV EDX,DWORD PTR DS:[64C3F4] ; haloce.00652028 console buffer
  004866F5   8BC2                     MOV EAX,EDX
  004866F7   8D78 01                  LEA EDI,DWORD PTR DS:[EAX+1]
  004866FA   8D9B 00000000            LEA EBX,DWORD PTR DS:[EBX];filler????????? EBX=EBX
  00486700   8A08                     MOV CL,BYTE PTR DS:[EAX]
  00486702   40                       INC EAX
  00486703   84C9                     TEST CL,CL
  00486705  ^75 F9                    JNZ SHORT haloce.00486700
  00486707   2BC7                     SUB EAX,EDI
  00486709   50                       PUSH EAX
  0048670A   52                       PUSH EDX
  0048670B   56                       PUSH ESI
  0048670C   E8 DC611500              CALL haloce.005DC8ED;string compare function
  00486711   83C4 0C                  ADD ESP,0C
  00486714   85C0                     TEST EAX,EAX
  00486716   75 1A                    JNZ SHORT haloce.00486732
  00486718   66:A1 00C46400           MOV AX,WORD PTR DS:[64C400]
  0048671E   8B15 04C46400            MOV EDX,DWORD PTR DS:[64C404]
  00486724   0FBFC8                   MOVSX ECX,AX
  00486727   66:40                    INC AX
  00486729   89348A                   MOV DWORD PTR DS:[EDX+ECX*4],ESI
  0048672C   66:A3 00C46400           MOV WORD PTR DS:[64C400],AX
  00486732   83C3 04                  ADD EBX,4
  00486735   4D                       DEC EBP
  00486736  ^75 98                    JNZ SHORT haloce.004866D0
  00486738   833D DC976300 FF         CMP DWORD PTR DS:[6397DC],-1;DS:[006397DC]=E1740000 (unknown tag)
  0048673F   74 18                    JE SHORT haloce.00486759
  
  00486741   8B3D 6C226E00            MOV EDI,DWORD PTR DS:[6E226C] ;->EDI=40464578 "psbs"
  00486747   6A 5C                    PUSH 5C
  00486749   6A 00                    PUSH 0
  0048674B   81C7 A8040000            ADD EDI,4A8
  00486751   E8 9AFDFFFF              CALL haloce.004864F0
  ->
   004864F0   51                       PUSH ECX
   004864F1   8B0F                     MOV ECX,DWORD PTR DS:[EDI];->ECX=00000001
   004864F3   33C0                     XOR EAX,EAX
   004864F5   3BC8                     CMP ECX,EAX;1=0?
   004864F7   890424                   MOV DWORD PTR SS:[ESP],EAX
   004864FA   0F8E AB000000            JLE haloce.004865AB
   00486500   53                       PUSH EBX
   00486501   55                       PUSH EBP
   00486502   0FBF6C24 10              MOVSX EBP,WORD PTR SS:[ESP+10]
   00486507   56                       PUSH ESI
   00486508   EB 06                    JMP SHORT haloce.00486510
   0048650A   8D9B 00000000            LEA EBX,DWORD PTR DS:[EBX]
   00486510   0FAF4424 18              /IMUL EAX,DWORD PTR SS:[ESP+18]
   00486515   0347 04                  |ADD EAX,DWORD PTR DS:[EDI+4];1st->EAX=404C2FB0 "startloop"
   00486518   8D1C28                   |LEA EBX,DWORD PTR DS:[EAX+EBP];1st->EBX=404C2FB0 "startloop"
   0048651B   E8 D0FCFFFF              |CALL haloce.004861F0
   00486520   66:3D FFFF               |CMP AX,0FFFF
   00486524   74 1E                    |JE SHORT haloce.00486544
   00486526   84E4                     |TEST AH,AH
   00486528   79 68                    |JNS SHORT haloce.00486592
   0048652A   0FBFC0                   |MOVSX EAX,AX
   0048652D   8B0485 88696200          |MOV EAX,DWORD PTR DS:[EAX*4+626988]
   00486534   85C0                     |TEST EAX,EAX
   00486536   74 5A                    |JE SHORT haloce.00486592
   00486538   8A48 0C                  |MOV CL,BYTE PTR DS:[EAX+C]
   0048653B   E8 00FEFFFF              |CALL haloce.00486340
   00486540   84C0                     |TEST AL,AL
   00486542   74 4E                    |JE SHORT haloce.00486592
   00486544   66:8B0D 00C46400         |MOV CX,WORD PTR DS:[64C400]
   0048654B   66:3B0D F0C36400         |CMP CX,WORD PTR DS:[64C3F0]
   00486552   7D 3E                    |JGE SHORT haloce.00486592
   00486554   8B15 F4C36400            |MOV EDX,DWORD PTR DS:[64C3F4]           ; haloce.00652028
   0048655A   8BC2                     |MOV EAX,EDX
   0048655C   8D70 01                  |LEA ESI,DWORD PTR DS:[EAX+1]
   0048655F   90                       |NOP
   00486560   8A08                     |/MOV CL,BYTE PTR DS:[EAX]
   00486562   40                       ||INC EAX
   00486563   84C9                     ||TEST CL,CL
   00486565  ^75 F9                    |\JNZ SHORT haloce.00486560
   00486567   2BC6                     |SUB EAX,ESI
   00486569   50                       |PUSH EAX
   0048656A   52                       |PUSH EDX
   0048656B   53                       |PUSH EBX
   0048656C   E8 7C631500              |CALL haloce.005DC8ED
   00486571   83C4 0C                  |ADD ESP,0C
   00486574   85C0                     |TEST EAX,EAX
   00486576   75 1A                    |JNZ SHORT haloce.00486592
   00486578   66:A1 00C46400           |MOV AX,WORD PTR DS:[64C400]
   0048657E   8B0D 04C46400            |MOV ECX,DWORD PTR DS:[64C404]
   00486584   0FBFD0                   |MOVSX EDX,AX
   00486587   66:40                    |INC AX
   00486589   891C91                   |MOV DWORD PTR DS:[ECX+EDX*4],EBX
   0048658C   66:A3 00C46400           |MOV WORD PTR DS:[64C400],AX
   00486592   8B4424 0C                |MOV EAX,DWORD PTR SS:[ESP+C]
   00486596   8B0F                     |MOV ECX,DWORD PTR DS:[EDI]
   00486598   40                       |INC EAX
   00486599   894424 0C                |MOV DWORD PTR SS:[ESP+C],EAX
   0048659D   0FBFC0                   |MOVSX EAX,AX
   004865A0   3BC1                     |CMP EAX,ECX
   004865A2  ^0F8C 68FFFFFF            \JL haloce.00486510
   004865A8   5E                       POP ESI
   004865A9   5D                       POP EBP
   004865AA   5B                       POP EBX
   004865AB   59                       POP ECX
   004865AC   C3                       RETN;AL=01
  <-
  00486756   83C4 08                  ADD ESP,8
  
  00486759   5F                       POP EDI
  0048675A   5E                       POP ESI
  0048675B   5D                       POP EBP
  0048675C   5B                       POP EBX
  0048675D   C3                       RETN
 <-
 00486A35   46               |INC ESI
 00486A36   83C7 04          |ADD EDI,4 ;next function in array of functions
 00486A39   4B               |DEC EBX
 00486A3A  ^75 E6            \JNZ SHORT haloce.00486A22
 00486A3C   0FBF15 00C46400  MOVSX EDX,WORD PTR DS:[64C400];index to the string array->EDX=0023
 00486A43   68 20634800      PUSH haloce.00486320
 ;hook sig
 ;\x6A\x04\x52\x55\xE8\x0F\x1E\x14\x00\x66\xA1
 ;xxxxx????xx
 00486A48   6A 04            PUSH 4
 00486A4A   52               PUSH EDX
 00486A4B   55               PUSH EBP;0018D840 array of strings
 ;registers when paused:
 ;EAX=00020000 (loop var)
 ;ECX=00000028 (loop var)
 ;EDX=00000023 (index so far at this point)
 ;EBX=00000000 (loop down counter and check)
 ;ESP=0018D3EC (stack pointing to array of strings ptr)
 ;EBP=0018D840  (array of strings ptr)
 ;ESI=00000012 (loop up counter)
 ;EDI=006249A8 haloce.006249A8 (array of functions)
 ;EIP=00486A4C haloce.00486A4C
 00486A4C   E8 0F1E1400      CALL haloce.005C8860;misc dev funtions?(huge function)(hook here)
 00486A51   66:A1 00C46400   MOV AX,WORD PTR DS:[64C400];EAX=-1->AX=0023
 00486A57   83C4 10          ADD ESP,10
 00486A5A   5F               POP EDI
 00486A5B   5E               POP ESI
 00486A5C   5D               POP EBP
 00486A5D   C705 04C46400 00000000 MOV DWORD PTR DS:[64C404],0;set array of strings pointer to 0
 
 00486A67   5B               POP EBX
 00486A68   C3               RETN;ret the num of matching cmds found
<-
004CA143   83C4 08          ADD ESP,8

0018D840 00601860 ;"cls" pointer to array of string pointers

004C9DE0   A0 701F6500      MOV AL,BYTE PTR DS:[651F70] ;1
004C9DE5   81EC 00040000    SUB ESP,400
004C9DEB   84C0             TEST AL,AL
004C9DED   0F84 97000000    JE haloce.004C9E8A
004C9DF3   8A8424 04040000  MOV AL,BYTE PTR SS:[ESP+404] ;->AL=00
004C9DFA   84C0             TEST AL,AL
004C9DFC   74 2A            JE SHORT haloce.004C9E28
;004C9DFE   A0 4CDE6400      MOV AL,BYTE PTR DS:[64DE4C]
;004C9E03   84C0             TEST AL,AL
;004C9E05   74 21            JE SHORT haloce.004C9E28
;004C9E07   B8 FFFFFFFF      MOV EAX,-1
;004C9E0C   56               PUSH ESI
;004C9E0D   8B35 50DE6400    MOV ESI,DWORD PTR DS:[64DE50]
;004C9E13   A3 54DE6400      MOV DWORD PTR DS:[64DE54],EAX
;004C9E18   A3 58DE6400      MOV DWORD PTR DS:[64DE58],EAX
;004C9E1D   E8 9E9C0000      CALL haloce.004D3AC0
;004C9E22   E8 9902FDFF      CALL haloce.0049A0C0
;004C9E27   5E               POP ESI
004C9E28   8B8C24 08040000  MOV ECX,DWORD PTR SS:[ESP+408];->ECX=005F363C
004C9E2F   8D8424 0C040000  LEA EAX,DWORD PTR SS:[ESP+40C];->EAX=0018D418
004C9E36   50               PUSH EAX
004C9E37   51               PUSH ECX
004C9E38   8D5424 08        LEA EDX,DWORD PTR SS:[ESP+8];->EDX=0018D00C
004C9E3C   52               PUSH EDX
004C9E3D   E8 410E1000      CALL haloce.005CAC83

004C9E42   8D4424 0C        LEA EAX,DWORD PTR SS:[ESP+C];->EAX=0018D00C
004C9E46   50               PUSH EAX
004C9E47   68 F4D55F00      PUSH haloce.005FD5F4                     ; ASCII "%s"
004C9E4C   33C0             XOR EAX,EAX
004C9E4E   C68424 13010000 00 MOV BYTE PTR SS:[ESP+113],0
004C9E56   E8 25FEFCFF      CALL haloce.00499C80
->

<-
004C9E5B   A0 83476B00      MOV AL,BYTE PTR DS:[6B4783]
004C9E60   83C4 14          ADD ESP,14
004C9E63   84C0             TEST AL,AL
004C9E65   74 23            JE SHORT haloce.004C9E8A
004C9E67   68 00040000      PUSH 400
004C9E6C   8D4C24 04        LEA ECX,DWORD PTR SS:[ESP+4]
004C9E70   68 18D65F00      PUSH haloce.005FD618                     ; ASCII ""
004C9E75   51               PUSH ECX
004C9E76   E8 F5101000      CALL haloce.005CAF70
004C9E7B   8D5424 0C        LEA EDX,DWORD PTR SS:[ESP+C]
004C9E7F   6A 01            PUSH 1
004C9E81   52               PUSH EDX
004C9E82   E8 5901F8FF      CALL haloce.00449FE0
004C9E87   83C4 14          ADD ESP,14
004C9E8A   81C4 00040000    ADD ESP,400
004C9E90   C3               RETN