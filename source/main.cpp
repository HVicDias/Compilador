#include <string>
#include <iostream>
#include "../libs/utils.h"
#include "../libs/linkedList.h"
#include "../libs/lexical_analyser.h"

using namespace std;

int main() {
    LinkedList list;
    FILE *file = openFile((char *) "../docs/gera1.txt");
    char character;
    do{
        character = (char) fgetc(file);
        cout << character << endl;
    } while (character != EOF);

    fclose(file);
    return 0;

    // Inserting nodes
    list.insertNode("1","2");
    list.insertNode("3","4");
    list.insertNode("5","6");
    list.insertNode("7","8");

    cout << "Elements of the list are: ";

    // Print the list
    list.printList();
    cout << endl;

    // Delete node at position 2.
    list.deleteNode(2);

    cout << "Elements of the list are: " << endl;
    list.printList();
    return 0;
}