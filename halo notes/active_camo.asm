;notes by Jesus7Freak
;haloce.1.09 player 0
;camo on
0047CB62 - 83 88 04020000 10 - or dword ptr [eax+00000204],10

;camo off
0047CB0D - 83 A0 04020000 EF - and dword ptr [eax+00000204],EF

0047C7C4   8B6C24 14        MOV EBP,DWORD PTR SS:[ESP+14]
0047C7C8   8B5C24 14        MOV EBX,DWORD PTR SS:[ESP+14] ;->EC700000
0047C7CC   50               PUSH EAX ;00000546 short
0047C7CD   55               PUSH EBP ;00000000 short
0047C7CE   E8 6D010000      CALL haloce.0047C940
->
0047C940   83FB FF          CMP EBX,-1
0047C943   55               PUSH EBP
0047C944   8B6C24 08        MOV EBP,DWORD PTR SS:[ESP+8] ;EBP=0 (1st param)
0047C948   56               PUSH ESI
0047C949   57               PUSH EDI
0047C94A   0F84 E9000000    JE haloce.0047CA39
0047C950   8BCB             MOV ECX,EBX ;->EC700000
0047C952   C1F9 10          SAR ECX,10 ;->ECX=FFFFEC70
0047C955   66:85DB          TEST BX,BX ;0000 > -1
0047C958   0F8C DB000000    JL haloce.0047CA39
0047C95E   A1 C0558100      MOV EAX,DWORD PTR DS:[8155C0] ;"players"
0047C963   66:3B58 20       CMP BX,WORD PTR DS:[EAX+20] ;max players =16   0000 < 16
0047C967   0F8D CC000000    JGE haloce.0047CA39
0047C96D   0FBF70 22        MOVSX ESI,WORD PTR DS:[EAX+22] ;->ESI=00000200 static player size
0047C971   8B78 34          MOV EDI,DWORD PTR DS:[EAX+34] ;->EDI=402AAFCC ptr to first static player
0047C974   0FBFD3           MOVSX EDX,BX ;static player index ->EDX=00000000
0047C977   0FAFF2           IMUL ESI,EDX ;static player offset->ESI=00000000
0047C97A   66:8B043E        MOV AX,WORD PTR DS:[ESI+EDI] ;player ID->AX=EC70
0047C97E   03F7             ADD ESI,EDI ;static player->ESI=402AAFCC
0047C980   66:85C0          TEST AX,AX ;valid ID?
0047C983   0F84 B0000000    JE haloce.0047CA39
0047C989   66:85C9          TEST CX,CX ;valid ID?
0047C98C   74 09            JE SHORT haloce.0047C997
0047C98E   66:3BC1          CMP AX,CX ;ID match?
0047C991   0F85 A2000000    JNZ haloce.0047CA39
0047C997   66:85ED          TEST BP,BP ;first param 0 check
0047C99A   0F8C 99000000    JL haloce.0047CA39
0047C9A0   66:83FD 02       CMP BP,2
0047C9A4   0F8D 8F000000    JGE haloce.0047CA39
0047C9AA   66:85ED          TEST BP,BP
0047C9AD   75 1A            JNZ SHORT haloce.0047C9C9
0047C9AF   8B4E 34          MOV ECX,DWORD PTR DS:[ESI+34] ;player biped obj tag->ECX=E2CD005E
;get valid player biped obj
0047C9B2   6A 03            PUSH 3
0047C9B4   E8 97E10700      CALL haloce.004FAB50
->
 004FAB50   57               PUSH EDI
 004FAB51   33FF             XOR EDI,EDI
 004FAB53   83F9 FF          CMP ECX,-1 ;valid obj tag?
 004FAB56   74 39            JE SHORT haloce.004FAB91
 004FAB58   56               PUSH ESI
 004FAB59   8BF1             MOV ESI,ECX ;->ESI=E2CD005E
 004FAB5B   C1FE 10          SAR ESI,10 ;->ESI=FFFFE2CD
 004FAB5E   66:85C9          TEST CX,CX ;valid index?
 004FAB61   7C 2D            JL SHORT haloce.004FAB90
 004FAB63   8B15 B0B37F00    MOV EDX,DWORD PTR DS:[7FB3B0] ;"object" ->EDX=400506B4
 004FAB69   66:3B4A 20       CMP CX,WORD PTR DS:[EDX+20] ;index within range? 005E < 0800
 004FAB6D   7D 21            JGE SHORT haloce.004FAB90
 004FAB6F   0FBF42 22        MOVSX EAX,WORD PTR DS:[EDX+22] ;obj table size->EAX=0000000C
 004FAB73   0FBFC9           MOVSX ECX,CX
 004FAB76   0FAFC1           IMUL EAX,ECX ;obj table offset->EAX=00000468
 004FAB79   0342 34          ADD EAX,DWORD PTR DS:[EDX+34] ;obj table->EAX=40050B54
 004FAB7C   66:8B08          MOV CX,WORD PTR DS:[EAX] ;obj ID->CX=E2CD
 004FAB7F   66:85C9          TEST CX,CX ;valid id?
 004FAB82   74 0C            JE SHORT haloce.004FAB90
 004FAB84   66:85F6          TEST SI,SI ;valid id?
 004FAB87   74 05            JE SHORT haloce.004FAB8E
 004FAB89   66:3BCE          CMP CX,SI ;id match?
 004FAB8C   75 02            JNZ SHORT haloce.004FAB90
 004FAB8E   8BF8             MOV EDI,EAX ;->EDI=40050B54
 004FAB90   5E               POP ESI
 004FAB91   33C0             XOR EAX,EAX
 004FAB93   85FF             TEST EDI,EDI ;valid address != NULL
 004FAB95   74 13            JE SHORT haloce.004FABAA
 004FAB97   8A4F 03          MOV CL,BYTE PTR DS:[EDI+3] ;ObjectType? ->CL=00
 004FAB9A   BA 01000000      MOV EDX,1
 004FAB9F   D3E2             SHL EDX,CL ;->EDX=00000001
 004FABA1   855424 08        TEST DWORD PTR SS:[ESP+8],EDX ;test against first param, 3 == 1?
 004FABA5   74 03            JE SHORT haloce.004FABAA
 004FABA7   8B47 08          MOV EAX,DWORD PTR DS:[EDI+8] ;object address->EAX=4006B45C
 004FABAA   5F               POP EDI
 004FABAB   C3               RETN
