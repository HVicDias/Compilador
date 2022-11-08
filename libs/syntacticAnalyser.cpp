#include <iostream>
#include <string>
#include <unistd.h>
#include "lexicalAnalyser.h"
#include "syntacticAnalyser.h"
#include "semanticAnalyser.h"

using namespace std;


Node analyseType(FILE *file, Node token, std::queue<string> identifierQueue, std::queue<int> lineNumberQueue) {
    if (token.simbolo != "sinteiro" && token.simbolo != "sbooleano") {
        cout << "This type is invalid, are you trying to say inteiro or booleano." << endl;
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
    std::queue<string> identifierQueue;
    std::queue<int> lineNumberQueue;

    do {
        if (token.simbolo == "sidentificador") {
            if (!searchDuplicatedVariableTable(token)) {
                identifierQueue.push(token.lexema);
                lineNumberQueue.push(lineNo);
            } else {
                cout << "ERRO " << endl;
                exit(1);
            }
            token = getToken(file);

            if (token.simbolo == "svirgula" || token.simbolo == "sdoispontos") {
                if (token.simbolo == "svirgula") {
                    token = getToken(file);
                    if (token.simbolo == "sdoispontos") {
                        cout << "Invalid expression, indentificador is expected after virgula." << endl;
                    }
                }
            } else {
                cout << "Invalid expression, virgula ou doispontos is expected after identificador." << endl;
            }
        } else {
            cout << "Invalid expression, expected identificador." << endl;
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
                cout << token.simbolo << " : " << token.lexema << endl;
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

Node analyseFunctionCall(FILE *file, Node token) {
    token = getToken(file);
    if (!searchDeclaratedFunctionTable(token.lexema)) {
        cout << "Funtion has not been declared in the code" << endl;
        exit(1);
    }

    token = getToken(file);

    return token;
}

std::pair<Node, std::string> analyseExpressions(FILE *file, Node token, std::string expression) {
    std::pair(token, expression) = analyseSimpleExpressions(file, token, expression);

    if (token.simbolo == "smaior" || token.simbolo == "smaiorig" ||
        token.simbolo == "smenor" || token.simbolo == "smenorig" ||
        token.simbolo == "sdif") {
        expression += token.lexema + ' ';
        token = getToken(file);
        std::pair(token, expression) = analyseSimpleExpressions(file, token, expression);
    }


    return {token, expression};
}

std::pair<Node, std::string> analyseFactor(FILE *file, Node token, std::string expression) {
    SymbolNode *currentNode = symbolTable.searchSymbol(token.lexema);

    if (token.simbolo == "sidentificador") {
        if (currentNode != nullptr) {
            if (currentNode->type == "função inteiro" || currentNode->type == "função booleano") {
                token = analyseFunctionCall(file, token);
            } else {
                expression += token.lexema + " ";
                token = getToken(file);
            }
        } else {
            printf("Erro15555");
        }

    } else if (token.simbolo == "snumero") {
        expression += token.lexema + " ";
        token = getToken(file);
    } else if (token.simbolo == "snao") {
        expression += token.lexema + " ";
        token = getToken(file);
        std::pair(token, expression) = analyseFactor(file, token, expression);
    } else if (token.simbolo == "sabre_parenteses") {
        expression += token.lexema + " ";
        token = getToken(file);
        std::pair(token, expression) = analyseExpressions(file, token, expression);
        if (token.simbolo == "sfecha_parenteses") {
            expression += token.lexema + " ";
            token = getToken(file);
        } else {

        }
    } else if (token.lexema == "verdadeiro" || token.lexema == "falso") {
//        expression += token.lexema + " ";
        token = getToken(file);
    } else {
        printf("Erro16");
    }

    return {token, expression};
}

std::pair<Node, std::string> analyseTerm(FILE *file, Node token, std::string expression) {
    std::pair(token, expression) = analyseFactor(file, token, expression);

    while (token.simbolo == "smult" || token.simbolo == "sdiv" || token.simbolo == "sse") {
        expression += token.lexema + ' ';
        token = getToken(file);
        std::pair(token, expression) = analyseFactor(file, token, expression);
    }

    return {token, expression};
}

std::pair<Node, std::string> analyseSimpleExpressions(FILE *file, Node token, std::string expression) {
    if (token.simbolo == "smais" || token.simbolo == "smenos") {
        expression += token.lexema + ' ';
        token = getToken(file);
    }

    std::pair(token, expression) = analyseTerm(file, token, expression);

    while (token.simbolo == "smais" || token.simbolo == "smenos" || token.simbolo == "sou" || token.simbolo == "se") {
        expression += token.lexema + ' ';
        token = getToken(file);
        std::pair(token, expression) = analyseTerm(file, token, expression);
    }

    return {token, expression};
}

Node analyseAttribution(FILE *file, Node token) {
    token = getToken(file);

    std::string expression;
    std::list<string> postfix;

    std::pair(token, expression) = analyseExpressions(file, token, expression);
    postfix = createInfixListFromExpression(expression);
    postfix = toPostfix(postfix);

    return token;
}

Node analyseProcedureCall(FILE *file, Node token) {
    token = getToken(file);

    if (!searchDeclaratedProcedureTable(token.lexema)) {
        cout << "Procedure has not been declared in the code" << endl;
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
    token = getToken(file);
    std::string expression;
    std::list<string> postfix;

    std::pair(token, expression) = analyseExpressions(file, token, expression);
    postfix = createInfixListFromExpression(expression);
    postfix = toPostfix(postfix);

    if (token.simbolo == "sfaca") {
        token = getToken(file);
        token = analyseSimpleCommands(file, token);
    } else {
        printf("Erro23");
    }

    return token;
}

Node analyseIf(FILE *file, Node token) {
    token = getToken(file);
    std::string expression;
    std::list<string> postfix;
    expression += token.lexema + " ";

    std::pair(token, expression) = analyseExpressions(file, token, expression);
    postfix = createInfixListFromExpression(expression);
    postfix = toPostfix(postfix);

    for (const string &c: postfix)
        cout << c;
    cout << token.lexema;
    if (token.simbolo == "sentao") {
        token = getToken(file);
        token = analyseSimpleCommands(file, token);
        if (token.simbolo == "ssenao") {
            token = getToken(file);
            token = analyseSimpleCommands(file, token);
        }
    } else {
        printf("Erro24");
    }

    return token;
}
