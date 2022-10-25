#ifndef COMPILER_SEMANTICANALYSER_H
#define COMPILER_SEMANTICANALYSER_H

#include "linkedList.h"
#include "symbolTable.h"

extern int lineNo;

extern SymbolTable symbolTable;

bool searchDuplicatedVariableTable(Node token);

bool searchDeclaratedVariableTable(std::string);

bool searchDeclaratedFunctionTable(std::string);

bool searchDeclaratedVariableOrFunctionTable(std::string);

bool searchDuplicatedProcedureTable(Node token);

#endif //COMPILER_SEMANTICANALYSER_H
