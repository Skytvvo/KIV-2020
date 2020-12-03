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
				while (lextable.table[i].lexeme != LEX_LEFTBRACE && i< lextable.size)
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
}