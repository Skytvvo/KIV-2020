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
	main5 DWORD 5
	main1 DWORD 1
	main4294967295 DWORD 2147483647
	main2 DWORD 2
	main3 DWORD 3
	mainhello BYTE 'hello', 0
	main4 DWORD 4
.data
	fisum1 DWORD ?
	fish1 DWORD ?
	anothersum1 DWORD ?
	anothers1 DWORD ?
	mainmore1 DWORD ?
	mainx1 DWORD ?
	mainvariable1 DWORD ?
	mainstr1 DWORD ?

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

main PROC
push main5
pop eax
push eax
pop mainmore1
push mainmore1
push main5
pop ebx
pop eax
cmp eax, ebx
jne SKIP23
push main1
pop eax
push eax
call outnum
SKIP23:
push main4294967295
pop eax
push eax
pop mainx1
push main2
push main3
call toPow
 pop ecx
 push eax
push main1
pop eax
pop ebx
add eax,ebx
push eax
jo OVERFLOW
pop eax
push eax
pop mainvariable1
push mainvariable1
pop eax
push eax
call outnum
push OFFSET mainhello
pop eax
push eax
pop mainstr1
push main5
call fi
 push eax
pop eax
push eax
pop mainx1
push main1
pop eax
push eax
call outnum
push  mainstr1
pop eax
push eax
call outstr
push main1
push main2
call another
 push eax
pop eax
push eax
pop mainx1
push main3
push main4
call toPow
 pop ecx
 push eax
pop eax
push eax
pop mainx1
push main1
call random
 pop ecx
 push eax
pop eax
push eax
pop mainx1
push mainx1
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
jo OVERFLOW
pop eax
push eax
	call		ExitProcess
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
 main ENDP
END main