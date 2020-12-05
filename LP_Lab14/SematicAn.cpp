#include "pch.h"

namespace SeAn
{
	void SeAn::CheckingReturnInMain(LT::LexTable& lextable, IT::IdTable& idtable)
	{

		short nestingLevel = 0;
		short warmingID = 0;//1 - не весь код возвращает значение(например return только в if блоке)
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
						throw ERROR_THROW(121);//ДОБАВИТЬ ОШИБКУ НА ОБЪЯВЛЕНИЕ ФУНКЦИИ В МЕЙНЕ
					if (lextable.table[lexIndexInMain].lexeme == LEX_RETURN)
					{
						
						if (lextable.table[lexIndexInMain + 1].idxTI != LT_TI_NULLIDX && lextable.table[lexIndexInMain+1].lexeme==LEX_ID)
						{
							if (idtable.table[lextable.table[lexIndexInMain + 1].idxTI].iddatatype != IT::IDDATATYPE::INT)
							{
								throw ERROR_THROW(125);//возвращаемый тип не int
							}
						}
						else if (lextable.table[lexIndexInMain + 1].lexeme != LEX_INTEGER_LITERAL)
						{
							throw ERROR_THROW(126);//ВОЗВРАЩАЕМЫЙ ТИП НЕ INT
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
					throw ERROR_THROW(120);//warming - не весь код возвращает return
				if (warmingID == 0)
					throw ERROR_THROW(122);//warming - нет return

				break;
			}
			
		}
	}

	void SeAn::CheckReturnInUserFunc(LT::LexTable& lextable, IT::IdTable& idtable)
	{

		for (int i = 0; i < lextable.size; i++)
		{//ищем пользовательскую функцию
			if (lextable.table[i].lexeme == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F 
				&& lextable.table[i-1].lexeme == LEX_FUNCTION)
			{
				//сохраняем текущий тип функции
				IT::IDDATATYPE CurrentTypeOfFunction = idtable.table[lextable.table[i].idxTI].iddatatype;
				//также и тип литерала
				unsigned char literalType;
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
				}
				//когда нашли,скипаем параметры и переходим к телу функции
				while (lextable.table[i].lexeme != LEX_LEFTBRACE && i < lextable.size)
				{
					i++;
				}
				short nestingLevel = 0;//проверяем,чтобы функция при всех условиях возвращала значение (например в случаях с if)
				short warmingID = 0;
				//WARMING ID:
				//-1 - return всегда возвращает значение
				//0 - return отсутствует в функции
				//1 - return не всегда возвращает значение
				for (; i < lextable.size && lextable.table[i].lexeme!=LEX_BRACELET; i++)
				{
					//проверяем на вложенность 
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
						//проверяем тип возвращаемого значения
						if (lextable.table[i + 1].lexeme != literalType && idtable.table[lextable.table[i + 1].idxTI].iddatatype != CurrentTypeOfFunction)
						{
							throw ERROR_THROW(103);//возвращаемый тип не соответствует типу функцииж
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
					throw ERROR_THROW(100);//ВЫШЕ ОПИСАНО
					break;
				case 1:
					throw ERROR_THROW(101);//ВЫШЕ ОПИСАНО
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
			//ищем вызов пользовательской функции
			if (lextable.table[i].lexeme == LEX_ID && lextable.table[i - 1].lexeme != LEX_FUNCTION &&
				idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::F)
			{
				int currentFuncIndex = i;//запоминаем позицию проверяемой функции
				int currentIndexParam = 0;//номер аргумента
				i++;
				//проверяем до конца вызова функции
				while (lextable.table[i].lexeme!=LEX_RIGHTHESIS && i < lextable.size)
				{
					// если это идентификатор или литерал
					if (lextable.table[i].lexeme == LEX_ID || (lextable.table[i].idxTI!=TI_NULLIDX&&
						idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L))
					{
					
						
							if (idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.amount <= currentIndexParam)
								throw ERROR_THROW(190);//если аргументов больше чем параметров в функции
							//проверка на соответствие типов параметров и аргументоав
							if (idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.types[currentIndexParam] !=
								idtable.table[lextable.table[i].idxTI].iddatatype
								)
							{
								throw ERROR_THROW(191);//тип аргумента не соответствует типу параметра
							}
							currentIndexParam++;
						
					}
					i++;
				}
				if (idtable.table[lextable.table[currentFuncIndex].idxTI].value.params.amount != currentIndexParam)
					throw ERROR_THROW(193);//СЛИШКОМ МАЛО АРГУМЕНТОВ ПЕРЕДАННО
			}
		}
	}
	void SeAn::CheckParamsStdFunc(LT::LexTable& lextable, IT::IdTable& idtable)
	{
		for (int i = 0; i < lextable.size; i++)
		{
			if (SeAn::FindSTD(lextable.table[i].lexeme) && lextable.table[i - 1].lexeme == LEX_FUNCTION &&
				idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::S)
				throw ERROR_THROW(194);//попытка переопределить статическую функцию

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
							}//тип арнгумента не соответсвует типу параметра
							else
							{
								amountParams++;
							}
						if (lextable.table[i].idxTI != LT_TI_NULLIDX)
							if(idtable.table[lextable.table[i].idxTI].idtype == IT::IDTYPE::L)
								if (idtable.table[lextable.table[i].idxTI].iddatatype != IT::IDDATATYPE::STR)
								{
									throw ERROR_THROW(197);//тип литерала не соответствует типу параметра
								}
								else {
									amountParams++;
								}
					}
					if (amountParams < 1)
					{
						throw ERROR_THROW(198);
					}//СЛИШКОМ МАЛО АРГУМЕНТОВ
					else if(amountParams>1)
					{
						throw ERROR_THROW(199);//СЛИШКОМ МНОГО
					}
						
					break;
				case LEX_RAND:
					break;
				case LEX_POW:
					break;
				case LEX_SIN:
					break;
				default:
					throw ERROR_THROW(195);//Ошибка компилятора: неопознаная статическая функция
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
		default:
			return false;
			break;
		}
	}
}

