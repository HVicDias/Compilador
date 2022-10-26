#include "../libs/utils.h"
#include "../libs/lexicalAnalyser.h"
#include "../libs/syntacticAnalyser.h"
#include "../libs/semanticAnalyser.h"

using namespace std;

int main() {
    Node token;
    SymbolNode *aux;
    lineNo = 1;
    FILE *file = openFile((char *) "../testes/sintatico/sint19.txt");
    cout << lineNo << endl;

    character = (char) fgetc(file);

    do {
        token = getToken(file);

        if (!token.lexema.empty() && !token.simbolo.empty()) {
            if (token.simbolo == "sprograma") {
                token = getToken(file);

                if (token.simbolo == "sidentificador") {
                    symbolTable.downLayer(token.lexema, token.lexema, token.lexema, "programa", lineNo);

                    token = getToken(file);

                    if (token.simbolo == "sponto_virgula") {
                        token = analyseBlock(file, token);

                        if (token.simbolo == "sponto") {
                            cout << "Sucesso" << endl;
                        } else {
                            cout << "Erro25" << endl;
                        }
                    } else {
                        cout << "Erro26" << endl;
                    }
                } else {
                    cout << "Erro27" << endl;
                }
            } else {
                if (character != EOF) {
                    cout << "Erro28" << endl;
                }
            }

        } else {
            cout << "Erro29" << endl;
        }


    } while (character != EOF);

    fclose(file);
    cout << lineNo << endl;

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