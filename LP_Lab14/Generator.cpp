#include "pch.h"



bool Gener::CodeGeneration(LT::LexTable& lextable, IT::IdTable& idtable)
{
	std::ofstream AsmFile(ASM_PATH);
	if (!AsmFile.is_open())
	{
		throw ERROR_THROW(100);//
	}
	bool gen_ok;
	AsmFile << BEGIN;
	AsmFile << ".const\n";

	for (int i = 0; i < idtable.size; i++)
	{
		if (idtable.table[i].idtype == IT::L)
		{
			AsmFile << "\t" << idtable.table[i].scope<<idtable.table[i].id;
			if (idtable.table[i].iddatatype == IT::INT)
			{
				AsmFile << " DWORD " << idtable.table[i].value.vint << std::endl;
			}
			else if (idtable.table[i].iddatatype == IT::STR)
			{
				AsmFile << " BYTE " <<'\''<< idtable.table[i].value.vstr.str<<'\'' << ", 0\n";
			}
			else if(idtable.table[i].iddatatype == IT::DOUBLE)
			{
				AsmFile << " QWORD " << idtable.table[i].value.vdouble << std::endl;
			}
		}
	}
	AsmFile << ".data\n";
	
	for (int i = 0; i < idtable.size; i++)
	{
		if (idtable.table[i].idtype == IT::IDTYPE::V)
		{
			AsmFile << "\t" << idtable.table[i].scope<< idtable.table[i].id;
			if (idtable.table[i].iddatatype == IT::DOUBLE)
			{
				AsmFile << " QWORD ?\n";
			}
			if (idtable.table[i].iddatatype == IT::STR)
			{
				AsmFile << " DWORD ?\n";
			}
			if (idtable.table[i].iddatatype == IT::INT)
			{
				AsmFile << " DWORD ?\n";
			}
		}
	}
	AsmFile << "\n.code\n";
	bool declaredMain = false;
	bool declaredFunc = false;
	int declaredFuncIndex;
	for (int i = 0; i < lextable.size; i++)
	{
		switch (lextable.table[i].lexeme)
		{
			case LEX_FUNCTION:
			{
					declaredFunc = true;
					i++;
					declaredFuncIndex = i;
					AsmFile << (idtable.table[lextable.table[i].idxTI]).id << " PROC ";
					i += 2;
					while (lextable.table[i].lexeme != LEX_RIGHTHESIS)
					{
						if (lextable.table[i].lexeme == LEX_ID)
						{
							
							AsmFile << idtable.table[lextable.table[i].idxTI].scope << idtable.table[lextable.table[i].idxTI].id << " : ";
							if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::INT)
							{
								AsmFile << " DWORD ";
							}
							else if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::STR)
							{
								AsmFile << " DWORD ";
							}
							else
							{
								AsmFile << " QWORD ";
							}
						}
						if(lextable.table[i+1].lexeme!=LEX_RIGHTHESIS&& lextable.table[i].lexeme == LEX_ID)
						AsmFile << ", ";
						
						i++;


					}
					AsmFile << std::endl;
				break;
			}
			case LEX_MAIN:
			{
				AsmFile << "main PROC\n";
				declaredMain = true;
				break;
			}
		/*	case LEX_LEFTBRACE:
			{

				break;
			}*/
			case LEX_BRACELET:
			{
				if (declaredFunc)
				{
					AsmFile << idtable.table[lextable.table[declaredFuncIndex].idxTI].id << " ENDP\n\n";
					declaredFunc = false;
				}
				else
				{
					AsmFile << "\tcall\t\tExitProcess\nmain ENDP\nEND main";
				}
				declaredFuncIndex = 0;
				break;
			}
			case LEX_RETURN:
			{
				i = Gener::GenExpHandler(AsmFile, lextable, idtable, ++i);
				if (!declaredMain)
				{
					
					AsmFile << "pop eax\n" << "ret\n";
				}
				break;
			}
			case LEX_PRINT:
			{
				IT::IDDATATYPE type = Gener::DetectType(lextable, idtable, i + 1);
				i = Gener::GenExpHandler(AsmFile, lextable, idtable, ++i);
				if (type == IT::INT)
				{
					AsmFile << "push eax\ncall outnum\n";
				}
				else if(type == IT::STR)
				{
					AsmFile << "push eax\ncall outstr\n";
				}
				/*else
				{

				}*/
				break;
			}
			case LEX_EQUALS:
			{
				int idx = lextable.table[i - 1].idxTI;
				i = Gener::GenExpHandler(AsmFile, lextable, idtable, ++i);
				AsmFile << "push eax\npop " << idtable.table[idx].scope << idtable.table[idx].id << std::endl;
				break;
			}
		default:
			break;
		}
	}
	
	std::stack<std::string> stk;
	AsmFile.close();
	return true;

}

