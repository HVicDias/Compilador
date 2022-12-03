#include <string>
#include "lexicalAnalyser.h"
#include "syntacticAnalyser.h"
#include "semanticAnalyser.h"
#include <stack>
#include "codeGenerator.h"
#include "mainwindow.h"
#include <cstring>


int currentMemoryAllocation = 1;
int currentLabel = 0;
bool lastReturn = false;
bool hadPop = false;
SymbolNode *attributionIdentifier;

std::stack<SymbolNode *> headerStack;

Node analyseType(FILE *file, Node token, std::queue<std::string> identifierQueue, std::queue<int> lineNumberQueue,
                 Ui::MainWindow *ui) {
    if (token.simbolo != "sinteiro" && token.simbolo != "sbooleano") {
        ui->ErrorArea->appendPlainText("Linha " + QString::number(lineNo) +
                                       ": Erro Sintático -> Tipo inválido, esperado inteiro ou booleano.");
    }
    auto *snippet = new CodeSnippet("ALLOC", currentMemoryAllocation, identifierQueue.size());
    codeGen.insertNode(snippet);
    while (!identifierQueue.empty()) {
        symbolTable.insertSymbol(identifierQueue.front(), symbolTable.symbolListNode->layerName, token.lexema,
                                 lineNumberQueue.front(), -1, -1, currentMemoryAllocation++);
        identifierQueue.pop();
        lineNumberQueue.pop();
    }
    token = getToken(file, ui);

    return token;
}

Node analyseVariables(FILE *file, Node token, Ui::MainWindow *ui) {
    std::queue<std::string> identifierQueue;
    std::queue<int> lineNumberQueue;

    do {
        if (token.simbolo == "sidentificador") {
            if (!searchDuplicatedVariableTable(token.lexema)) {
                identifierQueue.push(token.lexema);
                lineNumberQueue.push(lineNo);
            } else {
                ui->ErrorArea->appendPlainText("Linha " + QString::number(lineNo) +
                                               ": Erro Semântico -> Identificador \"" +
                                               QString::fromStdString(token.lexema) + "\" já declarado.");
            }

            token = getToken(file, ui);

            if (token.simbolo == "svirgula" || token.simbolo == "sdoispontos") {
                if (token.simbolo == "svirgula") {
                    token = getToken(file, ui);
                    if (token.simbolo == "sdoispontos") {
                        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                        ": Erro Sintático -> \":\" não esperado."));
                    }
                }
            } else {
                ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                ": Erro Sintático -> Esperado \",\" ou \":\" após um identificador."));
            }
        } else {
            ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                            ": Erro Sintático -> Esperado um identificador."));
        }
    } while (token.simbolo != "sdoispontos");

    token = getToken(file, ui);
    token = analyseType(file, token, identifierQueue, lineNumberQueue, ui);
    return token;
}

Node analyseVariablesStep(FILE *file, Node token, Ui::MainWindow *ui) {
    if (token.simbolo == "svar") {
        token = getToken(file, ui);

        if (token.simbolo == "sidentificador") {
            while (token.simbolo == "sidentificador") {
                token = analyseVariables(file, token, ui);

                if (token.simbolo == "sponto_virgula") {
                    token = getToken(file, ui);
                } else {
                    ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                    ": Erro Sintático -> Esperado \";\"."));
                }
            }
        } else {
            ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                            ": Erro Sintático -> Esperado um identificador."));
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
            token = getToken(file, ui);
        } else {
            ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                            ": Erro Sintático -> Esperado \";\"."));
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

        token = getToken(file, ui);
        token = analyseSimpleCommands(file, token, ui);

        while (token.simbolo != "sfim") {
            token = analyseSimpleCommands(file, token, ui);
        }


        token = getToken(file, ui);
//        if (token.simbolo == "sponto_virgula") {
//            token = getToken(file, ui);
//        }
    } else {
        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                        ": Erro Sintático -> Esperado inicio."));
        token = getToken(file, ui);
    }

    return token;
}

Node analyseBlock(FILE *file, Node token, Ui::MainWindow *ui) {
    token = getToken(file, ui);

    token = analyseVariablesStep(file, token, ui);
    token = analyseSubroutine(file, token, ui);
    token = analyseCommands(file, token, ui);

    return token;
}

