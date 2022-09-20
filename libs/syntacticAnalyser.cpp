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

Node analyseSubroutine(FILE* file, Node token) {
    while(token.simbolo == "sprocedimento" || token.simbolo == "sfunção"){
        if (token.simbolo == "sprocedimento"){
            token = analyseProcedureDeclaration(file, token);
        }else {
            token = analyseFunctionDeclaration(file, token);
        }
        if (token.simbolo == "sponto-virgula"){
            token = getToken(file);
        }else {
            printf("ERRO");
        }
    }

    return token;
}

Node analyseSimpleCommands(FILE* file, Node token) {
    if(token.simbolo == "sidentificador"){
        token = analyseAttributionAndProcedureCall(file, token);
    }else if(token.simbolo == "sse"){
        token = analyseIf(file, token);
    }else if(token.simbolo == "senquanto"){
        token = analyseWhile(file, token);
    }else if(token.simbolo == "sleia"){
        token = analyseRead(file, token);
    }else if(token.simbolo == "sescreva"){
        token = analyseWrite(file, token);
    }else{
        token = analyseCommands(file,token);
    }

    return token;
}

Node analyseCommands(FILE* file, Node token) {
    if (token.simbolo == "sinicio"){
        token = getToken(file);
        token = analyseSimpleCommands(file,token);
        while(token.simbolo != "sfim"){
            if(token.simbolo == "spontovirgula"){
                token = analyseSimpleCommands(file,token);
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

Node analyseBlock(FILE* file, Node token) {
    token = getToken(file);

    analyseEtVariables(file, token);
    analyseSubroutine(file, token);
    analyseCommands(file, token);

    return token;
}

Node analyseProcedureDeclaration(FILE* file, Node token) {
    token = getToken(file);

    if(token.simbolo == "sidentificador") {
        token = getToken(file);

        if(token.simbolo == "sponto_virgula") {
            token = analyseBlock(file, token);
        } else {
            printf("Erro");
        }
    } else {
        printf("Erro");
    }

    return token;
}

Node analyseFunctionDeclaration(FILE* file, Node token) {
    token = getToken(file);

    if(token.simbolo == "sidentificador") {
        token = getToken(file);

        if(token.simbolo == "sdoispontos") {
            token = getToken(file);

            if(token.simbolo == "sinteiro" || token.simbolo == "sbooleano") {
                token = getToken(file);

                if(token.simbolo == "sponto_virgula") {
                    token = analyseBlock(file, token);
                } else {
                    printf("Erro");
                }
            } else {
                printf("Erro");
            }
        } else {
            printf("Erro");
        }
    } else {
        printf("Erro");
    }

    return token;
}

Node analyseFunctionCall(FILE* file, Node token){
    return token;
}

Node analyseFactor(FILE* file, Node token){
    if (token.simbolo == "sidentificador"){
        token = analyseFunctionCall(file, token);
    }else if(token.simbolo == "snumero"){
        token = getToken(file);
    }else if(token.simbolo == "snao"){
        token = getToken(file);
        token = analyseFactor(file, token);
    }else if(token.simbolo == "sabre_parenteses"){
        token = getToken(file);
        token = analyseExpressions(file, token);
        if (token.simbolo == "sfecha_parenteses"){
            token = getToken(file);
        }else {
            printf("Erro");
        }
    } else if(token.lexema == "verdadeiro" || token.lexema == "falso") {
        token = getToken(file);
    } else {
        printf("Erro");
    }

    return token;
}

Node analyseTerm(FILE* file, Node token) {
    token = analyseFactor(file, token);

    while(token.simbolo == "smult" || token.simbolo == "sdiv" || token.simbolo == "if") {
        token = getToken(file);
        token = analyseFactor(file, token);
    }
    return token;
}

Node analyseExpressions(FILE* file, Node token) {
    token = analyseSimpleExpressions(file, token);
    if (token.simbolo == "smaior" || token.simbolo == "smaiorig" ||
    token.simbolo == "smenor" || token.simbolo == "smenorig" ||
    token.simbolo == "sdif"){
        token = getToken(file);
        token = analyseSimpleExpressions(file, token);
    }

    return token;
}

Node analyseSimpleExpressions(FILE* file, Node token) {
    if (token.simbolo == "smais" || token.simbolo == "smenos"){
        token = getToken(file);
        token = analyseTerm(file, token);
        while (token.simbolo == "smais" || token.simbolo == "smenos" || token.simbolo == "sou"){
            token = getToken(file);
            token = analyseTerm(file, token);
        }
    }

    return token;
}

Node analyseAttribution(FILE* file, Node token) {
    token = getToken(file);

    token = analyseExpressions(file, token);

    return token;
}

Node analyseProcedureCall(FILE* file, Node token) {
    return token;
}

Node analyseAttributionAndProcedureCall(FILE* file, Node token){
    token = getToken(file);
    if (token.simbolo == "satribuição"){
        token = analyseAttribution(file, token);
    }else {
        token = analyseProcedureCall(file, token);
    }

    return token;
}

Node analyseRead(FILE* file, Node token){

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

Node analyseWrite(FILE* file, Node token) {
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

Node analyseWhile(FILE* file, Node token){

    token = getToken(file);
    token = analyseExpressions(file, token);
    if (token.simbolo == "sfaça"){
        token = getToken(file);
        token = analyseSimpleCommands(file, token);
    }else {
        printf("Erro");
    }

    return token;
}

Node analyseIf(FILE* file, Node token){

    token = getToken(file);
    token = analyseExpressions(file, token);
    if (token.simbolo == "sentão"){
        token = getToken(file);
        token = analyseSimpleCommands(file, token);
        if (token.simbolo == "Ssenão"){
            token = getToken(file);
            token = analyseSimpleCommands(file, token);
        }
    }else {
        printf("Erro");
    }

    return token;
}
