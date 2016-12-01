;notes by Jesus7Freak

004D39BB   CC               INT3
004D39BC   CC               INT3
004D39BD   CC               INT3
004D39BE   CC               INT3
004D39BF   CC               INT3
;EDX=DATA_HEADER ptr
;
004D39C0   0FBF4A 22        MOVSX ECX,WORD PTR DS:[EDX+22]
004D39C4   53               PUSH EBX
004D39C5   66:8B5A 2C       MOV BX,WORD PTR DS:[EDX+2C]
004D39C9   55               PUSH EBP
004D39CA   8B6A 34          MOV EBP,DWORD PTR DS:[EDX+34]
004D39CD   56               PUSH ESI
004D39CE   0FBFF3           MOVSX ESI,BX
004D39D1   0FAFF1           IMUL ESI,ECX
004D39D4   83C8 FF          OR EAX,FFFFFFFF
004D39D7   03F5             ADD ESI,EBP
004D39D9   66:3B5A 20       CMP BX,WORD PTR DS:[EDX+20]
004D39DD   7D 60            JGE SHORT haloce.004D3A3F
004D39DF   90               NOP
004D39E0   66:833E 00       /CMP WORD PTR DS:[ESI],0
004D39E4   74 0D            |JE SHORT haloce.004D39F3
004D39E6   43               |INC EBX
004D39E7   03F1             |ADD ESI,ECX
004D39E9   66:3B5A 20       |CMP BX,WORD PTR DS:[EDX+20]
004D39ED  ^7C F1            \JL SHORT haloce.004D39E0
004D39EF   5E               POP ESI
004D39F0   5D               POP EBP
004D39F1   5B               POP EBX
004D39F2   C3               RETN
004D39F3   8BE9             MOV EBP,ECX
004D39F5   C1E9 02          SHR ECX,2
004D39F8   57               PUSH EDI
004D39F9   33C0             XOR EAX,EAX
004D39FB   8BFE             MOV EDI,ESI
004D39FD   F3:AB            REP STOS DWORD PTR ES:[EDI]
004D39FF   8BCD             MOV ECX,EBP
004D3A01   83E1 03          AND ECX,3
004D3A04   F3:AA            REP STOS BYTE PTR ES:[EDI]
004D3A06   66:8B42 32       MOV AX,WORD PTR DS:[EDX+32]
004D3A0A   66:8906          MOV WORD PTR DS:[ESI],AX
004D3A0D   66:FF42 32       INC WORD PTR DS:[EDX+32]
004D3A11   66:837A 32 00    CMP WORD PTR DS:[EDX+32],0
004D3A16   5F               POP EDI
004D3A17   75 06            JNZ SHORT haloce.004D3A1F
004D3A19   66:C742 32 0080  MOV WORD PTR DS:[EDX+32],8000
004D3A1F   66:FF42 30       INC WORD PTR DS:[EDX+30]
004D3A23   66:395A 2E       CMP WORD PTR DS:[EDX+2E],BX
004D3A27   8D43 01          LEA EAX,DWORD PTR DS:[EBX+1]
004D3A2A   66:8942 2C       MOV WORD PTR DS:[EDX+2C],AX
004D3A2E   7F 04            JG SHORT haloce.004D3A34
004D3A30   66:8942 2E       MOV WORD PTR DS:[EDX+2E],AX
004D3A34   0FBF06           MOVSX EAX,WORD PTR DS:[ESI]
004D3A37   0FBFCB           MOVSX ECX,BX
004D3A3A   C1E0 10          SHL EAX,10
004D3A3D   0BC1             OR EAX,ECX
004D3A3F   5E               POP ESI
004D3A40   5D               POP EBP
004D3A41   5B               POP EBX
004D3A42   C3               RETN
004D3A43   CC               INT3
004D3A44   CC               INT3
004D3A45   CC               INT3
004D3A46   CC               INT3
004D3A47   CC               INT3
004D3A48   CC               INT3
004D3A49   CC               INT3
004D3A4A   CC               INT3
004D3A4B   CC               INT3
004D3A4C   CC               INT3
004D3A4D   CC               INT3
004D3A4E   CC               INT3
004D3A4F   CC               INT3








