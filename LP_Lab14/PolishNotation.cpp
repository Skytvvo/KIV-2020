
#include "pch.h"

namespace PN
{
	
	
	int GetExpr(LT::LexTable lexTable, int i)
	{
		for (; lexTable.table[i].lexeme != LEX_SEMICOLON; i++);
		return i;
	}

	int Priorities(char operation)
	{
		if (operation == LEX_LEFTHESIS || operation == LEX_RIGHTHESIS)
			return 1;
		if (operation == LEX_MINUS || operation == LEX_PLUS)
			return 2;
		if (operation == LEX_DIRSLASH || operation == LEX_STAR||operation == LEX_MOD)
			return 3;
	}

	int ConverExpr(LT::Entry* expr, LT::LexTable lexTable, IT::IdTable idtable, int pos)
	{
		std::stack<LT::Entry> stack;
		int sizeExpr = 0;
		short leftBracket = 0;

		for (int j = GetExpr(lexTable, pos); pos < j; pos++)
		{
			if ((lexTable.table[pos].lexeme == LEX_ID || lexTable.table[pos].lexeme == LEX_LITERAL) && !CHECKFUNCTIONS(pos))
			{
				expr[sizeExpr++] = lexTable.table[pos];
			}
			else if (lexTable.table[pos].idxTI!=LT_TI_NULLIDX && CHECKFUNCTIONS(pos))
			{
				LT::Entry Copy = lexTable.table[pos];
				Copy.lexeme = PL_AT;
				pos++;
				for (int parmIndex = 0; parmIndex < idtable.table[Copy.idxTI].value.params.amount; pos++)
				{
					if (lexTable.table[pos].lexeme == LEX_ID || lexTable.table[pos].lexeme == LEX_LITERAL)
					{
						expr[sizeExpr++] = lexTable.table[pos];
						parmIndex++;
					}
				}
				expr[sizeExpr++] = Copy;
			}
			else if (lexTable.table[pos].lexeme == LEX_RIGHTHESIS)
			{
				
				while (stack.size())
				{
					if (stack.top().lexeme == LEX_LEFTHESIS)	break;
					expr[sizeExpr++] = stack.top();
					stack.pop();
				}
				stack.pop();	leftBracket--;
			}
			else if (lexTable.table[pos].lexeme == LEX_LEFTHESIS)
			{
				leftBracket++;
				stack.push(lexTable.table[pos]);
			}
			else if (stack.size() == 0 || stack.top().lexeme == LEX_LEFTHESIS)
			{
				
				stack.push(lexTable.table[pos]);
			}
			else
			{
				
				while (stack.size())
				{
					if (Priorities(lexTable.table[pos].lexeme) > Priorities(stack.top().lexeme)) break;
					expr[sizeExpr++] = stack.top();
					stack.pop();
				}
				stack.push(lexTable.table[pos]);
			}
		}
		
		while (stack.size() != 0)
		{
			expr[sizeExpr++] = stack.top();
			stack.pop();
		}
	
		std::cout << std::endl;
		return sizeExpr;
	}

	void AddNewExpr(LT::LexTable& lexTable, IT::IdTable& idTable, LT::Entry* expr, int numConver, int sizeExpr)
	{
		for (int j = numConver, g = 0; g < sizeExpr; j++, g++)
		{
			lexTable.table[j] = expr[g];
			if (ELEMIT)		idTable.table[lexTable.table[j].idxTI].idxfirstLE = j;
		}
	}

	void DelNULLEntryLT(LT::LexTable& lexTable, IT::IdTable& idTable, int numConver,int sizeExpr, int endExpr)
	{
		for (int r = endExpr - (numConver + sizeExpr), g = 0; g < r; g++)
		{
			for (int j = numConver + sizeExpr; j < lexTable.size; j++)
			{
				lexTable.table[j] = lexTable.table[j + 1];
				if (ELEMIT)		idTable.table[lexTable.table[j].idxTI].idxfirstLE = j;
			}
			lexTable.size--;
		}
	}

	void PolishNotation(LT::LexTable& lexTable, IT::IdTable& idTable)
	{
		Check check;
		LT::Entry expr[200];
		int numConver = 0;
		for (int i = 0; i < lexTable.size; i++)
		{
			if (lexTable.table[i].lexeme == LEX_EQUALS || lexTable.table[i].lexeme == LEX_RETURN || lexTable.table[i].lexeme == LEX_PRINT)
			{
		
				numConver = i + 1;
				int endExpr = GetExpr(lexTable, numConver);
				int sizeExpr = ConverExpr(expr, lexTable, idTable, numConver);
				AddNewExpr(lexTable, idTable, expr, numConver,sizeExpr);
				DelNULLEntryLT(lexTable, idTable, numConver,sizeExpr,endExpr);
			}
		}
	}
}

