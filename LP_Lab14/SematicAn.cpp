#include "pch.h"
//
namespace SeAn
{
	void SeAn::CheckingReturnInMain(LT::LexTable& lextable, IT::IdTable& idtable)
	{

		short nestingLevel = 0;
		short warmingID = 0;//1 - �� ���� ��� ���������� ��������(�������� return ������ � if �����)
		for (int lexema_I = 0; lexema_I < lextable.size; lexema_I++)
		{
			if (lextable.table[lexema_I].lexeme == LEX_MAIN)
			{
				for (int lexIndexInMain = lexema_I; lexIndexInMain < lextable.size; lexIndexInMain++)
				{
					if (lextable.table[lexIndexInMain].lexeme == LEX_LEFT_SQUARE_BRACE)
					{
						nestingLevel++;
					}
					if (lextable.table[lexIndexInMain].lexeme == LEX_RIGHT_SQUARE_BRACE)
					{
						nestingLevel--;
					}
					if (lextable.table[lexIndexInMain].lexeme == LEX_FUNCTION)
						throw ERROR_THROW(121);//�������� ������ �� ���������� ������� � �����
					if (lextable.table[lexIndexInMain].lexeme == LEX_RETURN)
					{
						
						if (lextable.table[lexIndexInMain + 1].idxTI != LT_TI_NULLIDX && (lextable.table[lexIndexInMain+1].lexeme==LEX_ID)||
							lextable.table[lexIndexInMain + 1].lexeme == LEX_LITERAL)
						{
							if (idtable.table[lextable.table[lexIndexInMain + 1].idxTI].iddatatype != IT::IDDATATYPE::INT)
							{
								throw ERROR_THROW(125);//������������ ��� �� int
							}
						}
						

						if (nestingLevel == 0)
						{
							warmingID = -1;
							return;
						}
						else
							warmingID = 1;
					}
					
				}
				if (warmingID == 1)
					throw ERROR_THROW(120);//warming - �� ���� ��� ���������� return
				if (warmingID == 0)
					throw ERROR_THROW(122);//warming - ��� return

				break;
			}
			
		}
	}

	void SeAn::CheckReturnInUserFunc(LT::LexTable& lextable, IT::IdTable& idtable)
	{

		for (int i = 0; i < lextable.size; i++)
		{//���� ���������������� �������
			if (lextable.table[i].lexeme == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F 
				&& lextable.table[i-1].lexeme == LEX_FUNCTION)
			{
				//��������� ������� ��� �������
				IT::IDDATATYPE CurrentTypeOfFunction = idtable.table[lextable.table[i].idxTI].iddatatype;
				//����� � ��� ��������
				/*unsigned char literalType;
				switch (CurrentTypeOfFunction)
				{
				case IT::INT:
					literalType = LEX_INTEGER_LITERAL;
					break;
				case IT::STR:
					literalType = LEX_STRING_LITERAL;
					break;
				case IT::DOUBLE:
					literalType = LEX_DOUBLE_LITERAL;
					break;
				default:
					break;
				}*/
				//����� �����,������� ��������� � ��������� � ���� �������
				while (lextable.table[i].lexeme != LEX_LEFTBRACE && i < lextable.size)
				{
					i++;
				}
				short nestingLevel = 0;//���������,����� ������� ��� ���� �������� ���������� �������� (�������� � ������� � if)
				short warmingID = 0;
				//WARMING ID:
				//-1 - return ������ ���������� ��������
				//0 - return ����������� � �������
				//1 - return �� ������ ���������� ��������
				for (; i < lextable.size && lextable.table[i].lexeme!=LEX_BRACELET; i++)
				{
					//��������� �� ����������� 
					if (lextable.table[i].lexeme == LEX_LEFT_SQUARE_BRACE)
					{
						nestingLevel++;
					}
					if (lextable.table[i].lexeme == LEX_RIGHT_SQUARE_BRACE)
					{
						nestingLevel--;
					}
					if (lextable.table[i].lexeme == LEX_RETURN)
					{
						//��������� ��� ������������� ��������
						if (idtable.table[lextable.table[i + 1].idxTI].iddatatype != CurrentTypeOfFunction)
						{
							throw ERROR_THROW(103);//������������ ��� �� ������������� ���� ��������
						}
						if (nestingLevel != 0)
							warmingID = 1;
						if (nestingLevel == 0)
							warmingID = -1;
					}
				}
				switch (warmingID)
				{
				case -1:
					break;
				case 0:
					throw ERROR_THROW(100);//���� �������
					break;
				case 1:
					throw ERROR_THROW(101);//���� �������
					break;
				default:
					break;
				}
			}
		}
	}

