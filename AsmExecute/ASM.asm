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
	fi5 DWORD 5
	fisd BYTE 'sd', 0
	anothersd BYTE 'sd', 0
	main5 DWORD 5
	mainhello BYTE 'hello', 0
	main5 DWORD 5
	main0 DWORD 0
.data
	fisum DWORD ?
	fis DWORD ?
	anothersum DWORD ?
	anothers DWORD ?
	mainx DWORD ?
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
pop fis
push fisum
pop eax
pop eax
ret
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
push OFFSET anothersd
pop eax
push eax
pop anothers
push anothersum
pop eax
pop eax
ret
another ENDP

main PROC
push main5
pop eax
push eax
pop mainx
push mainx
pop eax
push eax
call outnum
push OFFSET mainhello
pop eax
push eax
pop mainstr
push main5
pop eax
push eax
pop mainx
push  mainstr
pop eax
push eax
call outstr
push main0
pop eax
	call		ExitProcess
main ENDP
END main