int Gener::GenExpHandler(std::ofstream& AsmFile, LT::LexTable& LEXTABLE, IT::IdTable& idtable, int i) {
	for (; LEXTABLE.table[i].lexeme!=LEX_SEMICOLON; i++)
	{
		switch (LEXTABLE.table[i].lexeme)
		{
		case LEX_PLUS:
		{
			AsmFile << "pop eax" << std::endl <<
				"pop ebx" << std::endl << "add eax,ebx" << std::endl << "push eax" << std::endl;
			break;
		}
		case LEX_MINUS:
		{
			AsmFile << "pop eax" << std::endl <<
				"pop ebx" << std::endl << "sub eax,ebx" << std::endl << "push eax" << std::endl;
			break;
		}
		case LEX_STAR:
		{
			AsmFile << "pop eax" << std::endl <<
				"pop ebx" << std::endl << "mul eax,ebx" << std::endl << "push eax" << std::endl;
			break;
		}
		case LEX_DIRSLASH:
		{
			AsmFile << "pop eax" << std::endl <<
				"pop ebx" << std::endl << "div eax,ebx" << std::endl << "push eax" << std::endl;
			break;
		}
		case LEX_LITERAL:
		case LEX_ID:
		{
			if (idtable.table[LEXTABLE.table[i].idxTI].iddatatype == IT::INT)
			{
				AsmFile << "push "  <<idtable.table[LEXTABLE.table[i].idxTI].scope << idtable.table[LEXTABLE.table[i].idxTI].id <<std::endl;
			}
			/*else if(idtable.table[LEXTABLE.table[i].idxTI].iddatatype == IT::DOUBLE)
			{
				AsmFile << "push " << idtable.table[LEXTABLE.table[i].idxTI].id <<
					idtable.table[LEXTABLE.table[i].idxTI].scope << std::endl;
			}*/
			//ÞÇÀÒÜ RAX ÐÅÃÈÑÒÐÛ
			else if(idtable.table[LEXTABLE.table[i].idxTI].idtype == IT::L &&
				idtable.table[LEXTABLE.table[i].idxTI].iddatatype == IT::STR)
			{
				AsmFile<<"push OFFSET " <<
					idtable.table[LEXTABLE.table[i].idxTI].scope << 
					idtable.table[LEXTABLE.table[i].idxTI].id << std::endl;
			}
			else
			{
				AsmFile<<"push  " <<
					idtable.table[LEXTABLE.table[i].idxTI].scope << 
					idtable.table[LEXTABLE.table[i].idxTI].id << std::endl;
			}
			break;
		}
		case POLISH_FUNCTION:
		{
			AsmFile << "call "<< idtable.table[LEXTABLE.table[i].idxTI].id <<std::endl<<
				" push eax\n";

			break;
		}
		default:
			break;
		}
	}
	AsmFile << "pop eax\n";
	return i;
}

IT::IDDATATYPE Gener::DetectType(LT::LexTable lextable, IT::IdTable idtable, int i)
{
	for (;lextable.table[i].lexeme!=LEX_SEMICOLON ; i++)
	{
		if (lextable.table[i].idxTI != LT_TI_NULLIDX)
		{
			return idtable.table[lextable.table[i].idxTI].iddatatype;
		}
	}
}
