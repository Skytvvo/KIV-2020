#pragma once
#include "pch.h"

#define SEPSTREMP  "\n;------------------------------\n"
#define SEPSTR(x)  "\n;----------- " + string(x) + " ------------\n"


#define BEGIN ".586\n"\
".model flat, stdcall\n"\
"includelib libucrt.lib\n"\
"includelib kernel32.lib\n"\
"includelib \"..\\Debug\\BAAlib.lib\"\n"\
"ExitProcess PROTO:DWORD \n"\
".stack 4096\n"

#define END "call system_pause"\
				"\npush 0"\
				"\ncall ExitProcess"\
				"\nSOMETHINGWRONG:"\
				"\npush offset null_division"\
				"\ncall outstrline\n"\
				"call system_pause"\
				"\npush -1"\
				"\ncall ExitProcess"\
				"\nEXIT_OVERFLOW:"\
				"\npush offset overflow"\
				"\ncall outstrline\n"\
				"call system_pause"\
				"\npush -2"\
				"\ncall ExitProcess"\
				"\nmain ENDP\nend main"


#define EXTERN "\n outnum PROTO : DWORD\n"\
"\n outstr PROTO : DWORD\n"\
"\n outstrline PROTO : DWORD\n"\
"\n outnumline PROTO : DWORD\n"\
"\n system_pause PROTO \n"\
"\n random PROTO  : DWORD\n"\
"\n lenght PROTO  : DWORD\n"\
"\n power PROTO : DWORD, : DWORD\n"


#define ITENTRY(x)  idtable.table[lextable.table[x].idxTI]
#define LEXEMA(x)   lextable.table[x].lexeme


namespace Gener
{
	bool CodeGeneration(LT::LexTable& lttable,IT::IdTable& idtable, Parm::PARM& parm, Log::LOG& log);
};