;notes by Jesus7Freak
;haloce.1.09

;\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x54
;xxxxxxxxx

00579420   55               PUSH EBP
00579421   8BEC             MOV EBP,ESP
00579423   83E4 F8          AND ESP,FFFFFFF8
00579426   83EC 54          SUB ESP,54
00579429   8B45 08          MOV EAX,DWORD PTR SS:[EBP+8] ;obj tag
0057942C   8B0D B0B37F00    MOV ECX,DWORD PTR DS:[7FB3B0] ;"object"
00579432   8B51 34          MOV EDX,DWORD PTR DS:[ECX+34]
00579435   8B0D E46D8100    MOV ECX,DWORD PTR DS:[816DE4] ;meta
0057943B   25 FFFF0000      AND EAX,0FFFF
00579440   53               PUSH EBX
00579441   8D0440           LEA EAX,DWORD PTR DS:[EAX+EAX*2]
00579444   56               PUSH ESI
00579445   57               PUSH EDI
00579446   8B7C82 08        MOV EDI,DWORD PTR DS:[EDX+EAX*4+8] ;obj
0057944A   8B07             MOV EAX,DWORD PTR DS:[EDI] ;obj type tag or meta tag
0057944C   25 FFFF0000      AND EAX,0FFFF
00579451   C1E0 05          SHL EAX,5
00579454   8B5C08 14        MOV EBX,DWORD PTR DS:[EAX+ECX+14]
00579458   D983 F8020000    FLD DWORD PTR DS:[EBX+2F8] ;top speed ->ST0=0.275
0057945E   D9E1             FABS
00579460   D95C24 14        FSTP DWORD PTR SS:[ESP+14] ;Stack SS:[0018DB94]
00579464   D983 FC020000    FLD DWORD PTR DS:[EBX+2FC] ;top speed (reverse)->ST0=0.12
0057946A   D9E1             FABS
0057946C   D95C24 18        FSTP DWORD PTR SS:[ESP+18] ;Stack SS:[0018DB98]
00579470   D94424 14        FLD DWORD PTR SS:[ESP+14] ;->ST0=0.275
00579474   D85C24 18        FCOMP DWORD PTR SS:[ESP+18] ;0.275 == 0.12
00579478   DFE0             FSTSW AX ;->AX=0026
0057947A   F6C4 41          TEST AH,41
0057947D   75 0A            JNZ SHORT haloce.00579489
0057947F   8B5424 14        MOV EDX,DWORD PTR SS:[ESP+14] ;->EDX=3E8CCCCD (0.275)
00579483   895424 10        MOV DWORD PTR SS:[ESP+10],EDX ;Stack SS:[0018DB90]
00579487   EB 08            JMP SHORT haloce.00579491
;00579489   8B4424 18        MOV EAX,DWORD PTR SS:[ESP+18]
;0057948D   894424 10        MOV DWORD PTR SS:[ESP+10],EAX
00579491   D983 30030000    FLD DWORD PTR DS:[EBX+330];->ST0=0.0
00579497   D9E1             FABS
00579499   D95C24 1C        FSTP DWORD PTR SS:[ESP+1C] ;Stack SS:[0018DB9C]
0057949D   D983 34030000    FLD DWORD PTR DS:[EBX+334];->ST0=0.0
005794A3   D9E1             FABS
005794A5   D95C24 20        FSTP DWORD PTR SS:[ESP+20];Stack SS:[0018DBA0]
005794A9   D94424 1C        FLD DWORD PTR SS:[ESP+1C];->ST0=0.0
005794AD   D85C24 20        FCOMP DWORD PTR SS:[ESP+20] ;0.0 ==0.0
005794B1   DFE0             FSTSW AX;->AX=4026
005794B3   F6C4 41          TEST AH,41
005794B6   75 0A            JNZ SHORT haloce.005794C2
;005794B8   8B4C24 1C        MOV ECX,DWORD PTR SS:[ESP+1C]
;005794BC   894C24 24        MOV DWORD PTR SS:[ESP+24],ECX
;005794C0   EB 08            JMP SHORT haloce.005794CA
005794C2   8B5424 20        MOV EDX,DWORD PTR SS:[ESP+20];->EDX=00000000 (0.0)
005794C6   895424 24        MOV DWORD PTR SS:[ESP+24],EDX;Stack SS:[0018DBA4]
005794CA   D983 08030000    FLD DWORD PTR DS:[EBX+308];left turn angle->ST0=30.0
005794D0   D9E1             FABS
005794D2   D95C24 28        FSTP DWORD PTR SS:[ESP+28];Stack SS:[0018DBA8]
005794D6   D983 0C030000    FLD DWORD PTR DS:[EBX+30C];right turn angle->ST0=-30.0
005794DC   D9E1             FABS;->ST0=30.0
005794DE   D95C24 2C        FSTP DWORD PTR SS:[ESP+2C];Stack SS:[0018DBAC]
005794E2   D94424 28        FLD DWORD PTR SS:[ESP+28];->ST0=30.0
005794E6   D85C24 2C        FCOMP DWORD PTR SS:[ESP+2C];30.0 == 30.0
005794EA   DFE0             FSTSW AX;->AX=4026
005794EC   F6C4 41          TEST AH,41
005794EF   75 0A            JNZ SHORT haloce.005794FB
;005794F1   8B4424 28        MOV EAX,DWORD PTR SS:[ESP+28]
;005794F5   894424 38        MOV DWORD PTR SS:[ESP+38],EAX
;005794F9   EB 08            JMP SHORT haloce.00579503
005794FB   8B4C24 2C        MOV ECX,DWORD PTR SS:[ESP+2C];->ECX=41F00000
005794FF   894C24 38        MOV DWORD PTR SS:[ESP+38],ECX;Stack SS:[0018DBB8]
00579503   8D97 24010000    LEA EDX,DWORD PTR DS:[EDI+124];->EDX=40056FF8
00579509   8D83 1C030000    LEA EAX,DWORD PTR DS:[EBX+31C];->EAX=40695BA0
0057950F   895424 34        MOV DWORD PTR SS:[ESP+34],EDX;Stack SS:[0018DBB4]
00579513   894424 30        MOV DWORD PTR SS:[ESP+30],EAX;Stack SS:[0018DBB0]
00579517   C74424 3C 04000000   MOV DWORD PTR SS:[ESP+3C],4;Stack SS:[0018DBBC]
0057951F   90               NOP
00579520   8B4C24 30        MOV ECX,DWORD PTR SS:[ESP+30];Stack SS:[0018DBB0]
00579524   66:8B01          MOV AX,WORD PTR DS:[ECX];->AX=0000
00579527   66:85C0          TEST AX,AX
0057952A   0F84 85030000    JE haloce.005798B5
;00579530   D905 00226100    FLD DWORD PTR DS:[612200]
;00579536   0FBFC0           MOVSX EAX,AX
;00579539   48               DEC EAX
;0057953A   83F8 23          CMP EAX,23
;0057953D   0F87 6C030000    JA haloce.005798AF
;00579543   FF2485 E4985700  JMP DWORD PTR DS:[EAX*4+5798E4]
;0057954A   DDD8             FSTP ST
;0057954C   D987 D4040000    FLD DWORD PTR DS:[EDI+4D4]
;00579552   D9E1             FABS
;00579554   D87424 10        FDIV DWORD PTR SS:[ESP+10]
; 00579558   E9 26030000      JMP haloce.00579883
; 0057955D   DDD8             FSTP ST
; 0057955F   D987 D4040000    FLD DWORD PTR DS:[EDI+4D4]
; 00579565   D81D 00226100    FCOMP DWORD PTR DS:[612200]
; 0057956B   DFE0             FSTSW AX
; 0057956D   F6C4 05          TEST AH,5
; 00579570   7A 0F            JPE SHORT haloce.00579581
; 00579572   D905 00226100    FLD DWORD PTR DS:[612200]
; 00579578   D87424 14        FDIV DWORD PTR SS:[ESP+14]
; 0057957C   E9 02030000      JMP haloce.00579883
; 00579581   D987 D4040000    FLD DWORD PTR DS:[EDI+4D4]
; 00579587   D87424 14        FDIV DWORD PTR SS:[ESP+14]
; 0057958B   E9 F3020000      JMP haloce.00579883
; 00579590   DDD8             FSTP ST
; 00579592   D987 D4040000    FLD DWORD PTR DS:[EDI+4D4]
; 00579598   D81D 00226100    FCOMP DWORD PTR DS:[612200]
; 0057959E   DFE0             FSTSW AX
; 005795A0   F6C4 41          TEST AH,41
; 005795A3   75 11            JNZ SHORT haloce.005795B6
; 005795A5   D905 00226100    FLD DWORD PTR DS:[612200]
; 005795AB   D9E1             FABS
; 005795AD   D87424 18        FDIV DWORD PTR SS:[ESP+18]
; 005795B1   E9 CD020000      JMP haloce.00579883
; 005795B6   D987 D4040000    FLD DWORD PTR DS:[EDI+4D4]
; 005795BC   D9E1             FABS
; 005795BE   D87424 18        FDIV DWORD PTR SS:[ESP+18]
; 005795C2   E9 BC020000      JMP haloce.00579883
; 005795C7   DDD8             FSTP ST
; 005795C9   D987 D8040000    FLD DWORD PTR DS:[EDI+4D8]
; 005795CF   D9E1             FABS
; 005795D1   D87424 24        FDIV DWORD PTR SS:[ESP+24]
; 005795D5   E9 A9020000      JMP haloce.00579883
; 005795DA   DDD8             FSTP ST
; 005795DC   D987 D8040000    FLD DWORD PTR DS:[EDI+4D8]
; 005795E2   D9E1             FABS
; 005795E4   D87424 1C        FDIV DWORD PTR SS:[ESP+1C]
; 005795E8   E9 96020000      JMP haloce.00579883
; 005795ED   DDD8             FSTP ST
; 005795EF   D987 D8040000    FLD DWORD PTR DS:[EDI+4D8]
; 005795F5   D9E1             FABS
; 005795F7   D87424 20        FDIV DWORD PTR SS:[ESP+20]
; 005795FB   E9 83020000      JMP haloce.00579883
; 00579600   DDD8             FSTP ST
; 00579602   D987 D4040000    FLD DWORD PTR DS:[EDI+4D4]
; 00579608   D9E1             FABS
; 0057960A   D87424 10        FDIV DWORD PTR SS:[ESP+10]
; 0057960E   D987 D8040000    FLD DWORD PTR DS:[EDI+4D8]
; 00579614   D9E1             FABS
; 00579616   D87424 24        FDIV DWORD PTR SS:[ESP+24]
; 0057961A   DD5C24 40        FSTP QWORD PTR SS:[ESP+40]
; 0057961E   DC5424 40        FCOM QWORD PTR SS:[ESP+40]
; 00579622   DFE0             FSTSW AX
; 00579624   F6C4 41          TEST AH,41
; 00579627   0F84 56020000    JE haloce.00579883
; 0057962D   DDD8             FSTP ST
; 0057962F   DD4424 40        FLD QWORD PTR SS:[ESP+40]
; 00579633   E9 4B020000      JMP haloce.00579883
; 00579638   DDD8             FSTP ST
; 0057963A   D987 DC040000    FLD DWORD PTR DS:[EDI+4DC]
; 00579640   D9E1             FABS
; 00579642   D87424 38        FDIV DWORD PTR SS:[ESP+38]
; 00579646   E9 38020000      JMP haloce.00579883
; 0057964B   DDD8             FSTP ST
; 0057964D   D987 DC040000    FLD DWORD PTR DS:[EDI+4DC]
; 00579653   D9E1             FABS
; 00579655   D87424 28        FDIV DWORD PTR SS:[ESP+28]
; 00579659   E9 25020000      JMP haloce.00579883
; 0057965E   DDD8             FSTP ST
; 00579660   D987 DC040000    FLD DWORD PTR DS:[EDI+4DC]
; 00579666   D9E1             FABS
; 00579668   D87424 2C        FDIV DWORD PTR SS:[ESP+2C]
; 0057966C   E9 12020000      JMP haloce.00579883
; 00579671   8A87 CC040000    MOV AL,BYTE PTR DS:[EDI+4CC]
; 00579677   DDD8             FSTP ST
; 00579679   A8 04            TEST AL,4
; 0057967B   0F85 28020000    JNZ haloce.005798A9
; 00579681   D905 00226100    FLD DWORD PTR DS:[612200]
; 00579687   E9 23020000      JMP haloce.005798AF
; 0057968C   8A87 CC040000    MOV AL,BYTE PTR DS:[EDI+4CC]
; 00579692   DDD8             FSTP ST
; 00579694   A8 08            TEST AL,8
; 00579696   0F85 0D020000    JNZ haloce.005798A9
; 0057969C   D905 00226100    FLD DWORD PTR DS:[612200]
; 005796A2   E9 08020000      JMP haloce.005798AF
; 005796A7   8D47 68          LEA EAX,DWORD PTR DS:[EDI+68]
; 005796AA   DDD8             FSTP ST
; 005796AC   E8 AF82E8FF      CALL haloce.00401960
; 005796B1   D87424 10        FDIV DWORD PTR SS:[ESP+10]
; 005796B5   E9 C9010000      JMP haloce.00579883
; 005796BA   F647 10 1C       TEST BYTE PTR DS:[EDI+10],1C
; 005796BE   EB 04            JMP SHORT haloce.005796C4
; 005796C0   F647 10 02       TEST BYTE PTR DS:[EDI+10],2
; 005796C4   DDD8             FSTP ST
; 005796C6  ^74 B9            JE SHORT haloce.00579681
; 005796C8   8D47 68          LEA EAX,DWORD PTR DS:[EDI+68]
; 005796CB   E8 9082E8FF      CALL haloce.00401960
; 005796D0   D87424 10        FDIV DWORD PTR SS:[ESP+10]
; 005796D4   E9 AA010000      JMP haloce.00579883
; 005796D9   DDD8             FSTP ST
; 005796DB   D947 70          FLD DWORD PTR DS:[EDI+70]
; 005796DE   D84F 7C          FMUL DWORD PTR DS:[EDI+7C]
; 005796E1   D947 6C          FLD DWORD PTR DS:[EDI+6C]
; 005796E4   D84F 78          FMUL DWORD PTR DS:[EDI+78]
; 005796E7   DEC1             FADDP ST(1),ST
; 005796E9   D947 68          FLD DWORD PTR DS:[EDI+68]
; 005796EC   D84F 74          FMUL DWORD PTR DS:[EDI+74]
; 005796EF   DEC1             FADDP ST(1),ST
; 005796F1   D9E1             FABS
; 005796F3   D87424 10        FDIV DWORD PTR SS:[ESP+10]
; 005796F7   E9 87010000      JMP haloce.00579883
; 005796FC   DDD8             FSTP ST
; 005796FE   D987 88000000    FLD DWORD PTR DS:[EDI+88]
; 00579704   D84F 70          FMUL DWORD PTR DS:[EDI+70]
; 00579707   D987 84000000    FLD DWORD PTR DS:[EDI+84]
; 0057970D   D84F 6C          FMUL DWORD PTR DS:[EDI+6C]
; 00579710   DEC1             FADDP ST(1),ST
; 00579712   D987 80000000    FLD DWORD PTR DS:[EDI+80]
; 00579718   D84F 68          FMUL DWORD PTR DS:[EDI+68]
; 0057971B   DEC1             FADDP ST(1),ST
; 0057971D   D9E1             FABS
; 0057971F   D87424 10        FDIV DWORD PTR SS:[ESP+10]
; 00579723   E9 5B010000      JMP haloce.00579883
; 00579728   DDD8             FSTP ST
; 0057972A   D987 E4040000    FLD DWORD PTR DS:[EDI+4E4]
; 00579730   D8B3 10030000    FDIV DWORD PTR DS:[EBX+310]
; 00579736   E9 48010000      JMP haloce.00579883
; 0057973B   DDD8             FSTP ST
; 0057973D   D987 E8040000    FLD DWORD PTR DS:[EDI+4E8]
; 00579743   D8B3 10030000    FDIV DWORD PTR DS:[EBX+310]
; 00579749   E9 35010000      JMP haloce.00579883
; 0057974E   DDD8             FSTP ST
; 00579750   D987 D4040000    FLD DWORD PTR DS:[EDI+4D4]
; 00579756   D8A7 DC040000    FSUB DWORD PTR DS:[EDI+4DC]
; 0057975C   D9E1             FABS
; 0057975E   D87424 10        FDIV DWORD PTR SS:[ESP+10]
; 00579762   E9 1C010000      JMP haloce.00579883
; 00579767   DDD8             FSTP ST
; 00579769   D987 DC040000    FLD DWORD PTR DS:[EDI+4DC]
; 0057976F   D887 D4040000    FADD DWORD PTR DS:[EDI+4D4]
; 00579775   D9E1             FABS
; 00579777   D87424 10        FDIV DWORD PTR SS:[ESP+10]
; 0057977B   E9 03010000      JMP haloce.00579883
; 00579780   DDD8             FSTP ST
; 00579782   D987 E0040000    FLD DWORD PTR DS:[EDI+4E0]
; 00579788   D8B3 10030000    FDIV DWORD PTR DS:[EBX+310]
; 0057978E   E9 F0000000      JMP haloce.00579883
; 00579793   8D4F 74          LEA ECX,DWORD PTR DS:[EDI+74]
; 00579796   DDD8             FSTP ST
; 00579798   8D57 68          LEA EDX,DWORD PTR DS:[EDI+68]
; 0057979B   8D7424 48        LEA ESI,DWORD PTR SS:[ESP+48]
; 0057979F   8D4424 54        LEA EAX,DWORD PTR SS:[ESP+54]
; 005797A3   E8 C877F5FF      CALL haloce.004D0F70
; 005797A8   8BC6             MOV EAX,ESI
; 005797AA   E8 B181E8FF      CALL haloce.00401960
; 005797AF   D80D 38256100    FMUL DWORD PTR DS:[612538]
; 005797B5   D9C0             FLD ST
; 005797B7   DEC9             FMULP ST(1),ST
; 005797B9   E9 C5000000      JMP haloce.00579883
; 005797BE   DDD8             FSTP ST
; 005797C0   D987 F0040000    FLD DWORD PTR DS:[EDI+4F0]
; 005797C6   E9 B8000000      JMP haloce.00579883
; 005797CB   DDD8             FSTP ST
; 005797CD   D987 EC040000    FLD DWORD PTR DS:[EDI+4EC]
; 005797D3   E9 AB000000      JMP haloce.00579883
; 005797D8   DDD8             FSTP ST
; 005797DA   0FB697 D0040000  MOVZX EDX,BYTE PTR DS:[EDI+4D0]
; 005797E1   D947 7C          FLD DWORD PTR DS:[EDI+7C]
; 005797E4   895424 40        MOV DWORD PTR SS:[ESP+40],EDX
; 005797E8   D84F 70          FMUL DWORD PTR DS:[EDI+70]
; 005797EB   D947 78          FLD DWORD PTR DS:[EDI+78]
; 005797EE   D84F 6C          FMUL DWORD PTR DS:[EDI+6C]
; 005797F1   DEC1             FADDP ST(1),ST
; 005797F3   D947 74          FLD DWORD PTR DS:[EDI+74]
; 005797F6   D84F 68          FMUL DWORD PTR DS:[EDI+68]
; 005797F9   DEC1             FADDP ST(1),ST
; 005797FB   D9E1             FABS
; 005797FD   D87424 10        FDIV DWORD PTR SS:[ESP+10]
; 00579801   D987 D4040000    FLD DWORD PTR DS:[EDI+4D4]
; 00579807   D9E1             FABS
; 00579809   D87424 14        FDIV DWORD PTR SS:[ESP+14]
; 0057980D   DB4424 40        FILD DWORD PTR SS:[ESP+40]
; 00579811   D80D F8216100    FMUL DWORD PTR DS:[6121F8]
; 00579817   D805 04226100    FADD DWORD PTR DS:[612204]
; 0057981D   D80D FC216100    FMUL DWORD PTR DS:[6121FC]
; 00579823   D815 00226100    FCOM DWORD PTR DS:[612200]
; 00579829   DFE0             FSTSW AX
; 0057982B   F6C4 05          TEST AH,5
; 0057982E   7A 0A            JPE SHORT haloce.0057983A
; 00579830   DDD8             FSTP ST
; 00579832   D905 00226100    FLD DWORD PTR DS:[612200]
; 00579838   EB 15            JMP SHORT haloce.0057984F
; 0057983A   D815 04226100    FCOM DWORD PTR DS:[612204]
; 00579840   DFE0             FSTSW AX
; 00579842   F6C4 41          TEST AH,41
; 00579845   75 08            JNZ SHORT haloce.0057984F
; 00579847   DDD8             FSTP ST
; 00579849   D905 04226100    FLD DWORD PTR DS:[612204]
; 0057984F   D905 04226100    FLD DWORD PTR DS:[612204]
; 00579855   D8E1             FSUB ST,ST(1)
; 00579857   DECB             FMULP ST(3),ST
; 00579859   D8C9             FMUL ST,ST(1)
; 0057985B   DEC2             FADDP ST(2),ST
; 0057985D   DDD8             FSTP ST
; 0057985F   EB 22            JMP SHORT haloce.00579883
; 00579861   8D47 68          LEA EAX,DWORD PTR DS:[EDI+68]
; 00579864   DDD8             FSTP ST
; 00579866   E8 F580E8FF      CALL haloce.00401960
; 0057986B   D8B3 F8020000    FDIV DWORD PTR DS:[EBX+2F8]
; 00579871   D88F F0040000    FMUL DWORD PTR DS:[EDI+4F0]
; 00579877   D825 28236100    FSUB DWORD PTR DS:[612328]
; 0057987D   D80D 48266100    FMUL DWORD PTR DS:[612648]
; 00579883   D815 00226100    FCOM DWORD PTR DS:[612200]
; 00579889   DFE0             FSTSW AX
; 0057988B   F6C4 05          TEST AH,5
; 0057988E   7A 0A            JPE SHORT haloce.0057989A
; 00579890   DDD8             FSTP ST
; 00579892   D905 00226100    FLD DWORD PTR DS:[612200]
; 00579898   EB 15            JMP SHORT haloce.005798AF
; 0057989A   D815 04226100    FCOM DWORD PTR DS:[612204]
; 005798A0   DFE0             FSTSW AX
; 005798A2   F6C4 41          TEST AH,41
; 005798A5   75 08            JNZ SHORT haloce.005798AF
; 005798A7   DDD8             FSTP ST
; 005798A9   D905 04226100    FLD DWORD PTR DS:[612204]
; 005798AF   8B4424 34        MOV EAX,DWORD PTR SS:[ESP+34]
; 005798B3   D918             FSTP DWORD PTR DS:[EAX]
005798B5   8B5424 30        MOV EDX,DWORD PTR SS:[ESP+30];->EDX=40695BA0
005798B9   8B4C24 34        MOV ECX,DWORD PTR SS:[ESP+34];->ECX=40056FF8
005798BD   8B4424 3C        MOV EAX,DWORD PTR SS:[ESP+3C];->EAX=00000004
005798C1   83C2 02          ADD EDX,2;->EDX=40695BA2
005798C4   83C1 04          ADD ECX,4;->ECX=40056FFC
005798C7   48               DEC EAX;->EAX=00000003
005798C8   895424 30        MOV DWORD PTR SS:[ESP+30],EDX
005798CC   894C24 34        MOV DWORD PTR SS:[ESP+34],ECX
005798D0   894424 3C        MOV DWORD PTR SS:[ESP+3C],EAX
005798D4  ^0F85 46FCFFFF    JNZ haloce.00579520
005798DA   5F               POP EDI
005798DB   5E               POP ESI
005798DC   5B               POP EBX
005798DD   8BE5             MOV ESP,EBP
005798DF   5D               POP EBP
005798E0   C3               RETN