<-
0047C9B9   83C4 04          ADD ESP,4
0047C9BC   85C0             TEST EAX,EAX ;valid address?
0047C9BE   74 79            JE SHORT haloce.0047CA39
0047C9C0   F680 04020000 10 TEST BYTE PTR DS:[EAX+204],10 ; 41h == 10?
0047C9C7   75 70            JNZ SHORT haloce.0047CA39
0047C9C9   0FBFC5           MOVSX EAX,BP ;first param->EAX=0
0047C9CC   66:837C46 68 00  CMP WORD PTR DS:[ESI+EAX*2+68],0 ;static player.unknown DS:[402AB034]=0000
0047C9D2   8D7C46 68        LEA EDI,DWORD PTR DS:[ESI+EAX*2+68] ;->EDI=402AB034
0047C9D6   75 0A            JNZ SHORT haloce.0047C9E2
0047C9D8   55               PUSH EBP
0047C9D9   8BC3             MOV EAX,EBX ;static player tag->EAX=EC700000
0047C9DB   E8 50010000      CALL haloce.0047CB30 ;give players biped camo
->
 0047CB30   8B0D C0558100    MOV ECX,DWORD PTR DS:[8155C0]
 0047CB36   8B51 34          MOV EDX,DWORD PTR DS:[ECX+34]
 0047CB39   8B0D B0B37F00    MOV ECX,DWORD PTR DS:[7FB3B0]
 0047CB3F   25 FFFF0000      AND EAX,0FFFF
 0047CB44   C1E0 09          SHL EAX,9
 0047CB47   8B4410 34        MOV EAX,DWORD PTR DS:[EAX+EDX+34]
 0047CB4B   8B51 34          MOV EDX,DWORD PTR DS:[ECX+34]
 0047CB4E   25 FFFF0000      AND EAX,0FFFF
 0047CB53   66:837C24 04 00  CMP WORD PTR SS:[ESP+4],0
 0047CB59   8D0440           LEA EAX,DWORD PTR DS:[EAX+EAX*2]
 0047CB5C   8B4482 08        MOV EAX,DWORD PTR DS:[EDX+EAX*4+8]
 0047CB60   75 10            JNZ SHORT haloce.0047CB72
 0047CB62   8388 04020000 10 OR DWORD PTR DS:[EAX+204],10 ;Biped::IsInvisible
 0047CB69   66:C780 22040000 0000 MOV WORD PTR DS:[EAX+422],0
 0047CB72   C3               RETN