Node analyseProcedureDeclaration(FILE *file, Node token, Ui::MainWindow *ui) {
    token = getToken(file, ui);

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
                                                      "procedimento", lineNo, currentLabel, currentLabel, -1));
            symbolTable.downLayer(token.lexema, token.lexema, token.lexema, "procedimento", lineNo, currentLabel,
                                  currentLabel, -1);
        }

        token = getToken(file, ui);

        if (token.simbolo == "sponto_virgula") {
            token = analyseBlock(file, token, ui);
        } else {
            ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                            ": Erro Sintático -> Esperado \";\"."));
        }
    } else {
        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                        ": Erro Sintático -> Esperado um identificador."));
        token = getToken(file, ui);
    }

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
    token = getToken(file, ui);

    if (token.simbolo == "sidentificador") {
        if (!searchDeclaratedFunctionTable(token.lexema)) {
            identifier = token.lexema;
        } else {
            ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                            ": Erro Semântico -> Identificador já declarado."));
        }


        token = getToken(file, ui);
        if (token.simbolo == "sdoispontos") {
            token = getToken(file, ui);

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
                                                          "funcao " + token.lexema, lineNo, currentLabel,
                                                          currentLabel, -1));
                symbolTable.downLayer(identifier, identifier, identifier, "funcao " + token.lexema, lineNo,
                                      currentLabel, currentLabel, -1);
                token = getToken(file, ui);
                if (token.simbolo == "sponto_virgula") {
                    token = analyseBlock(file, token, ui);
                } else {
                    ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                    ": Erro Sintático -> Esperado \";\"."));
                }
            } else {
                ui->ErrorArea->appendPlainText("Linha " + QString::number(lineNo) +
                                               ": Erro Sintático -> Tipo inválido, esperado inteiro ou booleano.");
            }
        }
    } else {
        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                        ": Erro Sintático -> Esperado um identificador."));
    }

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
        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                        ": Erro Semântico -> Função não declarada."));
//        exit(1);
    } else {
        SymbolNode *node = symbolTable.searchSymbol(te.token.lexema);
    }
    te.expression += te.token.lexema + " ";
    te.token = getToken(file, ui);

    return te;
}

TokenExpression analyseExpressions(FILE *file, TokenExpression te, Ui::MainWindow *ui) {
    te = analyseSimpleExpressions(file, te, ui);

    if (te.token.simbolo == "smaior" || te.token.simbolo == "smaiorig" ||
        te.token.simbolo == "smenor" || te.token.simbolo == "smenorig" ||
        te.token.simbolo == "sdif" || te.token.simbolo == "sig") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file, ui);
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
                te.token = getToken(file, ui);
            }
        } else {
            ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                            ": Erro Semântico -> \"" + QString::fromStdString(te.token.lexema) +
                                            "\" não declarado."));
            te.token = getToken(file, ui);
        }
    } else if (te.token.simbolo == "snumero") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file, ui);
    } else if (te.token.simbolo == "snao") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file, ui);
        te = analyseFactor(file, te, ui);
    } else if (te.token.simbolo == "sverdadeiro" || te.token.simbolo == "sfalso") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file, ui);
    } else if (te.token.simbolo == "sabre_parenteses") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file, ui);
        te = analyseExpressions(file, te, ui);
        if (te.token.simbolo == "sfecha_parenteses") {
            te.expression += te.token.lexema + " ";
            te.token = getToken(file, ui);
        } else {
            ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                            ": Erro Sintático -> Esperado \")\"."));
        }
    } else {
        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                        ": Erro Sintático -> Símbolo \"" + QString::fromStdString(te.token.lexema) +
                                        "\" inválido."));
    }

    return te;
}

TokenExpression analyseTerm(FILE *file, TokenExpression te, Ui::MainWindow *ui) {
    te = analyseFactor(file, te, ui);

    while (te.token.simbolo == "smult" || te.token.simbolo == "sdiv" || te.token.simbolo == "se") {
        te.expression += te.token.lexema + " ";
        te.token = getToken(file, ui);
        te = analyseFactor(file, te, ui);
    }

    return te;
}

