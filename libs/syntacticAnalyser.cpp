#include <string>
#include "lexicalAnalyser.h"
#include "syntacticAnalyser.h"
#include "semanticAnalyser.h"
#include <iostream>
#include <stack>
#include "codeGenerator.h"
#include "mainwindow.h"


int currentMemoryAllocation = 1;
int currentLabel = 0;
bool lastReturn = false;
bool hadPop = false;
std::stack<SymbolNode *> headerStack;

Node analyseType(FILE *file, Node token, std::queue<std::string> identifierQueue, std::queue<int> lineNumberQueue,
                 Ui::MainWindow *ui) {
    if (token.simbolo != "sinteiro" && token.simbolo != "sbooleano") {
        ui->errorArea->appendPlainText("This type is invalid, are you trying to say inteiro or booleano.");
    }
    auto *snippet = new CodeSnippet("ALLOC", currentMemoryAllocation, identifierQueue.size());
    codeGen.insertNode(snippet);
    while (!identifierQueue.empty()) {
        symbolTable.insertSymbol(identifierQueue.front(), symbolTable.symbolListNode->layerName, token.lexema,
                                 lineNumberQueue.front(), -1, -1, currentMemoryAllocation++);
        identifierQueue.pop();
        lineNumberQueue.pop();
    }
    token = getToken(file);

    return token;
}

Node analyseVariables(FILE *file, Node token, Ui::MainWindow *ui) {
    std::queue<std::string> identifierQueue;
    std::queue<int> lineNumberQueue;

    do {
        if (token.simbolo == "sidentificador") {
            if (!searchDuplicatedVariableTable(token.lexema)) {
                identifierQueue.push(token.lexema);
                lineNumberQueue.push(lineNo + 1);
            } else {
                ui->errorArea->appendPlainText(QString::fromStdString(token.lexema + " has a previous declaration."));
//                std::cout << "ERRO " << lineNo << std::endl;
//                exit(1);
            }

            token = getToken(file);

            if (token.simbolo == "svirgula" || token.simbolo == "sdoispontos") {
                if (token.simbolo == "svirgula") {
                    token = getToken(file);
                    if (token.simbolo == "sdoispontos") {
                        ui->errorArea->appendPlainText(QString::fromStdString(
                                "Invalid expression, indentificador is expected after virgula."));
                    }
                }
            } else {
                ui->errorArea->appendPlainText(QString::fromStdString(
                        "Invalid expression, virgula ou doispontos is expected after identificador."));
            }
        } else {
            ui->errorArea->appendPlainText(QString::fromStdString("Invalid expression, expected identificador  1."));
        }
    } while (token.simbolo != "sdoispontos");

    token = getToken(file);
    token = analyseType(file, token, identifierQueue, lineNumberQueue, ui);
    return token;
}

Node analyseEtVariables(FILE *file, Node token, Ui::MainWindow *ui) {
    if (token.simbolo == "svar") {
        token = getToken(file);

        if (token.simbolo == "sidentificador") {
            while (token.simbolo == "sidentificador") {
                token = analyseVariables(file, token, ui);

                if (token.simbolo == "sponto_virgula") {
                    token = getToken(file);
                } else {
                    ui->errorArea->appendPlainText(
                            QString::fromStdString("Expected ; 1 at line ") + QString::number(lineNo) + ".");
                }
            }
        } else {
            ui->errorArea->appendPlainText(QString::fromStdString("Invalid expression, expected identificador 2."));
        }
    }

    return token;
}

Node analyseSubroutine(FILE *file, Node token, Ui::MainWindow *ui) {
    while (token.simbolo == "sprocedimento" || token.simbolo == "sfuncao") {
        if (token.simbolo == "sprocedimento") {
            token = analyseProcedureDeclaration(file, token, ui);
        } else {
            token = analyseFunctionDeclaration(file, token, ui);
        }

        if (token.simbolo == "sponto_virgula") {
            token = getToken(file);
        } else {
            ui->errorArea->appendPlainText(
                    QString::fromStdString("Expected ; 2 at line ") + QString::number(lineNo) + ".");
        }
    }

    return token;
}

Node analyseSimpleCommands(FILE *file, Node token, Ui::MainWindow *ui) {
    if (token.simbolo == "sidentificador") {
        token = analyseAttributionAndProcedureCall(file, token, ui);
    } else if (token.simbolo == "sse") {
        token = analyseIf(file, token, ui);
    } else if (token.simbolo == "senquanto") {
        token = analyseWhile(file, token, ui);
    } else if (token.simbolo == "sleia") {
        token = analyseRead(file, token, ui);
    } else if (token.simbolo == "sescreva") {
        token = analyseWrite(file, token, ui);
    } else {
        token = analyseCommands(file, token, ui);
    }

    return token;
}

