#include <stack>
#include <list>
#include "../libs/utils.h"
#include "../libs/lexicalAnalyser.h"
#include "../libs/syntacticAnalyser.h"
#include "../libs/semanticAnalyser.h"

list<string> createInfixListFromExpression(string basicString);

using namespace std;

int priority(string op) {
    if (op == ">" || op == "<" || op == ">=" || op == "<=" || op == "!=" || op == "==")
        return 1;

    if (op == "+" || op == "-")
        return 2;

    if (op == "*" || op == "div")
        return 3;

    return 0;
}

// string convert(list<string> expressionList) {
//    int i = 0;
//    string postfix = "";
//    stack<string> s;
//
//
//    while (!expressionList.empty()) {
//        if (expressionList.front() >= "a" && expressionList.front() <= "z" ||
//            expressionList.front() >= "A" && expressionList.front() <= "Z" ||
//            expressionList.front() >= "0" && expressionList.front() <= "9") {
//            postfix += expressionList.front();
//            expressionList.pop_front();
//            i++;
//        } else if (expressionList.front() == "(") {
//            s.push(expressionList.front());
//            expressionList.pop_front();
//            i++;
//        } else if (expressionList.front() == ")") {
//            while (s.top() != "(") {
//                postfix += s.top();
//                s.pop();
//            }
//            s.pop();
//            i++;
//        } else {
//            while (!s.empty() && priority(expressionList.front()) <= priority(s.top())) {
//                postfix += s.top();
//                s.pop();
//            }
//            s.push(expressionList.front());
//            expressionList.pop_front();
//        }
//    }
//    while (!s.empty()) {
//        postfix += s.top();
//        s.pop();
//    }
//
//
//    cout << postfix;
//    return postfix;
// }

list<string> createInfixListFromExpression(string infixExpression) {
    int i = 0;
    list<string> resultList;
    string result;
    string aux1, aux2;

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

int main() {
    Node token;
    SymbolNode *aux;
    lineNo = 1;
    FILE *file = openFile((char *) "../testes/sintatico/sint19.txt");
    cout << lineNo << endl;

    character = (char) fgetc(file);

//    do {
//        token = getToken(file);
//
//        if (!token.lexema.empty() && !token.simbolo.empty()) {
//            if (token.simbolo == "sprograma") {
//                token = getToken(file);
//
//                if (token.simbolo == "sidentificador") {
//                    symbolTable.downLayer(token.lexema, token.lexema, token.lexema, "programa", lineNo);
//
//                    token = getToken(file);
//
//                    if (token.simbolo == "sponto_virgula") {
//                        token = analyseBlock(file, token);
//
//                        if (token.simbolo == "sponto") {
//                            cout << "Sucesso" << endl;
//                        } else {
//                            cout << "Erro25" << endl;
//                        }
//                    } else {
//                        cout << "Erro26" << endl;
//                    }
//                } else {
//                    cout << "Erro27" << endl;
//                }
//            } else {
//                if (character != EOF) {
//                    cout << "Erro28" << endl;
//                }
//            }
//
//        } else {
//            cout << "Erro29" << endl;
//        }
//
//
//    } while (character != EOF);
//
    fclose(file);
//    cout << lineNo << endl;
    list<string> expressionList;
    string infix = "a+b*(c-d+e)>(f+g-h)+i";

    expressionList = createInfixListFromExpression(infix);

//    string postfix;
//    postfix = convert(expressionList);

//    symbolTable.printList();
    return 0;
}


//    Léxico
//    LinkedList TokenList;
//    int c = 0;
//    do {
//        token = getToken(file);
//        if (!token.lexema.empty() && !token.simbolo.empty()) {
//            TokenList.insertNode(token.lexema, token.simbolo);
//            c = 0;
//        } else {
//            c++;
//        }
//
//    } while (character != EOF && c != 10000);
//
//    TokenList.printList();
//    if (c == 10000) {
//        cout << "Lexema nao identificado" << endl;
//    }