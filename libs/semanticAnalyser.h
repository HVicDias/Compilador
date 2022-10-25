#ifndef COMPILER_SEMANTICANALYSER_H
#define COMPILER_SEMANTICANALYSER_H

#include "linkedList.h"
#include "symbolTable.h"

extern int lineNo;

extern SymbolTable symbolTable;

bool searchDuplicatedVariableTable(Node token);

bool searchDeclaratedVariableTable(Node token);

bool searchDeclaratedFunctionTable(Node token);

bool searchDuplicatedProcedureTable(Node token);

void setType(Node token);

#endif //COMPILER_SEMANTICANALYSER_H
