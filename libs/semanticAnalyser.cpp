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

std::list<std::string> createInfixListFromExpression(std::string infixExpression) {
    int i = 0;
    std::list<std::string> resultList;
    std::string result;
    std::string aux1, aux2;

    do {
        if (infixExpression[i] >= 65 && infixExpression[i] <= 90 || infixExpression[i] >= 97 &&
                                                                    infixExpression[i] <= 122 ||
            infixExpression[i] >= 48 && infixExpression[i] <= 57) {
            while (infixExpression[i] >= 65 && infixExpression[i] <= 90 || infixExpression[i] >= 97 &&
                                                                           infixExpression[i] <= 122 ||
                   infixExpression[i] >= 48 && infixExpression[i] <= 57) {

                result += infixExpression[i];
                i++;

            }

            resultList.push_front(result);
            result = "";
        }

        if (infixExpression[i] == '(') {
            result += infixExpression[i];
            resultList.push_front(result);
            result = "";
            i++;
        }

        if (infixExpression[i] == ')') {
            result += infixExpression[i];
            resultList.push_front(result);
            result = "";
            i++;
        }

        if (infixExpression[i] == '+' || infixExpression[i] == '-' || infixExpression[i] == '*') {
            result += infixExpression[i];
            resultList.push_front(result);
            result = "";
            i++;
        } else {
            if (infixExpression[i] == '<' || infixExpression[i] == '>' || infixExpression[i] == '!') {
                result += infixExpression[i];
                i++;
            }

            if (infixExpression[i] == '=') {
                result += infixExpression[i];
                i++;

                if (infixExpression[i] == '=') {
                    result += infixExpression[i];
                    i++;
                }
            }
            resultList.push_front(result);
            result = "";
        }

        while (infixExpression[i] == ' ') {
            i++;
        }
    } while (infixExpression[i] != '\0');

    resultList.reverse();
    resultList.remove("");

    return resultList;
}

int precedence(const std::string &op) {
    if (op == ">" || op == "<" || op == ">=" || op == "<=" || op == "!=" || op == "=")
        return 1;
    else if (op == "+" || op == "-")
        return 2;
    else if (op == "*" || op == "div")
        return 3;
    else
        return 0;
}

std::list<std::string> toPostfix(std::list<std::string> expressionList) {
    std::list<std::string> result;
    std::stack<std::string> s;

    while (!expressionList.empty()) {
        if (expressionList.front() >= "a" && expressionList.front() <= "z" && expressionList.front() != "e" ||
            expressionList.front() >= "A" && expressionList.front() <= "Z" ||
            expressionList.front() >= "0" && expressionList.front() <= "9") {
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
    if (strcmp(op, "*") == 0 || strcmp(op, "div") == 0 || strcmp(op, "+") == 0 ||
        strcmp(op, "-") == 0)
        return true;
    return false;
}

bool isBooleanOperation(const char *op) {
    if (strcmp(op, ">") == 0 || strcmp(op, "<") == 0 ||
        strcmp(op, ">=") == 0 || strcmp(op, "<=") == 0 ||
        strcmp(op, "!=") == 0 || strcmp(op, "=") == 0 || strcmp(op, "e") == 0 ||
        strcmp(op, "ou") == 0) {
        return true;
    }
    return false;
}

bool isNumber(const std::string &s) {
    for (char const &ch: s) {
        if (std::isdigit(ch) == 0)
            return false;
    }
    return true;
}

std::list<std::string> analysePostfix(std::list<std::string> postfix, Ui::MainWindow *ui) {
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
                    strcmp(op2->c_str(), "!I") != 0 && strcmp(op2->c_str(), "!B") != 0) {
                    if (symbolTable.searchSymbol(*op2) == nullptr) {
//                        exit(1);
                        ui->errorArea->appendPlainText("Invalid Expression.");
                    }

                    op2Symbol = symbolTable.searchSymbol(*op2);
                }

                auto op1 = std::prev(op2);

                if (!isNumber(*op1) && !isIntOperation(op1->c_str()) && !isBooleanOperation(op1->c_str()) &&
                    strcmp(op1->c_str(), "!I") != 0 && strcmp(op1->c_str(), "!B") != 0) {
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
                            if (isNumber(*op1) && isNumber(*op2) ||
                                strcmp(op1->c_str(), "!I") == 0 || strcmp(op2->c_str(), "!I") == 0) {
                                i->replace(i->begin(), i->end(), "!I");
                            } else {
//                                exit(1);
//                                QApplication::exit();
                                ui->errorArea->appendPlainText("Invalid Expression.");

                            }
                        }

                        if (isBooleanOperation(i->c_str())) {
                            if ((isNumber(*op1) && isNumber(*op2)) ||
                                (strcmp(op1->c_str(), "!B") == 0 || strcmp(op1->c_str(), "verdadeiro") == 0 ||
                                 strcmp(op1->c_str(), "falso") == 0 && strcmp(op2->c_str(), "!B") == 0 ||
                                 strcmp(op2->c_str(), "verdadeiro") == 0 ||
                                 strcmp(op2->c_str(), "falso") == 0)) {
                                i->replace(i->begin(), i->end(), "!B");
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
                                strcmp(op1->c_str(), "!I") == 0) {
                                i->replace(i->begin(), i->end(), "!I");
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
                                 strcmp(op1->c_str(), "!B") == 0 || strcmp(op1->c_str(), "verdadeiro") == 0 ||
                                 strcmp(op1->c_str(), "falso") == 0)) {
                                i->replace(i->begin(), i->end(), "!B");
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
                                strcmp(op2->c_str(), "!I") == 0) {
                                i->replace(i->begin(), i->end(), "!I");
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
                                 strcmp(op2->c_str(), "!B") == 0) || strcmp(op2->c_str(), "verdadeiro") == 0 ||
                                strcmp(op2->c_str(), "falso") == 0) {
                                i->replace(i->begin(), i->end(), "!B");
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
                                i->replace(i->begin(), i->end(), "!I");
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
                                i->replace(i->begin(), i->end(), "!B");
                            } else {
//                                exit(1);
//                                QApplication::exit();
                                ui->errorArea->appendPlainText("Invalid Expression.");

                            }
                        }
                    }
                }

                postfix.erase(op1);
                postfix.erase(op2);

                for (auto &j: postfix) {
                    std::cout << j.c_str();
                }
                std::cout << std::endl;
            }
        } else if (i == postfix.begin() && postfix.size() == 1) {
            if (isNumber(*i)) {
                i->replace(i->begin(), i->end(), "!I");
            } else if (strcmp(i->c_str(), "verdadeiro") == 0 ||
                       strcmp(i->c_str(), "falso") == 0) {
                i->replace(i->begin(), i->end(), "!B");
            } else {
                op1Symbol = symbolTable.searchSymbol(*i);
                if (op1Symbol->type == "inteiro" || op1Symbol->type == "funcao inteiro")
                    i->replace(i->begin(), i->end(), "!I");
                if (op1Symbol->type == "booleano" || op1Symbol->type == "funcao booleano")
                    i->replace(i->begin(), i->end(), "!B");
            }

            for (auto &j: postfix) {
                std::cout << j.c_str();
            }
            std::cout << std::endl;
        }
    }

    return postfix;
}
