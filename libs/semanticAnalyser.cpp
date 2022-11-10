#include "semanticAnalyser.h"
#include <iostream>
#include <iterator>
#include <string.h>
#include <ctype.h>

int lineNo;

SymbolTable symbolTable;

bool searchDuplicatedVariableTable(Node token) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchLocalSymbol(token.lexema);

    if (nodeToken == nullptr) {
        return false;
    } else {
        return true;
    }
}

bool searchDeclaratedVariableTable(std::string lexema) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchSymbol(lexema);

    if (nodeToken == nullptr
        //        || nodeToken->type == "booleano"
        || nodeToken->type == "função inteiro"
        || nodeToken->type == "função booleano"
        || nodeToken->type == "procedimento") {
        return false;
    } else {
        return true;
    }
}

bool searchDeclaratedFunctionTable(std::string lexema) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchSymbol(lexema);

    if (nodeToken == nullptr
        || nodeToken->type == "inteiro"
        || nodeToken->type == "booleano"
        || nodeToken->type == "procedimento") {
        return false;
    } else {
        return true;
    }
}

bool searchDeclaratedVariableOrFunctionTable(std::string lexema) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchSymbol(lexema);

    if (nodeToken == nullptr
        || nodeToken->type == "função booleano"
        || nodeToken->type == "procedimento") {
        return false;
    } else {
        return true;
    }
}

bool searchDeclaratedProcedureTable(std::string lexema) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchSymbol(lexema);

    if (nodeToken == nullptr
        || nodeToken->type == "inteiro"
        || nodeToken->type == "booleano"
        || nodeToken->type == "função inteiro"
        || nodeToken->type == "função booleano"
            ) {
        return false;
    } else {
        return true;
    }
}

