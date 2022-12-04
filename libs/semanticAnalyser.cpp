#include "semanticAnalyser.h"
#include "mainwindow.h"
#include <iostream>
#include <utility>
#include <cstring>
#include <cctype>

int lineNo;

SymbolTable symbolTable;


bool searchDuplicatedVariableTable(std::string lexema) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchLocalSymbol(std::move(lexema));

    if (nodeToken == nullptr) {
        return false;
    } else {
        return true;
    }
}

bool searchDeclaratedVariableTable(std::string lexema) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchSymbol(std::move(lexema));

    if (nodeToken == nullptr) {
        return false;
    } else {
        if (nodeToken->type != "funcao inteiro"
            && nodeToken->type != "funcao booleano"
            && nodeToken->type != "procedimento"
            && nodeToken->type != "booleano"
                ) {
            return true;
        } else {
            return false;
        }
    }

}

bool searchDeclaratedFunctionTable(std::string lexema) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchSymbol(std::move(lexema));

    if (nodeToken == nullptr) {
        return false;
    } else {
        if (nodeToken->type != "inteiro"
            && nodeToken->type != "booleano"
            && nodeToken->type != "procedimento") {
            return true;
        } else {
            return false;
        }
    }

}

bool searchDeclaratedVariableOrFunctionTable(std::string lexema) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchSymbol(std::move(lexema));

    if (nodeToken == nullptr) {
        return false;
    } else {
        if (nodeToken->type != "funcao booleano"
            && nodeToken->type != "booleano"
            && nodeToken->type != "procedimento") {
            return true;
        } else {
            return false;
        }
    }
}

bool searchDeclaratedProcedureTable(std::string lexema) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchSymbol(std::move(lexema));

    if (nodeToken == nullptr) {
        return false;
    } else {
        if (nodeToken->type != "inteiro"
            && nodeToken->type != "booleano"
            && nodeToken->type != "funcao inteiro"
            && nodeToken->type != "funcao booleano") {
            return true;
        } else {
            return false;
        }
    }

}

bool searchDuplicatedProcedureTable(std::string lexema) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchSymbol(std::move(lexema));

    if (nodeToken == nullptr) {
        return false;
    } else {
        return true;
    }
}


bool searchVariableAndFunctionTable(std::string lexema) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchSymbol(std::move(lexema));

    if (nodeToken == nullptr) {
        return false;
    } else {
        if (nodeToken->type != "procedimento") {
            return true;
        } else {
            return false;
        }
    }
}

bool isNumber(const std::string &s) {
    return std::ranges::all_of(s, [](char ch) {
        return std::isdigit(ch) != 0;
    });
}

std::list<std::string> createInfixListFromExpression(std::string infixExpression) {
    int i = 0;
    std::list<std::string> resultList;
    std::string result;

    do {
        result = "";
        while (infixExpression[i] != ' ') {
            result += infixExpression[i++];
        }
        resultList.push_back(result);
        i++;
    } while (i != infixExpression.length());

    resultList.remove("");

    return resultList;
}

int precedence(const std::string &op) {
    if (op == "nao")
        return 1;
    else if (op == "e" || op == "ou")
        return 2;
    else if (op == ">" || op == "<" || op == ">=" || op == "<=" || op == "!=" || op == "=")
        return 3;
    else if (op == "+" || op == "-")
        return 4;
    else if (op == "*" || op == "div")
        return 5;
    else if (op == "+u" || op == "-u")
        return 6;

    return 0;
}

std::list<std::string> toPostfix(std::list<std::string> expressionList) {
    std::list<std::string> result;
    std::stack<std::string> s;

    while (!expressionList.empty()) {
        if (searchVariableAndFunctionTable(expressionList.front()) || isNumber(expressionList.front()) ||
            expressionList.front() == "verdadeiro" || expressionList.front() == "falso") {
            result.push_back(expressionList.front());
            expressionList.pop_front();
        } else if (expressionList.front() == "(") {
            s.push(expressionList.front());
            expressionList.pop_front();
        } else if (expressionList.front() == ")") {
            while (s.top() != "(") {
                result.push_back(s.top());
                s.pop();
            }
            s.pop();
            expressionList.pop_front();
        } else {
            while (!s.empty() && precedence(expressionList.front()) <= precedence(s.top())) {
                result.push_back(s.top());
                s.pop();
            }

            s.push(expressionList.front());
            expressionList.pop_front();
        }
    }


    while (!s.empty()) {
        result.push_back(s.top());
        s.pop();
    }

    return result;
}

