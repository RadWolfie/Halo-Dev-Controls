;notes by Jesus7Freak
;console enabled but not open

004C9B40   A0 711F6500      MOV AL,BYTE PTR DS:[651F71] ;console enabled?
004C9B45   53               PUSH EBX
004C9B46   33DB             XOR EBX,EBX
004C9B48   3AC3             CMP AL,BL
004C9B4A   0F84 6C010000    JE haloce.004C9CBC
004C9B50   381D A8E76400    CMP BYTE PTR DS:[64E7A8],BL
004C9B56   0F85 60010000    JNZ haloce.004C9CBC
004C9B5C   803D 20D86A00 01 CMP BYTE PTR DS:[6AD820],1
004C9B63   75 0C            JNZ SHORT haloce.004C9B71
;004C9B65   E8 46FFFFFF      CALL haloce.004C9AB0
;004C9B6A   A0 701F6500      MOV AL,BYTE PTR DS:[651F70] ;console open?
;004C9B6F   5B               POP EBX
;004C9B70   C3               RETN
004C9B71   A0 701F6500      MOV AL,BYTE PTR DS:[651F70]
004C9B76   3AC3             CMP AL,BL
004C9B78   0F84 43010000    JE haloce.004C9CC1
;004C9B7E   6A 02            PUSH 2
;004C9B80   E8 FBA1FCFF      CALL haloce.00493D80
;004C9B85   83C4 04          ADD ESP,4
;004C9B88   3C 01            CMP AL,1
;004C9B8A   75 05            JNZ SHORT haloce.004C9B91
;004C9B8C   E8 5FFFFFFF      CALL haloce.004C9AF0
;004C9B91   55               PUSH EBP
;004C9B92   33ED             XOR EBP,EBP
;004C9B94   66:391D 741F6500 CMP WORD PTR DS:[651F74],BX
;004C9B9B   0F8E 1A010000    JLE haloce.004C9CBB
;004C9BA1   56               PUSH ESI
;004C9BA2   57               PUSH EDI
;004C9BA3   EB 0B            JMP SHORT haloce.004C9BB0
;004C9BA5   8DA424 00000000  LEA ESP,DWORD PTR SS:[ESP]
;004C9BAC   8D6424 00        LEA ESP,DWORD PTR SS:[ESP]
;004C9BB0   0FBFC5           MOVSX EAX,BP
;004C9BB3   0FBF0485 781F650>MOVSX EAX,WORD PTR DS:[EAX*4+651F78]
;004C9BBB   83C0 FA          ADD EAX,-6                               ; Switch (cases 6..66)
;004C9BBE   83F8 60          CMP EAX,60
;004C9BC1   0F87 E4000000    JA haloce.004C9CAB
;004C9BC7   0FB688 DC9C4C00  MOVZX ECX,BYTE PTR DS:[EAX+4C9CDC]
;004C9BCE   FF248D C49C4C00  JMP DWORD PTR DS:[ECX*4+4C9CC4]
;004C9BD5   381D 28206500    CMP BYTE PTR DS:[652028],BL              ; Cases 38,66 of switch 004C9BBB
;004C9BDB   74 29            JE SHORT haloce.004C9C06
;004C9BDD   53               PUSH EBX
;004C9BDE   BF 28206500      MOV EDI,haloce.00652028
;004C9BE3   E8 58030000      CALL haloce.004C9F40
;004C9BE8   83C4 04          ADD ESP,4
;004C9BEB   881D 28206500    MOV BYTE PTR DS:[652028],BL
;004C9BF1   66:891D 2E216500 MOV WORD PTR DS:[65212E],BX
;004C9BF8   66:C705 30216500>MOV WORD PTR DS:[652130],0FFFF
;004C9C01   E9 A5000000      JMP haloce.004C9CAB
;004C9C06   E8 25FEFFFF      CALL haloce.004C9A30
;004C9C0B   E9 9B000000      JMP haloce.004C9CAB
;004C9C10   E8 9B040000      CALL haloce.004CA0B0
;004C9C15   E9 91000000      JMP haloce.004C9CAB
;004C9C1A   66:8305 30296500>ADD WORD PTR DS:[652930],2               ; Case 4D of switch 004C9BBB
;004C9C22   66:A1 30296500   MOV AX,WORD PTR DS:[652930]              ; Case 4E of switch 004C9BBB
;004C9C28   66:48            DEC AX
;004C9C2A   33C9             XOR ECX,ECX
;004C9C2C   66:3BC3          CMP AX,BX
;004C9C2F   0F9EC1           SETLE CL
;004C9C32   66:A3 30296500   MOV WORD PTR DS:[652930],AX
;004C9C38   8B35 30296500    MOV ESI,DWORD PTR DS:[652930]
;004C9C3E   0FBF05 2C296500  MOVSX EAX,WORD PTR DS:[65292C]
;004C9C45   49               DEC ECX
;004C9C46   23CE             AND ECX,ESI
;004C9C48   0FBFC9           MOVSX ECX,CX
;004C9C4B   48               DEC EAX
;004C9C4C   3BC8             CMP ECX,EAX
;004C9C4E   7F 03            JG SHORT haloce.004C9C53
;004C9C50   66:8BC1          MOV AX,CX
;004C9C53   66:3D FFFF       CMP AX,0FFFF
;004C9C57   66:A3 30296500   MOV WORD PTR DS:[652930],AX
;004C9C5D   74 4C            JE SHORT haloce.004C9CAB
;004C9C5F   0FBF15 2E296500  MOVSX EDX,WORD PTR DS:[65292E]
;004C9C66   0FBFC0           MOVSX EAX,AX
;004C9C69   2BD0             SUB EDX,EAX
;004C9C6B   83C2 08          ADD EDX,8
;004C9C6E   8BC2             MOV EAX,EDX
;004C9C70   25 07000080      AND EAX,80000007
;004C9C75   79 05            JNS SHORT haloce.004C9C7C
;004C9C77   48               DEC EAX
;004C9C78   83C8 F8          OR EAX,FFFFFFF8
;004C9C7B   40               INC EAX
;004C9C7C   69C0 FF000000    IMUL EAX,EAX,0FF
;004C9C82   05 34216500      ADD EAX,haloce.00652134
;004C9C87   BA 28206500      MOV EDX,haloce.00652028
;004C9C8C   8D6424 00        LEA ESP,DWORD PTR SS:[ESP]
;004C9C90   8A08             MOV CL,BYTE PTR DS:[EAX]
;004C9C92   40               INC EAX
;004C9C93   880A             MOV BYTE PTR DS:[EDX],CL
;004C9C95   42               INC EDX
;004C9C96   3ACB             CMP CL,BL
;004C9C98  ^75 F6            JNZ SHORT haloce.004C9C90
;004C9C9A   BE 28216500      MOV ESI,haloce.00652128
;004C9C9F   E8 8C34F8FF      CALL haloce.0044D130
;004C9CA4   EB 05            JMP SHORT haloce.004C9CAB
;004C9CA6   E8 45FEFFFF      CALL haloce.004C9AF0                     ; Case 6 of switch 004C9BBB
;004C9CAB   45               INC EBP                                  ; Default case of switch 004C9BBB
;004C9CAC   66:3B2D 741F6500 CMP BP,WORD PTR DS:[651F74]
;004C9CB3  ^0F8C F7FEFFFF    JL haloce.004C9BB0
;004C9CB9   5F               POP EDI
;004C9CBA   5E               POP ESI
;004C9CBB   5D               POP EBP
;004C9CBC   A0 701F6500      MOV AL,BYTE PTR DS:[651F70]
004C9CC1   5B               POP EBX
004C9CC2   C3               RETN


