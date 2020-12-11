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

		LA::ShowIDtable(idtable);
		LA::ShowLexTable(lextable);
		/*MFST_TRACE_START
			MFST::Mfst mfst(lextable, GRB::getGreibach());
		mfst.start(*log.stream);
		
		*/

		SeAn::CheckingReturnInMain(lextable,idtable);
		SeAn::CheckReturnInUserFunc(lextable, idtable);
		SeAn::CheckParamsOfFunc(lextable, idtable);
		SeAn::CheckParamsStdFunc(lextable, idtable);
		SeAn::Types(lextable, idtable);

	

		PN::PolishNotation(lextable, idtable);
		LA::ShowLexTable(lextable);
		Gener::CodeGeneration(lextable, idtable);
		LT::Delete(lextable);
		IT::Delete(idtable);
		Log::Close(log);
	}
	catch (Error::ERROR e) {

		Log::WriteError(log, e);
	}



	system("pause");
	return 0;
}