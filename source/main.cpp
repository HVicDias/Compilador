#include "../gui/compilerGUI.h"


int main(int argc, char *argv[]) {
    compilerGUI(argc, argv);

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