bool isIntOperation(const char *op) {
    if (strcmpi(op, "*") == 0 || strcmpi(op, "div") == 0 || strcmpi(op, "+") == 0 ||
        strcmpi(op, "-") == 0)
        return true;
    return false;
}

bool isBooleanOperation(const char *op) {
    if (strcmpi(op, ">") == 0 || strcmpi(op, "<") == 0 ||
        strcmpi(op, ">=") == 0 || strcmpi(op, "<=") == 0 ||
        strcmpi(op, "!=") == 0 || strcmpi(op, "=") == 0 || strcmpi(op, "e") == 0 ||
        strcmpi(op, "ou") == 0) {
        return true;
    }
    return false;
}

bool isUnaryOperator(const char *op) {
    if (strcmpi(op, "+u") == 0 || strcmpi(op, "-u") == 0)
        return true;
    return false;
}

bool isNegateOperator(const char *op) {
    if (strcmpi(op, "nao") == 0)
        return true;
    return false;
}

bool analyseLogicOperator(const char *op) {
    if (strcmpi(op, "+") == 0) {
        codeGen.insertNode(new CodeSnippet("ADD"));
        return true;
    } else if (strcmpi(op, "-") == 0) {
        codeGen.insertNode(new CodeSnippet("SUB"));
    } else if (strcmpi(op, "*") == 0) {
        codeGen.insertNode(new CodeSnippet("MULT"));
        return true;
    } else if (strcmpi(op, "div") == 0) {
        codeGen.insertNode(new CodeSnippet("DIVI"));
        return true;
    }

    return false;
}

bool analyseRelationalOperator(const char *op) {
    if (strcmpi(op, "e") == 0) {
        codeGen.insertNode(new CodeSnippet("AND"));
        return true;
    } else if (strcmpi(op, "ou") == 0) {
        codeGen.insertNode(new CodeSnippet("OR"));
        return true;
    } else if (strcmpi(op, "nao") == 0) {
        codeGen.insertNode(new CodeSnippet("NEG"));
        return true;
    } else if (strcmpi(op, ">") == 0) {
        codeGen.insertNode(new CodeSnippet("CMA"));
        return true;
    } else if (strcmpi(op, "<") == 0) {
        codeGen.insertNode(new CodeSnippet("CME"));
        return true;
    } else if (strcmpi(op, ">=") == 0) {
        codeGen.insertNode(new CodeSnippet("CMAQ"));
        return true;
    } else if (strcmpi(op, "<=") == 0) {
        codeGen.insertNode(new CodeSnippet("CMEQ"));
        return true;
    } else if (strcmpi(op, "!=") == 0) {
        codeGen.insertNode(new CodeSnippet("CDIF"));
        return true;
    } else if (strcmpi(op, "=") == 0) {
        codeGen.insertNode(new CodeSnippet("CEQ"));
        return true;
    } else if (strcmpi(op, "-u") == 0) {
        codeGen.insertNode(new CodeSnippet("INV"));
        return true;
    } else if (strcmpi(op, "+u") == 0) {
        return true;
    }
    return false;
}

std::list<std::string>
analysePostfix(std::list<std::string> postfix, Ui::MainWindow *ui) {
    return analysePostfix(std::move(postfix), -2, ui);
}

