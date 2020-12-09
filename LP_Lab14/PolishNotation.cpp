
#include "pch.h"
//
//namespace PN
//{
//	int i = 0, k = 0;
//
//	int GetExpr(LT::LexTable lexTable, int i)
//	{
//		for (; lexTable.table[i].lexeme != LEX_SEMICOLON; i++);
//		return i;
//	}
//
//	int Priorities(char operation)
//	{
//		if (operation == LEX_LEFTHESIS || operation == LEX_RIGHTHESIS)
//			return 1;
//		if (operation == LEX_MINUS || operation == LEX_PLUS)
//			return 2;
//		if (operation == LEX_DIRSLASH || operation == LEX_STAR)
//			return 3;
//	}
//
//	void ConverExpr(LT::Entry* expr, LT::LexTable lexTable)
//	{
//		std::stack<LT::Entry> stack;
//		short leftBracket = 0;
//
//		for (int j = GetExpr(lexTable, ++i); i < j; i++)
//		{
//			if (lexTable.table[i].lexeme == LEX_ID || lexTable.table[i].lexeme == LEX_LITERAL)
//			{
//				expr[k++] = lexTable.table[i];
//			}
//			else if (lexTable.table[i].lexeme == LEX_RIGHTHESIS)
//			{
//				if (leftBracket == 0)	throw ERROR_THROW_IN(123, lexTable.table[i].sn, -1);
//				while (stack.size())
//				{
//					if (stack.top().lexeme == LEX_LEFTHESIS)	break;
//					expr[k++] = stack.top();
//					stack.pop();
//				}
//				stack.pop();	leftBracket--;
//			}
//			else if (lexTable.table[i].lexeme == LEX_LEFTHESIS)
//			{
//				leftBracket++;
//				stack.push(lexTable.table[i]);
//			}
//			else if (stack.size() == 0 || stack.top().lexeme == LEX_LEFTHESIS)
//			{
//				if (!CHECKOPERAND && lexTable.table[i].lexeme != LEX_LEFTHESIS) throw ERROR_THROW_IN(121, lexTable.table[i].sn, -1);
//				stack.push(lexTable.table[i]);
//			}
//			else
//			{
//				if (!CHECKOPERAND) throw ERROR_THROW_IN(121, lexTable.table[i].sn, -1);
//				while (stack.size())
//				{
//					if (Priorities(lexTable.table[i].lexeme) > Priorities(stack.top().lexeme)) break;
//					expr[k++] = stack.top();
//					stack.pop();
//				}
//				stack.push(lexTable.table[i]);
//			}
//		}
//		if (leftBracket != 0) throw ERROR_THROW_IN(123, lexTable.table[i - 1].sn, -1);
//		while (stack.size() != 0)
//		{
//			expr[k++] = stack.top();
//			stack.pop();
//		}
//	}
//
//	void AddNewExpr(LT::LexTable& lexTable, IT::IdTable& idTable, LT::Entry* expr, int numConver)
//	{
//		for (int j = numConver, g = 0; g < k; j++, g++)
//		{
//			lexTable.table[j] = expr[g];
//			if (ELEMIT)		idTable.table[lexTable.table[j].idxTI].idxfirstLE = j;
//		}
//	}
//
//	void DelNULLEntryLT(LT::LexTable& lexTable, IT::IdTable& idTable, int numConver)
//	{
//		for (int r = i - (numConver + k), g = 0; g < r; g++)
//		{
//			for (int j = numConver + k; j < lexTable.size; j++)
//			{
//				lexTable.table[j] = lexTable.table[j + 1];
//				if (ELEMIT)		idTable.table[lexTable.table[j].idxTI].idxfirstLE = j;
//			}
//			lexTable.size--;
//		}
//	}
//
//	void PolishNotation(LT::LexTable& lexTable, IT::IdTable& idTable)
//	{
//		Check check;
//		LT::Entry expr[200];
//		int numConver = 0;
//		for (; i < lexTable.size; i++, k = 0)
//		{
//			if (lexTable.table[i].lexeme == LEX_EQUALS)
//			{
//				if (!CHECKLVALUE)	throw ERROR_THROW_IN(122, lexTable.table[i].sn, -1);
//				numConver = i + 1;
//				ConverExpr(expr, lexTable);
//				AddNewExpr(lexTable, idTable, expr, numConver);
//				DelNULLEntryLT(lexTable, idTable, numConver);
//			}
//		}
//	}
//}
//