0050F010   83EC 08          SUB ESP,8
0050F013   D94424 0C        FLD DWORD PTR SS:[ESP+C]
0050F017   D9E1             FABS
0050F019   D9C0             FLD ST
0050F01B   D84A 08          FMUL DWORD PTR DS:[EDX+8]
0050F01E   D95C24 04        FSTP DWORD PTR SS:[ESP+4]
0050F022   D9C0             FLD ST
0050F024   D84A 0C          FMUL DWORD PTR DS:[EDX+C]
0050F027   D91C24           FSTP DWORD PTR SS:[ESP]
0050F02A   D94424 0C        FLD DWORD PTR SS:[ESP+C]
0050F02E   D81D 00226100    FCOMP DWORD PTR DS:[612200]
0050F034   DFE0             FSTSW AX
0050F036   F6C4 41          TEST AH,41
0050F039   75 52            JNZ SHORT haloce.0050F08D
0050F03B   D90424           FLD DWORD PTR SS:[ESP]
0050F03E   D9E0             FCHS
0050F040   D819             FCOMP DWORD PTR DS:[ECX]
0050F042   DFE0             FSTSW AX
0050F044   F6C4 01          TEST AH,1
0050F047   75 07            JNZ SHORT haloce.0050F050
0050F049   D90424           FLD DWORD PTR SS:[ESP]
0050F04C   D801             FADD DWORD PTR DS:[ECX]
0050F04E   EB 26            JMP SHORT haloce.0050F076
0050F050   D901             FLD DWORD PTR DS:[ECX]
0050F052   D81D 00226100    FCOMP DWORD PTR DS:[612200]
0050F058   DFE0             FSTSW AX
0050F05A   F6C4 01          TEST AH,1
0050F05D   75 08            JNZ SHORT haloce.0050F067
0050F05F   D94424 04        FLD DWORD PTR SS:[ESP+4]
0050F063   D801             FADD DWORD PTR DS:[ECX]
0050F065   EB 0F            JMP SHORT haloce.0050F076
0050F067   D901             FLD DWORD PTR DS:[ECX]
0050F069   D83424           FDIV DWORD PTR SS:[ESP]
0050F06C   D805 04226100    FADD DWORD PTR DS:[612204]
0050F072   D84C24 04        FMUL DWORD PTR SS:[ESP+4]
0050F076   D919             FSTP DWORD PTR DS:[ECX]
0050F078   D80A             FMUL DWORD PTR DS:[EDX];veh top speed
0050F07A   D811             FCOM DWORD PTR DS:[ECX]
0050F07C   DFE0             FSTSW AX
0050F07E   F6C4 05          TEST AH,5
0050F081   7B 64            JPO SHORT haloce.0050F0E7
0050F083   DDD8             FSTP ST
0050F085   D901             FLD DWORD PTR DS:[ECX]
0050F087   D919             FSTP DWORD PTR DS:[ECX]
0050F089   83C4 08          ADD ESP,8
0050F08C   C3               RETN
0050F08D   D94424 0C        FLD DWORD PTR SS:[ESP+C]
0050F091   D81D 00226100    FCOMP DWORD PTR DS:[612200]
0050F097   DFE0             FSTSW AX
0050F099   F6C4 05          TEST AH,5
0050F09C   7A 4F            JPE SHORT haloce.0050F0ED
0050F09E   D901             FLD DWORD PTR DS:[ECX]
0050F0A0   D81C24           FCOMP DWORD PTR SS:[ESP]
0050F0A3   D901             FLD DWORD PTR DS:[ECX]
0050F0A5   DFE0             FSTSW AX
0050F0A7   F6C4 01          TEST AH,1
0050F0AA   75 05            JNZ SHORT haloce.0050F0B1
0050F0AC   D82424           FSUB DWORD PTR SS:[ESP]
0050F0AF   EB 22            JMP SHORT haloce.0050F0D3
0050F0B1   D81D 00226100    FCOMP DWORD PTR DS:[612200]
0050F0B7   D901             FLD DWORD PTR DS:[ECX]
0050F0B9   DFE0             FSTSW AX
0050F0BB   F6C4 41          TEST AH,41
0050F0BE   7A 06            JPE SHORT haloce.0050F0C6
0050F0C0   D86424 04        FSUB DWORD PTR SS:[ESP+4]
0050F0C4   EB 0D            JMP SHORT haloce.0050F0D3
0050F0C6   D83424           FDIV DWORD PTR SS:[ESP]
0050F0C9   D825 04226100    FSUB DWORD PTR DS:[612204]
0050F0CF   D84C24 04        FMUL DWORD PTR SS:[ESP+4]
0050F0D3   D919             FSTP DWORD PTR DS:[ECX]
0050F0D5   D84A 04          FMUL DWORD PTR DS:[EDX+4]
0050F0D8   D9E0             FCHS
0050F0DA   D811             FCOM DWORD PTR DS:[ECX]
0050F0DC   DFE0             FSTSW AX
0050F0DE   F6C4 41          TEST AH,41
0050F0E1   74 04            JE SHORT haloce.0050F0E7
0050F0E3   DDD8             FSTP ST
0050F0E5   D901             FLD DWORD PTR DS:[ECX]
0050F0E7   D919             FSTP DWORD PTR DS:[ECX]
0050F0E9   83C4 08          ADD ESP,8
0050F0EC   C3               RETN
0050F0ED   DDD8             FSTP ST
0050F0EF   83C4 08          ADD ESP,8
0050F0F2   C3               RETN