<-
0047C9E0   EB 11            JMP SHORT haloce.0047C9F3
;0047C9E2   A1 44CD6800      MOV EAX,DWORD PTR DS:[68CD44]
;0047C9E7   85C0             TEST EAX,EAX
;0047C9E9   75 0B            JNZ SHORT haloce.0047C9F6
;0047C9EB   55               PUSH EBP
;0047C9EC   8BC3             MOV EAX,EBX
;0047C9EE   E8 8D010000      CALL haloce.0047CB80
0047C9F3   83C4 04          ADD ESP,4
0047C9F6   8B4424 14        MOV EAX,DWORD PTR SS:[ESP+14] ;2nd param time->EAX=00000546
0047C9FA   66:0107          ADD WORD PTR DS:[EDI],AX ;store time in static player
0047C9FD   66:833D D0476B00 0200 CMP WORD PTR DS:[6B47D0],2 ;checks something 2 == 2?
0047CA05   75 2C            JNZ SHORT haloce.0047CA33
0047CA07   8B76 34          MOV ESI,DWORD PTR DS:[ESI+34] ;player biped obj tag->ESI=E2CD005E
0047CA0A   8B15 B0B37F00    MOV EDX,DWORD PTR DS:[7FB3B0] ;"object"
0047CA10   8B52 34          MOV EDX,DWORD PTR DS:[EDX+34]
0047CA13   81E6 FFFF0000    AND ESI,0FFFF
0047CA19   8D0C76           LEA ECX,DWORD PTR DS:[ESI+ESI*2]
0047CA1C   8B4C8A 08        MOV ECX,DWORD PTR DS:[EDX+ECX*4+8] ;player biped obj->4006B45C
0047CA20   8B51 04          MOV EDX,DWORD PTR DS:[ECX+4] ;biped obj.unknown->EDX=00000000
0047CA23   85D2             TEST EDX,EDX
0047CA25   75 0C            JNZ SHORT haloce.0047CA33
;server sync
0047CA27   50               PUSH EAX ;on: 00000546
0047CA28   55               PUSH EBP ;00000000
0047CA29   8BCB             MOV ECX,EBX ;EC700000
0047CA2B   E8 10FEFFFF      CALL haloce.0047C840
->
 0047C840   83EC 08              SUB ESP,8
 0047C843   33C0                 XOR EAX,EAX
 0047C845   83F9 FF              CMP ECX,-1
 0047C848   74 17                JE SHORT haloce.0047C861
 0047C84A   56                   PUSH ESI
 0047C84B   8B35 182B6200        MOV ESI,DWORD PTR DS:[622B18] ; ->ESI=haloce.00622A90
 0047C851   83C6 0C              ADD ESI,0C ;->ESI=00622A9C
 0047C854   E8 177A0700          CALL haloce.004F4270
 ->
  004F4270   8A06             MOV AL,BYTE PTR DS:[ESI] ;AL=01
  004F4272   57               PUSH EDI
  004F4273   83CF FF          OR EDI,FFFFFFFF ;->EDI=FFFFFFFF
  004F4276   3C 01            CMP AL,1
  004F4278   75 27            JNZ SHORT haloce.004F42A1
  004F427A   83F9 FF          CMP ECX,-1 ;==-1?
  004F427D   74 22            JE SHORT haloce.004F42A1
  004F427F   85C9             TEST ECX,ECX ;==0?
  004F4281   8BC1             MOV EAX,ECX ;->EAX=EC700000
  004F4283   7D 02            JGE SHORT haloce.004F4287
  004F4285   F7D8             NEG EAX ;->EAX=13900000
  004F4287   99               CDQ
  004F4288   F77E 04          IDIV DWORD PTR DS:[ESI+4] ;DS:[00622AA0]=00000011->EAX=01269696
  004F428B   8B46 08          MOV EAX,DWORD PTR DS:[ESI+8] ;->EAX=0EB4EB20
  004F428E   8B54D0 04        MOV EDX,DWORD PTR DS:[EAX+EDX*8+4] ;EDX=A ->EDX=1141EB70
  004F4292   85D2             TEST EDX,EDX
  004F4294   74 0B            JE SHORT haloce.004F42A1
  004F4296   390A             CMP DWORD PTR DS:[EDX],ECX ;match?
  004F4298   74 0B            JE SHORT haloce.004F42A5
  ;004F429A   8B52 08          MOV EDX,DWORD PTR DS:[EDX+8]
  ;004F429D   85D2             TEST EDX,EDX
  ;004F429F  ^75 F5            JNZ SHORT haloce.004F4296
  ;004F42A1   8BC7             MOV EAX,EDI
  ;004F42A3   5F               POP EDI
  ;004F42A4   C3               RETN
  004F42A5   8B42 04          MOV EAX,DWORD PTR DS:[EDX+4] ;->EAX=00000001
  004F42A8   5F               POP EDI
  004F42A9   C3               RETN
 <-
 0047C859   83F8 FF              CMP EAX,-1
 0047C85C   5E                   POP ESI
 0047C85D   75 02                JNZ SHORT haloce.0047C861
 ;0047C85F   33C0                 XOR EAX,EAX
 0047C861   66:8B4C24 10         MOV CX,WORD PTR SS:[ESP+10] ;2nd param timer->CX=0546
 0047C866   6A 00                PUSH 0
 0047C868   894424 04            MOV DWORD PTR SS:[ESP+4],EAX ;Stack SS:[0018DB30]
 0047C86C   66:8B4424 10         MOV AX,WORD PTR SS:[ESP+10] ;->AX=0000
 0047C871   6A 01                PUSH 1
 0047C873   66:894424 0C         MOV WORD PTR SS:[ESP+C],AX ;Stack SS:[0018DB34]
 0047C878   6A 00                PUSH 0
 0047C87A   8D4424 18            LEA EAX,DWORD PTR SS:[ESP+18] ;->EAX=0018DB3C
 0047C87E   50                   PUSH EAX ;pointer (ptr ptr?) to data
 0047C87F   6A 00                PUSH 0
 0047C881   8D5424 14            LEA EDX,DWORD PTR SS:[ESP+14] ;->EDX=0018DB30
 0047C885   895424 20            MOV DWORD PTR SS:[ESP+20],EDX ;Stack SS:[0018DB3C]
 0047C889   6A 0E                PUSH 0E
 0047C88B   6A 00                PUSH 0
 0047C88D   BA F87F0000          MOV EDX,7FF8
 0047C892   B8 20CE8000          MOV EAX,haloce.0080CE20
 0047C897   66:894C24 22         MOV WORD PTR SS:[ESP+22],CX ;Stack SS:[0018DB36]
 0047C89C   C74424 2C 00000000   MOV DWORD PTR SS:[ESP+2C],0;Stack SS:[0018DB40]
 0047C8A4   E8 273D0700          CALL haloce.004F05D0 ;build network packet
 0047C8A9   83C4 1C              ADD ESP,1C
 0047C8AC   85C0                 TEST EAX,EAX ;0000002C
 0047C8AE   7E 1D                JLE SHORT haloce.0047C8CD
 0047C8B0   8B0D 84736B00        MOV ECX,DWORD PTR DS:[6B7384] ;007FC340
 0047C8B6   6A 03                PUSH 3
 0047C8B8   6A 00                PUSH 0
 0047C8BA   6A 00                PUSH 0
 0047C8BC   6A 01                PUSH 1
 0047C8BE   68 20CE8000          PUSH haloce.0080CE20
 0047C8C3   6A 01                PUSH 1
 0047C8C5   E8 66890600          CALL haloce.004E5230
 0047C8CA   83C4 18              ADD ESP,18
 0047C8CD   83C4 08              ADD ESP,8
 0047C8D0   C3                   RETN
<-
0047CA30   83C4 08          ADD ESP,8

0047CA33   5F               POP EDI
0047CA34   5E               POP ESI
0047CA35   B0 01            MOV AL,1
0047CA37   5D               POP EBP
0047CA38   C3               RETN
;0047CA39   5F               POP EDI
;0047CA3A   5E               POP ESI
;0047CA3B   32C0             XOR AL,AL
;0047CA3D   5D               POP EBP
;0047CA3E   C3               RETN
<-
0047C7D3   83C4 08          ADD ESP,8




