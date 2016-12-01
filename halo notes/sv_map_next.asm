;notes by Jesus7Freak
;haloceded1.09 bloodgulch
;console 004B3637

00484AC3

...1 ;inspect
00484AC3   . FF52 0C        CALL DWORD PTR DS:[EDX+C] ;EDX=00551950  halocede.004841B0
...2 ;sv_map_next
00484AC3   . FF52 0C        CALL DWORD PTR DS:[EDX+C] ;EDX=00555314  halocede.0047CF50
...3 ;inspect
00484AC3   . FF52 0C        CALL DWORD PTR DS:[EDX+C] ;EDX=00551950  halocede.004841B0


...2
00484AC3   . FF52 0C        CALL DWORD PTR DS:[EDX+C] ;EDX=00555314  halocede.0047CF50
->
0047CF50   . E8 1B0B0500        CALL halocede.004CDA70
->
 004CDA70   $ 66:833D F8CF6100 02    CMP WORD PTR DS:[61CFF8],2 ;DS:[0061CFF8]=0002 Local call from 0047CF50
 004CDA78   . 75 33                  JNZ SHORT halocede.004CDAAD
 004CDA7A   . E8 7125FCFF            CALL halocede.0048FFF0
 ->
  0048FFF0  /$ A1 6CC86100            MOV EAX,DWORD PTR DS:[61C86C];->EAX=00000000
  0048FFF5  |. 85C0                   TEST EAX,EAX
  0048FFF7  |. 74 09                  JE SHORT halocede.00490002
  ;0048FFF9  |. 50                     PUSH EAX
  ;0048FFFA  |. E8 A1F8FFFF            CALL halocede.0048F8A0
  ;0048FFFF  |. 83C4 04                ADD ESP,4
  00490002  |> A1 70C86100            MOV EAX,DWORD PTR DS:[61C870];->EAX=00000000
  00490007  |. 85C0                   TEST EAX,EAX
  00490009  |. 57                     PUSH EDI
  0049000A  |. 74 0A                  JE SHORT halocede.00490016
  ;0049000C  |. BF 70C86100            MOV EDI,halocede.0061C870
  ;00490011  |. E8 4A0B0000            CALL halocede.00490B60
  00490016  |> A1 78635A00            MOV EAX,DWORD PTR DS:[5A6378];->EAX=FFFFFFFF (-1)
  0049001B  |. 83CA FF                OR EDX,FFFFFFFF
  0049001E  |. 3BC2                   CMP EAX,EDX ;both at -1
  00490020  |. 66:C705 7EC86100 0000  MOV WORD PTR DS:[61C87E],0
  00490029  |. 74 1B                  JE SHORT halocede.00490046
  ;0049002B  |. 8025 C25D6100 F7       AND BYTE PTR DS:[615DC2],0F7
  ;00490032  |. B9 A0000000            MOV ECX,0A0
  ;00490037  |. 33C0                   XOR EAX,EAX
  ;00490039  |. BF C45D6100            MOV EDI,halocede.00615DC4
  ;0049003E  |. F3:AB                  REP STOS DWORD PTR ES:[EDI]
  ;00490040  |. 8915 78635A00          MOV DWORD PTR DS:[5A6378],EDX
  ;00490046  |> 5F                     POP EDI
  00490047  \. C3                     RETN
 <-
 004CDA7F   . 66:833D F8CF6100 02    CMP WORD PTR DS:[61CFF8],2 ;[61CFF8]=2
 004CDA87   . 75 34                  JNZ SHORT halocede.004CDABD
 004CDA89   . A1 101A6E00            MOV EAX,DWORD PTR DS:[6E1A10];->EAX=00000000
 004CDA8E   . 85C0                   TEST EAX,EAX
 004CDA90   . 75 2B                  JNZ SHORT halocede.004CDABD
 004CDA92   . A1 4CFB6100            MOV EAX,DWORD PTR DS:[61FB4C];->EAX=006C7980
 004CDA97   . C680 0F130000 01       MOV BYTE PTR DS:[EAX+130F],1;DS:[006C8C8F]=00 -> 01
 
 004CDA9E   . E8 AD3EF9FF            CALL halocede.00461950
 
 004CDAA3   . B8 01000000            MOV EAX,1 ;->EAX=00000001
 004CDAA8   .^E9 E340F9FF            JMP halocede.00461B90
 ...^
 00461B90  /$ 83EC 0C                SUB ESP,0C
 00461B93  |. 6A 00                  PUSH 0
 00461B95  |. 6A 01                  PUSH 1
 00461B97  |. 6A 00                  PUSH 0
 00461B99  |. 8D5424 10              LEA EDX,DWORD PTR SS:[ESP+10];->EDX=0018CF1C
 00461B9D  |. 52                     PUSH EDX
 00461B9E  |. 6A 00                  PUSH 0
 00461BA0  |. 894424 14              MOV DWORD PTR SS:[ESP+14],EAX;EAX=00000001 Stack SS:[0018CF18]=40488940
 00461BA4  |. 8D4C24 14              LEA ECX,DWORD PTR SS:[ESP+14];->ECX=0018CF18
 00461BA8  |. 6A 16                  PUSH 16
 00461BAA  |. 6A 00                  PUSH 0
 00461BAC  |. BA F87F0000            MOV EDX,7FF8
 00461BB1  |. B8 208D6D00            MOV EAX,halocede.006D8D20
 00461BB6  |. 894C24 20              MOV DWORD PTR SS:[ESP+20],ECX
 00461BBA  |. C74424 24 00000000     MOV DWORD PTR SS:[ESP+24],0
 00461BC2  |. E8 697C0700            CALL halocede.004D9830
 ->;ret EAX=00000027
 00461BC7  |. 83C4 1C                ADD ESP,1C
 
 00461BCA  |. 85C0                   TEST EAX,EAX
 00461BCC  |. 7E 1D                  JLE SHORT halocede.00461BEB
 00461BCE  |. 8B0D 4CFB6100          MOV ECX,DWORD PTR DS:[61FB4C]            ;  halocede.006C7980
 
 00461BD4  |. 6A 03                  PUSH 3
 00461BD6  |. 6A 00                  PUSH 0
 00461BD8  |. 6A 00                  PUSH 0
 00461BDA  |. 6A 01                  PUSH 1
 00461BDC  |. 68 208D6D00            PUSH halocede.006D8D20
 00461BE1  |. 6A 01                  PUSH 1
 00461BE3  |. E8 68AC0600            CALL halocede.004CC850
 00461BE8  |. 83C4 18                ADD ESP,18
 ->;ret EAX=00000001
 00461BEB  |> 83C4 0C                ADD ESP,0C;function clean up
 00461BEE  \. C3                     RETN
 
 ;004CDAAD   > A1 BC265700            MOV EAX,DWORD PTR DS:[5726BC]
 ;004CDAB2   . 68 EC8A5600            PUSH halocede.00568AEC ;  ASCII "sv_map_next is a server-only function!"
 ;004CDAB7   . E8 B412FCFF            CALL halocede.0048ED70
 ;004CDABC   . 59                     POP ECX
 ;004CDABD   > C3                     RETN