Node analyseCommands(FILE *file, Node token, Ui::MainWindow *ui) {
    if (token.simbolo == "sinicio") {
        SymbolNode *auxSymbol = headerStack.top();
        if (hadPop) {

            codeGen.insertNode(new CodeSnippet(auxSymbol->labelJump, "NULL"));
            hadPop = false;
        }

        token = getToken(file);
        token = analyseSimpleCommands(file, token, ui);


        while (token.simbolo != "sfim") {
            if (token.simbolo != "sfim") {
                token = analyseSimpleCommands(file, token, ui);
            }
        }
        token = getToken(file);
    } else {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Invalid expression, expected identificador 3.") + QString::number(lineNo));
    }

    return token;
}

Node analyseBlock(FILE *file, Node token, Ui::MainWindow *ui) {
    token = getToken(file);

    token = analyseEtVariables(file, token, ui);
    token = analyseSubroutine(file, token, ui);
    token = analyseCommands(file, token, ui);

    return token;
}

Node analyseProcedureDeclaration(FILE *file, Node token, Ui::MainWindow *ui) {
    token = getToken(file);

    if (token.simbolo == "sidentificador") {
        if (!searchDuplicatedProcedureTable(token.lexema)) {

            SymbolNode *auxSymbol = headerStack.top();
            if (!hadPop) {
                auxSymbol->labelStart = ++currentLabel;
                auxSymbol->labelJump = currentLabel;
            }
            if (!lastReturn) {
                codeGen.insertNode(new CodeSnippet("JMP", currentLabel));
                auxSymbol->labelStart = currentLabel - 1;
                auxSymbol->labelJump = currentLabel;
            } else {
                lastReturn = false;
            }
            codeGen.insertNode(new CodeSnippet(++currentLabel, "NULL"));
            hadPop = false;
            headerStack.push(symbolTable.insertSymbol(token.lexema, symbolTable.symbolListNode->layerName,
                                                      "procedimento", lineNo + 1, currentLabel, currentLabel, -1));
            symbolTable.downLayer(token.lexema, token.lexema, token.lexema, "procedimento", lineNo + 1, currentLabel,
                                  currentLabel, -1);
        }

        token = getToken(file);

        if (token.simbolo == "sponto_virgula") {
            token = analyseBlock(file, token, ui);
        } else {
            ui->errorArea->appendPlainText(
                    QString::fromStdString("Expected ; 5 at line ") +
                    QString::number(lineNo) + ".");
        }
    } else {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Invalid expression, expected identificador 4."));
    }

    symbolTable.printList();
    int numberDeletion = symbolTable.deleteLayer();
    if (numberDeletion != 0) {
        auto *snippet = new CodeSnippet("DALLOC", currentMemoryAllocation - numberDeletion, numberDeletion);
        currentMemoryAllocation -= numberDeletion;
        codeGen.insertNode(snippet);
    }
    headerStack.pop();
    hadPop = true;
    codeGen.insertNode(new CodeSnippet("RETURN"));
    lastReturn = true;
    return token;
}

Node analyseFunctionDeclaration(FILE *file, Node token, Ui::MainWindow *ui) {
    std::string identifier;
    token = getToken(file);

    if (token.simbolo == "sidentificador") {
        if (!searchDeclaratedFunctionTable(token.lexema)) {
            identifier = token.lexema;
        } else {
            ui->errorArea->appendPlainText(
                    QString::fromStdString("Variable already defined at line ") +
                    QString::number(lineNo) + ".");
        }


        token = getToken(file);
        if (token.simbolo == "sdoispontos") {
            token = getToken(file);

            if (token.simbolo == "sinteiro" || token.simbolo == "sbooleano") {
                SymbolNode *auxSymbol = headerStack.top();
                if (!hadPop) {
                    auxSymbol->labelStart = ++currentLabel;
                    auxSymbol->labelJump = currentLabel;
                }
                if (!lastReturn) {
                    codeGen.insertNode(new CodeSnippet("JMP", currentLabel));
                    auxSymbol->labelStart = currentLabel - 1;
                    auxSymbol->labelJump = currentLabel;
                } else {
                    lastReturn = false;
                }
                codeGen.insertNode(new CodeSnippet(++currentLabel, "NULL"));
                hadPop = false;
                headerStack.push(symbolTable.insertSymbol(identifier, symbolTable.symbolListNode->layerName,
                                                          "funcao " + token.lexema, lineNo + 1, currentLabel,
                                                          currentLabel, -1));
                symbolTable.downLayer(identifier, identifier, identifier, "funcao " + token.lexema, lineNo + 1,
                                      currentLabel, currentLabel, -1);
                token = getToken(file);
                if (token.simbolo == "sponto_virgula") {
                    token = analyseBlock(file, token, ui);
                } else {
                    ui->errorArea->appendPlainText(
                            QString::fromStdString("Expected ; 6 at line ") +
                            QString::number(lineNo) + ".");
                }
            } else {
                ui->errorArea->appendPlainText("This type is invalid, are you trying to say inteiro or booleano.");
            }
        }
    } else {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Invalid expression, expected identificador 5."));
    }

    symbolTable.printList();
    int numberDeletion = symbolTable.deleteLayer();
    if (numberDeletion != 0) {
        auto *snippet = new CodeSnippet("DALLOC", currentMemoryAllocation - numberDeletion, numberDeletion);
        currentMemoryAllocation -= numberDeletion;
        codeGen.insertNode(snippet);
    }
    headerStack.pop();
    hadPop = true;
    codeGen.insertNode(new CodeSnippet("RETURN"));
    lastReturn = true;
    return token;
}

