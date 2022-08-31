#include <string>
#include <iostream>
#include <utility>
#include "linkedList.h"

using namespace std;

Node::Node() {
    lexema = "";
    simbolo = "";
    next = nullptr;
}

// Parameterised Constructor
Node::Node(string lexema, string simbolo) {
    this->lexema = std::move(lexema);
    this->simbolo = std::move(simbolo);
    this->next = nullptr;
}

LinkedList::LinkedList() { head = nullptr; }

void LinkedList::deleteNode(int nodeIndex) {
    Node *currentNode = head, *auxList = nullptr;
    int ListLen = 0;

    if (head == nullptr) {
        cout << "List empty." << endl;
        return;
    }

    while (currentNode != nullptr) {
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
    Node *newNode = new Node(std::move(lexema), std::move(simbolo));

    if (head == nullptr) {
        head = newNode;
        return;
    }

    Node *auxNode = head;
    while (auxNode->next != nullptr) {
        auxNode = auxNode->next;
    }

    // Insert at the last.
    auxNode->next = newNode;
}

void LinkedList::printList() {
    Node *auxNode = head;

    // Check for empty list.
    if (head == nullptr) {
        cout << "List empty" << endl;
        return;
    }

    cout << "Lexema" << "\tSimbolo" << endl;
    while (auxNode != nullptr) {
        cout << auxNode->lexema << " : " << auxNode->simbolo << "\n";
        auxNode = auxNode->next;
    }
    cout << endl;
}