<-
0047CF55   . 8B4C24 08          MOV ECX,DWORD PTR SS:[ESP+8];->F373000A
0047CF59   . 33C0               XOR EAX,EAX;looks like that ret val was for nothing
0047CF5B   . E9 107D0000        JMP halocede.00484C70
...v
00484C70  /$ 81E1 FFFF0000          AND ECX,0FFFF
00484C76  |. 56                     PUSH ESI
00484C77  |. 8BF1                   MOV ESI,ECX
00484C79  |. 8B0D 70146E00          MOV ECX,DWORD PTR DS:[6E1470];"hs thread"
00484C7F  |. 69F6 18020000          IMUL ESI,ESI,218
00484C85  |. 57                     PUSH EDI
00484C86  |. 8B79 34                MOV EDI,DWORD PTR DS:[ECX+34]
00484C89  |. 8B5437 10              MOV EDX,DWORD PTR DS:[EDI+ESI+10]
00484C8D  |. 8B4A 04                MOV ECX,DWORD PTR DS:[EDX+4]
00484C90  |. 8B15 74146E00          MOV EDX,DWORD PTR DS:[6E1474]
00484C96  |. 8B52 34                MOV EDX,DWORD PTR DS:[EDX+34]
00484C99  |. 03FE                   ADD EDI,ESI
00484C9B  |. 81E1 FFFF0000          AND ECX,0FFFF
00484CA1  |. 8D0C89                 LEA ECX,DWORD PTR DS:[ECX+ECX*4]
00484CA4  |. 8D0C8A                 LEA ECX,DWORD PTR DS:[EDX+ECX*4]
00484CA7  |. F641 06 02             TEST BYTE PTR DS:[ECX+6],2
00484CAB  |. 0FBF51 02              MOVSX EDX,WORD PTR DS:[ECX+2]
00484CAF  |. 75 0C                  JNZ SHORT halocede.00484CBD
00484CB1  |. 8B1495 D89A5900        MOV EDX,DWORD PTR DS:[EDX*4+599AD8]
00484CB8  |. 66:8B12                MOV DX,WORD PTR DS:[EDX]
00484CBB  |. EB 16                  JMP SHORT halocede.00484CD3
00484CBD  |> 6BD2 5C                IMUL EDX,EDX,5C
00484CC0  |. 53                     PUSH EBX
00484CC1  |. 8B1D 4C946200          MOV EBX,DWORD PTR DS:[62944C]
00484CC7  |. 8B9B A0040000          MOV EBX,DWORD PTR DS:[EBX+4A0]
00484CCD  |. 66:8B541A 22           MOV DX,WORD PTR DS:[EDX+EBX+22]
00484CD2  |. 5B                     POP EBX
00484CD3  |> 66:8B49 04             MOV CX,WORD PTR DS:[ECX+4]
00484CD7  |. 66:3BD1                CMP DX,CX
00484CDA  |. 74 47                  JE SHORT halocede.00484D23
00484CDC  |. 66:83FA 03             CMP DX,3
00484CE0  |. 74 41                  JE SHORT halocede.00484D23
00484CE2  |. 66:83F9 2B             CMP CX,2B
00484CE6  |. 7C 06                  JL SHORT halocede.00484CEE
00484CE8  |. 66:83F9 30             CMP CX,30
00484CEC  |. 7E 35                  JLE SHORT halocede.00484D23
00484CEE  |> 66:83F9 25             CMP CX,25
00484CF2  |. 7C 19                  JL SHORT halocede.00484D0D
00484CF4  |. 66:83F9 2A             CMP CX,2A
00484CF8  |. 7F 13                  JG SHORT halocede.00484D0D
00484CFA  |. 66:83FA 2B             CMP DX,2B
00484CFE  |. 7C 23                  JL SHORT halocede.00484D23
00484D00  |. 66:83FA 30             CMP DX,30
00484D04  |. 7F 1D                  JG SHORT halocede.00484D23
00484D06  |. E8 95D20500            CALL halocede.004E1FA0
00484D0B  |. EB 16                  JMP SHORT halocede.00484D23
00484D0D  |> 0FBFC9                 MOVSX ECX,CX
00484D10  |. 6BC9 31                IMUL ECX,ECX,31
00484D13  |. 0FBFD2                 MOVSX EDX,DX
00484D16  |. 03CA                   ADD ECX,EDX
00484D18  |. 50                     PUSH EAX
00484D19  |. FF148D A0CB5900        CALL DWORD PTR DS:[ECX*4+59CBA0]
00484D20  |. 83C4 04                ADD ESP,4
00484D23  |> 8B4F 10                MOV ECX,DWORD PTR DS:[EDI+10]
00484D26  |. 8B11                   MOV EDX,DWORD PTR DS:[ECX]
00484D28  |. 8B4A 08                MOV ECX,DWORD PTR DS:[EDX+8]
00484D2B  |. 8B15 70146E00          MOV EDX,DWORD PTR DS:[6E1470]
00484D31  |. 8901                   MOV DWORD PTR DS:[ECX],EAX
00484D33  |. 8B42 34                MOV EAX,DWORD PTR DS:[EDX+34]
00484D36  |. 8B4C30 10              MOV ECX,DWORD PTR DS:[EAX+ESI+10]
00484D3A  |. 8B11                   MOV EDX,DWORD PTR DS:[ECX]
00484D3C  |. 03C6                   ADD EAX,ESI
00484D3E  |. 5F                     POP EDI
00484D3F  |. 8950 10                MOV DWORD PTR DS:[EAX+10],EDX
00484D42  |. 5E                     POP ESI
00484D43  \. C3                     RETN