;console enabled. and open, but empty

004C9B40   A0 711F6500      MOV AL,BYTE PTR DS:[651F71]
004C9B45   53               PUSH EBX
004C9B46   33DB             XOR EBX,EBX
004C9B48   3AC3             CMP AL,BL
004C9B4A   0F84 6C010000    JE haloce.004C9CBC
004C9B50   381D A8E76400    CMP BYTE PTR DS:[64E7A8],BL
004C9B56   0F85 60010000    JNZ haloce.004C9CBC
004C9B5C   803D 20D86A00 01 CMP BYTE PTR DS:[6AD820],1
004C9B63   75 0C            JNZ SHORT haloce.004C9B71
;004C9B65   E8 46FFFFFF      CALL haloce.004C9AB0
;004C9B6A   A0 701F6500      MOV AL,BYTE PTR DS:[651F70]
;004C9B6F   5B               POP EBX
;004C9B70   C3               RETN
004C9B71   A0 701F6500      MOV AL,BYTE PTR DS:[651F70]
004C9B76   3AC3             CMP AL,BL
004C9B78   0F84 43010000    JE haloce.004C9CC1
004C9B7E   6A 02            PUSH 2
004C9B80   E8 FBA1FCFF      CALL haloce.00493D80
004C9B85   83C4 04          ADD ESP,4
004C9B88   3C 01            CMP AL,1
004C9B8A   75 05            JNZ SHORT haloce.004C9B91
;004C9B8C   E8 5FFFFFFF      CALL haloce.004C9AF0
004C9B91   55               PUSH EBP
004C9B92   33ED             XOR EBP,EBP
004C9B94   66:391D 741F6500 CMP WORD PTR DS:[651F74],BX
004C9B9B   0F8E 1A010000    JLE haloce.004C9CBB
;004C9BA1   56               PUSH ESI
;004C9BA2   57               PUSH EDI
;004C9BA3   EB 0B            JMP SHORT haloce.004C9BB0
;004C9BA5   8DA424 00000000  LEA ESP,DWORD PTR SS:[ESP]
;004C9BAC   8D6424 00        LEA ESP,DWORD PTR SS:[ESP]
;004C9BB0   0FBFC5           MOVSX EAX,BP
;004C9BB3   0FBF0485 781F650>MOVSX EAX,WORD PTR DS:[EAX*4+651F78]
;004C9BBB   83C0 FA          ADD EAX,-6                               ; Switch (cases 6..66)
;004C9BBE   83F8 60          CMP EAX,60
;004C9BC1   0F87 E4000000    JA haloce.004C9CAB
;004C9BC7   0FB688 DC9C4C00  MOVZX ECX,BYTE PTR DS:[EAX+4C9CDC]
;004C9BCE   FF248D C49C4C00  JMP DWORD PTR DS:[ECX*4+4C9CC4]
;004C9BD5   381D 28206500    CMP BYTE PTR DS:[652028],BL              ; Cases 38,66 of switch 004C9BBB
;004C9BDB   74 29            JE SHORT haloce.004C9C06
;004C9BDD   53               PUSH EBX
;004C9BDE   BF 28206500      MOV EDI,haloce.00652028
;004C9BE3   E8 58030000      CALL haloce.004C9F40
;004C9BE8   83C4 04          ADD ESP,4
;004C9BEB   881D 28206500    MOV BYTE PTR DS:[652028],BL
;004C9BF1   66:891D 2E216500 MOV WORD PTR DS:[65212E],BX
;004C9BF8   66:C705 30216500>MOV WORD PTR DS:[652130],0FFFF
;004C9C01   E9 A5000000      JMP haloce.004C9CAB
;004C9C06   E8 25FEFFFF      CALL haloce.004C9A30
;004C9C0B   E9 9B000000      JMP haloce.004C9CAB
;004C9C10   E8 9B040000      CALL haloce.004CA0B0
;004C9C15   E9 91000000      JMP haloce.004C9CAB
;004C9C1A   66:8305 30296500>ADD WORD PTR DS:[652930],2               ; Case 4D of switch 004C9BBB
;004C9C22   66:A1 30296500   MOV AX,WORD PTR DS:[652930]              ; Case 4E of switch 004C9BBB
;004C9C28   66:48            DEC AX
;004C9C2A   33C9             XOR ECX,ECX
;004C9C2C   66:3BC3          CMP AX,BX
;004C9C2F   0F9EC1           SETLE CL
;004C9C32   66:A3 30296500   MOV WORD PTR DS:[652930],AX
;004C9C38   8B35 30296500    MOV ESI,DWORD PTR DS:[652930]
;004C9C3E   0FBF05 2C296500  MOVSX EAX,WORD PTR DS:[65292C]
;004C9C45   49               DEC ECX
;004C9C46   23CE             AND ECX,ESI
;004C9C48   0FBFC9           MOVSX ECX,CX
;004C9C4B   48               DEC EAX
;004C9C4C   3BC8             CMP ECX,EAX
;004C9C4E   7F 03            JG SHORT haloce.004C9C53
;004C9C50   66:8BC1          MOV AX,CX
;004C9C53   66:3D FFFF       CMP AX,0FFFF
;004C9C57   66:A3 30296500   MOV WORD PTR DS:[652930],AX
;004C9C5D   74 4C            JE SHORT haloce.004C9CAB
;004C9C5F   0FBF15 2E296500  MOVSX EDX,WORD PTR DS:[65292E]
;004C9C66   0FBFC0           MOVSX EAX,AX
;004C9C69   2BD0             SUB EDX,EAX
;004C9C6B   83C2 08          ADD EDX,8
;004C9C6E   8BC2             MOV EAX,EDX
;004C9C70   25 07000080      AND EAX,80000007
;004C9C75   79 05            JNS SHORT haloce.004C9C7C
;004C9C77   48               DEC EAX
;004C9C78   83C8 F8          OR EAX,FFFFFFF8
;004C9C7B   40               INC EAX
;004C9C7C   69C0 FF000000    IMUL EAX,EAX,0FF
;004C9C82   05 34216500      ADD EAX,haloce.00652134
;004C9C87   BA 28206500      MOV EDX,haloce.00652028
;004C9C8C   8D6424 00        LEA ESP,DWORD PTR SS:[ESP]
;004C9C90   8A08             MOV CL,BYTE PTR DS:[EAX]
;004C9C92   40               INC EAX
;004C9C93   880A             MOV BYTE PTR DS:[EDX],CL
;004C9C95   42               INC EDX
;004C9C96   3ACB             CMP CL,BL
;004C9C98  ^75 F6            JNZ SHORT haloce.004C9C90
;004C9C9A   BE 28216500      MOV ESI,haloce.00652128
;004C9C9F   E8 8C34F8FF      CALL haloce.0044D130
;004C9CA4   EB 05            JMP SHORT haloce.004C9CAB
;004C9CA6   E8 45FEFFFF      CALL haloce.004C9AF0                     ; Case 6 of switch 004C9BBB
;004C9CAB   45               INC EBP                                  ; Default case of switch 004C9BBB
;004C9CAC   66:3B2D 741F6500 CMP BP,WORD PTR DS:[651F74]
;004C9CB3  ^0F8C F7FEFFFF    JL haloce.004C9BB0
;004C9CB9   5F               POP EDI
;004C9CBA   5E               POP ESI
004C9CBB   5D               POP EBP
004C9CBC   A0 701F6500      MOV AL,BYTE PTR DS:[651F70]
004C9CC1   5B               POP EBX
004C9CC2   C3               RETN




