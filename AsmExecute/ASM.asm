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
	countsumcalculating_sum BYTE 'calculating sum', 0
	coutsomethcalculating_combocalculating_combo BYTE 'calculating combo', 0
	main4 DWORD 4
	main5 DWORD 5
	main10 DWORD 10
	maincounting_more_then_10 BYTE 'counting more then 10', 0
	maincounting_less_then_10 BYTE 'counting less then 10', 0
	main0 DWORD 0
	main6 DWORD 6
	maingo BYTE 'go', 0
	main1 DWORD 1
	main3 DWORD 3
	mainnew_calculating BYTE 'new calculating', 0
	main7 DWORD 7
.data
	mainoperation1 DWORD ?
	maincounter1 DWORD ?
	mainmessage1 DWORD ?

.code
countsum PROC countsuma1 :  DWORD , countsumb1 :  DWORD 
push OFFSET countsumcalculating_sum
pop eax
push eax
call outstrline
push countsuma1
push countsumb1
pop eax
pop ebx
add eax,ebx
push eax
jc OVERFLOW
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
countsum ENDP

coutsomethingelse PROC coutsomethaa1 :  DWORD , coutsomethbb1 :  DWORD 
push OFFSET coutsomethcalculating_combocalculating_combo
pop eax
push eax
call outstrline
push coutsomethaa1
push coutsomethbb1
pop eax
pop ebx
add eax,ebx
push eax
jc OVERFLOW
push coutsomethaa1
push coutsomethbb1
call countsum
 push eax
pop eax
pop ebx
add eax,ebx
push eax
jc OVERFLOW
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
coutsomethingelse ENDP

main PROC
push main4
push main5
call coutsomethingelse
 push eax
pop eax
push eax
pop mainoperation1
push mainoperation1
pop eax
push eax
call outnumline
push mainoperation1
push main10
pop ebx
pop eax
cmp eax, ebx
jb SKIP17
push OFFSET maincounting_more_then_10
pop eax
push eax
call outstrline
jae SKIPELSE21
SKIP17:
push OFFSET maincounting_less_then_10
pop eax
push eax
call outstrline
push OFFSET maincounting_less_then_10
pop eax
push eax
call outstrline
SKIPELSE21:
push main0
pop eax
push eax
pop maincounter1
TOWHILE29:
push maincounter1
push main6
pop ebx
pop eax
cmp eax, ebx
jae SKIP29
push maincounter1
call random
 pop ecx
 push eax
push OFFSET maingo
call strLength
 pop ecx
 push eax
push maincounter1
push maincounter1
call toPow
 pop ecx
 push eax
pop ebx
pop eax
mul ebx
push eax
jc OVERFLOW
pop eax
pop ebx
add eax,ebx
push eax
jc OVERFLOW
pop eax
push eax
pop mainoperation1
push mainoperation1
push main1
pop ebx
pop eax
sub eax,ebx
push eax
jc OVERFLOW
push main3

	pop ebx
	pop eax
test ebx, ebx
jz ZEROERROR
	cdq
	div ebx
	push eax
jc OVERFLOW
pop eax
push eax
pop mainoperation1
push mainoperation1
pop eax
push eax
call outnumline
push maincounter1
push main1
pop eax
pop ebx
add eax,ebx
push eax
jc OVERFLOW
pop eax
push eax
pop maincounter1
jmp TOWHILE29
SKIP29:
push OFFSET mainnew_calculating
pop eax
push eax
pop mainmessage1
push mainoperation1
push main7

	pop ebx
	pop eax
test ebx, ebx
jz ZEROERROR
	cdq
	div ebx
	push edx
jc OVERFLOW
push main1
pop eax
pop ebx
add eax,ebx
push eax
jc OVERFLOW
pop eax
push eax
pop mainoperation1
push mainoperation1
push main5
pop ebx
pop eax
cmp eax, ebx
jae SKIP39
push  mainmessage1
pop eax
push eax
call outstrline
push mainoperation1
pop eax
push eax
call outnumline
SKIP39:
push main0
pop eax
push eax
call	system_pause
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