bool searchDuplicatedProcedureTable(Node token) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchLocalSymbol(token.lexema);

    if (nodeToken == nullptr
        || nodeToken->type == "função booleano"
            ) {
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
        if (infixExpression[i] >= 'a' && infixExpression[i] <= 'z' || infixExpression[i] >= 'A' &&
                                                                      infixExpression[i] <= 'Z' ||
            infixExpression[i] >= '0' && infixExpression[i] <= '9') {
            while (infixExpression[i] >= 'a' && infixExpression[i] <= 'z' || infixExpression[i] >= 'A' &&
                                                                             infixExpression[i] <= 'Z' ||
                   infixExpression[i] >= '0' && infixExpression[i] <= '9') {

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

bool isBooleanOperaiton(const char *op) {
    if (strcmp(op, ">") == 0 || strcmp(op, "<") == 0 ||
        strcmp(op, ">=") == 0 || strcmp(op, "<=") == 0 ||
        strcmp(op, "!=") == 0 || strcmp(op, "=") == 0 || strcmp(op, "e") == 0 ||
        strcmp(op, "ou") == 0) {
        return true;
    }
    return false;
}

std::list<std::string> analysePostfix(std::list<std::string> postfix) {
    SymbolNode *op1Symbol = nullptr, *op2Symbol = nullptr;

    for (auto &j: postfix) {
        std::cout << j.c_str();
    }
    std::cout << std::endl;

    for (auto i = postfix.begin(); i != postfix.end(); i++) {
        if (i != postfix.begin() && i != std::next(postfix.begin())) {
            if (isIntOperation(i->c_str()) || isBooleanOperaiton(i->c_str())) {
                auto op2 = std::prev(i);

                if (!isdigit(op2->c_str()[0]) && !isIntOperation(op2->c_str()) && !isBooleanOperaiton(op2->c_str()) &&
                    strcmp(op2->c_str(), "!I") != 0 && strcmp(op2->c_str(), "!B") != 0) {
                    if (symbolTable.searchSymbol(*op2) == nullptr) {
                        std::cout << i->c_str();
                        std::cout << op2->c_str();
                        std::cout << "teste1";
                        exit(1);
                    }

                    op2Symbol = symbolTable.searchSymbol(*op2);
                }

                auto op1 = std::prev(op2);

                if (!isdigit(op1->c_str()[0]) && !isIntOperation(op1->c_str()) && !isBooleanOperaiton(op1->c_str()) &&
                    strcmp(op1->c_str(), "!I") != 0 && strcmp(op1->c_str(), "!B") != 0) {
                    if (symbolTable.searchSymbol(*op1) == nullptr) {
                        std::cout << i->c_str();
                        std::cout << "teste2";
                        exit(1);
                    }

                    op1Symbol = symbolTable.searchSymbol(*op1);
                }

                if (op1Symbol == nullptr) {
                    if (op2Symbol == nullptr) {
                        if (isIntOperation(i->c_str())) {
                            if (std::isdigit(op1->c_str()[0]) && std::isdigit(op2->c_str()[0]) ||
                                strcmp(op1->c_str(), "!I") == 0 || strcmp(op2->c_str(), "!I") == 0) {
                                postfix.erase(op1);
                                postfix.erase(op2);
                                i->replace(i->begin(), i->end(), "!I");
                            } else {
                                exit(1);
                            }
                        }

                        if (isBooleanOperaiton(i->c_str())) {
                            if (std::isdigit(op1->c_str()[0]) && std::isdigit(op2->c_str()[0]) ||
                                strcmp(op1->c_str(), "!B") == 0 || strcmp(op2->c_str(), "!B") == 0) {
                                postfix.erase(op1);
                                postfix.erase(op2);
                                i->replace(i->begin(), i->end(), "!B");
                            } else {
                                exit(1);
                            }
                        }
                    } else {
                        if (isIntOperation(i->c_str())) {
                            if (op2Symbol->type == "inteiro" && std::isdigit(op1->c_str()[0]) ||
                                strcmp(op1->c_str(), "!I") == 0 || strcmp(op2->c_str(), "!I") == 0) {
                                postfix.erase(op1);
                                postfix.erase(op2);
                                i->replace(i->begin(), i->end(), "!I");
                            } else {
                                exit(1);
                            }
                        }

                        if (isBooleanOperaiton(i->c_str())) {
                            if (op2Symbol->type == "inteiro" && std::isdigit(op1->c_str()[0]) ||
                                strcmp(op1->c_str(), "!B") == 0 || strcmp(op2->c_str(), "!B") == 0) {
                                postfix.erase(op1);
                                postfix.erase(op2);
                                i->replace(i->begin(), i->end(), "!B");
                            } else {
                                exit(1);
                            }
                        }
                    }
                } else {
                    if (op2Symbol == nullptr) {
                        if (isIntOperation(i->c_str())) {
                            if (op1Symbol->type == "inteiro" && std::isdigit(op2->c_str()[0]) ||
                                strcmp(op1->c_str(), "!I") == 0 || strcmp(op2->c_str(), "!I") == 0) {
                                postfix.erase(op1);
                                postfix.erase(op2);
                                i->replace(i->begin(), i->end(), "!I");
                            } else {
                                exit(1);
                            }
                        }
                        if (isBooleanOperaiton(i->c_str())) {
                            if (op1Symbol->type == "inteiro" && std::isdigit(op2->c_str()[0]) ||
                                strcmp(op1->c_str(), "!B") == 0 || strcmp(op2->c_str(), "!B") == 0) {
                                postfix.erase(op1);
                                postfix.erase(op2);
                                i->replace(i->begin(), i->end(), "!B");
                            } else {
                                exit(1);
                            }
                        }
                    } else {
                        if (isIntOperation(i->c_str())) {
                            if (op1Symbol->type == op2Symbol->type) {
                                postfix.erase(op1);
                                postfix.erase(op2);
                                i->replace(i->begin(), i->end(), "!I");
                            } else {
                                exit(1);
                            }
                        }

                        if (isBooleanOperaiton(i->c_str())) {
                            if (op1Symbol->type == op2Symbol->type) {
                                postfix.erase(op1);
                                postfix.erase(op2);
                                i->replace(i->begin(), i->end(), "!B");
                            } else {
                                exit(1);
                            }
                        }
                    }
                }

                for (auto &j: postfix) {
                    std::cout << j.c_str();
                }
                std::cout << std::endl;
            }
        }
    }

    return postfix;
}
