#ifndef COMPILER_SEMANTICANALYSER_H
#define COMPILER_SEMANTICANALYSER_H

#include <list>
#include <stack>
#include "linkedList.h"
#include "symbolTable.h"
#include "ui_mainwindow.h"

extern int lineNo;

extern SymbolTable symbolTable;

bool searchDuplicatedVariableTable(std::string);

bool searchDeclaratedVariableTable(std::string);

bool searchDeclaratedFunctionTable(std::string);

bool searchDeclaratedFunctionTable(std::string);

bool searchDeclaratedVariableOrFunctionTable(std::string);

bool searchDeclaratedProcedureTable(std::string);

bool searchDuplicatedProcedureTable(std::string);

bool searchVariableAndFunctionTable(std::string);

std::list<std::string> createInfixListFromExpression(std::string);

int precedence(const std::string &);

bool isIntOperation(const char *);

bool isBooleanOperation(const char *);

bool isNumber(const std::string *);

std::list<std::string> toPostfix(std::list<std::string> expressionList);

std::list<std::string> analysePostfix(std::list<std::string>, Ui::MainWindow *);

std::list<std::string> analysePostfix(std::list<std::string>, int, Ui::MainWindow *);

#endif //COMPILER_SEMANTICANALYSER_H
