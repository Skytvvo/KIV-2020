#pragma once
#include "pch.h"
#define SEPSTREMP  "\n;------------------------------\n"
#define SEPSTR(x)  "\n;----------- " + string(x) + " ------------\n"


#define BEGIN ".586\n"\
".model flat, stdcall\n"\
"includelib libucrt.lib\n"\
"includelib kernel32.lib\n"\
"includelib ..\\Debug\\FuncLib.lib\n"\
"ExitProcess PROTO:DWORD \n"\
"EXTRN random: PROC \n"\
"EXTRN countSin: PROC\n"\
"EXTRN strLength: PROC\n"\
"EXTRN outstr: PROC\n"\
"EXTRN outnum: PROC\n"\
"EXTRN outnumline: PROC\n"\
"EXTRN outstrline: PROC\n"\
"EXTRN system_pause:PROC\n"\
".stack 4096\n"


				

#define ITENTRY(x)  idtable.table[lextable.table[x].idxTI]
#define LEXEMA(x)   lextable.table[x].lexeme




namespace Gener
{
	bool CodeGeneration(LT::LexTable& lextable, IT::IdTable& idtable);
	int GenExpHandler(std::ofstream& AsmFile, LT::LexTable& LEXTABLE, IT::IdTable& idtable,int pos);
	IT::IDDATATYPE DetectType(LT::LexTable lextable, IT::IdTable idtable, int i);
};