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
             << endl;
        auxNode = auxNode->next;
    }
}

SymbolListNode::SymbolListNode(std::string layerName, string identifier, string scope, string type, int lineNo)
        : SymbolList() {
    previous = nullptr;
    this->layerName = layerName;
    symbolNode = new SymbolNode(identifier, scope, type, lineNo);
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

void SymbolTable::downLayer(std::string layerName, string identifier, string scope, string type, int lineNo) {
    SymbolListNode *newNode = new SymbolListNode(layerName, identifier, scope, type, lineNo);
    newNode->previous = symbolListNode;
    symbolListNode = newNode;
}

SymbolNode *SymbolTable::searchSymbol(std::string lexema) {
    SymbolListNode *currentNode = symbolListNode;
    SymbolNode *currentSymbol;

    if (currentNode == nullptr) {
        cout << "SymbolTable Head is null" << endl;
        exit(1);
    }

    while (currentNode != nullptr) {
        currentSymbol = currentNode->symbolNode;
        currentNode->printList();

        while (currentSymbol != 0) {
            if (currentSymbol->identifier == lexema) {
                return currentSymbol;
            }
            currentSymbol = currentSymbol->next;
        }

        currentNode = currentNode->previous;
    }

    return nullptr;
}

SymbolNode *SymbolTable::searchScopeSymbol(std::string lexema, std::string scope) {
    SymbolListNode *currentNode = symbolListNode;
    SymbolNode *currentSymbol;

    if (currentNode == nullptr) {
        cout << "SymbolTable Head is null" << endl;
        exit(1);
    }

    while (currentNode != nullptr) {
        currentSymbol = currentNode->symbolNode;

        if (currentNode->layerName == scope) {
            while (currentSymbol != 0) {
                if (currentSymbol->identifier == lexema) {
                    return currentSymbol;
                }
                currentSymbol = currentSymbol->next;
            }
            return nullptr;
        }

        currentNode = currentNode->previous;
    }

    return nullptr;
}

SymbolNode *SymbolTable::searchLocalSymbol(std::string lexema) {
    SymbolListNode *currentNode = symbolListNode;
    SymbolNode *currentSymbol;

    if (currentNode == nullptr) {
        cout << "SymbolTable Head is null" << endl;
        exit(1);
    }

    currentSymbol = currentNode->symbolNode;


    while (currentSymbol != 0) {
        if (currentSymbol->identifier == lexema) {
            return currentSymbol;
        }
        currentSymbol = currentSymbol->next;
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
    cout << "Head Info " << auxNode->symbolNode->identifier << " : " << auxNode->symbolNode->scope << " : "
         << auxNode->symbolNode->type << " : " << auxNode->symbolNode->lineNo << endl;
    while (auxNode != nullptr) {
        auxNode->printList();
        cout << "Head Info " << auxNode->symbolNode->identifier << " : " << auxNode->symbolNode->scope << " : "
             << auxNode->symbolNode->type << " : " << auxNode->symbolNode->lineNo << endl;
        auxNode = auxNode->previous;
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
