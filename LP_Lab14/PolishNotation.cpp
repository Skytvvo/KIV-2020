#include "pch.h"
#include "PolishNotation.h"

int getOperationsPriority(char operation) {
	if (operation == LEX_LEFTHESIS || operation == LEX_RIGHTHESIS)
		return 1;
	else if (operation == LEX_PLUS || operation == LEX_MINUS)
		return 2;
	else if (operation == LEX_STAR || operation == LEX_DIRSLASH)
		return 3;
	return EOF;
}

std::vector<LT::Entry> convert(std::vector<LT::Entry> entries) {
	std::vector<LT::Entry> output;
	std::stack<LT::Entry> stack;

	for (int i = 0; entries[i].lexeme != LEX_SEMICOLON; ++i) {
		const LT::Entry& entry = entries[i];

		if (entry.lexeme == LEX_PLUS || entry.lexeme == LEX_MINUS || entry.lexeme == LEX_STAR || entry.lexeme == LEX_DIRSLASH) {
			if (!stack.empty() && stack.top().lexeme != LEX_LEFTHESIS) {
				while (!stack.empty() && getOperationsPriority(entry.lexeme) <= getOperationsPriority(stack.top().lexeme)) {
					output.push_back(stack.top());
					stack.pop();
				}
			}

			stack.push(entry);
		}
		else if (entry.lexeme == LEX_COMMA) {
			while (!stack.empty() && stack.top().lexeme != LEX_LEFTHESIS) {
				output.push_back(stack.top());
				stack.pop();
			}
		}
		else if (entry.lexeme == LEX_LEFTHESIS) {
			stack.push(entry);
		}
		else if (entry.lexeme == LEX_RIGHTHESIS) {
			while (stack.top().lexeme != LEX_LEFTHESIS) {
				output.push_back(stack.top());
				stack.pop();
			}
			stack.pop();

			if (!stack.empty() && stack.top().lexeme == POLISH_FUNCTION) {
				output.push_back(stack.top());
				stack.pop();
			}
		}
		else if (entry.lexeme == LEX_FUNCTION|| entry.lexeme == LEX_LENGTH || 
			entry.lexeme == LEX_POW|| entry.lexeme == LEX_RAND|| entry.lexeme == LEX_SIN) {
			LT::Entry tmp = entry;
			tmp.lexeme = POLISH_FUNCTION;
			stack.push(tmp);
		}
		else {
			output.push_back(entry);
		}
	}

	while (!stack.empty()) {
		output.push_back(stack.top());
		stack.pop();
	}

	output.push_back(entries.back());

	return output;
}

bool PolishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable) {
	std::vector<LT::Entry> infixExpressionEntries;
	int lefthesisCounter = 0, rightesisCounter = 0;
	int operandsCounter = 0, operationsCounter = 0;

	for (int i = lextable_pos; i < lextable.size; ++i) {
		const char& lexeme = lextable.table[i].lexeme;
		
		if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F) {
			LT::Entry tmp = LT::GetEntry(lextable, i);
			tmp.lexeme = LEX_FUNCTION;
			infixExpressionEntries.push_back(tmp);

			operandsCounter--;
		}
		else {
			infixExpressionEntries.push_back(LT::GetEntry(lextable, i));

			if (lexeme == LEX_COMMA)
				operandsCounter--;
		}

		if (lexeme == LEX_LEFTHESIS)
			lefthesisCounter++;
		else if (lexeme == LEX_RIGHTHESIS)
			rightesisCounter++;
		else if (lexeme == LEX_PLUS || lexeme == LEX_MINUS || lexeme == LEX_STAR || lexeme == LEX_DIRSLASH)
			operationsCounter++;
		else if (lexeme == LEX_ID || lexeme == LEX_INTEGER_LITERAL || lexeme == LEX_STRING_LITERAL || lexeme == LEX_DOUBLE_LITERAL)
			operandsCounter++;

		if (lexeme == LEX_SEMICOLON)
			break;
	}

	if (lefthesisCounter != rightesisCounter || operandsCounter - operationsCounter != 1)
		return false;

	std::vector<LT::Entry> postfixExpressionEntries = convert(infixExpressionEntries);
	for (int i = 0; i < (int)infixExpressionEntries.size(); ++i) {
		if (i < (int)postfixExpressionEntries.size()) {
			lextable.table[i + lextable_pos] = postfixExpressionEntries[i];
		}
		else {
			lextable.table[i + lextable_pos] = { FORBIDDEN_SYMBOL, EOF, EOF };
		}
	}

	return true;
}