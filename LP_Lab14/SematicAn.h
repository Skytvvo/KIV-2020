#pragma once
#include "pch.h"

namespace SeAn
{
	struct  Semantic
	{
		LT::LexTable lextable;
		IT::IdTable idtable;
		Semantic(LT::LexTable lextable, IT::IdTable idtable)
		{
			this->lextable = lextable;
			this->idtable = idtable;
		}
		
	};
	void CheckingReturnInMain(LT::LexTable& lextable, IT::IdTable& idtable);
	void CheckParamsOfFunc(LT::LexTable& lextable, IT::IdTable& idtable);
	void CheckParamsStdFunc(LT::LexTable& lextable, IT::IdTable& idtable);
	
	void Types(LT::LexTable& lextable, IT::IdTable& idtable);
	void CheckReturnInUserFunc(LT::LexTable& lextable, IT::IdTable& idtable);
	
	bool FindSTD(unsigned char lex);
}