#include <string>
#include <iostream>
#include "linkedList.h"

using namespace std;

Node::Node() {
    lexema = "";
    simbolo = "";
    next = NULL;
}

// Parameterised Constructor
Node::Node(string lexema, string simbolo) {
    this->lexema = lexema;
    this->simbolo = simbolo;
    this->next = NULL;
}

LinkedList::LinkedList() { head = NULL; }

void LinkedList::deleteNode(int nodeIndex) {
    Node *currentNode = head, *auxList = NULL;
    int ListLen = 0;

    if (head == NULL) {
        cout << "List empty." << endl;
        return;
    }

    while (currentNode != NULL) {
        currentNode = currentNode->next;
        ListLen++;
    }

    if (ListLen < nodeIndex) {
        cout << "Index out of range" << endl;
        return;
    }

    currentNode = head;

    if (nodeIndex == 1) {
        head = head->next;
        delete currentNode;
        return;
    }

    while (nodeIndex-- > 1) {
        auxList = currentNode;
        currentNode = currentNode->next;
    }

    auxList->next = currentNode->next;

    delete currentNode;
}

void LinkedList::insertNode(string lexema, string simbolo) {
    Node *newNode = new Node(lexema, simbolo);

    if (head == NULL) {
        head = newNode;
        return;
    }

    Node *auxNode = head;
    while (auxNode->next != NULL) {
        auxNode = auxNode->next;
    }

    // Insert at the last.
    auxNode->next = newNode;
}

void LinkedList::printList() {
    Node *auxNode = head;

    // Check for empty list.
    if (head == NULL) {
        cout << "List empty" << endl;
        return;
    }

    while (auxNode != NULL) {
        cout << auxNode->simbolo << " : " << auxNode->lexema << "\n";
        auxNode = auxNode->next;
    }
    cout << endl;
}
