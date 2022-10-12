#include "../libs/utils.h"
#include "../libs/linkedList.h"
#include "../libs/lexicalAnalyser.h"
#include "../libs/syntacticAnalyser.h"
#include "../libs/semanticAnalyser.h"
#include "../libs/symbolTable.h"

using namespace std;

int main() {
    LinkedList TokenList;
    Node token;
    lineNumber = 0;
    FILE *file = openFile((char *) "../Sintatico/sint19.txt");
    cout << lineNumber;
//    int c = 0;

    character = (char) fgetc(file);
//    Léxico
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

    do {
        token = getToken(file);

        if (!token.lexema.empty() && !token.simbolo.empty()) {
            if (token.simbolo == "sprograma") {
                token = getToken(file);

                if (token.simbolo == "sidentificador") {
                    symbolTable.insertNode(token.lexema, "nomedeprograma", "", "");
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

    return 0;
}