;0047CF60   . E8 5B0B0500        CALL halocede.004CDAC0
;0047CF65   . 8B4C24 08          MOV ECX,DWORD PTR SS:[ESP+8]
;0047CF69   . 33C0               XOR EAX,EAX
;0047CF6B   . E9 007D0000        JMP halocede.00484C70
;0047CF70   . 0FBF4424 04        MOVSX EAX,WORD PTR SS:[ESP+4]
;0047CF75   . 8B0485 D89A5900    MOV EAX,DWORD PTR DS:[EAX*4+599AD8]
;0047CF7C   . 8B4C24 0C          MOV ECX,DWORD PTR SS:[ESP+C]
;0047CF80   . 56                 PUSH ESI
;0047CF81   . 8B7424 0C          MOV ESI,DWORD PTR SS:[ESP+C]
;0047CF85   . 51                 PUSH ECX
;0047CF86   . 8D50 1C            LEA EDX,DWORD PTR DS:[EAX+1C]
;0047CF89   . 0FBF40 1A          MOVSX EAX,WORD PTR DS:[EAX+1A]
;0047CF8D   . 52                 PUSH EDX
;0047CF8E   . 50                 PUSH EAX
;0047CF8F   . 56                 PUSH ESI
;0047CF90   . E8 EB7E0000        CALL halocede.00484E80
;0047CF95   . 83C4 10            ADD ESP,10
;0047CF98   . 85C0               TEST EAX,EAX
;0047CF9A   . 74 16              JE SHORT halocede.0047CFB2
;0047CF9C   . 53                 PUSH EBX
;0047CF9D   . 8B58 04            MOV EBX,DWORD PTR DS:[EAX+4]
;0047CFA0   . 8B00               MOV EAX,DWORD PTR DS:[EAX]
;0047CFA2   . E8 990B0500        CALL halocede.004CDB40
;0047CFA7   . 5B                 POP EBX
;0047CFA8   . 8BCE               MOV ECX,ESI
;0047CFAA   . 33C0               XOR EAX,EAX
;0047CFAC   . 5E                 POP ESI
;0047CFAD   . E9 BE7C0000        JMP halocede.00484C70
;0047CFB2   > 5E                 POP ESI
;0047CFB3   . C3                 RETN
<-





