#pragma once
#define LT_MAXSIZE				4096
#define LT_TI_NULLIDX			((int)0xffffffff)
#define LEX_INTEGER				'n'
#define LEX_STRING				's'
#define LEX_SHORT				'S'
#define LEX_UINT				'U'
#define LEX_DOUBLE				'D'
#define LEX_DATATYPE			't'
#define LEX_ID					'i'
#define LEX_INTEGER_LITERAL		'1'
#define LEX_STRING_LITERAL		'2'
#define LEX_DOUBLE_LITERAL		'3'


#define LEX_POW					'P' //������� ���������� � �������
#define LEX_RAND				'R' //������� ��������� ��������� �����
#define LEX_SIN					'N' //������� ������������ ������
#define LEX_MOD					'%' //��������� ������� �� �������
#define LEX_CONCAT				'C' //������� ���������� ����������� 2-� �����
#define LEX_LENGTH				'L' //������� ���������� ����� ������

#define LEX_LITERAL				'l'
#define LEX_FUNCTION			'f'
#define LEX_DECLARE				'd'
#define LEX_RETURN				'r'
#define LEX_PRINT				'p'
#define LEX_MAIN				'm'
#define LEX_SEMICOLON			';'
#define LEX_COMMA				','
#define LEX_LEFTBRACE			'{'
#define LEX_BRACELET			'}'
#define LEX_LEFTHESIS			'('
#define LEX_RIGHTHESIS			')'
#define LEX_PLUS				'+'
#define LEX_MINUS				'-'
#define LEX_STAR				'*'
#define LEX_DIRSLASH			'/'
#define LEX_EQUALS				'='

#define LEX_BIGGERTHEN          '>'
#define LEX_SMALLERTHEN         '<'
#define LEX_NOTEQUAL            '!'
#define LEX_SMALLEROREQUAL      '�'
#define LEX_BIGGEROREQUAL       '�'    
#define LEX_EQUAL_ID            '~'    

//
//
namespace LT {
	struct Entry {
		char lexeme;
		int sn;
		int idxTI;
	};

	struct LexTable {
		int maxsize;
		int size;
		Entry* table;
	};

	LexTable Create(int size);
	void Add(LexTable& lextable, Entry entry);
	Entry GetEntry(LexTable& lextable, int n);
	void Delete(LexTable& lextable);
}