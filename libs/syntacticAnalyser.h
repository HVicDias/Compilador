#ifndef COMPILER_SYNTACTICANALYSER_H
#define COMPILER_SYNTACTICANALYSER_H

#include <queue>
#include "linkedList.h"
#include "ui_mainwindow.h"

extern int currentMemoryAllocation;
extern int currentLabel;
extern bool lastReturn;
extern bool hadPop;
extern std::stack<SymbolNode *> headerStack;

class TokenExpression {
public:
    Node token;
    std::string expression;
};

Node analyseType(FILE *, Node, std::queue<std::string>, std::queue<int>, Ui::MainWindow *);

Node analyseVariables(FILE *, Node, Ui::MainWindow *);

Node analyseEtVariables(FILE *, Node, Ui::MainWindow *);

Node analyseProcedureDeclaration(FILE *, Node, Ui::MainWindow *);

Node analyseFunctionDeclaration(FILE *, Node, Ui::MainWindow *);

Node analyseSubroutine(FILE *, Node, Ui::MainWindow *);

Node analyseCommands(FILE *, Node, Ui::MainWindow *);

Node analyseBlock(FILE *, Node, Ui::MainWindow *);

TokenExpression analyseFunctionCall(FILE *, TokenExpression, Ui::MainWindow *);

TokenExpression analyseFactor(FILE *, TokenExpression, Ui::MainWindow *);

TokenExpression analyseTerm(FILE *, TokenExpression, Ui::MainWindow *);

TokenExpression analyseExpressions(FILE *, TokenExpression, Ui::MainWindow *);

TokenExpression analyseSimpleExpressions(FILE *, TokenExpression, Ui::MainWindow *);

Node analyseAttribution(FILE *, Node, Node, Ui::MainWindow *);

Node analyseProcedureCall(FILE *, Node, Ui::MainWindow *);

Node analyseAttributionAndProcedureCall(FILE *, Node, Ui::MainWindow *);

Node analyseRead(FILE *, Node, Ui::MainWindow *);

Node analyseWrite(FILE *, Node, Ui::MainWindow *);

Node analyseWhile(FILE *, Node, Ui::MainWindow *);

Node analyseIf(FILE *, Node, Ui::MainWindow *);

#endif //COMPILER_SYNTACTICANALYSER_H