;console enabled. and open, "/hog *" and enter pressed
004C9B40   A0 711F6500      MOV AL,BYTE PTR DS:[651F71]
004C9B45   53               PUSH EBX
004C9B46   33DB             XOR EBX,EBX
004C9B48   3AC3             CMP AL,BL
004C9B4A   0F84 6C010000    JE haloce.004C9CBC
004C9B50   381D A8E76400    CMP BYTE PTR DS:[64E7A8],BL
004C9B56   0F85 60010000    JNZ haloce.004C9CBC
004C9B5C   803D 20D86A00 01 CMP BYTE PTR DS:[6AD820],1
004C9B63   75 0C            JNZ SHORT haloce.004C9B71
;004C9B65   E8 46FFFFFF      CALL haloce.004C9AB0
;004C9B6A   A0 701F6500      MOV AL,BYTE PTR DS:[651F70]
;004C9B6F   5B               POP EBX
;004C9B70   C3               RETN
004C9B71   A0 701F6500      MOV AL,BYTE PTR DS:[651F70]
004C9B76   3AC3             CMP AL,BL
004C9B78   0F84 43010000    JE haloce.004C9CC1
004C9B7E   6A 02            PUSH 2
004C9B80   E8 FBA1FCFF      CALL haloce.00493D80
->
 00493D80   8B0D 54C76400    MOV ECX,DWORD PTR DS:[64C754]
 00493D86   32C0             XOR AL,AL
 00493D88   85C9             TEST ECX,ECX
 00493D8A   74 15            JE SHORT haloce.00493DA1
 00493D8C   8A0D 49C56400    MOV CL,BYTE PTR DS:[64C549]
 00493D92   84C9             TEST CL,CL
 00493D94   75 0B            JNZ SHORT haloce.00493DA1
 00493D96   0FBF4424 04      MOVSX EAX,WORD PTR SS:[ESP+4]
 00493D9B   8A80 68C76400    MOV AL,BYTE PTR DS:[EAX+64C768]
