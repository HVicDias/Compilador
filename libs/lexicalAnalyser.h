#ifndef COMPILER_LEXICALANALYSER_H
#define COMPILER_LEXICALANALYSER_H

#include "linkedList.h"

extern int lineNo;

extern char character;

bool isSpace();

bool isCommentary();

bool isDigit();

bool isLetter();

bool isUnderscore();

bool isArithmeticOperator();

bool isRelationalOperator();

bool isPunctuation();

void jumpSpaces(FILE *file);

void jumpComentary(FILE *file);

Node handleDigit(FILE *file);

Node handleIdAndSpecialWord(FILE *file);

Node handleAttribution(FILE *file);

Node handleArithmeticOperator(FILE *file);

Node handleRelationalOperator(FILE *file);

Node handlePunctuation(FILE *file);

Node getToken(FILE *file);

#endif //COMPILER_LEXICALANALYSER_H
