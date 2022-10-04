#include <iostream>
#include <string>
#include <unistd.h>
#include "lexicalAnalyser.h"
#include "syntacticAnalyser.h"

using namespace std;

Node analyseType(FILE *file, Node token) {
    if (token.simbolo != "sinteiro" && token.simbolo != "sbooleano") {
        printf("Erro1");
    }
    token = getToken(file);
    return token;
}

Node analyseVariables(FILE *file, Node token) {
    do {
        if (token.simbolo == "sidentificador") {
            token = getToken(file);
            if (token.simbolo == "svirgula" || token.simbolo == "sdoispontos") {
                if (token.simbolo == "svirgula") {
                    token = getToken(file);
                    if (token.simbolo == "sdoispontos") {
                        printf("Erro2");
                    }
                }
            } else {
                printf("Erro3");
            }
        } else {
            printf("Erro4");
        }
    } while (token.simbolo != "sdoispontos");

    token = getToken(file);
    token = analyseType(file, token);
    return token;
}

Node analyseEtVariables(FILE *file, Node token) {
    if (token.simbolo == "svar") {
        token = getToken(file);

        if (token.simbolo == "sidentificador") {
            while (token.simbolo == "sidentificador") {
                token = analyseVariables(file, token);

                if (token.simbolo == "sponto_virgula") {
                    token = getToken(file);
                } else {
                    printf("Erro5");
                }
            }
        } else {
            printf("Erroababab");
        }
    }

    return token;
}

Node analyseSubroutine(FILE *file, Node token) {
    while (token.simbolo == "sprocedimento" || token.simbolo == "sfunção") {
        if (token.simbolo == "sprocedimento") {
            token = analyseProcedureDeclaration(file, token);
        } else {
            token = analyseFunctionDeclaration(file, token);
        }
        if (token.simbolo == "sponto_virgula") {
            token = getToken(file);
        } else {
            printf("Erro7");
        }
    }

    return token;
}

Node analyseSimpleCommands(FILE *file, Node token) {
    if (token.simbolo == "sidentificador") {
        token = analyseAttributionAndProcedureCall(file, token);
    } else if (token.simbolo == "sse") {
        token = analyseIf(file, token);
    } else if (token.simbolo == "senquanto") {
        token = analyseWhile(file, token);
    } else if (token.simbolo == "sleia") {
        token = analyseRead(file, token);
    } else if (token.simbolo == "sescreva") {
        token = analyseWrite(file, token);
    } else {
        token = analyseCommands(file, token);
    }

    return token;
}

Node analyseCommands(FILE *file, Node token) {
    if (token.simbolo == "sinicio") {
        token = getToken(file);
        token = analyseSimpleCommands(file, token);
        while (token.simbolo != "sfim") {
            if (token.simbolo == "sponto_virgula") {
                token = analyseSimpleCommands(file, token);
            } else {
                printf("Erro8");
            }
        }
        token = getToken(file);
    } else {
        printf("Erro9");
    }

    return token;
}

Node analyseBlock(FILE *file, Node token) {
    token = getToken(file);

    token = analyseEtVariables(file, token);
    token = analyseSubroutine(file, token);
    token = analyseCommands(file, token);

    return token;
}

Node analyseProcedureDeclaration(FILE *file, Node token) {
    token = getToken(file);

    if (token.simbolo == "sidentificador") {
        token = getToken(file);

        if (token.simbolo == "sponto_virgula") {
            token = analyseBlock(file, token);
        } else {
            printf("Erro");
        }
    } else {
        printf("Erro10");
    }

    return token;
}

Node analyseFunctionDeclaration(FILE *file, Node token) {
    token = getToken(file);

    if (token.simbolo == "sidentificador") {
        token = getToken(file);

        if (token.simbolo == "sdoispontos") {
            token = getToken(file);

            if (token.simbolo == "sinteiro" || token.simbolo == "sbooleano") {
                token = getToken(file);

                if (token.simbolo == "sponto_virgula") {

                    token = analyseBlock(file, token);
                } else {
                    printf("Erro11");
                }
            } else {
                printf("Erro12");
            }
        }
    } else {
        printf("Erro14");
    }

    return token;
}

