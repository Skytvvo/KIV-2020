#include "pch.h"
#include "Error.h"
#include "FST.h"
#include "Greibach.h"
#include "In.h"
#include "IT.h"
#include "Lexer.h"
#include "Log.h"
#include "LT.h"
#include "MFST.h"
#include "Parm.h"
#include "PolishNotation.h"



int _tmain(int argc, _TCHAR* argv[]) {
	setlocale(LC_ALL, "rus");
	
	Log::LOG log = Log::INITLOG;

	try {

		

		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		Log::Write(log, L"Тест:", L" без ошибок ", "\n", L"");
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getin(parm.in, parm.out);
		Log::WriteIn(log, in);
		LT::LexTable lextable = LT::Create(in.size);
		IT::IdTable idtable = IT::Create(in.size);

		LA::Scan(lextable, idtable, in, parm, log);
		for (int i = 0; i < idtable.size; i++)
		{
			std::cout <<"ID:"<< idtable.table[i].id << '\t' << "iddatatype:" << idtable.table[i].iddatatype << '\t' << "idtype:" << idtable.table[i].idtype << '\t' << "scope:" << idtable.table[i].scope << '\t' << "vint:" << ( idtable.table[i].value.vint  ) << "vstr:" << (idtable.table[i].value.vstr.str) << "vdouble:" << (idtable.table[i].value.vdouble) << std::endl;
		}
		MFST_TRACE_START
			MFST::Mfst mfst(lextable, GRB::getGreibach());
		mfst.start(*log.stream);
		
		

		SeAn::CheckingReturnInMain(lextable,idtable);
		SeAn::CheckReturnInUserFunc(lextable, idtable);
		SeAn::CheckParamsOfFunc(lextable, idtable);
		SeAn::CheckParamsStdFunc(lextable, idtable);
		SeAn::Types(lextable, idtable);
		for (int i = 0, k = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexeme == LEX_EQUALS) {
				PolishNotation(++i,lextable,idtable);
			}
		}
		for (int i = 0; i < lextable.size; i++)
		{
			std::cout << lextable.table[i].lexeme ;
		}
		LT::Delete(lextable);
		IT::Delete(idtable);
		Log::Close(log);
	}
	catch (Error::ERROR e) {

		Log::WriteError(log, e);
	}

#ifdef _DEBUG
	int hasMemoryLeaks = _CrtDumpMemoryLeaks();
#else
	system("pause");
#endif // _DEBUG
	system("pause");
	return 0;
}