<-
004C9B85   83C4 04          ADD ESP,4
004C9B88   3C 01            CMP AL,1
004C9B8A   75 05            JNZ SHORT haloce.004C9B91
;004C9B8C   E8 5FFFFFFF      CALL haloce.004C9AF0
004C9B91   55               PUSH EBP
004C9B92   33ED             XOR EBP,EBP
004C9B94   66:391D 741F6500 CMP WORD PTR DS:[651F74],BX
004C9B9B   0F8E 1A010000    JLE haloce.004C9CBB
004C9BA1   56               PUSH ESI
004C9BA2   57               PUSH EDI
004C9BA3   EB 0B            JMP SHORT haloce.004C9BB0
;004C9BA5   8DA424 00000000  LEA ESP,DWORD PTR SS:[ESP]
;004C9BAC   8D6424 00        LEA ESP,DWORD PTR SS:[ESP]
004C9BB0   0FBFC5           MOVSX EAX,BP
004C9BB3   0FBF0485 781F6500 MOVSX EAX,WORD PTR DS:[EAX*4+651F78]
004C9BBB   83C0 FA          ADD EAX,-6                               ; Switch (cases 6..66)
004C9BBE   83F8 60          CMP EAX,60
004C9BC1   0F87 E4000000    JA haloce.004C9CAB
004C9BC7   0FB688 DC9C4C00  MOVZX ECX,BYTE PTR DS:[EAX+4C9CDC]
004C9BCE   FF248D C49C4C00  JMP DWORD PTR DS:[ECX*4+4C9CC4]