Node analyseFunctionCall(FILE *file, Node token) {
    return token;
}

Node analyseFactor(FILE *file, Node token) {
    if (token.simbolo == "sidentificador") {
        token = analyseFunctionCall(file, token);
        token = getToken(file);
    } else if (token.simbolo == "snumero") {
        token = getToken(file);
    } else if (token.simbolo == "snao") {
        token = getToken(file);
        token = analyseFactor(file, token);
    } else if (token.simbolo == "sabre_parenteses") {
        token = getToken(file);
        token = analyseExpressions(file, token);
        if (token.simbolo == "sfecha_parenteses") {
            token = getToken(file);
        } else {
            printf("Erro15");
        }
    } else if (token.lexema == "verdadeiro" || token.lexema == "falso") {
        token = getToken(file);
    } else {
        printf("Erro16");
    }

    return token;
}

Node analyseTerm(FILE *file, Node token) {
    token = analyseFactor(file, token);

    while (token.simbolo == "smult" || token.simbolo == "sdiv" || token.simbolo == "sse") {
        token = getToken(file);
        token = analyseFactor(file, token);
    }

    return token;
}

Node analyseExpressions(FILE *file, Node token) {
    token = analyseSimpleExpressions(file, token);

    if (token.simbolo == "smaior" || token.simbolo == "smaiorig" ||
        token.simbolo == "smenor" || token.simbolo == "smenorig" ||
        token.simbolo == "sdif") {
        token = getToken(file);
        token = analyseSimpleExpressions(file, token);
    }

    return token;
}

Node analyseSimpleExpressions(FILE *file, Node token) {
    if (token.simbolo == "smais" || token.simbolo == "smenos") {
        token = getToken(file);
    }

    token = analyseTerm(file, token);

    while (token.simbolo == "smais" || token.simbolo == "smenos" || token.simbolo == "sou") {
        token = getToken(file);
        token = analyseTerm(file, token);
    }

    return token;
}

Node analyseAttribution(FILE *file, Node token) {
    token = getToken(file);

    token = analyseExpressions(file, token);

    return token;
}

Node analyseProcedureCall(FILE *file, Node token) {
    return token;
}

Node analyseAttributionAndProcedureCall(FILE *file, Node token) {
    token = getToken(file);
    if (token.simbolo == "satribuição") {
        token = analyseAttribution(file, token);
    } else {
        token = analyseProcedureCall(file, token);
    }

    return token;
}

Node analyseRead(FILE *file, Node token) {
    token = getToken(file);

    if (token.simbolo == "sabre_parenteses") {
        token = getToken(file);
        if (token.simbolo == "sidentificador") {
            token = getToken(file);
            if (token.simbolo == "sfecha_parenteses") {
                token = getToken(file);
            } else {
                printf("Erro17");
            }
        } else {
            printf("Erro18");
        }
    } else {
        printf("Erro19");
    }

    return token;
}

Node analyseWrite(FILE *file, Node token) {
    token = getToken(file);

    if (token.simbolo == "sabre_parenteses") {
        token = getToken(file);
        if (token.simbolo == "sidentificador") {
            token = getToken(file);
            if (token.simbolo == "sfecha_parenteses") {
                token = getToken(file);
            } else {
                printf("Erro20");
            }
        } else {
            printf("Erro21");
        }
    } else {
        printf("Erro22");
    }

    return token;
}

Node analyseWhile(FILE *file, Node token) {

    token = getToken(file);
    token = analyseExpressions(file, token);
    if (token.simbolo == "sfaça") {
        token = getToken(file);
        token = analyseSimpleCommands(file, token);
    } else {
        printf("Erro23");
    }

    return token;
}

Node analyseIf(FILE *file, Node token) {
    token = getToken(file);
    token = analyseExpressions(file, token);
    cout << token.simbolo << endl;
    if (token.simbolo == "sentao") {
        token = getToken(file);
        token = analyseSimpleCommands(file, token);
        if (token.simbolo == "Ssenao") {
            token = getToken(file);
            token = analyseSimpleCommands(file, token);
        }
    } else {
        printf("Erro24");
    }

    return token;
}
