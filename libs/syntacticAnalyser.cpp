#include <string>
#include "lexicalAnalyser.h"
#include "syntacticAnalyser.h"
#include "semanticAnalyser.h"

Node analyseType(FILE *file, Node token, std::queue<std::string> identifierQueue, std::queue<int> lineNumberQueue,
                 Ui::MainWindow *ui) {
    if (token.simbolo != "sinteiro" && token.simbolo != "sbooleano") {
        ui->errorArea->appendPlainText("This type is invalid, are you trying to say inteiro or booleano.");
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

Node analyseVariables(FILE *file, Node token, Ui::MainWindow *ui) {
    std::queue<std::string> identifierQueue;
    std::queue<int> lineNumberQueue;

    do {
        if (token.simbolo == "sidentificador") {
            if (!searchDuplicatedVariableTable(token.lexema)) {
                identifierQueue.push(token.lexema);
                lineNumberQueue.push(lineNo);
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
            ui->errorArea->appendPlainText(QString::fromStdString("Invalid expression, expected identificador."));
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
                            QString::fromStdString("Expected , at line ") + QString::number(lineNo) + ".");
                }
            }
        } else {
            ui->errorArea->appendPlainText(QString::fromStdString("Invalid expression, expected identificador."));
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
                    QString::fromStdString("Expected , at line ") + QString::number(lineNo) + ".");
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
        token = getToken(file);
        token = analyseSimpleCommands(file, token, ui);
        while (token.simbolo != "sfim") {
            if (token.simbolo == "sponto_virgula") {
                token = getToken(file);

                if (token.simbolo != "sfim") {
                    token = analyseSimpleCommands(file, token, ui);
                }
            } else {
                ui->errorArea->appendPlainText(
                        QString::fromStdString("Invalid expression, expected identificador.") +
                        QString::number(lineNo) + ".");
//                exit(1);
            }
        }
        token = getToken(file);
    } else {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Invalid expression, expected identificador."));
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
            symbolTable.downLayer(token.lexema, token.lexema, token.lexema, "procedimento", lineNo);
        }

        token = getToken(file);

        if (token.simbolo == "sponto_virgula") {
            token = analyseBlock(file, token, ui);
        } else {
            ui->errorArea->appendPlainText(
                    QString::fromStdString("Expected , at line ") +
                    QString::number(lineNo) + ".");
        }
    } else {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Invalid expression, expected identificador."));
    }

//    symbolTable.deleteLayer();

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
                symbolTable.insertSymbol(identifier, identifier, "funcao " + token.lexema, lineNo);

                token = getToken(file);

                if (token.simbolo == "sponto_virgula") {
                    token = analyseBlock(file, token, ui);
                } else {
                    ui->errorArea->appendPlainText(
                            QString::fromStdString("Expected , at line ") +
                            QString::number(lineNo) + ".");
                }
            } else {
                ui->errorArea->appendPlainText("This type is invalid, are you trying to say inteiro or booleano.");
            }
        }
    } else {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Invalid expression, expected identificador."));
    }

//    symbolTable.deleteLayer();

    return token;
}

TokenExpression analyseFunctionCall(FILE *file, TokenExpression te, Ui::MainWindow *ui) {
    if (!searchDeclaratedFunctionTable(te.token.lexema)) {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Function has not been declared in the code"));
//        exit(1);
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
    SymbolNode *currentNode = symbolTable.searchSymbol(te.token.lexema);
    if (te.token.simbolo == "sidentificador") {
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
//        expression += token.lexema + " ";
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

    while (te.token.simbolo == "smult" || te.token.simbolo == "sdiv" || te.token.simbolo == "sse") {
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
    token = getToken(file);

    if (!searchDeclaratedProcedureTable(token.lexema)) {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Procedure has not been declared in the code"));
//        exit(1);
    }

    token = getToken(file);

    return token;
}

Node analyseAttributionAndProcedureCall(FILE *file, Node token, Ui::MainWindow *ui) {
    token = getToken(file);

    if (token.simbolo == "satribuicao") {
        token = analyseAttribution(file, token, ui);
    } else {
        token = analyseProcedureCall(file, token, ui);
    }

    return token;
}

Node analyseRead(FILE *file, Node token, Ui::MainWindow *ui) {
    token = getToken(file);

    if (token.simbolo == "sabre_parenteses") {
        token = getToken(file);
        if (token.simbolo == "sidentificador") {
            if (searchDeclaratedVariableTable(token.lexema)) {
                token = getToken(file);
                if (token.simbolo == "sfecha_parenteses") {
                    token = getToken(file);
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
                    QString::fromStdString("Invalid expression, expected identificador."));
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
                token = getToken(file);
                if (token.simbolo == "sfecha_parenteses") {
                    token = getToken(file);
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
                    QString::fromStdString("Invalid expression, expected identificador."));
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


    if (te.token.simbolo == "sfaca") {
        te.token = getToken(file);
        te.token = analyseSimpleCommands(file, te.token, ui);
    } else {
        ui->errorArea->appendPlainText(
                QString::fromStdString("Expected faca at the start of the expression"));
    }

    postfix = createInfixListFromExpression(te.expression);
    postfix = toPostfix(postfix);
    analysePostfix(postfix, ui);

    return te.token;
}

Node analyseIf(FILE *file, Node token, Ui::MainWindow *ui) {
    TokenExpression te;
    token = getToken(file);
    te.token = token;
    std::list<std::string> postfix;

    te = analyseExpressions(file, te, ui);


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

    postfix = createInfixListFromExpression(te.expression);
    postfix = toPostfix(postfix);
    analysePostfix(postfix, ui);

    return te.token;
}
