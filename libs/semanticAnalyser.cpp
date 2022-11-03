#include "semanticAnalyser.h"
#include "stack"

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

                if (infixExpression[i] == 'd') {
                    aux1 = result;
                    aux2 += infixExpression[i];
                    result += infixExpression[i];
                    i++;

                    if (infixExpression[i] == 'i') {
                        aux2 += infixExpression[i];
                        result += infixExpression[i];
                        i++;
                        if (infixExpression[i] == 'v') {
                            aux2 += infixExpression[i];
                            resultList.push_front(aux1);
                            resultList.push_front(aux2);
                            result = "";
                            aux1 = aux2 = "";
                            i++;
                        } else {
                            aux2 = aux1 = "";
                        }
                    } else {
                        aux2 = aux1 = "";
                    }
                } else {
                    result += infixExpression[i];
                    i++;
                }
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
    if (op == ">" || op == "<" || op == ">=" || op == "<=" || op == "!=" || op == "==")
        return 1;
    else if (op == "+" || op == "-")
        return 2;
    else if (op == "*" || op == "div")
        return 3;
    else
        return 0;
}

std::string toPostfix(std::list<std::string> expressionList) {
    std::string postfix;
    std::stack<std::string> s;

    while (!expressionList.empty()) {
        if (expressionList.front() >= "a" && expressionList.front() <= "z" ||
            expressionList.front() >= "A" && expressionList.front() <= "Z" ||
            expressionList.front() >= "0" && expressionList.front() <= "9") {
            postfix += expressionList.front();
            expressionList.pop_front();
        } else if (expressionList.front() == "(") {
            s.push(expressionList.front());
            expressionList.pop_front();
        } else if (expressionList.front() == ")") {
            while (s.top() != "(") {
                postfix += s.top();
                s.pop();
            }
            s.pop();
            expressionList.pop_front();
        } else {
            while (!s.empty() && precedence(expressionList.front()) <= precedence(s.top())) {
                postfix += s.top();
                s.pop();
            }

            s.push(expressionList.front());
            expressionList.pop_front();
        }
    }

    while (!s.empty()) {
        postfix += s.top();
        s.pop();
    }

    return postfix;
}
