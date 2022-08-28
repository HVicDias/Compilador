#include <string.h>
#include <string>

#include "utils.cpp"

using namespace std;

typedef struct Token {
    string lexema;
    string simbolo;
    struct Token *next;
} Token;

int main() {
    Token *TokenList = (Token *) malloc(sizeof(Token));

    FILE *f = openFile((char *) "gera1.txt");
    cout << fgetc(f) << endl;

    return 0;
}
