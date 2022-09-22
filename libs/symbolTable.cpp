#include <string>
#include <iostream>
#include <utility>
#include "symbolTable.h"

using namespace std;

SymbolNode::SymbolNode() {
    identifier = "";
    scope = "";
    type = "";
    lineNo = -1;
    next = nullptr;
}

// Parameterised Constructor
SymbolNode::SymbolNode(string identifier, string scope, string type, int lineNo) {
    this->identifier = std::move(identifier);
    this->scope = std::move(scope);
    this->type = std::move(type);
    this->lineNo = lineNo;
    this->next = nullptr;
}

SymbolList::SymbolList() { head = nullptr; }

void SymbolList::deleteNode(int nodeIndex) {
    SymbolNode *currentNode = head, *auxList = nullptr;
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

void SymbolList::insertNode(string identifier, string scope, string type, int lineNo) {
    SymbolNode *newNode = new SymbolNode(std::move(identifier), std::move(scope), std::move(type), lineNo);

    if (head == nullptr) {
        head = newNode;
        return;
    }

    SymbolNode *auxNode = head;
    while (auxNode->next != nullptr) {
        auxNode = auxNode->next;
    }

    // Insert at the last.
    auxNode->next = newNode;
}

void SymbolList::printList() {
    SymbolNode *auxNode = head;

    // Check for empty list.
    if (head == nullptr) {
        cout << "List empty" << endl;
        return;
    }

    cout << "Identifier" << "\tScope" << "\ttype" << "\tLineNumber" <<  endl;
    while (auxNode != nullptr) {
        cout << auxNode->identifier << " : "<< auxNode->scope << " : " << auxNode->type << " : " << auxNode->lineNo << "\n";
        auxNode = auxNode->next;
    }
    cout << endl;
}

SymbolListNode::SymbolListNode() : SymbolList(){
    previous = nullptr;
}

SymbolTable::SymbolTable()
{
    head = nullptr;
}

void SymbolTable::deleteNode() {
    SymbolListNode *currentNode = head;

    if (head == nullptr) {
        cout << "List empty." << endl;
        return;
    }

    head = head->previous;
    delete currentNode;
    return;
}

void SymbolTable::insertNode(string lexema, string simbolo) {
    SymbolListNode *newNode = new SymbolListNode();
    newNode->previous = head;
    head = newNode;
}

void SymbolTable::printList() {
    SymbolListNode *auxNode = head;
    int leafs = 0;

    // Check for empty list.
    if (head == nullptr) {
        cout << "Empty Tree" << endl;
        return;
    }

    cout << "Head" << endl;
    while (auxNode != nullptr) {
        auxNode->printList();
        auxNode = auxNode->previous;
        cout << "Head -" << leafs << endl;
        leafs++;
    }
    cout << endl;
}
