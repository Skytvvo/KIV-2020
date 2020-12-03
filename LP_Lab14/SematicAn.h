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
	void CheckParamsOfFunc();
	void CheckParamsStdFunc();
	void Types();
	void CheckReturnInUserFunc(LT::LexTable& lextable, IT::IdTable& idtable);
	void CheckFuncByAmountParams();

	void BeginToCheck(Log::LOG& log);
}