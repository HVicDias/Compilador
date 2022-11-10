#include <iostream>
#include <string>
#include <unistd.h>
#include "lexicalAnalyser.h"
#include "syntacticAnalyser.h"
#include "semanticAnalyser.h"


Node analyseType(FILE *file, Node token, std::queue<std::string> identifierQueue, std::queue<int> lineNumberQueue) {
    if (token.simbolo != "sinteiro" && token.simbolo != "sbooleano") {
        std::cout << "This type is invalid, are you trying to say inteiro or booleano." << std::endl;
    }
    while (!identifierQueue.empty()) {
        symbolTable.insertSymbol(identifierQueue.front(), symbolTable.symbolListNode->layerName, token.lexema,
                                 lineNumberQueue.front());
        identifierQueue.pop();
        lineNumberQueue.pop();
    }

    token = getToken(file);
    return token;
}

Node analyseVariables(FILE *file, Node token) {
    std::queue<std::string> identifierQueue;
    std::queue<int> lineNumberQueue;

    do {
        if (token.simbolo == "sidentificador") {
            if (!searchDuplicatedVariableTable(token)) {
                identifierQueue.push(token.lexema);
                lineNumberQueue.push(lineNo);
            } else {
                std::cout << "ERRO " << std::endl;
                exit(1);
            }
            token = getToken(file);

            if (token.simbolo == "svirgula" || token.simbolo == "sdoispontos") {
                if (token.simbolo == "svirgula") {
                    token = getToken(file);
                    if (token.simbolo == "sdoispontos") {
                        std::cout << "Invalid expression, indentificador is expected after virgula." << std::endl;
                    }
                }
            } else {
                std::cout << "Invalid expression, virgula ou doispontos is expected after identificador." << std::endl;
            }
        } else {
            std::cout << "Invalid expression, expected identificador." << std::endl;
        }
    } while (token.simbolo != "sdoispontos");

    token = getToken(file);
    token = analyseType(file, token, identifierQueue, lineNumberQueue);
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
                token = getToken(file);
                if (token.simbolo != "sfim") {
                    token = analyseSimpleCommands(file, token);
                }
            } else {
                std::cout << token.simbolo << " : " << token.lexema << " " << lineNo << std::endl;
                printf("Erro8");
                exit(0);
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
        if (!searchDeclaratedProcedureTable(token.lexema)) {
            symbolTable.downLayer(token.lexema, token.lexema, token.lexema, "procedimento", lineNo);
        }

        token = getToken(file);

        if (token.simbolo == "sponto_virgula") {
            token = analyseBlock(file, token);
        } else {
            printf("Erro");
        }
    } else {
        printf("Erro10");
    }

//    symbolTable.deleteLayer();

    return token;
}

Node analyseFunctionDeclaration(FILE *file, Node token) {
    std::string identifier;
    token = getToken(file);

    if (token.simbolo == "sidentificador") {
        token = getToken(file);
        if (!searchDeclaratedVariableOrFunctionTable(token.lexema)) {
            identifier = token.lexema;
        }

        if (token.simbolo == "sdoispontos") {
            token = getToken(file);

            if (token.simbolo == "sinteiro" || token.simbolo == "sbooleano") {
                symbolTable.insertSymbol(identifier, identifier, "função " + token.lexema, lineNo);

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

//    symbolTable.deleteLayer();

    return token;
}

TokenExpression analyseFunctionCall(FILE *file, TokenExpression te) {
    te.token = getToken(file);
    te.expression += te.token.lexema + " ";
    if (!searchDeclaratedFunctionTable(te.token.lexema)) {
        std::cout << "Funtion has not been declared in the code" << std::endl;
        exit(1);
    }

    te.token = getToken(file);

    return te;
}

TokenExpression analyseExpressions(FILE *file, TokenExpression te) {
    te = analyseSimpleExpressions(file, te);

    if (te.token.simbolo == "smaior" || te.token.simbolo == "smaiorig" ||
        te.token.simbolo == "smenor" || te.token.simbolo == "smenorig" ||
        te.token.simbolo == "sdif") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file);
        te = analyseSimpleExpressions(file, te);
    }

    return te;
}

TokenExpression analyseFactor(FILE *file, TokenExpression te) {
    SymbolNode *currentNode = symbolTable.searchSymbol(te.token.lexema);

    if (te.token.simbolo == "sidentificador") {
        if (currentNode != nullptr) {
            if (currentNode->type == "função inteiro" || currentNode->type == "função booleano") {
                te = analyseFunctionCall(file, te);
            } else {
                te.expression += te.token.lexema + " ";
                te.token = getToken(file);
            }
        } else {
            printf("Erro15555");
        }

    } else if (te.token.simbolo == "snumero") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file);
    } else if (te.token.simbolo == "snao") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file);
        te = analyseFactor(file, te);
    } else if (te.token.simbolo == "sabre_parenteses") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file);
        te = analyseExpressions(file, te);
        if (te.token.simbolo == "sfecha_parenteses") {
            te.expression += te.token.lexema + " ";
            te.token = getToken(file);
        } else {

        }
    } else if (te.token.lexema == "verdadeiro" || te.token.lexema == "falso") {
//        expression += token.lexema + " ";
        te.token = getToken(file);
    } else {
        printf("Erro16");
    }

    return te;
}

