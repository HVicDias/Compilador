#include <string.h>
#include <string>

#include "../libs/utils.h"
#include "../libs/lexical_analyser.h"

typedef struct  Token{
    string lexema;
    string simbolo;
    Token *next;
}Token;

using namespace std;

int main() {
    Token *TokenList = (Token *) malloc(sizeof(Token));
    FILE *file = openFile((char *) "../docs/gera1.txt");
    char character;
    do{
        character = (char) fgetc(file);
        cout << character << endl;
    } while (character !=EOF);

    fclose(file);
    return 0;
}
