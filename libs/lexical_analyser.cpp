#include "lexical_analyser.h"
#include <string>

using namespace std;

char character;

//class Token {
//    public:
//        string lexema;
//        string simbolo;
//        Token next;
//};

//Algoritmo Trata Dígito
//        Def num : Palavra
//        Inicio
//num  caractere
//Ler(caracter)
//Enquanto caractere é dígito
//Faça {
//num  num + caractere
//Ler(caracter)
//}
//token.símbolo  snúmero
//        token.lexema  num
//        Fim.

bool isDigit() {
    return (character > 47 && character < 58);
}

bool isLetter() {
    return ((character > 64 && character < 91) || (character > 96 && character < 123));
}

bool isUnderscore(){
    return (character == 95);
}
//
//Token *handleDigit(FILE *file) {
//    string digito;
//    Token *token;
//    while (!isDigit())
//    {
//        digito += character;
//        character = fgetc(file);
//    }
//    token->lexema = digito;
//    token->simbolo = "snumero";
//    token = token->next;
//    return token;
//}

//Token *handleIdSpecialWord(FILE *file) {
//    string palavra;
//    Token *token;
//    while ((character > 64 && character < 91) || (character > 96 && character < 123))
//    {
//        palavra += character;
//        character = fgetc(file);
//    }
//    token->lexema = palavra;
//      if(palavra == "programa"){
//           token->simbolo = "sprograma";
//      }else if(palavra == "se"){
//           token->simbolo = "sse";
//      }else if(palavra == "entao"){
//          token->simbolo = "sentao";
//    token->simbolo = "snumero";
//    token = token->next;
//    return token;
//}



//Token *getToken(FILE *file) {
//    if(isDigit()) {
//        handleDigit(file);
//    } else if(isLetter()) {
//        //handleIdSpecialWord(file);
//        printf("da");
//    }
//}
//
//



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