004D3A43 CC               int         3    
004D3A44 CC               int         3    
004D3A45 CC               int         3    
004D3A46 CC               int         3    
004D3A47 CC               int         3    
004D3A48 CC               int         3    
004D3A49 CC               int         3    
004D3A4A CC               int         3    
004D3A4B CC               int         3    
004D3A4C CC               int         3    
004D3A4D CC               int         3    
004D3A4E CC               int         3    
004D3A4F CC               int         3 
;EDX=item tag
;EAX=DATA_HEADER ptr
;invalidate item from data header (all items with the same ID are invalidated)
004D3A50 56               push        esi  
004D3A51 8B F2            mov         esi,edx
004D3A53 C1 FE 10         sar         esi,10h ;SI= item ID
004D3A56 66 85 D2         test        dx,dx ;DX= item index
004D3A59 57               push        edi  
004D3A5A 7C 25            jl          004D3A81 
004D3A5C 66 3B 50 2E      cmp         dx,word ptr [eax+2Eh]  ;DATA_HEADER.NumOfItems
004D3A60 7D 1F            jge         004D3A81 
004D3A62 0F BF 48 22      movsx       ecx,word ptr [eax+22h] ;DATA_HEADER.ItemSize
004D3A66 0F BF FA         movsx       edi,dx 
004D3A69 0F AF CF         imul        ecx,edi 
004D3A6C 03 48 34         add         ecx,dword ptr [eax+34h] ;ECX=DATA_HEADER.ItemArray_ptrv[i] (ptr to item)
004D3A6F 66 8B 39         mov         di,word ptr [ecx] 
004D3A72 66 85 FF         test        di,di ;is ID valid?
004D3A75 74 0A            je          004D3A81 
004D3A77 66 85 F6         test        si,si ;is ID from param valid?
004D3A7A 74 07            je          004D3A83 
004D3A7C 66 3B F7         cmp         si,di 
004D3A7F 74 02            je          004D3A83 
004D3A81 33 C9            xor         ecx,ecx;crashes game
004D3A83 66 C7 01 00 00   mov         word ptr [ecx],0 ;invalidate item (change items ID to 0)
004D3A88 66 3B 50 2C      cmp         dx,word ptr [eax+2Ch] ;DATA_HEADER.LastItemIndexInvalidated?
004D3A8C 7D 04            jge         004D3A92 
004D3A8E 66 89 50 2C      mov         word ptr [eax+2Ch],dx 
004D3A92 0F BF 70 2E      movsx       esi,word ptr [eax+2Eh]
004D3A96 0F BF D2         movsx       edx,dx 
004D3A99 42               inc         edx  
004D3A9A 5F               pop         edi  
004D3A9B 3B D6            cmp         edx,esi 
004D3A9D 5E               pop         esi  
004D3A9E 75 17            jne         004D3AB7 
004D3AA0 0F BF 50 22      movsx       edx,word ptr [eax+22h]
004D3AA4 2B CA            sub         ecx,edx ;ECX=DATA_HEADER.ItemArray_ptrv[i - 1] (ptr to item) (moves down an index)
004D3AA6 66 FF 48 2E      dec         word ptr [eax+2Eh] ;decrease the num of items
004D3AAA 66 83 78 2E 00   cmp         word ptr [eax+2Eh],0 ;check if theres any more items
004D3AAF 7E 06            jle         004D3AB7 
004D3AB1 66 83 39 00      cmp         word ptr [ecx],0 ;test if that item is valid
004D3AB5 74 ED            je          004D3AA4 
004D3AB7 66 FF 48 30      dec         word ptr [eax+30h] ; decrease the DATA_HEADER.NextItemIndex
004D3ABB C3               ret              
004D3ABC CC               int         3    
004D3ABD CC               int         3    
004D3ABE CC               int         3    
004D3ABF CC               int         3    




004D3ABC   CC               INT3
004D3ABD   CC               INT3
004D3ABE   CC               INT3
004D3ABF   CC               INT3
;ESI=DATA_HEADER ptr
;reset DATA_HEADER
004D3AC0   57               PUSH EDI
004D3AC1   6A 02            PUSH 2
004D3AC3   8D7E 32          LEA EDI,DWORD PTR DS:[ESI+32]
004D3AC6   56               PUSH ESI
004D3AC7   57               PUSH EDI
004D3AC8   66:C746 2E 0000  MOV WORD PTR DS:[ESI+2E],0
004D3ACE   66:C746 30 0000  MOV WORD PTR DS:[ESI+30],0
004D3AD4   66:C746 2C 0000  MOV WORD PTR DS:[ESI+2C],0
004D3ADA   E8 01540F00      CALL haloce.005C8EE0
004D3ADF   66:810F 0080     OR WORD PTR DS:[EDI],8000
004D3AE4   83C4 0C          ADD ESP,0C
004D3AE7   33C0             XOR EAX,EAX
004D3AE9   66:3946 20       CMP WORD PTR DS:[ESI+20],AX
004D3AED   5F               POP EDI
004D3AEE   7E 1A            JLE SHORT haloce.004D3B0A
004D3AF0   0FBF4E 22        /MOVSX ECX,WORD PTR DS:[ESI+22]
004D3AF4   0FBFD0           |MOVSX EDX,AX
004D3AF7   0FAFCA           |IMUL ECX,EDX
004D3AFA   8B56 34          |MOV EDX,DWORD PTR DS:[ESI+34]
004D3AFD   40               |INC EAX
004D3AFE   66:C70411 0000   |MOV WORD PTR DS:[ECX+EDX],0
004D3B04   66:3B46 20       |CMP AX,WORD PTR DS:[ESI+20]
004D3B08  ^7C E6            \JL SHORT haloce.004D3AF0
004D3B0A   C3               RETN
004D3B0B   CC               INT3
004D3B0C   CC               INT3
004D3B0D   CC               INT3
004D3B0E   CC               INT3
004D3B0F   CC               INT3

