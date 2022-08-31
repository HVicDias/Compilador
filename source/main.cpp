#include "../libs/utils.h"
#include "../libs/linkedList.h"
#include "../libs/lexical_analyser.h"

using namespace std;

int main() {
    LinkedList TokenList;
    Node token;
    FILE *file = openFile((char *) "../docs/gera1.txt");

    character = (char) fgetc(file);
    do {
        token = getToken(file);
        if (!token.lexema.empty() && !token.simbolo.empty())
            TokenList.insertNode(token.lexema, token.simbolo);
    } while (character != EOF);

    fclose(file);

    TokenList.printList();

    return 0;
}