;when enter key is pressed   :case 38h: enter key
004C9BD5   381D 28206500    CMP BYTE PTR DS:[652028],BL              ; Cases 38,66 of switch 004C9BBB
004C9BDB   74 29            JE SHORT haloce.004C9C06
004C9BDD   53               PUSH EBX
;console parser
004C9BDE   BF 28206500      MOV EDI,haloce.00652028                  ; ASCII "/hog *"
004C9BE3   E8 28DED65D      CALL haloce.004C9F40
004C9BE8   83C4 04          ADD ESP,4

004C9BEB   881D 28206500    MOV BYTE PTR DS:[652028],BL
004C9BF1   66:891D 2E216500 MOV WORD PTR DS:[65212E],BX
004C9BF8   66:C705 30216500 FFFF MOV WORD PTR DS:[652130],0FFFF
004C9C01   E9 A5000000      JMP haloce.004C9CAB

;004C9C06   E8 25FEFFFF      CALL haloce.004C9A30
;004C9C0B   E9 9B000000      JMP haloce.004C9CAB

;when tab is pressed   ;case 1E: tab key
;004C9C10   E8 9B040000      CALL haloce.004CA0B0 
;004C9C15   E9 91000000      JMP haloce.004C9CAB

;when up arror or down arrow is pressed   ;case 4D: up arrow key   ;case 4E: down arrow key
;004C9C1A   66:8305 30296500>ADD WORD PTR DS:[652930],2               ; Case 4D of switch 004C9BBB
;004C9C22   66:A1 30296500   MOV AX,WORD PTR DS:[652930]              ; Case 4E of switch 004C9BBB
;004C9C28   66:48            DEC AX
;004C9C2A   33C9             XOR ECX,ECX
;004C9C2C   66:3BC3          CMP AX,BX
;004C9C2F   0F9EC1           SETLE CL
;004C9C32   66:A3 30296500   MOV WORD PTR DS:[652930],AX
;004C9C38   8B35 30296500    MOV ESI,DWORD PTR DS:[652930]
;004C9C3E   0FBF05 2C296500  MOVSX EAX,WORD PTR DS:[65292C]
;004C9C45   49               DEC ECX
;004C9C46   23CE             AND ECX,ESI
;004C9C48   0FBFC9           MOVSX ECX,CX
;004C9C4B   48               DEC EAX
;004C9C4C   3BC8             CMP ECX,EAX
;004C9C4E   7F 03            JG SHORT haloce.004C9C53
;004C9C50   66:8BC1          MOV AX,CX
;004C9C53   66:3D FFFF       CMP AX,0FFFF
;004C9C57   66:A3 30296500   MOV WORD PTR DS:[652930],AX
;004C9C5D   74 4C            JE SHORT haloce.004C9CAB
;004C9C5F   0FBF15 2E296500  MOVSX EDX,WORD PTR DS:[65292E]
;004C9C66   0FBFC0           MOVSX EAX,AX
;004C9C69   2BD0             SUB EDX,EAX
;004C9C6B   83C2 08          ADD EDX,8
;004C9C6E   8BC2             MOV EAX,EDX
;004C9C70   25 07000080      AND EAX,80000007
;004C9C75   79 05            JNS SHORT haloce.004C9C7C
;004C9C77   48               DEC EAX
;004C9C78   83C8 F8          OR EAX,FFFFFFF8
;004C9C7B   40               INC EAX
;004C9C7C   69C0 FF000000    IMUL EAX,EAX,0FF
;004C9C82   05 34216500      ADD EAX,haloce.00652134                  ; ASCII "/console 2"
;004C9C87   BA 28206500      MOV EDX,haloce.00652028                  ; ASCII "/hog *"
;004C9C8C   8D6424 00        LEA ESP,DWORD PTR SS:[ESP]
;004C9C90   8A08             MOV CL,BYTE PTR DS:[EAX]
;004C9C92   40               INC EAX
;004C9C93   880A             MOV BYTE PTR DS:[EDX],CL
;004C9C95   42               INC EDX
;004C9C96   3ACB             CMP CL,BL
;004C9C98  ^75 F6            JNZ SHORT haloce.004C9C90
;004C9C9A   BE 28216500      MOV ESI,haloce.00652128
;004C9C9F   E8 8C34F8FF      CALL haloce.0044D130
;004C9CA4   EB 05            JMP SHORT haloce.004C9CAB

