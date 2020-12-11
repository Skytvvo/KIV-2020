.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib ..\Debug\FuncLib.lib
ExitProcess PROTO:DWORD 
EXTRN random: PROC 
EXTRN countSin: PROC
EXTRN strLength: PROC
EXTRN outstr: PROC
EXTRN outnum: PROC
EXTRN outnumline: PROC
EXTRN outstrline: PROC
EXTRN system_pause:PROC
.stack 4096
.const
ZEROMESSAGE  BYTE 'Ошибка:деление на ноль',0
	fi5 DWORD 5
	fisd BYTE 'sd', 0
	fis BYTE 's', 0
	another1 DWORD 1
	main5 DWORD 5
	main1 DWORD 1
	mainhello BYTE 'hello', 0
	main2 DWORD 2
.data
	fisum DWORD ?
	fish DWORD ?
	anothersum DWORD ?
	anothers DWORD ?
	mainx DWORD ?
	mainvariable DWORD ?
	mainstr DWORD ?

.code
fi PROC finum :  DWORD 
push fi5
pop eax
push eax
pop fisum
push OFFSET fisd
pop eax
push eax
pop fish
push OFFSET fis
pop eax
push eax
call outstr
push fisum
pop eax
push eax
call outnum
push OFFSET fis
pop eax
push eax
call outstr
push fisum
pop eax
 
ret
ZEROERROR:
push OFFSET ZEROMESSAGE
call outstrline
push -1
	call		ExitProcess
fi ENDP

another PROC anothera :  DWORD , anotherb :  DWORD 
push anothera
push anotherb
pop eax
pop ebx
add eax,ebx
push eax
pop eax
push eax
pop anothersum
push  anothers
pop eax
push eax
pop anothers
push another1
call fi
 push eax
push another1
pop eax
pop ebx
add eax,ebx
push eax
pop eax
 
ret
ZEROERROR:
push OFFSET ZEROMESSAGE
call outstrline
push -1
	call		ExitProcess
another ENDP

main PROC
push main5
push main5
pop ebx
pop eax
cmp eax, ebx
jne SKIP22
push main1
pop eax
push eax
call outnum
SKIP22:
push main5
pop eax
push eax
pop mainx
push main1
pop eax
push eax
pop mainvariable
push mainvariable
pop eax
push eax
call outnum
push OFFSET mainhello
pop eax
push eax
pop mainstr
push main5
call fi
 push eax
pop eax
push eax
pop mainx
push main1
pop eax
push eax
call outnum
push  mainstr
pop eax
push eax
call outstr
push main1
push main2
call another
 push eax
pop eax
push eax
pop mainx
push mainx
pop eax
push eax
call outnum
push main1
push main2
call another
 push eax
push main5
pop eax
pop ebx
add eax,ebx
push eax
pop eax
push eax
	call		ExitProcess
ZEROERROR:
push OFFSET ZEROMESSAGE
call outstrline
push -1
	call		ExitProcess
main ENDP
END main