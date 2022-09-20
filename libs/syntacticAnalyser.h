#ifndef COMPILER_SYNTACTICANALYSER_H
#define COMPILER_SYNTACTICANALYSER_H

Node analyseType(FILE* file, Node token);

Node analyseVariables(FILE* file, Node token);

Node analyseEtVariables(FILE* file, Node token);

Node analyseProcedureDeclaration(FILE* file, Node token);

Node analyseFunctionDeclaration(FILE* file, Node token);

Node analyseSubroutine(FILE* file, Node token);

Node analyseCommands(FILE* file, Node token);

Node analyseFunctionCall(FILE* file, Node token);

Node analyseFactor(FILE* file, Node token);

Node analyseTerm(FILE* file, Node token);

Node analyseExpressions(FILE* file, Node token);

Node analyseSimpleExpressions(FILE* file, Node token);

Node analyseAttribution(FILE* file, Node token);

Node analyseProcedureCall(FILE* file, Node token);

Node analyseAttributionAndProcedureCall(FILE* file, Node token);

Node analyseRead(FILE* file, Node token);

Node analyseWrite(FILE* file, Node token);

Node analyseWhile(FILE* file, Node token);

Node analyseIf(FILE* file, Node token);
#endif //COMPILER_SYNTACTICANALYSER_H
