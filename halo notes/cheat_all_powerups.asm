;notes by Jesus7Freak
;haloce1.09 p0 bloodgulch
;console 004C9BE3
...1 ;inspect
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F5DD8   haloce.0048C8D0
...2 ;cheat_all_powerups
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F6DA4   haloce.0047FC30
...3 ;inspect
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F5DD8   haloce.0048C8D0




...2
0048D1E0   51               PUSH ECX;ECX=403D8501
0048D1E1   57               PUSH EDI;EDI=F3690000
0048D1E2   50               PUSH EAX;EAX=00000095
0048D1E3   FF52 0C          CALL DWORD PTR DS:[EDX+C] ;EDX=005F6DA4   haloce.0047FC30
->
0047FC30   A1 80226E00      MOV EAX,DWORD PTR DS:[6E2280];object list->EAX=40848198
0047FC35   8B88 58010000    MOV ECX,DWORD PTR DS:[EAX+158];->ECX=00000005 (5 powers ups avaiable)
0047FC3B   85C9             TEST ECX,ECX
0047FC3D   74 08            JE SHORT haloce.0047FC47
0047FC3F   8B88 5C010000    MOV ECX,DWORD PTR DS:[EAX+15C];->ECX=40848AF4
0047FC45   EB 02            JMP SHORT haloce.0047FC49
...
0047FC49   0FBF80 58010000  MOVSX EAX,WORD PTR DS:[EAX+158]

0047FC50   50               PUSH EAX;EAX=00000005
0047FC51   51               PUSH ECX;ECX=40848AF4
0047FC52   E8 29B7FDFF      CALL haloce.0045B380;returns (WORD) how many objects spawned
0047FC57   8B4C24 10        MOV ECX,DWORD PTR SS:[ESP+10];restore ECX=F3690000
0047FC5B   83C4 08          ADD ESP,8

0047FC5E   33C0             XOR EAX,EAX;EAX=00000000
0047FC60   E9 2BD70000      JMP haloce.0048D390
...;scripting part
0048D390   81E1 FFFF0000    AND ECX,0FFFF
0048D396   56               PUSH ESI
0048D397   8BF1             MOV ESI,ECX
0048D399   8B0D B0558100    MOV ECX,DWORD PTR DS:[8155B0];hs thread
0048D39F   69F6 18020000    IMUL ESI,ESI,218
0048D3A5   57               PUSH EDI
0048D3A6   8B79 34          MOV EDI,DWORD PTR DS:[ECX+34]
0048D3A9   8B5437 10        MOV EDX,DWORD PTR DS:[EDI+ESI+10]
0048D3AD   8B4A 04          MOV ECX,DWORD PTR DS:[EDX+4]
0048D3B0   8B15 B4558100    MOV EDX,DWORD PTR DS:[8155B4];script node
0048D3B6   8B52 34          MOV EDX,DWORD PTR DS:[EDX+34]
0048D3B9   03FE             ADD EDI,ESI
0048D3BB   81E1 FFFF0000    AND ECX,0FFFF
0048D3C1   8D0C89           LEA ECX,DWORD PTR DS:[ECX+ECX*4]
0048D3C4   8D0C8A           LEA ECX,DWORD PTR DS:[EDX+ECX*4]
0048D3C7   F641 06 02       TEST BYTE PTR DS:[ECX+6],2
0048D3CB   0FBF51 02        MOVSX EDX,WORD PTR DS:[ECX+2]
0048D3CF   75 0C            JNZ SHORT haloce.0048D3DD

