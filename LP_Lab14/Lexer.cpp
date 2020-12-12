#include "pch.h"
#include "Lexer.h"
#include "FST.h"


char LA::Tokenize(const char* string) {
	FST::FST nanomachinesSon[] = {FST_ELSE, FST_IF,FST_WHILE,FST_LEFT_SQUARE_BRACE,FST_RIGHT_SQUARE_BRACE,
		FST_INTEGER, FST_STRING, FST_FUNCTION, FST_DECLARE,
		FST_RETURN, FST_PRINT, FST_MAIN,
		FST_LEFTHESIS, FST_RIGHTHESIS, FST_SEMICOLON, FST_COMMA,
		FST_LEFTBRACE, FST_BRACELET,
		FST_PLUS, FST_MINUS, FST_STAR, FST_DIRSLASH, FST_EQUALS,
		 FST_STRING_LITERAL, FST_INTEGER_LITERAL, FST_DOUBLE, FST_UINT, GRAPH_CONCAT,
		GRAPH_POW,GRAPH_RANDOM,GRAPH_SIN, FST_MOD, FST_BIGGEROREQUAL,FST_SMALLEROREQUAL,FST_NOTEQUAL,FST_SMALLERTHAN,
		FST_BIGGERTHAN,FST_EQUALS_TWO,FST_DOUBLE_LITERAL,GRAPH_LENGTH ,FST_ID
	};	
	const int size = sizeof(nanomachinesSon) / sizeof(nanomachinesSon[0]); 
	const char tokens[] = {LEX_ELSE, LEX_IF, LEX_WHILE,LEX_LEFT_SQUARE_BRACE,LEX_RIGHT_SQUARE_BRACE,

		LEX_INTEGER, LEX_STRING, LEX_FUNCTION, LEX_DECLARE,
		LEX_RETURN, LEX_PRINT, LEX_MAIN,
		LEX_LEFTHESIS, LEX_RIGHTHESIS, LEX_SEMICOLON, LEX_COMMA,
		LEX_LEFTBRACE, LEX_BRACELET,
		LEX_PLUS, LEX_MINUS, LEX_STAR, LEX_DIRSLASH, LEX_EQUALS,
		 LEX_STRING_LITERAL, LEX_INTEGER_LITERAL,
		LEX_DOUBLE, LEX_UINT, LEX_CONCAT,LEX_POW,LEX_RAND, LEX_SIN, LEX_MOD,LEX_BIGGEROREQUAL,LEX_SMALLEROREQUAL
		,LEX_NOTEQUAL,LEX_SMALLERTHEN,LEX_BIGGERTHEN,LEX_EQUAL_ID,LEX_DOUBLE_LITERAL,LEX_LENGTH,LEX_ID
		
	};
	
	for (int i = 0; i < size; ++i) {
		if (execute(string, nanomachinesSon[i])) {
			return tokens[i];
		}
	}

	return EOF;
}