	void SeAn::CheckParamsOfFunc(LT::LexTable& lextable, IT::IdTable& idtable)
	{
		
		for (int i = 0; i < lextable.size; i++)
		{
			//���� ����� ���������������� �������
			if (lextable.table[i].lexeme == LEX_ID && lextable.table[i - 1].lexeme != LEX_FUNCTION &&
				idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F)
			{
				int currentFuncIndex = i;//���������� ������� ����������� �������
				int currentIndexParam = 0;//����� ���������
				i++;
				//��������� �� ����� ������ �������
				while (lextable.table[i].lexeme!=LEX_RIGHTHESIS && i < lextable.size)
				{
					// ���� ��� ������������� ��� �������
					if (lextable.table[i].lexeme == LEX_ID || (lextable.table[i].idxTI!=TI_NULLIDX&&
						idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L))
					{
					
						
							if (idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.amount <= currentIndexParam)
								throw ERROR_THROW(190);//���� ���������� ������ ��� ���������� � �������
							//�������� �� ������������ ����� ���������� � �����������
							if (idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.types[currentIndexParam] !=
								idtable.table[lextable.table[i].idxTI].iddatatype
								)
							{
								throw ERROR_THROW(191);//��� ��������� �� ������������� ���� ���������
							}
							currentIndexParam++;
						
					}
					i++;
				}
				if (idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.amount != currentIndexParam)
					throw ERROR_THROW(193);//������� ���� ���������� ���������
			}
		}
	}
	
