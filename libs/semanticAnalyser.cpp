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

//    if (nodeToken != nullptr)
//        std::cout << nodeToken->identifier << " : " << nodeToken->scope << std::endl;

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

bool isNumber(const std::string &s) {
    for (char const &ch: s) {
        if (std::isdigit(ch) == 0)
            return false;
    }
    return true;
}

//bool isArithmeticOperator(const std::string &s) {
//    if (s == "+" || s == "-" || s == "*" || s == "div")
//        return true;
//    return false;
//}
//
//bool isRelationalOperator(const std::string &s) {
//    if (s == ">" || s == "<" || s == ">=" || s == "<=" || s == "=" || s == "!=")
//        return true;
//    return false;
//}
//
//bool isPositiveOrNegative(const std::string &s) {
//    if (s == "+u" || s == "-u")
//        return true;
//    return false;
//}
//
//bool isOpenParenthesis(const std::string &s) {
//    if (s == "(")
//        return true;
//    return false;
//}
//
//bool isCloseParenthesis(const std::string &s) {
//    if (s == ")")
//        return true;
//    return false;
//}

// - a + b
std::list<std::string> createInfixListFromExpression(std::string infixExpression) {
    int i = 0;
    std::list<std::string> resultList;
    std::string result;

    do {
        result = "";
        while (infixExpression[i] != ' ') {
            result += infixExpression[i];
            i++;
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
        if (searchDeclaratedVariableOrFunctionTable(expressionList.front()) ||
            isNumber(expressionList.front())) {
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

void analyseLogicOperator(const char *op) {
    if (strcmpi(op, "+") == 0) {
        codeGen.insertNode(new CodeSnippet("ADD"));
    } else if (strcmpi(op, "-") == 0) {
        codeGen.insertNode(new CodeSnippet("SUB"));
    } else if (strcmpi(op, "*") == 0) {
        codeGen.insertNode(new CodeSnippet("MULT"));
    } else if (strcmpi(op, "div") == 0) {
        codeGen.insertNode(new CodeSnippet("DIVI"));
    }
}

void analyseRelationalOperator(const char *op) {
    if (strcmpi(op, "e") == 0) {
        codeGen.insertNode(new CodeSnippet("AND"));
    } else if (strcmpi(op, "ou") == 0) {
        codeGen.insertNode(new CodeSnippet("OR"));
    } else if (strcmpi(op, "nao") == 0) {
        codeGen.insertNode(new CodeSnippet("NEG"));
    } else if (strcmpi(op, ">") == 0) {
        codeGen.insertNode(new CodeSnippet("CMA"));
    } else if (strcmpi(op, "<") == 0) {
        codeGen.insertNode(new CodeSnippet("CME"));
    } else if (strcmpi(op, ">=") == 0) {
        codeGen.insertNode(new CodeSnippet("CMAQ"));
    } else if (strcmpi(op, "<=") == 0) {
        codeGen.insertNode(new CodeSnippet("CMEQ"));
    } else if (strcmpi(op, "!=") == 0) {
        codeGen.insertNode(new CodeSnippet("CDIF"));
    } else if (strcmpi(op, "=") == 0) {
        codeGen.insertNode(new CodeSnippet("CEQ"));
    }
}

std::list<std::string> analysePostfix(std::list<std::string> postfix, Ui::MainWindow *ui) {
    return analysePostfix(std::move(postfix), 0, ui);
}

std::list<std::string> analysePostfix(std::list<std::string> postfix, int attribution, Ui::MainWindow *ui) {
    SymbolNode *op1Symbol = nullptr, *op2Symbol = nullptr;

    for (auto &j: postfix) {
        std::cout << j.c_str();
    }
    std::cout << std::endl;

    for (auto i = postfix.begin(); i != postfix.end(); i++) {
        if (i != postfix.begin() && postfix.size() != 1) {
            if (isIntOperation(i->c_str()) || isBooleanOperation(i->c_str())) {
                auto op2 = std::prev(i);

                if (!isNumber(*op2) && !isIntOperation(op2->c_str()) && !isBooleanOperation(op2->c_str()) &&
                    strcmpi(op2->c_str(), "#I") != 0 && strcmpi(op2->c_str(), "#B") != 0) {
                    if (symbolTable.searchSymbol(*op2) == nullptr) {
//                        exit(1);
                        ui->errorArea->appendPlainText("Invalid Expression.");
                    }

                    op2Symbol = symbolTable.searchSymbol(*op2);
                }

                auto op1 = std::prev(op2);

                if (!isNumber(*op1) && !isIntOperation(op1->c_str()) && !isBooleanOperation(op1->c_str()) &&
                    strcmpi(op1->c_str(), "#I") != 0 && strcmpi(op1->c_str(), "#B") != 0) {
                    if (symbolTable.searchSymbol(*op1) == nullptr) {
//                        exit(1);
//                        QApplication::exit();
                        ui->errorArea->appendPlainText("Invalid Expression.");
                    }

                    op1Symbol = symbolTable.searchSymbol(*op1);
                }

                if (op1Symbol == nullptr) {
                    if (op2Symbol == nullptr) {
                        if (isIntOperation(i->c_str())) {
                            if ((isNumber(*op1) || strcmpi(op1->c_str(), "#I") == 0) &&
                                (isNumber(*op2) ||
                                 strcmpi(op2->c_str(), "#I") == 0)) {
                                // op1 || op1Symbol && op2 || op2Symbol
                                // op1 i op2
                                //  a  *  b
                                // op1 é varivel ou constante
                                i->replace(i->begin(), i->end(), "#I");
                            } else {
//                                exit(1);
//                                QApplication::exit();
                                ui->errorArea->appendPlainText("Invalid Expression.");

                            }
                        }
                        if (isBooleanOperation(i->c_str())) {
                            if (((isNumber(*op1) || strcmpi(op1->c_str(), "#I") == 0) &&
                                 (isNumber(*op2) ||
                                  strcmpi(op2->c_str(), "#I") == 0)) ||
                                (strcmpi(op1->c_str(), "#B") == 0 || strcmpi(op1->c_str(), "verdadeiro") == 0 ||
                                 strcmpi(op1->c_str(), "falso") == 0 && strcmpi(op2->c_str(), "#B") == 0 ||
                                 strcmpi(op2->c_str(), "verdadeiro") == 0 ||
                                 strcmpi(op2->c_str(), "falso") == 0)) {

                                i->replace(i->begin(), i->end(), "#B");
                            } else {
//                                exit(1);
//                                QApplication::exit();
                                ui->errorArea->appendPlainText("Invalid Expression.");

                            }
                        }
                    } else {
                        if (isIntOperation(i->c_str())) {
                            if (op2Symbol->type == "inteiro" ||
                                op2Symbol->type == "funcao inteiro" && isNumber(*op1) ||
                                strcmpi(op1->c_str(), "#I") == 0) {

                                i->replace(i->begin(), i->end(), "#I");
                            } else {
//                                exit(1);
//                                QApplication::exit();
                                ui->errorArea->appendPlainText("Invalid Expression.");
                            }
                        }

                        if (isBooleanOperation(i->c_str())) {
                            if ((op2Symbol->type == "inteiro" ||
                                 op2Symbol->type == "funcao inteiro" && isNumber(*op1)) ||
                                (op2Symbol->type == "booleano" ||
                                 op2Symbol->type == "funcao booleano" ||
                                 strcmpi(op1->c_str(), "#B") == 0 || strcmpi(op1->c_str(), "verdadeiro") == 0 ||
                                 strcmpi(op1->c_str(), "falso") == 0)) {
                                i->replace(i->begin(), i->end(), "#B");
                            } else {
//                                exit(1);
//                                QApplication::exit();
                                ui->errorArea->appendPlainText("Invalid Expression.");

                            }
                        }
                    }
                } else {
                    if (op2Symbol == nullptr) {
                        if (isIntOperation(i->c_str())) {
                            if (op1Symbol->type == "inteiro" ||
                                op1Symbol->type == "funcao inteiro" && isNumber(*op2) ||
                                strcmpi(op2->c_str(), "#I") == 0) {

                                i->replace(i->begin(), i->end(), "#I");
                            } else {
//                                exit(1);
//                                QApplication::exit();
                                ui->errorArea->appendPlainText("Invalid Expression.");

                            }
                        }
                        if (isBooleanOperation(i->c_str())) {
                            if ((op1Symbol->type == "inteiro" ||
                                 op1Symbol->type == "funcao inteiro" && isNumber(*op2)) ||
                                (op1Symbol->type == "booleano" ||
                                 op1Symbol->type == "funcao booleano" &&
                                 strcmpi(op2->c_str(), "#B") == 0) || strcmpi(op2->c_str(), "verdadeiro") == 0 ||
                                strcmpi(op2->c_str(), "falso") == 0) {

                                i->replace(i->begin(), i->end(), "#B");
                            } else {
//                                exit(1);
//                                QApplication::exit();
                                ui->errorArea->appendPlainText("Invalid Expression.");

                            }
                        }
                    } else {
                        if (isIntOperation(i->c_str())) {
                            if (op1Symbol->type == "inteiro" ||
                                op1Symbol->type == "funcao inteiro" && op2Symbol->type == "inteiro" ||
                                op2Symbol->type == "funcao inteiro") {

                                i->replace(i->begin(), i->end(), "#I");
                            } else {
//                                exit(1);
//                                QApplication::exit();
                                ui->errorArea->appendPlainText("Invalid Expression.");

                            }
                        }

                        if (isBooleanOperation(i->c_str())) {
                            if ((op1Symbol->type == "inteiro" ||
                                 op1Symbol->type == "funcao inteiro" && op2Symbol->type == "inteiro" ||
                                 op2Symbol->type == "funcao inteiro") ||
                                (op1Symbol->type == "booleano" ||
                                 op1Symbol->type == "funcao booleano" && op2Symbol->type == "booleano" ||
                                 op2Symbol->type == "funcao booleano")) {

                                i->replace(i->begin(), i->end(), "#B");
                            } else {
//                                exit(1);
//                                QApplication::exit();
                                ui->errorArea->appendPlainText("Invalid Expression.");

                            }
                        }
                    }
                }

                op1Symbol = nullptr;
                op2Symbol = nullptr;

                postfix.erase(op1);
                postfix.erase(op2);

                for (auto &j: postfix) {
                    std::cout << j.c_str();
                }
                std::cout << std::endl;
            }
        } else if (i == postfix.begin() && postfix.size() == 1) {
            if (isNumber(*i)) {
                codeGen.insertNode(new CodeSnippet("LDC", *i));
                codeGen.insertNode(new CodeSnippet("STR", attribution));
                i->replace(i->begin(), i->end(), "#I");
            } else if (strcmpi(i->c_str(), "verdadeiro") == 0 ||
                       strcmpi(i->c_str(), "falso") == 0) {
                if (strcmpi(i->c_str(), "falso") == 0) {
                    codeGen.insertNode(new CodeSnippet("LDC", 0));
                } else if (strcmpi(i->c_str(), "verdadeiro") == 0) {
                    codeGen.insertNode(new CodeSnippet("LDC", 1));
                }

                codeGen.insertNode(new CodeSnippet("STR", attribution));

                i->replace(i->begin(), i->end(), "#B");
            } else {
                op1Symbol = symbolTable.searchSymbol(*i);
                if (op1Symbol->memoryAllocation != -1) {
                    codeGen.insertNode(new CodeSnippet("LDV", op1Symbol->memoryAllocation));
                } else {
                    codeGen.insertNode(new CodeSnippet("LDV", 0));
                }
                if (op1Symbol->type == "inteiro" || op1Symbol->type == "funcao inteiro")
                    i->replace(i->begin(), i->end(), "#I");
                if (op1Symbol->type == "booleano" || op1Symbol->type == "funcao booleano")
                    i->replace(i->begin(), i->end(), "#B");
                codeGen.insertNode(new CodeSnippet("STR", attribution));
            }

            for (auto &j: postfix) {
                std::cout << j.c_str();
            }
            std::cout << std::endl;
        }
    }

//    if (postfix.size() != 1) {
//        for (auto i = postfix.begin(); i != postfix.end(); i++) {
//            if (strcmpi(i->c_str(), "nao") == 0) {
//                auto op1 = std::next(i);
//
//                if (strcmpi(op1->c_str(), "#B") == 0 || strcmpi(op1->c_str(), "verdadeiro") == 0 ||
//                    strcmpi(op1->c_str(), "falso") == 0) {
//                    analyseRelationalOperator(i->c_str());
//
//                    i->replace(i->begin(), i->end(), "#B");
//                    postfix.erase(op1);
//                }
//            }
//        }
//
//        for (auto &j: postfix) {
//            std::cout << j.c_str();
//        }
//        std::cout << std::endl;
//    }

    return postfix;
}