short LA::Scan(LT::LexTable& lextable, IT::IdTable& idtable, In::IN& in, Parm::PARM& parm, Log::LOG& log) {
	short lexErrors = 0;
	std::ofstream outfile(parm.out);
	outfile << "01 ";

	std::string accumulator = "";
	std::string curScope = "";
	std::string prevScope = "";
	std::string curFunction = "";
	std::string prevId = "";
	std::string processingFunction = "";
	bool declaredNewFunction = false;

	for (int i = 0, line = 1; in.text[i]; ++i) {
		IT::IDDATATYPE iddatatype;
		auto fillTables = [&] {
			char token = Tokenize(accumulator.c_str());
			if (token == EOF) {
				lexErrors++;
				Log::Write(log, accumulator.c_str(), " - ", "");
				Error::ERROR temperr = Error::geterrorin(129, line, -1);
				*log.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line
					<< " в столбце " << temperr.inext.col << std::endl;
			}

			int ti_idx = TI_NULLIDX;

			iddatatype = (token == LEX_INTEGER || token == LEX_INTEGER_LITERAL) ?
				(IT::IDDATATYPE::INT) :
				(token == LEX_STRING || token == LEX_STRING_LITERAL) ? IT::IDDATATYPE::STR :
				(token == LEX_DOUBLE || token == LEX_DOUBLE_LITERAL) ? IT::IDDATATYPE::DOUBLE : iddatatype;

			if (token == LEX_LEFTHESIS) {
				declaredNewFunction = lextable.size >= 2 && lextable.table[lextable.size - 2].lexeme == LEX_FUNCTION;

				if (declaredNewFunction) {
					prevScope = curScope;
					curScope += curFunction;
				}
			}
			else if (token == LEX_RIGHTHESIS && declaredNewFunction) {
				curScope = prevScope;
				declaredNewFunction = false;
			}
			else if (token == LEX_LEFTBRACE) {
				prevScope = curScope;
				curScope += curFunction;
			}
			else if (token == LEX_BRACELET) {
				curScope.clear();
				curFunction.clear();
			}
			else if (token == LEX_INTEGER_LITERAL ) {
				ti_idx = IT::IsId(idtable, curScope.c_str(), accumulator.c_str(), true);
				if (ti_idx== LT_TI_NULLIDX) {
					IT::Add(idtable, { lextable.size,  curScope.c_str(), accumulator.c_str(), IT::IDDATATYPE::INT, IT::IDTYPE::L });
					if (atoll(accumulator.c_str()) >_UI32_MAX)
					{
						lexErrors++;
						Error::ERROR temperr = Error::geterrorin(132, line, -1);
						*log.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line
							<< " в столбце " << temperr.inext.col << std::endl;
						
					}
					idtable.table[idtable.size - 1].value.vint = atoi(accumulator.c_str());
				}
			
				token = LEX_LITERAL;
			}
			else if (token == LEX_LENGTH)
			{
				
				IT::Add(idtable, { lextable.size,  curScope.c_str(), "strLength", iddatatype, IT::IDTYPE::S ,{1, IT::LENGHT_PARAMS} });
				
				//исправить добавление повторного литерала
			}
			else if (token == LEX_POW)
			{
				
				IT::Add(idtable, { lextable.size,  curScope.c_str(), "toPow", iddatatype, IT::IDTYPE::S , {2, IT::LENGHT_POW} });
			}
			else if (token == LEX_RAND)
			{
			
				IT::Add(idtable, { lextable.size,  curScope.c_str(), "random", iddatatype, IT::IDTYPE::S, {1, IT::LENGHT_RAND} });
			}
			else if (token == LEX_PRINT)
			{
				IT::Add(idtable, { lextable.size,  curScope.c_str(), "print", iddatatype, IT::IDTYPE::S });
				idtable.table[idtable.size - 1].value.params.amount = 1;
			}
			else if (token == LEX_STRING_LITERAL) {
				std::string literal = accumulator.substr(1, accumulator.size() - 2);
				ti_idx = IT::IsId(idtable, curScope.c_str(), literal.c_str(), true);
				if (ti_idx == LT_TI_NULLIDX) {
					IT::Add(idtable, { lextable.size,  curScope.c_str(), literal.c_str(), IT::IDDATATYPE::STR, IT::IDTYPE::L });
					strcpy_s(idtable.table[idtable.size - 1].value.vstr.str, literal.c_str());
					idtable.table[idtable.size - 1].value.vstr.len = literal.length();
					strcpy_s(idtable.table[idtable.size - 1].id, literal.c_str());
				}
				token = LEX_LITERAL;
				
			}
			else if (token == LEX_ID || token == LEX_POW || token == LEX_LENGTH || token == LEX_RAND || token == LEX_SIN||token ==LEX_PRINT) {
				std::string id = accumulator.substr(0, ID_MAXSIZE);
				ti_idx = IT::IsId(idtable, curScope.c_str(), id.c_str());
				
				if (ti_idx == TI_NULLIDX) {
					if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexeme == LEX_DECLARE
						&& lextable.table[lextable.size - 1].lexeme == LEX_DATATYPE) {
						IT::Add(idtable, { lextable.size,  curScope.c_str(), id.c_str(), iddatatype, IT::IDTYPE::V });
					}
					else if (lextable.size >= 1 && (lextable.table[lextable.size - 1].lexeme == LEX_POW) ||
						(lextable.table[lextable.size - 1].lexeme == LEX_RAND )|| (lextable.table[lextable.size - 1].lexeme == LEX_LENGTH)||
						lextable.size >= 1 && (lextable.table[lextable.size - 1].lexeme == LEX_SIN) ||
						lextable.size >= 1 && (lextable.table[lextable.size - 1].lexeme == LEX_PRINT)) {
						IT::Add(idtable, { lextable.size,  curScope.c_str(), id.c_str(), iddatatype, IT::IDTYPE::V });
					}
					else if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexeme == LEX_FUNCTION) {
						curFunction = id;
						processingFunction = id;
						IT::Add(idtable, { lextable.size,  curScope.c_str(), id.c_str(), iddatatype, IT::IDTYPE::F });
					}
					else if (lextable.size >= 1 && lextable.table[lextable.size - 1].lexeme == LEX_DATATYPE
						&& declaredNewFunction) {
						IT::Add(idtable, { lextable.size,  curScope.c_str(), id.c_str(), iddatatype, IT::IDTYPE::P });
					}
					else {
						lexErrors++;
						Log::Write(log, accumulator.c_str(), " - ", "");
						Error::ERROR temperr = Error::geterrorin(124, line, -1);
						*log.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line
							<< " в столбце " << temperr.inext.col << std::endl;
						
					}
				}
				else if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexeme == LEX_DECLARE
					|| lextable.size >= 1 && lextable.table[lextable.size - 1].lexeme == LEX_FUNCTION) {
					lexErrors++;
					Log::Write(log, accumulator.c_str(), " - ", "");
					Error::ERROR temperr = Error::geterrorin(123, line, -1);
					*log.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line
						<< " в столбце " << temperr.inext.col << std::endl;
					
				}

			}
			else if (token == LEX_MAIN && curScope.empty()) {
				ti_idx = IT::IsId(idtable, "", accumulator.c_str());

				if (ti_idx != TI_NULLIDX) {
					lexErrors++;
					Log::Write(log, accumulator.c_str(), " - ", "");
					Error::ERROR temperr = Error::geterrorin(131, line, -1);
					*log.stream << '(' << temperr.id << ')' << temperr.message << " в строке " << temperr.inext.line
						<< " в столбце " << temperr.inext.col << std::endl;
					
				}
				curScope.clear();
				curFunction = accumulator;

				IT::Add(idtable, { lextable.size, "", accumulator.c_str(), IT::IDTYPE::F, 0 });
			}
			
			token = (token == LEX_INTEGER || token == LEX_STRING || token == LEX_DOUBLE) ? LEX_DATATYPE : token;

			
		
			if (token == LEX_ID  || token == LEX_MAIN) {
				if (ti_idx == TI_NULLIDX) {
					LT::Add(lextable, { token, line, idtable.size - 1 });
				}
				else {
					LT::Add(lextable, { token, line, ti_idx });
				}
			}
			else if (token == LEX_LITERAL)
			{
				if (ti_idx == LT_TI_NULLIDX) {
					LT::Add(lextable, { token, line, idtable.size - 1 });
				}
				else
				{
					LT::Add(lextable, { token, line, ti_idx});

				}
			}
			else if (SeAn::FindSTD(token))
			{
				LT::Add(lextable, { token, line, idtable.size - 1 });
			}
			else {
				LT::Add(lextable, { token, line, LT_TI_NULLIDX });
			}

			outfile << token;
			accumulator.clear();
		};

		unsigned char& ch = in.text[i];
		if (in.code[ch] == In::IN::T) {
			accumulator += ch;
		}
		else if (in.code[ch] == In::IN::Q) {
			do {
				accumulator += ch;
				i++;
				ch = in.text[i];
			} while (in.code[ch] != In::IN::Q);
			accumulator += ch;
			fillTables();
		}
		else if (in.code[ch] == In::IN::S || in.code[ch] == In::IN::O || ch == IN_CODE_DELIM) {
			if (!accumulator.empty()) {
				if (ch == '=')
				{
					prevId = accumulator;
				}
				fillTables();
			}
			if (in.code[ch] == In::IN::O) {

				accumulator = ch;
				fillTables();
			}
		}

		if (ch == IN_CODE_DELIM) {
			line++;
			if (in.text[i + 1]) {
				outfile << "\n" << std::setw(2) << std::setfill('0') << line << " ";
				i++;
			}
		}
	}

	if (IT::IsId(idtable, "", "main") == TI_NULLIDX) {//есть ли main
		lexErrors++;
		Error::ERROR temperr = Error::geterror(130);
		*log.stream << '(' << temperr.id << ')' << temperr.message << " в строке "<< std::endl;
		throw ERROR_THROW(130);
	}
	LA::WriteDataForFunctions(lextable, idtable);
	outfile.close();
	return lexErrors;
}
void LA::WriteDataForFunctions(LT::LexTable& lextable, IT::IdTable& idtable)
{
	for (int  i = 0; i < lextable.size; i++)
	{
		if (lextable.table[i].lexeme == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F &&
			lextable.table[i-1].lexeme==LEX_FUNCTION){ 
			int currentFuncIndex = i;
			short sizeParams = 0;
			idtable.table[lextable.table[i].idxTI].value.params.amount = 0;
			idtable.table[lextable.table[i].idxTI].value.params.types = new IT::IDDATATYPE;
			i += 2;
			while (lextable.table[i].lexeme!=LEX_RIGHTHESIS)
			{
				if (lextable.table[i].lexeme == LEX_ID)
				{
					idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.amount++;
					int j = 0;
					IT::IDDATATYPE* buffer = new IT::IDDATATYPE[idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.amount];
					for (; j < idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.amount-1; j++)
					{
						buffer[j] = idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.types[j];
					}
					idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.types[j] = idtable.table[lextable.table[i].idxTI].iddatatype;
				}
				i++;
			}

		}
	}
}

