.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib ..\Debug\FuncLib.lib
ExitProcess PROTO:DWORD 
EXTRN random: PROC 
EXTRN toPow: PROC
EXTRN strLength: PROC
EXTRN outstr: PROC
EXTRN outnum: PROC
EXTRN outnumline: PROC
EXTRN outstrline: PROC
EXTRN system_pause:PROC
.stack 4096
.const
ZEROMESSAGE  BYTE 'Ошибка:деление на ноль',0
OVERFLOWMESSAGE  BYTE 'Ошибка:переполнение типа',0
	fi5 DWORD 5
	fisd BYTE 'sd', 0
	fis BYTE 's', 0
	anothersd BYTE 'sd', 0
	another1 DWORD 1
	5 DWORD 5
	1 DWORD 1
	4294967295 DWORD 2147483647
	2 DWORD 2
	3 DWORD 3
	hello BYTE 'hello', 0
	4 DWORD 4
.data
	fisum1 DWORD ?
	fish1 DWORD ?
	anothersum1 DWORD ?
	anothers1 DWORD ?
	x1 DWORD ?
	variable1 DWORD ?
	str1 DWORD ?

.code
fi PROC finum1 :  DWORD 
push fi5
pop eax
push eax
pop fisum1
push OFFSET fisd
pop eax
push eax
pop fish1
push OFFSET fis
pop eax
push eax
call outstr
push fisum1
pop eax
push eax
call outnum
push OFFSET fis
pop eax
push eax
call outstr
push fisum1
pop eax
 
ret
ZEROERROR:
push OFFSET ZEROMESSAGE
call outstrline
push -1
	call		ExitProcess
OVERFLOW:
push OFFSET OVERFLOWMESSAGE
call outstrline
push -1
	call		ExitProcess
fi ENDP

another PROC anothera1 :  DWORD , anotherb1 :  DWORD 
push anothera1
push anotherb1
pop eax
pop ebx
add eax,ebx
push eax
jo OVERFLOW
pop eax
push eax
pop anothersum1
push OFFSET anothersd
pop eax
push eax
pop anothers1
push another1
call fi
 push eax
push another1
pop eax
pop ebx
add eax,ebx
push eax
jo OVERFLOW
pop eax
 
ret
ZEROERROR:
push OFFSET ZEROMESSAGE
call outstrline
push -1
	call		ExitProcess
OVERFLOW:
push OFFSET OVERFLOWMESSAGE
call outstrline
push -1
	call		ExitProcess
another ENDP

push 5
pop eax
push eax
pop another1