	void SeAn::CheckParamsStdFunc(LT::LexTable& lextable, IT::IdTable& idtable)
	{
		for (int i = 0; i < lextable.size; i++)
		{
			if (SeAn::FindSTD(lextable.table[i].lexeme) && lextable.table[i - 1].lexeme == LEX_FUNCTION)
					throw ERROR_THROW(194);//������� �������������� ����������� �������

			if (SeAn::FindSTD(lextable.table[i].lexeme))
			{
			
				short amountParams = 0;

				switch (lextable.table[i].lexeme)
				{
				case LEX_LENGTH:
					i++;
					while (lextable.table[i].lexeme!=LEX_RIGHTHESIS && i<lextable.size)
					{
						if (lextable.table[i].lexeme == LEX_ID)
							if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::STR)
							{
								throw ERROR_THROW(196);
							}//��� ���������� �� ������������ ���� ���������
							else
							{
								amountParams++;
							}
						if (lextable.table[i].idxTI != LT_TI_NULLIDX)
							if(idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L)
								if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::STR)
								{
									throw ERROR_THROW(197);//��� �������� �� ������������� ���� ���������
								}
								else {
									amountParams++;
								}
						i++;
					}
					if (amountParams < 1)
					{
						throw ERROR_THROW(198);
					}//������� ���� ����������
					else if(amountParams>1)
					{
						throw ERROR_THROW(199);//������� �����
					}
						
					break;
				case LEX_RAND:
					i++;
					while (lextable.table[i].lexeme != LEX_RIGHTHESIS && i < lextable.size)
					{
						if (lextable.table[i].lexeme == LEX_ID)
							if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::INT)
							{
								throw ERROR_THROW(196);
							}//��� ���������� �� ������������ ���� ���������
							else
							{
								amountParams++;
							}
						if (lextable.table[i].idxTI != LT_TI_NULLIDX)
							if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L)
								if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::INT)
								{
									throw ERROR_THROW(197);//��� �������� �� ������������� ���� ���������
								}
								else {
									amountParams++;
								}
						i++;
					}
					if (amountParams < 1)
					{
						throw ERROR_THROW(198);
					}//������� ���� ����������
					else if (amountParams > 1)
					{
						throw ERROR_THROW(199);//������� �����
					}

					break;
				case LEX_POW:
					i++;
					while (lextable.table[i].lexeme != LEX_RIGHTHESIS && i < lextable.size)
					{
						if (lextable.table[i].lexeme == LEX_ID)
							if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::INT)
							{
								throw ERROR_THROW(196);
							}//��� ���������� �� ������������ ���� ���������
							else
							{
								amountParams++;
							}
						if (lextable.table[i].idxTI != LT_TI_NULLIDX)
							if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L)
								if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::INT)
								{
									throw ERROR_THROW(197);//��� �������� �� ������������� ���� ���������
								}
								else {
									amountParams++;
								}
						i++;
					}
					if (amountParams < 2)
					{
						throw ERROR_THROW(198);
					}//������� ���� ����������
					else if (amountParams > 2)
					{
						throw ERROR_THROW(199);//������� �����
					}

					break;
				case LEX_SIN:
					i++;
					while (lextable.table[i].lexeme != LEX_RIGHTHESIS && i < lextable.size)
					{
						if (lextable.table[i].lexeme == LEX_ID)
							if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::DOUBLE)
							{
								throw ERROR_THROW(196);
							}//��� ���������� �� ������������ ���� ���������
							else
							{
								amountParams++;
							}
						if (lextable.table[i].idxTI != LT_TI_NULLIDX)
							if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L)
								if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::DOUBLE)
								{
									throw ERROR_THROW(197);//��� �������� �� ������������� ���� ���������
								}
								else {
									amountParams++;
								}
						i++;
					}
					if (amountParams < 1)
					{
						throw ERROR_THROW(198);
					}//������� ���� ����������
					else if (amountParams > 1)
					{
						throw ERROR_THROW(199);//������� �����
					}

					break;
				case LEX_PRINT:
				{
					i += 2;
					while (lextable.table[i].lexeme != LEX_RIGHTHESIS && i < lextable.size)
					{
						amountParams++;
						i++;
					}
					if (amountParams < 1)
					{
						throw ERROR_THROW(198);
					}//������� ���� ����������
					else if (amountParams > 1)
					{
						throw ERROR_THROW(199);//������� �����
					}
					break;
				}
				default:
					throw ERROR_THROW(195);//������ �����������: ����������� ����������� �������
					break;
				}
			}
		}
	}
	bool SeAn::FindSTD(unsigned char lex)
	{
		switch (lex)
		{
		case LEX_LENGTH:
			return true;
			break;
		case LEX_RAND:
			return true;
		case LEX_POW:
			return true;
		case LEX_SIN:
			return true;
		case LEX_PRINT:
			return true;
		default:
			return false;
			break;
		}
	}

	void SeAn::Types(LT::LexTable& lextable, IT::IdTable& idtable)
	{
		IT::IDDATATYPE currentType;
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexeme == LEX_EQUALS )
			{
				currentType = idtable.table[lextable.table[i - 1].idxTI].iddatatype;
				while (lextable.table[i].lexeme!=LEX_SEMICOLON && i<lextable.size)
				{
					i++;
					if ((lextable.table[i].lexeme == LEX_ID || lextable.table[i].lexeme == LEX_LITERAL)
						&& idtable.table[lextable.table[i].idxTI].iddatatype != currentType)
					{
						if (idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F ||
							idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::S)
						{
							throw ERROR_THROW(200);
						}//������� ���������� �� ��� ��� ������
						else if(idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::V ||
							idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L)
						{
							throw ERROR_THROW(201);
						}//�� ������ ��� ������ ������������
					}
					else if (lextable.table[i].lexeme == LEX_ID
						&& (idtable.table[lextable.table[i].idxTI].idtype != IT::IDTYPE::F&&
							idtable.table[lextable.table[i].idxTI].idtype != IT::IDTYPE::S)
						&& lextable.table[i + 1].lexeme == LEX_LEFTHESIS)
					{
						throw ERROR_THROW(709);//�� ������ ����� �������
					}
					else if (lextable.table[i].lexeme == LEX_LENGTH && currentType != IT::IDDATATYPE::INT)
					{
						throw ERROR_THROW(202);
					}//��� �������������� �� ��������� � ����� �������
					else if (lextable.table[i].lexeme == LEX_RAND && currentType != IT::IDDATATYPE::INT)
					{
						throw ERROR_THROW(202);
					}
					else if (lextable.table[i].lexeme == LEX_POW && currentType != IT::IDDATATYPE::INT)
					{
						throw ERROR_THROW(202);
					}
					else if(lextable.table[i].lexeme == LEX_SIN && currentType !=IT::IDDATATYPE::DOUBLE)
					{
						throw ERROR_THROW(202);
					}
					if (SeAn::FindSTD(lextable.table[i].lexeme) ||
						lextable.table[i].lexeme == LEX_FUNCTION)
					{
						while (lextable.table[i].lexeme != LEX_RIGHTHESIS && i < lextable.size)
							i++;
					}
				}
			}
		}
	}
}

