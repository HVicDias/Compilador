#include "../libs/utils.h"
#include "../libs/lexicalAnalyser.h"
#include "../libs/syntacticAnalyser.h"
#include "../libs/semanticAnalyser.h"
#include "../libs/codeGenerator.h"
#include "../gui/compilerGUI.h"

using namespace std;

int main(int argc, char *argv[]) {
    Node token;
    lineNo = 1;
    FILE *file = openFile((char *) "../testes/sintatico/sint19.txt");
    cout << lineNo << endl;
    compilerGUI(argc, argv);
    character = (char) fgetc(file);

    CodeGenerator codeGen;
    auto *snippet = new CodeSnippet(10, "SUM", 2, 3);
    codeGen.insertNode(snippet);
    codeGen.printList();
    codeGen.generateCode();

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