;004C9CA6   E8 45FEFFFF      CALL haloce.004C9AF0                     ; Case 6 of switch 004C9BBB

004C9CAB   45               INC EBP                                  ; Default case of switch 004C9BBB
004C9CAC   66:3B2D 741F6500 CMP BP,WORD PTR DS:[651F74]
004C9CB3  ^0F8C F7FEFFFF    JL haloce.004C9BB0
004C9CB9   5F               POP EDI
004C9CBA   5E               POP ESI
004C9CBB   5D               POP EBP
004C9CBC   A0 701F6500      MOV AL,BYTE PTR DS:[651F70]
004C9CC1   5B               POP EBX
004C9CC2   C3               RETN




;forcing the main thread to use console:

char *buffer = (char*)0x00652028;
buffer[0] = 's';
buffer[1] = 'v';
buffer[2] = '_';
buffer[3] = 'p';
buffer[4] = 'l';
buffer[5] = 'a';
buffer[6] = 'y';
buffer[7] = 'e';
buffer[8] = 'r';
buffer[9] = 's';
buffer[10] = 0;

__asm 
{
   MOV EBX,0x651F74
   MOV WORD PTR[EBX],2
   
   MOV EDI,0x651F70
   MOV BYTE PTR[EDI],1
   
   MOV ESI,0x651F78
   MOV WORD PTR[ESI],0x38
   
}