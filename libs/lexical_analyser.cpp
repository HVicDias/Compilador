#include "lexical_analyser.h"
#include <string>

using namespace std;

char character;

typedef struct Token {
    string lexema;
    string simbolo;
    Token *next;
} Token;

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
    return (character == 21 || character == 60 || character == 61 || character == 62);
}

bool isPunctuation() {
    return (character == 59 || character == 44 || character == 40 || character == 41 || character == 46);
}


Token *handleDigit(FILE *file) {
    Token *token;
    string digito;

    while (!isDigit()) {
        digito += character;
        character = fgetc(file);
    }

    token->lexema = digito;
    token->simbolo = "snumero";
    token = token->next;
    return token;
}

Token *handleIdAndSpecialWord(FILE *file) {
    Token *token;
    string palavra;

    while (isLetter() || isUnderscore()) {
        palavra += character;
        character = fgetc(file);
    }

    token->lexema = palavra;
    if (palavra == "programa") {
        token->simbolo = "sprograma";
    } else if (palavra == "se") {
        token->simbolo = "sse";
    } else if (palavra == "entao") {
        token->simbolo = "sentao";
    } else if (palavra == "senao") {
        token->simbolo = "ssenao";
    } else if (palavra == "enquanto") {
        token->simbolo = "senquanto";
    } else if (palavra == "faca") {
        token->simbolo = "sfaca";
    } else if (palavra == "inicio") {
        token->simbolo = "sinicio";
    } else if (palavra == "fim") {
        token->simbolo = "sfim";
    } else if (palavra == "escreva") {
        token->simbolo = "sescreva";
    } else if (palavra == "leia") {
        token->simbolo = "sleia";
    } else if (palavra == "var") {
        token->simbolo = "svar";
    } else if (palavra == "inteiro") {
        token->simbolo = "sinteiro";
    } else if (palavra == "booleano") {
        token->simbolo = "sbooleano";
    } else if (palavra == "verdadeiro") {
        token->simbolo = "sverdadeiro";
    } else if (palavra == "falso") {
        token->simbolo = "sfalso";
    } else if (palavra == "procedimento") {
        token->simbolo = "sprocediento";
    } else if (palavra == "funcao") {
        token->simbolo = "sfuncao";
    } else if (palavra == "div") {
        token->simbolo = "sdiv";
    } else if (palavra == "e") {
        token->simbolo = "se";
    } else if (palavra == "ou") {
        token->simbolo = "sou";
    } else if (palavra == "nao") {
        token->simbolo = "snao";
    } else {
        token->simbolo = "sidentificador";
    }

    token = token->next;
    return token;
}


Token *handleAttribution(FILE *file) {
    Token *token;
    string palavra;

    if (character == ':') {
        palavra += character;
        character = fgetc(file);

        if (character == '=') {
            palavra += character;
            character = fgetc(file);
        } else {
            exit(1);
        }
    }

    token->lexema = palavra;
    token->simbolo = "satribuicao";
    token = token->next;

    return token;
}

Token *handleArithmeticOperator(FILE *file) {
    Token *token;
    string palavra;

    if (character == '+') {
        token->simbolo = "smais";
    } else if (character == '-') {
        token->simbolo = "smenos";
    } else if (character == '*') {
        token->simbolo = "smult";
    } else if (character == 'd') {
        palavra += character;
        character = fgetc(file);

        if (character == 'i') {
            palavra += character;
            character = fgetc(file);

            if (character != 'v') {
                printf("Error");
                exit(1);
            }
        } else {
            printf("Error");
            exit(1);
        }
    }
    palavra += character;
    character = fgetc(file);

    token->lexema = palavra;
    token = token->next;

    return token;
}

Token *handleRelationalOperator(FILE *file) {
    Token *token;
    string palavra;

    if (character == '!') {
        token->simbolo = 'sdif';
    } else if (character == '<') {
        token->simbolo = 'smenor';
    } else if (character == '>') {
        token->simbolo = 'smaior';
    } else if (character == '=') {
        palavra += character;
        character = fgetc(file);

        token->lexema = palavra;
        token->simbolo = 'sig';
        token = token->next;

        return token;
    }
    palavra += character;
    character = fgetc(file);

    if (character == '=') {
        palavra += character;

        if (token->simbolo != "sdif") {
            token->simbolo += 'ig';
        }

        character = fgetc(file);
    }

    token->lexema = palavra;
    token = token->next;

    return token;
}

Token *handlePunctuation(FILE *file) {
    Token *token;
    string palavra;

    if (character == ';') {
        token->simbolo = "sponto_virgula";
    } else if (character == ',') {
        token->simbolo = "svirgula";
    } else if (character == '(') {
        token->simbolo = "sabre_parenteses";
    } else if (character == ')') {
        token->simbolo = "sfecha_parenteses";
    } else if (character == '.') {
        token->simbolo = "sponto";
    }
    palavra += character;
    character = fgetc(file);

    token->lexema = palavra;
    token = token->next;

    return token;
}

Token *getToken(FILE *file) {
    if (isDigit()) {
        handleDigit(file);
    } else if (isLetter()) {
        handleIdAndSpecialWord(file);

        if (character == ':') {
            handleAttribution(file);
        } else if (isArithmeticOperator()) {
            handleArithmeticOperator(file);
        } else if (isRelationalOperator()) {
            handleRelationalOperator(file);
        } else if (isPunctuation()) {
            handlePunctuation(file);
        } else {
            printf("Error");
            exit(1);
        }
    }
}





//Algoritmo Pega Token
//Inicio
// Se caractere é digito
//   Então Trata Digito
//   Senão Se caractere é letra
//              Então Trata Identificador e Palavra Reservada
//                      Senão Se caractere = “:”
//                                    Então Trata Atribuição
//                                    Senão Se caractere E {+,-,*}
//                                                Então Trata Operador Aritmético
//                                                Senão Se caractere E {!,<,>,=}
//                                                             EntãoTrataOperadorRelacional
//                                                             Senão Se caractere E {;  ,  (  ) .}
//                                                                         Então Trata Pontuação
//                                                                         Senão ERRO
//Fim.