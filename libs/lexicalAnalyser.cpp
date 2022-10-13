#include <string>
#include <iostream>
#include "lexicalAnalyser.h"

using namespace std;

char character;

bool isSpace() {
    return (character == ' ' || character == '\n');
}

bool isCommentary() {
    return (character == '{');
}

bool isDigit() {
    return (character > 47 && character < 58);
}

bool isLetter() {
    return ((character > 64 && character < 91) || (character > 96 && character < 123));
}

bool isUnderscore() {
    return (character == 95);
}

bool isArithmeticOperator() {
    return (character == 42 || character == 43 || character == 45);
}

bool isRelationalOperator() {
    return (character == 33 || character == 60 || character == 61 || character == 62);
}

bool isPunctuation() {
    return (character == 59 || character == 44 || character == 40 || character == 41 || character == 46);
}

void jumpSpaces(FILE *file) {
    while (character == ' ' || character == '\n') {
        if (character == '\n') {
            cout << (int) character << "//" << lineNo << endl;
            lineNo++;
        }
        character = (char) fgetc(file);
    }
}

void jumpComentary(FILE *file) {
    do {
        character = (char) fgetc(file);

        if (character == EOF) {
            printf("erro falta de }");
            exit(1);
        }
    } while (character != '}');

    character = (char) fgetc(file);
}

Node handleDigit(FILE *file) {
    string lexema;

    while (isDigit()) {
        lexema += character;
        character = (char) fgetc(file);
    }

    return {lexema, "snumero"};
}

Node handleIdAndSpecialWord(FILE *file) {
    string lexema;
    string simbolo;

    while (isLetter() || isDigit() || isUnderscore()) {
        lexema += character;
        character = (char) fgetc(file);
    }

    if (lexema == "programa") {
        simbolo = "sprograma";
    } else if (lexema == "se") {
        simbolo = "sse";
    } else if (lexema == "entao") {
        simbolo = "sentao";
    } else if (lexema == "senao") {
        simbolo = "ssenao";
    } else if (lexema == "enquanto") {
        simbolo = "senquanto";
    } else if (lexema == "faca") {
        simbolo = "sfaca";
    } else if (lexema == "inicio") {
        simbolo = "sinicio";
    } else if (lexema == "fim") {
        simbolo = "sfim";
    } else if (lexema == "escreva") {
        simbolo = "sescreva";
    } else if (lexema == "leia") {
        simbolo = "sleia";
    } else if (lexema == "var") {
        simbolo = "svar";
    } else if (lexema == "inteiro") {
        simbolo = "sinteiro";
    } else if (lexema == "booleano") {
        simbolo = "sbooleano";
    } else if (lexema == "verdadeiro") {
        simbolo = "sverdadeiro";
    } else if (lexema == "falso") {
        simbolo = "sfalso";
    } else if (lexema == "procedimento") {
        simbolo = "sprocedimento";
    } else if (lexema == "funcao") {
        simbolo = "sfuncao";
    } else if (lexema == "div") {
        simbolo = "sdiv";
    } else if (lexema == "e") {
        simbolo = "se";
    } else if (lexema == "ou") {
        simbolo = "sou";
    } else if (lexema == "nao") {
        simbolo = "snao";
    } else {
        simbolo = "sidentificador";
    }

    return {lexema, simbolo};
}


Node handleAttribution(FILE *file) {
    string lexema;

    if (character == ':') {
        lexema += character;
        character = (char) fgetc(file);

        if (character == '=') {
            lexema += character;
            character = (char) fgetc(file);

            return {lexema, "satribuicao"};
        } else {

            return {lexema, "sdoispontos"};
        }
    } else {
        exit(1);
    }
}

Node handleArithmeticOperator(FILE *file) {
    string lexema;
    string simbolo;

    if (character == '+') {
        simbolo = "smais";
    } else if (character == '-') {
        simbolo = "smenos";
    } else if (character == '*') {
        simbolo = "smult";
    }
    lexema += character;
    character = (char) fgetc(file);
    return {lexema, simbolo};
}

Node handleRelationalOperator(FILE *file) {
    string lexema;
    string simbolo;
    simbolo = "";

    if (character == '<') {
        lexema += character;
        simbolo = "smenor";
        character = (char) fgetc(file);
    } else if (character == '>') {
        lexema += character;
        simbolo = "smaior";
        character = (char) fgetc(file);
    } else if (character == '!') {
        lexema += character;
        simbolo = "sdif";
        character = (char) fgetc(file);
    }

    if (character == '=') {
        lexema += character;
        if (simbolo.empty()) {
            simbolo = "sig";
            character = (char) fgetc(file);
        } else {
            if (simbolo == "smenor" || simbolo == "smaior") {
                simbolo += "ig";
            }
        }
        character = (char) fgetc(file);
    }

    if (lexema == "!") {
        simbolo = "caracter invalido";
    }

    return {lexema, simbolo};
}

Node handlePunctuation(FILE *file) {
    string lexema;
    string simbolo;

    if (character == ';') {
        simbolo = "sponto_virgula";
    } else if (character == ',') {
        simbolo = "svirgula";
    } else if (character == '(') {
        simbolo = "sabre_parenteses";
    } else if (character == ')') {
        simbolo = "sfecha_parenteses";
    } else if (character == '.') {
        simbolo = "sponto";
    }
    lexema += character;
    character = (char) fgetc(file);
    return {lexema, simbolo};
}

Node getToken(FILE *file) {
    while (isSpace() || isCommentary()) {
        if (isCommentary()) {
            jumpComentary(file);
        }

        if (isSpace()) {
            jumpSpaces(file);
        }
    }

    if (isDigit()) {
        return handleDigit(file);
    } else if (isLetter()) {
        return handleIdAndSpecialWord(file);
    } else if (character == ':') {
        return handleAttribution(file);
    } else if (isArithmeticOperator()) {
        return handleArithmeticOperator(file);
    } else if (isRelationalOperator()) {
        return handleRelationalOperator(file);
    } else if (isPunctuation()) {
        return handlePunctuation(file);
    } else {
        string s_character;
        s_character += character;
        character = (char) fgetc(file);

        return {s_character, "invalid symbol"};
    }
}