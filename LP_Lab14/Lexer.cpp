#include "pch.h"
#include "Lexer.h"
#include "FST.h"


char LA::Tokenize(const char* string) {
	FST::FST nanomachinesSon[] = {
		FST_INTEGER, FST_STRING, FST_FUNCTION, FST_DECLARE,
		FST_RETURN, FST_PRINT, FST_MAIN,
		FST_LEFTHESIS, FST_RIGHTHESIS, FST_SEMICOLON, FST_COMMA,
		FST_LEFTBRACE, FST_BRACELET,
		FST_PLUS, FST_MINUS, FST_STAR, FST_DIRSLASH, FST_EQUALS,
		 FST_STRING_LITERAL, FST_INTEGER_LITERAL,
		FST_SHORT, FST_DOUBLE, FST_UINT, GRAPH_CONCAT,
		GRAPH_POW,GRAPH_RANDOM,GRAPH_SIN, FST_MOD, FST_BIGGEROREQUAL,FST_SMALLEROREQUAL,FST_NOTEQUAL,FST_SMALLERTHAN,
		FST_BIGGERTHAN,FST_EQUALS_TWO,FST_DOUBLE_LITERAL,GRAPH_LENGTH ,FST_ID
	};
	const int size = sizeof(nanomachinesSon) / sizeof(nanomachinesSon[0]); 
	const char tokens[] = { 

		LEX_INTEGER, LEX_STRING, LEX_FUNCTION, LEX_DECLARE,
		LEX_RETURN, LEX_PRINT, LEX_MAIN,
		LEX_LEFTHESIS, LEX_RIGHTHESIS, LEX_SEMICOLON, LEX_COMMA,
		LEX_LEFTBRACE, LEX_BRACELET,
		LEX_PLUS, LEX_MINUS, LEX_STAR, LEX_DIRSLASH, LEX_EQUALS,
		 LEX_STRING_LITERAL, LEX_INTEGER_LITERAL,LEX_SHORT,
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

void LA::Scan(LT::LexTable& lextable, IT::IdTable& idtable, In::IN& in, Parm::PARM& parm, Log::LOG& log) {
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
				Log::Write(log, accumulator.c_str(), " - ", "");
				throw ERROR_THROW(129);
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
				int literal = std::stoi(accumulator);
				std::string id = prevId.substr(0, ID_MAXSIZE);
				ti_idx = IT::IsId(idtable, curScope.c_str(), id.c_str());
				if (ti_idx != TI_NULLIDX && prevId != "")
				{
					idtable.table[ti_idx].value.vint = literal;
					prevId.clear();
				}
			
		
			}
			else if (token == LEX_DOUBLE_LITERAL)
			{				
				//Добавить ошибку на неправильный литерл дабла
				//пример: double x = 1g.02
				double literalValue = std::stod(accumulator);
				std::string id = prevId.substr(0, ID_MAXSIZE);
				ti_idx = IT::IsId(idtable, curScope.c_str(), id.c_str());
				if (ti_idx != TI_NULLIDX && prevId != "")
				{
					idtable.table[ti_idx].value.vdouble = literalValue;
					prevId.clear();
				}
			
				
			}
			else if (token == LEX_LENGTH)
			{
				IT::Add(idtable, { lextable.size,  curScope.c_str(), "lengt", iddatatype, IT::IDTYPE::F });
			}
			else if (token == LEX_POW)
			{
				IT::Add(idtable, { lextable.size,  curScope.c_str(), "pow", iddatatype, IT::IDTYPE::F });
			}
			else if (token == LEX_RAND)
			{
				IT::Add(idtable, { lextable.size,  curScope.c_str(), "rand", iddatatype, IT::IDTYPE::F });
			}
			else if (token == LEX_CONCAT)
			{
				IT::Add(idtable, { lextable.size,  curScope.c_str(), "conca", iddatatype, IT::IDTYPE::F });
			}
			else if (token == LEX_SIN)
			{
				IT::Add(idtable, { lextable.size,  curScope.c_str(), "sin", iddatatype, IT::IDTYPE::F });
			}
			else if (token == LEX_STRING_LITERAL) {
				std::string literal = accumulator.substr(1, accumulator.size() - 2);
				std::string id = prevId.substr(0, ID_MAXSIZE);
				ti_idx = IT::IsId(idtable, curScope.c_str(), id.c_str());
				if (ti_idx != TI_NULLIDX && prevId!="")
				{
					strcpy_s(idtable.table[ti_idx].value.vstr.str, literal.c_str());
					idtable.table[ti_idx].value.vstr.len = literal.length();
					prevId.clear();
				}
			
				
			}
			else if (token == LEX_ID || token == LEX_POW || token == LEX_LENGTH || token == LEX_RAND || token == LEX_SIN) {
				std::string id = accumulator.substr(0, ID_MAXSIZE);
				ti_idx = IT::IsId(idtable, curScope.c_str(), id.c_str());
				
				if (ti_idx == TI_NULLIDX) {
					if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexeme == LEX_DECLARE
						&& lextable.table[lextable.size - 1].lexeme == LEX_DATATYPE) {
						IT::Add(idtable, { lextable.size,  curScope.c_str(), id.c_str(), iddatatype, IT::IDTYPE::V });
					}
					else if (lextable.size >= 1 && (lextable.table[lextable.size - 1].lexeme == LEX_POW) ||
						(lextable.table[lextable.size - 1].lexeme == LEX_RAND )|| (lextable.table[lextable.size - 1].lexeme == LEX_LENGTH)||
						lextable.size >= 1 && (lextable.table[lextable.size - 1].lexeme == LEX_CONCAT)) {
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
						Log::Write(log, accumulator.c_str(), " - ", "");
						throw ERROR_THROW(124);
					}
				}
				else if (lextable.size >= 2 && lextable.table[lextable.size - 2].lexeme == LEX_DECLARE
					|| lextable.size >= 1 && lextable.table[lextable.size - 1].lexeme == LEX_FUNCTION) {
					Log::Write(log, accumulator.c_str(), " - ", "");
					throw ERROR_THROW(123);
				}

			}
			else if (token == LEX_MAIN && curScope.empty()) {
				ti_idx = IT::IsId(idtable, "", accumulator.c_str());

				if (ti_idx != TI_NULLIDX) {
					Log::Write(log, accumulator.c_str(), " - ", "");
					throw ERROR_THROW(131);
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
		throw ERROR_THROW(130);
	}

	outfile.close();
}