TokenExpression analyseSimpleExpressions(FILE *file, TokenExpression te, Ui::MainWindow *ui) {
    if (te.token.simbolo == "smais" || te.token.simbolo == "smenos") {
        if (te.token.simbolo == "smais")
            te.expression += "+u ";
        if (te.token.simbolo == "smenos")
            te.expression += "-u ";
        te.token = getToken(file, ui);
    }

    te = analyseTerm(file, te, ui);

    while (te.token.simbolo == "smais" || te.token.simbolo == "smenos" || te.token.simbolo == "sou") {
        te.expression += te.token.lexema + ' ';
        te.token = getToken(file, ui);
        te = analyseTerm(file, te, ui);
    }

    return te;
}

Node analyseAttribution(FILE *file, Node token, Node attributionToken, Ui::MainWindow *ui) {
    TokenExpression te;
    SymbolNode *tableToken = symbolTable.searchSymbol(attributionToken.lexema);

    token = getToken(file, ui);
    te.token = token;

    std::list<std::string> postfix;

    te = analyseExpressions(file, te, ui);
    if (ui->ErrorArea->toPlainText().isEmpty()) {
        postfix = createInfixListFromExpression(te.expression);
        postfix = toPostfix(postfix);
        postfix = analysePostfix(postfix, tableToken->memoryAllocation, ui);
        if ((attributionIdentifier->type == "booleano" || attributionIdentifier->type == "funcao booleano")
            && strcmpi(postfix.begin()->c_str(), "#B") != 0) {
            ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                            ": Erro Semântico -> Expressão inválida."));
        } else if ((attributionIdentifier->type == "inteiro" || attributionIdentifier->type == "funcao inteiro")
                   && strcmpi(postfix.begin()->c_str(), "#I") != 0) {
            ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                            ": Erro Semântico -> Expressão inválida."));
        }
        postfix.clear();
    }

    return te.token;
}

Node analyseProcedureCall(FILE *file, Node token, Ui::MainWindow *ui) {
    if (!searchDeclaratedProcedureTable(token.lexema)) {
        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                        ": Erro Semântico -> Procedimento não declarado."));

    } else {
        SymbolNode *node = symbolTable.searchSymbol(token.lexema);
        codeGen.insertNode(new CodeSnippet("CALL", node->labelStart));
        token = getToken(file, ui);
    }

    return token;
}

Node analyseAttributionAndProcedureCall(FILE *file, Node token, Ui::MainWindow *ui) {
    Node auxAttributionToken = token;

    if (!searchDeclaratedProcedureTable(token.lexema)) {
        if (searchVariableAndFunctionTable(token.lexema)) {
            attributionIdentifier = symbolTable.searchSymbol(token.lexema);
        } else {
            ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                            ": Erro Semântico -> Identificador não encontrado."));
        }
        token = getToken(file, ui);
    }
    if (token.simbolo == "satribuicao") {
        token = analyseAttribution(file, token, auxAttributionToken, ui);
    } else {
        token = analyseProcedureCall(file, token, ui);
    }

    if (token.simbolo == "sponto_virgula") {
        token = getToken(file, ui);
    } else {
        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                        ": Erro Sintático -> Esperado \";\"."));
    }

    return token;
}

Node analyseRead(FILE *file, Node token, Ui::MainWindow *ui) {
    token = getToken(file, ui);

    if (token.simbolo == "sabre_parenteses") {
        token = getToken(file, ui);

        if (token.simbolo == "sidentificador") {
            if (searchDeclaratedVariableTable(token.lexema)) {
                auto auxToken = symbolTable.searchSymbol(token.lexema);
                auto *snippet = new CodeSnippet("RD");
                codeGen.insertNode(snippet);
                snippet = new CodeSnippet("STR", auxToken->memoryAllocation);
                codeGen.insertNode(snippet);
                token = getToken(file, ui);

                if (token.simbolo == "sfecha_parenteses") {
                    token = getToken(file, ui);

                    if (token.simbolo == "sponto_virgula") {
                        token = getToken(file, ui);
                    } else {
                        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                        ": Erro Sintático -> Esperado \";\"."));
                    }
                } else {
                    ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                    ": Erro Sintático -> Esperado \")\" no final da expressão."));
                }
            } else {
                ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                ": Erro Sintático -> Identificador não declarado."));
            }
        } else {
            ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                            ": Erro Sintático -> Esperado um identificador."));
        }
    } else {
        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                        ": Erro Sintático -> Esperado \"(\" no começo da expressão."));
    }

    return token;
}

