#include <cstring>
#include <string>
#include "lexicalAnalyser.h"
#include "syntacticAnalyser.h"

using namespace std;


Node analyseType(FILE* file, Node token) {
    if(token.simbolo != "sinteiro" && token.simbolo != "sbooleano") {
        printf("Erro");
    }
    token = getToken(file);

    return token;
}

Node analyseVariables(FILE* file, Node token) {
    while(token.simbolo != "sdoispontos") {
        if(token.simbolo == "sindentificador") {
            token = getToken(file);

            if(token.simbolo == "svirgula" || token.simbolo == "sdoispontos") {
                if(token.simbolo == "svirgula") {
                    token = getToken(file);
                    if(token.simbolo == "sdoispontos") {
                        printf("Erro");
                    }
                }
            } else {
                printf("Erro");
            }
        } else {
            printf("Erro");
        }
    }
    token = getToken(file);
    token = analyseType(file, token);

    return token;
}

Node analyseEtVariables(FILE* file, Node token) {
    if(token.simbolo == "svar") {
        token = getToken(file);

        if(token.simbolo == "sidentificador") {
            token = analyseVariables(file, token);

            if(token.simbolo == "sponto_vírgula") {
                token = getToken(file);
            } else {
                printf("Erro");
            }
        }
    } else {
        printf("Erro");
    }

    return token;
}

//----------------------------//

Node analyseAtribChprocedimento(FILE* file, Node token){

    token = getToken(file);
    if (token.simbolo == "satribuição"){
        analyseAtribuicao(file, token);
    }else {
        chamadaProcedimento(file, token);
    }

    return token;
}

Node analyseLeia(FILE* file, Node token){

    token = getToken(file);
    if (token.simbolo == "sabre_parenteses"){
        token = getToken(file);
        if (token.simbolo == "sidentificador"){
            token = getToken(file);
            if (token.simbolo == "sfecha_parenteses"){
                token = getToken(file);
            }else {
                printf("Erro");
            }
        }else {
            printf("Erro");
        }
    }else {
        printf("Erro");
    }

    return token;
}

Node analyseEscreva(FILE* file, Node token){

    token = getToken(file);
    if (token.simbolo == "sabre_parenteses"){
        token = getToken(file);
        if (token.simbolo == "sidentificador"){
            token = getToken(file);
            if (token.simbolo == "sfecha_parenteses"){
                token = getToken(file);
            }else {
                printf("Erro");
            }
        }else {
            printf("Erro");
        }
    }else {
        printf("Erro");
    }

    return token;
}

Node analyseEnquanto(FILE* file, Node token){

    token = getToken(file);
    analyseExpressao(file, token);
    if (token.simbolo == "sfaça"){
        token = getToken(file);
        analyseSimpleCommands(file, token);
    }else {
        printf("Erro");
    }

    return token;
}

Node analyseSe(FILE* file, Node token){

    token = getToken(file);
    analyseExpressao(file, token);
    if (token.simbolo == "sentão"){
        token = getToken(file);
        analyseSimpleCommands(file, token);
        if (token.simbolo == "Ssenão"){
            token = getToken(file);
            analyseSimpleCommands(file, token);
        }
    }else {
        printf("Erro");
    }

    return token;
}

Node analyseSimpleCommands(FILE* file, Node token) {

    if(token.simbolo == "sidentificador"){
        analyseAtribChprocedimento(file, token);
    }else if(token.simbolo == "sse"){
        analyseSe(file, token);
    }else if(token.simbolo == "senquanto"){
        analyseEnquanto(file, token);
    }else if(token.simbolo == "sleia"){
        analyseLeia(file, token);
    }else if(token.simbolo == "sescreva"){
        analyseEscreva(file, token);
    }else{
        analyseCommands(file,token);
    }

    return token;
}

Node analyseCommands(FILE* file, Node token) {

    if (token.simbolo == "sinicio"){
        token = getToken(file);
        analyseSimpleCommands(file,token);
        while(token.simbolo != "sfim"){
            if(token.simbolo == "spontovirgula"){
                analyseSimpleCommands(file,token);
            }else{
                printf("Erro");
            }
        }
        token = getToken(file);
    }else {
        printf("Ërro");
    }

    return token;
}

//----------------------------------------------//


