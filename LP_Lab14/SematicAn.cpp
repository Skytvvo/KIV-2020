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
					if (lextable.table[lexIndexInMain].lexeme == LEX_LEFTBRACE)
					{
						nestingLevel++;
					}
					if (lextable.table[lexIndexInMain].lexeme == LEX_BRACELET)
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

						if (nestingLevel == 1)
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
}