TokenExpression analyseTerm(FILE *file, TokenExpression te) {
    te = analyseFactor(file, te);

    while (te.token.simbolo == "smult" || te.token.simbolo == "sdiv" || te.token.simbolo == "sse") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file);
        te = analyseFactor(file, te);
    }

    return te;
}

TokenExpression analyseSimpleExpressions(FILE *file, TokenExpression te) {
    if (te.token.simbolo == "smais" || te.token.simbolo == "smenos") {
        te.expression += te.token.lexema + ' ';
        te.token = getToken(file);
    }

    te = analyseTerm(file, te);

    while (te.token.simbolo == "smais" || te.token.simbolo == "smenos" || te.token.simbolo == "sou" ||
           te.token.simbolo == "se") {
        te.expression += te.token.lexema + ' ';
        te.token = getToken(file);
        te = analyseTerm(file, te);
    }

    return te;
}

Node analyseAttribution(FILE *file, Node token) {
    TokenExpression te;
    token = getToken(file);
    te.token = token;

    std::list<std::string> postfix;

    te = analyseExpressions(file, te);
    postfix = createInfixListFromExpression(te.expression);
    postfix = toPostfix(postfix);
    analysePostfix(postfix);

    return te.token;
}

Node analyseProcedureCall(FILE *file, Node token) {
    token = getToken(file);

    if (!searchDeclaratedProcedureTable(token.lexema)) {
        std::cout << "Procedure has not been declared in the code" << std::endl;
        exit(1);
    }

    token = getToken(file);

    return token;
}

Node analyseAttributionAndProcedureCall(FILE *file, Node token) {
    token = getToken(file);

    if (token.simbolo == "satribuicao") {
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
            if (searchDeclaratedVariableTable(token.lexema)) {
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
            printf("Erro188");
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
            if (searchDeclaratedVariableOrFunctionTable(token.lexema)) {
                token = getToken(file);
                if (token.simbolo == "sfecha_parenteses") {
                    token = getToken(file);
                } else {
                    printf("Erro20");
                }
            } else {
                printf("Erro2111");
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
    TokenExpression te;
    token = getToken(file);
    te.token = token;

    std::list<std::string> postfix;

    te = analyseExpressions(file, te);


    if (te.token.simbolo == "sfaca") {
        te.token = getToken(file);
        te.token = analyseSimpleCommands(file, te.token);
    } else {
        printf("Erro23");
    }

    postfix = createInfixListFromExpression(te.expression);
    postfix = toPostfix(postfix);
    analysePostfix(postfix);

    return te.token;
}

Node analyseIf(FILE *file, Node token) {
    TokenExpression te;
    token = getToken(file);
    te.token = token;
    std::list<std::string> postfix;

    te = analyseExpressions(file, te);


    if (te.token.simbolo == "sentao") {
        te.token = getToken(file);
        te.token = analyseSimpleCommands(file, te.token);
        if (te.token.simbolo == "ssenao") {
            te.token = getToken(file);
            te.token = analyseSimpleCommands(file, te.token);
        }
    } else {
        printf("Erro24");
    }

    postfix = createInfixListFromExpression(te.expression);
    postfix = toPostfix(postfix);
    analysePostfix(postfix);
//    for (const std::string &c: postfix)
//        cout << c;
//
//    cout << " -> ";

    return te.token;
}
