#pragma once
#include "LT.h"
#include "IT.h"

#define POLISH_FUNCTION '@'
#define FORBIDDEN_SYMBOL '\0'

bool PolishNotation(int lextable_pos, LT::LexTable& lextable, IT::IdTable& idtable);
void ClearLextable(LT::LexTable& lextable , IT::IdTable& idtable);
std::vector<LT::Entry> convert(std::vector<LT::Entry> entries, IT::IdTable& idtable);