std::list<std::string>
analysePostfix(std::list<std::string> postfix, int attribution, Ui::MainWindow *ui) {
    SymbolNode *op1Symbol = nullptr, *op2Symbol = nullptr;

    bool hasInv = false;
    for (auto &j: postfix) {
        if (analyseRelationalOperator(j.c_str())) {

        } else if (analyseLogicOperator(j.c_str())) {

        } else if (isNumber(j)) {
            codeGen.insertNode(new CodeSnippet("LDC", j));
        } else if (symbolTable.searchSymbol(j) != nullptr) {
            auto node = symbolTable.searchSymbol(j);
            if (node->memoryAllocation > 0) {
                codeGen.insertNode(new CodeSnippet("LDV", node->memoryAllocation));
            } else {
                codeGen.insertNode(new CodeSnippet("CALL", node->labelStart));
                codeGen.insertNode(new CodeSnippet("LDV", 0));
            }
        } else if (strcmpi(j.c_str(), "verdadeiro") == 0) {
            codeGen.insertNode(new CodeSnippet("LDC", 1));
        } else if (strcmpi(j.c_str(), "falso") == 0) {
            codeGen.insertNode(new CodeSnippet("LDC", 0));
        } else {
            break;
        }
    }

    for (auto &j: postfix) {
        std::cout << j.c_str();
    }
    std::cout << std::endl;

    for (auto i = postfix.begin(); i != postfix.end(); i++) {
        if (i != postfix.begin() && postfix.size() != 1) {
            if (isIntOperation(i->c_str()) || isBooleanOperation(i->c_str())) {
                auto op2 = std::prev(i);

                if (!isNumber(*op2) && !isIntOperation(op2->c_str()) && !isBooleanOperation(op2->c_str()) &&
                    strcmpi(op2->c_str(), "#I") != 0 && strcmpi(op2->c_str(), "#B") != 0 &&
                    strcmpi(op2->c_str(), "verdadeiro") != 0 && strcmpi(op2->c_str(), "falso") != 0) {
                    if (symbolTable.searchSymbol(*op2) == nullptr) {
//                        exit(1);
                        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                        ": Erro Semântico -> Expressão inválida."));
                    }

                    op2Symbol = symbolTable.searchSymbol(*op2);
                }

                if (op2 == postfix.begin())
                    break;

                auto op1 = std::prev(op2);


                if (!isNumber(*op1) && !isIntOperation(op1->c_str()) && !isBooleanOperation(op1->c_str()) &&
                    strcmpi(op1->c_str(), "#I") != 0 && strcmpi(op1->c_str(), "#B") != 0
                    && strcmpi(op1->c_str(), "verdadeiro") != 0 && strcmpi(op1->c_str(), "falso") != 0) {
                    if (symbolTable.searchSymbol(*op1) == nullptr) {
                        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                        ": Erro Semântico -> Expressão inválida."));
                    }
                    op1Symbol = symbolTable.searchSymbol(*op1);
                }

                if (op1Symbol == nullptr) {
                    if (op2Symbol == nullptr) {
                        if (isIntOperation(i->c_str())) {
                            if ((isNumber(*op1) || strcmpi(op1->c_str(), "#I") == 0) &&
                                (isNumber(*op2) || strcmpi(op2->c_str(), "#I") == 0)) {
                                i->replace(i->begin(), i->end(), "#I");
                            } else {
                                ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                                ": Erro Semântico -> Expressão inválida."));
                            }
                        }
                        if (isBooleanOperation(i->c_str())) {
                            if (((isNumber(*op1) || strcmpi(op1->c_str(), "#I") == 0) &&
                                 (isNumber(*op2) || strcmpi(op2->c_str(), "#I") == 0))
                                || ((strcmpi(op1->c_str(), "#B") == 0
                                     || strcmpi(op1->c_str(), "verdadeiro") == 0
                                     || strcmpi(op1->c_str(), "falso") == 0)
                                    && (strcmpi(op2->c_str(), "#B") == 0 ||
                                        strcmpi(op2->c_str(), "verdadeiro") == 0 ||
                                        strcmpi(op2->c_str(), "falso") == 0))) {
                                i->replace(i->begin(), i->end(), "#B");
                            } else {
                                ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                                ": Erro Semântico -> Expressão inválida."));
                            }
                        }
                    } else {
                        if (isIntOperation(i->c_str())) {
                            if ((op2Symbol->type == "inteiro" || op2Symbol->type == "funcao inteiro")
                                && ((isNumber(*op1)) || strcmpi(op1->c_str(), "#I") == 0)) {

                                i->replace(i->begin(), i->end(), "#I");
                            } else {
                                ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                                ": Erro Semântico -> Expressão inválida."));
                            }
                        }

                        if (isBooleanOperation(i->c_str())) {
                            if ((op2Symbol->type == "inteiro" || op2Symbol->type == "funcao inteiro")
                                && ((isNumber(*op1)) || strcmpi(op1->c_str(), "#I") == 0)
                                || ((op2Symbol->type == "booleano" || op2Symbol->type == "funcao booleano")
                                    && (strcmpi(op1->c_str(), "#B") == 0
                                        || strcmpi(op1->c_str(), "verdadeiro") == 0
                                        || strcmpi(op1->c_str(), "falso") == 0))) {
                                i->replace(i->begin(), i->end(), "#B");
                            } else {
                                ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                                ": Erro Semântico -> Expressão inválida."));
                            }
                        }
                    }
                } else {
                    if (op2Symbol == nullptr) {
                        if (isIntOperation(i->c_str())) {
                            if ((op1Symbol->type == "inteiro" || op1Symbol->type == "funcao inteiro")
                                && (isNumber(*op2) || strcmpi(op2->c_str(), "#I") == 0)) {
                                i->replace(i->begin(), i->end(), "#I");
                            } else {
                                ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                                ": Erro Semântico -> Expressão inválida."));
                            }
                        }
                        if (isBooleanOperation(i->c_str())) {
                            if (((op1Symbol->type == "inteiro" || op1Symbol->type == "funcao inteiro")
                                 && (isNumber(*op2) || strcmpi(op2->c_str(), "#I") == 0))
                                || ((op1Symbol->type == "booleano" ||
                                     op1Symbol->type == "funcao booleano")
                                    && (strcmpi(op2->c_str(), "#B") == 0
                                        || strcmpi(op2->c_str(), "verdadeiro") == 0
                                        || strcmpi(op2->c_str(), "falso") == 0))) {
                                i->replace(i->begin(), i->end(), "#B");
                            } else {
                                ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                                ": Erro Semântico -> Expressão inválida."));
                            }
                        }
                    } else {
                        if (isIntOperation(i->c_str())) {
                            if ((op1Symbol->type == "inteiro" || op1Symbol->type == "funcao inteiro")
                                && (op2Symbol->type == "inteiro" || op2Symbol->type == "funcao inteiro")) {
                                i->replace(i->begin(), i->end(), "#I");
                            } else {
                                ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                                ": Erro Semântico -> Expressão inválida."));
                            }
                        }

                        if (isBooleanOperation(i->c_str())) {
                            if (((op1Symbol->type == "inteiro" || op1Symbol->type == "funcao inteiro")
                                 && (op2Symbol->type == "inteiro" || op2Symbol->type == "funcao inteiro"))
                                || ((op1Symbol->type == "booleano" || op1Symbol->type == "funcao booleano")
                                    && (op2Symbol->type == "booleano" || op2Symbol->type == "funcao booleano"))) {
                                i->replace(i->begin(), i->end(), "#B");
                            } else {
                                ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                                ": Erro Semântico -> Expressão inválida."));
                            }
                        }
                    }
                }

                op1Symbol = nullptr;
                op2Symbol = nullptr;

                postfix.erase(op1);
                postfix.erase(op2);
            } else if (isUnaryOperator(i->c_str())) {
                auto op1 = std::prev(i);
                op1Symbol = symbolTable.searchSymbol(*op1);

                if (op1Symbol != nullptr) {
                    if (op1Symbol->type == "inteiro" || op1Symbol->type == "funcao inteiro"
                        || strcmpi(op1->c_str(), "#I") == 0)
                        i->replace(i->begin(), i->end(), "#I");
                    else {
                        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                        ": Erro Semântico -> Expressão inválida."));
                    }
                } else if (isNumber(*op1)) {
                    i->replace(i->begin(), i->end(), "#I");
                } else {
                    ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                    ": Erro Semântico -> Expressão inválida."));
                }

                postfix.erase(op1);
            } else if (isNegateOperator(i->c_str())) {
                auto op1 = std::prev(i);
                op1Symbol = symbolTable.searchSymbol(*op1);

                if (op1Symbol != nullptr) {
                    if (op1Symbol->type == "booleano" || op1Symbol->type == "funcao booleano")
                        i->replace(i->begin(), i->end(), "#B");
                    else {
                        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                        ": Erro Semântico -> Expressão inválida."));
                    }
                } else if (strcmpi(op1->c_str(), "verdadeiro") == 0 || strcmpi(op1->c_str(), "falso") == 0
                           || strcmpi(op1->c_str(), "#B") == 0) {
                    i->replace(i->begin(), i->end(), "#B");
                } else {
                    ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                                    ": Erro Semântico -> Expressão inválida."));
                }

                postfix.erase(op1);

                for (auto &j: postfix) {
                    std::cout << j.c_str();
                }
                std::cout << std::endl;
            }
        } else if (i == postfix.begin() && postfix.size() == 1) {
            if (isNumber(*i)) {
                i->replace(i->begin(), i->end(), "#I");
            } else if (strcmpi(i->c_str(), "verdadeiro") == 0 ||
                       strcmpi(i->c_str(), "falso") == 0) {
                i->replace(i->begin(), i->end(), "#B");
            } else {
                op1Symbol = symbolTable.searchSymbol(*i);
                if (op1Symbol->type == "inteiro" || op1Symbol->type == "funcao inteiro")
                    i->replace(i->begin(), i->end(), "#I");
                if (op1Symbol->type == "booleano" || op1Symbol->type == "funcao booleano")
                    i->replace(i->begin(), i->end(), "#B");
            }
        }
    }
    if (attribution != -2) {
        if (attribution != -1) {
            codeGen.insertNode(new CodeSnippet("STR", attribution));
        } else {
            codeGen.insertNode(new CodeSnippet("STR", 0));
        }
    }

    for (auto &j: postfix) {
        std::cout << j.c_str();
    }
    std::cout << std::endl;

    std::cout << postfix.size() << std::endl;
    if (postfix.size() != 1)
        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                        ": Erro Semântico -> Expressão inválida."));

    return postfix;
}