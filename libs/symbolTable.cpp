#include <string>
#include <iostream>
#include <utility>
#include "invertedTree.h"

using namespace std;

Node::Node() {
    lexema = "";
    simbolo = "";
    previous = nullptr;
}

// Parameterised Constructor
Node::Node(string lexema, string simbolo) {
    this->lexema = std::move(lexema);
    this->simbolo = std::move(simbolo);
    this->previous = nullptr;
}

LinkedList::LinkedList()
{
    head = nullptr;
}

void LinkedList::deleteNode(int nodeIndex) {
    Node *currentNode = head;

    if (head == nullptr) {
        cout << "List empty." << endl;
        return;
    }

    head = head->previous;
    delete currentNode;
    return;
}

void LinkedList::insertNode(string lexema, string simbolo) {
    Node *newNode = new Node(std::move(lexema), std::move(simbolo));
    newNode->previous = head;
    head = newNode;
}

void LinkedList::printList() {
    Node *auxNode = head;

    // Check for empty list.
    if (head == nullptr) {
        cout << "Empty Tree" << endl;
        return;
    }

    cout << "Lexema" << "\tSimbolo" << endl;
    while (auxNode != nullptr) {
        cout << auxNode->lexema << " : " << auxNode->simbolo << "\n";
        auxNode = auxNode->previous;
    }
    cout << endl;
}
