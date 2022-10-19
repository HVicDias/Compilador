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

SymbolList::SymbolList() { symbolNode = nullptr; }

void SymbolList::deleteNode(int nodeIndex) {
    SymbolNode *currentNode = symbolNode, *auxList = nullptr;
    int ListLen = 0;

    if (symbolNode == nullptr) {
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

    currentNode = symbolNode;

    if (nodeIndex == 1) {
        symbolNode = symbolNode->next;
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
    auto *newNode = new SymbolNode(std::move(identifier), std::move(scope), std::move(type), lineNo);

    if (symbolNode == nullptr) {
        symbolNode = newNode;
        return;
    }

    SymbolNode *auxNode = symbolNode;
    while (auxNode->next != nullptr) {
        auxNode = auxNode->next;
    }

    // Insert at the last.
    auxNode->next = newNode;
}

void SymbolList::printList() {
    SymbolNode *auxNode = symbolNode;

    // Check for empty list.
    if (symbolNode == nullptr) {
        cout << "List empty" << endl;
        return;
    }

    cout << "Identifier" << "\tScope" << "\ttype" << "\tLineNumber" << endl;
    while (auxNode != nullptr) {
        cout << auxNode->identifier << " : " << auxNode->scope << " : " << auxNode->type << " : " << auxNode->lineNo
             << "\n";
        auxNode = auxNode->next;
    }
    cout << endl;
}

SymbolListNode::SymbolListNode(std::string layerName) : SymbolList() {
    previous = nullptr;
    this->layerName = layerName;
}

SymbolTable::SymbolTable() {
    symbolListNode = nullptr;
}

void SymbolTable::insertSymbol(string identifier, string scope, string type, int lineNo) {
    if (symbolListNode != nullptr) {
        symbolListNode->insertNode(identifier, scope, type, lineNo);
    } else {
        cout << "SymbolTable Head is null" << endl;
        exit(1);
    }
}

void SymbolTable::downLayer(std::string layerName) {
    SymbolListNode *newNode = new SymbolListNode(layerName);
    newNode->previous = symbolListNode;
    symbolListNode = newNode;
}

SymbolNode* SymbolTable::searchSymbol(std::string lexema) {
    SymbolListNode *currentNode = symbolListNode;

    if (currentNode == nullptr) {
        cout << "SymbolTable Head is null" << endl;
        exit(1);
    }

    SymbolNode* currentSymbol = symbolListNode->symbolNode;

    if (currentSymbol == nullptr) {
        cout << "SymbolTable have no node is null" << endl;
        exit(1);
    }

    while(currentNode != nullptr) {
        while (currentSymbol != nullptr){
            if(currentSymbol->identifier == lexema){
                return currentSymbol;
            }
            currentSymbol = currentSymbol->next;
        }
        currentNode = currentNode->previous;
        currentSymbol = currentNode->symbolNode;
    }

    return nullptr;
}

void SymbolTable::printList() {
    SymbolListNode *auxNode = symbolListNode;
    int leafs = 0;

    // Check for empty list.
    if (symbolListNode == nullptr) {
        cout << "Empty Tree" << endl;
        return;
    }

    cout << "Head " << auxNode->layerName << endl;
    while (auxNode != nullptr) {
        auxNode->printList();
        auxNode = auxNode->previous;
        cout << "Head -" << leafs << " " << auxNode->layerName << endl;
        leafs++;
    }
    cout << endl;
}

void SymbolTable::deleteLayer() {
    SymbolListNode *currentNode = symbolListNode;

    if (currentNode == nullptr) {
        cout << "List empty." << endl;
        return;
    }

    symbolListNode = symbolListNode->previous;
    delete currentNode;
}
