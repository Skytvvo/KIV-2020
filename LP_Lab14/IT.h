#pragma once

#define ID_MAXSIZE		5
#define SCOPE_MAXSIZE	10
#define TI_MAXSIZE		4096
#define TI_INT_DEFAULT	0x00000000
#define TI_STR_DEFAULT	0x00
#define TI_NULLIDX		((int)0xffffffff)
#define TI_STR_MAXSIZE	255
#define RANDOM_TYPE IT::IDDATATYPE::INT
#define POW_TYPE IT::IDDATATYPE::INT
#define LENGHT_TYPE IT::IDDATATYPE::INT
#define CONCAT_TYPE IT::IDDATATYPE::STR
#define SIN_TYPE IT::IDDATATYPE::DOUBLE
#define MAX_PARAMS_COUNT 5			   	 //максимальное количество параметров у функции
#define POW_PARAMS_COUNT 2				 //кол-во параметров у функции pow
#define RANDOM_PARAMS_COUNT 1				 //кол-во параметров у функции rand
#define LENGHT_PARAMS_COUNT 1				//кол-во параметров у функции lenght
#define SIN_PARAMS_COUNT 1				//кол-во параметров у функции sin
#define CONCAT_PARAMS_COUNT 2				 //кол-во параметров у функции pow
#define POW_PARAMS_CNT 2				 //кол-во параметров у функции pow
#define RANDOM_PARAMS_CNT 1				 //кол-во параметров у функции rand
#define LENGHT_PARAMS_CNT 1				//кол-во параметров у функции lenght


//ILya2002 types
#define TI_UINT_MAXSIZE   UINT32_MAX
#define TI_UINT_MINSIZE	  UINT32_MIN

//Vitallik types
#define TI_INT_MAXSIZE   INT32_MAX		 
#define TI_INT_MINSIZE   INT32_MIN
#define TI_DOUBLE_MAXSIZE  DBL_MAX	
#define TI_DOUBLE_MINSIZE  -DBL_MAX	
#define TI_DOUBLE_DEFAULT	0x00000000

//Denis data types
#define TI_SHORT_MAXSIZE INT16_MAX
#define TI_SHORT_MINSIZE INT16_MIN
#define TI_SHORT_DEFAULT 0x0000	 

namespace IT {
	enum IDDATATYPE { INT = 1, STR = 2, SYM = 3,PROC = 4, UNDEF = 5,UINT = 6,SHORT = 7 , DOUBLE = 8};//типы данных идентификаторов: числовой, строковый,символьный неопределенный
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, S = 5, Z = 6 };//типы идентификаторов: V = переменная, F = функция, P =параметр, L =литерал, S = стандартная функция Z - знак
	enum STDFNC { F_POW, F_RANDOM, F_LENGTH, F_NOT_STD, F_CONCAT, F_SIN, };//стандартные функции
	static const IDDATATYPE POW_PARAMS[] = { IT::IDDATATYPE::INT, IT::IDDATATYPE::INT };
	static const IDDATATYPE RANDOM_PARAMS[] = { IT::IDDATATYPE::INT };
    static const IDDATATYPE SIN_PARAMS[] = { IT::IDDATATYPE::INT };
	static const IDDATATYPE LENGHT_PARAMS[] = { IT::IDDATATYPE::STR };
	static const IDDATATYPE CONCAT_PARAMS[] = { IT::IDDATATYPE::STR,IT::IDDATATYPE::STR };

	struct Entry {
		int idxfirstLE;
		char scope[SCOPE_MAXSIZE];
		char id[ID_MAXSIZE];
		IDDATATYPE iddatatype;
		IDTYPE idtype;
		union {
			int vint;
			struct {
				char len;
				char str[TI_STR_MAXSIZE - 1];
			} vstr;
			double vdouble;
			struct
			{
				IDDATATYPE* types;
				
				short amount;
			} params;
		} value;
		Entry() = default;
		Entry(int idxfirstLE, const char* scope, const char* id, IDTYPE idtype, int value);
		Entry(int idxfirstLE, const char* scope, const char* id, IDTYPE idtype, double value);
		Entry(int idxfirstLE, const char* scope, const char* id, IDTYPE idtype, const char* value);
		Entry(int idxfirstLE, const char* scope, const char* id, IDDATATYPE iddatatype, IDTYPE idtype);
	};

	struct IdTable {
		int maxsize;
		int size;
		Entry* table;
	};
	IdTable Create(int size);
	void Add(IdTable& idtable, const Entry& entry);
	Entry GetEntry(IdTable& idtable, int n);
	int IsId(IdTable& idtable, const char scope[SCOPE_MAXSIZE], const char id[ID_MAXSIZE]);
	int IsLiteral(IdTable& idtable, const char literal[TI_STR_MAXSIZE]);
	int IsLiteral(IdTable& idtable, int literal);
	void Delete(IdTable& idtable);
}