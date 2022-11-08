#ifndef COMPILER_SYNTACTICANALYSER_H
#define COMPILER_SYNTACTICANALYSER_H

#include <queue>
#include "linkedList.h"

class TokenExpression {
public:
    Node token;
    std::string expression;
};

Node analyseType(FILE *file, Node token, std::queue<std::string>, std::queue<int>);

Node analyseVariables(FILE *file, Node token);

Node analyseEtVariables(FILE *file, Node token);

Node analyseProcedureDeclaration(FILE *file, Node token);

Node analyseFunctionDeclaration(FILE *file, Node token);

Node analyseSubroutine(FILE *file, Node token);

Node analyseCommands(FILE *file, Node token);

Node analyseBlock(FILE *file, Node token);

Node analyseFunctionCall(FILE *file, Node token);

TokenExpression analyseFactor(FILE *file, TokenExpression);

TokenExpression analyseTerm(FILE *file, TokenExpression);

TokenExpression analyseExpressions(FILE *file, TokenExpression);

TokenExpression analyseSimpleExpressions(FILE *file, TokenExpression);

Node analyseAttribution(FILE *file, Node token);

Node analyseProcedureCall(FILE *file, Node token);

Node analyseAttributionAndProcedureCall(FILE *file, Node token);

Node analyseRead(FILE *file, Node token);

Node analyseWrite(FILE *file, Node token);

Node analyseWhile(FILE *file, Node token);

Node analyseIf(FILE *file, Node token);

#endif //COMPILER_SYNTACTICANALYSER_H
