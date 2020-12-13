#include "pch.h"
#include "IT.h"
#include "Error.h"

IT::IdTable IT::Create(int size) {
	if (size > TI_MAXSIZE) {
		throw ERROR_THROW(126);
	}

	IdTable idtable;
	idtable.maxsize = size;
	idtable.size = 0;
	idtable.table = DBG_NEW Entry[size];
	return idtable;
}

void IT::Add(IdTable& idtable, const Entry& entry) {
	if (idtable.size + 1 > idtable.maxsize) {
		throw ERROR_THROW(127);
	}

	idtable.table[idtable.size] = entry;
	idtable.size++;
}

IT::Entry IT::GetEntry(IdTable& idtable, int n) {
	if (n < 0 || n > idtable.size - 1) {
		throw ERROR_THROW(128);
	}

	return idtable.table[n];
}

int IT::IsId(IdTable& idtable, const char scope[SCOPE_MAXSIZE], const char id[ID_MAXSIZE]) {
	for (int i = 0; i < idtable.size; ++i) {
		if (idtable.table[i].idtype != IT::IDTYPE::L
			&& !memcmp(scope, idtable.table[i].scope, (strlen(idtable.table[i].scope) < SCOPE_MAXSIZE ?
				strlen(idtable.table[i].scope) : SCOPE_MAXSIZE))
			&& !memcmp(id, idtable.table[i].id, (strlen(idtable.table[i].id) < ID_MAXSIZE ?
				strlen(idtable.table[i].id) : ID_MAXSIZE))) {
			return i;
		}
	}

	return TI_NULLIDX;
}

int IT::IsId(IdTable& idtable, const char scope[SCOPE_MAXSIZE], const char id[ID_MAXSIZE], bool lexFlag) {
	for (int i = 0; i < idtable.size; ++i) {
		if (!memcmp(scope, idtable.table[i].scope, (strlen(idtable.table[i].scope) < SCOPE_MAXSIZE ?
				strlen(idtable.table[i].scope) : SCOPE_MAXSIZE))
			&& !memcmp(id, idtable.table[i].id, (strlen(idtable.table[i].id) < ID_MAXSIZE ?
				strlen(idtable.table[i].id) : ID_MAXSIZE))&& (idtable.table[i].idtype == IT::L)) {
			return i;
		}
	}

	return TI_NULLIDX;
}

int IT::IsLiteral(IdTable& idtable, const char literal[TI_STR_MAXSIZE]) {
	for (int i = 0; i < idtable.size; ++i) {
		if (!strcmp(idtable.table[i].value.vstr.str, literal) &&
			idtable.table[i].idtype == IT::IDTYPE::L) {
			return i;
		}
	}
	return TI_NULLIDX;
}

int IT::IsLiteral(IdTable& idtable, int literal) {
	for (int i = 0; i < idtable.size; ++i) {
		if (idtable.table[i].value.vint == literal &&
			idtable.table[i].idtype == IT::IDTYPE::L) {
			return i;
		}
	}
	return TI_NULLIDX;
}

void IT::Delete(IdTable& idtable) {
	if (!idtable.table) {
		return;
	}

	delete[] idtable.table;
	idtable.table = nullptr;
}

IT::Entry::Entry(int idxfirstLE, const char* scope, const char* id, IDTYPE idtype, int value)
	: idxfirstLE(idxfirstLE), iddatatype(IDDATATYPE::UINT), idtype(idtype)
{
	memcpy_s(this->id, sizeof(this->id), id, sizeof(this->id));
	memcpy_s(this->scope, sizeof(this->scope), scope, sizeof(this->scope));

	this->value.vint = value;
}


IT::Entry::Entry(int idxfirstLE, const char* scope, const char* id, IDTYPE idtype, const char* value)
	: idxfirstLE(idxfirstLE), iddatatype(IDDATATYPE::STR), idtype(idtype)
{
	memcpy_s(this->id, sizeof(this->id), id, sizeof(this->id));
	memcpy_s(this->scope, sizeof(this->scope), scope, sizeof(this->scope));

	this->value.vstr.len = (char)strlen(value);
	strcpy_s(this->value.vstr.str, value);
}

IT::Entry::Entry(int idxfirstLE, const char* scope, const char* id, IDDATATYPE iddatatype, IDTYPE idtype)
	: idxfirstLE(idxfirstLE), iddatatype(iddatatype), idtype(idtype)
{
	memcpy_s(this->id, sizeof(this->id), id, sizeof(this->id));
	memcpy_s(this->scope, sizeof(this->scope), scope, sizeof(this->scope));

	if (this->iddatatype == IDDATATYPE::UINT) {
		value.vint = TI_INT_DEFAULT;
	}
	else {
		value.vstr.len = TI_STR_DEFAULT;
	}
}IT::Entry::Entry(int idxfirstLE, const char* scope, const char* id, IDDATATYPE iddatatype, IDTYPE idtype, Entry::parm value)
	: idxfirstLE(idxfirstLE), iddatatype(iddatatype), idtype(idtype)
{
	memcpy_s(this->id, sizeof(this->id), id, sizeof(this->id));
	memcpy_s(this->scope, sizeof(this->scope), scope, sizeof(this->scope));

	this->value.params.amount = value.amount;
	this->value.params.types = value.types;
}