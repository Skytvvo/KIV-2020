//#pragma once
//#include "IT.h"
//#include "LT.h"
//#include <stack>
//#include <iostream>
//#include "Error.h"
//
//#define CHECKOPERAND	((lexTable.table[i - 1].lexeme == LEX_ID ||\
//	lexTable.table[i - 1].lexeme == LEX_RIGHTHESIS ||\
//	lexTable.table[i - 1].lexeme == LEX_LITERAL) &&\
//	(lexTable.table[i + 1].lexeme == LEX_ID ||\
//	lexTable.table[i + 1].lexeme == LEX_LITERAL ||\
//	lexTable.table[i + 1].lexeme == LEX_LEFTHESIS) ||\
//	(lexTable.table[i - 1].lexeme == LEX_EQUALS &&\
//	(lexTable.table[i].lexeme == LEX_PLUS || lexTable.table[i].lexeme == LEX_MINUS)))
//
//#define CHECKLVALUE		(lexTable.table[i - 1].lexeme == LEX_ID &&\
//	lexTable.table[i - 2].lexeme == LEX_SEMICOLON)
//
//#define ELEMIT			lexTable.table[j].lexeme == LEX_ID ||\
//	lexTable.table[j].lexeme == LEX_LITERAL
//
//namespace PN
//{
//	struct Check
//	{
//		short operand = 0;
//		short operation = 0;
//
//	};
//
//	int GetExpr(LT::LexTable lexTable, int i);
//	void PolishNotation(LT::LexTable& lexTable, IT::IdTable& idTable);
//	int Priorities(char operation);
//	void ConverExpr(LT::Entry* expr, LT::LexTable lexTable);
//	void AddNewExpr(LT::LexTable& lexTable, IT::IdTable& idTable, LT::Entry* expr, int numConver);
//	void DelNULLEntryLT(LT::LexTable& lexTable, IT::IdTable& idTable, int numConver);
//}