#include <string>
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

/*
// Inserting nodes
list.insertNode("1", "2");
list.insertNode("3", "4");
list.insertNode("5", "6");
list.insertNode("7", "8");

cout << "Elements of the list are: ";

// Print the list
list.printList();
cout << endl;

// Delete node at position 2.
list.deleteNode(2);

cout << "Elements of the list are: " << endl;
list.printList();
return 0;
 */
