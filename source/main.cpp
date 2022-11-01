#include <stack>
#include "../libs/utils.h"
#include "../libs/lexicalAnalyser.h"
#include "../libs/syntacticAnalyser.h"
#include "../libs/semanticAnalyser.h"

using namespace std;

int priority(char alpha) {
    if (alpha == '+' || alpha == '-')
        return 1;

    if (alpha == '*' || alpha == '/')
        return 2;

    if (alpha == '^')
        return 3;
    return 0;
}

string convert(string infix) {
    int i = 0;
    string postfix = "";
    stack<int> s;


    while (infix[i] != '\0') {
        if (infix[i] >= 'a' && infix[i] <= 'z' || infix[i] >= 'A' && infix[i] <= 'Z' ||
            infix[i] >= '0' && infix[i] <= '9') {
            postfix += infix[i];
            i++;
        } else if (infix[i] == '(') {
            s.push(infix[i]);
            i++;
        } else if (infix[i] == ')') {
            while (s.top() != '(') {
                postfix += s.top();
                s.pop();
            }
            s.pop();
            i++;
        } else {
            while (!s.empty() && priority(infix[i]) <= priority(s.top())) {
                postfix += s.top();
                s.pop();
            }
            s.push(infix[i]);
            i++;
        }
    }
    while (!s.empty()) {
        postfix += s.top();
        s.pop();
    }


    cout << postfix;
    return postfix;
}

// var+abc
// |var||abc|+
// replace('|', '')
// verificar se variaveis ja foram declaradas
// adicionar precedencia de >, >=, <, <=, !=, ==
// montar conforme recebe a expressâo?

// x > 10 -> x10>

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

    string exp = "a+b*(c^d-e)^(f+g*h)-i";
    string a = toPostfix(exp);
    cout << a << endl;

    string infix = "a+b*(c^d-e)^(f+g*h)-i";
    string postfix;
    postfix = convert(infix);

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