void LA::ShowIDtable(IT::IdTable& idtable, std::ofstream* outfile)
{
	*outfile << "----------------------------------------------------[ID таблица]--------------------------------------------" << std::endl;
	*outfile << "[ID]----------[SCOPE]---------[TYPE]-----------[IDTYPE]----------[value]--------------[LEidX]---------------" << std::endl;
	for (int i = 0; i < idtable.size; i++)
	{
		*outfile <<'['<< i<<"] "<<idtable.table[i].id << "\t\t" << idtable.table[i].scope << "\t\t\t\t";
		switch (idtable.table[i].iddatatype)
		{
		case IT::INT:
			*outfile << "INT\t\t\t\t";
			break;
		case IT::STR:
			*outfile << "STR\t\t\t\t";
			break;
		default:
			break;
		}

		switch (idtable.table[i].idtype)
		{
		case IT::IDTYPE::V:
			*outfile << "variable\t\t\t\t";
			break;
		case IT::IDTYPE::F:
			*outfile << "function\t\t\t\t";
			break;
		case IT::IDTYPE::S:
			*outfile << "std function\t\t\t\t";
			break;
		case IT::IDTYPE::L:
			*outfile << "literal\t\t\t\t";
			break;
		case IT::IDTYPE::P:
			*outfile << "params\t\t\t";
			break;
		default:
			break;
		}
		if (idtable.table[i].idtype == IT::IDTYPE::L)
		{
			if (idtable.table[i].iddatatype == IT::IDDATATYPE::INT)
			{
				*outfile << idtable.table[i].value.vint;
			}
			
			if (idtable.table[i].iddatatype == IT::IDDATATYPE::STR)
			{
				*outfile << idtable.table[i].value.vstr.str;
			}
		}
		else {
			*outfile << "NULL";
		}
		*outfile << '\t' << idtable.table[i].idxfirstLE << std::endl;
	}
	*outfile << "------------------------------------------------------------------------------------------------------------" << std::endl;
}
void LA::ShowLexTable(LT::LexTable& lextable, std::ofstream* outfile)
{
	*outfile << "---------------------------------------[LEXTABLE]----------------------------------------------------------" << std::endl;
	*outfile << "-[LEXEME]--------[строка]-----[idx]------------------------------------------------------------------------" << std::endl;
	for (int i = 0; i < lextable.size; i++)
	{
		*outfile <<'['<< i<<"] " << lextable.table[i].lexeme << '\t' << lextable.table[i].sn<<'\t';
		if (lextable.table[i].idxTI != TI_NULLIDX)
			*outfile << lextable.table[i].idxTI;
		else
			*outfile << "NULL";
		*outfile << std::endl;
	}
	*outfile << "-----------------------------------------------------------------------------------------------------------" << std::endl;
}