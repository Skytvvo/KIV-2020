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
	main0 DWORD 0
.data
	mainoperation1 DWORD ?

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
jo OVERFLOW
push coutsomethaa1
push coutsomethbb1
call countsum
 push eax
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