TokenExpression analyseFunctionCall(FILE *file, TokenExpression te, Ui::MainWindow *ui) {
    if (!searchDeclaratedFunctionTable(te.token.lexema)) {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Function has not been declared in the code"));
//        exit(1);
    } else {
        SymbolNode *node = symbolTable.searchSymbol(te.token.lexema);
        codeGen.insertNode(new CodeSnippet("CALL", node->labelStart));
    }
    te.expression += te.token.lexema + " ";
    te.token = getToken(file);

    return te;
}

TokenExpression analyseExpressions(FILE *file, TokenExpression te, Ui::MainWindow *ui) {
    te = analyseSimpleExpressions(file, te, ui);

    if (te.token.simbolo == "smaior" || te.token.simbolo == "smaiorig" ||
        te.token.simbolo == "smenor" || te.token.simbolo == "smenorig" ||
        te.token.simbolo == "sdif") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file);
        te = analyseSimpleExpressions(file, te, ui);
    }

    return te;
}

TokenExpression analyseFactor(FILE *file, TokenExpression te, Ui::MainWindow *ui) {
    if (te.token.simbolo == "sidentificador") {
        SymbolNode *currentNode = symbolTable.searchSymbol(te.token.lexema);

        if (currentNode != nullptr) {
            if (currentNode->type == "funcao inteiro" || currentNode->type == "funcao booleano") {
                te = analyseFunctionCall(file, te, ui);
            } else {
                te.expression += te.token.lexema + " ";
                te.token = getToken(file);
            }
        } else {
            ui->errorArea->appendPlainText(
                    QString::fromStdString(te.token.lexema + " is not defined."));
        }

    } else if (te.token.simbolo == "snumero") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file);
    } else if (te.token.simbolo == "snao") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file);
        te = analyseFactor(file, te, ui);
    } else if (te.token.simbolo == "sabre_parenteses") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file);
        te = analyseExpressions(file, te, ui);
        if (te.token.simbolo == "sfecha_parenteses") {
            te.expression += te.token.lexema + " ";
            te.token = getToken(file);
        } else {

        }
    } else if (te.token.lexema == "verdadeiro" || te.token.lexema == "falso") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file);
    } else {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Invalid symbol at line ") +
                QString::number(lineNo) + ".");
    }

    return te;
}

TokenExpression analyseTerm(FILE *file, TokenExpression te, Ui::MainWindow *ui) {
    te = analyseFactor(file, te, ui);

    while (te.token.simbolo == "smult" || te.token.simbolo == "sdiv" || te.token.simbolo == "se") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file);
        te = analyseFactor(file, te, ui);
    }

    return te;
}

TokenExpression analyseSimpleExpressions(FILE *file, TokenExpression te, Ui::MainWindow *ui) {
    if (te.token.simbolo == "smais" || te.token.simbolo == "smenos") {
        te.expression += te.token.lexema + ' ';
        te.token = getToken(file);
    }

    te = analyseTerm(file, te, ui);

    while (te.token.simbolo == "smais" || te.token.simbolo == "smenos" || te.token.simbolo == "sou" ||
           te.token.simbolo == "se") {
        te.expression += te.token.lexema + ' ';
        te.token = getToken(file);
        te = analyseTerm(file, te, ui);
    }

    return te;
}

Node analyseAttribution(FILE *file, Node token, Ui::MainWindow *ui) {
    TokenExpression te;
    token = getToken(file);
    te.token = token;

    std::list<std::string> postfix;

    te = analyseExpressions(file, te, ui);

    postfix = createInfixListFromExpression(te.expression);
    postfix = toPostfix(postfix);
    analysePostfix(postfix, ui);

    return te.token;
}

Node analyseProcedureCall(FILE *file, Node token, Ui::MainWindow *ui) {
    if (!searchDeclaratedProcedureTable(token.lexema)) {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Procedure has not been declared in the code"));
//        exit(1);
    } else {
        SymbolNode *node = symbolTable.searchSymbol(token.lexema);
        codeGen.insertNode(new CodeSnippet("CALL", node->labelStart));
    }
    token = getToken(file);

    return token;
}