Node analyseWrite(FILE *file, Node token, Ui::MainWindow *ui) {
    token = getToken(file, ui);

    if (token.simbolo == "sabre_parenteses") {
        token = getToken(file, ui);

        if (token.simbolo == "sidentificador") {
            if (searchDeclaratedVariableOrFunctionTable(token.lexema)) {
                auto auxToken = symbolTable.searchSymbol(token.lexema);
                CodeSnippet *snippet = nullptr;
                if (auxToken->memoryAllocation != -1) {
                    snippet = new CodeSnippet("LDV", auxToken->memoryAllocation);
                }
                codeGen.insertNode(snippet);
                snippet = new CodeSnippet("PRN");
                codeGen.insertNode(snippet);
                token = getToken(file, ui);

                if (token.simbolo == "sfecha_parenteses") {
                    token = getToken(file, ui);

                    if (token.simbolo == "sponto_virgula") {
                        token = getToken(file, ui);
                    } else {
                        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                        ": Erro Sintático -> Esperado \";\"."));
                    }
                } else {
                    ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                    ": Erro Sintático -> Esperado \")\" no final da expressão."));
                }
            } else {
                ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                ": Erro Sintático -> Identificador não declarado."));
            }
        } else {
            ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                            ": Erro Sintático -> Esperado um identificador."));
        }
    } else {
        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                        ": Erro Sintático -> Esperado \"(\" no começo da expressão."));
    }

    return token;
}

Node analyseWhile(FILE *file, Node token, Ui::MainWindow *ui) {
    TokenExpression te;
    token = getToken(file, ui);
    te.token = token;

    std::list<std::string> postfix;

    te = analyseExpressions(file, te, ui);
    codeGen.insertNode(new CodeSnippet(++currentLabel, "NULL"));

    if (ui->ErrorArea->toPlainText().isEmpty()) {
        postfix = createInfixListFromExpression(te.expression);
        postfix = toPostfix(postfix);
        postfix = analysePostfix(postfix, ui);
        if (strcmpi(postfix.begin()->c_str(), "#B") != 0)
            ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                            ": Erro Semântico -> Expressão inválida."));
        postfix.clear();
    }

    if (te.token.simbolo == "sfaca") {
        codeGen.insertNode(new CodeSnippet("JMPF", ++currentLabel));
        te.token = getToken(file, ui);
        te.token = analyseSimpleCommands(file, te.token, ui);

        codeGen.insertNode(new CodeSnippet("JMP", currentLabel - 1));
        codeGen.insertNode(new CodeSnippet(currentLabel, "NULL"));
    } else {
        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                        ": Erro Sintático -> Esperado \"faca\" no começo da expressão."));
    }

    te.token = getToken(file, ui);

    return te.token;
}

Node analyseIf(FILE *file, Node token, Ui::MainWindow *ui) {
    TokenExpression te;
    token = getToken(file, ui);

    te.token = token;
    std::list<std::string> postfix;

    te = analyseExpressions(file, te, ui);

    if (ui->ErrorArea->toPlainText().isEmpty()) {
        postfix = createInfixListFromExpression(te.expression);
        postfix = toPostfix(postfix);
        postfix = analysePostfix(postfix, ui);
        if (strcmpi(postfix.begin()->c_str(), "#B") != 0)
            ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                            ": Erro Semântico -> Expressão inválida."));
        postfix.clear();
    }

    if (te.token.simbolo == "sentao") {
        codeGen.insertNode(new CodeSnippet("JMPF", ++currentLabel));
        te.token = getToken(file, ui);
        te.token = analyseSimpleCommands(file, te.token, ui);
        if (te.token.simbolo == "sponto_virgula") {
            te.token = getToken(file, ui);
        }

        if (te.token.simbolo == "ssenao") {
            codeGen.insertNode(new CodeSnippet("JMP", ++currentLabel));
            codeGen.insertNode(new CodeSnippet(currentLabel - 1, "NULL"));
            te.token = getToken(file, ui);
            te.token = analyseSimpleCommands(file, te.token, ui);
            if (te.token.simbolo == "sponto_virgula") {
                te.token = getToken(file, ui);
            }
            codeGen.insertNode(new CodeSnippet(currentLabel, "NULL"));
        } else {
            codeGen.insertNode(new CodeSnippet(currentLabel, "NULL"));
        }
    } else {
        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                        ": Erro Sintático -> Esperado \"entao\" no começo da expressão."));
    }

    return te.token;
}
