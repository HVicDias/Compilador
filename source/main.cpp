#include <string.h>
#include <string>

#include "../libs/utils.h"

using namespace std;

typedef struct Token {
    string lexema;
    string simbolo;
    struct Token *next;
} Token;

int main() {
    Token *TokenList = (Token *) malloc(sizeof(Token));

    FILE *f = openFile((char *) "../docs/gera1.txt");
    cout << (char) fgetc(f) << endl;
    cout << "Teste" << endl;
    return 0;
}
