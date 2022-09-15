#include "../libs/utils.h"
#include "../libs/linkedList.h"
#include "../libs/lexicalAnalyser.h"

using namespace std;

int main() {
    LinkedList TokenList;
    Node token;
    FILE *file = openFile((char *) "../Lexico/teste_8.txt");
    int c = 0;
    character = (char) fgetc(file);
    do {
        token = getToken(file);
        if (!token.lexema.empty() && !token.simbolo.empty()) {
            TokenList.insertNode(token.lexema, token.simbolo);
            c = 0;
        } else {
            c++;
        }

    } while (character != EOF && c != 10000);

    fclose(file);

    TokenList.printList();
    if (c == 10000) {
        cout << "Lexema nao identificado" << endl;
    }
    return 0;
}
