#include "../libs/utils.h"
#include "../libs/linkedList.h"
#include "../libs/lexicalAnalyser.h"
#include "../libs/syntacticAnalyser.h"

using namespace std;

int main() {
    LinkedList TokenList;
    Node token;

    FILE *file = openFile((char *) "../Sintatico/sint1.txt");

    int c = 0;

    character = (char) fgetc(file);
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

    do {
        token = getToken(file);
        cout << (int) token.simbolo[0] << endl;
        cout << token.simbolo << endl;
        cout << token.lexema << endl;
        if (!token.lexema.empty() && !token.simbolo.empty()) {
            if (token.simbolo == "sprograma") {
                token = getToken(file);

                if (token.simbolo == "sidentificador") {
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
                cout << "Erro28" << endl;
            }
        } else {
            cout << "Erro29" << endl;
        }

    } while (character != EOF);

    fclose(file);

    TokenList.printList();
    if (c == 10000) {
        cout << "Lexema nao identificado" << endl;
    }
    return 0;
}


//Algoritmo Analisador Sintático <programa>
//início
//Léxico(token)
//se token.simbolo = sprograma

//então início
//Léxico(token)
//se token.simbolo = sidentificador
//então início
//Léxico(token)
//se token.simbolo = spontovirgula
//então início
//analisa_bloco
//        se token.simbolo = sponto
//então se acabou arquivo ou é comentário
//        então sucesso
//        senão ERRO
//            senão ERRO
//   fim
//senão ERRO
//fim
//        senão ERRO
//        fim
//senão ERRO
//fim.