Node analyseAttributionAndProcedureCall(FILE *file, Node token, Ui::MainWindow *ui) {
    if (!searchDeclaratedProcedureTable(token.lexema))
        token = getToken(file);

    if (token.simbolo == "satribuicao") {
        token = analyseAttribution(file, token, ui);
    } else {
        token = analyseProcedureCall(file, token, ui);
    }

    if (token.simbolo == "sponto_virgula") {
        token = getToken(file);
    } else {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Expected ;"));
    }

    return token;
}

Node analyseRead(FILE *file, Node token, Ui::MainWindow *ui) {
    token = getToken(file);

    if (token.simbolo == "sabre_parenteses") {
        token = getToken(file);

        if (token.simbolo == "sidentificador") {
            if (searchDeclaratedVariableTable(token.lexema)) {
                auto auxToken = symbolTable.searchSymbol(token.lexema);
                auto *snippet = new CodeSnippet("RD");
                codeGen.insertNode(snippet);
                snippet = new CodeSnippet("STR", auxToken->memoryAllocation);
                codeGen.insertNode(snippet);
                token = getToken(file);

                if (token.simbolo == "sfecha_parenteses") {
                    token = getToken(file);

                    if (token.simbolo == "sponto_virgula") {
                        token = getToken(file);
                    } else {
                        ui->errorArea->appendPlainText(
                                QString::fromStdString("Expected ponto virgula"));
                    }
                } else {
                    ui->errorArea->appendPlainText(
                            QString::fromStdString("Expected ) at the end of the expression"));
                }
            } else {
                ui->errorArea->appendPlainText(
                        QString::fromStdString("Identifier is not defined."));
            }
        } else {
            ui->errorArea->appendPlainText(
                    QString::fromStdString("Invalid expression, expected identificador 6."));
        }
    } else {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Expected ( at the start of the expression"));
    }

    return token;
}

Node analyseWrite(FILE *file, Node token, Ui::MainWindow *ui) {
    token = getToken(file);

    if (token.simbolo == "sabre_parenteses") {
        token = getToken(file);

        if (token.simbolo == "sidentificador") {
            if (searchDeclaratedVariableOrFunctionTable(token.lexema)) {
                auto auxToken = symbolTable.searchSymbol(token.lexema);
                auto *snippet = new CodeSnippet("LDV", auxToken->memoryAllocation);
                codeGen.insertNode(snippet);
                snippet = new CodeSnippet("PRN");
                codeGen.insertNode(snippet);
                token = getToken(file);

                if (token.simbolo == "sfecha_parenteses") {
                    token = getToken(file);

                    if (token.simbolo == "sponto_virgula") {
                        token = getToken(file);
                    } else {
                        ui->errorArea->appendPlainText(
                                QString::fromStdString("Expected ponto virgula"));
                    }
                } else {
                    ui->errorArea->appendPlainText(
                            QString::fromStdString("Expected ) at the end of the expression"));
                }
            } else {
                ui->errorArea->appendPlainText(
                        QString::fromStdString("Identifier is not defined."));
            }
        } else {
            ui->errorArea->appendPlainText(
                    QString::fromStdString("Invalid expression, expected identificador 7."));
        }
    } else {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Expected ( at the start of the expression"));
    }

    return token;
}

Node analyseWhile(FILE *file, Node token, Ui::MainWindow *ui) {
    TokenExpression te;
    token = getToken(file);
    te.token = token;

    std::list<std::string> postfix;

    te = analyseExpressions(file, te, ui);

    postfix = createInfixListFromExpression(te.expression);
    postfix = toPostfix(postfix);
    analysePostfix(postfix, ui);

    if (te.token.simbolo == "sfaca") {
        te.token = getToken(file);
        te.token = analyseSimpleCommands(file, te.token, ui);
    } else {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Expected faca at the start of the expression"));
    }

    te.token = getToken(file);

    return te.token;
}

Node analyseIf(FILE *file, Node token, Ui::MainWindow *ui) {
    TokenExpression te;
    token = getToken(file);

    te.token = token;
    std::list<std::string> postfix;

    te = analyseExpressions(file, te, ui);
    postfix = createInfixListFromExpression(te.expression);
    postfix = toPostfix(postfix);
    analysePostfix(postfix, ui);

    if (te.token.simbolo == "sentao") {
        te.token = getToken(file);
        te.token = analyseSimpleCommands(file, te.token, ui);

        if (te.token.simbolo == "ssenao") {
            te.token = getToken(file);
            te.token = analyseSimpleCommands(file, te.token, ui);
        }
    } else {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Expected entao at the start of the expression"));
    }

    return te.token;
}
