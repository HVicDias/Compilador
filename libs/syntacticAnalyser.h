#ifndef COMPILER_SYNTACTICANALYSER_H
#define COMPILER_SYNTACTICANALYSER_H

#include <queue>
#include "linkedList.h"

class TokenExpression {
public:
    Node token;
    std::string expression;
};

Node analyseType(FILE *, Node, std::queue<std::string>, std::queue<int>);

Node analyseVariables(FILE *, Node);

Node analyseEtVariables(FILE *, Node);

Node analyseProcedureDeclaration(FILE *, Node);

Node analyseFunctionDeclaration(FILE *, Node);

Node analyseSubroutine(FILE *, Node);

Node analyseCommands(FILE *, Node);

Node analyseBlock(FILE *, Node);

TokenExpression analyseFunctionCall(FILE *, TokenExpression);

TokenExpression analyseFactor(FILE *, TokenExpression);

TokenExpression analyseTerm(FILE *, TokenExpression);

TokenExpression analyseExpressions(FILE *, TokenExpression);

TokenExpression analyseSimpleExpressions(FILE *, TokenExpression);

Node analyseAttribution(FILE *, Node);

Node analyseProcedureCall(FILE *, Node);

Node analyseAttributionAndProcedureCall(FILE *, Node);

Node analyseRead(FILE *, Node);

Node analyseWrite(FILE *, Node);

Node analyseWhile(FILE *, Node);

Node analyseIf(FILE *, Node);

#endif //COMPILER_SYNTACTICANALYSER_H
